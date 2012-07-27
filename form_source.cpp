#include "form_source.h"
//----------------------------------------------------------------------------------------------

FormSource::FormSource (QWidget* parent, const QString& body) :  FormSourceUI (parent)
{
	m_text_source->setPlainText(body);
}
//----------------------------------------------------------------------------------------------

FormSource::~FormSource ()
{
}
//----------------------------------------------------------------------------------------------

void FormSource::closeEvent (QCloseEvent* event)
{
	// сохранение layout
	save();

	event->accept();

	deleteLater();
}
//----------------------------------------------------------------------------------------------
