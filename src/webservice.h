/*!
 * \file
 * \brief Класс для работы с вебсервисами RSDN
 */

#ifndef _avalon_webservice_h_
#define _avalon_webservice_h_

#include "iprogress.h"
#include "model/all.h"

/*!
 * \brief Класс для работы с вебсервисами RSDN
 */
class AWebservice : public QObject
{
	Q_OBJECT

	public:

		/*!
		 * \brief Конструктор
		 * \param parent Родительский виджет (для отображения сообщения об ошибках)
		 * \param progress Прогресс
		 */
		AWebservice  (QWidget* parent, IProgress* progress = NULL);
		~AWebservice ();

		/*!
		 * \brief Получение списка форумов
		 * \param list Список групп/форумов
		 * \return Флаг ошибки
		 */
		bool getForumList (AForumGroupInfoList& list);

		/*!
		 * \brief Получение списка пользователей
		 * \param last_row_version Текущая версия строк (снимка)
		 * \param list Список пользователей
		 * \param row_version Новая версия строк (снимка)
		 * \return Флаг ошибки
		 */
		bool getUserList (const QString& last_row_version, AUserInfoList& list, QString& row_version);

		/*!
		 * \brief Получение списка сообщений
		 * \param last_row_version Текущая версия строк (снимка)
		 * \param query Описатель запроса данных (поломанные топики, сообщения на докачку, список форумов)
		 * \param list Список сообщений
		 * \param row_version Новая версия строк (снимка)
		 * \return Флаг ошибки
		 */
		bool getMessageList (const ARowVersion& last_row_version, const ADataQuery& query, ADataList& list, ARowVersion& row_version);

		/*!
		 * \brief Отправка данных
		 * \param list_messages Список сообщений к отправке
		 * \param list_rating Список рейтингов к отправке
		 * \param list_moderate Список модерилок к отправке
		 * \param commit_info Описатель результата коммита
		 * \return Флаг ошибки
		 */
		bool postChange (const AMessage2SendList& list_messages, const ARating2SendList& list_rating, const AModerate2SendList& list_moderate, ACommitInfo& commit_info);

		/*!
		 * \brief Получение текста ошибки
		 * \return Текст ошибки
		 */
		QString error() { return m_error; }

		/*!
		 * \brief Возвращает прокси-сервер по умолчанию
		 * \param webkit Флаг запроса прокси для области отображения сообщений
		 * \return Описатель прокси-сервера
		 */
		static QNetworkProxy defaultProxy (bool webkit = false);

	private:

		/*!
		 * \brief Прогресс
		 */
		IProgress* m_progress;

		/*!
		 * \brief Текст ошибки
		 */
		QString m_error;

		/*!
		 * \brief Содержимое заголовков
		 */
		QString m_headers;

		/*!
		 * \brief Содержимое ответа
		 */
		QString m_body;

		/*!
		 * \brief Логин на rsdn
		 */
		QString m_rsdn_login;

		/*!
		 * \brief Пароль на rsdn
		 */
		QString m_rsdn_password;

		/*!
		 * \brief Протокол доступа к веб-сервису
		 */
		QString m_rsdn_proto;

	private:

		/*!
		 * \brief Парсит ответ со списком форумов и заносит результат в список
		 * \param data Строка с данными HTTP ответа
		 * \param list Список групп/форумов
		 */
		static void parseForumList (const QString& data, AForumGroupInfoList& list);

		/*!
		 * \brief Парсит ответ со списком пользователей и заносит результат в список
		 * \param data Строка с данными HTTP ответа
		 * \param list Список пользователей
		 * \param row_version Версия строк (снимка)
		 * \return Пустая строка, или сообщение об ошибке
		 */
		static QString parseUserList (const QString& data, AUserInfoList& list, QString& row_version);

		/*!
		 * \brief Парсит ответ со списком сообщений и заносит результат в список
		 * \param data Строка с данными HTTP ответа
		 * \param list Список сообщений
		 * \param row_version Версия строк (снимка)
		 * \return Пустая строка, или сообщение об ошибке
		 */
		static QString parseMessageList (const QString& data, ADataList& list, ARowVersion& row_version);

		/*!
		 * \brief Парсит ответ отправки данных и возвращает куку для сессии
		 * \param header Строка с данными HTTP заголовка ответа
		 * \param cookie Кука
		 * \return Пустая строка, или сообщение об ошибке
		 */
		static QString parsePostChange (const QString& header, QString& cookie);

		/*!
		 * \brief Парсит ответ со списком закоммиченых данных и заносит результат в список
		 * \param data Строка с данными HTTP ответа
		 * \param commit_info Описатель результата коммита
		 * \return Пустая строка, или сообщение об ошибке
		 */
		static QString parsePostChangeCommit (const QString& data, ACommitInfo& commit_info);

		/*!
		 * \brief Возвращает текст между from-to не включая их из строки source
		 * \param source Указатель на исходную строку
		 * \param from Начальный разделитель
		 * \param to Конечный разделитель
		 * \return Строка между разделителями
		 */
		static QString getTextBetween (const QString* source, const QString& from, const QString& to);

		/*!
		 * \brief Итерация по блокам from-to со сдвигом смещения
		 * \param source Указатель на исходную строку
		 * \param from Начальный разделитель
		 * \param to Конечный разделитель
		 * \param seed Смещение
		 * \return Строка между разделителями
		 */
		static QString getNextBlock (const QString* source, const QString& from, const QString& to, int& seed);

		/*!
		 * \brief Преобразование строки в дату/время (требуется в связи с отсчетом времени с начала эпохи UNIX)
		 * \param value Строковое значение даты
		 * \return Объект преобразования
		 */
		static QDateTime getDateTimeFromString (const QString& value);

		/*!
		 * \brief Преобразование строки в булево значение
		 * \param value Строковое значение флага
		 * \return Объект преобразования
		 */
		static bool getBooleanFromString (const QString& value);

		/*!
		 * \brief Красивое форматирование байт в KB/MB/GB
		 * \param size Количество байт
		 * \return Строка с числом и единицей измерения
		 */
		static QString formatPrettyBytes (qint64 size);

		/*!
		 * \brief Подготовка полей запроса
		 * \param request Запрос для заполнения
		 * \param proto Протокол (http | https)
		 * \param action Значение SOAPAction (без части http://rsdn.ru/Janus/)
		 * \param length Значение Content-Length
		 */
		static void prepareRequest (QNetworkRequest& request, const QString& proto, const QString& action, qint64 length);

		/*!
		 * \brief Выполнение запроса
		 * \param request Запрос
		 * \param data Данные для запроса
		 * \return Флаг ошибки
		 */
		bool makeRequest (const QNetworkRequest& request, const QString& data);

		/*!
		 * \brief Парсинг ответа
		 * \param request Запрос
		 * \return Флаг ошибки
		 */
		bool parseReply (QNetworkReply* reply);

	private slots:

		//
		// события HTTP обработчика
		//

		void process_download_progress (qint64 done, qint64 total);        /*!< \brief Прогресс чтения данных   */
		void process_upload_progress   (qint64 done, qint64 total);        /*!< \brief Прогресс отправки данных */
		void process_ssl_errors        (const QList<QSslError> &errors);   /*!< \brief Ошибка SSL               */
};

#endif
