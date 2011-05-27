/*!
 * \file
 * \brief GUI формы подписки на форумы
 *
 * $Date: 2008-09-18 03:51:17 +0400 (Чтв, 18 Сен 2008) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 103 $
 * $URL: svn://opensvn.ru/avalon/trunk/form_subscribe_ui.h $
 */

#ifndef _avalon_form_subscribe_ui_h_
#define _avalon_form_subscribe_ui_h_

#include "sysheaders.h"

/*!
 * \brief GUI формы подписки на форумы
 */
class FormSubscribeUI : public QDialog
{
	Q_OBJECT

	public:

		FormSubscribeUI  (QWidget* parent);
		~FormSubscribeUI ();

	protected:

		QTreeWidget* m_tree_forum; /*!< \brief Дерево форумов */

		QPushButton* m_button_ok;          /*!< \brief OK           */
		QPushButton* m_button_refresh;     /*!< \brief Обновить     */
		QPushButton* m_button_select_all;  /*!< \brief Выбрать все  */
		QPushButton* m_button_select_none; /*!< \brief Отменить все */
		QPushButton* m_button_cancel;      /*!< \brief Отмена       */

		QHBoxLayout* m_layout_form;   /*!< \brief Layout формы  */
		QVBoxLayout* m_layout_tree;   /*!< \brief Layout дерева */
		QVBoxLayout* m_layout_button; /*!< \brief Layout кнопок */

		QSpacerItem* m_spacer_button; /*!< \brief Выравнивание кнопок вверх */
};

#endif
