#include "message_tree.h"
//----------------------------------------------------------------------------------------------
#include "global.h"
#include "icon_effect.h"
#include "tree_widget_item.h"
#include "forms/form_source.h"
#include "forms/form_message.h"
#include "forms/form_moderate.h"
#include "storage/storage_factory.h"
//----------------------------------------------------------------------------------------------
/*!
 * \brief Максимальная длина истории для навигации Вперед / Назад
 * Сделано исключительно на всякий случай, для любителей не закрывать приложения на ноутах :)
 * Т.к. в обычной жизни длина истории врядли будет превышать 10-100
 */
#define MAX_HISTORY_LENGTH 1024
//----------------------------------------------------------------------------------------------
/*!
 * \brief Элемент для дерева сообщений
 */
typedef ATreeWidgetItemT<AMessageInfoGUI> MessageTreeWidgetItem;
//----------------------------------------------------------------------------------------------

AMessageTree::AMessageTree (QWidget* parent, QWidget* form) : QTreeWidget (parent), IMessageTree ()
{
	//
	// инициализация полей
	//

	m_parent        = form;
	m_forum_tree    = 0;
	m_message_view  = 0;
	m_main_form     = 0;
	m_history_index = -1;

	m_current_forum.ID        = 0;
	m_current_forum.IDGroup   = 0;
	m_current_forum.Rated     = false;
	m_current_forum.InTop     = false;
	m_current_forum.RateLimit = 0;

	m_max_topic_to_show  = 0;
	m_mark_as_read_timer = 500;

	//
	// инициализация GUI виджета
	//

#ifndef Q_WS_WIN
	setFrameShadow(QFrame::Plain);
#endif

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setContextMenuPolicy(Qt::CustomContextMenu);

	setColumnCount(3);

	headerItem()->setText(0, QString::fromUtf8("тема"));
	headerItem()->setText(1, QString::fromUtf8("автор"));
	headerItem()->setText(2, QString::fromUtf8("дата"));

	headerItem()->setTextAlignment(0, Qt::AlignCenter);
	headerItem()->setTextAlignment(1, Qt::AlignCenter);
	headerItem()->setTextAlignment(2, Qt::AlignCenter);

	//
	// иконки
	//

	m_message_read   = QIcon(":/icons/messageread.png");
	m_child_unread   = QIcon(":/icons/childunread.png");
	m_message_unread = QIcon(":/icons/messageunread.png");

	m_child_unread_my   = AIconEffect::unionIcons(":/icons/childunread.png",   ":/icons/myunread.png");
	m_message_unread_my = AIconEffect::unionIcons(":/icons/messageunread.png", ":/icons/myunread.png");

	m_rating_plus_1 = QIcon(":/icons/rate_plus_1.png");
	m_rating_1      = QIcon(":/icons/rate_1.png");
	m_rating_2      = QIcon(":/icons/rate_2.png");
	m_rating_3      = QIcon(":/icons/rate_3.png");
	m_rating_cross  = QIcon(":/icons/rate_cross.png");
	m_rating_smile  = QIcon(":/icons/rate_smile.png");
	m_rating_plus   = QIcon(":/icons/rate_plus.png");
	m_rating_minus  = QIcon(":/icons/rate_minus.png");

	m_moderate      = QIcon(":/icons/moderate.png");

	//
	// контекстное меню (для основного дерева сообщений)
	//

	m_menu = new QMenu(this);

	m_menu_reply = m_menu->addAction(QString::fromUtf8("Ответить"));
	m_menu_reply->setIcon(QIcon(":/icons/reply16.png"));

	m_menu->addSeparator();

	m_menu_mark_message_as_read = m_menu->addAction(QString::fromUtf8("Пометить сообщение как прочитанное"));
	m_menu_mark_message_as_read->setIcon(QIcon(":/icons/messageread.png"));

	m_menu_mark_message_as_unread = m_menu->addAction(QString::fromUtf8("Пометить сообщение как непрочитанное"));
	m_menu_mark_message_as_unread->setIcon(QIcon(":/icons/messageunread.png"));

	m_menu->addSeparator();

	m_menu_mark_thread_as_read = m_menu->addAction(QString::fromUtf8("Пометить ветку как прочитанную"));
	m_menu_mark_thread_as_read->setIcon(QIcon(":/icons/markpatrialasread16.png"));

	m_menu_mark_thread_as_unread = m_menu->addAction(QString::fromUtf8("Пометить ветку как непрочитанную"));
	m_menu_mark_thread_as_unread->setIcon(QIcon(":/icons/markpatrialasunread16.png"));

	//
	// контекстное спец-меню (для оценок, модерилок и исходящих сообщений)
	//

	m_menu_special = new QMenu(this);

	m_menu_special_plus_1 = m_menu_special->addAction(QString::fromUtf8("+1"));
	m_menu_special_plus_1->setIcon(m_rating_plus_1);

	m_menu_special_1 = m_menu_special->addAction(QString::fromUtf8("Интересно"));
	m_menu_special_1->setIcon(m_rating_1);

	m_menu_special_2 = m_menu_special->addAction(QString::fromUtf8("Спасибо"));
	m_menu_special_2->setIcon(m_rating_2);

	m_menu_special_3 = m_menu_special->addAction(QString::fromUtf8("Супер"));
	m_menu_special_3->setIcon(m_rating_3);

	m_menu_special_smile = m_menu_special->addAction(QString::fromUtf8(":)"));
	m_menu_special_smile->setIcon(m_rating_smile);

	m_menu_special_plus = m_menu_special->addAction(QString::fromUtf8("Согласен"));
	m_menu_special_plus->setIcon(m_rating_plus);

	m_menu_special_minus = m_menu_special->addAction(QString::fromUtf8("Несогласен"));
	m_menu_special_minus->setIcon(m_rating_minus);

	m_menu_special->addSeparator();

	m_menu_special_edit = m_menu_special->addAction(QString::fromUtf8("Изменить"));
	m_menu_special_edit->setIcon(QIcon(":/icons/edit16.png"));

	m_menu_special_delete = m_menu_special->addAction(QString::fromUtf8("Удалить"));
	m_menu_special_delete->setIcon(QIcon(":/icons/delete16.png"));

	//
	// события меню
	//

	connect(m_menu_reply,                  SIGNAL(triggered()), this, SLOT(menu_reply_triggered()));
	connect(m_menu_mark_message_as_read,   SIGNAL(triggered()), this, SLOT(menu_mark_message_as_read_triggered()));
	connect(m_menu_mark_message_as_unread, SIGNAL(triggered()), this, SLOT(menu_mark_message_as_unread_triggered()));
	connect(m_menu_mark_thread_as_read,    SIGNAL(triggered()), this, SLOT(menu_mark_thread_as_read_triggered()));
	connect(m_menu_mark_thread_as_unread,  SIGNAL(triggered()), this, SLOT(menu_mark_thread_as_unread_triggered()));

	//
	// события спец-меню
	//

	connect(m_menu_special_delete, SIGNAL(triggered()), this, SLOT(menu_special_delete_triggered()));
	connect(m_menu_special_edit,   SIGNAL(triggered()), this, SLOT(menu_special_edit_triggered()));

	connect(m_menu_special_plus_1, SIGNAL(triggered()), this, SLOT(menu_special_plus_1_triggered()));
	connect(m_menu_special_1,      SIGNAL(triggered()), this, SLOT(menu_special_1_triggered()));
	connect(m_menu_special_2,      SIGNAL(triggered()), this, SLOT(menu_special_2_triggered()));
	connect(m_menu_special_3,      SIGNAL(triggered()), this, SLOT(menu_special_3_triggered()));
	connect(m_menu_special_smile,  SIGNAL(triggered()), this, SLOT(menu_special_smile_triggered()));
	connect(m_menu_special_plus,   SIGNAL(triggered()), this, SLOT(menu_special_plus_triggered()));
	connect(m_menu_special_minus,  SIGNAL(triggered()), this, SLOT(menu_special_minus_triggered()));

	//
	// события виджета
	//

	connect(this, SIGNAL(itemSelectionChanged()),                    this, SLOT(selection_changed()));
	connect(this, SIGNAL(itemExpanded(QTreeWidgetItem*)),            this, SLOT(expand_item(QTreeWidgetItem*)));
	connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),  this, SLOT(item_double_clicked(QTreeWidgetItem*, int)));
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(context_menu_request(const QPoint&)));

	connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(vertical_scroll(int)));

	// событие таймера отметки как прочитанное
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timer_on_timer()));
}
//----------------------------------------------------------------------------------------------

AMessageTree::~AMessageTree ()
{
}
//----------------------------------------------------------------------------------------------

void AMessageTree::resizeEvent (QResizeEvent* event)
{
	QTreeWidget::resizeEvent(event);

	// при ресайзе по высоте могут внизу появиться незагруженные топики
	scrollTopics();
}
//----------------------------------------------------------------------------------------------

void AMessageTree::save ()
{
	QSettings settings;

	settings.setValue("message_tree/colums/0/width", columnWidth(0));
	settings.setValue("message_tree/colums/1/width", columnWidth(1));
	settings.setValue("message_tree/colums/2/width", columnWidth(2));
}
//----------------------------------------------------------------------------------------------

void AMessageTree::restore ()
{
	QSettings settings;

	setColumnWidth(0, settings.value("message_tree/colums/0/width", 700).toInt());
	setColumnWidth(1, settings.value("message_tree/colums/1/width", 100).toInt());
	setColumnWidth(2, settings.value("message_tree/colums/2/width", 100).toInt());
}
//----------------------------------------------------------------------------------------------

