#include "form_moderate_ui.h"
//----------------------------------------------------------------------------------------------
#include "storage/storage_factory.h"
//----------------------------------------------------------------------------------------------

FormModerateUI::FormModerateUI (QWidget* parent) : QDialog (parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint
#if QT_VERSION >= 0x040500
 | Qt::WindowCloseButtonHint
#endif
)
{
	AModerateAction action;

	action.Action      = "MoveMessage";
	action.Description = QString::fromUtf8("Переместить сообщение");
	action.ForumValid  = true;
	m_actions.append(action);

	action.Action      = "DeleteMessage";
	action.Description = QString::fromUtf8("Удалить сообщение");
	action.ForumValid  = false;
	m_actions.append(action);

	action.Action      = "DeleteThread";
	action.Description = QString::fromUtf8("Удалить тему");
	action.ForumValid  = false;
	m_actions.append(action);

	action.Action      = "DeleteErrorMessage";
	action.Description = QString::fromUtf8("Удалить сообщение как ошибочное");
	action.ForumValid  = false;
	m_actions.append(action);

	action.Action      = "SplitThread";
	action.Description = QString::fromUtf8("Выделить в новую тему");
	action.ForumValid  = true;
	m_actions.append(action);

	action.Action      = "CloseTopic";
	action.Description = QString::fromUtf8("Закрыть тему");
	action.ForumValid  = false;
	m_actions.append(action);

	action.Action      = "OpenTopic";
	action.Description = QString::fromUtf8("Открыть тему");
	action.ForumValid  = false;
	m_actions.append(action);

	setWindowIcon(QIcon(":/icons/moderate.png"));

	setWindowTitle(QString::fromUtf8("модерирование"));

	resize(500, 333);

	m_layout = new QHBoxLayout(this);
		m_layout_left = new QVBoxLayout();
			m_layout_combo_label = new QHBoxLayout();
				m_layout_label = new QVBoxLayout();
				m_layout_combo = new QVBoxLayout();
			m_layout_description = new QVBoxLayout();
		m_layout_button = new QVBoxLayout();

	m_label_action = new QLabel(this);
	m_label_action->setText(QString::fromUtf8("Действие"));
	m_layout_label->addWidget(m_label_action);

	m_label_forum = new QLabel(this);
	m_label_forum->setText(QString::fromUtf8("Форум"));
	m_layout_label->addWidget(m_label_forum);

	m_combo_action = new QComboBox(this);

	for (int i = 0; i < m_actions.count(); i++)
		m_combo_action->insertItem(i, m_actions[i].Description);

	m_layout_combo->addWidget(m_combo_action);

	m_combo_forum = new QComboBox(this);
	m_layout_combo->addWidget(m_combo_forum);

	m_spacer_combo = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
	m_layout_combo->addItem(m_spacer_combo);

	m_label_description = new QLabel(this);
	m_label_description->setText(QString::fromUtf8("Комментарий"));
	m_layout_description->addWidget(m_label_description);

	m_text_description = new QTextEdit(this);

	#ifndef Q_WS_WIN
		m_text_description->setFrameShadow(QFrame::Plain);
	#endif

	m_layout_description->addWidget(m_text_description);

	m_button_ok = new QPushButton(this);
	m_button_ok->setText(QString::fromUtf8("OK"));
	m_button_ok->setShortcut(QKeySequence("Return"));
	m_button_ok->setDefault(true);
	m_layout_button->addWidget(m_button_ok);

	m_button_cancel = new QPushButton(this);
	m_button_cancel->setText(QString::fromUtf8("Отмена"));
	m_button_cancel->setShortcut(QKeySequence("Esc"));
	m_layout_button->addWidget(m_button_cancel);

	m_spacer_button = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	m_layout_button->addItem(m_spacer_button);

	m_layout_combo_label->addLayout(m_layout_label);
	m_layout_combo_label->addLayout(m_layout_combo);
	m_layout_left->addLayout(m_layout_combo_label);
	m_layout_left->addLayout(m_layout_description);
	m_layout->addLayout(m_layout_left);
	m_layout->addLayout(m_layout_button);

	// получение хранилища
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	// получение списка форумов
	AForumGroupInfoList list;

	if (storage->getForumList(list, false) == false)
	{
		storage->showError(this);
		return;
	}

	for (int group_index = 0; group_index < list.count(); group_index++)
		for (int forum_index = 0; forum_index < list[group_index].Forums.count(); forum_index++)
		{
			AModerateForum forum;

			forum.ID   = list[group_index].Forums[forum_index].ID;
			forum.Name = list[group_index].Group.Name + "::" + list[group_index].Forums[forum_index].Name;

			m_forums.append(forum);
		}

	for (int i = 0; i < m_forums.count(); i++)
		m_combo_forum->insertItem(i, m_forums[i].Name);

	m_combo_forum->setMaxVisibleItems(10);

	connect(m_combo_action, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(combo_action_current_index_changed(const QString&)));

	// восстановление layout
	restore();
}
//----------------------------------------------------------------------------------------------

FormModerateUI::~FormModerateUI ()
{
}
//----------------------------------------------------------------------------------------------

void FormModerateUI::save ()
{
	QSettings settings;

	if (windowState() == Qt::WindowNoState)
	{
		QRect rect = geometry();

		settings.setValue("form_moderate/left",   rect.left());
		settings.setValue("form_moderate/top",    rect.top());
		settings.setValue("form_moderate/width",  rect.width());
		settings.setValue("form_moderate/height", rect.height());
	}

	settings.setValue("form_moderate/state", static_cast<int>(windowState()));
}
//----------------------------------------------------------------------------------------------

void FormModerateUI::restore ()
{
	QSettings settings;

	QRect screen = QDesktopWidget().screenGeometry(this);

	int w = settings.value("form_moderate/width",  width()).toInt();
	int h = settings.value("form_moderate/height", height()).toInt();
	int x = settings.value("form_moderate/left",   (screen.width()  - w) / 2).toInt();
	int y = settings.value("form_moderate/top",    (screen.height() - h) / 2).toInt();

	// setGeometry как-то некорректно работает :(
	resize(w, h);
	move(x, y);

	Qt::WindowStates state = static_cast<Qt::WindowStates>(settings.value("form_moderate/state", Qt::WindowNoState).toInt());

	setWindowState(state);
}
//----------------------------------------------------------------------------------------------

void FormModerateUI::combo_action_current_index_changed (const QString& /*text*/)
{
	m_combo_forum->setEnabled(m_actions[m_combo_action->currentIndex()].ForumValid);
	m_label_forum->setEnabled(m_combo_forum->isEnabled());
}
//----------------------------------------------------------------------------------------------
