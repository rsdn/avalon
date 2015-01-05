#include "form_date_ui.h"
//----------------------------------------------------------------------------------------------

FormDateUI::FormDateUI (QWidget* parent) : QDialog (parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint)
{
	setWindowTitle(QString::fromUtf8("дата / время"));

	QIcon icon;
	icon.addFile(":/icons/date16.png",  QSize(16, 16));
	icon.addFile(":/icons/date22.png",  QSize(22, 22));
	icon.addFile(":/icons/date24.png",  QSize(24, 24));
	icon.addFile(":/icons/date32.png",  QSize(32, 32));
	icon.addFile(":/icons/date48.png",  QSize(48, 48));
	icon.addFile(":/icons/date64.png",  QSize(64, 64));
	icon.addFile(":/icons/date128.png", QSize(128, 128));
	setWindowIcon(icon);

	resize(300, 220);
	setFixedSize(width(), height());

	m_layout = new QHBoxLayout(this);

	m_calendar_layout = new QVBoxLayout();
	m_layout->addLayout(m_calendar_layout);

	m_button_layout = new QVBoxLayout();
	m_layout->addLayout(m_button_layout);

	m_text_date = new QCalendarWidget(this);
	m_text_date->setFirstDayOfWeek(Qt::Monday);
	m_text_date->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
	m_text_date->setGridVisible(true);
	m_calendar_layout->addWidget(m_text_date);

	m_text_time = new QTimeEdit(this);
	m_text_time->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
	m_text_time->setDisplayFormat("hh:mm");
	m_calendar_layout->addWidget(m_text_time);

	m_button_ok = new QPushButton(this);
	m_button_ok->setText(QString::fromUtf8("OK"));
	m_button_ok->setShortcut(QKeySequence("Return"));
	m_button_ok->setDefault(true);
	m_button_layout->addWidget(m_button_ok);

	m_button_cancel = new QPushButton(this);
	m_button_cancel->setText(QString::fromUtf8("Отмена"));
	m_button_cancel->setShortcut(QKeySequence("Esc"));
	m_button_layout->addWidget(m_button_cancel);

	m_spacer_button = new QSpacerItem(1, height(), QSizePolicy::Maximum, QSizePolicy::Expanding);
	m_button_layout->addItem(m_spacer_button);
}
//----------------------------------------------------------------------------------------------

FormDateUI::~FormDateUI ()
{
}
//----------------------------------------------------------------------------------------------
