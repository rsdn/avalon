//----------------------------------------------------------------------------------------------
// $Date: 2010-08-26 06:32:00 +0400 (Чтв, 26 Авг 2010) $
// $Author: antonbatenev.ya.ru $
// $Revision: 359 $
// $URL: svn://opensvn.ru/avalon/trunk/spell_edit.cpp $
//----------------------------------------------------------------------------------------------
#include "spell_edit.h"
//----------------------------------------------------------------------------------------------
#ifdef AVALON_USE_ASPELL
//----------------------------------------------------------------------------------------------

ASpellTextEdit::ASpellTextEdit (QWidget* parent) : QTextEdit (parent)
{
	m_spellchecker = NULL;

	if (QSettings().value("ui/spellchecker", true).toInt() == true)
		m_spellchecker = new ASpellChecker(document());
}
//----------------------------------------------------------------------------------------------

ASpellTextEdit::~ASpellTextEdit ()
{
	delete m_spellchecker;
}
//----------------------------------------------------------------------------------------------

void ASpellTextEdit::contextMenuEvent (QContextMenuEvent* event)
{
	QMenu* menu = createStandardContextMenu();

	if (m_spellchecker != NULL)
	{
		QString selected_text = textCursor().selectedText();

		if (selected_text.length() > 0)
		{
			m_suggest_list.clear();

			if (m_spellchecker->spellWord(selected_text, &m_suggest_list) == 1)
			{
				if (m_suggest_list.count() > 0)
				{
					menu->addSeparator();

					for (int i = 0; i < m_suggest_list.count() && i < 10; i++)
					{
						QAction* action = menu->addAction(m_suggest_list[i]);
						action->setMenuRole(QAction::ApplicationSpecificRole);
					}

					connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(menu_triggered(QAction*)));

					menu->addSeparator();

					QAction* action = menu->addAction(QString::fromUtf8("Добавить в словарь"));
					connect(action, SIGNAL(triggered()), this, SLOT(menu_add_triggered()));
				}
			}
		}
	}

	menu->exec(event->globalPos());

	delete menu;
}
//----------------------------------------------------------------------------------------------

void ASpellTextEdit::menu_triggered (QAction* action)
{
	if (action->menuRole() == QAction::ApplicationSpecificRole)
		textCursor().insertText(action->text());
}
//----------------------------------------------------------------------------------------------

void ASpellTextEdit::menu_add_triggered ()
{
	QString selected_text = textCursor().selectedText();

	if (m_spellchecker->addWord(selected_text) == true)
		m_spellchecker->rehighlight();
}
//----------------------------------------------------------------------------------------------
#endif   // AVALON_USE_ASPELL
//----------------------------------------------------------------------------------------------
