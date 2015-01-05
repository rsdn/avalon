#include "form_main.h"
//----------------------------------------------------------------------------------------------
#include "global.h"
#include "form_date.h"
#include "webservice.h"
#include "form_input.h"
#include "form_progress.h"
#include "form_settings.h"
#include "form_subscribe.h"
#include "storage/storage_factory.h"
//----------------------------------------------------------------------------------------------

AFormMain::AFormMain () : AFormMainUI (), IFormMain ()
{
	m_message_view->setMainForm(this);

	//
	// меню
	//

	//
	// меню "Файл"
	//

	connect(m_menu_file_exit, SIGNAL(triggered()), this, SLOT(menu_file_exit_triggered()));

	//
	// меню "Вид"
	//

	connect(m_menu_view_source, SIGNAL(triggered()), this, SLOT(menu_view_source_triggered()));

	//
	// меню "Перейти"
	//

	connect(m_menu_goto_next_unread_article, SIGNAL(triggered()), this, SLOT(menu_goto_next_unread_article_triggered()));
	connect(m_menu_goto_next_unread_thread,  SIGNAL(triggered()), this, SLOT(menu_goto_next_unread_thread_triggered()));
	connect(m_menu_goto_next_unread_forum,   SIGNAL(triggered()), this, SLOT(menu_goto_next_unread_forum_triggered()));
	connect(m_menu_goto_by_id,               SIGNAL(triggered()), this, SLOT(menu_goto_by_id_triggered()));

	//
	// меню "Сервис"
	//

	connect(m_menu_service_synchronize,            SIGNAL(triggered()), this, SLOT(menu_service_synchronize_triggered()));
	connect(m_menu_service_download,               SIGNAL(triggered()), this, SLOT(menu_service_download_triggered()));
	connect(m_menu_service_new_message,            SIGNAL(triggered()), this, SLOT(menu_service_new_message_triggered()));
	connect(m_menu_service_reply,                  SIGNAL(triggered()), this, SLOT(menu_service_reply_triggered()));
	connect(m_menu_service_mark_all_as_read,       SIGNAL(triggered()), this, SLOT(menu_service_mark_all_as_read_triggered()));
	connect(m_menu_service_mark_patrial_as_read,   SIGNAL(triggered()), this, SLOT(menu_service_mark_patrial_as_read_triggered()));
	connect(m_menu_service_mark_all_as_unread,     SIGNAL(triggered()), this, SLOT(menu_service_mark_all_as_unread_triggered()));
	connect(m_menu_service_mark_patrial_as_unread, SIGNAL(triggered()), this, SLOT(menu_service_mark_patrial_as_unread_triggered()));
	connect(m_menu_service_subscribe,              SIGNAL(triggered()), this, SLOT(menu_service_subscribe_triggered()));
	connect(m_menu_service_settings,               SIGNAL(triggered()), this, SLOT(menu_service_settings_triggered()));

#ifdef AVALON_USE_ZLIB
	connect(m_menu_service_storage_compress,       SIGNAL(triggered()), this, SLOT(menu_service_storage_compress_triggered()));
	connect(m_menu_service_storage_uncompress,     SIGNAL(triggered()), this, SLOT(menu_service_storage_uncompress_triggered()));
#endif

	//
	// меню "?"
	//

	connect(m_menu_q_yandex_url,    SIGNAL(triggered()), this, SLOT(menu_q_yandex_url_triggered()));
	connect(m_menu_q_wikipedia_url, SIGNAL(triggered()), this, SLOT(menu_q_wikipedia_url_triggered()));
	connect(m_menu_q_google_url,    SIGNAL(triggered()), this, SLOT(menu_q_google_url_triggered()));
	connect(m_menu_q_rsdn_url,      SIGNAL(triggered()), this, SLOT(menu_q_rsdn_url_triggered()));
	connect(m_menu_q_about,         SIGNAL(triggered()), this, SLOT(menu_q_about_triggered()));

	//
	// тулбар
	//

	connect(m_tool_bar_synchronize, SIGNAL(triggered()), this, SLOT(menu_service_synchronize_triggered()));
	connect(m_tool_bar_new_message, SIGNAL(triggered()), this, SLOT(menu_service_new_message_triggered()));
	connect(m_tool_bar_backward,    SIGNAL(triggered()), this, SLOT(tool_bar_backward_triggered()));
	connect(m_tool_bar_forward,     SIGNAL(triggered()), this, SLOT(tool_bar_forward_triggered()));

	// интерфейсы
	m_forum_tree->setMainForm(this);
	m_message_tree->setMainForm(this);

	// проверка корректности соединения
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() != NULL)
		if (storage->ping() == false)
			menu_service_settings_triggered();

	// загрузка дерева форумов
	// вызывается здесь после того, как в дереве форумов установлен интерфейс главной формы
	// т.к. из перезагрузки выполняется управление включением/выключением возможных действий (см. IFormMain::setEnabledAction)
	m_forum_tree->reload();

	// установка прокси для отображения сообщений
	m_message_view->View->page()->networkAccessManager()->setProxy(AWebservice::defaultProxy(true));

	// таймер периодической синхронизации
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timer_on_timer()));

	int interval = synchronizeInterval();

	if (interval > 0)
		m_timer.start(interval);
	else
		m_timer.stop();

	// текст статуса по умолчанию
	setDefaultStatus();
}
//----------------------------------------------------------------------------------------------

