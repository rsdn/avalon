/*!
 * \file
 * \brief Интерфейс для работы с деревом форумов
 *
 * $Date: 2010-12-03 02:53:46 +0300 (Птн, 03 Дек 2010) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 369 $
 * $URL: svn://opensvn.ru/avalon/trunk/iforum_tree.h $
 */

#ifndef _avalon_iforum_tree_h_
#define _avalon_iforum_tree_h_

#include "sysheaders.h"

/*!
 * \brief Интерфейс для работы с деревом форумов
 */
class IForumTree
{
	public:

		virtual ~IForumTree () {}

		/*!
		 * \brief Перезагрузка дерева форумов
		 */
		virtual void reload () = 0;

		/*!
		 * \brief Перезагрузка числа непрочитаных сообщений
		 * \param reload_message_tree необходимость перезагрузки дерева сообщений
		 */
		virtual void reloadUnread (bool reload_message_tree) = 0;

		/*!
		 * \brief Изменение числа непрочитанных сообщений для текущего форума в дереве форумов
		 * \param count дельта числа непрочитанных сообщений,
		 *              положительное значение увеличивает число на count,
		 *              отрицательное значение уменьшает число на count,
		 *              проверка соответствия итогового значения с реальным числом непрочитанных сообщений в дереве сообщений не производится.
		 * \param count_my дельта числа непрочитанных сообщений мне (по аналогии с параметром count)
		 * \param count_topics дельта числа непрочитанных топиков (по аналогии с параметром count)
		 * \param id_forum идентификатор требуемого форума (0 - текущий)
		 */
		virtual void changeUnreadCount (int count, int count_my, int count_topics, int id_forum = 0) = 0;

		/*!
		 * \brief Создание нового сообщение в текущем форуме
		 */
		virtual void newMessage () = 0;

		/*!
		 * \brief Перейти к следующему непрочитанному форуму
		 */
		virtual void gotoNextUnreadForum () = 0;

		/*!
		 * \brief Перемещение выделения на форум с требуемым id
		 * \param id форум для смены выделения
		 * \return true, если есть выделение, false если заданный форум не найден
		 */
		virtual bool selectForum (int id) = 0;
};

#endif
