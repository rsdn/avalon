/*!
 * \file
 * \brief Форма для отображения прогресса работы и для HTTP запросов
 */

#ifndef _avalon_form_request_h_
#define _avalon_form_request_h_

#include "iprogress.h"
#include "form_request_ui.h"

/*!
 * \brief Форма для отображения прогресса работы и для HTTP запросов
 */
class FormRequest :
	public FormRequestUI,
	public IProgress
{
	Q_OBJECT

	public:

		/*!
		 * \brief Конструктор для простого прогресса
		 * \param parent Родительский виджет (форма)
		 */
		FormRequest  (QWidget* parent);

		/*!
		 * \brief Конструктор для HTTP запроса с прогрессом
		 * \param parent Родительский виджет (форма)
		 * \param host Адрес хоста
		 * \param port Номер порта
		 * \param header Заголовок HTTP
		 * \param data POST данные для HTTP
		 */
		FormRequest  (QWidget* parent, const QString& host, quint16 port, const QString& header, const QString& data);

		~FormRequest ();

		/*!
		 * \brief Получение HTTP ответа
		 * \param error - флаг ошибки если тело ответа не может быть раскодированно - если true, то результат есть описание ошибки
		 * \return Тело HTTP ответа
		 */
		QString getResponse (bool &error);

		/*!
		 * \brief Получение заголовка HTTP ответа
		 * \return Тело заголовка HTTP ответа
		 */
		QString getResponseHeader ();

		/*!
		 * \brief Возвращает прокси-сервер по умолчанию
		 * \param webkit Флаг запроса прокси для области отображения сообщений
		 * \return Описатель прокси-сервера
		 */
		static QNetworkProxy defaultProxy (bool webkit = false);

	private:

		/*!
		 * \brief Обработчик HTTP
		 */
		QNetworkAccessManager m_http;

		/*!
		 * \brief Наименование протокола (HTTP/HTTPS)
		 */
		QString m_proto;

		/*!
		 * \brief Текст ошибки запроса (если есть)
		 */
		QString m_error;

		/*!
		 * \brief Заголовки ответа
		 */
		QString m_header;

		/*!
		 * \brief Тело ответа
		 */
		QString m_body;

		/*!
		 * \brief Красивое форматирование байт в KB/MB/GB
		 * \param size Количество байт
		 * \return Строка с числом и единицей измерения
		 */
		static QString formatPrettyBytes (qint64 size);

		/*!
		 * \brief Обработчик прогресса с пустым статусом
		 * \param percent [0..100] процент выполнения
		 */
		void onProgress (int percent);

		/*!
		 * \brief Обработчик прогресса
		 * \param percent [0..100] процент выполнения
		 * \param status Строка состояния
		 */
		void onProgress (int percent, const QString& status);

	private slots:

		//
		// события HTTP обработчика
		//

		void process_finished          (QNetworkReply* reply);              /*!< \brief Обработка завершения запроса */
		void process_download_progress (qint64 done, qint64 total);         /*!< \brief Прогресс чтения данных       */
		void process_upload_progress   (qint64 done, qint64 total);         /*!< \brief Прогресс отправки данных     */
		void process_ssl_errors        (const QList<QSslError> &errors);    /*!< \brief Ошибка SSL                   */
};

#endif   // _avalon_form_request_h_
