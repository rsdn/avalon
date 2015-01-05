#include "spellchecker.h"
//----------------------------------------------------------------------------------------------
#ifdef AVALON_USE_ASPELL
//----------------------------------------------------------------------------------------------

ASpellChecker::ASpellChecker (QTextDocument* parent) : QSyntaxHighlighter (parent)
{
	QFile file(dictFile());

	if (file.open(QIODevice::ReadOnly) == true)
	{
		QDataStream in(&file);
		in >> m_stop_words;
		file.close();
	}

	m_aspell_speller_en = NULL;
	m_aspell_speller_ru = NULL;

	m_aspell_config_en = new_aspell_config();
	m_aspell_config_ru = NULL;

	if (m_aspell_config_en != NULL)
	{
		//
		// спеллер английского языка
		//

		bool fail = false;

		if (aspell_config_replace(m_aspell_config_en, "lang", "en_US") == false)
			fail = true;

		if (fail == false && aspell_config_replace(m_aspell_config_en, "encoding", "utf-8") == false)
			fail = true;

		if (fail == false)
		{
			AspellCanHaveError* possible_err = new_aspell_speller(m_aspell_config_en);

			if (aspell_error_number(possible_err) == 0)
				m_aspell_speller_en = to_aspell_speller(possible_err);
		}

		//
		// Спеллер русского языка
		//

		fail = false;

		m_aspell_config_ru = aspell_config_clone(m_aspell_config_en);

		if (m_aspell_config_ru != NULL)
		{
			if (aspell_config_replace(m_aspell_config_ru, "lang", "ru_RU") == false)
				fail = true;

			if (fail == false && aspell_config_replace(m_aspell_config_ru, "encoding", "utf-8") == false)
				fail = true;

			if (fail == false)
			{
				AspellCanHaveError* possible_err = new_aspell_speller(m_aspell_config_ru);

				if (aspell_error_number(possible_err) == 0)
					m_aspell_speller_ru = to_aspell_speller(possible_err);
			}

		}   // if (fail == false)

		//
		// Проверка инициализации и освобождение ресурсов в случае неудачи
		//

		if (m_aspell_speller_en == NULL)
		{
			delete_aspell_config(m_aspell_config_en);
			m_aspell_config_en = NULL;
		}

		if (m_aspell_speller_ru == NULL)
		{
			delete_aspell_config(m_aspell_config_ru);
			m_aspell_config_ru = NULL;
		}

	}   // if (m_aspell_config_en != NULL)
}
//----------------------------------------------------------------------------------------------

ASpellChecker::~ASpellChecker ()
{
	delete_aspell_speller(m_aspell_speller_en);
	delete_aspell_speller(m_aspell_speller_ru);
	delete_aspell_config(m_aspell_config_en);
	delete_aspell_config(m_aspell_config_ru);
}
//----------------------------------------------------------------------------------------------

void ASpellChecker::markWord (bool correct, int start, int length)
{
	QTextCharFormat format;

	if (correct == true)
		format.setUnderlineStyle(QTextCharFormat::NoUnderline);
	else
	{
		format.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
		format.setUnderlineColor(Qt::red);
	}

	setFormat(start, length, format);
}
//----------------------------------------------------------------------------------------------

void ASpellChecker::suggestList (AspellSpeller* speller, const char* word, int size, QStringList* suggest_list)
{
	const AspellWordList* suggestions = aspell_speller_suggest(speller, word, size);

	AspellStringEnumeration* elements = aspell_word_list_elements(suggestions);

	const char* suggest;

	while ((suggest = aspell_string_enumeration_next(elements)) != NULL)
		suggest_list->append(QString::fromUtf8(suggest));

	delete_aspell_string_enumeration(elements);
}
//----------------------------------------------------------------------------------------------

