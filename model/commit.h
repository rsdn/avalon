/*!
 * \file
 * \brief Модели для коммита информации отосланой на сервер
 */

#ifndef _avalon_commit_info_h_
#define _avalon_commit_info_h_

#include "../sysheaders.h"

/*!
 * \brief Описатель исключения коммита при отправке сообщений/оценок/модерилок на сервер
 *
 * После отправки данных (сообщений/модерилок/оценок) на сервер
 * (см. AWebservice::postChange_WebserviceQuery и AWebservice::postChange_WebserviceParse),
 * необходимо проверить результат отправки, для чего вызывается операция коммита отправленных данных
 * (см. AWebservice::postChangeCommit_WebserviceQuery) и анализируется результат
 * (см. AWebservice::postChangeCommit_WebserviceParse).
 *
 * Результатом является структура ACommitInfo со списком структур ACommitExceptionInfo
 * для каждого отправленного сообщения/оценки/модерилки, которые не прошли операцию коммита.
 * Анализируя списки исключений можно судить об успешности отправки каждого элемента.
 */
typedef struct ACommitExceptionInfo
{
	QString Exception; /*!< \brief Текст исключения                  */
	int     ID;        /*!< \brief Локальный id элемента в хранилище */
	QString Info;      /*!< \brief Дополнительная информация         */
} ACommitExceptionInfo;

/*!
 * \brief Список исключений
 */
typedef QList<ACommitExceptionInfo> ACommitExceptionInfoList;

/*!
 * \brief Описатель коммита
 * За подробностями см. ACommitExceptionInfo
 */
typedef struct ACommitInfo
{
	QList<int>               Messages;           /*!< \brief Список ID успешно закоммиченых сообщений                */
	ACommitExceptionInfoList MessagesExceptions; /*!< \brief Список исключений для неуспешно закомиченных сообщений  */
	QList<int>               Rating;             /*!< \brief Список ID успешно закоммиченых рейтингов                */
	ACommitExceptionInfoList RatingExceptions;   /*!< \brief Список исключений для неуспешно закомиченных рейтингов  */
	QList<int>               Moderate;           /*!< \brief Список ID успешно закоммиченых модерилок                */
	ACommitExceptionInfoList ModerateExceptions; /*!< \brief Список исключений для неуспешно закомиченных модерилок  */
} ACommitInfo;

#endif
