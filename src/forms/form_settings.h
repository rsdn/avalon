/*!
 * \file
 * \brief Форма настроек программы
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

		/*!
		 * \brief Создание базы SQLite
		 */
		void createSQLiteDatabase();

		/*!
		 * \brief Создание базы MySQL
		 */
		void createMySQLDatabase();

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

		/*!
		 * \brief Выбор файла для создания нового хранилища
		 */
		void button_database_create_clicked ();

		/*!
		 * \brief Смена типа БД
		 */
		void combo_database_type_current_index_changed (const QString& text);
};

#endif   // _avalon_form_settings_h_
