#include "form_settings.h"
//----------------------------------------------------------------------------------------------
#include "global.h"
#include "storage/storage_factory.h"
//----------------------------------------------------------------------------------------------

FormSettings::FormSettings (QWidget* parent) : FormSettingsUI (parent)
{
	connect(m_button_cancel_network, SIGNAL(clicked()), this, SLOT(reject()));
	connect(m_button_ok_network,     SIGNAL(clicked()), this, SLOT(button_ok_clicked()));

	connect(m_button_cancel_storage, SIGNAL(clicked()), this, SLOT(reject()));
	connect(m_button_ok_storage,     SIGNAL(clicked()), this, SLOT(button_ok_clicked()));

	connect(m_button_cancel_ui, SIGNAL(clicked()), this, SLOT(reject()));
	connect(m_button_ok_ui,     SIGNAL(clicked()), this, SLOT(button_ok_clicked()));

	connect(m_button_cancel_other, SIGNAL(clicked()), this, SLOT(reject()));
	connect(m_button_ok_other,     SIGNAL(clicked()), this, SLOT(button_ok_clicked()));

	connect(m_check_use_proxy, SIGNAL(stateChanged(int)), this, SLOT(check_use_proxy_state_changed(int)));

	connect(m_combo_database_type, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(combo_database_type_current_index_changed(const QString&)));

	connect(m_button_database_file, SIGNAL(clicked()), this, SLOT(button_database_file_clicked()));

	connect(m_button_database_create, SIGNAL(clicked()), this, SLOT(button_database_create_clicked()));

	restore();
}
//----------------------------------------------------------------------------------------------

FormSettings::~FormSettings ()
{
}
//----------------------------------------------------------------------------------------------

void FormSettings::button_ok_clicked ()
{
	save();

	accept();
}
//----------------------------------------------------------------------------------------------

void FormSettings::button_database_file_clicked ()
{
	QFileDialog dialog(this, QString::fromUtf8("выбор файла хранилища"), m_text_database_file->text());

	if (dialog.exec() == QDialog::Accepted)
		m_text_database_file->setText(dialog.selectedFiles()[0]);
}
//----------------------------------------------------------------------------------------------

void FormSettings::button_database_create_clicked ()
{
	QSettings settings;

	QString old_value = settings.value("sqlite/file", QDir::homePath() + "/avalon/avalon.db").toString();

	QString new_value = QFileDialog::getSaveFileName(this, QString::fromUtf8("Имя файла данных"), m_text_database_file->text());

	if (new_value.length() > 0)
	{
		if (QFile::exists(new_value) == true)
			if (QFile::resize(new_value, 0) == false)
			{
				QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Ошибка записи в файл"));
				return;
			}

		// хранилище будет получать настройки из файла настроек
		settings.setValue("sqlite/file", new_value);

		// получение хранилища
		std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

		if (storage.get() == NULL)
		{
			settings.setValue("sqlite/file", old_value);
			QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
			return;
		}

		// создание базы
		if (storage->createDatabase() == false)
		{
			settings.setValue("sqlite/file", old_value);
			storage->showError(this);
			return;
		}

		// в настройках пока сохранено старое значение на случай нажатия отмены
		settings.setValue("sqlite/file", old_value);

		// замена текста на имя файла вновь созданной базы
		m_text_database_file->setText(new_value);
	}
}
//----------------------------------------------------------------------------------------------

void FormSettings::check_use_proxy_state_changed (int state)
{
	bool e = false;

	if (state == Qt::Checked)
		e = true;

	m_combo_proxy_type->setEnabled(e);
	m_text_proxy_host->setEnabled(e);
	m_text_proxy_port->setEnabled(e);
	m_text_proxy_login->setEnabled(e);
	m_text_proxy_password->setEnabled(e);
	m_check_use_proxy_webkit->setEnabled(e);
}
//----------------------------------------------------------------------------------------------

void FormSettings::combo_database_type_current_index_changed (const QString& text)
{
	bool e = false;

	if (text == "MySQL")
		e = true;

	m_text_database_host->setEnabled(e);
	m_text_database_port->setEnabled(e);
	m_text_database_name->setEnabled(e);
	m_text_database_login->setEnabled(e);
	m_text_database_password->setEnabled(e);
	m_text_database_file->setEnabled(!e);
	m_button_database_file->setEnabled(!e);
	m_button_database_create->setEnabled(!e);
}
//----------------------------------------------------------------------------------------------

void FormSettings::save ()
{
	QSettings settings;

	//
	// сеть
	//

	settings.setValue("rsdn/host",     m_text_rsdn_host->text());
	settings.setValue("rsdn/port",     m_text_rsdn_port->text());
	settings.setValue("rsdn/login",    m_text_rsdn_login->text());
	settings.setValue("rsdn/password", m_text_rsdn_password->text());

	settings.setValue("proxy/enabled",        (m_check_use_proxy->checkState() == Qt::Checked ? 1 : 0));
	settings.setValue("proxy/type",           m_combo_proxy_type->currentText());
	settings.setValue("proxy/host",           m_text_proxy_host->text());
	settings.setValue("proxy/port",           m_text_proxy_port->text());
	settings.setValue("proxy/login",          m_text_proxy_login->text());
	settings.setValue("proxy/password",       m_text_proxy_password->text());
	settings.setValue("proxy/enabled_webkit", (m_check_use_proxy_webkit->checkState() == Qt::Checked ? 1 : 0));

	//
	// хранилище
	//

	settings.setValue("storage/type", m_combo_database_type->currentText());

	settings.setValue("mysql/host",     m_text_database_host->text());
	settings.setValue("mysql/port",     m_text_database_port->text());
	settings.setValue("mysql/name",     m_text_database_name->text());
	settings.setValue("mysql/login",    m_text_database_login->text());
	settings.setValue("mysql/password", m_text_database_password->text());

	settings.setValue("sqlite/file", m_text_database_file->text());

	//
	// интерфейс
	//

	settings.setValue("ui/mark_as_read_timer",   m_text_mark_as_read_timer->text());
	settings.setValue("ui/max_topic_to_show",    m_text_max_topic_to_show->text());
	settings.setValue("ui/synchronize_interval", m_text_synchronize_interval->text());
	settings.setValue("ui/colorer",              m_combo_colorer->currentText());
	settings.setValue("ui/tagline",              m_text_tagline->text());

#ifdef AVALON_USE_ZLIB
	settings.setValue("storage/compression", (m_check_compression->checkState() == Qt::Checked ? 1 : 0));
#endif

#ifdef AVALON_USE_ASPELL
	settings.setValue("ui/spellchecker", (m_check_spellchecker->checkState() == Qt::Checked ? 1 : 0));
#endif

	QtMsgType logLevel = (QtMsgType)m_combo_logging_level->currentIndex();
	settings.setValue("other/logging_level", logLevel);
	g_logger.setDebugLevel(logLevel);
}
//----------------------------------------------------------------------------------------------

