/*!
 * \file
 * \brief Модели для работы с модерилками
 */

#ifndef _avalon_moderate_info_h_
#define _avalon_moderate_info_h_

#include "sysheaders.h"

/*!
 * \brief Описатель модерилки
 */
typedef struct AModerateInfo
{
	int       IDMessage;   /*!< \brief ID сообщения    */
	int       IDTopic;     /*!< \brief ID топика       */
	int       IDUser;      /*!< \brief ID пользователя */
	int       IDForum;     /*!< \brief ID форума       */
	QDateTime Created;     /*!< \brief Дата            */
} AModerateInfo;

/*!
 * \brief Список модерилок
 */
typedef QList<AModerateInfo> AModerateInfoList;

/*!
 * \brief Описатель модерилки к отправке
 */
typedef struct AModerate2Send
{
	int       ID;            /*!< \brief ID локальный              */
	int       IDMessage;     /*!< \brief ID сообщения              */
	QString   Action;        /*!< \brief Действие {MoveMessage, DeleteMessage, DeleteThread, DeleteErrorMessage, SplitThread, CloseTopic, OpenTopic} */
	int       IDForum;       /*!< \brief ID форума для перемещения */
	QString   Description;   /*!< \brief Описание                  */
	bool      AsModerator;   /*!< \brief Как администратор         */
	QDateTime Date;          /*!< \brief Дата                      */
} AModerate2Send;

/*!
 * \brief Список модерилок к отправке
 */
typedef QList<AModerate2Send> AModerate2SendList;

#endif   // _avalon_moderate_info_h_
