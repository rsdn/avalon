/*!
 * \file
 * \brief Модели для работы с группами форумов
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

#endif   // _avalon_forum_group_info_h_
