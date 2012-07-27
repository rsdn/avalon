#include "database_error.h"
//----------------------------------------------------------------------------------------------

ADatabaseError::ADatabaseError ()
{
}
//----------------------------------------------------------------------------------------------

ADatabaseError::~ADatabaseError ()
{
}
//----------------------------------------------------------------------------------------------

void ADatabaseError::showError (QWidget* parent)
{
	QMessageBox::critical(parent, QString::fromUtf8("Ошибка!"), m_error_message);
}
//----------------------------------------------------------------------------------------------

QString ADatabaseError::getLastError () const
{
	return m_error_message;
}
//----------------------------------------------------------------------------------------------

void ADatabaseError::setLastError (const QString& message)
{
	m_error_message = message;
}
//----------------------------------------------------------------------------------------------
