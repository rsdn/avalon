/*!
 * \file
 * \brief GUI формы запроса ввода пользователя (аналог InputBox)
 */

#ifndef _avalon_form_input_h_
#define _avalon_form_input_h_

#include "form_input_ui.h"

/*!
 * \brief GUI формы запроса ввода пользователя (аналог InputBox)
 */
class FormInput : public FormInputUI
{
	Q_OBJECT

	public:

		/*!
		 * \brief Конструктор
		 * \param parent Родительский виджет (форма)
		 * \param header Заголовок окна
		 * \param message Текст приглашения
		 * \param text Текст ввода
		 */
		FormInput  (QWidget* parent, const QString& header = QString(), const QString& message = QString(), const QString& text = QString());
		~FormInput ();

		QString text ();
};

#endif   // _avalon_form_input_h_
