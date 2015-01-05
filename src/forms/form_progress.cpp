#include "form_progress.h"
//----------------------------------------------------------------------------------------------

FormProgress::FormProgress (QWidget* parent) : FormProgressUI (parent), IProgress ()
{
	connect(m_button_cancel, SIGNAL(clicked()), this, SLOT(reject()));
}
//----------------------------------------------------------------------------------------------

FormProgress::~FormProgress ()
{
}
//----------------------------------------------------------------------------------------------

void FormProgress::onProgress (int percent)
{
	m_progress_bar->setMinimum(0);
	m_progress_bar->setMaximum(100);
	m_progress_bar->setValue(percent);

	QCoreApplication::processEvents();
}
//----------------------------------------------------------------------------------------------

void FormProgress::onProgress (int percent, const QString& status)
{
	if (status.length() > 0)
		new QListWidgetItem(status, m_list_progress);

	onProgress(percent);
}
//----------------------------------------------------------------------------------------------

void FormProgress::onProgress (int minimum, int maximum, int value)
{
	m_progress_bar->setMinimum(minimum);
	m_progress_bar->setMaximum(maximum);
	m_progress_bar->setValue(value);

	QCoreApplication::processEvents();
}
//----------------------------------------------------------------------------------------------

void FormProgress::onProgress (int minimum, int maximum, int value, const QString& status)
{
	if (status.length() > 0)
		new QListWidgetItem(status, m_list_progress);

	onProgress(minimum, maximum, value);
}
//----------------------------------------------------------------------------------------------
