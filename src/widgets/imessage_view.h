/*!
 * \file
 * \brief Интерфейс для работы с областью отображения HTML сообщения извне
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
		 * \param forum Дескриптор форума (не спец-форума)
		 */
		virtual void setMessage (const AMessageInfo& message, const AForumInfo* forum = NULL) = 0;
};

#endif   // _avalon_imessage_view_h_
