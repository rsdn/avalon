/*!
 * \file
 * \brief Класс QTextEdit с проверкой орфографии
 */

#ifndef _avalon_spell_edit_h_
#define _avalon_spell_edit_h_

#include "spellchecker.h"

#ifdef AVALON_USE_ASPELL

/*!
 * \brief Класс QTextEdit с проверкой орфографии
 */
class ASpellTextEdit : public QTextEdit
{
	Q_OBJECT

	public:

		/*!
		 * \brief Стандартный конструктор QTextEdit
		 */
		ASpellTextEdit  (QWidget* parent);
		~ASpellTextEdit ();

	protected:

		/*!
		 * \brief Перегрузка стандартного метода вызова контекстного меню QTextEdit
		 * для предложения вариантов исправления слов
		 */
		virtual void contextMenuEvent (QContextMenuEvent* event);

	private:

		/*!
		 * \brief Класс спеллчекера для предложений вариантов исправления слов
		 */
		ASpellChecker* m_spellchecker;

		/*!
		 * \brief Список вариантов для исправления слова
		 */
		QStringList m_suggest_list;

	private slots:

		/*!
		 * \brief Событие контекстного меню (на замену слова)
		 */
		void menu_triggered (QAction* action);

		/*!
		 * \brief Событие контекстного меню на добавление слова в словарь
		 */
		void menu_add_triggered ();
};

#else   // AVALON_USE_ASPELL

/*!
 * \brief Класс QTextEdit без проверки орфографии
 */
typedef QTextEdit ASpellTextEdit;

#endif   // AVALON_USE_ASPELL

#endif   // _avalon_spell_edit_h_
