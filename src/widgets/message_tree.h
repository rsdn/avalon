/*!
 * \file
 * \brief Виджет дерева сообщений
 */

#ifndef _avalon_message_tree_h_
#define _avalon_message_tree_h_

#include "../interfaces.h"

/*!
 * \brief Виджет дерева сообщений
 */
class AMessageTree :
	public  QTreeWidget,           // стандартное дерево Qt
	public  IMessageTree,          // интерфес для взаимодействия со списком форумов и отображением сообщения
	private IMessageInfoGUIFactory // интерфейс фабрики создания элементов дерева
{
	Q_OBJECT

	public:

		/*!
		 * \brief Конструктор
		 * \param parent Родительский виджет
		 * \param form Главная форма приложения для отображения сообщений об ошибках
		 */
		AMessageTree  (QWidget* parent, QWidget* form);
		~AMessageTree ();

		/*!
		 * \brief Сохранение размеров layout-а
		 */
		void save ();

		/*!
		 * \brief Восстановление размеров layout-а
		 */
		void restore ();

		/*!
		 * \brief Установка интерфейса для работы с деревом форумов
		 * \param itf Интерфейс
		 */
		void setForumTree (IForumTree* itf);

		/*!
		 * \brief Установка интерфейса для работы с областью отображения сообщения
		 * \param itf Интерфейс
		 */
		void setMessageView (IMessageView* itf);

		/*!
		 * \brief Установка интерфейса для работы с главной формой
		 * \param itf Интерфейс
		 */
		void setMainForm (IFormMain* itf);

	protected:

		/*!
		 * \brief При изменении размеров происходит псевдо-скроллинг сообщений (см. vertical_scroll)
		 */
                void resizeEvent (QResizeEvent* event);

	private:

		QWidget* m_parent; /*!< \brief Главная форма приложения (для отображения сообщений об ошибках) */

		IForumTree*   m_forum_tree;   /*!< \brief Интерфейс дерева форумов        */
		IMessageView* m_message_view; /*!< \brief Интерфейс отображения сообщения */
		IFormMain*    m_main_form;    /*!< \brief Интерфейс главной формы         */

		//
		// иконки для отображения статусов сообщений
		//

		QIcon m_message_read;      /*!< \brief Сообщение прочитано                     */
		QIcon m_child_unread;      /*!< \brief Есть непрочитанные дочерние сообщения   */
		QIcon m_message_unread;    /*!< \brief Сообщение не прочитано                  */
		QIcon m_child_unread_my;   /*!< \brief В ветке есть сообщение для меня         */
		QIcon m_message_unread_my; /*!< \brief В ветке есть сообщение для меня         */

		//
		// иконки для отображения оценок
		//

		QIcon m_rating_plus_1; /*!< \brief "+1"  */
		QIcon m_rating_1;      /*!< \brief "1"   */
		QIcon m_rating_2;      /*!< \brief "2"   */
		QIcon m_rating_3;      /*!< \brief "3"   */
		QIcon m_rating_cross;  /*!< \brief "Х"   */
		QIcon m_rating_smile;  /*!< \brief ":)"  */
		QIcon m_rating_plus;   /*!< \brief "+"   */
		QIcon m_rating_minus;  /*!< \brief "-"   */
		QIcon m_moderate;      /*!< \brief Бомба */

		/*!
		 * \brief Таймер для пометки сообщений как прочитанные
		 */
		QTimer m_timer;

		/*!
		 * \brief Основное контекстное меню (для основного дерева сообщений)
		 */
		QMenu* m_menu;

		QAction* m_menu_reply;                  /*!< \brief Ответить                             */
		QAction* m_menu_mark_message_as_read;   /*!< \brief Пометить сообщение как прочитанное   */
		QAction* m_menu_mark_message_as_unread; /*!< \brief Пометить сообщение как непрочитанное */
		QAction* m_menu_mark_thread_as_read;    /*!< \brief Пометить ветку как прочитанную       */
		QAction* m_menu_mark_thread_as_unread;  /*!< \brief Пометить ветку как непрочитанную     */

		/*!
		 * \brief Контекстное меню в спец-фоорумах (оценки, модерилки, исходящие сообщения)
		 */
		QMenu* m_menu_special;

		QAction* m_menu_special_edit;   /*!< \brief Изменить */
		QAction* m_menu_special_delete; /*!< \brief Удалить  */

		QAction* m_menu_special_plus_1; /*!< \brief +1 */
		QAction* m_menu_special_1;      /*!< \brief 1  */
		QAction* m_menu_special_2;      /*!< \brief 2  */
		QAction* m_menu_special_3;      /*!< \brief 3  */
		QAction* m_menu_special_smile;  /*!< \brief :) (смайл)     */
		QAction* m_menu_special_plus;   /*!< \brief + (согласен)   */
		QAction* m_menu_special_minus;  /*!< \brief - (несогласен) */

		//
		// внутренние переменные
		//

		AForumInfo m_current_forum;      /*!< \brief Текущий отображаемый форум                                     */

		QList<int> m_history;            /*!< \brief Список с ID просмотренных сообщений для навигации вперед/назад */
		int        m_history_index;      /*!< \brief Текущий индекс в списке истории                                */

		int        m_max_topic_to_show;  /*!< \brief Максимальное количество топиков для отображения                */
		int        m_mark_as_read_timer; /*!< \brief Время на пометку сообщения как прочитанного (ms)               */

		/*!
		 * \brief Дозагрузка информации о топиках, находящихся в видимой области отображения
		 */
		void scrollTopics ();

		/*!
		 * \brief Построение дерева (топика) из списка элементов
		 * \param root Родительский элемент
		 * \param list Список дочерних элементов для построения
		 */
		void buildTree (AMessageInfoGUI* root, AMessageInfoGUIPtrList* list);

		/*!
		 * \brief Раскрытие дерева элементов до всех непрочитанных сообщений
		 * \param widget_item Родительский элемент дерева для раскрытия
		 */
		void expandUnreadChild (QTreeWidgetItem* widget_item);

		/*!
                 * \brief Функция проверки установки выделения, в случае отсутствия выделения, выделяет элемент, согласно параметру select_first
                 * \param select_first Флаг выделения, если имеет значение true, то выделяется первый элемент, иначе, последний (из корневых)
                 * \return true, если есть выделение, false при отсутствии элементов
                 */
                bool checkCurrentItem (bool select_first = true);

		/*!
		 * \brief Редактирование оценки к отправке
		 * \param new_rate Новая оценка
		 */
		void changeRating (int new_rate);

		/*
		 * \brief Расширенная реализация IMessageTree::gotoNextUnreadArticle
		 * \param item Элемент дерева, относительно которого требуется вести поиск
		 */
		void gotoNextUnreadArticle (QTreeWidgetItem* current_item);

		/*
		 * \brief Пометка сообщения как прочитанного/непрочитанного
		 * \param item Элемент дерева
		 * \param is_read Флаг прочитанного
		 */
		void markItemAsRead (QTreeWidgetItem* item, bool is_read);

		/*
		 * \brief Пометка ветки как прочитанной/непрочитанной
		 * \param item Элемент дерева
		 * \param is_read Флаг прочитанного
		 * \param count Количество элементов, которые были помечены
		 * \param count Количество элементов, которые были помечены и являлись ответами мне
		 */
		void markThreadAsRead (QTreeWidgetItem* parent, bool is_read, int& count, int& count_my);

		/*
		 * \brief Пометка ветки как прочитанной/непрочитанной в хранилище
		 * \param id ID топика
		 * \param is_read Флаг прочитанного
		 * \return true, если ветка в хранилище помечена
		 */
		bool markThreadAsRead (int id, bool is_read);

		/*!
		 * \brief Загрузить сообщения к отправке
		 * \param drafts - Флаг черновиков
		 */
		void loadMessage2Send (bool drafts);

		/*!
		 * \brief Загрузить рейтинги к отправке
		 */
		void loadRating2Send ();

		/*!
		 * \brief Загрузить модерилки к отправке
		 */
		void loadModerate2Send ();

		/*!
		 * \brief Загрузить мои сообщения
		 */
		void loadMyMessages();

		/*!
		 * \brief Загрузить ответы мне
		 */
		void loadAnswers2Me();

	// IMessageTree
	public:

		void changeForum (const AForumInfo* forum_info);

		void showSource ();

		void processUrl (const QString& url);

		void gotoNextUnreadArticle ();
		void gotoNextUnreadThread  ();

		void getSelectedPath (QList<int>& path);
		bool selectByPath (const QList<int>* path);

		void gotoBackward ();
		void gotoForward  ();

		void reply ();

	// IMessageInfoGUIFactory
	private:

		AMessageInfoGUI* createItem ();

	private slots:

		/*!
		 * \brief Скроллинг (см. алгоритм работы с деревом в trac)
		 */
		void vertical_scroll (int value);

		/*!
		 * \brief Разворачивание элемента (подгрузка веток, если необходимо, см. алгоритм работы с деревом в trac)
		 */
		void expand_item (QTreeWidgetItem* item);

		/*!
		 * \brief Смена выделения (загрузка другого сообщения)
		 */
		void selection_changed ();

		/*!
		 * \brief Двойной клик (редактирование)
		 */
		void item_double_clicked (QTreeWidgetItem* item, int column);

		/*!
		 * \brief Таймер для пометки сообщения как прочитанного
		 */
		void timer_on_timer ();

		/*!
		 * \brief Запрос всплывающего меню
		 */
		void context_menu_request (const QPoint& pos);

		//
		// Основное контекстное меню
		//

		void menu_reply_triggered                  (); /*!< \brief Ответить                             */
		void menu_mark_message_as_read_triggered   (); /*!< \brief Пометить сообщение как прочитанное   */
		void menu_mark_message_as_unread_triggered (); /*!< \brief Пометить сообщение как непрочитанное */
		void menu_mark_thread_as_read_triggered    (); /*!< \brief Пометить ветку как прочитанную       */
		void menu_mark_thread_as_unread_triggered  (); /*!< \brief Пометить ветку как непрочитанную     */

		//
		// Специальное контекстное меню
		//

		void menu_special_edit_triggered   (); /*!< \brief Спецменю "Изменить" */
		void menu_special_delete_triggered (); /*!< \brief Спецменю "Удалить"  */

		void menu_special_plus_1_triggered (); /*!< \brief "+1" */
		void menu_special_1_triggered      (); /*!< \brief "1"  */
		void menu_special_2_triggered      (); /*!< \brief "2"  */
		void menu_special_3_triggered      (); /*!< \brief "3"  */
		void menu_special_smile_triggered  (); /*!< \brief ":)" (смайл)     */
		void menu_special_plus_triggered   (); /*!< \brief "+" (согласен)   */
		void menu_special_minus_triggered  (); /*!< \brief "-" (несогласен) */
};

#endif   // _avalon_message_tree_h_
