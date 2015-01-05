#include "form_request.h"
//----------------------------------------------------------------------------------------------

FormRequest::FormRequest (QWidget* parent) : FormRequestUI (parent), IProgress ()
{
	connect(m_button_cancel, SIGNAL(clicked()), this, SLOT(reject()));
}
//----------------------------------------------------------------------------------------------

FormRequest::~FormRequest ()
{
}
//----------------------------------------------------------------------------------------------

FormRequest::FormRequest (QWidget* parent, const QNetworkRequest& request, const QString& data) : FormRequestUI (parent), IProgress ()
{
	connect(m_button_cancel, SIGNAL(clicked()), this, SLOT(reject()));

	m_http.setProxy(defaultProxy());

	QNetworkReply* reply = NULL;
	if (data.length() == 0)
		reply = m_http.get(request);
	else
		reply = m_http.post(request, data.toUtf8());

	connect(&m_http, SIGNAL(finished(QNetworkReply*)), this, SLOT(process_finished(QNetworkReply*)));

	connect(reply, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(process_ssl_errors(const QList<QSslError>&)));
	connect(reply, SIGNAL(downloadProgress(qint64, qint64)),   this, SLOT(process_download_progress(qint64, qint64)));
	connect(reply, SIGNAL(uploadProgress(qint64, qint64)),     this, SLOT(process_upload_progress(qint64, qint64)));
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
	return m_header;
}
//----------------------------------------------------------------------------------------------

QString FormRequest::getResponse (bool &error)
{
	error = m_error.length() > 0 ? true : false;
	return (error == true ? m_error : m_body);
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

void FormRequest::process_finished (QNetworkReply* reply)
{
	reply->deleteLater();

	m_error = "";
	if (reply->error() != QNetworkReply::NoError)
	{
		m_error = reply->errorString();
		new QListWidgetItem(m_error, m_list_progress);
		return;
	}

	m_header = "";
	const QList<QNetworkReply::RawHeaderPair> pairs = reply->rawHeaderPairs();
	for (int i = 0; i < pairs.count(); i++)
	{
		QNetworkReply::RawHeaderPair pair = pairs[i];
		QString     header = QString::fromUtf8(pair.first.constData());
		QStringList values = QString::fromUtf8(pair.second.constData()).split("\n");
		for (int j = 0; j < values.count(); j++)
			m_header += header + ": " + values[j] + "\r\n";
	}

#ifndef AVALON_USE_ZLIB
	m_body = QString::fromUtf8(reply->readAll());
#else
	if (reply->hasRawHeader("Content-Encoding") == true)
	{
		QString content_encoding = QString::fromUtf8(reply->rawHeader("Content-Encoding")).toLower();
		if (content_encoding != "gzip")
		{
			m_body = QString::fromUtf8(reply->readAll());
			accept();
			return;
		}

		// ответ пришел закодированным в gzip
		QByteArray array = reply->readAll();

		QTemporaryFile tmp_file;
		if (tmp_file.open() == false)
		{
			m_error = tmp_file.errorString();
			return;
		}
		if (tmp_file.write(array) == -1)
		{
			m_error = tmp_file.errorString();
			return;
		}

		QString tmp_file_name = tmp_file.fileName();
		tmp_file.close();
		array.clear();

		gzFile zip_file = gzopen(tmp_file_name.toUtf8().constData(), "rb");
		if (zip_file == NULL)
		{
			// TODO: if errno is zero, the zlib error is Z_MEM_ERROR
			m_error = QString::fromUtf8("Ошибка открытия файла: ") + tmp_file_name;
			return;
		}

		char buffer[1024];
		while (true)
		{
			int read = gzread(zip_file, buffer, 1024);
			if (read == -1)
			{
				// TODO: gzerror(gzFile file, int *errnum);
				gzclose(zip_file);
				m_error = QString::fromUtf8("Ошибка чтения файла: ") + tmp_file_name;
				return;
			}
			else if (read == 0)
				break;

			array.append(QByteArray(buffer, read));
		}

		gzclose(zip_file);

		m_body = QString::fromUtf8(array.constData());
	}
	else
		m_body = QString::fromUtf8(reply->readAll());
#endif   // AVALON_USE_ZLIB

	accept();
}
//----------------------------------------------------------------------------------------------

void FormRequest::process_ssl_errors (const QList<QSslError> &errors)
{
	for (int i = 0; i < errors.count(); i++)
		new QListWidgetItem(errors[i].errorString(), m_list_progress);
}
//----------------------------------------------------------------------------------------------

void FormRequest::process_download_progress (qint64 done, qint64 total)
{
	// при включенном сжатии, размер данных неопределен
	if (total != 0)
		setWindowTitle(m_proto + QString::fromUtf8(" - чтение ") + formatPrettyBytes(done) + "/" + formatPrettyBytes(total));
	else
		setWindowTitle(m_proto + QString::fromUtf8(" - чтение ") + formatPrettyBytes(done));

	m_progress_bar->setMaximum(total);
	m_progress_bar->setValue(done);
}
//----------------------------------------------------------------------------------------------

void FormRequest::process_upload_progress (qint64 done, qint64 total)
{
	setWindowTitle(m_proto + QString::fromUtf8(" - отправка ") + formatPrettyBytes(done) + "/" + formatPrettyBytes(total));

	m_progress_bar->setMaximum(total);
	m_progress_bar->setValue(done);
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
