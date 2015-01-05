/*!
 * \file
 * \brief Интерфейс для отображения прогресса
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

		/*!
		 * \brief Прогресс
		 * \param minimum Минимальное значение
		 * \param maximum Максимальное значение
		 * \param value Текущее значение
		 */
		virtual void onProgress (int minimum, int maximum, int value) = 0;

		/*!
		 * \brief Прогресс
		 * \param minimum Минимальное значение
		 * \param maximum Максимальное значение
		 * \param value Текущее значение
		 * \param status Состояние операции
		 */
		virtual void onProgress (int minimum, int maximum, int value, const QString& status) = 0;
};

#endif   // _avalon_iprogress_h_