void FormSettings::restore ()
{
	QSettings settings;

	//
	// сеть
	//

	m_text_rsdn_host->setText     (settings.value("rsdn/host",     "www.rsdn.ru").toString());
	m_text_rsdn_port->setText     (settings.value("rsdn/port",     "80"         ).toString());
	m_text_rsdn_login->setText    (settings.value("rsdn/login",    ""           ).toString());
	m_text_rsdn_password->setText (settings.value("rsdn/password", ""           ).toString());

	bool use_proxy = settings.value("proxy/enabled", false).toInt();

	if (use_proxy == true)
		m_check_use_proxy->setCheckState(Qt::Checked);
	else
		m_check_use_proxy->setCheckState(Qt::Unchecked);

	QString proxy_type = settings.value("proxy/type", "HTTP").toString();

	int idx = m_combo_proxy_type->findText(proxy_type);

	if (idx != -1)
		m_combo_proxy_type->setCurrentIndex(idx);
	else
		m_combo_proxy_type->setCurrentIndex(0);

	check_use_proxy_state_changed(m_check_use_proxy->checkState());

	m_text_proxy_host->setText     (settings.value("proxy/host",     "").toString());
	m_text_proxy_port->setText     (settings.value("proxy/port",     "").toString());
	m_text_proxy_login->setText    (settings.value("proxy/login",    "").toString());
	m_text_proxy_password->setText (settings.value("proxy/password", "").toString());

	bool use_proxy_webkit = settings.value("proxy/enabled_webkit", use_proxy).toInt();

	if (use_proxy_webkit == true)
		m_check_use_proxy_webkit->setCheckState(Qt::Checked);
	else
		m_check_use_proxy_webkit->setCheckState(Qt::Unchecked);

	//
	// хранилище
	//

	QString database_type = settings.value("storage/type", "SQLite").toString();

	idx = m_combo_database_type->findText(database_type);

	if (idx != -1)
		m_combo_database_type->setCurrentIndex(idx);
	else
		m_combo_database_type->setCurrentIndex(0);

#ifdef Q_WS_WIN
	m_text_database_host->setText(settings.value("mysql/host", "127.0.0.1").toString());
	m_text_database_port->setText(settings.value("mysql/port", "3306"     ).toString());
#else
	m_text_database_host->setText(settings.value("mysql/host", "localhost"      ).toString());
	m_text_database_port->setText(settings.value("mysql/port", "/tmp/mysql.sock").toString());
#endif

	m_text_database_name->setText     (settings.value("mysql/name",     "avalon").toString());
	m_text_database_login->setText    (settings.value("mysql/login",    "root"  ).toString());
	m_text_database_password->setText (settings.value("mysql/password", ""      ).toString());

	m_text_database_file->setText(settings.value("sqlite/file", QDir::homePath() + "/avalon/avalon.db").toString());

	combo_database_type_current_index_changed(m_combo_database_type->currentText());

	//
	// интерфейс
	//

	m_text_mark_as_read_timer->setText   (settings.value("ui/mark_as_read_timer",   "500"        ).toString());
	m_text_max_topic_to_show->setText    (settings.value("ui/max_topic_to_show",    "0"          ).toString());
	m_text_synchronize_interval->setText (settings.value("ui/synchronize_interval", "0"          ).toString());
	m_text_tagline->setText              (settings.value("ui/tagline",              "%%version%%").toString());

	QString colorer_name = settings.value("ui/colorer", COLORER_DEFAULT_NAME).toString();

	idx = m_combo_colorer->findText(colorer_name);

	if (idx != -1)
		m_combo_colorer->setCurrentIndex(idx);
	else
		m_combo_colorer->setCurrentIndex(0);

#ifdef AVALON_USE_ZLIB
	bool compression = settings.value("storage/compression", false).toInt();

	if (compression == true)
		m_check_compression->setCheckState(Qt::Checked);
	else
		m_check_compression->setCheckState(Qt::Unchecked);
#endif

#ifdef AVALON_USE_ASPELL
	bool spellchecker = settings.value("ui/spellchecker", true).toInt();

	if (spellchecker == true)
		m_check_spellchecker->setCheckState(Qt::Checked);
	else
		m_check_spellchecker->setCheckState(Qt::Unchecked);
#endif

	int loggingLevel = settings.value("other/logging_level", 1).toInt();
	m_combo_logging_level->setCurrentIndex(loggingLevel);

	m_text_rsdn_host->setFocus();
}
//----------------------------------------------------------------------------------------------
