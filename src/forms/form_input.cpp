#include "form_input.h"
//----------------------------------------------------------------------------------------------

FormInput::FormInput (QWidget* parent, const QString& header, const QString& message, const QString& text) : FormInputUI (parent, header, message, text)
{
}
//----------------------------------------------------------------------------------------------

FormInput::~FormInput ()
{
}
//----------------------------------------------------------------------------------------------

QString FormInput::text ()
{
	return m_text->text();
}
//----------------------------------------------------------------------------------------------
