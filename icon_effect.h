/*!
 * \file
 * \brief Класс для работы эффектами иконок
 *
 * $Date: 2009-03-28 10:11:23 +0300 (Сбт, 28 Мар 2009) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 193 $
 * $URL: svn://opensvn.ru/avalon/trunk/icon_effect.h $
 */

#ifndef _avalon_icon_effect_h_
#define _avalon_icon_effect_h_

#include "sysheaders.h"

/*!
 * \brief Класс для работы эффектами иконок
 */
class AIconEffect
{
	public:

		/*!
		 * \brief Объединение 2-х иконок
		 * \param file1 Иконка-подложка
		 * \param file2 Иконка-наложение
		 * \return Иконка с наложением
		 */
		static QIcon unionIcons (const QString& file1, const QString& file2);
};

#endif