void AMessageTree::setForumTree (IForumTree* itf)
{
	m_forum_tree = itf;
}
//----------------------------------------------------------------------------------------------

void AMessageTree::setMessageView (IMessageView* itf)
{
	m_message_view = itf;
}
//----------------------------------------------------------------------------------------------

void AMessageTree::setMainForm (IFormMain* itf)
{
	m_main_form = itf;
}
//----------------------------------------------------------------------------------------------

void AMessageTree::context_menu_request (const QPoint& pos)
{
	if (currentItem() != NULL)
	{
		if (m_current_forum.IDGroup != SPECIAL_ID_GROUP || m_current_forum.ID == SPECIAL_ID_FORUM_MY_MESSAGES || m_current_forum.ID == SPECIAL_ID_FORUM_ANSWERS_TO_ME)
			m_menu->exec(viewport()->mapToGlobal(pos));
		else
		{
			if (m_current_forum.ID == SPECIAL_ID_FORUM_MESSAGE2SEND  ||
			    m_current_forum.ID == SPECIAL_ID_FORUM_MODERATE2SEND ||
			    m_current_forum.ID == SPECIAL_ID_FORUM_DRAFTS
			   )
			{
				m_menu_special_edit->setVisible(true);

				bool rate_visible = false;

				m_menu_special_plus_1->setVisible(rate_visible);
				m_menu_special_1->setVisible(rate_visible);
				m_menu_special_2->setVisible(rate_visible);
				m_menu_special_3->setVisible(rate_visible);
				m_menu_special_smile->setVisible(rate_visible);
				m_menu_special_plus->setVisible(rate_visible);
				m_menu_special_minus->setVisible(rate_visible);
			}
			else
			{
				m_menu_special_edit->setVisible(false);

				bool rate_visible = true;

				m_menu_special_plus_1->setVisible(rate_visible);
				m_menu_special_1->setVisible(rate_visible);
				m_menu_special_2->setVisible(rate_visible);
				m_menu_special_3->setVisible(rate_visible);
				m_menu_special_smile->setVisible(rate_visible);
				m_menu_special_plus->setVisible(rate_visible);
				m_menu_special_minus->setVisible(rate_visible);
			}

			m_menu_special->exec(viewport()->mapToGlobal(pos));
		}
	}
}
//----------------------------------------------------------------------------------------------

bool AMessageTree::checkCurrentItem (bool select_first)
{
	if (currentItem() != NULL)
		return true;

	if (topLevelItemCount() == 0)
		return false;

	if (select_first == true)
		setCurrentItem(topLevelItem(0));
	else
	{
		// последний топик в списке
		MessageTreeWidgetItem* topic_item = static_cast<MessageTreeWidgetItem*>(topLevelItem(topLevelItemCount() - 1));

		// если элемент еще не загружен, то загружаем
		if (topic_item->pag()->IsInfoLoaded == false)
			setCurrentItem(topic_item);

		// если дочерние элементы еще не загружены, то загружаем
		if (topic_item->pag()->IsChildLoaded == false)
			topic_item->setExpanded(true);

		// выделяем последний лист в топике
		MessageTreeWidgetItem* last_item = static_cast<MessageTreeWidgetItem*>(ATreeWidgetItem::lastLeaf(topic_item));

		setCurrentItem(last_item);
	}

	return true;
}
//----------------------------------------------------------------------------------------------

void AMessageTree::changeForum (const AForumInfo* forum_info)
{
	//
	// перезагрузка настроек
	//
	QSettings settings;

	m_max_topic_to_show  = settings.value("ui/max_topic_to_show",  "0").toUInt();
	m_mark_as_read_timer = settings.value("ui/mark_as_read_timer", "500").toUInt();

	//
	// базовая очистка
	//

	m_current_forum.ID        = 0;
	m_current_forum.IDGroup   = 0;
	m_current_forum.ShortName = "";
	m_current_forum.Name      = "";
	m_current_forum.Rated     = false;
	m_current_forum.InTop     = false;
	m_current_forum.RateLimit = 0;

	if (m_main_form != NULL)
		m_main_form->setEnabledAction(aaViewSource, false);

	// ускорение очистки дерева
	blockSignals(true);
	clear();
	blockSignals(false);

	m_message_view->clear();

	scrollToTop();

	// если выделена группа, то просто очистка
	if (forum_info == NULL)
	{
		headerItem()->setText(0, QString::fromUtf8("тема"));

		QFont font = headerItem()->font(0);
		font.setBold(false);
		headerItem()->setFont(0, font);

		return;
	}

	//
	// есть форум для отображения
	//

	m_current_forum.ID        = forum_info->ID;
	m_current_forum.IDGroup   = forum_info->IDGroup;
	m_current_forum.ShortName = forum_info->ShortName;
	m_current_forum.Name      = forum_info->Name;
	m_current_forum.Rated     = forum_info->Rated;
	m_current_forum.InTop     = forum_info->InTop;
	m_current_forum.RateLimit = forum_info->RateLimit;

	// вывод наименования форума в хедер
	headerItem()->setText(0, m_current_forum.Name);

	//
	// загрузка топиков в зависимости от типа форума
	//

	// получение хранилища
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	// если группа форумов обыкновенная
	if (m_current_forum.IDGroup != SPECIAL_ID_GROUP)
	{
		// получение топиков
		// ориентировочное время выполнения 68 ms (для форума NET, 28199 топиков)
		// основное время занимает запрос данных из хранилища
		QList<int> topic_list;

		if (storage->getForumTopicList(m_current_forum.ID, m_max_topic_to_show, topic_list) == false)
		{
			storage->showError(m_parent);
			return;
		}

		// установка топиков
		// ориентировочное время выполнения 50 ms (для форума NET, 28199 топиков)
		// основное время занимает создание объекта MessageTreeWidgetItem
		QList<QTreeWidgetItem*> items;

		for (int i = 0; i < topic_list.count(); i++)
		{
			// начальное заполнение дескриптора топика
			AMessageInfoGUI* info = createItem();

			info->ID = topic_list[i];

			items.append(info->Item);
		}

		// отображение на виджет
		addTopLevelItems(items);

		// дозагрузка информации о топиках, находящихся в видимой области отображения
		scrollTopics();
	}
	else   // if (forum_info->IDGroup != SPECIAL_ID_GROUP)
	{
		// специальная группа форумов
		if (m_current_forum.ID == SPECIAL_ID_FORUM_MESSAGE2SEND)
			loadMessage2Send(false);
		else if (m_current_forum.ID == SPECIAL_ID_FORUM_RATING2SEND)
			loadRating2Send();
		else if (m_current_forum.ID == SPECIAL_ID_FORUM_MODERATE2SEND)
			loadModerate2Send();
		else if (m_current_forum.ID == SPECIAL_ID_FORUM_DRAFTS)
			loadMessage2Send(true);
		else if (m_current_forum.ID == SPECIAL_ID_FORUM_MY_MESSAGES)
			loadMyMessages();
		else if (m_current_forum.ID == SPECIAL_ID_FORUM_ANSWERS_TO_ME)
			loadAnswers2Me();
	}
}
//----------------------------------------------------------------------------------------------

void AMessageTree::vertical_scroll (int /*value*/)
{
	scrollTopics();
}
//----------------------------------------------------------------------------------------------

void AMessageTree::scrollTopics ()
{
	// Среднее время выполнения для дозагрузки одного элемента 5 ms
	// Среднее время выполнения для дозагрузки страницы из 56 топиков (полный экран 1280х1024) 32 ms

	// пустой форум или спец-группа (кроме моих сообщений и ответов мне)
	if (m_current_forum.ID == 0 || (m_current_forum.IDGroup == SPECIAL_ID_GROUP && m_current_forum.ID != SPECIAL_ID_FORUM_MY_MESSAGES && m_current_forum.ID != SPECIAL_ID_FORUM_ANSWERS_TO_ME))
		return;

	//
	// Определение диапазона возможной подгрузки данных
	//

	// поиск корня верхнего видимого элемента в отображении
	QTreeWidgetItem* top_item = itemAt(1, 1);

	if (top_item == NULL)
		return; // нет элементов в дереве(?)

	top_item = ATreeWidgetItem::rootItem(top_item);

	int top_index = indexOfTopLevelItem(top_item);

	// поиск корня нижнего видимого элемента в отображении
	QTreeWidgetItem* bottom_item = itemAt(1, height() - 1);

	int bottom_index;

	if (bottom_item == NULL)
		bottom_index = topLevelItemCount();
	else
	{
		bottom_item = ATreeWidgetItem::rootItem(bottom_item);

		bottom_index = indexOfTopLevelItem(bottom_item) + 1;
	}

	//
	// Поиск еще не загруженных топиков в диапазоне
	//

	AMessageInfoGUIPtrList list;

	while (top_index < bottom_index)
	{
		MessageTreeWidgetItem* item = static_cast<MessageTreeWidgetItem*>(topLevelItem(top_index));

		AMessageInfoGUI* info = item->pag();

		if (info->IsInfoLoaded == false)
			list.append(info);

		top_index++;
	}

	//
	// загрузка топиков, которые не загружены
	//

	if (list.count() == 0)
		return;

	// получение хранилища
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	AGlobal* global = AGlobal::getInstance();

	if (storage->getTopicInfoList(m_current_forum.ID, list, global->Me.ID) == false)
	{
		storage->showError(m_parent);
		return;
	}

	//
	// Установка загруженой информации
	//

	for (int i = 0; i < list.count(); i++)
	{
		AMessageInfoGUI* info = list[i];

		info->IsInfoLoaded       = true;
		info->UnreadChildCount   = 0;
		info->UnreadChildCountMy = 0;

		info->Item->setText(0, info->Subject);

		// TODO: сделать общую функцию формирования имени/ника
		if (info->IDUser == 0)
			info->Item->setText(1, global->AnonymousName /*+ info->UserTitle*/);
		else
			info->Item->setText(1, info->UserNick);

		info->Item->setText(2, info->MessageDate.toString(global->DateFormat));

		if (info->HasChild == true)
			info->Item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

		if (info->IsRead == true)
		{
			if (info->HasUnreadChildMy)
				info->Item->setIcon(0, m_child_unread_my);
			else if (info->HasUnreadChild)
				info->Item->setIcon(0, m_child_unread);
			else
				info->Item->setIcon(0, m_message_read);
		}
		else
		{
			if (info->HasUnreadChildMy)
				info->Item->setIcon(0, m_message_unread_my);
			else
				info->Item->setIcon(0, m_message_unread);
		}
	}
}
//----------------------------------------------------------------------------------------------