AFormMain::~AFormMain ()
{
}
//----------------------------------------------------------------------------------------------

void AFormMain::menu_file_exit_triggered ()
{
	close();
}
//----------------------------------------------------------------------------------------------

void AFormMain::menu_view_source_triggered ()
{
	m_message_tree->showSource();
}
//----------------------------------------------------------------------------------------------

void AFormMain::menu_service_synchronize_triggered ()
{
	// сохранение текущего выделения в дереве сообщений
	QList<int> restore_path;

	m_message_tree->getSelectedPath(restore_path);

	// получение хранилища
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	//
	// отправка сообщений/модерилок/рейтингов
	//

	AWebservice        webservice(this, this);
	AMessageInfoList   messages_temp;
	AMessage2SendList  messages;
	ARating2SendList   ratings;
	AModerate2SendList moderates;

	if (storage->getMessage2SendList(messages_temp, false) == false)
	{
		storage->showError(this);
		return;
	}

	if (storage->getRating2SendList(ratings) == false)
	{
		storage->showError(this);
		return;
	}

	if (storage->getModerate2SendList(moderates) == false)
	{
		storage->showError(this);
		return;
	}

	for (int i = 0; i < messages_temp.count(); i++)
	{
		AMessage2Send info;

		info.ID       = messages_temp[i].ID;
		info.IDParent = messages_temp[i].IDParent;
		info.IDForum  = messages_temp[i].IDForum;
		info.Subject  = messages_temp[i].Subject;
		info.Message  = messages_temp[i].Message;
		info.Date     = messages_temp[i].MessageDate;

		messages.append(info);
	}

	messages_temp.clear();

	// отправка данных
	if (messages.count() != 0 || ratings.count() != 0 || moderates.count() != 0)
	{
		ACommitInfo commit_info;
		if (webservice.postChange(messages, ratings, moderates, commit_info) == true)
		{
			setDefaultStatus();

			// сохранение ответа
			if (storage->setCommitResult(commit_info) == false)
			{
				storage->showError(this);
				return;
			}

			m_forum_tree->reloadUnread(true);

			// проверка исключений
			if (commit_info.MessagesExceptions.count() != 0 || commit_info.RatingExceptions.count() != 0 || commit_info.ModerateExceptions.count() != 0)
			{
				QString warning = "";

				if (commit_info.MessagesExceptions.count() != 0)
				{
					warning += QString::fromUtf8("<b>Отправка сообщений<b>:\n<pre>");

					for (int i = 0; i < commit_info.MessagesExceptions.count(); i++)
					{
						warning += "ID        : " + QString::number(commit_info.MessagesExceptions[i].ID) + "\n";
						warning += "Exception : " + commit_info.MessagesExceptions[i].Exception + "\n";
						warning += "Info      : " + commit_info.MessagesExceptions[i].Info + "\n";
						warning += "\n";
					}

					warning += "</pre>\n";
				}

				if (commit_info.RatingExceptions.count() != 0)
				{
					warning += QString::fromUtf8("<b>Отправка рейтингов</b>:\n<pre>");

					for (int i = 0; i < commit_info.RatingExceptions.count(); i++)
					{
						warning += "ID        : " + QString::number(commit_info.RatingExceptions[i].ID) + "\n";
						warning += "Exception : " + commit_info.RatingExceptions[i].Exception + "\n";
						warning += "Info      : " + commit_info.RatingExceptions[i].Info + "\n";
						warning += "\n";
					}

					warning += "</pre>\n";
				}

				if (commit_info.ModerateExceptions.count() != 0)
				{
					warning += QString::fromUtf8("<b>Отправка рейтингов</b>:\n<pre>");

					for (int i = 0; i < commit_info.ModerateExceptions.count(); i++)
					{
						warning += "ID        : " + QString::number(commit_info.ModerateExceptions[i].ID) + "\n";
						warning += "Exception : " + commit_info.ModerateExceptions[i].Exception + "\n";
						warning += "Info      : " + commit_info.ModerateExceptions[i].Info + "\n";
						warning += "\n";
					}

					warning += "</pre>\n";
				}

				QMessageBox::warning(this, QString::fromUtf8("Ошибки при отправке данных!"), warning, QMessageBox::Ok);
			}
		}
		else
		{
			setDefaultStatus();
			QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), webservice.error());
			return;
		}
	}

	//
	// получение версий строк
	//

	ARowVersion row_version;

	if (storage->getRowVersion(row_version) == false)
	{
		storage->showError(this);
		return;
	}

	//
	// синхронизация списка пользователей
	//

	// для того, чтобы загрузить весь список пользователей
	// необходимо повторить операцию N раз, т.к. команда RSDN
	// ограничила максимальное кол-во записей в 1000

	QString user_row_version = "";

	while (user_row_version != row_version.User)
	{
		user_row_version = row_version.User;

		AUserInfoList list;
		if (webservice.getUserList(user_row_version, list, row_version.User) == true)
		{
			setDefaultStatus();

			// сохранение ответа
			if (storage->setUserList(list, row_version.User) == false)
			{
				storage->showError(this);
				return;
			}
		}
		else
		{
			setDefaultStatus();
			QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), webservice.error());
			return;
		}
	}

	// после первой синхронизации списка пользователей может быть полезно перегрузить текущего пользователя,
	// т.к. он может быть неизвестен (см. #48)
	AGlobal::getInstance()->reload();

	//
	// синхронизация сообщений
	//

	// для того, чтобы загрузить весь список сообщений
	// необходимо повторить операцию N раз, т.к. могут быть, например, оборваные ветки

	QString message_row_version  = "";
	QString rating_row_version   = "";
	QString moderate_row_version = "";
	bool    has_broken           = true;

	while (!(
		message_row_version  == row_version.Message  &&
		rating_row_version   == row_version.Rating   &&
		moderate_row_version == row_version.Moderate &&
		has_broken           == false
	))
	{
		message_row_version  = row_version.Message;
		rating_row_version   = row_version.Rating;
		moderate_row_version = row_version.Moderate;


		ADataQuery query;

		// получение параметров запроса
		if (storage->getMessageQuery(query) == false)
		{
			storage->showError(this);
			return;
		}

		ADataList list;
		if (webservice.getMessageList(row_version, query, list, row_version) == true)
		{
			setDefaultStatus();

			// сохранение ответа
			if (storage->setMessageList(list, row_version, true) == false)
			{
				storage->showError(this);
				return;
			}

			// получение количества поломанных топиков
			if (storage->hasBroken(has_broken) == false)
			{
				storage->showError(this);
				return;
			}
		}
		else
		{
			setDefaultStatus();
			QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), webservice.error());
			return;
		}

		// перезагрузка количества непрочитаных сообщений
		m_forum_tree->reloadUnread(true);
	}

	// восстановление положения выделения в дереве сообщений
	m_message_tree->selectByPath(&restore_path);

	setDefaultStatus();
}
//----------------------------------------------------------------------------------------------

