#include "forum_tree.h"
//----------------------------------------------------------------------------------------------
#include "global.h"
#include "model/all.h"
#include "icon_effect.h"
#include "forms/form_date.h"
#include "tree_widget_item.h"
#include "forms/form_message.h"
#include "storage/storage_factory.h"
//----------------------------------------------------------------------------------------------
/*!
 * \brief Элемент для дерева группы форумов с информацией о группе
 */
typedef ATreeWidgetItemT<AGroupInfo> GroupTreeWidgetItem;
//----------------------------------------------------------------------------------------------
/*!
 * \brief Элемент, который хранится в каждой ноде дерева виджета списка форумов
 */
typedef struct AForumInfoGUI
{
	AForumInfo Info;          /*!< \brief Описатель форума                         */
	int        UnreadTopics;  /*!< \brief Количество непрочитанных (новых) топиков */
	int        UnreadCount;   /*!< \brief Количество непрочитанных сообщений       */
	int        UnreadCountMy; /*!< \brief Количество непрочитанных сообщений мне   */
} AForumInfoGUI;
//----------------------------------------------------------------------------------------------
/*!
 * \brief Элемент для дерева форумов с информацией о форуме
 */
typedef ATreeWidgetItemT<AForumInfoGUI> ForumTreeWidgetItem;
//----------------------------------------------------------------------------------------------

AForumTree::AForumTree (QWidget* parent, QWidget* form) : QTreeWidget (parent), IForumTree ()
{
	//
	// инициализация полей
	//

	m_parent       = form;
	m_message_tree = 0;
	m_message_view = 0;
	m_main_form    = 0;

	m_total_unread_count = 0;

	m_specail_group_count = 0;

	//
	// инициализация GUI виджета
	//

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setContextMenuPolicy(Qt::CustomContextMenu);

	setColumnCount(2);

	headerItem()->setText(0, QString::fromUtf8("форум"));
	headerItem()->setText(1, QString::fromUtf8("кол-во"));

	headerItem()->setTextAlignment(0, Qt::AlignCenter);
	headerItem()->setTextAlignment(1, Qt::AlignCenter);

	//
	// контекстное меню
	//

	m_menu = new QMenu(this);

	m_menu_mark_all_as_read = m_menu->addAction(QString::fromUtf8("Пометить все сообщения как прочитанные"));
	m_menu_mark_all_as_read->setIcon(QIcon(":/icons/markallasread16.png"));

	m_menu_mark_patrial_as_read = m_menu->addAction(QString::fromUtf8("Пометить сообщения до даты как прочитанные"));
	m_menu_mark_patrial_as_read->setIcon(QIcon(":/icons/markpatrialasread16.png"));

	m_menu->addSeparator();

	m_menu_mark_all_as_unread = m_menu->addAction(QString::fromUtf8("Пометить все сообщения как не прочитанные"));
	m_menu_mark_all_as_unread->setIcon(QIcon(":/icons/markallasunread16.png"));

	m_menu_mark_patrial_as_unread = m_menu->addAction(QString::fromUtf8("Пометить сообщения после даты как не прочитанные"));
	m_menu_mark_patrial_as_unread->setIcon(QIcon(":/icons/markpatrialasunread16.png"));

	m_menu->addSeparator();

	m_menu_unsubscribe = m_menu->addAction(QString::fromUtf8("Отписаться"));
	m_menu_unsubscribe->setIcon(QIcon(":/icons/unsubscribe16.png"));

	//
	// иконки статусов форумов
	//

	m_forum_read             = QIcon(":/icons/messageread.png");
	m_forum_unread_topics    = QIcon(":/icons/messageunread.png");
	m_forum_unread_child     = QIcon(":/icons/childunread.png");
	m_forum_unread_child_my  = AIconEffect::unionIcons(":/icons/childunread.png",   ":/icons/myunread.png");
	m_forum_unread_topics_my = AIconEffect::unionIcons(":/icons/messageunread.png", ":/icons/myunread.png");

	//
	// события виджета
	//

	connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(current_item_changed(QTreeWidgetItem*, QTreeWidgetItem*)));
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(context_menu_request(const QPoint&)));

	//
	// события меню
	//

	connect(m_menu_mark_all_as_read,       SIGNAL(triggered()), this, SLOT(menu_mark_all_as_read_triggered()));
	connect(m_menu_mark_patrial_as_read,   SIGNAL(triggered()), this, SLOT(menu_mark_patrial_as_read_triggered()));
	connect(m_menu_mark_all_as_unread,     SIGNAL(triggered()), this, SLOT(menu_mark_all_as_unread_triggered()));
	connect(m_menu_mark_patrial_as_unread, SIGNAL(triggered()), this, SLOT(menu_mark_patrial_as_unread_triggered()));
	connect(m_menu_unsubscribe,            SIGNAL(triggered()), this, SLOT(menu_unsubscribe_triggered()));
}
//----------------------------------------------------------------------------------------------