void AMessageTree::expand_item (QTreeWidgetItem* item_expanded)
{
	MessageTreeWidgetItem* item = static_cast<MessageTreeWidgetItem*>(item_expanded);

	AMessageInfoGUI* info = item->pag();

	// если дочерние сообщения не загружены (а это может быть только для топика в текущей реализации)
	if (info->HasChild == true && info->IsChildLoaded == false)
	{
		// получение хранилища
		std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

		if (storage.get() == NULL)
		{
			QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
			return;
		}

		// заполнение списка сообщений
		// ориентировочное время выполнения 111 ms (для топика 279396 - 5149 сообщений)
		// основное время занимает присвоение данных полям (преобразование из QVariant в QSqlQuery)
		AMessageInfoGUIPtrList list;

		if (storage->getTopicMessageList(m_current_forum.ID, info->ID, list, this) == false)
		{
			storage->showError(m_parent);
			return;
		}

		// постройка дерева, обход в ширину быстрее чем в высоту
		// ориентировочное время выполнения 900 ms (для топика 279396 - 5149 сообщений)
		// наиболее высокое дерево составляет 1555 сообщений (для топика 1099540)
		// наиболее широкое дерево (глубина рекурсии) пока неизвестно
		buildTree(info, &list);

		// для предотвращения повторной загрузки
		info->IsChildLoaded = true;

		// раскрытие дерева до уровня непрочитаных топиков
		if (info->UnreadChildCount != 0)
			expandUnreadChild(item);
	}
}
//----------------------------------------------------------------------------------------------

void AMessageTree::expandUnreadChild (QTreeWidgetItem* widget_item)
{
	MessageTreeWidgetItem* item = static_cast<MessageTreeWidgetItem*>(widget_item);

	if (item->pag()->UnreadChildCount != 0)
	{
		for (int i = 0; i < item->childCount(); i++)
			if ((static_cast<MessageTreeWidgetItem*>(item->child(i)))->pag()->UnreadChildCount != 0)
				expandUnreadChild(item->child(i));

		expandItem(item);
	}
}
//----------------------------------------------------------------------------------------------

void AMessageTree::buildTree (AMessageInfoGUI* root, AMessageInfoGUIPtrList* list)
{
	if (list->count() == 0)
		return;

	int list_index = 0;

	AGlobal* global = AGlobal::getInstance();

	while (list_index < list->count())
	{
		AMessageInfoGUI* info = list->at(list_index);

		if (info->IDParent == root->ID)
		{
			list->removeAt(list_index);

			info->IsInfoLoaded       = true;
			info->IsChildLoaded      = true;
			info->UnreadChildCount   = 0;
			info->UnreadChildCountMy = 0;

			info->Item->setText(0, info->Subject);

			// TODO: сделать общую функцию формирования имени/ника
			if (info->IDUser != 0)
				info->Item->setText(1, info->UserNick);
			else
				info->Item->setText(1, global->AnonymousName /*+ info->UserTitle*/);

			info->Item->setText(2, info->MessageDate.toString(global->DateFormat));

			root->Item->addChild(info->Item);

			// установка родителям свойств непрочитанных сообщений
			if (info->IsRead == false && root->IDUser == global->Me.ID)
			{
				// сообщение не прочитано и это ответ не мне
				info->Item->setIcon(0, m_message_unread_my);

				MessageTreeWidgetItem* item_parent = static_cast<MessageTreeWidgetItem*>(info->Item->parent());

				while (item_parent != NULL)
				{
					AMessageInfoGUI* info_parent = item_parent->pag();

					if (info_parent->HasUnreadChildMy != true)
					{
						info_parent->HasUnreadChild   = true;
						info_parent->HasUnreadChildMy = true;

						if (info_parent->IsRead == true)
							item_parent->setIcon(0, m_child_unread_my);
						else
							item_parent->setIcon(0, m_message_unread_my);
					}

					info_parent->UnreadChildCount++;
					info_parent->UnreadChildCountMy++;

					item_parent = static_cast<MessageTreeWidgetItem*>(item_parent->parent());

				}   // while (item_parent != NULL)
			}
			else if (info->IsRead == false)
			{
				// сообщение не прочитано, но это ответ не мне
				info->Item->setIcon(0, m_message_unread);

				MessageTreeWidgetItem* item_parent = static_cast<MessageTreeWidgetItem*>(info->Item->parent());

				while (item_parent != NULL)
				{
					AMessageInfoGUI* info_parent = item_parent->pag();

					if (info_parent->HasUnreadChild != true)
					{
						info_parent->HasUnreadChild = true;

						if (info_parent->IsRead == true)
							item_parent->setIcon(0, m_child_unread);
					}

					info_parent->UnreadChildCount++;

					item_parent = static_cast<MessageTreeWidgetItem*>(item_parent->parent());

				}   // while (item_parent != NULL)
			}
			else   // else if (info->IsRead == false)
			{
				// сообщение прочитано
				info->Item->setIcon(0, m_message_read);
			}

			// достройка дочерних веток для текущей
			if (info->HasChild)
			{
				buildTree(info, list);
				list_index = 0;
			}
		}
		else   // if (info->ID == id_parent)
		{
			list_index++;
		}
	}
}
//----------------------------------------------------------------------------------------------

void AMessageTree::timer_on_timer ()
{
	MessageTreeWidgetItem* item = static_cast<MessageTreeWidgetItem*>(currentItem());

	if (item != NULL)
		markItemAsRead(item, true);
}
//----------------------------------------------------------------------------------------------

void AMessageTree::showSource ()
{
	MessageTreeWidgetItem* item = static_cast<MessageTreeWidgetItem*>(currentItem());

	if (item == NULL)
		return;

	std::auto_ptr<FormSource> form(new FormSource(m_parent, item->pag()->Message));

	form->exec();
}
//----------------------------------------------------------------------------------------------

void AMessageTree::selection_changed ()
{
	MessageTreeWidgetItem* item = static_cast<MessageTreeWidgetItem*>(currentItem());

	if (item == NULL)
		return;

	AMessageInfoGUI* info = item->pag();

	if (m_main_form != NULL)
		m_main_form->setEnabledAction(aaViewSource, true);

	//
	// добавление смены выделения в историю
	//

	if (m_history_index + 1 != m_history.count())
	{
		// произошла смена выделения посреди истории?
		if (m_history[m_history_index] != info->ID)
		{
			// смена ветви истории
			while (m_history_index + 1 != m_history.count())
				m_history.removeLast();

			m_history.append(info->ID);
			m_history_index++;
		}
	}
	else if (m_history_index == -1 || m_history[m_history_index] != info->ID)
	{
		if (m_history.count() == MAX_HISTORY_LENGTH)
		{
			m_history.removeFirst();
			m_history_index--;
		}

		// смена выделения идет вместе с историей
		m_history.append(info->ID);
		m_history_index++;
	}

	if (m_main_form != NULL && m_history.count() > 1)
	{
		if (m_history_index < 1)
			m_main_form->setEnabledAction(aaBackward, false);
		else
			m_main_form->setEnabledAction(aaBackward, true);

		if (m_history_index + 1 == m_history.count())
			m_main_form->setEnabledAction(aaForward, false);
		else
			m_main_form->setEnabledAction(aaForward, true);
	}

	//
	// загрузка тела сообщения, если тело еще не загружено
	//

	if (info->IsBodyLoaded == false)
	{
		// получение хранилища
		std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

		if (storage.get() == NULL)
		{
			QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
			return;
		}

		// получение доп-информации о сообщении
		QString body;

		if (storage->getMessageBody(info->ID, body) == false)
		{
			storage->showError(m_parent);
			return;
		}

		info->IsBodyLoaded = true;
		info->Message      = body;
	}

	// отображение сообщения
	if (m_current_forum.IDGroup != SPECIAL_ID_GROUP)
		m_message_view->setMessage(*info, &m_current_forum);
	else if (m_current_forum.ID == SPECIAL_ID_FORUM_RATING2SEND   ||
	         m_current_forum.ID == SPECIAL_ID_FORUM_MODERATE2SEND ||
	         m_current_forum.ID == SPECIAL_ID_FORUM_MY_MESSAGES   ||
	         m_current_forum.ID == SPECIAL_ID_FORUM_ANSWERS_TO_ME)
	{
		// в спец-форумах типа "Ответы мне" короткое имя форума
		// для формирования ссылки на сообщение неизвестно - получаем из хранилища

		// получение хранилища
		std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

		if (storage.get() == NULL)
		{
			QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
			return;
		}

		// получение информации о форуме сообщения
		AForumInfo forum;
		if (storage->getForumInfo(info->IDForum, forum) == false)
		{
			storage->showError(m_parent);
			return;
		}

		m_message_view->setMessage(*info, &forum);
	}
	else
		m_message_view->setMessage(*info);

	// взвод таймера для пометки как прочитанного
	if (info->IsRead == false)
	{
		if (m_mark_as_read_timer > 0)
			m_timer.start(m_mark_as_read_timer);
		else
			m_timer.stop();
	}
	else
		m_timer.stop();

}
//----------------------------------------------------------------------------------------------

