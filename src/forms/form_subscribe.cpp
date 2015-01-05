#include "form_subscribe.h"
//----------------------------------------------------------------------------------------------
#include "form_request.h"
#include "webservice.h"
#include "widgets/tree_widget_item.h"
#include "storage/storage_factory.h"
//----------------------------------------------------------------------------------------------
/*!
 * \brief Элемент для дерева форумов с информацией о форуме
 */
typedef ATreeWidgetItemT<AForumInfo> ForumTreeWidgetItem;
//----------------------------------------------------------------------------------------------

FormSubscribe::FormSubscribe (QWidget* parent) :  FormSubscribeUI (parent)
{
	connect(m_button_ok,          SIGNAL(clicked()), this, SLOT(button_ok_clicked()));
	connect(m_button_refresh,     SIGNAL(clicked()), this, SLOT(button_refresh_clicked()));
	connect(m_button_select_all,  SIGNAL(clicked()), this, SLOT(button_select_all_clicked()));
	connect(m_button_select_none, SIGNAL(clicked()), this, SLOT(button_select_none_clicked()));
	connect(m_button_cancel,      SIGNAL(clicked()), this, SLOT(reject()));

	// загрузка дерева
	reload();
}
//----------------------------------------------------------------------------------------------

FormSubscribe::~FormSubscribe ()
{
}
//----------------------------------------------------------------------------------------------

void FormSubscribe::reload ()
{
	// очистка дерева
	m_tree_forum->clear();

	m_last_subscribed.clear();

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

	// получение списка подписаных форумов
	ASubscribedForumInfoList list_subscribed;

	if (storage->getSubscribedForumList(list_subscribed) == false)
	{
		storage->showError(this);
		return;
	}

	// отображение списка форумов
	for (int group_index = 0; group_index < list.count(); group_index++)
	{
		// группа
		QTreeWidgetItem* group_item = new QTreeWidgetItem(m_tree_forum);

		group_item->setText(0, list[group_index].Group.Name);

		// форумы в группе
		for (int forum_index = 0; forum_index < list[group_index].Forums.count(); forum_index++)
		{
			ForumTreeWidgetItem* forum_item = new ForumTreeWidgetItem(group_item);

			forum_item->setTag(list[group_index].Forums[forum_index]);

			forum_item->setText(0, list[group_index].Forums[forum_index].Name);

			forum_item->setCheckState(0, Qt::Unchecked);

			// определение подписки на данный форум
			for (int subscribed_index = 0; subscribed_index < list_subscribed.count(); subscribed_index++)
				if (list_subscribed[subscribed_index].IDForum == list[group_index].Forums[forum_index].ID)
				{
					m_last_subscribed.append(list_subscribed[subscribed_index]);

					forum_item->setCheckState(0, Qt::Checked);
				}
		}

		group_item->setExpanded(true);
	}
}
//----------------------------------------------------------------------------------------------

void FormSubscribe::button_ok_clicked ()
{
	// создание списка подписаных форумов
	ASubscribedForumInfoList list;

	for (int group_index = 0; group_index < m_tree_forum->topLevelItemCount(); group_index++)
	{
		QTreeWidgetItem* group_item = m_tree_forum->topLevelItem(group_index);

		for (int forum_index = 0; forum_index < group_item->childCount(); forum_index++)
		{
			ForumTreeWidgetItem* forum_item = static_cast<ForumTreeWidgetItem*>(group_item->child(forum_index));

			if (forum_item->checkState(0) == Qt::Checked)
			{
				ASubscribedForumInfo info;

				info.IDForum = forum_item->pag()->ID;

				list.append(info);
			}
		}
	}

	// получение хранилища
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	// проверка отписаных форумов
	bool clean = false;
	QList<int> list_unsubscribe;

	for (int i = 0; i < m_last_subscribed.count(); i++)
	{
		bool found = false;

		for (int j = 0; j < list.count(); j++)
			if (m_last_subscribed[i].IDForum == list[j].IDForum)
			{
				found = true;
				break;
			}

		if (found == false)
		{
			if (list_unsubscribe.count() == 0)
			{
				QMessageBox::StandardButton answer = QMessageBox::question(this, QString::fromUtf8("Внимание!"), QString::fromUtf8("Вы отказались от подписки одного или нескольких форумов.\nУдалить сообщения этих форумов из хранилища данных?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel);

				if (answer == QMessageBox::Cancel)
					return;
				else if (answer == QMessageBox::Yes)
					clean = true;
			}

			list_unsubscribe.append(m_last_subscribed[i].IDForum);
		}
	}

	// удаление сообщений из старой подписки
	if (list_unsubscribe.count() > 0)
		if (storage->unsubscribe(list_unsubscribe, idsForum, clean) == false)
		{
			storage->showError(this);
			return;
		}

	// сохранение подписаных форумов
	if (storage->setSubscribedForumList(list) == false)
	{
		storage->showError(this);
		return;
	}

	// принятие диалога
	accept();
}
//----------------------------------------------------------------------------------------------

void FormSubscribe::button_refresh_clicked ()
{
	AWebservice webservice(this);

	AForumGroupInfoList list;
	if (webservice.getForumList(list) == true)
	{
		// получение хранилища
		std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

		if (storage.get() == NULL)
		{
			QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
			return;
		}

		// сохранение списка форумов
		if (storage->setForumList(list) == false)
		{
			storage->showError(this);
			return;
		}

		// перезагрузка
		reload();
	}
	else
		QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), webservice.error());
}
//----------------------------------------------------------------------------------------------

void FormSubscribe::button_select_all_clicked ()
{
	for (int group_index = 0; group_index < m_tree_forum->topLevelItemCount(); group_index++)
	{
		QTreeWidgetItem* group_item = m_tree_forum->topLevelItem(group_index);

		for (int forum_index = 0; forum_index < group_item->childCount(); forum_index++)
		{
			QTreeWidgetItem* forum_item = group_item->child(forum_index);
			forum_item->setCheckState(0, Qt::Checked);
		}
	}
}
//----------------------------------------------------------------------------------------------

void FormSubscribe::button_select_none_clicked ()
{
	for (int group_index = 0; group_index < m_tree_forum->topLevelItemCount(); group_index++)
	{
		QTreeWidgetItem* group_item = m_tree_forum->topLevelItem(group_index);

		for (int forum_index = 0; forum_index < group_item->childCount(); forum_index++)
		{
			QTreeWidgetItem* forum_item = group_item->child(forum_index);
			forum_item->setCheckState(0, Qt::Unchecked);
		}
	}
}
//----------------------------------------------------------------------------------------------
