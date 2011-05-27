//----------------------------------------------------------------------------------------------
// $Date: 2010-07-10 09:07:26 +0400 (Сбт, 10 Июл 2010) $
// $Author: antonbatenev.ya.ru $
// $Revision: 344 $
// $URL: svn://opensvn.ru/avalon/trunk/form_date.cpp $
//----------------------------------------------------------------------------------------------
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
