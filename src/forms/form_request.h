/*!
 * \file
 * \brief Форма для отображения прогресса работы
 */

#ifndef _avalon_form_request_h_
#define _avalon_form_request_h_

#include "iprogress.h"
#include "form_request_ui.h"

/*!
 * \brief Форма для отображения прогресса работы
 */
class FormRequest :
	public FormRequestUI,
	public IProgress
{
	Q_OBJECT

	public:

		/*!
		 * \brief Конструктор для простого прогресса
		 * \param parent Родительский виджет (форма)
		 */
		FormRequest  (QWidget* parent);
		~FormRequest ();

	// IProgress
	private:

		void onProgress (int percent);
		void onProgress (int percent, const QString& status);
		void onProgress (int minimum, int maximum, int value);
		void onProgress (int minimum, int maximum, int value, const QString& status);
};

#endif   // _avalon_form_request_h_
