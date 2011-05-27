/*!
 * \file
 * \brief Виджет отображения сообщений
 *
 * $Date: 2011-04-07 11:11:49 +0400 (Чтв, 07 Апр 2011) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 406 $
 * $URL: svn://opensvn.ru/avalon/trunk/message_view.h $
 */

#ifndef _avalon_message_view_h_
#define _avalon_message_view_h_

#include "web_view.h"
#include "interfaces.h"

/*!
 * \brief Виджет отображения сообщений
 */
class AMessageView :
	public AWebViewWidget,
	public IMessageView // интерфес для взаимодействия со списком сообщений и форумов
{
	Q_OBJECT

	public:

		AMessageView  (QWidget* parent);
		~AMessageView ();

		/*!
		 * \brief Установка интерфейса для работы с деревом форумов
		 * \param itf Интерфейс
		 */
		void setForumTree (IForumTree* itf);

		/*!
		 * \brief Установка интерфейса для работы с деревом сообщений
		 * \param itf Интерфейс
		 */
		void setMessageTree (IMessageTree* itf);

		/*!
		 * \brief Установка интерфейса для работы с главной формой
		 * \param itf Интерфейс
		 */
		void setMainForm (IFormMain* itf);

	private:

		IForumTree*   m_forum_tree;   /*!< \brief Интерфейс дерева форумов   */
		IMessageTree* m_message_tree; /*!< \brief Интерфейс дерева сообщений */
		IFormMain*    m_main_form;    /*!< \brief Интерфейс главной формы    */

	// IMessageView
	private:

		void clear ();
		void setMessage (const AMessageInfo& message, bool special = false, bool rated = false);

	private slots:

		/*!
		 * \brief Клик по линку
		 * \param url URL линка
		 */
		void link_clicked (const QUrl& url);

		/*!
		 * \brief Перемещение курсора над линком (см. Qt::QWebView)
		 */
		void link_hover (const QString& link, const QString& title, const QString& textContent);
};

#endif