void AFormMain::menu_service_new_message_triggered ()
{
	m_forum_tree->newMessage();
}
//----------------------------------------------------------------------------------------------

void AFormMain::menu_service_reply_triggered ()
{
	m_message_tree->reply();
}
//----------------------------------------------------------------------------------------------

void AFormMain::menu_service_mark_all_as_read_triggered ()
{
	// получение хранилища
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	if (storage->setIDsAsRead(QList<int>(), idsAll, true, QDateTime()) == false)
	{
		storage->showError(this);
		return;
	}

	// перезагрузка количества непрочитаных
	m_forum_tree->reloadUnread(true);
}
//----------------------------------------------------------------------------------------------

void AFormMain::menu_service_mark_patrial_as_read_triggered ()
{
	std::auto_ptr<FormDate> form(new FormDate(this, true));

	if (form->exec() == QDialog::Accepted)
	{
		// получение хранилища
		std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

		if (storage.get() == NULL)
		{
			QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
			return;
		}

		if (storage->setIDsAsRead(QList<int>(), idsAll, true, form->SelectedDate()) == false)
		{
			storage->showError(this);
			return;
		}

		// перезагрузка количества непрочитаных
		m_forum_tree->reloadUnread(true);
	}
}
//----------------------------------------------------------------------------------------------

