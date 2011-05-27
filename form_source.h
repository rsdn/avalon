/*!
 * \file
 * \brief Форма просмотра сообщения в raw
 *
 * $Date: 2009-03-22 04:07:17 +0300 (Вск, 22 Мар 2009) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 172 $
 * $URL: svn://opensvn.ru/avalon/trunk/form_source.h $
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

#endif
