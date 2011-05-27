/*!
 * \file
 * \brief GUI главной формы приложения
 *
 * $Date: 2011-04-07 12:00:34 +0400 (Чтв, 07 Апр 2011) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 416 $
 * $URL: svn://opensvn.ru/avalon/trunk/form_main_ui.h $
 */

#ifndef _avalon_form_main_ui_h_
#define _avalon_form_main_ui_h_

#include "forum_tree.h"
#include "message_tree.h"
#include "message_view.h"

/*!
 * \brief Класс для генерации GUI части главной формы
 */
class AFormMainUI : public QMainWindow
{
	Q_OBJECT;

	public:

		AFormMainUI ();

		virtual ~AFormMainUI ();

		/*!
		 * \brief Функция сохранения расположения и размеров элементов управления формы <при выходе>
		 */
		void save ();

		/*!
		 * \brief Функция восстановления расположения и размеров элементов управления формы <при старте>
		 */
		void restore ();

	protected:

		/*!
		 * \brief Функция отработки закрытия формы (производится вызов AFormMainUI::Save() для сохранения расположения и размеров элементов формы)
		 */
		void virtual closeEvent (QCloseEvent* event);

	protected:

		/*!
		 * \brief Главное меню
		 */
		QMenuBar* m_menubar;
		QMenu*       m_menu_file;                              /*!< \brief Файл / ... */
		QAction*        m_menu_file_exit;                      /*!< \brief Файл / Выход */
		QMenu*       m_menu_view;                              /*!< \brief Вид / ... */
		QAction*        m_menu_view_source;                    /*!< \brief Вид / Просмотр исходника */
		QMenu*       m_menu_goto;                              /*!< \brief Перейти / ... */
		QAction*        m_menu_goto_next_unread_article;       /*!< \brief Перейти / Следующая непрочитаная статья */
		QAction*        m_menu_goto_next_unread_thread;        /*!< \brief Перейти / Следующая непрочитаная ветка  */
		QAction*        m_menu_goto_next_unread_forum;         /*!< \brief Перейти / Следующий непрочитаный форум  */
		QMenu*       m_menu_service;                           /*!< \brief Сервис / ... */
		QAction*        m_menu_service_synchronize;            /*!< \brief Сервис / Синхронизировать          */
		QAction*        m_menu_service_download;               /*!< \brief Сервис / Загрузить сообщение/ветку */
		QAction*        m_menu_service_new_message;            /*!< \brief Сервис / Новое сообщение           */
		QAction*        m_menu_service_reply;                  /*!< \brief Сервис / Ответить на сообщение     */
		QAction*        m_menu_service_mark_all_as_read;       /*!< \brief Сервис / Пометить все как непрочитанные        */
		QAction*        m_menu_service_mark_patrial_as_read;   /*!< \brief Сервис / Пометить до даты как прочитанные      */
		QAction*        m_menu_service_mark_all_as_unread;     /*!< \brief Сервис / Пометить все как непрочитанные        */
		QAction*        m_menu_service_mark_patrial_as_unread; /*!< \brief Сервис / Пометить после даты как непрочитанные */
		QAction*        m_menu_service_subscribe;              /*!< \brief Сервис / Подписка на форумы                    */
		QAction*        m_menu_service_settings;               /*!< \brief Сервис / Настройка                             */
#ifdef AVALON_USE_ZLIB
		QMenu*          m_menu_service_storage;                /*!< \brief Сервис / Хранилище                             */
		QAction*           m_menu_service_storage_compress;    /*!< \brief Сервис / Хранилище / Упаковать                 */
		QAction*           m_menu_service_storage_uncompress;  /*!< \brief Сервис / Хранилище / Распаковать               */
#endif
		QMenu*       m_menu_q;                                 /*!< \brief ? / ... */
		QAction*        m_menu_q_yandex_url;                   /*!< \brief ? / URL Яндекс          */
		QAction*        m_menu_q_wikipedia_url;                /*!< \brief ? / URL Википедия       */
		QAction*        m_menu_q_google_url;                   /*!< \brief ? / URL Google          */
		QAction*        m_menu_q_rsdn_url;                     /*!< \brief ? / URL RSDN            */
		QAction*        m_menu_q_update;                       /*!< \brief ? / Проверка обновлений */
		QAction*        m_menu_q_about;                        /*!< \brief ? / О программе         */

		/*!
		 * \brief Тулбар
		 */
		QToolBar* m_tool_bar;
		QAction*     m_tool_bar_synchronize; /*!< \brief Синхронизировать */
		QAction*     m_tool_bar_new_message; /*!< \brief Новое сообщение  */
		QAction*     m_tool_bar_backward;    /*!< \brief Назад            */
		QAction*     m_tool_bar_forward;     /*!< \brief Вперед           */

		/*!
		 * \brief Центральный вертикальный сплиттер, разделяющий дерево форумов и дерево сообщений с областью просмотра)
		 */
		QSplitter*    m_splitter;
		AForumTree*      m_forum_tree;      /*!< \brief Дерево подписаных форумов */
		QSplitter*       m_splitter_right;  /*!< \brief Правый горизонтальный сплиттер, разделяющий дерево сообщений и отображение сообщения */
		AMessageTree*       m_message_tree; /*!< \brief Дерево статей/сообщений */
		AMessageView*       m_message_view; /*!< \brief Виджет для отображения сообщения */

		/*!
		 * \brief Статусбар
		 */
		QStatusBar* m_status_bar;
};

#endif   // _avalon_form_main_ui_h_
