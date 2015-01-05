/*!
 * \file
 * \brief GUI формы для отображения прогресса работы
 */

#ifndef _avalon_form_request_ui_h_
#define _avalon_form_request_ui_h_

#include "sysheaders.h"

/*!
 * \brief GUI формы для отображения прогресса работы
 */
class FormRequestUI : public QDialog
{
	Q_OBJECT

	public:

		FormRequestUI  (QWidget* parent);
		~FormRequestUI ();

	protected:

		QVBoxLayout*        m_layout;        /*!< \brief Основной layout                       */
		   QVBoxLayout*     m_layout_form;   /*!< \brief Layout для прогресса и списка событий */
		      QProgressBar* m_progress_bar;  /*!< \brief Прогресс события                      */
		      QListWidget*  m_list_progress; /*!< \brief Список событий                        */
		   QHBoxLayout*     m_layout_button; /*!< \brief Layout для кнопки                     */
		      QPushButton*  m_button_cancel; /*!< \brief Отмена                                */
};

#endif   // _avalon_form_request_ui_h_
