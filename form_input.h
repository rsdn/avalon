/*!
 * \file
 * \brief GUI формы запроса ввода пользователя (аналог InputBox)
 *
 * $Date: 2009-04-27 05:21:25 +0400 (Пнд, 27 Апр 2009) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 236 $
 * $URL: svn://opensvn.ru/avalon/trunk/form_input.h $
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

#endif
