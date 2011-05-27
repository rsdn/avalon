/*!
 * \file
 * \brief GUI формы просмотра сообщения в raw
 *
 * $Date: 2009-03-22 04:07:17 +0300 (Вск, 22 Мар 2009) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 172 $
 * $URL: svn://opensvn.ru/avalon/trunk/form_source_ui.h $
 */

#ifndef _avalon_form_source_ui_h_
#define _avalon_form_source_ui_h_

#include "sysheaders.h"

/*!
 * \brief GUI формы просмотра сообщения в raw
 */
class FormSourceUI : public QDialog
{
	Q_OBJECT

	public:

		FormSourceUI  (QWidget* parent);
		~FormSourceUI ();

	protected:

		/*!
		 * \brief Функция сохранения расположения и размеров элементов управления формы <при выходе>
		 */
		void save ();

		/*!
		 * \brief Функция восстановления расположения и размеров элементов управления формы <при старте>
		 */
		void restore ();

		QVBoxLayout*  m_layout;      /*!< \brief Общий layout        */
		QTextEdit*    m_text_source; /*!< \brief Raw текст сообщения */
};

#endif
