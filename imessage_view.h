/*!
 * \file
 * \brief Интерфейс для работы с областью отображения HTML сообщения извне
 *
 * $Date: 2008-09-18 04:11:22 +0400 (Чтв, 18 Сен 2008) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 104 $
 * $URL: svn://opensvn.ru/avalon/trunk/imessage_view.h $
 */

#ifndef _avalon_imessage_view_h_
#define _avalon_imessage_view_h_

#include "model/all.h"

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
