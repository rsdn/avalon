#include "form_request.h"
//----------------------------------------------------------------------------------------------

FormRequest::FormRequest (QWidget* parent) : FormRequestUI (parent), IProgress ()
{
	m_hack    = 0;
	m_to_send = 0;

	connect(m_button_cancel, SIGNAL(clicked()), this, SLOT(reject()));
}
//----------------------------------------------------------------------------------------------

FormRequest::~FormRequest ()
{
}
//----------------------------------------------------------------------------------------------

FormRequest::FormRequest (QWidget* parent, const QString& host, quint16 port, const QString& header, const QString& data) : FormRequestUI (parent), IProgress ()
{
	bool https = (port == 443 ? true : false);

	m_hack = 0;

	QNetworkProxy proxy = defaultProxy();

	QNetworkProxy::ProxyType proxy_type = proxy.type();

	if (proxy_type == QNetworkProxy::HttpCachingProxy)
		m_http.setProxy(proxy);
	else if (proxy_type == QNetworkProxy::Socks5Proxy || proxy_type == QNetworkProxy::HttpProxy)
	{
		// transparent proxy, QHttp bug workaround
		if (https == false)
		{
			QTcpSocket* socket = new QTcpSocket(this);
			socket->setProxy(proxy);
			m_http.setSocket(socket);
		}
		else
		{
			QSslSocket* socket = new QSslSocket(this);
			socket->setProxy(proxy);
			m_http.setSocket(socket);
		}
	}

	m_http.setHost(host, (https == false ? QHttp::ConnectionModeHttp : QHttp::ConnectionModeHttps), port);

	QHttpRequestHeader request_header(header);
	QByteArray         request_data   = data.toUtf8();

	m_to_send = header.length() + request_data.size() + 2 /*\r\n*/;

	connect(&m_http, SIGNAL(stateChanged(int)),          this, SLOT(process_state_changed(int)));
	connect(&m_http, SIGNAL(dataReadProgress(int, int)), this, SLOT(process_data_read_progress(int, int)));
	connect(&m_http, SIGNAL(dataSendProgress(int, int)), this, SLOT(process_data_send_progress(int, int)));
	connect(&m_http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader&)), this, SLOT(process_response_header_received(const QHttpResponseHeader&)));
	connect(&m_http, SIGNAL(requestStarted(int)),        this, SLOT(process_request_started(int)));
	connect(&m_http, SIGNAL(requestFinished(int, bool)), this, SLOT(process_request_finished(int, bool)));

	if (https == true)
		connect(&m_http, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(process_ssl_errors(const QList<QSslError>&)));

	m_http.request(request_header, request_data);

	connect(m_button_cancel, SIGNAL(clicked()), this, SLOT(reject()));
}
//----------------------------------------------------------------------------------------------

QNetworkProxy FormRequest::defaultProxy (bool webkit)
{
	QSettings settings;

	QNetworkProxy proxy;

	if (settings.value("proxy/enabled", false).toInt() != false)
	{
		if (webkit == true && settings.value("proxy/enabled_webkit", true).toInt() == false)
			return proxy;

		QString proxy_type = settings.value("proxy/type", "HTTP").toString();

		if (proxy_type == "HTTP")
			proxy.setType(QNetworkProxy::HttpCachingProxy);
		else if (proxy_type == "SOCKS 5")
			proxy.setType(QNetworkProxy::Socks5Proxy);
		else if (proxy_type == "CONNECT")
			proxy.setType(QNetworkProxy::HttpProxy);
		else
			proxy.setType(QNetworkProxy::DefaultProxy);

		if (proxy.type() != QNetworkProxy::DefaultProxy)
		{
			proxy.setHostName (settings.value("proxy/host",     "").toString());
			proxy.setPort     (settings.value("proxy/port",     0 ).toInt());
			proxy.setUser     (settings.value("proxy/login",    "").toString());
			proxy.setPassword (settings.value("proxy/password", "").toString());
		}
	}

	return proxy;
}
//----------------------------------------------------------------------------------------------