AForumTree::~AForumTree ()
{
}
//----------------------------------------------------------------------------------------------

void AForumTree::save ()
{
	QSettings settings;

	settings.setValue("forum_tree/colums/0/width", columnWidth(0));
	settings.setValue("forum_tree/colums/1/width", columnWidth(1));
}
//----------------------------------------------------------------------------------------------

void AForumTree::restore ()
{
	QSettings settings;

	setColumnWidth(0, settings.value("forum_tree/colums/0/width", 200).toInt());
	setColumnWidth(1, settings.value("forum_tree/colums/1/width", 50).toInt());
}
//----------------------------------------------------------------------------------------------

void AForumTree::setMessageTree (IMessageTree* itf)
{
	m_message_tree = itf;
}
//----------------------------------------------------------------------------------------------

void AForumTree::setMessageView (IMessageView* itf)
{
	m_message_view = itf;
}
//----------------------------------------------------------------------------------------------

void AForumTree::setMainForm (IFormMain* itf)
{
	m_main_form = itf;
}
//----------------------------------------------------------------------------------------------

void AForumTree::reload ()
{
	// очистка дерева
	clear();

	// получение хранилища
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	// получение списка форумов
	AForumGroupInfoList list;

	if (storage->getForumList(list, true) == false)
	{
		storage->showError(m_parent);
		return;
	}

	// получение количества непрочитаных сообщений
	AUnreadForumCountInfoList unread;

	if (storage->getUnreadCount(unread, AGlobal::getInstance()->Me.ID) == false)
	{
		storage->showError(m_parent);
		return;
	}

	m_total_unread_count = 0;

	//
	// добавление спец-группы с исходящими и прочими
	//

	AForumGroupInfo special_group;

	special_group.Group.ID        = SPECIAL_ID_GROUP;
	special_group.Group.Name      = QString::fromUtf8("Локальные");
	special_group.Group.SortOrder = 0;

	AForumInfo special_message;

	special_message.ID        = SPECIAL_ID_FORUM_MESSAGE2SEND;
	special_message.IDGroup   = special_group.Group.ID;
	special_message.ShortName = QString::fromUtf8("message2send");
	special_message.Name      = QString::fromUtf8("Сообщения");
	special_message.Rated     = false;
	special_message.InTop     = false;
	special_message.RateLimit = 0;

	AForumInfo special_rating;

	special_rating.ID        = SPECIAL_ID_FORUM_RATING2SEND;
	special_rating.IDGroup   = special_group.Group.ID;
	special_rating.ShortName = QString::fromUtf8("rating2send");
	special_rating.Name      = QString::fromUtf8("Оценки");
	special_rating.Rated     = false;
	special_rating.InTop     = false;
	special_rating.RateLimit = 0;

	AForumInfo special_moderate;

	special_moderate.ID        = SPECIAL_ID_FORUM_MODERATE2SEND;
	special_moderate.IDGroup   = special_group.Group.ID;
	special_moderate.ShortName = QString::fromUtf8("moderate2send");
	special_moderate.Name      = QString::fromUtf8("Модерилки");
	special_moderate.Rated     = false;
	special_moderate.InTop     = false;
	special_moderate.RateLimit = 0;

	AForumInfo special_draft;

	special_draft.ID        = SPECIAL_ID_FORUM_DRAFTS;
	special_draft.IDGroup   = special_group.Group.ID;
	special_draft.ShortName = QString::fromUtf8("drafts");
	special_draft.Name      = QString::fromUtf8("Черновики");
	special_draft.Rated     = false;
	special_draft.InTop     = false;
	special_draft.RateLimit = 0;

	AForumInfo special_my_messages;

	special_my_messages.ID        = SPECIAL_ID_FORUM_MY_MESSAGES;
	special_my_messages.IDGroup   = special_group.Group.ID;
	special_my_messages.ShortName = QString::fromUtf8("my_messages");
	special_my_messages.Name      = QString::fromUtf8("Мои сообщения");
	special_my_messages.Rated     = false;
	special_my_messages.InTop     = false;
	special_my_messages.RateLimit = 0;

	AForumInfo special_answers_to_me;

	special_answers_to_me.ID        = SPECIAL_ID_FORUM_ANSWERS_TO_ME;
	special_answers_to_me.IDGroup   = special_group.Group.ID;
	special_answers_to_me.ShortName = QString::fromUtf8("answers2me");
	special_answers_to_me.Name      = QString::fromUtf8("Ответы мне");
	special_answers_to_me.Rated     = true;
	special_answers_to_me.InTop     = false;
	special_answers_to_me.RateLimit = 0;

	special_group.Forums.append(special_message);
	special_group.Forums.append(special_rating);
	special_group.Forums.append(special_moderate);
	special_group.Forums.append(special_draft);
	special_group.Forums.append(special_my_messages);
	special_group.Forums.append(special_answers_to_me);

	m_specail_group_count = 1;

	list.insert(0, special_group);

	//
	// отображение списка форумов
	//

	for (int group_index = 0; group_index < list.count(); group_index++)
	{
		// группа
		GroupTreeWidgetItem* group_item = new GroupTreeWidgetItem(this);

		group_item->setText(0, list[group_index].Group.Name);

		group_item->setTag(list[group_index].Group);

		// форумы в группе
		for (int forum_index = 0; forum_index < list[group_index].Forums.count(); forum_index++)
		{
			ForumTreeWidgetItem* forum_item = new ForumTreeWidgetItem(group_item);

			AForumInfoGUI info;

			info.Info          = list[group_index].Forums[forum_index];
			info.UnreadCount   = 0;
			info.UnreadCountMy = 0;
			info.UnreadTopics  = 0;

			forum_item->setTag(info);

			forum_item->setText(0, list[group_index].Forums[forum_index].Name);

			forum_item->setTextAlignment(1, Qt::AlignCenter);

			/*
			// установка иконок форумов для спец группы
			// (внешний вид мне не нравится, по этому убрано)
			if (info.Info.IDGroup == SPECIAL_ID_GROUP)
			{
				if (info.Info.ID == SPECIAL_ID_FORUM_MESSAGE2SEND)
					forum_item->setIcon(0, QIcon(":/icons/new16.png"));
				else if (info.Info.ID == SPECIAL_ID_FORUM_RATING2SEND)
					forum_item->setIcon(0, QIcon(":/icons/rate_plus.png"));
				else if (info.Info.ID == SPECIAL_ID_FORUM_MODERATE2SEND)
					forum_item->setIcon(0, QIcon(":/icons/moderate.png"));
			}
			*/

			// поиск количества непрочитаных сообщений
			bool found = false;

			for (int i = 0; i < unread.count(); i++)
				if (unread[i].IDForum == list[group_index].Forums[forum_index].ID)
				{
					int unread_count = unread[i].Count;

					if (unread_count == 0)
						break;

					if (list[group_index].Forums[forum_index].IDGroup != SPECIAL_ID_GROUP)
						m_total_unread_count += unread_count;

					forum_item->pag()->UnreadCount   = unread_count;
					forum_item->pag()->UnreadCountMy = unread[i].CountMy;
					forum_item->pag()->UnreadTopics  = unread[i].Topics;

					forum_item->setText(1, QString::number(unread_count));

					QFont font = forum_item->font(1);
					font.setBold(true);
					forum_item->setFont(1, font);

					if (list[group_index].Forums[forum_index].IDGroup != SPECIAL_ID_GROUP)
					{
						if (unread[i].CountMy > 0)
						{
							if (unread[i].Topics > 0)
								forum_item->setIcon(0, m_forum_unread_topics_my);
							else
								forum_item->setIcon(0, m_forum_unread_child_my);
						}
						else
						{
							if (unread[i].Topics > 0)
								forum_item->setIcon(0, m_forum_unread_topics);
							else
								forum_item->setIcon(0, m_forum_unread_child);
						}
					}

					found = true;

					break;
				}

			if (found == false)
			{
				forum_item->setText(1, "-");

				QFont font = forum_item->font(1);
				font.setBold(false);
				forum_item->setFont(1, font);

				if (list[group_index].Forums[forum_index].IDGroup != SPECIAL_ID_GROUP)
					forum_item->setIcon(0, m_forum_read);
			}
		}

		group_item->setExpanded(true);
	}

	// установка возможных действий
	if (m_main_form != NULL)
	{
		if (list.count() > m_specail_group_count)
		{
			if (m_total_unread_count > 0)
				m_main_form->setEnabledAction(aaPrevNextUnreadForum, true);
			else
				m_main_form->setEnabledAction(aaPrevNextUnreadForum, false);
		}
		else
			m_main_form->setEnabledAction(aaPrevNextUnreadForum, false);
	}
}
//----------------------------------------------------------------------------------------------

