/*!
 * \file
 * \brief Главная форма приложения
 */

#ifndef _avalon_form_main_h_
#define _avalon_form_main_h_

#include "form_main_ui.h"
#include "interfaces.h"

/*!
 * \brief Класс главной формы приложения
 */
class AFormMain :
	public AFormMainUI,
	public IFormMain
{
	Q_OBJECT;

	public:

		AFormMain  ();
		~AFormMain ();

	private:

		/*!
		 * \brief Таймер для периодической синхронизации
		 */
		QTimer m_timer;

		/*!
		 * \brief Обновление статуса по умолчанию
		 */
		void setDefaultStatus ();

		/*!
		 * \brief Получение интервала периодической синхронизации
		 * \return число ms между синхронизациями
		 */
		int synchronizeInterval ();

		/*!
		 * \brief Проверка наличия обновлений
		 */
		void checkUpdate ();

	private slots:

		//
		// обработчики меню
		//

		//
		// Меню "Файл"
		//

		void menu_file_exit_triggered (); /*!< \brief Файл / Выход */

		//
		// Меню "Вид"
		//

		void menu_view_source_triggered (); /*!< \brief Вид / Просмотр исходника */

		//
		// Меню "Перейти"
		//

		void menu_goto_next_unread_article_triggered (); /*!< \brief К следующему непрочитанному сообщению */
		void menu_goto_next_unread_thread_triggered  (); /*!< \brief К следующей непрочитанной ветке       */
		void menu_goto_next_unread_forum_triggered   (); /*!< \brief К следующему непрочитанному форуму    */
		void menu_goto_by_id_triggered               (); /*!< \brief К сообщению по ID/ссылке              */

		//
		// Меню "Сервис"
		//

		void menu_service_synchronize_triggered            (); /*!< \brief Сервис / Синхронизировать */
		void menu_service_download_triggered               (); /*!< \brief Сервис / Загрузить сообщение / ветку */
		void menu_service_new_message_triggered            (); /*!< \brief Сервис / Новое сообщение */
		void menu_service_reply_triggered                  (); /*!< \brief Сервис / Ответить */
		void menu_service_mark_all_as_read_triggered       (); /*!< \brief Сервис / Пометить все как прочитанные */
		void menu_service_mark_patrial_as_read_triggered   (); /*!< \brief Сервис / Пометить до даты как прочитанные */
		void menu_service_mark_all_as_unread_triggered     (); /*!< \brief Сервис / Пометить все как непрочитанные */
		void menu_service_mark_patrial_as_unread_triggered (); /*!< \brief Сервис / Пометить после даты как непрочитанные */
		void menu_service_subscribe_triggered              (); /*!< \brief Сервис / Подписка на форумы */
		void menu_service_settings_triggered               (); /*!< \brief Сервис / Настройки */

#ifdef AVALON_USE_ZLIB
		void menu_service_storage_compress_triggered       (); /*!< \brief Сервис / Хранилище / Упаковать   */
		void menu_service_storage_uncompress_triggered     (); /*!< \brief Сервис / Хранилище / Распаковать */
#endif

		//
		// Меню "?"
		//

		void menu_q_yandex_url_triggered     (); /*!< \brief ? / Перейти на Яндекс         */
		void menu_q_wikipedia_url_triggered  (); /*!< \brief ? / Перейти на Википедию      */
		void menu_q_google_url_triggered     (); /*!< \brief ? / Перейти на Google         */
		void menu_q_rsdn_url_triggered       (); /*!< \brief ? / Перейти на RSDN           */
		void menu_q_update_triggered         (); /*!< \brief ? / Обновление                */
		void menu_q_about_triggered          (); /*!< \brief ? / О программе               */

		//
		// Тулбар
		//

		void tool_bar_backward_triggered (); /*!< \brief "Назад"  */
		void tool_bar_forward_triggered  (); /*!< \brief "Вперед" */

		/*!
		 * \brief Обработчик таймера для периодической синхронизации с RSDN
		 */
		void timer_on_timer ();

	// IFormMain
	private:

		void showStatus (const QString& value);

		void setEnabledAction (AvalonActions actions, bool enabled);
};

#endif   // _avalon_form_main_h_
