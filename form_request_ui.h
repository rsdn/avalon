/*!
 * \file
 * \brief GUI формы для отображения прогресса работы и для HTTP запросов
 *
 * $Date: 2010-08-26 05:43:49 +0400 (Чтв, 26 Авг 2010) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 358 $
 * $URL: svn://opensvn.ru/avalon/trunk/form_request_ui.h $
 */

#ifndef _avalon_form_request_ui_h_
#define _avalon_form_request_ui_h_

#include "sysheaders.h"

/*!
 * \brief GUI формы для отображения прогресса работы и для HTTP запросов
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
