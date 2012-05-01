//----------------------------------------------------------------------------------------------
// $Date: 2010-12-04 20:27:36 +0300 (Сбт, 04 Дек 2010) $
// $Author: antonbatenev.ya.ru $
// $Revision: 372 $
// $URL: svn://opensvn.ru/avalon/trunk/form_settings_ui.cpp $
//----------------------------------------------------------------------------------------------
#include "form_settings_ui.h"
//----------------------------------------------------------------------------------------------
#include "colorer.h"
//----------------------------------------------------------------------------------------------

FormSettingsUI::FormSettingsUI (QWidget* parent) : QDialog (parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint
#if QT_VERSION >= 0x040500
 | Qt::WindowCloseButtonHint
#endif
)
{
	setWindowTitle(QString::fromUtf8("настройки"));

	QIcon icon;
	icon.addFile(":/icons/settings16.png",  QSize(16, 16));
	icon.addFile(":/icons/settings22.png",  QSize(22, 22));
	icon.addFile(":/icons/settings24.png",  QSize(24, 24));
	icon.addFile(":/icons/settings32.png",  QSize(32, 32));
	icon.addFile(":/icons/settings48.png",  QSize(48, 48));
	icon.addFile(":/icons/settings64.png",  QSize(64, 64));
	icon.addFile(":/icons/settings128.png", QSize(128, 128));
	setWindowIcon(icon);

	resize(549, 366);

	//
	// таб
	//

	m_layout = new QHBoxLayout(this);

	m_tab = new QTabWidget(this);
	m_layout->addWidget(m_tab);

	m_network_settings = new QWidget();
	m_tab->addTab(m_network_settings, QString::fromUtf8("сеть"));

	m_storage_settings = new QWidget();
	m_tab->addTab(m_storage_settings, QString::fromUtf8("хранилище"));

	m_ui_settings = new QWidget();
	m_tab->addTab(m_ui_settings, QString::fromUtf8("интерфейс"));

	//
	// таб настроек сети
	//

	m_layout_network_spacer = new QVBoxLayout(m_network_settings);

	m_layout_network = new QHBoxLayout();
	m_layout_network_spacer->addLayout(m_layout_network);

	m_network_spacer = new QSpacerItem(1, height(), QSizePolicy::Maximum, QSizePolicy::Expanding);
	m_layout_network_spacer->addItem(m_network_spacer);

	//
	// подписи к полям (сеть)
	//

	m_layout_label_network  = new QVBoxLayout();
	m_layout_network->addLayout(m_layout_label_network);

	m_label_rsdn_host = new QLabel(this);
	m_label_rsdn_host->setText(QString::fromUtf8("хост"));
	m_layout_label_network->addWidget(m_label_rsdn_host);

	m_label_rsdn_port = new QLabel(this);
	m_label_rsdn_port->setText(QString::fromUtf8("порт"));
	m_layout_label_network->addWidget(m_label_rsdn_port);

	m_label_rsdn_login = new QLabel(this);
	m_label_rsdn_login->setText(QString::fromUtf8("логин"));
	m_layout_label_network->addWidget(m_label_rsdn_login);

	m_label_rsdn_password = new QLabel(this);
	m_label_rsdn_password->setText(QString::fromUtf8("пароль"));
	m_layout_label_network->addWidget(m_label_rsdn_password);

	m_label_use_proxy = new QLabel(this);
	m_label_use_proxy->setText(QString::fromUtf8(""));
	m_layout_label_network->addWidget(m_label_use_proxy);

	m_label_proxy_type = new QLabel(this);
	m_label_proxy_type->setText(QString::fromUtf8("тип"));
	m_layout_label_network->addWidget(m_label_proxy_type);

	m_label_proxy_host = new QLabel(this);
	m_label_proxy_host->setText(QString::fromUtf8("хост"));
	m_layout_label_network->addWidget(m_label_proxy_host);

	m_label_proxy_port = new QLabel(this);
	m_label_proxy_port->setText(QString::fromUtf8("порт"));
	m_layout_label_network->addWidget(m_label_proxy_port);

	m_label_proxy_login = new QLabel(this);
	m_label_proxy_login->setText(QString::fromUtf8("логин"));
	m_layout_label_network->addWidget(m_label_proxy_login);

	m_label_proxy_password = new QLabel(this);
	m_label_proxy_password->setText(QString::fromUtf8("пароль"));
	m_layout_label_network->addWidget(m_label_proxy_password);

	m_label_use_proxy_webkit = new QLabel(this);
	m_label_use_proxy_webkit->setText(QString::fromUtf8(""));
	m_layout_label_network->addWidget(m_label_use_proxy_webkit);

	//
	// поля (сеть)
	//

	m_layout_text_network = new QVBoxLayout();
	m_layout_network->addLayout(m_layout_text_network);

	m_text_rsdn_host = new QLineEdit(this);
	m_layout_text_network->addWidget(m_text_rsdn_host);

	m_text_rsdn_port = new QLineEdit(this);
	m_layout_text_network->addWidget(m_text_rsdn_port);

	m_text_rsdn_login = new QLineEdit(this);
	m_layout_text_network->addWidget(m_text_rsdn_login);

	m_text_rsdn_password = new QLineEdit(this);
	m_text_rsdn_password->setEchoMode(QLineEdit::Password);
	m_layout_text_network->addWidget(m_text_rsdn_password);

	m_check_use_proxy = new QCheckBox(this);
	m_check_use_proxy->setText(QString::fromUtf8("использовать прокси сервер"));
	m_layout_text_network->addWidget(m_check_use_proxy);

	m_combo_proxy_type = new QComboBox(this);
	m_combo_proxy_type->insertItem(0, "CONNECT");
	m_combo_proxy_type->insertItem(0, "SOCKS 5");
	m_combo_proxy_type->insertItem(0, "HTTP");
	// TODO: default прокси для Windows и Mac
	m_layout_text_network->addWidget(m_combo_proxy_type);

	m_text_proxy_host = new QLineEdit(this);
	m_layout_text_network->addWidget(m_text_proxy_host);

	m_text_proxy_port = new QLineEdit(this);
	m_layout_text_network->addWidget(m_text_proxy_port);

	m_text_proxy_login = new QLineEdit(this);
	m_layout_text_network->addWidget(m_text_proxy_login);

	m_text_proxy_password = new QLineEdit(this);
	m_text_proxy_password->setEchoMode(QLineEdit::Password);
	m_layout_text_network->addWidget(m_text_proxy_password);

	m_check_use_proxy_webkit = new QCheckBox(this);
	m_check_use_proxy_webkit->setText(QString::fromUtf8("использовать прокси при просмотре сообщений"));
	m_layout_text_network->addWidget(m_check_use_proxy_webkit);

	//
	// кнопки (сеть)
	//

	m_layout_button_network = new QVBoxLayout();
	m_layout_network->addLayout(m_layout_button_network);

	m_button_ok_network = new QPushButton(this);
	m_button_ok_network->setText(QString::fromUtf8("OK"));
	m_button_ok_network->setShortcut(QKeySequence("Return"));
	m_button_ok_network->setDefault(true);
	m_layout_button_network->addWidget(m_button_ok_network);

	m_button_cancel_network = new QPushButton(this);
	m_button_cancel_network->setText(QString::fromUtf8("Отмена"));
	m_button_cancel_network->setShortcut(QKeySequence("Esc"));
	m_layout_button_network->addWidget(m_button_cancel_network);

	m_spacer_button_network = new QSpacerItem(1, 1, QSizePolicy::Maximum, QSizePolicy::Expanding);
	m_layout_button_network->addItem(m_spacer_button_network);

	//
	// таб настроек хранилища
	//

	m_layout_storage_spacer = new QVBoxLayout(m_storage_settings);

	m_layout_storage = new QHBoxLayout();
	m_layout_storage_spacer->addLayout(m_layout_storage);

	m_storage_spacer = new QSpacerItem(1, height(), QSizePolicy::Maximum, QSizePolicy::Expanding);
	m_layout_storage_spacer->addItem(m_storage_spacer);

	//
	// подписи к полям (хранилище)
	//

	m_layout_label_storage  = new QVBoxLayout();
	m_layout_storage->addLayout(m_layout_label_storage);

	m_label_database_type = new QLabel(this);
	m_label_database_type->setText(QString::fromUtf8("тип"));
	m_layout_label_storage->addWidget(m_label_database_type);

	m_label_database_host = new QLabel(this);
	m_label_database_host->setText(QString::fromUtf8("хост"));
	m_layout_label_storage->addWidget(m_label_database_host);

	m_label_database_port = new QLabel(this);
	m_label_database_port->setText(QString::fromUtf8("порт / сокет"));
	m_layout_label_storage->addWidget(m_label_database_port);

	m_label_database_name = new QLabel(this);
	m_label_database_name->setText(QString::fromUtf8("схема"));
	m_layout_label_storage->addWidget(m_label_database_name);

	m_label_database_login = new QLabel(this);
	m_label_database_login->setText(QString::fromUtf8("логин"));
	m_layout_label_storage->addWidget(m_label_database_login);

	m_label_database_password = new QLabel(this);
	m_label_database_password->setText(QString::fromUtf8("пароль"));
	m_layout_label_storage->addWidget(m_label_database_password);

	m_label_database_file = new QLabel(this);
	m_label_database_file->setText(QString::fromUtf8("файл"));
	m_layout_label_storage->addWidget(m_label_database_file);

	//
	// поля (хранилище)
	//

	m_layout_text_storage = new QVBoxLayout();
	m_layout_storage->addLayout(m_layout_text_storage);

	m_combo_database_type = new QComboBox(this);
	m_combo_database_type->insertItem(0, "MySQL");
	m_combo_database_type->insertItem(0, "SQLite");
	m_layout_text_storage->addWidget(m_combo_database_type);

	m_text_database_host = new QLineEdit(this);
	m_layout_text_storage->addWidget(m_text_database_host);

	m_text_database_port = new QLineEdit(this);
	m_layout_text_storage->addWidget(m_text_database_port);

	m_text_database_name = new QLineEdit(this);
	m_layout_text_storage->addWidget(m_text_database_name);

	m_text_database_login = new QLineEdit(this);
	m_layout_text_storage->addWidget(m_text_database_login);

	m_text_database_password = new QLineEdit(this);
	m_text_database_password->setEchoMode(QLineEdit::Password);
	m_layout_text_storage->addWidget(m_text_database_password);

	m_layout_database_file = new QHBoxLayout();

	m_text_database_file = new QLineEdit(this);
	m_layout_database_file->addWidget(m_text_database_file);

	m_button_database_file = new QPushButton(this);
	m_button_database_file->setFlat(true);
	m_button_database_file->setIcon(QIcon(":/icons/open16.png"));
	m_button_database_file->setMaximumSize(m_text_database_file->height(), m_text_database_file->height());
	m_layout_database_file->addWidget(m_button_database_file);

	m_layout_text_storage->addLayout(m_layout_database_file);

	//
	// кнопки (хранилище)
	//

	m_layout_button_storage = new QVBoxLayout();
	m_layout_storage->addLayout(m_layout_button_storage);

	m_button_ok_storage = new QPushButton(this);
	m_button_ok_storage->setText(QString::fromUtf8("OK"));
	m_button_ok_storage->setShortcut(QKeySequence("Return"));
	m_button_ok_storage->setDefault(true);
	m_layout_button_storage->addWidget(m_button_ok_storage);

	m_button_cancel_storage = new QPushButton(this);
	m_button_cancel_storage->setText(QString::fromUtf8("Отмена"));
	m_button_cancel_storage->setShortcut(QKeySequence("Esc"));
	m_layout_button_storage->addWidget(m_button_cancel_storage);

	m_spacer_button_storage = new QSpacerItem(1, 1, QSizePolicy::Maximum, QSizePolicy::Expanding);
	m_layout_button_storage->addItem(m_spacer_button_storage);

	//
	// таб настроек интерфейса
	//

	m_layout_ui_spacer = new QVBoxLayout(m_ui_settings);

	m_layout_ui = new QHBoxLayout();

	m_layout_ui_spacer->addLayout(m_layout_ui);

	m_ui_spacer = new QSpacerItem(1, height(), QSizePolicy::Maximum, QSizePolicy::Expanding);
	m_layout_ui_spacer->addItem(m_ui_spacer);

	//
	// подписи к полям (интерфейс)
	//

	m_layout_label_ui  = new QVBoxLayout();
	m_layout_ui->addLayout(m_layout_label_ui);

	m_label_mark_as_read_timer = new QLabel(this);
	m_label_mark_as_read_timer->setText(QString::fromUtf8("пометить как прочитанное (ms)"));
	m_layout_label_ui->addWidget(m_label_mark_as_read_timer);

	m_label_max_topic_to_show = new QLabel(this);
	m_label_max_topic_to_show->setText(QString::fromUtf8("максимальное количество тем"));
	m_layout_label_ui->addWidget(m_label_max_topic_to_show);

	m_label_synchronize_interval = new QLabel(this);
	m_label_synchronize_interval->setText(QString::fromUtf8("интервал автообновления (минут)"));
	m_layout_label_ui->addWidget(m_label_synchronize_interval);

	m_label_colorer = new QLabel(this);
	m_label_colorer->setText(QString::fromUtf8("подсветка синтаксиса"));
	m_layout_label_ui->addWidget(m_label_colorer);

	m_label_tagline = new QLabel(this);
	m_label_tagline->setText(QString::fromUtf8("tagline"));
	m_layout_label_ui->addWidget(m_label_tagline);

#ifdef AVALON_USE_ZLIB
	m_label_compression = new QLabel(this);
	m_label_compression->setText(QString::fromUtf8(""));
	m_layout_label_ui->addWidget(m_label_compression);
#endif

#ifdef AVALON_USE_ASPELL
	m_label_spellchecker = new QLabel(this);
	m_label_spellchecker->setText(QString::fromUtf8(""));
	m_layout_label_ui->addWidget(m_label_spellchecker);
#endif

	//
	// поля (интерфейс)
	//

	m_layout_text_ui = new QVBoxLayout();
	m_layout_ui->addLayout(m_layout_text_ui);

	m_text_mark_as_read_timer = new QLineEdit(this);
	m_timer_validator = new QIntValidator(0, INT_MAX, m_text_mark_as_read_timer);
	m_text_mark_as_read_timer->setValidator(m_timer_validator);
	m_text_mark_as_read_timer->setAlignment(Qt::AlignRight);
	m_text_mark_as_read_timer->setText("1500");
	m_layout_text_ui->addWidget(m_text_mark_as_read_timer);

	m_text_max_topic_to_show = new QLineEdit(this);
	m_max_topic_show_validator = new QIntValidator(0, INT_MAX, m_text_mark_as_read_timer);
	m_text_max_topic_to_show->setValidator(m_max_topic_show_validator);
	m_text_max_topic_to_show->setAlignment(Qt::AlignRight);
	m_text_max_topic_to_show->setText("0");
	m_layout_text_ui->addWidget(m_text_max_topic_to_show);

	m_text_synchronize_interval = new QLineEdit(this);
	m_update_interval_validator = new QIntValidator(0, INT_MAX, m_text_mark_as_read_timer);
	m_text_synchronize_interval->setValidator(m_update_interval_validator);
	m_text_synchronize_interval->setAlignment(Qt::AlignRight);
	m_text_synchronize_interval->setText("0");
	m_layout_text_ui->addWidget(m_text_synchronize_interval);

	m_combo_colorer = new QComboBox(this);
	m_combo_colorer->insertItem(0, COLORER_ENSCRIPT_NAME);
	m_combo_colorer->insertItem(1, COLORER_COLORER_TAKE5_NAME);
	m_combo_colorer->insertItem(2, COLORER_GNU_SOURCE_HIGHLIGHT_NAME);
	m_combo_colorer->insertItem(3, COLORER_HIGHLIGHT_JS_NAME);
	m_layout_text_ui->addWidget(m_combo_colorer);

	m_text_tagline = new QLineEdit(this);
	m_text_tagline->setText("%%version%%");
	m_layout_text_ui->addWidget(m_text_tagline);

#ifdef AVALON_USE_ZLIB
	m_check_compression = new QCheckBox(this);
	m_check_compression->setText(QString::fromUtf8("сжатие тел сообщений"));
	m_layout_text_ui->addWidget(m_check_compression);
#endif

#ifdef AVALON_USE_ASPELL
	m_check_spellchecker = new QCheckBox(this);
	m_check_spellchecker->setText(QString::fromUtf8("проверка орфографии"));
	m_layout_text_ui->addWidget(m_check_spellchecker);
#endif

	//
	// кнопки (интерфейс)
	//

	m_layout_button_ui = new QVBoxLayout();
	m_layout_ui->addLayout(m_layout_button_ui);

	m_button_ok_ui = new QPushButton(this);
	m_button_ok_ui->setText(QString::fromUtf8("OK"));
	m_button_ok_ui->setShortcut(QKeySequence("Return"));
	m_button_ok_ui->setDefault(true);
	m_layout_button_ui->addWidget(m_button_ok_ui);

	m_button_cancel_ui = new QPushButton(this);
	m_button_cancel_ui->setText(QString::fromUtf8("Отмена"));
	m_button_cancel_ui->setShortcut(QKeySequence("Esc"));
	m_layout_button_ui->addWidget(m_button_cancel_ui);

	m_spacer_button_ui = new QSpacerItem(1, 1, QSizePolicy::Maximum, QSizePolicy::Expanding);
	m_layout_button_ui->addItem(m_spacer_button_ui);
}
//----------------------------------------------------------------------------------------------

FormSettingsUI::~FormSettingsUI ()
{
}
//----------------------------------------------------------------------------------------------
