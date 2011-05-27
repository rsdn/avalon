/*!
 * \file
 * \brief GUI формы выбора даты/времени
 *
 * $Date: 2010-08-10 04:24:45 +0400 (Втр, 10 Авг 2010) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 350 $
 * $URL: svn://opensvn.ru/avalon/trunk/form_date_ui.h $
 */

#ifndef _avalon_form_date_ui_h_
#define _avalon_form_date_ui_h_

#include "sysheaders.h"

/*!
 * \brief GUI формы выбора даты/времени
 */
class FormDateUI : public QDialog
{
	Q_OBJECT

	public:

		FormDateUI  (QWidget* parent);
		~FormDateUI ();

	protected:

		QHBoxLayout* m_layout;                  /*!< \brief Общий layout         */
		   QVBoxLayout* m_calendar_layout;      /*!< \brief Layout для календаря */
		      QCalendarWidget* m_text_date;     /*!< \brief Календарь            */
		      QTimeEdit*       m_text_time;     /*!< \brief Время                */
		   QVBoxLayout* m_button_layout;        /*!< \brief Layout для кнопок    */
		      QPushButton* m_button_ok;         /*!< \brief OK                   */
		      QPushButton* m_button_cancel;     /*!< \brief Отмена               */
		      QSpacerItem* m_spacer_button;     /*!< \brief Выравнивание кнопок  */
};

#endif
