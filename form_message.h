/*!
 * \file
 * \brief Форма создания сообщения или ответа на сообщение
 *
 * $Date: 2010-07-03 19:05:06 +0400 (Сбт, 03 Июл 2010) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 326 $
 * $URL: svn://opensvn.ru/avalon/trunk/form_message.h $
 */

#ifndef _avalon_form_message_h_
#define _avalon_form_message_h_

#include "model/all.h"
#include "interfaces.h"
#include "form_message_ui.h"

/*!
 * \brief Форма создания сообщения или ответа на сообщение.
 */
class FormMessage : public FormMessageUI
{
	Q_OBJECT

	public:

		/*!
		 * \brief Конструктор.
		 * \param parent Родительская форма.
		 * \param is_reply true, если форма создана для ответа на сообщение.
		 * \param info Описатель с информацией о сообщении.
		 * \param edit_id ID редактирования существующего сообщения к отправке
		 */
		FormMessage  (QWidget* parent, bool is_reply, const AMessageInfoGUI& info, int edit_id = 0);
		~FormMessage ();

		/*!
		 * \brief Установка интерфейса для взаимодействия со списком форумов.
		 * \param itf Указатель на интерфейс
		 */
		void setForumTree (IForumTree* itf);

		/*!
		 * \brief Установка интерфейса для работы с главной формой
		 * \param itf Интерфейс
		 */
		void setMainForm (IFormMain* itf);

	private:

		IForumTree* m_forum_tree; /*!< \brief Интерфейс для взаимодействия со списком форумов */
		IFormMain*  m_main_form;  /*!< \brief Интерфейс главной формы                         */

		/*!
		 * \brief Флаг изменения текста
		 */
		bool m_text_changed;

		/*!
		 * \brief Текущее редактируемое сообщение.
		 */
		AMessageInfoGUI m_info;

		/*!
		 * \brief Форум, для которого предназначено сообщение
		 */
		AForumInfo m_forum_info;

		/*!
		 * \brief ID редактирования уже существующего сообщения к отправке
		 */
		int m_edit_id;

		/*!
		 * \brief Событие закрытия формы (см. Qt::QDialog).
		 */
		void closeEvent (QCloseEvent* event);

		/*!
		 * \brief Поместить сообщение к отправке.
		 * \param draft - флаг черновика
		 */
		void sendMessage (bool draft);

	private slots:

		/*!
		 * \brief Смена табов межу редактированием и предпросмотром.
		 */
		void tab_changed (int tab_index);

		/*!
		 * \brief Отправить.
		 */
		void button_send_triggered ();

		/*!
		 * \brief В черновики.
		 */
		void button_draft_triggered ();

		/*!
		 * \brief Смена текста темы сообщения.
		 */
		void text_subject_text_changed (const QString& text);

		/*!
		 * \brief Событие клика по линку
		 */
		void link_clicked (const QUrl& url);

		/*!
		 * \brief Событие нахождение мыши над линком
		 */
		void link_hover (const QString& link, const QString& title, const QString& textContent);

		/*!
		 * \brief Событие изменения текста сообщения
		 */
		void text_changed ();
};

#endif
