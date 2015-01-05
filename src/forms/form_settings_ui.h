/*!
 * \file
 * \brief GUI формы настроек программы
 */

#ifndef _avalon_form_settings_ui_h_
#define _avalon_form_settings_ui_h_

#include "sysheaders.h"

/*!
 * \brief GUI формы настроек программы
 */
class FormSettingsUI : public QDialog
{
	Q_OBJECT

	public:

		FormSettingsUI  (QWidget* parent);
		~FormSettingsUI ();

	protected:

		QHBoxLayout* m_layout;                                        /*!< \brief Layout для таба                         */
		   QTabWidget* m_tab;                                         /*!< \brief Таб                                     */
		      QWidget* m_network_settings;                            /*!< \brief Виджет таба для настроек сети           */
		         QVBoxLayout* m_layout_network_spacer;                /*!< \brief Layout для выравнивания по вертикали    */
		            QHBoxLayout* m_layout_network;                    /*!< \brief Общий layout для настроек сети          */
		               QVBoxLayout* m_layout_label_network;           /*!< \brief Layout для надписей                     */
		                  QLabel* m_label_rsdn_proto;                 /*!< \brief Протокол доступа к веб-сервису RSDN     */
		                  QLabel* m_label_rsdn_login;                 /*!< \brief Логин на RSDN                           */
		                  QLabel* m_label_rsdn_password;              /*!< \brief Пароль на RSDN                          */
		                  QLabel* m_label_use_proxy;                  /*!< \brief Флаг использования прокси               */
		                  QLabel* m_label_proxy_type;                 /*!< \brief Тип прокси-сервера                      */
		                  QLabel* m_label_proxy_host;                 /*!< \brief Хост прокси-сервеа                      */
		                  QLabel* m_label_proxy_port;                 /*!< \brief Порт прокси-сервера                     */
		                  QLabel* m_label_proxy_login;                /*!< \brief Пользователь прокси-сервера             */
		                  QLabel* m_label_proxy_password;             /*!< \brief Пароль прокси-сервера                   */
		                  QLabel* m_label_use_proxy_webkit;           /*!< \brief Флаг использования прокси для webkit    */
		               QVBoxLayout* m_layout_text_network;            /*!< \brief Layout для текстовых полей              */
		                  QComboBox* m_combo_rsdn_proto;              /*!< \brief Протокол доступа к веб-сервису RSDN     */
		                  QLineEdit* m_text_rsdn_login;               /*!< \brief Логин на RSDN                           */
		                  QLineEdit* m_text_rsdn_password;            /*!< \brief Пароль на RSDN                          */
		                  QCheckBox* m_check_use_proxy;               /*!< \brief Флаг использования прокси               */
		                  QComboBox* m_combo_proxy_type;              /*!< \brief Тип прокси-сервера                      */
		                  QLineEdit* m_text_proxy_host;               /*!< \brief Хост прокси-сервера                     */
		                  QLineEdit* m_text_proxy_port;               /*!< \brief Порт прокси-сервера                     */
		                  QLineEdit* m_text_proxy_login;              /*!< \brief Пользователь прокси-сервера             */
		                  QLineEdit* m_text_proxy_password;           /*!< \brief Пароль прокси-сервера                   */
		                  QCheckBox* m_check_use_proxy_webkit;        /*!< \brief Флаг использования прокси для webkit    */
		               QVBoxLayout* m_layout_button_network;          /*!< \brief Layout для кнопок                       */
		                  QPushButton* m_button_ok_network;           /*!< \brief OK                                      */
		                  QPushButton* m_button_cancel_network;       /*!< \brief Отмена                                  */
		                  QSpacerItem* m_spacer_button_network;       /*!< \brief Выравнивание кнопок вверх               */
		            QSpacerItem* m_network_spacer;                    /*!< \brief Выравнивание всего вверх                */
		      QWidget* m_storage_settings;                            /*!< \brief Виджет таба для настроек хранилища      */
		         QVBoxLayout* m_layout_storage_spacer;                /*!< \brief Layout для выравнивания по вертикали    */
		            QHBoxLayout* m_layout_storage;                    /*!< \brief Общий layout для настроек хранилища     */
		               QVBoxLayout* m_layout_label_storage;           /*!< \brief Layout для надписей                     */
		                  QLabel* m_label_database_type;              /*!< \brief Тип хранилища                           */
		                  QLabel* m_label_database_host;              /*!< \brief Хост БД                                 */
		                  QLabel* m_label_database_port;              /*!< \brief Порт/сокет БД                           */
		                  QLabel* m_label_database_name;              /*!< \brief Имя базы                                */
		                  QLabel* m_label_database_login;             /*!< \brief Логин БД                                */
		                  QLabel* m_label_database_password;          /*!< \brief Пароль БД                               */
		                  QLabel* m_label_database_file;              /*!< \brief Файл базы (для файловых БД)             */
		               QVBoxLayout* m_layout_text_storage;            /*!< \brief Layout для текстовых полей              */
		                  QComboBox* m_combo_database_type;           /*!< \brief Тип базы хранилища                      */
		                  QLineEdit* m_text_database_host;            /*!< \brief Хост БД                                 */
		                  QLineEdit* m_text_database_port;            /*!< \brief Порт/сокет БД                           */
		                  QLineEdit* m_text_database_name;            /*!< \brief Имя базы                                */
		                  QLineEdit* m_text_database_login;           /*!< \brief Логин БД                                */
		                  QLineEdit* m_text_database_password;        /*!< \brief Пароль БД                               */
		                  QHBoxLayout* m_layout_database_file;        /*!< \brief Layout для поля выбора файла            */
		                     QLineEdit*   m_text_database_file;       /*!< \brief Файл базы (для файловых БД)             */
		                     QPushButton* m_button_database_file;     /*!< \brief Кнопка выбора файла базы                */
		                     QPushButton* m_button_database_create;   /*!< \brief Кнопка создания файла базы              */
		               QVBoxLayout* m_layout_button_storage;          /*!< \brief Layout для кнопок                       */
		                  QPushButton* m_button_ok_storage;           /*!< \brief OK                                      */
		                  QPushButton* m_button_cancel_storage;       /*!< \brief Отмена                                  */
		                  QSpacerItem* m_spacer_button_storage;       /*!< \brief Выравнивание кнопок вверх               */
		            QSpacerItem* m_storage_spacer;                    /*!< \brief Выравнивание всего вверх                */
		      QWidget* m_ui_settings;                                 /*!< \brief Виджет таба для настроек интерфейса     */
		         QVBoxLayout* m_layout_ui_spacer;                     /*!< \brief Layout для выравнивания по вертикали    */
		            QHBoxLayout* m_layout_ui;                         /*!< \brief Общий layout для настроек интерфейса    */
		               QVBoxLayout* m_layout_label_ui;                /*!< \brief Layout для надписей                     */
		                  QLabel* m_label_mark_as_read_timer;         /*!< \brief Время пометки как прочитанное           */
		                  QLabel* m_label_max_topic_to_show;          /*!< \brief Максимальное количество топиков         */
		                  QLabel* m_label_synchronize_interval;       /*!< \brief Интервал автообновления                 */
		                  QLabel* m_label_tagline;                    /*!< \brief Tagline                                 */
#ifdef AVALON_USE_ZLIB
		                  QLabel* m_label_compression;                /*!< \brief Включить сжатие тел сообщений           */
#endif
#ifdef AVALON_USE_ASPELL
		                  QLabel* m_label_spellchecker;               /*!< \brief Включить проверку орфографии            */
#endif
		               QVBoxLayout* m_layout_text_ui;                 /*!< \brief Layout для текстовых полей              */
		                  QLineEdit* m_text_mark_as_read_timer;       /*!< \brief Время пометки как прочитанное           */
		                  QLineEdit* m_text_max_topic_to_show;        /*!< \brief Максимальное количество топиков         */
		                  QLineEdit* m_text_synchronize_interval;     /*!< \brief Интервал автообновления                 */
		                  QLineEdit* m_text_tagline;                  /*!< \brief Tagline                                 */
#ifdef AVALON_USE_ZLIB
		                  QCheckBox* m_check_compression;             /*!< \brief Включить сжатие тел сообщений           */
#endif
#ifdef AVALON_USE_ASPELL
		                  QCheckBox* m_check_spellchecker;            /*!< \brief Включить проверку орфографии            */
#endif
		               QVBoxLayout* m_layout_button_ui;               /*!< \brief Layout для кнопок                       */
		                  QPushButton* m_button_ok_ui;                /*!< \brief OK                                      */
		                  QPushButton* m_button_cancel_ui;            /*!< \brief Отмена                                  */
		                  QSpacerItem* m_spacer_button_ui;            /*!< \brief Выравнивание кнопок вверх               */
		            QSpacerItem* m_ui_spacer;                         /*!< \brief Выравнивание всего вверх                */

	private:

		QIntValidator* m_timer_validator;             /*!< \brief Валидатор значения в поле "время пометки как прочитанное" */
		QIntValidator* m_max_topic_show_validator;    /*!< \brief Валидатор значения в поле "Максимальное кол-во топиков"   */
		QIntValidator* m_update_interval_validator;   /*!< \brief Валидатор значения в поле "Интервал автообновления"       */
};

#endif   // _avalon_form_settings_ui_h_
