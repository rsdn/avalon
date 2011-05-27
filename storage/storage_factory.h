/*!
 * \file
 * \brief Работа с хранилищами данных
 *
 * $Date: 2009-03-30 19:53:38 +0400 (Пнд, 30 Мар 2009) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 197 $
 * $URL: svn://opensvn.ru/avalon/trunk/storage/storage_factory.h $
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
