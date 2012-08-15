/*!
 * \file
 * \brief Интерфейс для работы с главной формой приложения
 */

#ifndef _avalon_iform_main_h_
#define _avalon_iform_main_h_

#include "../sysheaders.h"

/*!
 * \brief Перечисление действий/меню для включения отключения
 */
typedef enum AvalonActions
{
	aaNewMessage,            /*!< \brief Действие создания сообщения  */
	aaViewSource,            /*!< \brief Действие просмотра источника */
	aaPrevNextUnreadArticle, /*!< \brief Действие показать предыдущее/следующее непрочитанное сообщение */
	aaPrevNextUnreadThread,  /*!< \brief Действие показать предыдущую/следующую непрочитанную ветку     */
	aaPrevNextUnreadForum,   /*!< \brief Действие показать предыдущий/следующий непрочитанный форум     */
	aaBackward,              /*!< \brief Действие "Назад"  */
	aaForward                /*!< \brief Действие "Вперед" */
} AvalonActions;

/*!
 * \brief Интерфейс для взаимодействия с главной формой
 */
class IFormMain
{
	public:

		/*!
		 * \brief Отображение строки статуса в статус-баре
		 * \param value строка для отображения
		 */
		virtual void showStatus (const QString& value) = 0;

		/*!
		 * \brief Блокировка/разблокировка кнопок на тулбаре и элементов меню
		 * \param action кнопка/действие (см. AvalonActions)
		 * \param enabled флаг включения или отключения группы элементов
		 */
		virtual void setEnabledAction (AvalonActions action, bool enabled) = 0;
};

#endif