QString FormRequest::getResponseHeader ()
{
	return m_http.lastResponse().toString();
}
//----------------------------------------------------------------------------------------------

QString FormRequest::getResponse (bool &error)
{
	error = false;

	QByteArray array = m_http.readAll();

#ifndef AVALON_USE_ZLIB
	return QString::fromUtf8(array.constData());
#else

	// проверка на то, что ответ вернулся закодированным
	QHttpResponseHeader header = m_http.lastResponse();

	// надеюсь, что функция hasKey() нечувствительна к регистру и опредилит поле правильно
	if (header.hasKey("Content-Encoding") != true)
		return QString::fromUtf8(array.constData());

	// получение типа кодирования (может не совпадать регистр)
	QString content_encoding = header.value("Content-Encoding").toLower();

	// проверка на тип кодирования
	if (content_encoding != "gzip" /*&& content_encoding != "deflate"*/)
		return QString::fromUtf8(array.constData());

	//
	// ответ пришел закодированным в gzip
	//

	QTemporaryFile tmp_file;

	if (tmp_file.open() == false)
	{
		error = true;
		return tmp_file.errorString();
	}

	if (tmp_file.write(array) == -1)
	{
		error = true;
		return tmp_file.errorString();
	}

	QString tmp_file_name = tmp_file.fileName();

	tmp_file.close();

	array.clear();

	gzFile zip_file = gzopen(tmp_file_name.toUtf8().constData(), "rb");

	if (zip_file == NULL)
	{
		error = true;

		// TODO: if errno is zero, the zlib error is Z_MEM_ERROR

		return QString::fromUtf8("Ошибка открытия файла: ") + tmp_file_name;
	}

	char buffer[1024];

	while (true)
	{
		int read = gzread(zip_file, buffer, 1024);

		if (read == -1)
		{
			gzclose(zip_file);

			error = true;

			// TODO: gzerror(gzFile file, int *errnum);

			return QString::fromUtf8("Ошибка чтения файла: ") + tmp_file_name;
		}
		else if (read == 0)
			break;

		array.append(QByteArray(buffer, read));
	}

	gzclose(zip_file);

	return QString::fromUtf8(array.constData());

#endif   // AVALON_USE_ZLIB
}
//----------------------------------------------------------------------------------------------

QString FormRequest::formatPrettyBytes (qint64 size)
{
	char buf[64];

        if (size < (qint64)1024)
        {
                sprintf(buf, "%0d bytes", (int)size);
                return QString::fromUtf8(buf);
        }
        else if (size < (qint64)1024 * 1024)
        {
                sprintf(buf, "%0.2f KB", (double)size / 1024);
                return QString::fromUtf8(buf);
        }
        else if (size < (qint64)1024 * 1024 * 1024)
        {
                sprintf(buf, "%0.2f MB", (double)size / 1024 / 1024);
                return QString::fromUtf8(buf);
        }
        else
        {
                sprintf(buf, "%0.2f GB", (double)size / 1024 / 1024 / 1024);
                return QString::fromUtf8(buf);
        }
}
//----------------------------------------------------------------------------------------------

void FormRequest::process_data_read_progress (int done, int total)
{
	// при включенном сжатии, размер данных неопределен
	if (total != 0)
		setWindowTitle(QString::fromUtf8("HTTP - чтение ") + formatPrettyBytes(done) + "/" + formatPrettyBytes(total));
	else
		setWindowTitle(QString::fromUtf8("HTTP - чтение ") + formatPrettyBytes(done));

	m_progress_bar->setMaximum(total);
	m_progress_bar->setValue(done);
}
//----------------------------------------------------------------------------------------------

void FormRequest::process_data_send_progress (int done, int total)
{
	setWindowTitle(QString::fromUtf8("HTTP - отправка ") + formatPrettyBytes(done) + "/" + formatPrettyBytes(total));

	m_progress_bar->setMaximum(total);
	m_progress_bar->setValue(done);
}
//----------------------------------------------------------------------------------------------

