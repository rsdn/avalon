/*!
 * \file
 * \brief Форма подписки на форумы
 *
 * $Date: 2008-09-18 03:51:17 +0400 (Чтв, 18 Сен 2008) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 103 $
 * $URL: svn://opensvn.ru/avalon/trunk/form_subscribe.h $
 */

#ifndef _avalon_form_subscribe_h_
#define _avalon_form_subscribe_h_

#include "model/all.h"
#include "form_subscribe_ui.h"

/*!
 * \brief Форма подписки на форумы
 */
class FormSubscribe : public FormSubscribeUI
{
	Q_OBJECT

	public:

		FormSubscribe  (QWidget* parent);
		~FormSubscribe ();

	private:

		/*!
		 * \brief Cписок подписаных форумов при последней загрузке
		 */
		ASubscribedForumInfoList m_last_subscribed;

		/*!
		 * \brief Перезагрузка дерева форумов из хранилища
		 */
		void reload ();

	private slots:

		void button_ok_clicked          (); /*!< \brief OK           */
		void button_refresh_clicked     (); /*!< \brief Отмена       */
		void button_select_all_clicked  (); /*!< \brief Выбрать все  */
		void button_select_none_clicked (); /*!< \brief Отменить все */
};

#endif