void AForumTree::reloadUnread (bool reload_message_tree)
{
	// получение хранилища
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	// получение количества непрочитаных сообщений
	AUnreadForumCountInfoList unread;

	if (storage->getUnreadCount(unread, AGlobal::getInstance()->Me.ID) == false)
	{
		storage->showError(m_parent);
		return;
	}

	m_total_unread_count = 0;

	// отображение информации
	for (int group_index = 0; group_index < topLevelItemCount(); group_index++)
	{
		QTreeWidgetItem* group_item = topLevelItem(group_index);

		for (int forum_index = 0; forum_index < group_item->childCount(); forum_index++)
		{
			ForumTreeWidgetItem* forum_item = static_cast<ForumTreeWidgetItem*>(group_item->child(forum_index));

			AForumInfoGUI* pitem = forum_item->pag();

			// поиск количества непрочитаных сообщений
			bool found = false;

			for (int i = 0; i < unread.count(); i++)
				if (unread[i].IDForum == pitem->Info.ID)
				{
					int unread_count = unread[i].Count;

					if (unread_count == 0)
						break;

					if (pitem->Info.IDGroup != SPECIAL_ID_GROUP)
						m_total_unread_count += unread_count;

					pitem->UnreadCount   = unread_count;
					pitem->UnreadCountMy = unread[i].CountMy;
					pitem->UnreadTopics  = unread[i].Topics;

					forum_item->setText(1, QString::number(unread_count));

					QFont font = forum_item->font(1);
					font.setBold(true);
					forum_item->setFont(1, font);

					if (pitem->Info.IDGroup != SPECIAL_ID_GROUP)
					{
						if (unread[i].CountMy > 0)
						{
							if (unread[i].Topics > 0)
								forum_item->setIcon(0, m_forum_unread_topics_my);
							else
								forum_item->setIcon(0, m_forum_unread_child_my);
						}
						else
						{
							if (unread[i].Topics > 0)
								forum_item->setIcon(0, m_forum_unread_topics);
							else
								forum_item->setIcon(0, m_forum_unread_child);
						}
					}

					found = true;

					break;
				}

			if (found == false)
			{
				pitem->UnreadCount   = 0;
				pitem->UnreadCountMy = 0;

				forum_item->setText(1, "-");

				QFont font = forum_item->font(1);
				font.setBold(false);
				forum_item->setFont(1, font);

				if (pitem->Info.IDGroup != SPECIAL_ID_GROUP)
					forum_item->setIcon(0, m_forum_read);
			}
		}
	}

	// установка возможных действий
	if (m_total_unread_count > 0)
		m_main_form->setEnabledAction(aaPrevNextUnreadForum, true);
	else
		m_main_form->setEnabledAction(aaPrevNextUnreadForum, false);

	// перезагрузка дерева сообщений
	if (reload_message_tree == true)
		current_item_changed(currentItem(), currentItem());
        else
	{
		// если перезагрузка не указана, но текущая группа специальная
		QTreeWidgetItem* item = getSelectedForumItem();

		if (item != NULL && isGroup(item) == false)
		{
			AForumInfoGUI* info = static_cast<ForumTreeWidgetItem*>(item)->pag();

			if (info->Info.ID == SPECIAL_ID_FORUM_MESSAGE2SEND || info->Info.ID == SPECIAL_ID_FORUM_DRAFTS)
				current_item_changed(currentItem(), currentItem());
		}
	}
}
//----------------------------------------------------------------------------------------------

