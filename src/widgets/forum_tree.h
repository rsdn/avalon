/*!
 * \file
 * \brief Виджет дерева форумов
 */

#ifndef _avalon_forum_tree_h_
#define _avalon_forum_tree_h_

#include "interfaces.h"

/*!
 * \brief Виджет дерева форумов
 */
class AForumTree :
	public QTreeWidget,
	public IForumTree
{
	Q_OBJECT

	public:

		/*!
		 * \param parent Родительский виджет
		 * \param form Главная форма приложения для отображения сообщений об ошибках
		 */
		AForumTree  (QWidget* parent, QWidget* form);
		~AForumTree ();

		/*!
		 * \brief Сохранение размеров дерева форумов
		 */
		void save ();

		/*!
		 * \brief Восстановление размеров дерева форумов
		 */
		void restore ();

		/*!
		 * \brief Установка интерфейса дерева сообщений
		 */
		void setMessageTree (IMessageTree* itf);

		/*!
		 * \brief Установка интерфейса виджета отображения сообщения
		 */
		void setMessageView (IMessageView* itf);

		/*!
		 * \brief Установка интерфейса главной формы
		 */
		void setMainForm (IFormMain* itf);

	private:

		/*!
		 * \brief Главная форма приложения для отображения ошибок
		 */
		QWidget* m_parent;

		IMessageTree* m_message_tree; /*!< \brief Интерфейс виджета дерева сообщений          */
		IMessageView* m_message_view; /*!< \brief Интерфейс виджета для отображения сообщения */
		IFormMain*    m_main_form;    /*!< \brief Интерфейс главной формы приложения          */

		int m_total_unread_count; /*!< \brief Общее количество непрочитанных сообщений в форумах исключая спецфорумы */

		int m_specail_group_count; /*!< \brief Общее количество спец-групп форумов */

		//
		// иконки для отображения статусов сообщений
		//

		QIcon m_forum_read;             /*!< \brief Все сообщения прочитанны                                                  */
		QIcon m_forum_unread_topics;    /*!< \brief Есть непрочитанные (новые) топики                                         */
		QIcon m_forum_unread_child;     /*!< \brief Есть непрочитанные сообщения и нет новых топиков или сообщений для меня   */
		QIcon m_forum_unread_child_my;  /*!< \brief Есть непрочитанные сообщения для меня и нет новых (непрочитанных) топиков */
		QIcon m_forum_unread_topics_my; /*!< \brief Есть непрочитанные (новые) топики И непрочитанные сообщения для меня      */

		/*!
		 * \brief Функция проверки установки выделения, в случае отсутствия выделения, выделяет самый верхний элемент
		 * \return true, если есть выделение, false при отсутствии элементов
		 */
		bool checkCurrentItem ();

		/*!
		 * \brief Функция проверки элемента дерева на то, что этот элемент принадлежит группе форумов
		 * \param item элемент дерева
		 * \return true, если элемент является элементом группы, иначе false
		 */
		bool isGroup (QTreeWidgetItem* item);

		/*!
		 * \brief Функция для получения текущего выделенного элемента форума в дереве
		 * \return текущий выделенный элемент форума или NULL, если элемент является группой
		 */
		QTreeWidgetItem* getSelectedForumItem ();

		/*!
		 * \brief Возвращает количество непрочитанных сообщений в текущем форуме
		 * \return количество сообщений в текущем форуме или -1, если форум не выбран
		 */
		int getSelectedUnreadCount ();

		/*!
		 * \brief Всплывающее контекстное меню
		 */
		QMenu* m_menu;

		QAction* m_menu_mark_all_as_read;       /*!< \brief Пометить все как прочитанные */
		QAction* m_menu_mark_patrial_as_read;   /*!< \brief Пометить до даты как прочитанные */
		QAction* m_menu_mark_all_as_unread;     /*!< \brief Пометить все как непрочитанные */
		QAction* m_menu_mark_patrial_as_unread; /*!< \brief Пометить после даты как непрочитанное */
		QAction* m_menu_unsubscribe;            /*!< \brief Отписаться от форума/группы */

	// IForumTree
	public:

		void reload ();

		void reloadUnread (bool reload_message_tree);

		void changeUnreadCount (int count, int count_my, int count_topics, int id_forum);

		void newMessage ();

		void gotoNextUnreadForum ();

		bool selectForum (int id);

	private slots:

		/*!
		 * \brief Смена выделения в дереве
		 */
		void current_item_changed (QTreeWidgetItem* current, QTreeWidgetItem* previous);

		/*!
		 * \brief Запрос контекстного меню
		 * \param pos Точка
		 */
		void context_menu_request (const QPoint& pos);

		void menu_mark_all_as_read_triggered       (); /*!< \brief Пометить все как прочитанные */
		void menu_mark_patrial_as_read_triggered   (); /*!< \brief Пометить до даты как прочитанные */
		void menu_mark_all_as_unread_triggered     (); /*!< \brief Пометить все как непрочитанные */
		void menu_mark_patrial_as_unread_triggered (); /*!< \brief Пометить после даты как непрочитанное */
		void menu_unsubscribe_triggered            (); /*!< \brief Отписаться от форума/группы */
};

#endif
