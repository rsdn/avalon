/*!
 * \file
 * \brief Модели для работы с группами форумов
 *
 * $Date: 2008-10-27 02:19:26 +0300 (Пнд, 27 Окт 2008) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 118 $
 * $URL: svn://opensvn.ru/avalon/trunk/model/forum_group.h $
 */

#ifndef _avalon_forum_group_info_h_
#define _avalon_forum_group_info_h_

#include "forum.h"
#include "group.h"

/*!
 * \brief Группа описателей форумов
 */
typedef struct AForumGroupInfo
{
	/*!
	 * \brief Описатель группы форумов
	 */
	AGroupInfo Group;

	/*!
	 * \brief Список описателей форумов в группе
	 */
	AForumInfoList Forums;
} AForumGroupInfo;

/*!
 * \brief Дерево форумов
 */
typedef QList<AForumGroupInfo> AForumGroupInfoList;

#endif
