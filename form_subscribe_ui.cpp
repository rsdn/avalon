#include "form_subscribe_ui.h"
//----------------------------------------------------------------------------------------------

FormSubscribeUI::FormSubscribeUI (QWidget* parent) : QDialog (parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint
#if QT_VERSION >= 0x040500
 | Qt::WindowCloseButtonHint
#endif
)
{
	setWindowTitle(QString::fromUtf8("подписка"));

	QIcon icon;
	icon.addFile(":/icons/subscribe16.png",  QSize(16, 16));
	icon.addFile(":/icons/subscribe22.png",  QSize(22, 22));
	icon.addFile(":/icons/subscribe32.png",  QSize(32, 32));
	icon.addFile(":/icons/subscribe48.png",  QSize(48, 48));
	icon.addFile(":/icons/subscribe64.png",  QSize(64, 64));
	setWindowIcon(icon);

	resize(600, 500);
	setFixedSize(width(), height());

	m_layout_form = new QHBoxLayout(this);

	m_layout_tree = new QVBoxLayout();
	m_layout_form->addLayout(m_layout_tree);

	m_layout_button = new QVBoxLayout();
	m_layout_form->addLayout(m_layout_button);

	m_tree_forum = new QTreeWidget();

	#ifndef Q_WS_WIN
		m_tree_forum->setFrameShadow(QFrame::Plain);
	#endif

	m_tree_forum->setSelectionMode(QAbstractItemView::NoSelection);
	m_tree_forum->setColumnCount(1);
	m_tree_forum->header()->hide();
	m_layout_tree->addWidget(m_tree_forum);

	m_button_ok = new QPushButton(this);
	m_button_ok->setText(QString::fromUtf8("OK"));
	m_button_ok->setShortcut(QKeySequence("Return"));
	m_button_ok->setDefault(true);
	m_layout_button->addWidget(m_button_ok);

	m_button_refresh = new QPushButton(this);
	m_button_refresh->setText(QString::fromUtf8("Обновить"));
	m_button_refresh->setShortcut(QKeySequence("F5"));
	m_layout_button->addWidget(m_button_refresh);

	m_button_select_all = new QPushButton(this);
	m_button_select_all->setText(QString::fromUtf8("Выбрать все"));
	m_layout_button->addWidget(m_button_select_all);

	m_button_select_none = new QPushButton(this);
	m_button_select_none->setText(QString::fromUtf8("Отменить все"));
	m_layout_button->addWidget(m_button_select_none);

	m_spacer_button = new QSpacerItem(1, height(), QSizePolicy::Maximum, QSizePolicy::Expanding);
	m_layout_button->addItem(m_spacer_button);

	m_button_cancel = new QPushButton(this);
	m_button_cancel->setText(QString::fromUtf8("Отмена"));
	m_button_cancel->setShortcut(QKeySequence("Esc"));
	m_layout_button->addWidget(m_button_cancel);
}
//----------------------------------------------------------------------------------------------

FormSubscribeUI::~FormSubscribeUI ()
{
}
//----------------------------------------------------------------------------------------------