bool AForumTree::isGroup (QTreeWidgetItem* item)
{
	return item->childCount() != 0;
}
//----------------------------------------------------------------------------------------------

QTreeWidgetItem* AForumTree::getSelectedForumItem ()
{
	QTreeWidgetItem* item = currentItem();

	if (item == NULL)
		return NULL;

	if (isGroup(item) == true)
		return NULL;

	return item;
}
//----------------------------------------------------------------------------------------------

int AForumTree::getSelectedUnreadCount ()
{
	ForumTreeWidgetItem* forum_item = static_cast<ForumTreeWidgetItem*>(getSelectedForumItem());

	if (forum_item == NULL)
		return -1;

	return forum_item->pag()->UnreadCount;
}
//----------------------------------------------------------------------------------------------

void AForumTree::current_item_changed (QTreeWidgetItem* /*current*/, QTreeWidgetItem* /*previous*/)
{
	ForumTreeWidgetItem* forum_item = static_cast<ForumTreeWidgetItem*>(getSelectedForumItem());

	if (m_main_form != NULL)
	{
		// установка возможных действий
		m_main_form->setEnabledAction(aaNewMessage,            false);
		m_main_form->setEnabledAction(aaPrevNextUnreadArticle, false);
		m_main_form->setEnabledAction(aaPrevNextUnreadThread,  false);
	}

	if (forum_item != NULL)
	{
		m_message_tree->changeForum(&forum_item->pag()->Info);

		if (m_main_form != NULL)
		{
			if (forum_item->pag()->Info.IDGroup != SPECIAL_ID_GROUP)
			{
				// установка возможных действий
				m_main_form->setEnabledAction(aaNewMessage, true);

				if (forum_item->pag()->UnreadCount != 0)
				{
					m_main_form->setEnabledAction(aaPrevNextUnreadArticle, true);
					m_main_form->setEnabledAction(aaPrevNextUnreadThread,  true);
				}
			}
		}
	}
	else
		m_message_tree->changeForum(NULL);
}
//----------------------------------------------------------------------------------------------