void FormRequest::process_response_header_received (const QHttpResponseHeader& /*resp*/)
{
	new QListWidgetItem(QString::fromUtf8("Получен заголовок ответа"), m_list_progress);
}
//----------------------------------------------------------------------------------------------

void FormRequest::process_request_started (int /*id*/)
{
	if (m_hack > 2)
		new QListWidgetItem(QString::fromUtf8("Запрос отправлен"), m_list_progress);
	else
		m_hack++;
}
//----------------------------------------------------------------------------------------------

void FormRequest::process_request_finished (int /*id*/, bool error)
{
	if (m_hack > 1 || error == true)
	{
		if (error == true)
			new QListWidgetItem(QString::fromUtf8("Ошибка запроса - ") + m_http.errorString(), m_list_progress);
		else
		{
			int status_code = m_http.lastResponse().statusCode();

			if (status_code == 200)
			{
				new QListWidgetItem(QString::fromUtf8("Запрос завершен"), m_list_progress);

				accept();
			}
			else if (status_code > 100 && status_code < 511 /* при соединении с SOCKS прокси могу появляться артефакты с кодом состояния якобоы более 5xx */)
				new QListWidgetItem(QString::fromUtf8("Ошибка запроса - ") + QString::number(m_http.lastResponse().statusCode()) + " - " + m_http.lastResponse().reasonPhrase(), m_list_progress);
		}
	}
	else
		m_hack++;
}
//----------------------------------------------------------------------------------------------

void FormRequest::process_state_changed (int state)
{
	switch (state)
	{
		case QHttp::Unconnected:

			setWindowTitle("HTTP");

			new QListWidgetItem(QString::fromUtf8("Нет соединения"), m_list_progress);

			break;

		case QHttp::HostLookup:

			setWindowTitle("HTTP");

			new QListWidgetItem(QString::fromUtf8("Поиск хоста"), m_list_progress);

			break;

		case QHttp::Connecting:

			setWindowTitle("HTTP");

			new QListWidgetItem(QString::fromUtf8("Соединение"), m_list_progress);

			break;

		case QHttp::Sending:

			setWindowTitle("HTTP");

			new QListWidgetItem(QString::fromUtf8("Отправка ") + formatPrettyBytes(m_to_send), m_list_progress);

			break;

		case QHttp::Reading:

			setWindowTitle("HTTP");

			new QListWidgetItem(QString::fromUtf8("Чтение"), m_list_progress);

			break;

		case QHttp::Connected:

			setWindowTitle("HTTP");

			new QListWidgetItem(QString::fromUtf8("Соединение установлено"), m_list_progress);

			break;

		case QHttp::Closing:

			setWindowTitle("HTTP");

			new QListWidgetItem((QString)QString::fromUtf8("Чтение ") + formatPrettyBytes(m_http.lastResponse().toString().length() + m_http.bytesAvailable()), m_list_progress);
			new QListWidgetItem(QString::fromUtf8("Закрытие соединения"), m_list_progress);

			break;
	}
}
//----------------------------------------------------------------------------------------------

void FormRequest::onProgress (int percent)
{
	m_progress_bar->setValue(percent);

	QCoreApplication::processEvents();
}
//----------------------------------------------------------------------------------------------

void FormRequest::onProgress (int percent, const QString& status)
{
	if (status.length() > 0)
		new QListWidgetItem(status, m_list_progress);

	m_progress_bar->setValue(percent);

	QCoreApplication::processEvents();
}
//----------------------------------------------------------------------------------------------

void FormRequest::process_ssl_errors (const QList<QSslError> &errors)
{
	for (int i = 0; i < errors.count(); i++)
		new QListWidgetItem(QString::fromUtf8("Ошибка запроса - ") + errors[i].errorString(), m_list_progress);
}
//----------------------------------------------------------------------------------------------
