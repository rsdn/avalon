/*!
 * \file
 * \brief Интерфейс для отображения прогресса
 *
 * $Date: 2011-04-07 11:11:49 +0400 (Чтв, 07 Апр 2011) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 406 $
 * $URL: svn://opensvn.ru/avalon/trunk/iprogress.h $
 */

#ifndef _avalon_iprogress_h_
#define _avalon_iprogress_h_

#include "sysheaders.h"

/*!
 * \brief Интерфейс для отображения прогресса выполнения к-л операции
 */
class IProgress
{
	public:

		/*!
		 * \brief Прогресс
		 * \param percent Количество выполненых процентов 0..100
		 */
		virtual void onProgress (int percent) = 0;

		/*!
		 * \brief Прогресс
		 * \param percent Количество выполненых процентов 0..100
		 * \param status Состояние операции
		 */
		virtual void onProgress (int percent, const QString& status) = 0;
};

#endif
