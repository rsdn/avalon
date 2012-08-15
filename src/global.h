/*!
 * \file
 * \brief Заголовочный файл для глобальных настроек
 */

#ifndef _avalon_global_h_
#define _avalon_global_h_

#include "model/user.h"

/*!
 * \brief Глобальные настройки
 */
class AGlobal
{
	private:

		AGlobal ();

	public:

		/*!
		 * \brief Стандартный singleton
		 */
		static AGlobal* getInstance ();

		~AGlobal ();

		/*!
		 * \brief Перезагрузка настроек
		 */
		void reload ();

		//
		// Общие переменные
		//

		AUserInfo Me;          /*!< \brief Текущий пользователь avalon */
#ifdef AVALON_USE_ZLIB
		bool      Compression; /*!< \brief Сжатие тел сообщений        */
#endif

		//
		// Константы форматирования
		//

		QString AnonymousName; /*!< \brief Отображаемое имя анонима */
		QString DateFormat;    /*!< \brief Формат даты/времени      */
};

#endif   // _avalon_global_h_
