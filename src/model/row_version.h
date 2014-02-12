/*!
 * \file
 * \brief Модели для работы с версиями данных (строк)
 */

#ifndef _avalon_row_version_info_h_
#define _avalon_row_version_info_h_

#include "sysheaders.h"

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
