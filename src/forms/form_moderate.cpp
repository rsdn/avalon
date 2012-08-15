#include "form_moderate.h"
//----------------------------------------------------------------------------------------------
#include "storage/storage_factory.h"
//----------------------------------------------------------------------------------------------

FormModerate::FormModerate  (QWidget* parent, int id_message, int edit_id) : FormModerateUI (parent)
{
	m_forum_tree = 0;

	m_id_edit    = edit_id;
	m_id_message = id_message;

	if (m_id_edit != 0)
	{
		m_button_ok->setEnabled(false);

		// получение информации о модерилке
		std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

		if (storage.get() == NULL)
		{
			QMessageBox::critical(parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
			return;
		}

		AModerate2Send info;

		if (storage->getModerate2SendInfo(m_id_edit, info) == false)
		{
			storage->showError(parent);
			return;
		}

		// установка значений контролов

		// действие
		for (int i = 0; i < m_actions.count(); i++)
			if (m_actions[i].Action == info.Action)
			{
				m_combo_action->setCurrentIndex(i);
				break;
			}

		// форум
		if (info.IDForum != 0)
		{
			for (int i = 0; i < m_forums.count(); i++)
				if (m_forums[i].ID == info.IDForum)
				{
					m_combo_forum->setCurrentIndex(i);
					break;
				}
		}

		// описание
		m_text_description->setText(info.Description);

		m_button_ok->setEnabled(true);
	}

	connect(m_button_ok,     SIGNAL(clicked()), this, SLOT(button_ok_clicked()));
	connect(m_button_cancel, SIGNAL(clicked()), this, SLOT(button_cancel_clicked()));
}
//----------------------------------------------------------------------------------------------

FormModerate::~FormModerate ()
{
}
//----------------------------------------------------------------------------------------------

void FormModerate::closeEvent (QCloseEvent* event)
{
	// сохранение layout
	save();

	event->accept();

	deleteLater();
}
//----------------------------------------------------------------------------------------------

void FormModerate::setForumTree (IForumTree* itf)
{
	m_forum_tree = itf;
}
//----------------------------------------------------------------------------------------------

void FormModerate::button_ok_clicked ()
{
	AModerate2Send info;

	int action_idx = m_combo_action->currentIndex();

	info.IDMessage   = m_id_message;
	info.Action      = m_actions[action_idx].Action;
	info.IDForum     = 0;
	info.Description = m_text_description->toPlainText();
	info.AsModerator = false;
	info.Date        = QDateTime::currentDateTime();

	if (m_id_edit == 0)
		info.ID = info.Date.toTime_t(); // это локальный ID
	else
		info.ID = m_id_edit;

	if (m_actions[action_idx].ForumValid)
		info.IDForum = m_forums[m_combo_forum->currentIndex()].ID;

	// получение хранилища
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	if (storage->addModerate2Send(info) == false)
	{
		storage->showError(this);
		return;
	}

	// обновление в дереве форумов количества непрочитаных сообщений
	if (m_forum_tree != NULL)
		m_forum_tree->reloadUnread(false);

	// сохранение layout
	save();

	// OK
	accept();
}
//----------------------------------------------------------------------------------------------

void FormModerate::button_cancel_clicked ()
{
	// сохранение layout
	save();

	// Отмена
	reject();
}
//----------------------------------------------------------------------------------------------