void AMessageTree::markItemAsRead (QTreeWidgetItem* widget_item, bool is_read)
{
	MessageTreeWidgetItem* item = static_cast<MessageTreeWidgetItem*>(widget_item);

	AMessageInfoGUI* info = item->pag();

	// если уже прочитан
	if (info->IsRead == is_read)
		return;

	// пометить как прочитанное в хранилище
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	if (storage->setIDsAsRead(QList<int>() << info->ID, idsMessage, is_read, QDateTime()) == false)
	{
		storage->showError(m_parent);
		return;
	}

	// установка флага как прочитанного и пометка в дереве
	info->IsRead = is_read;

	if (is_read == true)
	{
		AGlobal* global = AGlobal::getInstance();

		// установка иконки помечаемому сообщению
		if (info->HasUnreadChildMy)
			item->setIcon(0, m_child_unread_my);
		else if (info->HasUnreadChild)
			item->setIcon(0, m_child_unread);
		else
			item->setIcon(0, m_message_read);

		// флаг того, что это было сообщение для меня
		bool message_to_me = false;

		// установка флагов и иконок у родительских сообщений
		MessageTreeWidgetItem* item_parent = static_cast<MessageTreeWidgetItem*>(item->parent());

		if (item_parent != NULL && item_parent->pag()->IDUser == global->Me.ID)
			message_to_me = true;

		while (item_parent != NULL)
		{
			AMessageInfoGUI* info_parent = item_parent->pag();

			int old_unread_count    = info_parent->UnreadChildCount;
			int old_unread_count_my = info_parent->UnreadChildCountMy;

			// уменьшение количества непрочитанных дочерних
			// и установка соответствующих флагов
			if (info_parent->UnreadChildCount > 0)
				info_parent->UnreadChildCount--;

			if (message_to_me == true && info_parent->UnreadChildCountMy > 0)
				info_parent->UnreadChildCountMy--;

			if (info_parent->UnreadChildCountMy == 0)
				info_parent->HasUnreadChildMy = false;

			if (info_parent->UnreadChildCount == 0)
				info_parent->HasUnreadChild = false;

			// установка иконок
			if (info_parent->IsRead == false)
			{
				// родитель непрочитан

				// если среди дочерних прочитали дочернее мне и больше непрочитанных дочерних мне нет
				// и должна произойти смена иконки
				if (old_unread_count_my != 0 && info_parent->UnreadChildCountMy == 0)
				{
					// исключение составляет момент, когда элемент, у которого должна поменяться иконка
					// сам по себе является ответом мне. В этом случае, у него свойства
					// HasUnreadChildMy = false и UnreadChildCountMy = 0, однако, поскольку он является ответом мне
					// то пометка с него сниматься не должна

					MessageTreeWidgetItem* item_parent_parent = static_cast<MessageTreeWidgetItem*>(item_parent->parent());

					if (item_parent_parent == NULL || item_parent_parent->pag()->IDUser != global->Me.ID)
						item_parent->setIcon(0, m_message_unread);
				}
			}
			else
			{
				// TODO: уменьшить возможное количество смен иконок на точно такие же

				// родитель прочитан

				// если среди дочерних есть непрочитанные мне
				// и должна произойти смена иконки
				if (old_unread_count_my != info_parent->UnreadChildCountMy && info_parent->UnreadChildCountMy > 0)
					item_parent->setIcon(0, m_child_unread_my);

				// если среди дочерних есть непрочитанные
				// и должна произойти смена иконки
				else if (old_unread_count != info_parent->UnreadChildCount && info_parent->UnreadChildCount > 0)
				{
					// если, конечно, нет непрочитанных мне
					if (info_parent->UnreadChildCountMy == 0)
						item_parent->setIcon(0, m_child_unread);
				}

				// нет дочерних непрочитанных
				else
					item_parent->setIcon(0, m_message_read);
			}

			item_parent = static_cast<MessageTreeWidgetItem*>(item_parent->parent());
		}

		// уменьшение количества непрочитанных в дереве форума
		if (message_to_me == true)
			m_forum_tree->changeUnreadCount(-1, -1, (item->parent() == NULL ? -1 : 0));
		else
			m_forum_tree->changeUnreadCount(-1, 0, (item->parent() == NULL ? -1 : 0));

		// спец-форумы "Мои сообщения" и "Ответы мне"
		if (m_current_forum.ID == SPECIAL_ID_FORUM_MY_MESSAGES)
		{
			if (info->Special != 0 && info->Special == global->Me.ID)
			{
				// мое сообщение одновременно является ответом мне
				m_forum_tree->changeUnreadCount(-1, -1, (info->IDParent == 0 ? -1 : 0), info->IDForum);
				m_forum_tree->changeUnreadCount(-1, -1, 0, SPECIAL_ID_FORUM_MY_MESSAGES);
			}
			else
				m_forum_tree->changeUnreadCount(-1, 0, (info->IDParent == 0 ? -1 : 0), info->IDForum);
		}
		else if (m_current_forum.ID == SPECIAL_ID_FORUM_ANSWERS_TO_ME)
		{
			// для форума "Ответы мне" флаг message_to_me установлен не будет - уменьшаем дополнительно
			m_forum_tree->changeUnreadCount(0, -1, 0);

			// реальный форум
			m_forum_tree->changeUnreadCount(-1, -1, 0 /* ответ мне не может быть новым топиком */, info->IDForum);

			// форум "Мои сообщения"
			if (info->IDUser == global->Me.ID)
				m_forum_tree->changeUnreadCount(-1, 0, 0 /* ответ мне не может быть новым топиком */, SPECIAL_ID_FORUM_MY_MESSAGES);
		}
		else
		{
			if (message_to_me == true)
				m_forum_tree->changeUnreadCount(-1, -1, 0 /* в форуме все есть топик */, SPECIAL_ID_FORUM_ANSWERS_TO_ME);

			if (info->IDUser == global->Me.ID)
				m_forum_tree->changeUnreadCount(-1, 0, 0 /* в форуме все есть топик */, SPECIAL_ID_FORUM_MY_MESSAGES);
		}
	}
	else   // if (is_read == true)
	{
		AGlobal* global = AGlobal::getInstance();

		// флаг того, что это было сообщение для меня
		bool message_to_me = false;

		// установка флагов и иконок у родительских сообщений
		MessageTreeWidgetItem* parent_item = static_cast<MessageTreeWidgetItem*>(item->parent());

		// для текущего сообщения
		if (parent_item != NULL && parent_item->pag()->IDUser == global->Me.ID)
			message_to_me = true;

		if (message_to_me == true || info->HasUnreadChildMy == true)
			item->setIcon(0, m_message_unread_my);
		else
			item->setIcon(0, m_message_unread);

		// для родительских сообщений
		while (parent_item != NULL)
		{
			AMessageInfoGUI* parent_info = parent_item->pag();

			// смена иконки при необходимости
			if (message_to_me == true && parent_info->HasUnreadChildMy == false)
			{
				if (parent_info->IsRead == true)
					parent_item->setIcon(0, m_child_unread_my);
				else
					parent_item->setIcon(0, m_message_unread_my);
			}
			else if (parent_info->IsRead == true && parent_info->HasUnreadChild == false)
				parent_item->setIcon(0, m_child_unread);

			parent_info->UnreadChildCount++;
			parent_info->HasUnreadChild = true;

			if (message_to_me == true)
			{
				parent_info->UnreadChildCountMy++;
				parent_info->HasUnreadChildMy = true;
			}

			parent_item = static_cast<MessageTreeWidgetItem*>(parent_item->parent());
		}

		// увеличение количества непрочитанных в дереве форума
		if (message_to_me == true)
			m_forum_tree->changeUnreadCount(1, 1, (item->parent() == NULL ? 1 : 0));
		else
			m_forum_tree->changeUnreadCount(1, 0, (item->parent() == NULL ? 1 : 0));

		// спец-форумы "Мои сообщения" и "Ответы мне"
		if (m_current_forum.ID == SPECIAL_ID_FORUM_MY_MESSAGES)
		{
			if (info->Special != 0 && info->Special == global->Me.ID)
			{
				// мое сообщение одновременно является ответом мне
				m_forum_tree->changeUnreadCount(1, 1, (info->IDParent == 0 ? 1 : 0), info->IDForum);
				m_forum_tree->changeUnreadCount(1, 1, 0, SPECIAL_ID_FORUM_MY_MESSAGES);
			}
			else
				m_forum_tree->changeUnreadCount(1, 0, (info->IDParent == 0 ? 1 : 0), info->IDForum);
		}
		else if (m_current_forum.ID == SPECIAL_ID_FORUM_ANSWERS_TO_ME)
		{
			// для форума "Ответы мне" флаг message_to_me установлен не будет - увеличиваем дополнительно
			m_forum_tree->changeUnreadCount(0, 1, 0);

			// реальный форум
			m_forum_tree->changeUnreadCount(1, 1, 0 /* ответ мне не может быть новым топиком */, info->IDForum);

			// форум "Мои сообщения"
			if (info->IDUser == global->Me.ID)
				m_forum_tree->changeUnreadCount(1, 0, 0 /* ответ мне не может быть новым топиком */, SPECIAL_ID_FORUM_MY_MESSAGES);
		}
		else
		{
			if (message_to_me == true)
				m_forum_tree->changeUnreadCount(1, 1, 0 /* в форуме все есть топик */, SPECIAL_ID_FORUM_ANSWERS_TO_ME);

			if (info->IDUser == global->Me.ID)
				m_forum_tree->changeUnreadCount(1, 0, 0 /* в форуме все есть топик */, SPECIAL_ID_FORUM_MY_MESSAGES);
		}
	}   // if (is_read == true) ... else
}
//----------------------------------------------------------------------------------------------