void AForumTree::changeUnreadCount (int count, int count_my, int count_topics, int id_forum)
{
	ForumTreeWidgetItem* forum_item = NULL;

	if (id_forum == 0)
		forum_item = static_cast<ForumTreeWidgetItem*>(getSelectedForumItem());
	else
	{
		for (int group_index = 0; group_index < topLevelItemCount(); group_index++)
		{
			QTreeWidgetItem* group_item = topLevelItem(group_index);

			for (int forum_index = 0; forum_index < group_item->childCount(); forum_index++)
			{
				ForumTreeWidgetItem* forum_item_temp = static_cast<ForumTreeWidgetItem*>(group_item->child(forum_index));

				if (forum_item_temp->pag()->Info.ID == id_forum)
				{
					forum_item = forum_item_temp;
					break;
				}
			}

			if (forum_item != NULL)
				break;
		}
	}

	if (forum_item == NULL)
		return;

	AForumInfoGUI* info = forum_item->pag();

	info->UnreadCount   += count;
	info->UnreadCountMy += count_my;
	info->UnreadTopics  += count_topics;

	if (forum_item->pag()->UnreadCount != 0)
	{
		forum_item->setText(1, QString::number(forum_item->pag()->UnreadCount));

		QFont font = forum_item->font(1);
		font.setBold(true);
		forum_item->setFont(1, font);

		if (info->Info.IDGroup != SPECIAL_ID_GROUP)
		{
			if (info->UnreadCountMy > 0)
			{
				if (info->UnreadTopics > 0)
					forum_item->setIcon(0, m_forum_unread_topics_my);
				else
					forum_item->setIcon(0, m_forum_unread_child_my);
			}
			else
			{
				if (info->UnreadTopics > 0)
					forum_item->setIcon(0, m_forum_unread_topics);
				else
					forum_item->setIcon(0, m_forum_unread_child);
			}
		}

		// установка возможных действий
		m_main_form->setEnabledAction(aaPrevNextUnreadArticle, true);
		m_main_form->setEnabledAction(aaPrevNextUnreadThread,  true);
	}
	else
	{
		forum_item->setText(1, "-");

		QFont font = forum_item->font(1);
		font.setBold(false);
		forum_item->setFont(1, font);

		if (info->Info.IDGroup != SPECIAL_ID_GROUP)
			forum_item->setIcon(0, m_forum_read);

		// установка возможных действий
		m_main_form->setEnabledAction(aaPrevNextUnreadArticle, false);
		m_main_form->setEnabledAction(aaPrevNextUnreadThread,  false);
	}

	if (info->Info.IDGroup != SPECIAL_ID_GROUP)
	{
		m_total_unread_count += count;

		// установка возможных действий
		if (m_total_unread_count > 0)
			m_main_form->setEnabledAction(aaPrevNextUnreadForum, true);
		else
			m_main_form->setEnabledAction(aaPrevNextUnreadForum, false);
	}
}
//----------------------------------------------------------------------------------------------

