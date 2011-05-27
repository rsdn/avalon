/*!
 * \file
 * \brief Модели для работы группами форумов
 *
 * $Date: 2008-10-27 02:19:26 +0300 (Пнд, 27 Окт 2008) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 118 $
 * $URL: svn://opensvn.ru/avalon/trunk/model/group.h $
 */

#ifndef _avalon_group_info_h_
#define _avalon_group_info_h_

#include "../sysheaders.h"

/*!
 * \brief Описатель группы форумов
 */
typedef struct AGroupInfo
{
	int     ID;        /*!< \brief ID группы           */
	QString Name;      /*!< \brief Наименование группы */
	int     SortOrder; /*!< \brief Порядок сортировки  */
} AGroupInfo;

/*!
 * \brief Список групп
 */
typedef QList<AGroupInfo> AGroupInfoList;

/*!
 * \brief ID группы для служебных сообщений ("локальные")
 */
const int SPECIAL_ID_GROUP = -1;

#endif
