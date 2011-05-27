/*!
 * \file
 * \brief Cабклассинг QWebView, т.к. он криво масштабируется и установка дефолтного поведения
 *
 * $Date: 2011-04-07 11:26:34 +0400 (Чтв, 07 Апр 2011) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 410 $
 * $URL: svn://opensvn.ru/avalon/trunk/web_view.h $
 */

#ifndef _avalon_web_view_h_
#define _avalon_web_view_h_

#include "sysheaders.h"

/*!
 * \brief Cабклассинг QWebView для установки дефолтного поведения
 */
class AWebView : public QWebView
{
	Q_OBJECT

	public:

		AWebView  (QWidget* parent);
		~AWebView () {}

		/*!
		 * \brief Флаг того, что мышь находится над ссылкой
		 * необходим для выдачи соответствующего QAction для возможности копирования ссылки, а не текста
		 * устанавливается в дочерних классах
		 */
		bool LinkHovered;

	protected:

		/*!
		 * \brief Кастомизация контекстного меню
		 */
		virtual void contextMenuEvent (QContextMenuEvent* event);

		/*!
		 * \brief Обработчик клавиш для реализации копирования по хоткею
		 */
		virtual void keyPressEvent (QKeyEvent* event);

	private slots:

		// меню
		void menu_yandex_triggered ();           /*!< \brief Поиск Яндекс         */
		void menu_wikipedia_triggered ();        /*!< \brief Поиск Википедии      */
		void menu_slovari_yandex_triggered ();   /*!< \brief Поиск Яндекс.Словари */
		void menu_google_triggered ();           /*!< \brief Поиск Google         */
		void menu_google_translate_triggered (); /*!< \brief Переводчик Google    */
		void menu_rsdn_triggered ();             /*!< \brief Поиск RSDN           */
};

/*!
 * \brief Cабклассинг AWebView, т.к. он криво масштабируется
 */
class AWebViewWidget : public QFrame
{
	Q_OBJECT

	public:

		AWebViewWidget  (QWidget* parent);
		~AWebViewWidget ();

		/*!
		 * \brief Объект сабклассинга
		 */
		AWebView* View;

	protected:

		/*!
		 * \brief Масштабирование
		 */
		void resizeEvent (QResizeEvent* event);
};

#endif