void ASpellChecker::highlightBlock (const QString& text)
{
	if (m_aspell_speller_en != NULL)
	{
		QRegExp word_regexp(QString::fromUtf8("[a-zA-Z]+[a-zA-Z\\-]+[a-zA-Z]+"));

		int pos = 0;

		while ((pos = word_regexp.indexIn(text, pos)) != -1)
		{
			int len = word_regexp.matchedLength();

			QString word = text.mid(pos, len);

			if (m_stop_words.contains(word) == false && isNativeCase(word) == true)
			{
				QByteArray data = word.toUtf8();

				int correct = aspell_speller_check(m_aspell_speller_en, data.data(), data.length());

				if (correct == 1)
					markWord(true, pos, len);
				else if (correct == 0)
					markWord(false, pos, len);
			}

			pos += len;

		}   // while ((pos = word_regexp.indexIn(text, pos)) != -1)

	}   // if (m_aspell_speller_en != NULL)

	if (m_aspell_speller_ru != NULL)
	{
		QRegExp word_regexp(QString::fromUtf8("[а-яА-ЯёЁ]+[а-яА-ЯёЁ\\-]+[а-яА-ЯёЁ]+"));

		int pos = 0;

		while ((pos = word_regexp.indexIn(text, pos)) != -1)
		{
			int len = word_regexp.matchedLength();

			QString word = text.mid(pos, len);

			if (m_stop_words.contains(word) == false)
			{
				word = word.replace(QString::fromUtf8("ё"), QString::fromUtf8("е"));
				word = word.replace(QString::fromUtf8("Ё"), QString::fromUtf8("Е"));

				QByteArray data = word.toUtf8();

				int correct = aspell_speller_check(m_aspell_speller_ru, data.data(), data.length());

				if (correct == 1)
					markWord(true, pos, len);
				else if (correct == 0)
					markWord(false, pos, len);
			}

			pos += len;

		}   // while ((pos = word_regexp.indexIn(text, pos)) != -1)

	}   // if (m_aspell_speller_ru != NULL)
}
//----------------------------------------------------------------------------------------------

int ASpellChecker::spellWord (const QString& text, QStringList* suggest_list)
{
	if (m_aspell_speller_en != NULL)
	{
		QRegExp word_regexp(QString::fromUtf8("^[a-zA-Z]+[a-zA-Z\\-]+[a-zA-Z]+$"));

		int pos = word_regexp.indexIn(text);

		if (pos == 0)
		{
			if (m_stop_words.contains(text) == true || isNativeCase(text) == false)
				return 0;

			QByteArray data = text.toUtf8();

			int correct = aspell_speller_check(m_aspell_speller_en, data.data(), data.length());

			if (correct == -1)
				return -1;
			else if (correct == 1)
				return 0;

			suggestList(m_aspell_speller_en, data.data(), data.length(), suggest_list);

			return 1;

		}   // if (pos == 0)

	}   // if (m_aspell_speller_en != NULL)

	if (m_aspell_speller_ru != NULL)
	{
		QRegExp word_regexp(QString::fromUtf8("^[а-яА-ЯёЁ]+[а-яА-ЯёЁ\\-]+[а-яА-ЯёЁ]+$"));

		QString word = text;

		int pos = word_regexp.indexIn(word);

		if (pos == 0)
		{
			if (m_stop_words.contains(word) == true)
				return 0;

			word = word.replace(QString::fromUtf8("ё"), QString::fromUtf8("е"));
			word = word.replace(QString::fromUtf8("Ё"), QString::fromUtf8("Е"));

			QByteArray data = word.toUtf8();

			int correct = aspell_speller_check(m_aspell_speller_ru, data.data(), data.length());

			if (correct == -1)
				return -1;
			else if (correct == 1)
				return 0;

			suggestList(m_aspell_speller_ru, data.data(), data.length(), suggest_list);

			return 1;

		}   // if (pos == 0)

	}   // if (m_aspell_speller_ru != NULL)

	return 0;
}
//----------------------------------------------------------------------------------------------

bool ASpellChecker::isNativeCase (const QString& text)
{
	int lower = 0;
	int upper = 0;

	for (int i = 0; i < text.length(); i++)
	{
		if (text[i].isUpper() == true)
		{
			upper++;

			if (lower > 0)
				return false;
		}
		else if (text[i].isLower() == true)
		{
			lower++;

			if (upper > 1)
				return false;
		}
	}

	return true;
}
//----------------------------------------------------------------------------------------------

QString ASpellChecker::dictFile ()
{
#ifndef AVALON_PACKAGE
	QString path = QCoreApplication::applicationDirPath();
#else
	QString path = QDir::homePath() + "/.avalon";
#endif

	return path + "/avalon.dic";
}
//----------------------------------------------------------------------------------------------

bool ASpellChecker::addWord (const QString& text)
{
	QFile file(dictFile());

	if (file.open(QIODevice::WriteOnly | QIODevice::Truncate) == true)
	{
		m_stop_words.insert(text);

		QDataStream out(&file);

		out << m_stop_words;

		file.close();

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------------------------
#endif // AVALON_USE_ASPELL
//----------------------------------------------------------------------------------------------
