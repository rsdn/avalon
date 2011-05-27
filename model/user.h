/*!
 * \file
 * \brief Модели для работы с пользователями
 *
 * $Date: 2008-10-27 02:19:26 +0300 (Пнд, 27 Окт 2008) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 118 $
 * $URL: svn://opensvn.ru/avalon/trunk/model/user.h $
 */

#ifndef _avalon_user_info_h_
#define _avalon_user_info_h_

#include "../sysheaders.h"

/*!
 * \brief Описатель пользователя
 */
typedef struct AUserInfo
{
	int     ID;             /*!< \brief ID                 */
	QString Name;           /*!< \brief Логин пользователя */
	QString Nick;           /*!< \brief Ник                */
	QString RealName;       /*!< \brief Настоящее имя      */
	QString Email;          /*!< \brief Мыло               */
	QString Homepage;       /*!< \brief Домашняя страница  */
	QString Specialization; /*!< \brief Специализация      */
	QString WhereFrom;      /*!< \brief Откуда             */
	QString Origin;         /*!< \brief Подпись            */
} AUserInfo;

/*!
 * \brief Список пользователей
 */
typedef QList<AUserInfo> AUserInfoList;

#endif
