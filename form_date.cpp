#include "form_date.h"
//----------------------------------------------------------------------------------------------

FormDate::FormDate  (QWidget* parent, bool /*as_read*/) : FormDateUI (parent)
{
	QDateTime now = QDateTime::currentDateTime();
	m_selected_datetime = QDateTime(now.date(), QTime(now.time().hour(), now.time().minute()));

	m_text_date->setSelectedDate(m_selected_datetime.date());
	m_text_time->setTime(m_selected_datetime.time());

	connect(m_button_ok,     SIGNAL(clicked()), this, SLOT(button_ok_clicked()));
	connect(m_button_cancel, SIGNAL(clicked()), this, SLOT(reject()));
}
//----------------------------------------------------------------------------------------------

FormDate::~FormDate ()
{
}
//----------------------------------------------------------------------------------------------

void FormDate::button_ok_clicked ()
{
	m_selected_datetime.setDate(m_text_date->selectedDate());
	m_selected_datetime.setTime(m_text_time->time());

	accept();
}
//----------------------------------------------------------------------------------------------