void AFormMain::menu_service_mark_all_as_unread_triggered ()
{
	// однако, когда база 6 гигов и промахнешься пунктом меню, то наступает локальный коллапс :)
	QMessageBox::StandardButton result = QMessageBox::question(this, QString::fromUtf8("Внимание!"), QString::fromUtf8("Операция может занять (очень) длительное время! Вы уверены что вы хотите пометить <u><b>все</b></u> сообщения как <u><b>непрочитанные</b></u>?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

	if (result != QMessageBox::Yes)
		return;

	// получение хранилища
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	if (storage->setIDsAsRead(QList<int>(), idsAll, false, QDateTime()) == false)
	{
		storage->showError(this);
		return;
	}

	// перезагрузка количества непрочитаных
	m_forum_tree->reloadUnread(true);
}
//----------------------------------------------------------------------------------------------

void AFormMain::menu_service_mark_patrial_as_unread_triggered ()
{
	std::auto_ptr<FormDate> form(new FormDate(this, false));

	if (form->exec() == QDialog::Accepted)
	{
		// получение хранилища
		std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

		if (storage.get() == NULL)
		{
			QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
			return;
		}

		if (storage->setIDsAsRead(QList<int>(), idsAll, false, form->SelectedDate()) == false)
		{
			storage->showError(this);
			return;
		}

		// перезагрузка количества непрочитаных
		m_forum_tree->reloadUnread(true);
	}
}
//----------------------------------------------------------------------------------------------

void AFormMain::menu_service_subscribe_triggered ()
{
	FormSubscribe* form = new FormSubscribe(this);

	if (form->exec() == QDialog::Accepted)
	{
		delete form;

		m_forum_tree->reload();
	}
	else
		delete form;
}
//----------------------------------------------------------------------------------------------

void AFormMain::menu_service_settings_triggered ()
{
	FormSettings* form = new FormSettings(this);

	if (form->exec() == QDialog::Accepted)
	{
		delete form;

		// перезагрузка глобальных настроек
		AGlobal::getInstance()->reload();

		// перезагрузка дерева форумов
		m_forum_tree->reload();

		// переустановка прокси для отображения сообщений
		m_message_view->View->page()->networkAccessManager()->setProxy(AWebservice::defaultProxy(true));

		// таймер синхронизации
		QSettings settings;

		int interval = synchronizeInterval();

		if (interval > 0)
			m_timer.start(interval);
		else
			m_timer.stop();

		// обновление текста статуса
		setDefaultStatus();
	}
	else
		delete form;
}
//----------------------------------------------------------------------------------------------

void AFormMain::menu_q_yandex_url_triggered ()
{
	QDesktopServices::openUrl(QString("http://www.yandex.ru"));
}
//----------------------------------------------------------------------------------------------

void AFormMain::menu_q_wikipedia_url_triggered ()
{
	QDesktopServices::openUrl(QString("https://ru.wikipedia.org"));
}
//----------------------------------------------------------------------------------------------

void AFormMain::menu_q_google_url_triggered ()
{
	QDesktopServices::openUrl(QString("https://www.google.ru"));
}
//----------------------------------------------------------------------------------------------

void AFormMain::menu_q_rsdn_url_triggered ()
{
	QDesktopServices::openUrl(QString("https://rsdn.ru"));
}
//----------------------------------------------------------------------------------------------

void AFormMain::menu_q_about_triggered ()
{
	QString text = QString::fromUtf8(
		"<b>avalon</b> - кросс-платформенный клиент для чтения форумов <a href='https://rsdn.ru'>rsdn</a> аналогичный проекту <a href='https://rsdn.ru/projects/janus/article/article.xml'>janus</a> команды rsdn под windows.<br/><br/>"
		"<b>домашняя страница</b>: <a href='https://github.com/rsdn/avalon'>https://github.com/rsdn/avalon</a><br/><br/>"
		"<b>разработчики</b>:"
		"<ul>"
		"<li><a href='https://github.com/abbat'>Anton Batenev</a></li>"
		"<li><a href='https://github.com/dmitriid'>Dmitrii 'Mamut' Dimandt</a></li>"
		"<li><a href='https://github.com/Sheridan'>Горлов Максим</a></li>"
		"<li><a href='http://stanishevskiy.moikrug.ru/'>std.denis</a></li>"
		"<li><a href='http://ua-coder.blogspot.com/'>Anatoliy</a></li>"
		"<li><a href='https://github.com/astavonin'>Alexander Stavonin</a></li>"
		"</ul>"
	);

	QMessageBox::about(this, "avalon/" + getVersionString(), text);
}
//----------------------------------------------------------------------------------------------

void AFormMain::menu_goto_next_unread_article_triggered ()
{
	if (m_menu_goto_next_unread_article->isEnabled() == true)
		m_message_tree->gotoNextUnreadArticle();
}
//----------------------------------------------------------------------------------------------

void AFormMain::menu_goto_next_unread_thread_triggered ()
{
	if (m_menu_goto_next_unread_thread->isEnabled() == true)
		m_message_tree->gotoNextUnreadThread();
}
//----------------------------------------------------------------------------------------------

void AFormMain::menu_goto_next_unread_forum_triggered ()
{
	if (m_menu_goto_next_unread_forum->isEnabled() == true)
		m_forum_tree->gotoNextUnreadForum();
}
//----------------------------------------------------------------------------------------------

void AFormMain::menu_goto_by_id_triggered ()
{
	std::auto_ptr<FormInput> form(new FormInput(this, QString::fromUtf8("Перейти к сообщению"), QString::fromUtf8("Введите URL или номер сообщения / ветки"), ""));

	QIcon icon;
	icon.addFile(":/icons/download16.png",  QSize(16, 16));
	icon.addFile(":/icons/download22.png",  QSize(22, 22));
	icon.addFile(":/icons/download24.png",  QSize(24, 24));
	icon.addFile(":/icons/download32.png",  QSize(32, 32));
	icon.addFile(":/icons/download48.png",  QSize(48, 48));
	icon.addFile(":/icons/download64.png",  QSize(64, 64));
	icon.addFile(":/icons/download128.png", QSize(128, 128));
	form->setWindowIcon(icon);

	if (form->exec() == QDialog::Accepted)
	{
		QString text = form->text().trimmed();

		bool is_int     = false;
		int  id_message = text.toInt(&is_int);
		int  id_forum   = 0;

		if (is_int == false)
		{
			QRegExp reg_id_message("rsdn\\.ru.+(\\d+).+");

			if (reg_id_message.indexIn(text) == -1)
			{
				QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Текст ") + text + QString::fromUtf8(" не является ни ссылкой ни числом требуемого формата."));
				return;
			}

			QString str_id_message = reg_id_message.cap(1);

			id_message = str_id_message.toInt(&is_int);
		}

		if (is_int == false || id_message < 1)
		{
			QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Текст ") + text + QString::fromUtf8(" не содержит корректного числа."));
			return;
		}

		std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

		if (storage.get() == NULL)
		{
			QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
			return;
		}

		QList<int> path;

		if (storage->getMessagePath(id_message, id_forum, path) == false)
		{
			storage->showError(this);
			return;
		}

		if (id_forum == 0 || path.count() == 0)
		{
			QMessageBox::information(this, QString::fromUtf8("Внимание!"), QString::fromUtf8("Сообщение не найдено! Можете попробовать загрузить его дополнительно."));
			return;
		}

		if (m_forum_tree->selectForum(id_forum) == false)
		{
			QMessageBox::information(this, QString::fromUtf8("Внимание!"), QString::fromUtf8("Вы не подписаны на форум в котором находится запрашиваемое сообщение."));
			return;
		}

		if (m_message_tree->selectByPath(&path) == false)
		{
			QMessageBox::information(this, QString::fromUtf8("Внимание!"), QString::fromUtf8("Сообщение не может быть выбрано (скорее всего, это ошибка программы)."));
			return;
		}
	}
}
//----------------------------------------------------------------------------------------------

void AFormMain::showStatus (const QString& value)
{
	if (value.length() > 0)
		m_status_bar->showMessage(value);
	else
		setDefaultStatus();
}
//----------------------------------------------------------------------------------------------

void AFormMain::setEnabledAction (AvalonActions action, bool enabled)
{
	if (action == aaNewMessage)
	{
		m_menu_service_new_message->setEnabled(enabled);
		m_tool_bar_new_message->setEnabled(enabled);
		m_menu_service_reply->setEnabled(m_menu_view_source->isEnabled());
	}
	else if (action == aaViewSource)
	{
		m_menu_view_source->setEnabled(enabled);
		m_menu_service_reply->setEnabled(m_menu_service_new_message->isEnabled());
	}
	else if (action == aaPrevNextUnreadArticle)
		m_menu_goto_next_unread_article->setEnabled(enabled);
	else if (action == aaPrevNextUnreadThread)
		m_menu_goto_next_unread_thread->setEnabled(enabled);
	else if (action == aaPrevNextUnreadForum)
		m_menu_goto_next_unread_forum->setEnabled(enabled);
	else if (action == aaBackward)
		m_tool_bar_backward->setEnabled(enabled);
	else if (action == aaForward)
		m_tool_bar_forward->setEnabled(enabled);
}
//----------------------------------------------------------------------------------------------

void AFormMain::timer_on_timer ()
{
	// синхронизация
	menu_service_synchronize_triggered();
}
//----------------------------------------------------------------------------------------------

#ifdef AVALON_USE_ZLIB
void AFormMain::menu_service_storage_compress_triggered ()
{
	QMessageBox::StandardButton result = QMessageBox::question(this, QString::fromUtf8("Внимание!"), QString::fromUtf8("Операция может занять (очень) длительное время! По завершении сжатия суммарный размер данных уменьшится приблизительно в два раза. Вы уверены что вы хотите упаковать хранилище?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

	if (result != QMessageBox::Yes)
		return;

	// получение хранилища
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	std::auto_ptr<FormProgress> form(new FormProgress(this));

	form->setWindowTitle(QString::fromUtf8("Сжатие хранилища"));

	QIcon icon;
	icon.addFile(":/icons/compress16.png",  QSize(16, 16));
	icon.addFile(":/icons/compress24.png",  QSize(24, 24));
	icon.addFile(":/icons/compress32.png",  QSize(32, 32));
	icon.addFile(":/icons/compress48.png",  QSize(48, 48));
	icon.addFile(":/icons/compress256.png", QSize(256, 256));
	form->setWindowIcon(icon);

	form->setVisible(true);

	if (storage->compressStorage(form.get()) == false)
		storage->showError(form.get());

	form->exec();
}
#endif   // AVALON_USE_ZLIB
//----------------------------------------------------------------------------------------------

#ifdef AVALON_USE_ZLIB
void AFormMain::menu_service_storage_uncompress_triggered ()
{
	QMessageBox::StandardButton result = QMessageBox::question(this, QString::fromUtf8("Внимание!"), QString::fromUtf8("Операция может занять (очень) длительное время! По завершении распаковки суммарный размер данных увеличится приблизительно в два раза. Вы уверены что вы хотите распаковать хранилище?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

	if (result != QMessageBox::Yes)
		return;

	// получение хранилища
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	std::auto_ptr<FormProgress> form(new FormProgress(this));

	form->setWindowTitle(QString::fromUtf8("Распаковка хранилища"));

	QIcon icon;
	icon.addFile(":/icons/compress16.png",  QSize(16, 16));
	icon.addFile(":/icons/compress24.png",  QSize(24, 24));
	icon.addFile(":/icons/compress32.png",  QSize(32, 32));
	icon.addFile(":/icons/compress48.png",  QSize(48, 48));
	icon.addFile(":/icons/compress256.png", QSize(256, 256));
	form->setWindowIcon(icon);

	form->setVisible(true);

	if (storage->uncompressStorage(form.get()) == false)
		storage->showError(form.get());

	form->exec();
}
#endif   // AVALON_USE_ZLIB
//----------------------------------------------------------------------------------------------

void AFormMain::tool_bar_backward_triggered ()
{
	m_message_tree->gotoBackward();
}
//----------------------------------------------------------------------------------------------

void AFormMain::tool_bar_forward_triggered  ()
{
	m_message_tree->gotoForward();
}
//----------------------------------------------------------------------------------------------

void AFormMain::menu_service_download_triggered()
{
	std::auto_ptr<FormInput> form(new FormInput(this, QString::fromUtf8("Загрузка сообщения / ветки"), QString::fromUtf8("Введите URL или номер сообщения / ветки"), ""));

	QIcon icon;
	icon.addFile(":/icons/download16.png",  QSize(16, 16));
	icon.addFile(":/icons/download22.png",  QSize(22, 22));
	icon.addFile(":/icons/download24.png",  QSize(24, 24));
	icon.addFile(":/icons/download32.png",  QSize(32, 32));
	icon.addFile(":/icons/download48.png",  QSize(48, 48));
	icon.addFile(":/icons/download64.png",  QSize(64, 64));
	icon.addFile(":/icons/download128.png", QSize(128, 128));
	form->setWindowIcon(icon);

	if (form->exec() == QDialog::Accepted)
	{
		QString text = form->text().trimmed();

		bool is_int = false;
		int message_id = text.toInt(&is_int);

		if (is_int == false)
		{
			QRegExp reg_message_id("rsdn\\.ru.+(\\d+).+");

			if (reg_message_id.indexIn(text) == -1)
			{
				QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Текст ") + text + QString::fromUtf8(" не является ни ссылкой ни числом требуемого формата."));
				return;
			}

			QString str_message_id = reg_message_id.cap(1);

			message_id = str_message_id.toInt(&is_int);
		}

		if (is_int == false || message_id < 1)
		{
			QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Текст ") + text + QString::fromUtf8(" не содержит корректного числа."));
			return;
		}

		std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

		if (storage.get() == NULL)
		{
			QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
			return;
		}

		if (storage->addBroken(message_id, true) == false)
			storage->showError(this);
		else
			QMessageBox::information(this, QString::fromUtf8("Внимание!"), QString::fromUtf8("Сообщение ") + QString::number(message_id) + QString::fromUtf8(" будет загруженно при следующей синхронизации"));
	}
}
//----------------------------------------------------------------------------------------------

void AFormMain::setDefaultStatus ()
{
	m_status_bar->showMessage("");
}
//----------------------------------------------------------------------------------------------

int AFormMain::synchronizeInterval ()
{
	return QSettings().value("ui/synchronize_interval", "0").toUInt() * 1000 * 60;
}
//----------------------------------------------------------------------------------------------

void AFormMain::onProgress (int /*percent*/)
{
	/*m_progress_bar->setMinimum(0);
	m_progress_bar->setMaximum(100);
	m_progress_bar->setValue(percent);*/

	QCoreApplication::processEvents();
}
//----------------------------------------------------------------------------------------------

void AFormMain::onProgress (int percent, const QString& status)
{
	if (status.length() > 0)
		m_status_bar->showMessage(status);

	onProgress(percent);
}
//----------------------------------------------------------------------------------------------

void AFormMain::onProgress (int /*minimum*/, int /*maximum*/, int /*value*/)
{
	/*m_progress_bar->setMinimum(minimum);
	m_progress_bar->setMaximum(maximum);
	m_progress_bar->setValue(value);*/

	QCoreApplication::processEvents();
}
//----------------------------------------------------------------------------------------------

void AFormMain::onProgress (int minimum, int maximum, int value, const QString& status)
{
	if (status.length() > 0)
		m_status_bar->showMessage(status);

	onProgress(minimum, maximum, value);
}
//----------------------------------------------------------------------------------------------
