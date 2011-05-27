/*!
 * \file
 * \brief Форма для ввода информации о модерировании
 *
 * $Date: 2009-03-22 04:07:17 +0300 (Вск, 22 Мар 2009) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 172 $
 * $URL: svn://opensvn.ru/avalon/trunk/form_moderate.h $
 */

#ifndef _avalon_form_moderate_h_
#define _avalon_form_moderate_h_

#include "form_moderate_ui.h"
#include "iforum_tree.h"

/*!
 * \brief Форма для ввода информации о модерировании
 */
class FormModerate : public FormModerateUI
{
	Q_OBJECT

	public:

		/*!
		 * \brief Конструктор
		 * \param parent Родительский виджет / форма
		 * \param id_message ID сообщения, для которого ставится модерилка
		 * \param edit_id ID редактирования существующей модерилки к отправке
		 */
		FormModerate  (QWidget* parent, int id_message, int edit_id = 0);
		~FormModerate ();

		/*!
		 * \brief Установка интерфейса для взаимодействия с деревом форумов
		 * \param itf Указатель на интерфейс
		 */
		void setForumTree (IForumTree* itf);

	protected:

		/*!
		 * \brief Событие закрытия формы (см. Qt::QDialog).
		 */
		virtual void closeEvent (QCloseEvent* event);

	private:

		/*!
		 * \brief Интерфейс для взаимодействия с деревом форумов
		 */
		IForumTree* m_forum_tree;

		/*!
		 * \brief ID сообщения
		 */
		int m_id_message;

		/*!
		 * \brief ID редактируемой модерилки
		 */
		int m_id_edit;

	private slots:

		/*!
		 * \brief OK
		 */
		void button_ok_clicked ();

		/*!
		 * \brief Отмена
		 */
		void button_cancel_clicked ();
};

#endif