void AForumTree::context_menu_request (const QPoint& pos)
{
	QTreeWidgetItem* item = currentItem();

	if (item == NULL)
		return;

	// проверка на спец-директорию
	if (isGroup(item) == true)
	{
		GroupTreeWidgetItem* group_item = static_cast<GroupTreeWidgetItem*>(item);

		if (group_item->pag()->ID == SPECIAL_ID_GROUP)
			return;
	}
	else
	{
		ForumTreeWidgetItem* forum_item = static_cast<ForumTreeWidgetItem*>(item);

		if (forum_item->pag()->Info.IDGroup == SPECIAL_ID_GROUP)
		{
			if (forum_item->pag()->Info.ID == SPECIAL_ID_FORUM_MY_MESSAGES || forum_item->pag()->Info.ID == SPECIAL_ID_FORUM_ANSWERS_TO_ME)
				m_menu_unsubscribe->setVisible(false);
			else
				return;
		}
		else
			m_menu_unsubscribe->setVisible(true);
	}

	// не спец-директория - отображаем группу
	m_menu->exec(viewport()->mapToGlobal(pos));
}
//----------------------------------------------------------------------------------------------

void AForumTree::menu_mark_all_as_read_triggered ()
{
	QTreeWidgetItem* item = currentItem();

	if (item == NULL)
		return;

	// получение хранилища
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	if (isGroup(item) == true)
	{
		int id = (static_cast<GroupTreeWidgetItem*>(item))->pag()->ID;

		if (storage->setIDsAsRead(QList<int>() << id, idsGroup, true, QDateTime()) == false)
		{
			storage->showError(m_parent);
			return;
		}
	}
	else
	{
		AForumInfoGUI* info = (static_cast<ForumTreeWidgetItem*>(item))->pag();

		if (storage->setIDsAsRead(QList<int>() << info->Info.ID, idsForum, true, QDateTime()) == false)
		{
			storage->showError(m_parent);
			return;
		}

		// обновление дерева сообщений
		m_message_tree->changeForum(&info->Info);
	}

	// перезагрузка количества непрочитаных
	reloadUnread(true);
}
//----------------------------------------------------------------------------------------------

