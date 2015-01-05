/*!
 * \file
 * \brief Класс подсветки синтаксиса орфографии
 */

#ifndef _avalon_spellchecker_h_
#define _avalon_spellchecker_h_

#include "sysheaders.h"

#ifdef AVALON_USE_ASPELL

/*!
 * \brief Класс подсветки синтаксиса орфографии
 */
class ASpellChecker : public QSyntaxHighlighter
{
	Q_OBJECT

	public:

		/*!
		 * \brief Стандартный конструктор QSyntaxHighlighter
		 */
		ASpellChecker  (QTextDocument* parent);
		~ASpellChecker ();

		/*!
		 * \brief Метод проверки слова
		 * \param text Слово для проверки (в случае, если слово не одно, проверка всегда положительна)
		 * \param suggest_list Список предложений, если слово неправильно
		 * \return -1 ошибка, 0 - ОК, 1 - в слове ошибка, в списке suggest варианты исправления
		 */
		int spellWord (const QString& text, QStringList* suggest_list);

		/*!
		 * \brief Метод добавления слова в словарь
		 * \param text Слово для добавления
		 * \return false - ошибка сохранения словаря
		 */
		bool addWord (const QString& text);

	protected:

		/*!
		 * \brief Метод QSyntaxHighlighter для подсветки
		 */
		virtual void highlightBlock (const QString& text);

	private:

		/*!
		 * \brief Метод подсветки блока текста
		 * \param correct Флаг корректности слова (true - слово правильное)
		 * \param start Начало слова
		 * \param length Длина слова
		 */
		void markWord (bool correct, int start, int length);

		/*!
		 * \brief Метод получения списка вариантов для замены ошибочного слова
		 * \param speller Класс проверки орфографии для требуемого языка
		 * \param word Ошибочное слово в кодировке UTF-8
		 * \param size Длина слова в байтах
		 * \param suggest_list Список вариантов
		 */
		void suggestList (AspellSpeller* speller, const char* word, int size, QStringList* suggest_list);

		/*!
		 * \brief Метод определения слов со "сбитым" регистром (эти слова исключаются из проверки)
		 * Используется только для латиницы, чтобы игнорировать составные идентификаторы в участках кода
		 * \param text слово для проверки
		 * \return true, если слово с естественным регистром букв,
		 * false, если слово со "сбитым" регистром букв:
		 *
		 * country - true - слово в нижнем регистре
		 * USSR - true - аббревиатура
		 * Russia - true - имена собственные могут начинаться с заглавной
		 * camelCase - false - заглавная буква в середине слова
		 * TUrl - false - более одной заглавной буквы при наличии строчных
		 *
		 */
		bool isNativeCase (const QString& text);

		/*!
		 * \brief Получение имени файла для словаря
		 * \return Имя файла словаря
		 */
		QString dictFile ();

		AspellConfig*  m_aspell_config_en;    /*!< \brief Конфиг aspell для проверки английского  */
		AspellConfig*  m_aspell_config_ru;    /*!< \brief Конфиг aspell для проверки русского     */

		AspellSpeller* m_aspell_speller_en;   /*!< \brief Спеллер aspell для проверки английского */
		AspellSpeller* m_aspell_speller_ru;   /*!< \brief Спеллер aspell для проверки русского    */

		QSet<QString>  m_stop_words;          /*!< \brief Стоп-слова (rsdn specific)              */
};

#endif   // AVALON_USE_ASPELL

#endif   // _avalon_spellchecker_h_