void AMessageTree::menu_mark_message_as_read_triggered ()
{
	// остановка таймера пометки как прочитанного, т.к. его операция должна быть в любом случае прервана
	m_timer.stop();

	QTreeWidgetItem* item = currentItem();

	if (item == NULL)
		return;

	markItemAsRead(item, true);
}
//----------------------------------------------------------------------------------------------

void AMessageTree::menu_mark_message_as_unread_triggered ()
{
	// остановка таймера пометки как прочитанного, т.к. его операция должна быть в любом случае прервана
	m_timer.stop();

	QTreeWidgetItem* item = currentItem();

	if (item == NULL)
		return;

	markItemAsRead(item, false);
}
//----------------------------------------------------------------------------------------------

bool AMessageTree::markThreadAsRead (int id, bool is_read)
{
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return false;
	}

	if (storage->setIDsAsRead(QList<int>() << id, idsTopic, is_read, QDateTime()) == false)
	{
		storage->showError(m_parent);
		return false;
	}

	return true;
}
//----------------------------------------------------------------------------------------------

void AMessageTree::menu_mark_thread_as_read_triggered ()
{
	// остановка таймера пометки как прочитанного, т.к. его операция должна быть в любом случае прервана
	m_timer.stop();

	QTreeWidgetItem* item = currentItem();

	if (item == NULL)
		return;

	// спец-форумы "Мои сообщения" и "Ответы мне"
	if (m_current_forum.ID == SPECIAL_ID_FORUM_MY_MESSAGES || m_current_forum.ID == SPECIAL_ID_FORUM_ANSWERS_TO_ME)
	{
		AMessageInfoGUI* info = (static_cast<MessageTreeWidgetItem*>(item))->pag();

		int id       = info->ID;
		int id_topic = info->IDTopic != 0 ? info->IDTopic : id;

		// пометка в хранилище
		if (markThreadAsRead(id_topic, true) != true)
			return;

                // обновление количества непрочитаных в дереве форума с перезагрузкой
                m_forum_tree->reloadUnread(true);

		// переход на выделенное
		QList<int> path;
		path.append(id);

		selectByPath(&path);

		return;
	}

	// поиск родителя
	MessageTreeWidgetItem* parent = static_cast<MessageTreeWidgetItem*>(ATreeWidgetItem::rootItem(item));

	AMessageInfoGUI* info = parent->pag();

	// пометка в хранилище
	if (markThreadAsRead(info->ID, true) != true)
		return;

	// пометка в дереве
	if (info->IsChildLoaded == true)
	{
		int count        = 0;
		int count_my     = 0;
		int count_topics = (info->IsRead == false ? 1 : 0);

		markThreadAsRead(parent, true, count, count_my);

		m_forum_tree->changeUnreadCount(-count, -count_my, -count_topics);
	}
	else
	{
		parent->setIcon(0, m_message_read);

		AMessageInfoGUI* info = parent->pag();

		info->HasUnreadChild     = false;
		info->HasUnreadChildMy   = false;
		info->UnreadChildCount   = 0;
		info->UnreadChildCountMy = 0;

                // поскольку дочерние элементы еще не загружены,
                // обновление количества непрочитаных в дереве форума
                m_forum_tree->reloadUnread(false);
	}
}
//----------------------------------------------------------------------------------------------

void AMessageTree::menu_mark_thread_as_unread_triggered ()
{
	// остановка таймера пометки как прочитанного, т.к. его операция должна быть в любом случае прервана
	m_timer.stop();

	QTreeWidgetItem* item = currentItem();

	if (item == NULL)
		return;

	// спец-форумы "Мои сообщения" и "Ответы мне"
	if (m_current_forum.ID == SPECIAL_ID_FORUM_MY_MESSAGES || m_current_forum.ID == SPECIAL_ID_FORUM_ANSWERS_TO_ME)
	{
		AMessageInfoGUI* info = (static_cast<MessageTreeWidgetItem*>(item))->pag();

		int id       = info->ID;
		int id_topic = info->IDTopic != 0 ? info->IDTopic : id;

		// пометка в хранилище
		if (markThreadAsRead(id_topic, false) != true)
			return;

                // обновление количества непрочитаных в дереве форума с перезагрузкой
                m_forum_tree->reloadUnread(true);

		// переход на выделенное
		QList<int> path;
		path.append(id);

		selectByPath(&path);

		// остановка таймера для предотвращения пометки как прочитанного
		m_timer.stop();

		return;
	}

	// поиск родителя
	MessageTreeWidgetItem* parent = static_cast<MessageTreeWidgetItem*>(ATreeWidgetItem::rootItem(item));

	AMessageInfoGUI* info = parent->pag();

	// пометка в хранилище
	if (markThreadAsRead(info->ID, false) != true)
		return;

	// пометка в дереве
	if (info->IsChildLoaded == true)
	{
		int count        = 0;
		int count_my     = 0;
		int count_topics = (info->IsRead == true ? 1 : 0);

		markThreadAsRead(parent, false, count, count_my);

		m_forum_tree->changeUnreadCount(count, count_my, count_topics);
	}
	else
	{
		parent->setIcon(0, m_message_unread);

		AMessageInfoGUI* info = parent->pag();

		if (info->HasChild == true)
			info->HasUnreadChild = true;
		else
			info->HasUnreadChild = false;

		info->HasUnreadChildMy = false;

                // поскольку дочерние элементы еще не загружены,
                // обновление количества непрочитаных в дереве форума
                m_forum_tree->reloadUnread(false);

		// перегружаем информацию о топике (возможно, что есть непрочитанные сообщения для меня)
		info->IsInfoLoaded = false;

		scrollTopics();
	}
}
//----------------------------------------------------------------------------------------------