void AForumTree::menu_mark_patrial_as_read_triggered ()
{
	QTreeWidgetItem* item = currentItem();

	if (item == NULL)
		return;

	std::auto_ptr<FormDate> form(new FormDate(m_parent, true));

	if (form->exec() == QDialog::Accepted)
	{
		// получение хранилища
		std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

		if (storage.get() == NULL)
		{
			QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
			return;
		}

		if (isGroup(item) == true)
		{
			int id = (static_cast<GroupTreeWidgetItem*>(item))->pag()->ID;

			if (storage->setIDsAsRead(QList<int>() << id, idsGroup, true, form->SelectedDate()) == false)
			{
				storage->showError(m_parent);
				return;
			}
		}
		else
		{
			AForumInfoGUI* info = (static_cast<ForumTreeWidgetItem*>(item))->pag();

			if (storage->setIDsAsRead(QList<int>() << info->Info.ID, idsForum, true, form->SelectedDate()) == false)
			{
				storage->showError(m_parent);
				return;
			}

			// обновление дерева сообщений
			m_message_tree->changeForum(&info->Info);
		}

		// перезагрузка количества непрочитаных
		reloadUnread(true);
	}
}
//----------------------------------------------------------------------------------------------

void AForumTree::menu_mark_all_as_unread_triggered ()
{
	QTreeWidgetItem* item = currentItem();

	if (item == NULL)
		return;

	// получение хранилища
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	if (isGroup(item) == true)
	{
		// однако, когда база 6 гигов и промахнешься пунктом меню, то наступает локальный коллапс :)
		QMessageBox::StandardButton result = QMessageBox::question(this, QString::fromUtf8("Внимание!"), QString::fromUtf8("Операция может занять (очень) длительное время! Вы уверены что вы хотите пометить <u><b>все</b></u> сообщения в этой группе как <u><b>непрочитанные</b></u>?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

		if (result != QMessageBox::Yes)
			return;

		int id = (static_cast<GroupTreeWidgetItem*>(item))->pag()->ID;

		if (storage->setIDsAsRead(QList<int>() << id, idsGroup, false, QDateTime()) == false)
		{
			storage->showError(m_parent);
			return;
		}
	}
	else
	{
		// однако, когда база 6 гигов и промахнешься пунктом меню, то наступает локальный коллапс :)
		QMessageBox::StandardButton result = QMessageBox::question(this, QString::fromUtf8("Внимание!"), QString::fromUtf8("Операция может занять (очень) длительное время! Вы уверены что вы хотите пометить <u><b>все</b></u> сообщения в этом форуме как <u><b>непрочитанные</b></u>?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

		if (result != QMessageBox::Yes)
			return;

		AForumInfoGUI* info = (static_cast<ForumTreeWidgetItem*>(item))->pag();

		if (storage->setIDsAsRead(QList<int>() << info->Info.ID, idsForum, false, QDateTime()) == false)
		{
			storage->showError(m_parent);
			return;
		}

		// обновление дерева сообщений
		m_message_tree->changeForum(&info->Info);
	}

	// перезагрузка количества непрочитаных
	reloadUnread(true);
}
//----------------------------------------------------------------------------------------------

void AForumTree::menu_mark_patrial_as_unread_triggered ()
{
	QTreeWidgetItem* item = currentItem();

	if (item == NULL)
		return;

	std::auto_ptr<FormDate> form(new FormDate(m_parent, false));

	if (form->exec() == QDialog::Accepted)
	{
		// получение хранилища
		std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

		if (storage.get() == NULL)
		{
			QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
			return;
		}

		if (isGroup(item) == true)
		{
			int id = (static_cast<GroupTreeWidgetItem*>(item))->pag()->ID;

			if (storage->setIDsAsRead(QList<int>() << id, idsGroup, false, form->SelectedDate()) == false)
			{
				storage->showError(m_parent);
				return;
			}
		}
		else
		{
			AForumInfoGUI* info = (static_cast<ForumTreeWidgetItem*>(item))->pag();

			if (storage->setIDsAsRead(QList<int>() << info->Info.ID, idsForum, false, form->SelectedDate()) == false)
			{
				storage->showError(m_parent);
				return;
			}

			// обновление дерева сообщений
			m_message_tree->changeForum(&info->Info);
		}

		// перезагрузка количества непрочитаных
		reloadUnread(true);
	}
}
//----------------------------------------------------------------------------------------------

void AForumTree::menu_unsubscribe_triggered ()
{
	QTreeWidgetItem* item = currentItem();

	if (item == NULL)
		return;

	QString from = QString::fromUtf8("группы");

	if (isGroup(item) == false)
		from = QString::fromUtf8("форума");

	// демократия
	if (QMessageBox::question(m_parent, QString::fromUtf8("Внимание!"), QString::fromUtf8("Вы уверены, что хотите отписаться от ") + from + "?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) != QMessageBox::Yes)
		return;

	QMessageBox::StandardButton answer = QMessageBox::question(m_parent, QString::fromUtf8("Внимание!"), QString::fromUtf8("Удалить сообщения ") + from + QString::fromUtf8(" из хранилища данных?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel);

	if (answer == QMessageBox::Cancel)
		return;

	bool clean = false;

	if (answer == QMessageBox::Yes)
		clean = true;

	// получение хранилища
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	if (isGroup(item) == true)
	{
		int id = (static_cast<GroupTreeWidgetItem*>(item))->pag()->ID;

		if (storage->unsubscribe(QList<int>() << id, idsGroup, clean) == false)
		{
			storage->showError(m_parent);
			return;
		}
	}
	else
	{
		int id = (static_cast<ForumTreeWidgetItem*>(item))->pag()->Info.ID;

		if (storage->unsubscribe(QList<int>() << id, idsForum, clean) == false)
		{
			storage->showError(m_parent);
			return;
		}
	}

	// обновление дерева сообщений
	m_message_tree->changeForum(NULL);

	// перезагрузка дерева
	reload();
}
//----------------------------------------------------------------------------------------------

void AForumTree::newMessage ()
{
	QTreeWidgetItem* item = currentItem();

	if (item == NULL)
		return;

	// проверка спец-группы
	if (isGroup(item) == true)
		return;

	ForumTreeWidgetItem* forum_item = static_cast<ForumTreeWidgetItem*>(item);

	if (forum_item->pag()->Info.IDGroup == SPECIAL_ID_GROUP)
		return;

	AMessageInfoGUI info;

	info.ID             = 0;
	info.IDParent       = 0;
	info.IDUser         = 0;
	info.IDForum        = forum_item->pag()->Info.ID;
	info.Subject        = QString::fromUtf8("(тема сообщения)");
	info.Message        = "Hello!\n\n";
	info.MessageDate    = QDateTime::currentDateTime();

	FormMessage* form = new FormMessage(NULL, false, info);

	form->setForumTree(this);
	form->setMainForm(m_main_form);

	form->show();
}
//----------------------------------------------------------------------------------------------

bool AForumTree::checkCurrentItem ()
{
	if (currentItem() != NULL)
		return true;

	if (topLevelItemCount() == 0)
		return false;

	setCurrentItem(topLevelItem(0));

	return true;
}
//----------------------------------------------------------------------------------------------

void AForumTree::gotoNextUnreadForum ()
{
	if (checkCurrentItem() == false)
		return;

	QTreeWidgetItem* item = currentItem();

	while (true)
	{
		QTreeWidgetItem* next_item = ATreeWidgetItem::nextItem(item);

		if (next_item == NULL)
			next_item = topLevelItem(0);

		if (isGroup(next_item) == false)
		{
			ForumTreeWidgetItem* forum_item = static_cast<ForumTreeWidgetItem*>(next_item);

			if (forum_item->pag()->Info.IDGroup != SPECIAL_ID_GROUP && forum_item->pag()->UnreadCount > 0)
			{
				setCurrentItem(next_item);
				break;
			}
		}

		item = next_item;
	}
}
//----------------------------------------------------------------------------------------------

bool AForumTree::selectForum (int id)
{
	for (int group_index = 0; group_index < topLevelItemCount(); group_index++)
	{
		QTreeWidgetItem* group_item = topLevelItem(group_index);

		for (int forum_index = 0; forum_index < group_item->childCount(); forum_index++)
		{
			ForumTreeWidgetItem* forum_item = static_cast<ForumTreeWidgetItem*>(group_item->child(forum_index));

			if (forum_item->pag()->Info.ID == id)
			{
				setCurrentItem(forum_item);
				return true;
			}
		}
	}

	return false;
}
//----------------------------------------------------------------------------------------------
