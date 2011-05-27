/*!
 * \file
 * \brief Модели для работы с версиями данных (строк)
 *
 * $Date: 2009-03-30 19:53:38 +0400 (Пнд, 30 Мар 2009) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 197 $
 * $URL: svn://opensvn.ru/avalon/trunk/model/row_version.h $
 */

#ifndef _avalon_row_version_info_h_
#define _avalon_row_version_info_h_

#include "../sysheaders.h"

/*!
 * \brief Описатель версий строк
 */
typedef struct ARowVersion
{
	QString User;     /*!< \brief Пользователи */
	QString Message;  /*!< \brief Сообщения    */
	QString Rating;   /*!< \brief Рейтинг      */
	QString Moderate; /*!< \brief Модерилки    */
} ARowVersion;

#endif
