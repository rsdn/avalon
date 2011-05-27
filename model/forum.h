/*!
 * \file
 * \brief Модели для работы с форумами
 *
 * $Date: 2009-07-17 19:36:33 +0400 (Птн, 17 Июл 2009) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 272 $
 * $URL: svn://opensvn.ru/avalon/trunk/model/forum.h $
 */

#ifndef _avalon_forum_info_h_
#define _avalon_forum_info_h_

#include "../sysheaders.h"

/*!
 * \brief Описатель форума
 */
typedef struct AForumInfo
{
	int     ID;        /*!< \brief ID форума                               */
	int     IDGroup;   /*!< \brief ID группы                               */
	QString ShortName; /*!< \brief Короткое имя                            */
	QString Name;      /*!< \brief Полное имя                              */
	bool    Rated;     /*!< \brief Оценивается ли форум                    */
	bool    InTop;     /*!< \brief Участвует ли оценки этого форума в топе */
	int     RateLimit; /*!< \brief Лимит оценки в форуме                   */
} AForumInfo;

/*!
 * \brief Список форумов
 */
typedef QList<AForumInfo> AForumInfoList;

/*!
 * \brief Описатель id подписанного форума
 */
typedef struct ASubscribedForumInfo
{
	int  IDForum; /*!< \brief ID форума                           */
	bool IsFirst; /*!< \brief true - первая подписка, иначе false */
} ASubscribedForumInfo;

/*!
 * \brief Список id подписаных форумов
 */
typedef QList<ASubscribedForumInfo> ASubscribedForumInfoList;

/*!
 * \brief Описатель количества непрочитаных сообщений в форуме
 */
typedef struct AUnreadForumCountInfo
{
	int IDForum; /*!< \brief ID форума                                */
	int Topics;  /*!< \brief Количество непрочитанных (новых) топиков */
	int Count;   /*!< \brief Количество непрочитанных сообщений       */
	int CountMy; /*!< \brief Количество непрочитанных сообщений мне   */
} AUnreadForumCountInfo;

/*!
 * \brief Список количества непрочитаных сообщений
 */
typedef QList<AUnreadForumCountInfo> AUnreadForumCountInfoList;

/*!
 * \brief Константа для спец-форума "сообщения к отправке"
 */
const int SPECIAL_ID_FORUM_MESSAGE2SEND = -1;

/*!
 * \brief Константа для спец-форума "рейтинг к отправке"
 */
const int SPECIAL_ID_FORUM_RATING2SEND = -2;

/*!
 * \brief Константа для спец-форума "модерилки к отправке"
 */
const int SPECIAL_ID_FORUM_MODERATE2SEND = -3;

/*!
 * \brief Константа для спец-форума "черновики"
 */
const int SPECIAL_ID_FORUM_DRAFTS = -4;

/*!
 * \brief Константа для спец-форума "мои сообщения"
 */
const int SPECIAL_ID_FORUM_MY_MESSAGES = -5;

/*!
 * \brief Константа для спец-форума "ответы мне"
 */
const int SPECIAL_ID_FORUM_ANSWERS_TO_ME = -6;

#endif
