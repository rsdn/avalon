#include "form_message.h"
//----------------------------------------------------------------------------------------------
#include "../parser.h"
#include "../global.h"
#include "../storage/storage_factory.h"
//----------------------------------------------------------------------------------------------

FormMessage::FormMessage (QWidget* parent, bool is_reply, const AMessageInfoGUI& info, int edit_id) : FormMessageUI (parent)
{
	m_text_changed = false;

	if (edit_id != 0)
		is_reply = false;

	m_edit_id = edit_id;

	connect(m_text_view->View->page(), SIGNAL(linkClicked(const QUrl&)), this, SLOT(link_clicked(const QUrl&)));
	connect(m_text_view->View->page(), SIGNAL(linkHovered(const QString&, const QString&, const QString&)), this, SLOT(link_hover(const QString&, const QString&, const QString&)));

	m_forum_tree = NULL;
	m_main_form  = NULL;

	m_info = info;

	if (m_edit_id == 0)
	{
		m_info.IDParent = m_info.ID;
		m_info.ID = 0;
	}

	// получение информации о форуме сообщения
	// работает "по тихому", т.к. если возникнет ошибка, то это не сильно принципиально
	// и можно продолжать нормальную работу, а не пугать пользователя
	// http://www.rsdn.ru/forum/message/3315591.1.aspx

	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() != NULL)
	{
		AForumInfo forum_info;

		if (storage->getForumInfo(m_info.IDForum, forum_info) != false)
			m_forum_info = forum_info;
	}

	// формирование текста при ответе
	if (is_reply == true)
	{
		QString subject = m_info.Subject;
		QString body    = m_info.Message;

		//
		// корректировка темы
		//

		if (subject.length() > 3 && subject.mid(0, 4) == "Re: ")
			subject.insert(2, "[2]");
		else if (subject.length() > 3 && subject.mid(0, 3) == "Re[")
		{
			int pos = subject.indexOf("]");

			if (pos != -1)
			{
				int num = subject.mid(3, pos - 3).toInt();

				if (num == 0)
					subject = "Re: " + subject;
				else
				{
					subject.remove(3, pos - 3);
					subject.insert(3, QString::number(num + 1));
				}
			}
			else
				subject = "Re: " + subject;
		}
		else
			subject = "Re: " + subject;

		m_info.Subject = subject;

		//
		// корректировка тела сообщения
		//

		body = AParser::normalizeBody(body, info.UserNick).trimmed();

		if (body.length() > 0)
			body += "\r\n\r\n";

		if (info.IDUser == 0)
			body = QString::fromUtf8("Здравствуйте, Аноним, Вы писали:\r\n\r\n") + body;
		else
			body = QString::fromUtf8("Здравствуйте, ") + info.UserNick + QString::fromUtf8(", Вы писали:\r\n\r\n") + body;

		m_info.Message = body;

	}   // if (is_reply == true)
	else
	{
		// замена HTML спец-символов
		m_info.Message.replace("&gt;",  ">");
		m_info.Message.replace("&lt;",  "<");
		m_info.Message.replace("&amp;", "&");
	}

	// удаление таглайнов
	QRegExp tagline("\\[tagline\\](.+)\\[/tagline\\]");
	tagline.setMinimal(true);
	m_info.Message.replace(tagline, "");
	//m_info.Message = m_info.Message.trimmed();

	// установка остальных полей
	m_text_subject->setText(m_info.Subject);
	m_text_source->setPlainText(m_info.Message);
	text_subject_text_changed(m_info.Subject);

	if (is_reply == true)
	{
		QIcon icon;
		icon.addFile(":/icons/reply16.png",  QSize(16, 16));
		icon.addFile(":/icons/reply22.png",  QSize(22, 22));
		icon.addFile(":/icons/reply32.png",  QSize(32, 32));
		icon.addFile(":/icons/reply48.png",  QSize(48, 48));
		setWindowIcon(icon);

		m_text_source->setFocus();

		m_text_source->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
	}
	else if (m_edit_id != 0)
	{
		QIcon icon;
		icon.addFile(":/icons/edit16.png",  QSize(16, 16));
		icon.addFile(":/icons/edit22.png",  QSize(22, 22));
		icon.addFile(":/icons/edit32.png",  QSize(32, 32));
		icon.addFile(":/icons/edit48.png",  QSize(48, 48));
		icon.addFile(":/icons/edit64.png",  QSize(64, 64));
		setWindowIcon(icon);

		m_text_source->setFocus();

		m_text_source->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
	}
	else
	{
		m_text_subject->setFocus();
		m_text_subject->selectAll();
	}

	// установка обработчиков формы
	connect(m_button_send,  SIGNAL(triggered()),                 this, SLOT(button_send_triggered()));
	connect(m_button_draft, SIGNAL(triggered()),                 this, SLOT(button_draft_triggered()));
	connect(m_text_subject, SIGNAL(textChanged(const QString&)), this, SLOT(text_subject_text_changed(const QString&)));
	connect(m_tab_message,  SIGNAL(currentChanged(int)),         this, SLOT(tab_changed(int)));
	connect(m_text_source,  SIGNAL(textChanged()),               this, SLOT(text_changed()));
}
//----------------------------------------------------------------------------------------------

