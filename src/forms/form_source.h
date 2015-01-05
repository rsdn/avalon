/*!
 * \file
 * \brief Форма просмотра сообщения в raw
 */

#ifndef _avalon_form_source_h_
#define _avalon_form_source_h_

#include "form_source_ui.h"

/*!
 * \brief Форма просмотра сообщения в raw
 */
class FormSource : public FormSourceUI
{
	Q_OBJECT

	public:

		/*!
		 * \brief Конструктор
		 * \param parent Родительский виджет (форма)
		 * \param body RAW текст сообщения
		 */
		FormSource  (QWidget* parent, const QString& body);
		~FormSource ();

	protected:

		/*!
		 * \brief Событие закрытия формы (см. Qt::QDialog).
		 */
		void closeEvent (QCloseEvent* event);
};

#endif   // _avalon_form_source_h_
