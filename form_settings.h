/*!
 * \file
 * \brief Форма настроек программы
 *
 * $Date: 2010-08-26 05:43:49 +0400 (Чтв, 26 Авг 2010) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 358 $
 * $URL: svn://opensvn.ru/avalon/trunk/form_settings.h $
 */

#ifndef _avalon_form_settings_h_
#define _avalon_form_settings_h_

#include "form_settings_ui.h"

/*!
 * \brief Форма настроек программы
 */
class FormSettings : public FormSettingsUI
{
	Q_OBJECT

	public:

		FormSettings  (QWidget* parent);
		~FormSettings ();

	private:

		/*!
		 * \brief Сохранение настроек
		 */
		void save    ();

		/*!
		 * \brief Восстановление настроек
		 */
		void restore ();

	private slots:

		/*!
		 * \brief OK
		 */
		void button_ok_clicked ();

		/*!
		 * \brief Включение / отключение прокси-сервера
		 */
		void check_use_proxy_state_changed (int state);

		/*!
		 * \brief Выбор файла для файловых хранилищ
		 */
		void button_database_file_clicked ();
        void button_database_create_clicked ();
        void text_changed_slot(const QString &path);


		/*!
		 * \brief Смена типа БД
		 */
		void combo_database_type_current_index_changed (const QString& text);
};

#endif   // _avalon_form_settings_h_
