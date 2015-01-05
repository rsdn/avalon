/*!
 * \file
 * \brief GUI формы запроса ввода пользователя (аналог InputBox)
 */

#ifndef _avalon_form_input_ui_h_
#define _avalon_form_input_ui_h_

#include "sysheaders.h"

/*!
 * \brief GUI формы запроса ввода пользователя (аналог InputBox)
 */
class FormInputUI : public QDialog
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
		FormInputUI  (QWidget* parent, const QString& header = QString(), const QString& message = QString(), const QString& text = QString());
		~FormInputUI ();

	protected:

		QHBoxLayout*       m_layout;          /*!< \brief Основной layout           */
		   QVBoxLayout*    m_form_layout;     /*!< \brief Layout формы              */
		      QLabel*      m_label;           /*!< \brief Надпись                   */
		      QLineEdit*   m_text;            /*!< \brief Текст                     */
		   QVBoxLayout*    m_button_layout;   /*!< \brief Layout кнопок             */
		      QPushButton* m_button_ok;       /*!< \brief OK                        */
		      QPushButton* m_button_cancel;   /*!< \brief Отмена                    */
		      QSpacerItem* m_spacer_button;   /*!< \brief Выравнивание кнопок вверх */
};

#endif   // _avalon_form_input_ui_h_
