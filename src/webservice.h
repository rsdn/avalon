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
class AWebservice
{
	public:

		/*!
		 * \brief Возвращает заголовок и данные для POST запроса получения списка форумов
		 * \param header Заголовок HTTP запроса
		 * \param data Данные для POST запроса
		 * \param progress Прогресс
		 */
		static void getForumList_WebserviceQuery (QString& header, QString& data, IProgress* progress = NULL);

		/*!
		 * \brief Парсит ответ со списком форумов и заносит результат в список
		 * \param data Строка с данными HTTP ответа
		 * \param list Список групп/форумов
		 * \param progress Прогресс
		 */
		static void getForumList_WebserviceParse (const QString& data, AForumGroupInfoList& list, IProgress* progress = NULL);

		/*!
		 * \brief Возвращает заголовок и данные для POST запроса получения списка форумов
		 * \param header Заголовок HTTP запроса
		 * \param data Данные для POST запроса
		 * \param last_row_version Версия строк (снимка)
		 * \param progress Прогресс
		 */
		static void getUserList_WebserviceQuery (QString& header, QString& data, const QString& last_row_version, IProgress* progress = NULL);

		/*!
		 * \brief Парсит ответ со списком пользователей и заносит результат в список
		 * \param data Строка с данными HTTP ответа
		 * \param list Список пользователей
		 * \param row_version Версия строк (снимка)
		 * \param progress Прогресс
		 * \return Пустая строка, или сообщение об ошибке
		 */
		static QString getUserList_WebserviceParse (const QString& data, AUserInfoList& list, QString& row_version, IProgress* progress = NULL);

		/*!
		 * \brief Возвращает заголовок и данные для POST запроса получения списка сообщений
		 * \param header Заголовок HTTP запроса
		 * \param data Данные для POST запроса
		 * \param row_version Версия строк (снимка)
		 * \param query Описатель запроса данных (поломанные топики, сообщения на докачку, список форумов)
		 * \param progress Прогресс
		 */
		static void getMessageList_WebserviceQuery (QString& header, QString& data, const ARowVersion& row_version, const ADataQuery& query, IProgress* progress = NULL);

		/*!
		 * \brief Парсит ответ со списком сообщений и заносит результат в список
		 * \param data Строка с данными HTTP ответа
		 * \param list Список сообщений
		 * \param row_version Версия строк (снимка)
		 * \param progress Прогресс
		 * \return Пустая строка, или сообщение об ошибке
		 */
		static QString getMessageList_WebserviceParse (const QString& data, ADataList& list, ARowVersion& row_version, IProgress* progress = NULL);

		/*!
		 * \brief Возвращает заголовок и данные для POST запроса отправки данных
		 * \param header Заголовок HTTP запроса
		 * \param data Данные для POST запроса
		 * \param list_messages Список сообщений к отправке
		 * \param list_rating Список рейтингов к отправке
		 * \param list_moderate Список модерилок к отправке
		 * \param progress Прогресс
		 */
		static void postChange_WebserviceQuery (QString& header, QString& data, const AMessage2SendList& list_messages, const ARating2SendList& list_rating, const AModerate2SendList& list_moderate, IProgress* progress = NULL);

		/*!
		 * \brief Парсит ответ отправки данных и возвращает куку для сессии
		 * \param header Строка с данными HTTP заголовка ответа
		 * \param cookie Кука
		 * \param progress Прогресс
		 * \return Пустая строка, или сообщение об ошибке
		 */
		static QString postChange_WebserviceParse (const QString& header, QString& cookie, IProgress* progress = NULL);

		/*!
		 * \brief Возвращает заголовок и данные для POST запроса коммита данных
		 * \param header Заголовок HTTP запроса
		 * \param data Данные для POST запроса
		 * \param cookie Кука
		 * \param progress Прогресс
		 */
		static void postChangeCommit_WebserviceQuery (QString& header, QString& data, const QString& cookie, IProgress* progress = NULL);

		/*!
		 * \brief Парсит ответ со списком закоммиченых данных и заносит результат в список
		 * \param data Строка с данными HTTP ответа
		 * \param commit_info Описатель результата коммита
		 * \param progress Прогресс
		 * \return Пустая строка, или сообщение об ошибке
		 */
		static QString postChangeCommit_WebserviceParse (const QString& data, ACommitInfo& commit_info, IProgress* progress = NULL);
};

#endif
