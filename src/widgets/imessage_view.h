/*!
 * \file
 * \brief Интерфейс для работы с областью отображения HTML сообщения извне
 */

#ifndef _avalon_imessage_view_h_
#define _avalon_imessage_view_h_

#include "../model/all.h"

/*!
 * \brief Интерфейс для работы с областью отображения HTML сообщения извне
 */
class IMessageView
{
	public:

		/*!
		 * \brief Очистить область (когда нечего отображать)
		 */
		virtual void clear () = 0;

		/*!
		 * \brief Отобразить сообщение
		 * \param message Дескриптор сообщения
		 * \param special Флаг того, что это сообщение на отправку
		 * \param rated Флаг того, что сообщение может быть оценено
		 */
		virtual void setMessage (const AMessageInfo& message, bool special = false, bool rated = false) = 0;
};

#endif
