/*!
 * \file
 * \brief GUI формы для ввода информации о модерировании
 */

#ifndef _avalon_form_moderate_ui_h_
#define _avalon_form_moderate_ui_h_

#include "../sysheaders.h"

/*!
 * \brief Описатель действия для списка действий при создании модерилки
 */
typedef struct AModerateAction
{
	QString Action;      /*!< \brief Внутреннее описание               */
	QString Description; /*!< \brief Человекочитаемое описание         */
	bool    ForumValid;  /*!< \brief Валидность ID форума для действия */
} AModerateAction;

/*!
 * \brief Описатель форума для перемещения при создании модерилки
 */
typedef struct AModerateForum
{
	int     ID;   /*!< \brief ID                        */
	QString Name; /*!< \brief Человекочитаемое описание */
} AModerateForum;

/*!
 * \brief GUI формы для ввода информации о модерировании
 */
class FormModerateUI : public QDialog
{
	Q_OBJECT

	public:

		/*!
		 * \brief Конструктор
		 * \param parent Родительский виджет
		 */
		FormModerateUI  (QWidget* parent);
		~FormModerateUI ();

	protected:

		/*!
		 * \brief Функция сохранения расположения и размеров элементов управления формы <при выходе>
		 */
		void save ();

		/*!
		 * \brief Функция восстановления расположения и размеров элементов управления формы <при старте>
		 */
		void restore ();

		/*!
		 * \brief Список возможных действий
		 */
		QList<AModerateAction> m_actions;

		/*!
		 * \brief Список форумов
		 */
		QList<AModerateForum>  m_forums;

		QLabel* m_label_action;      /*!< \brief Действие */
		QLabel* m_label_forum;       /*!< \brief Форум    */
		QLabel* m_label_description; /*!< \brief Описание */

		QComboBox* m_combo_action; /*!< \brief Действие */
		QComboBox* m_combo_forum;  /*!< \brief Форум    */

		QTextEdit* m_text_description; /*!< \brief Описание */

		QPushButton* m_button_ok;     /*!< \brief OK     */
		QPushButton* m_button_cancel; /*!< \brief Отмена */

		QHBoxLayout* m_layout;                               /*!< \brief Общий layout                  */
			QVBoxLayout* m_layout_left;                  /*!< \brief Layout левой части от кнопок  */
				QHBoxLayout* m_layout_combo_label;   /*!< \brief Layout для подписей и списков */
					QVBoxLayout* m_layout_label; /*!< \brief Layout для подписей           */
					QVBoxLayout* m_layout_combo; /*!< \brief Layout для списков            */
				QVBoxLayout* m_layout_description;   /*!< \brief Layout для описания           */
			QVBoxLayout* m_layout_button;                /*!< \brief Layout для кнопок             */

		QSpacerItem* m_spacer_button; /*!< \brief Удерживает кнопки вверху     */
		QSpacerItem* m_spacer_combo;  /*!< \brief Устанавливает ширину списков */

	private	slots:

		/*!
		 * \brief Смена списка с действиями
		 * \param text Текущий текст в списке
		 */
		void combo_action_current_index_changed (const QString& text);
};

#endif
