/*!
 * \file
 * \brief Работа с хранилищами данных
 */

#ifndef _avalon_storage_factory_h_
#define _avalon_storage_factory_h_

#include "istorage.h"

/*!
 * \brief Фабрика хранилищ данных
 * Предназначена для создания экземпляров классов для работы с хранилищем установленного в настройках приложения типа
 */
class AStorageFactory
{
	public:

		/*!
		 * \brief Получение хранилища типа определенного в настройках
		 * \return хранилище типа определенного в настройках или NULL, если тип не определен
		 */
		static IAStorage* getStorage ();
};

#endif
