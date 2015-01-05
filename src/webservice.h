/*!
 * \file
 * \brief Класс для работы с вебсервисами RSDN
 */

#ifndef _avalon_webservice_h_
#define _avalon_webservice_h_

#include "model/all.h"

/*!
 * \brief Класс для работы с вебсервисами RSDN
 */
class AWebservice
{
	public:

		/*!
		 * \brief Заполнение полей запроса значениями по умолчанию
		 * \param request Запрос для заполнения
		 * \param proto Протокол (http | https)
		 * \param action Значение SOAPAction (без части http://rsdn.ru/Janus/)
		 * \param length Значение Content-Length
		 */
		static void defaultRequest(QNetworkRequest& request, const QString& proto, const QString& action, qint64 length);

		/*!
		 * \brief Возвращает заголовок и данные для POST запроса получения списка форумов
		 * \param request Запрос для заполнения
		 * \param data Данные для POST запроса
		 * \param progress Прогресс
		 */
		static void getForumList_WebserviceQuery (QNetworkRequest& request, QString& data);

		/*!
		 * \brief Парсит ответ со списком форумов и заносит результат в список
		 * \param data Строка с данными HTTP ответа
		 * \param list Список групп/форумов
		 * \param progress Прогресс
		 */
		static void getForumList_WebserviceParse (const QString& data, AForumGroupInfoList& list);

		/*!
		 * \brief Возвращает заголовок и данные для POST запроса получения списка форумов
		 * \param request Запрос для заполнения
		 * \param data Данные для POST запроса
		 * \param last_row_version Версия строк (снимка)
		 * \param progress Прогресс
		 */
		static void getUserList_WebserviceQuery (QNetworkRequest& request, QString& data, const QString& last_row_version);

		/*!
		 * \brief Парсит ответ со списком пользователей и заносит результат в список
		 * \param data Строка с данными HTTP ответа
		 * \param list Список пользователей
		 * \param row_version Версия строк (снимка)
		 * \param progress Прогресс
		 * \return Пустая строка, или сообщение об ошибке
		 */
		static QString getUserList_WebserviceParse (const QString& data, AUserInfoList& list, QString& row_version);

		/*!
		 * \brief Возвращает заголовок и данные для POST запроса получения списка сообщений
		 * \param request Запрос для заполнения
		 * \param data Данные для POST запроса
		 * \param row_version Версия строк (снимка)
		 * \param query Описатель запроса данных (поломанные топики, сообщения на докачку, список форумов)
		 * \param progress Прогресс
		 */
		static void getMessageList_WebserviceQuery (QNetworkRequest& request, QString& data, const ARowVersion& row_version, const ADataQuery& query);

		/*!
		 * \brief Парсит ответ со списком сообщений и заносит результат в список
		 * \param data Строка с данными HTTP ответа
		 * \param list Список сообщений
		 * \param row_version Версия строк (снимка)
		 * \param progress Прогресс
		 * \return Пустая строка, или сообщение об ошибке
		 */
		static QString getMessageList_WebserviceParse (const QString& data, ADataList& list, ARowVersion& row_version);

		/*!
		 * \brief Возвращает заголовок и данные для POST запроса отправки данных
		 * \param request Запрос для заполнения
		 * \param data Данные для POST запроса
		 * \param list_messages Список сообщений к отправке
		 * \param list_rating Список рейтингов к отправке
		 * \param list_moderate Список модерилок к отправке
		 * \param progress Прогресс
		 */
		static void postChange_WebserviceQuery (QNetworkRequest& request, QString& data, const AMessage2SendList& list_messages, const ARating2SendList& list_rating, const AModerate2SendList& list_moderate);

		/*!
		 * \brief Парсит ответ отправки данных и возвращает куку для сессии
		 * \param header Строка с данными HTTP заголовка ответа
		 * \param cookie Кука
		 * \param progress Прогресс
		 * \return Пустая строка, или сообщение об ошибке
		 */
		static QString postChange_WebserviceParse (const QString& header, QString& cookie);

		/*!
		 * \brief Возвращает заголовок и данные для POST запроса коммита данных
		 * \param request Запрос для заполнения
		 * \param data Данные для POST запроса
		 * \param cookie Кука
		 * \param progress Прогресс
		 */
		static void postChangeCommit_WebserviceQuery (QNetworkRequest& request, QString& data, const QString& cookie);

		/*!
		 * \brief Парсит ответ со списком закоммиченых данных и заносит результат в список
		 * \param data Строка с данными HTTP ответа
		 * \param commit_info Описатель результата коммита
		 * \param progress Прогресс
		 * \return Пустая строка, или сообщение об ошибке
		 */
		static QString postChangeCommit_WebserviceParse (const QString& data, ACommitInfo& commit_info);

	private:

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
};

#endif
