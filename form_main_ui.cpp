//----------------------------------------------------------------------------------------------
// $Date: 2011-04-07 12:00:34 +0400 (Чтв, 07 Апр 2011) $
// $Author: antonbatenev.ya.ru $
// $Revision: 416 $
// $URL: svn://opensvn.ru/avalon/trunk/form_main_ui.cpp $
//----------------------------------------------------------------------------------------------
#include "form_main_ui.h"
//----------------------------------------------------------------------------------------------

AFormMainUI::AFormMainUI () : QMainWindow ()
{
	//
	// главное окно
	//

	setWindowIcon(QIcon(":/icons/main.png"));
	setWindowTitle(QString::fromUtf8("avalon"));

	//
	// меню
	//

	m_menubar = new QMenuBar(this);

	// верхний ряд меню
	m_menu_file    = m_menubar->addMenu(QString::fromUtf8("&Файл"));
	m_menu_view    = m_menubar->addMenu(QString::fromUtf8("&Вид"));
	m_menu_goto    = m_menubar->addMenu(QString::fromUtf8("&Перейти"));
	m_menu_service = m_menubar->addMenu(QString::fromUtf8("&Сервис"));
	m_menu_q       = m_menubar->addMenu(QString::fromUtf8("&?"));

	// меню "Файл"
	m_menu_file_exit = m_menu_file->addAction(QString::fromUtf8("Выход"));
	m_menu_file_exit->setIcon(QIcon(":/icons/exit16.png"));
	m_menu_file_exit->setMenuRole(QAction::QuitRole);

	// меню "Вид"
	m_menu_view_source = m_menu_view->addAction(QString::fromUtf8("Просмотр источника"));
	m_menu_view_source->setIcon(QIcon(":/icons/viewsource16.png"));
	m_menu_view_source->setShortcut(QString("V"));
	m_menu_view_source->setEnabled(false);

	// меню "Перейти"
	m_menu_goto_next_unread_article = m_menu_goto->addAction(QString::fromUtf8("Следующая непрочитанная статья"));
	m_menu_goto_next_unread_article->setIcon(QIcon(":/icons/nextunreadarticle16.png"));
	m_menu_goto_next_unread_article->setShortcut(QString("M"));
	m_menu_goto_next_unread_article->setEnabled(false);

	m_menu_goto_next_unread_thread = m_menu_goto->addAction(QString::fromUtf8("Следующая непрочитанная ветка"));
	m_menu_goto_next_unread_thread->setIcon(QIcon(":/icons/nextunreadthread16.png"));
	m_menu_goto_next_unread_thread->setShortcut(QString("T"));
	m_menu_goto_next_unread_thread->setEnabled(false);

	m_menu_goto_next_unread_forum = m_menu_goto->addAction(QString::fromUtf8("Следующий непрочитанный форум"));
	m_menu_goto_next_unread_forum->setIcon(QIcon(":/icons/nextunreadforum16.png"));
	m_menu_goto_next_unread_forum->setShortcut(QString("F"));
	m_menu_goto_next_unread_forum->setEnabled(false);

	// меню "Сервис"
	m_menu_service_synchronize = m_menu_service->addAction(QString::fromUtf8("Синхронизировать"));
	m_menu_service_synchronize->setIcon(QIcon(":/icons/synchronize16.png"));

	m_menu_service_download = m_menu_service->addAction(QString::fromUtf8("Загрузить сообщение / ветку"));
	m_menu_service_download->setIcon(QIcon(":/icons/download16.png"));

	m_menu_service->addSeparator();

	m_menu_service_new_message = m_menu_service->addAction(QString::fromUtf8("Новое сообщение"));
	m_menu_service_new_message->setIcon(QIcon(":/icons/new16.png"));
	m_menu_service_new_message->setShortcut(QString("CTRL+N"));
	m_menu_service_new_message->setEnabled(false);

	m_menu_service->addSeparator();

	m_menu_service_reply = m_menu_service->addAction(QString::fromUtf8("Ответить"));
	m_menu_service_reply->setIcon(QIcon(":/icons/reply16.png"));
	m_menu_service_reply->setShortcut(QString("CTRL+R"));
	m_menu_service_reply->setEnabled(false);

	m_menu_service->addSeparator();

	m_menu_service_mark_all_as_read = m_menu_service->addAction(QString::fromUtf8("Пометить все, как прочитанные"));
	m_menu_service_mark_all_as_read->setIcon(QIcon(":/icons/markallasread16.png"));

	m_menu_service_mark_patrial_as_read = m_menu_service->addAction(QString::fromUtf8("Пометить до даты, как прочитанные"));
	m_menu_service_mark_patrial_as_read->setIcon(QIcon(":/icons/markpatrialasread16.png"));

	m_menu_service->addSeparator();

	m_menu_service_mark_all_as_unread = m_menu_service->addAction(QString::fromUtf8("Пометить все, как непрочитанные"));
	m_menu_service_mark_all_as_unread->setIcon(QIcon(":/icons/markallasunread16.png"));

	m_menu_service_mark_patrial_as_unread = m_menu_service->addAction(QString::fromUtf8("Пометить после даты, как непрочитанные"));
	m_menu_service_mark_patrial_as_unread->setIcon(QIcon(":/icons/markpatrialasunread16.png"));

	m_menu_service->addSeparator();

	m_menu_service_subscribe = m_menu_service->addAction(QString::fromUtf8("Подписка на форумы"));
	m_menu_service_subscribe->setIcon(QIcon(":/icons/subscribe16.png"));

	m_menu_service->addSeparator();

	m_menu_service_settings = m_menu_service->addAction(QString::fromUtf8("Настройки"));
	m_menu_service_settings->setIcon(QIcon(":/icons/settings16.png"));
	m_menu_service_settings->setMenuRole(QAction::PreferencesRole);

#ifdef AVALON_USE_ZLIB
	m_menu_service->addSeparator();

	m_menu_service_storage = m_menu_service->addMenu(QString::fromUtf8("Хранилище"));
	m_menu_service_storage->setIcon(QIcon(":/icons/storage16.png"));

	m_menu_service_storage_compress = m_menu_service_storage->addAction(QString::fromUtf8("Сжать тела сообщений"));
	m_menu_service_storage_compress->setIcon(QIcon(":/icons/compress16.png"));

	m_menu_service_storage_uncompress = m_menu_service_storage->addAction(QString::fromUtf8("Распаковать тела сообщений"));
	m_menu_service_storage_uncompress->setIcon(QIcon(":/icons/compress16.png"));
#endif

	// меню "?"
	m_menu_q_yandex_url = m_menu_q->addAction(QString::fromUtf8("Яндекс"));
	m_menu_q_yandex_url->setIcon(QIcon(":/icons/yandex.ico"));

	m_menu_q_wikipedia_url = m_menu_q->addAction(QString::fromUtf8("Википедия"));
	m_menu_q_wikipedia_url->setIcon(QIcon(":/icons/wikipedia.ico"));

	m_menu_q_google_url = m_menu_q->addAction(QString::fromUtf8("Google"));
	m_menu_q_google_url->setIcon(QIcon(":/icons/google.ico"));

	m_menu_q_rsdn_url = m_menu_q->addAction(QString::fromUtf8("RSDN"));
	m_menu_q_rsdn_url->setIcon(QIcon(":/icons/rsdn16.png"));

	m_menu_q->addSeparator();

	m_menu_q_update = m_menu_q->addAction(QString::fromUtf8("Обновление"));
	m_menu_q_update->setIcon(QIcon(":/icons/update16.png"));

	if (!(QSysInfo::WordSize == 32 || QSysInfo::WordSize == 64))
		m_menu_q_update->setVisible(false);

#if (defined(Q_WS_WIN) || defined(Q_WS_MAC) || defined(Q_WS_X11))
	if (m_menu_q_update->isVisible() == true)
		m_menu_q->addSeparator();
#else
	m_menu_q_update->setVisible(false);
#endif

	m_menu_q_about = m_menu_q->addAction(QString::fromUtf8("О программе"));
	m_menu_q_about->setIcon(QIcon(":/icons/help16.png"));
	m_menu_q_about->setMenuRole(QAction::AboutRole);

	setMenuBar(m_menubar);

	//
	// тулбар
	//

	m_tool_bar = new QToolBar(this);
	m_tool_bar->setWindowTitle(QString::fromUtf8("Панель инструментов"));
	m_tool_bar->setMovable(false);

	m_tool_bar_synchronize = m_tool_bar->addAction(m_menu_service_synchronize->text());
	m_tool_bar_synchronize->setIcon(QIcon(":/icons/synchronize24.png"));

	m_tool_bar_new_message = m_tool_bar->addAction(m_menu_service_new_message->text());
	m_tool_bar_new_message->setIcon(QIcon(":/icons/new24.png"));
	m_tool_bar_new_message->setEnabled(false);

	m_tool_bar->addSeparator();

	m_tool_bar_backward = m_tool_bar->addAction(QString::fromUtf8("Назад"));
	m_tool_bar_backward->setIcon(QIcon(":/icons/backward24.png"));
	m_tool_bar_backward->setShortcut(QString("Backspace"));
	m_tool_bar_backward->setEnabled(false);

	m_tool_bar_forward = m_tool_bar->addAction(QString::fromUtf8("Вперед"));
	m_tool_bar_forward->setIcon(QIcon(":/icons/forward24.png"));
	m_tool_bar_forward->setEnabled(false);

	addToolBar(m_tool_bar);

	//
	// центр
	//

	m_splitter = new QSplitter(this);
	m_splitter->setOrientation(Qt::Horizontal);

	m_forum_tree = new AForumTree(m_splitter, this);
	m_splitter->addWidget(m_forum_tree);

	m_splitter_right = new QSplitter(m_splitter);
	m_splitter_right->setOrientation(Qt::Vertical);
	m_splitter->addWidget(m_splitter_right);

	m_message_tree = new AMessageTree(m_splitter_right, this);
	m_splitter_right->addWidget(m_message_tree);

	m_message_view = new AMessageView(m_splitter_right);
	m_splitter_right->addWidget(m_message_view);

	m_splitter->setStretchFactor(1, 30);
	m_splitter_right->setStretchFactor(1, 30);

	//
	// статусбар
	//

	m_status_bar = new QStatusBar(this);
	setStatusBar(m_status_bar);

	setCentralWidget(m_splitter);

	// восстановление layout
	restore();

	//
	// установка перекрестных интерфейсов
	//

	m_forum_tree->setMessageTree(m_message_tree);
	m_forum_tree->setMessageView(m_message_view);

	m_message_tree->setForumTree(m_forum_tree);
	m_message_tree->setMessageView(m_message_view);

	m_message_view->setForumTree(m_forum_tree);
	m_message_view->setMessageTree(m_message_tree);
}
//----------------------------------------------------------------------------------------------

AFormMainUI::~AFormMainUI ()
{
}
//----------------------------------------------------------------------------------------------

void AFormMainUI::closeEvent (QCloseEvent* event)
{
	// сохранение layout
	save();

	event->accept();
}
//----------------------------------------------------------------------------------------------

void AFormMainUI::save ()
{
	QSettings settings;

	settings.setValue("layout/window",  this->saveGeometry());
	settings.setValue("layout/toolbar", m_tool_bar->isVisible());
	settings.setValue("layout/form",    m_splitter->saveState());
	settings.setValue("layout/right",   m_splitter_right->saveState());

	m_forum_tree->save();
	m_message_tree->save();
}
//----------------------------------------------------------------------------------------------

void AFormMainUI::restore ()
{
	QSettings settings;

	this->restoreGeometry(settings.value("layout/window").toByteArray());
	m_tool_bar->setVisible(settings.value("layout/toolbar", true).toBool());
	m_splitter->restoreState(settings.value("layout/form").toByteArray());
	m_splitter_right->restoreState(settings.value("layout/right").toByteArray());

	m_forum_tree->restore();
	m_message_tree->restore();
}
//----------------------------------------------------------------------------------------------