void AMessageTree::menu_reply_triggered ()
{
	MessageTreeWidgetItem* item = static_cast<MessageTreeWidgetItem*>(currentItem());

	if (item == NULL)
		return;

	//
	// проверка на некропостинг
	//

	MessageTreeWidgetItem* root = static_cast<MessageTreeWidgetItem*>(ATreeWidgetItem::rootItem(item));

	int diff      = item->pag()->MessageDate.daysTo(QDateTime::currentDateTime());
	int root_diff = root->pag()->MessageDate.daysTo(QDateTime::currentDateTime());

	QMessageBox::StandardButton necro_result = QMessageBox::No;

	if (diff > 31)
	{
		necro_result = QMessageBox::question(m_parent, QString::fromUtf8("Внимание!"), QString::fromUtf8("Сообщение, на которое вы хотите ответить, было отправлено более месяца назад! Вы уверены, что хотите ответить?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

		if (necro_result != QMessageBox::Yes)
			return;
	}

	if (necro_result != QMessageBox::Yes && root_diff > 31 * 6)
	{
		necro_result = QMessageBox::question(m_parent, QString::fromUtf8("Внимание!"), QString::fromUtf8("Тема, в которой вы хотите ответить, была создана более полугода назад! Вы уверены, что хотите ее поднять?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

		if (necro_result != QMessageBox::Yes)
			return;
	}

	//
	// отображение формы ответа
	//

	FormMessage* form = new FormMessage(NULL, true, *(item->pag()));

	form->setForumTree(m_forum_tree);
	form->setMainForm(m_main_form);

	form->show();
}
//----------------------------------------------------------------------------------------------

void AMessageTree::loadMessage2Send (bool drafts)
{
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	AMessageInfoList list;

	if (storage->getMessage2SendList(list, drafts) == false)
	{
		storage->showError(m_parent);
		return;
	}

	QList<QTreeWidgetItem*> items;

	AGlobal* global = AGlobal::getInstance();

	for (int i = 0; i < list.count(); i++)
	{
		AMessageInfoGUI* info = createItem();

		info->ID             = list[i].ID;
		info->IDTopic        = list[i].IDTopic;
		info->IDParent       = list[i].IDParent;
		info->IDUser         = list[i].IDUser;
		info->IDForum        = list[i].IDForum;
		info->Subject        = list[i].Subject;
		info->MessageName    = list[i].MessageName;
		info->UserNick       = list[i].UserNick;
		info->Message        = list[i].Message;
		info->IDArticle      = list[i].IDArticle;
		info->MessageDate    = list[i].MessageDate;
		info->UpdateDate     = list[i].UpdateDate;
		info->UserRole       = list[i].UserRole;
		info->UserTitle      = list[i].UserTitle;
		info->UserTitleColor = list[i].UserTitleColor;
		info->LastModerated  = list[i].LastModerated;

		info->IsInfoLoaded     = true;
		info->IsRead           = true;
		info->IsBodyLoaded     = true;
		info->IsChildLoaded    = true;
		info->HasUnreadChild   = 0;
		info->HasUnreadChildMy = 0;
		info->Special          = list[i].ID;

		info->Item->setText(0, info->Subject);

		if (info->IDUser == 0)
		{
			info->UserNick = QString::fromUtf8("(локальный)");
			info->Item->setText(1, info->UserNick);
		}
		else
			info->Item->setText(1, info->UserNick);

		info->Item->setText(2, info->MessageDate.toString(global->DateFormat));

		items.append(info->Item);
	}

	addTopLevelItems(items);
}
//----------------------------------------------------------------------------------------------

void AMessageTree::processUrl (const QString& url)
{
	MessageTreeWidgetItem* item = static_cast<MessageTreeWidgetItem*>(currentItem());

	if (item == NULL)
		return;

	int     rate;
	QString title;
	QString message;

	// оценки, модерилки
	if (url == "avalon:rate_plus_1")
	{
		rate    = -3;
		title   = QString::fromUtf8("Оценка +1");
		message = QString::fromUtf8("Поставить оценку +1?");
	}
	else if (url == "avalon:rate_1")
	{
		rate    = 1;
		title   = QString::fromUtf8("Оценка 1");
		message = QString::fromUtf8("Оценить сообщение как интересное?");
	}
	else if (url == "avalon:rate_2")
	{
		rate    = 2;
		title   = QString::fromUtf8("Оценка 2");
		message = QString::fromUtf8("Сказать спасибо за сообщение?");
	}
	else if (url == "avalon:rate_3")
	{
		rate    = 3;
		title   = QString::fromUtf8("Оценка 3");
		message = QString::fromUtf8("Оценить сообщение как супер?");
	}
	else if (url == "avalon:rate_cross")
	{
		rate    = -1;
		title   = QString::fromUtf8("Удаление оценки");
		message = QString::fromUtf8("Вы действительно хотите удалить оценку?");
	}
	else if (url == "avalon:rate_smile")
	{
		rate    = -2;
		title   = QString::fromUtf8("Оценка :)");
		message = QString::fromUtf8("Оценить сообщение как смешное?");
	}
	else if (url == "avalon:rate_plus")
	{
		rate    = -4;
		title   = QString::fromUtf8("Оценка +");
		message = QString::fromUtf8("Согласиться с сообщением?");
	}
	else if (url == "avalon:rate_minus")
	{
		rate    = 0;
		title   = QString::fromUtf8("Оценка -");
		message = QString::fromUtf8("Не согласиться с сообщением?");
	}
	else if (url == "avalon:moderate")
	{
		FormModerate* form = new FormModerate(m_parent, item->pag()->ID);

		form->setForumTree(m_forum_tree);

		form->exec();

		return;
	}
	else
	{
		QString external_url = url;

		external_url.replace("avalon:", "");

		// проверка на то, что URL является ссылкой на сообщение RSDN
		// TODO: дополнить регэксп на ссылку вида https://rsdn.ru/forum/Message.aspx?mid=3900833&only=1
		// пример встречается по ссылке https://rsdn.ru/forum/unix/3901241.1
		QRegExp rsdn_url("^(http://){0,1}((www|gzip)\\.){0,1}rsdn.ru/forum/.+/(\\d+)(\\.(1|flat|aspx)){0,1}", Qt::CaseInsensitive);

		if (rsdn_url.indexIn(external_url) == -1)
			QDesktopServices::openUrl(external_url);
		else
		{
			// URL является ссылкой на сообщение RSDN, получение id сообщения
			QList<int> path;

			bool is_int     = false;
			int  id_forum   = 0;
			int  id_message = rsdn_url.cap(4).toInt(&is_int);

			// проверка того, что текущее выделение не есть это же сообщение
			MessageTreeWidgetItem* item = static_cast<MessageTreeWidgetItem*>(currentItem());

			if (item != NULL && item->pag()->ID == id_message)
			{
				QDesktopServices::openUrl(external_url);
				return;
			}

			// попытка получения пути сообщения
			std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

			// "тихая" отработка ошибок, т.к. ошибки не критичны
			if (storage.get() == NULL)
			{
				QDesktopServices::openUrl(external_url);
				return;
			}

			if (storage->getMessagePath(id_message, id_forum, path) == false)
			{
				QDesktopServices::openUrl(external_url);
				return;
			}

			if (id_forum == 0 || path.count() == 0)
			{
				QDesktopServices::openUrl(external_url);
				return;
			}

			if (m_current_forum.ID != id_forum && (m_forum_tree == NULL || m_forum_tree->selectForum(id_forum) == false))
			{
				QDesktopServices::openUrl(external_url);
				return;
			}

			// переход на сообщение
			if (selectByPath(&path) == false)
			{
				QDesktopServices::openUrl(external_url);
				return;
			}
		}

		return;
	}

	if (QMessageBox::question(m_parent, title, message, QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
		return;

	// получение хранилища
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	// подготовка данных
	ARating2Send info;

	info.IDMessage = item->pag()->ID;
	info.Rate      = rate;
	info.Date      = QDateTime::currentDateTime();
	info.ID        = info.Date.toTime_t(); // это локальный ID

	// добавление рейтинга
	if (storage->addRating2Send(info) == false)
	{
		storage->showError(m_parent);
		return;
	}

	// обновление в дереве форумов количества непрочитанных сообщений
	if (m_forum_tree != NULL)
		m_forum_tree->reloadUnread(false);
}
//----------------------------------------------------------------------------------------------

void AMessageTree::gotoNextUnreadArticle (QTreeWidgetItem* current_item)
{
	AMessageInfoGUI* info = NULL;

	MessageTreeWidgetItem* item = static_cast<MessageTreeWidgetItem*>(current_item);

	// если не было выделения, то ищем первый непрочитанный топик или топик с непрочитанными сообщениями
	if (item == NULL)
	{
		for (int i = 0; i < topLevelItemCount(); i++)
		{
			item = static_cast<MessageTreeWidgetItem*>(topLevelItem(i));

			info = item->pag();

			if (info->IsInfoLoaded == false)
				return;
			else if (info->IsRead == false)
			{
				setCurrentItem(item);

				scrollToItem(item, QAbstractItemView::PositionAtCenter);

				return;
			}
			else if (info->HasUnreadChild == true)
				break;
		}
	}
	else
		info = item->pag();

	// флаг петли
	bool loopback = false;

	// есть выделение или найден топик с непрочитанными сообщениями
	while (info != NULL)
	{
		if (info->HasUnreadChild == true && info->IsChildLoaded == false)
			item->setExpanded(true);

		// выбор следующего элемента
		if (info->HasUnreadChild == true)
			// если есть дочерние непрочитанные, то выбираем первый дочерний
			item = static_cast<MessageTreeWidgetItem*>(ATreeWidgetItem::firstChild(item));
		else
		{
			// если нет дочерних непрочитанных, то ...

			// смотрим, есть ли у элемента родитель и есть ли у родителя непрочитанные дочерние
			MessageTreeWidgetItem* parent_item = static_cast<MessageTreeWidgetItem*>(item->parent());

			// если есть родитель и у него нет непрочитанных дочерних, то поднимаемся на уровень родителя
			if (parent_item != NULL && parent_item->pag()->HasUnreadChild == false)
				item = parent_item;
			else
			{
				// у родителя есть непрочитанные дочерние или нет родителя - переходим к следующему брату
				// несмотря на то, что непрочитанный брат может быть и выше
				MessageTreeWidgetItem* sibling_item = static_cast<MessageTreeWidgetItem*>(ATreeWidgetItem::nextSibling(item));

				// если следующего брата нет
				if (sibling_item == NULL && parent_item != NULL)
				{
					// движение вверх по дереву, пока не найдется следующий брат у родителя
					while (parent_item != NULL)
					{
						MessageTreeWidgetItem* last_child_item = static_cast<MessageTreeWidgetItem*>(ATreeWidgetItem::lastChild(parent_item));

						if (last_child_item != item)
							break;

						item = parent_item;

						parent_item = static_cast<MessageTreeWidgetItem*>(item->parent());
					}

					// сдвиг на следующего брата в соседней подветке
					item = static_cast<MessageTreeWidgetItem*>(ATreeWidgetItem::nextSibling(item));
				}
				else
					item = sibling_item;
			}
		}

		// если достигнут конец дерева, то переходим в корень
		if (item == NULL)
			item = static_cast<MessageTreeWidgetItem*>(topLevelItem(0));

		info = item->pag();

		// если найденый топик с незагруженной информацией
		if (info->IsInfoLoaded == false)
		{
			if (loopback == true)
				return;

			item = static_cast<MessageTreeWidgetItem*>(topLevelItem(0));
			info = item->pag();

			loopback = true;
		}

		if (info->IsRead == false)
			break;
	}

	setCurrentItem(item);

	scrollToItem(item, QAbstractItemView::PositionAtCenter);
}
//----------------------------------------------------------------------------------------------

void AMessageTree::loadRating2Send ()
{
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	ARating2SendList rating_list;
	AMessageInfoList message_list;

	if (storage->getRating2SendList(message_list, rating_list) == false)
	{
		storage->showError(m_parent);
		return;
	}

	QList<QTreeWidgetItem*> items;

	AGlobal* global = AGlobal::getInstance();

	for (int i = 0; i < message_list.count(); i++)
	{
		AMessageInfoGUI* info = createItem();

		info->ID             = message_list[i].ID;
		info->IDTopic        = message_list[i].IDTopic;
		info->IDParent       = message_list[i].IDParent;
		info->IDUser         = message_list[i].IDUser;
		info->IDForum        = message_list[i].IDForum;
		info->Subject        = message_list[i].Subject;
		info->MessageName    = message_list[i].MessageName;
		info->UserNick       = message_list[i].UserNick;
		info->Message        = message_list[i].Message;
		info->IDArticle      = message_list[i].IDArticle;
		info->MessageDate    = message_list[i].MessageDate;
		info->UpdateDate     = message_list[i].UpdateDate;
		info->UserRole       = message_list[i].UserRole;
		info->UserTitle      = message_list[i].UserTitle;
		info->UserTitleColor = message_list[i].UserTitleColor;
		info->LastModerated  = message_list[i].LastModerated;

		info->IsInfoLoaded     = true;
		info->IsRead           = true;
		info->IsBodyLoaded     = true;
		info->IsChildLoaded    = true;
		info->HasUnreadChild   = 0;
		info->HasUnreadChildMy = 0;
		info->Special          = rating_list[i].ID;

		info->Item->setText(0, info->Subject);

		if (info->IDUser == 0)
		{
			info->UserNick = QString::fromUtf8("(локальный)");
			info->Item->setText(1, info->UserNick);
		}
		else
			info->Item->setText(1, info->UserNick);

		info->Item->setText(2, info->MessageDate.toString(global->DateFormat));

		int rate = rating_list[i].Rate;

		if (rate == -4)
			info->Item->setIcon(0, m_rating_plus);
		else if (rate == -3)
			info->Item->setIcon(0, m_rating_plus_1);
		else if (rate == -2)
			info->Item->setIcon(0, m_rating_smile);
		else if (rate == -1)
			info->Item->setIcon(0, m_rating_cross);
		else if (rate == 0)
			info->Item->setIcon(0, m_rating_minus);
		else if (rate == 1)
			info->Item->setIcon(0, m_rating_1);
		else if (rate == 2)
			info->Item->setIcon(0, m_rating_2);
		else if (rate == 3)
			info->Item->setIcon(0, m_rating_3);

		items.append(info->Item);
	}

	addTopLevelItems(items);
}
//----------------------------------------------------------------------------------------------

void AMessageTree::loadModerate2Send ()
{
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	AMessageInfoList   message_list;
	AModerate2SendList moderate_list;

	if (storage->getModerate2SendList(message_list, moderate_list) == false)
	{
		storage->showError(m_parent);
		return;
	}

	QList<QTreeWidgetItem*> items;

	AGlobal* global = AGlobal::getInstance();

	for (int i = 0; i < message_list.count(); i++)
	{
		AMessageInfoGUI* info = createItem();

		info->ID             = message_list[i].ID;
		info->IDTopic        = message_list[i].IDTopic;
		info->IDParent       = message_list[i].IDParent;
		info->IDUser         = message_list[i].IDUser;
		info->IDForum        = message_list[i].IDForum;
		info->Subject        = message_list[i].Subject;
		info->MessageName    = message_list[i].MessageName;
		info->UserNick       = message_list[i].UserNick;
		info->Message        = message_list[i].Message;
		info->IDArticle      = message_list[i].IDArticle;
		info->MessageDate    = message_list[i].MessageDate;
		info->UpdateDate     = message_list[i].UpdateDate;
		info->UserRole       = message_list[i].UserRole;
		info->UserTitle      = message_list[i].UserTitle;
		info->UserTitleColor = message_list[i].UserTitleColor;
		info->LastModerated  = message_list[i].LastModerated;

		info->IsInfoLoaded     = true;
		info->IsRead           = true;
		info->IsBodyLoaded     = true;
		info->IsChildLoaded    = true;
		info->HasUnreadChild   = 0;
		info->HasUnreadChildMy = 0;
		info->Special          = moderate_list[i].ID;

		info->Item->setText(0, info->Subject);

		if (info->IDUser == 0)
		{
			info->UserNick = QString::fromUtf8("(локальный)");
			info->Item->setText(1, info->UserNick);
		}
		else
			info->Item->setText(1, info->UserNick);

		info->Item->setText(2, info->MessageDate.toString(global->DateFormat));

		info->Item->setIcon(0, m_moderate);

		items.append(info->Item);
	}

	addTopLevelItems(items);
}
//----------------------------------------------------------------------------------------------

void AMessageTree::loadMyMessages()
{
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	QList<int> list;

	if (storage->getMyMessageList(AGlobal::getInstance()->Me.ID, m_max_topic_to_show, list) == false)
	{
		storage->showError(m_parent);
		return;
	}

	QList<QTreeWidgetItem*> items;

	for (int i = 0; i < list.count(); i++)
	{
		AMessageInfoGUI* info = createItem();

		info->ID = list[i];

		items.append(info->Item);
	}

	addTopLevelItems(items);

	scrollTopics();
}
//----------------------------------------------------------------------------------------------

void AMessageTree::loadAnswers2Me()
{
	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}


	QList<int> list;

	if (storage->getAnswers2MeList(AGlobal::getInstance()->Me.ID, m_max_topic_to_show, list) == false)
	{
		storage->showError(m_parent);
		return;
	}

	QList<QTreeWidgetItem*> items;

	for (int i = 0; i < list.count(); i++)
	{
		AMessageInfoGUI* info = createItem();

		info->ID = list[i];

		items.append(info->Item);
	}

	addTopLevelItems(items);

	scrollTopics();
}
//----------------------------------------------------------------------------------------------

void AMessageTree::gotoNextUnreadThread ()
{
	AMessageInfoGUI* info = NULL;

	MessageTreeWidgetItem* item = static_cast<MessageTreeWidgetItem*>(currentItem());

	// если не было выделения, то ищем следующую непрочитанную статью (она будет первой)
	if (item == NULL)
	{
		gotoNextUnreadArticle(NULL);
		return;
	}

	// item установлен на топик
	item = static_cast<MessageTreeWidgetItem*>(ATreeWidgetItem::rootItem(item));
	info = item->pag();

	// флаг петли
	bool loopback = false;

	// поиск следующего топика с непрочитанными сообщениями
	while (true)
	{
		// следующий топик за текущим
		item = static_cast<MessageTreeWidgetItem*>(ATreeWidgetItem::nextSibling(item));

		// если достигнут конец дерева, то переходим в корень
		if (item == NULL)
			item = static_cast<MessageTreeWidgetItem*>(topLevelItem(0));

		info = item->pag();

		// если найденый топик с незагруженной информацией
		if (info->IsInfoLoaded == false)
		{
			if (loopback == true)
				return;

			item = static_cast<MessageTreeWidgetItem*>(topLevelItem(0));
			info = item->pag();

			loopback = true;
		}

		if (info->IsRead == false)
			break;

		if (info->HasUnreadChild == true)
		{
			gotoNextUnreadArticle(item);
			return;
		}
	}

	setCurrentItem(item);

	scrollToItem(item, QAbstractItemView::PositionAtCenter);
}
//----------------------------------------------------------------------------------------------

void AMessageTree::menu_special_delete_triggered ()
{
	MessageTreeWidgetItem* item = static_cast<MessageTreeWidgetItem*>(currentItem());

	if (item == NULL)
		return;

	if (!(m_current_forum.ID == SPECIAL_ID_FORUM_MESSAGE2SEND  ||
	      m_current_forum.ID == SPECIAL_ID_FORUM_RATING2SEND   ||
	      m_current_forum.ID == SPECIAL_ID_FORUM_MODERATE2SEND ||
	      m_current_forum.ID == SPECIAL_ID_FORUM_DRAFTS
	))
		return;

	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	if (storage->deleteSpecial(QList<int>() << item->pag()->Special, m_current_forum.ID) == false)
	{
		storage->showError(m_parent);
		return;
	}

	// обновление в дереве форумов количества непрочитанных сообщений
	if (m_forum_tree != NULL)
		m_forum_tree->reloadUnread(true);
}
//----------------------------------------------------------------------------------------------

void AMessageTree::item_double_clicked (QTreeWidgetItem* item, int /*column*/)
{
	if (item == NULL)
		return;

	setCurrentItem(item);

	if (!(m_current_forum.ID == SPECIAL_ID_FORUM_MESSAGE2SEND  ||
	      m_current_forum.ID == SPECIAL_ID_FORUM_RATING2SEND   ||
	      m_current_forum.ID == SPECIAL_ID_FORUM_MODERATE2SEND ||
	      m_current_forum.ID == SPECIAL_ID_FORUM_DRAFTS
	))
		return;

	menu_special_edit_triggered();
}
//----------------------------------------------------------------------------------------------

void AMessageTree::menu_special_edit_triggered ()
{
	MessageTreeWidgetItem* item = static_cast<MessageTreeWidgetItem*>(currentItem());

	if (item == NULL)
		return;

	if (!(m_current_forum.ID == SPECIAL_ID_FORUM_MESSAGE2SEND  ||
	      m_current_forum.ID == SPECIAL_ID_FORUM_RATING2SEND   ||
	      m_current_forum.ID == SPECIAL_ID_FORUM_MODERATE2SEND ||
	      m_current_forum.ID == SPECIAL_ID_FORUM_DRAFTS
	))
		return;

	if (m_current_forum.ID == SPECIAL_ID_FORUM_MESSAGE2SEND || m_current_forum.ID == SPECIAL_ID_FORUM_DRAFTS)
	{
		FormMessage* form = new FormMessage(NULL, false, *(item->pag()), item->pag()->Special);

		form->setForumTree(m_forum_tree);
		form->setMainForm(m_main_form);

		form->show();
	}
	else if (m_current_forum.ID == SPECIAL_ID_FORUM_MODERATE2SEND)
	{
		FormModerate* form = new FormModerate(m_parent, item->pag()->ID, item->pag()->Special);

		form->setForumTree(m_forum_tree);

		form->exec();

		return;
	}
}
//----------------------------------------------------------------------------------------------

void AMessageTree::changeRating (int new_rate)
{
	MessageTreeWidgetItem* item = static_cast<MessageTreeWidgetItem*>(currentItem());

	if (item == NULL)
		return;

	if (m_current_forum.ID != SPECIAL_ID_FORUM_RATING2SEND)
		return;

	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() == NULL)
	{
		QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
		return;
	}

	if (storage->changeRating(item->pag()->Special, new_rate) == false)
	{
		storage->showError(m_parent);
		return;
	}

	if (new_rate == -4)
		item->setIcon(0, m_rating_plus);
	else if (new_rate == -3)
		item->setIcon(0, m_rating_plus_1);
	else if (new_rate == -2)
		item->setIcon(0, m_rating_smile);
	else if (new_rate == -1)
		item->setIcon(0, m_rating_cross);
	else if (new_rate == 0)
		item->setIcon(0, m_rating_minus);
	else if (new_rate == 1)
		item->setIcon(0, m_rating_1);
	else if (new_rate == 2)
		item->setIcon(0, m_rating_2);
	else if (new_rate == 3)
		item->setIcon(0, m_rating_3);
}
//----------------------------------------------------------------------------------------------

void AMessageTree::menu_special_plus_1_triggered ()
{
	changeRating(-3);
}
//----------------------------------------------------------------------------------------------

void AMessageTree::menu_special_1_triggered ()
{
	changeRating(1);
}
//----------------------------------------------------------------------------------------------

void AMessageTree::menu_special_2_triggered ()
{
	changeRating(2);
}
//----------------------------------------------------------------------------------------------

void AMessageTree::menu_special_3_triggered ()
{
	changeRating(3);
}
//----------------------------------------------------------------------------------------------

void AMessageTree::menu_special_smile_triggered ()
{
	changeRating(-2);
}
//----------------------------------------------------------------------------------------------

void AMessageTree::menu_special_plus_triggered ()
{
	changeRating(-4);
}
//----------------------------------------------------------------------------------------------

void AMessageTree::menu_special_minus_triggered ()
{
	changeRating(0);
}
//----------------------------------------------------------------------------------------------

void AMessageTree::getSelectedPath (QList<int>& path)
{
	MessageTreeWidgetItem* item = static_cast<MessageTreeWidgetItem*>(currentItem());

	if (item == NULL)
		return;

	path.insert(0, item->pag()->ID);

	while (true)
	{
		item = static_cast<MessageTreeWidgetItem*>(item->parent());

		if (item != NULL)
			path.insert(0, item->pag()->ID);
		else
			break;
	}
}
//----------------------------------------------------------------------------------------------

bool AMessageTree::selectByPath (const QList<int>* path)
{
	if (path == NULL || path->count() == 0)
		return false;

	int root_id = path->at(0);

	MessageTreeWidgetItem* root_item = NULL;

	// поиск в корне
	for (int i = 0; i < topLevelItemCount(); i++)
	{
		MessageTreeWidgetItem* item = static_cast<MessageTreeWidgetItem*>(topLevelItem(i));

		AMessageInfoGUI* info = item->pag();

		if (item->pag()->ID == root_id)
		{
			root_item = item;

			// переход на корень (и его загрузка, при необходимости)
			if (info->IsInfoLoaded == false)
			{
				scrollToItem(root_item, QAbstractItemView::PositionAtCenter);
				setCurrentItem(root_item);
			}

			// ... и разворот, если ищется некорневое сообщение
			if (path->count() > 1 && root_item->isExpanded() == false)
				root_item->setExpanded(true);

			break;
		}
	}

	if (root_item == NULL)
		return false;

	// поиск в дочерних
	for (int i = 1; i < path->count(); i++)
	{
		bool found = false;

		int next_id = path->at(i);

		for (int j = 0; j < root_item->childCount(); j++)
		{
			MessageTreeWidgetItem* item = static_cast<MessageTreeWidgetItem*>(root_item->child(j));

			if (item->pag()->ID == next_id)
			{
				found     = true;
				root_item = item;

				if (i + 1 != path->count() && root_item->isExpanded() == false)
					root_item->setExpanded(true);

				break;
			}
		}

		if (found == false)
		{
			setCurrentItem(root_item);

			return false;
		}
	}

	scrollToItem(root_item, QAbstractItemView::PositionAtCenter);

	setCurrentItem(root_item);

	return true;
}
//----------------------------------------------------------------------------------------------

void AMessageTree::gotoNextUnreadArticle ()
{
	gotoNextUnreadArticle(currentItem());
}
//----------------------------------------------------------------------------------------------

AMessageInfoGUI* AMessageTree::createItem ()
{
	MessageTreeWidgetItem* item = new MessageTreeWidgetItem();

	AMessageInfoGUI* info = item->pag();

	info->Item = item;

	return info;
}
//----------------------------------------------------------------------------------------------

void AMessageTree::markThreadAsRead (QTreeWidgetItem* parent, bool is_read, int& count, int& count_my)
{
	MessageTreeWidgetItem* item = static_cast<MessageTreeWidgetItem*>(parent);

	AMessageInfoGUI* info = item->pag();

	if (info->IsRead != is_read)
		count++;

	info->IsRead = is_read;

	if (is_read == true)
	{
		item->setIcon(0, m_message_read);

		if (info->UnreadChildCountMy > count_my)
			count_my = info->UnreadChildCountMy;

		info->HasUnreadChild     = false;
		info->HasUnreadChildMy   = false;
		info->UnreadChildCount   = 0;
		info->UnreadChildCountMy = 0;

		for (int i = 0; i < item->childCount(); i++)
			markThreadAsRead(item->child(i), is_read, count, count_my);
	}
	else   // if (is_read == true)
	{
		AGlobal* global = AGlobal::getInstance();

		info->HasUnreadChild     = false;
		info->HasUnreadChildMy   = false;
		info->UnreadChildCount   = 0;
		info->UnreadChildCountMy = 0;

		// флаг того, что есть дочерние сообщения для меня
		bool inc_unread_my = false;

		MessageTreeWidgetItem* parent = static_cast<MessageTreeWidgetItem*>(item->parent());

		// установка иконки на текущее сообщение, если это ответ мне
		if (parent != NULL && parent->pag()->IDUser == global->Me.ID)
		{
			count_my++;
			inc_unread_my = true;
			item->setIcon(0, m_message_unread_my);
		}
		else
			item->setIcon(0, m_message_unread);

		while (parent != NULL)
		{
			info = parent->pag();

			if (inc_unread_my == true)
			{
				if (info->HasUnreadChildMy == false)
					parent->setIcon(0, m_message_unread_my);

				info->HasUnreadChildMy = true;
				info->UnreadChildCountMy++;
			}

			info->HasUnreadChild = true;
			info->UnreadChildCount++;

			parent = static_cast<MessageTreeWidgetItem*>(parent->parent());
		}

		for (int i = 0; i < item->childCount(); i++)
			markThreadAsRead(item->child(i), is_read, count, count_my);

	}   // if (is_read == true) ... else
}
//----------------------------------------------------------------------------------------------

void AMessageTree::gotoBackward ()
{
	if (m_history_index > 0)
	{
		QList<int> path;

		int id_forum   = 0;
		int id_message = m_history[m_history_index - 1];

		std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

		if (storage.get() == NULL)
		{
			QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
			return;
		}

		if (storage->getMessagePath(id_message, id_forum, path) == false)
		{
			storage->showError(m_parent);
			return;
		}

		if (id_forum == 0 || path.count() == 0)
			// TODO: возможно, удалить из истории ошибочный элемент?
			return;

		if (m_current_forum.ID != id_forum && (m_forum_tree == NULL || m_forum_tree->selectForum(id_forum) == false))
			// TODO: возможно, удалить из истории ошибочный элемент?
			return;

		m_history_index--;

		selectByPath(&path);
	}

	if (m_main_form != NULL)
	{
		if (m_history_index < 1)
			m_main_form->setEnabledAction(aaBackward, false);
		if (m_history_index + 1 > m_history.count())
			m_main_form->setEnabledAction(aaForward, true);
	}
}
//----------------------------------------------------------------------------------------------

void AMessageTree::gotoForward ()
{
	if (m_history_index + 1 < m_history.count())
	{
		QList<int> path;

		int id_forum   = 0;
		int id_message = m_history[m_history_index + 1];

		std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

		if (storage.get() == NULL)
		{
			QMessageBox::critical(m_parent, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
			return;
		}

		if (storage->getMessagePath(id_message, id_forum, path) == false)
		{
			storage->showError(m_parent);
			return;
		}

		if (id_forum == 0 || path.count() == 0)
			// TODO: возможно, удалить из истории ошибочный элемент?
			return;

		if (m_current_forum.ID != id_forum && (m_forum_tree == NULL || m_forum_tree->selectForum(id_forum) == false))
			// TODO: возможно, удалить из истории ошибочный элемент?
			return;

		m_history_index++;

		selectByPath(&path);
	}

	if (m_main_form != NULL)
	{
		if (m_history_index + 1 == m_history.count())
			m_main_form->setEnabledAction(aaForward, false);
		if (m_history_index > 0)
			m_main_form->setEnabledAction(aaBackward, true);
	}
}
//----------------------------------------------------------------------------------------------

void AMessageTree::reply ()
{
	menu_reply_triggered();
}
//----------------------------------------------------------------------------------------------