FormMessage::~FormMessage ()
{
}
//----------------------------------------------------------------------------------------------

void FormMessage::closeEvent (QCloseEvent* event)
{
	// попытка сохранить сообщение в черновики, если оно изменилось
	if (m_text_changed == true)
	{
		QMessageBox::StandardButton result = QMessageBox::question(this, QString::fromUtf8("Сообщение изменено!"), QString::fromUtf8("Сохранить сообщение в черновиках?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel);

		if (result == QMessageBox::Yes)
		{
			sendMessage(true);
			event->ignore();
			return;
		}
		else if (result == QMessageBox::Cancel)
		{
			event->ignore();
			return;
		}
	}

	// сохранение layout
	save();

	event->accept();

	deleteLater();
}
//----------------------------------------------------------------------------------------------

void FormMessage::setForumTree (IForumTree* itf)
{
	m_forum_tree = itf;
}
//----------------------------------------------------------------------------------------------

void FormMessage::setMainForm (IFormMain* itf)
{
	m_main_form = itf;
}
//----------------------------------------------------------------------------------------------

void FormMessage::sendMessage (bool draft)
{
	// получение хранилища
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	// формирование сообщения на добавление
	AMessage2Send info;

	info.IDParent = m_info.IDParent;
	info.IDForum  = m_info.IDForum;
	info.Subject  = m_text_subject->text();
	info.Message  = m_text_source->toPlainText();
	info.Date     = QDateTime::currentDateTime();
	info.Draft    = draft;

	if (m_edit_id == 0)
		info.ID = info.Date.toTime_t(); // это локальный ID
	else
		info.ID = m_edit_id; // редактируемый ID

	// замена спецсимволов HTML
	info.Message.replace("&", "&amp;");
	info.Message.replace(">", "&gt;");
	info.Message.replace("<", "&lt;");

	// помещение в хранилище
	if (storage->addMessage2Send(info) == false)
	{
		storage->showError(this);
		return;
	}

	// обновление в дереве форумов количества непрочитаных сообщений
	if (m_forum_tree != NULL)
		m_forum_tree->reloadUnread(false);

	deleteLater();
}
//----------------------------------------------------------------------------------------------

void FormMessage::button_send_triggered ()
{
	sendMessage(false);
}
//----------------------------------------------------------------------------------------------

void FormMessage::button_draft_triggered ()
{
	sendMessage(true);
}
//----------------------------------------------------------------------------------------------

void FormMessage::text_subject_text_changed (const QString& text)
{
	if (m_forum_info.Name.length() != 0)
		setWindowTitle(text + " - " + m_forum_info.Name);
	else
		setWindowTitle(text);
}
//----------------------------------------------------------------------------------------------

void FormMessage::tab_changed (int tab_index)
{
	if (tab_index == 0)
		m_text_view->View->setHtml(QString());
	else if (tab_index == 1)
	{
		// получение хранилища
		std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

		if (storage.get() == NULL)
		{
			QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
			return;
		}

		// формирование сообщения
		QString body = m_text_source->toPlainText();

		body.replace("&", "&amp;");
		body.replace(">", "&gt;");
		body.replace("<", "&lt;");

		AGlobal* global = AGlobal::getInstance();

		AMessageInfo info;

		info.ID             = 0;
		info.IDTopic        = 0;
		info.IDParent       = 0;
		info.IDUser         = global->Me.ID;
		info.IDForum        = 0;
		info.Subject        = m_text_subject->text();
		info.UserNick       = global->Me.Nick;
		info.Message        = body;
		info.MessageDate    = QDateTime::currentDateTime();

		body = AParser::formatMessage(info);

		m_text_view->View->setHtml(body);
	}
}
//----------------------------------------------------------------------------------------------

void FormMessage::link_clicked (const QUrl& url)
{
	QDesktopServices::openUrl(url);
}
//----------------------------------------------------------------------------------------------

void FormMessage::link_hover (const QString& link, const QString& /*title*/, const QString& /*textContent*/)
{
	bool avalon_link = link.indexOf("avalon:") == 0;

	if (link.length() != 0 && avalon_link == false)
		m_text_view->View->LinkHovered = true;
	else
		m_text_view->View->LinkHovered = false;

	if (avalon_link == false)
		m_main_form->showStatus(link);
}
//----------------------------------------------------------------------------------------------

void FormMessage::text_changed ()
{
	m_text_changed = true;
}
//----------------------------------------------------------------------------------------------
