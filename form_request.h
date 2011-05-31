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
		 * \param https Флаг необходимости установки HTTPS соединения
		 */
		FormRequest  (QWidget* parent, const QString& host, quint16 port, const QString& header, const QString& data, bool https = false);

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
		 * \return Описатель прокси-сервера
		 */
		static QNetworkProxy defaultProxy ();

	private:

		/*!
		 * \brief Обработчик HTTP
		 */
		QHttp m_http;

		/*!
		 * \brief Байт к отправке
		 */
		int m_to_send;

		/*!
		 * \brief Хак, чтобы не отображалось две попытки соединения
		 */
		int m_hack;

		/*!
		 * \brief Красивое форматирование байт в KB/MB/GB
		 * \param size Количество байт
		 * \return Строка с числом и единицей измерения
		 */
		QString formatPrettyBytes (qint64 size);

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

		void process_state_changed            (int state);                       /*!< \brief Смена состояния              */
		void process_data_read_progress       (int done, int total);             /*!< \brief Прогресс чтения данных       */
		void process_data_send_progress       (int done, int total);             /*!< \brief Прогресс отправки данных     */
		void process_response_header_received (const QHttpResponseHeader& resp); /*!< \brief Получен заголовок ответа     */
		void process_request_started          (int id);                          /*!< \brief Начало выполнения запроса    */
		void process_request_finished         (int id, bool error);              /*!< \brief Окончание выполнения запроса */

};

#endif   // _avalon_form_request_h_
