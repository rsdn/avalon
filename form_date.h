/*!
 * \file
 * \brief Форма выбора даты/времени
 *
 * $Date: 2009-03-30 19:53:38 +0400 (Пнд, 30 Мар 2009) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 197 $
 * $URL: svn://opensvn.ru/avalon/trunk/form_date.h $
 */

#ifndef _avalon_form_date_h_
#define _avalon_form_date_h_

#include "form_date_ui.h"

/*!
 * \brief Форма выбора даты/времени
 */
class FormDate : public FormDateUI
{
	Q_OBJECT

	public:

		/*!
		 * \brief Конструктор
		 * \param parent Родительский виджет-форма
		 * \param as_read true для смены иконки на "как прочитанное"
		 */
		FormDate  (QWidget* parent, bool as_read);
		~FormDate ();

		/*!
		 * \brief Возвращает выбранную дату/время
		 * \return Дата и время
		 */
		QDateTime SelectedDate ()
		{
			return m_selected_datetime;
		}

	private:

		/*!
		 * \brief Выбранные дата и время
		 */
		QDateTime m_selected_datetime;

	private slots:

		/*!
		 * \brief OK
		 */
		void button_ok_clicked ();

};

#endif
