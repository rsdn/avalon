/*!
 * \file
 * \brief Шаблон файла версии, который меняется при каждом билде
 *
 * $Date: 2011-04-20 17:27:00 +0400 (Срд, 20 Апр 2011) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 419 $
 * $URL: svn://opensvn.ru/avalon/trunk/version.h $
 */

#ifndef _avalon_version_h_
#define _avalon_version_h_

/*!
 * \brief Номер ревизии (заменяется автоматически svn-ом при коммите version.h)
 */
#define AVALON_REVISION "$Revision: 419 $"

/*!
 * \brief Дата билда (заменяется автоматически при каждом билде в version.h, что и приводит к смене номера ревизии)
 */
#define AVALON_DATE "Срд Апр 20 17:27:04 MSD 2011"

#endif
