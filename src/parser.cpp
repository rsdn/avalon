#include "parser.h"
//----------------------------------------------------------------------------------------------

AQuotedString AParser::parseQuote (const QString& line)
{
	int length = line.length();

	AQuotedString result;
	result.QuoteLevel = 0;

	// короткие строки или строки, начинающиеся с пробела разбирать не имеет смысла
	if (length < 4 || line[0] == ' ')
	{
		result.Data = line;
		return result;
	}

	for (int i = 0; i < length - 3; i++)
	{
		bool found = false;

		// поиск символа квотирования с "забеганием вперед" не более чем на три возможных символа в квоте
		for (int j = i; j - i < 5 && j < length - 3; j++)
			if (line[j] == '&' && line[j + 1] == 'g' && line[j + 2] == 't' && line[j + 3] == ';')
			{
				QString quote_part  = line.mid(i, j - i + 4);
				int     space_count = quote_part.count(' ');

				// пробел может быть только в начале квоты и количеством не более одного
				if ((space_count == 1 && quote_part[0] != ' ') || space_count > 1)
					break;

				found = true;
				result.QuoteLevel++;
				result.QuoteText += quote_part;
				i = j + 3;

				break;
			}

		if (found == false)
			break;
	}

	result.Data = line.mid(result.QuoteText.length(), length - result.QuoteText.length());
	result.QuoteText.replace(" ", "");

	return result;
}
//----------------------------------------------------------------------------------------------

AQuotedStringList AParser::parseQuotes (const QString& source)
{
	AQuotedStringList result;

	QStringList list = source.split('\n');

	for (int i = 0; i < list.count(); i++)
	{
		// удаление второй части из CRLF, если таковая имеется
		QString source_line = list[i];
		source_line.replace("\r", "");

		AQuotedString parsed_line = parseQuote(source_line);

		result.append(parsed_line);
	}

	return result;
}
//----------------------------------------------------------------------------------------------

AParsedBlockList AParser::parseBlocks (const QString& source)
{
	/*!
	 * \brief Описатель "монолитного" тэга для парсера сообщений
	 */
	typedef struct AStrongTag
	{
		const char*         OpenPart;  /*!< \brief Открывающая часть */
		const char*         ClosePart; /*!< \brief Закрывающая часть */
		AParsedBlockType    Type;      /*!< \brief Тип тэга-блока    */
		AParsedBlockSubType SubType;   /*!< \brief Подтип тэга-блока */
	} AStrongTag;

	// Список "монолитных" тэгов для парсера сообщений
	const AStrongTag strong_tags [] =
	{
		// тэги кода
		{ "[code]",             "[/code]",      pbtCode,      pbstSourceCode },
		{ "[asm]",              "[/asm]",       pbtAssembler, pbstSourceCode },
		{ "[ccode]",            "[/ccode]",     pbtC,         pbstSourceCode },
		{ "[c]",                "[/c]",         pbtC,         pbstSourceCode },
		{ "[cpp]",              "[/cpp]",       pbtCPP,       pbstSourceCode },
		{ "[vc]",               "[/vc]",        pbtCPP,       pbstSourceCode },
		{ "[c#]",               "[/c#]",        pbtCSharp,    pbstSourceCode },
		{ "[csharp]",           "[/csharp]",    pbtCSharp,    pbstSourceCode },
		{ "[cs]",               "[/cs]",        pbtCSharp,    pbstSourceCode },
		{ "[msil]",             "[/msil]",      pbtMSIL,      pbstSourceCode },
		{ "[midl]",             "[/midl]",      pbtIDL,       pbstSourceCode },
		{ "[pascal]",           "[/pascal]",    pbtPascal,    pbstSourceCode },
		{ "[vb]",               "[/vb]",        pbtBasic,     pbstSourceCode },
		{ "[sql]",              "[/sql]",       pbtSQL,       pbstSourceCode },
		{ "[perl]",             "[/perl]",      pbtPerl,      pbstSourceCode },
		{ "[php]",              "[/php]",       pbtPHP,       pbstSourceCode },
		{ "[java]",             "[/java]",      pbtJava,      pbstSourceCode },
		{ "[xml]",              "[/xml]",       pbtXML,       pbstSourceCode },
		{ "[lisp]",             "[/lisp]",      pbtLisp,      pbstSourceCode },
		{ "[haskell]",          "[/haskell]",   pbtHaskell,   pbstSourceCode },
		{ "[ruby]",             "[/ruby]",      pbtRuby,      pbstSourceCode },
		{ "[nemerle]",          "[/nemerle]",   pbtNemerle,   pbstSourceCode },
		{ "[python]",           "[/python]",    pbtPython,    pbstSourceCode },

		// https://rsdn.ru/forum/rsdn/3227340.1
		{ "[code=]",            "[/code]",      pbtCode,      pbstSourceCode },
		{ "[code=assembler]",   "[/code]",      pbtAssembler, pbstSourceCode },
		{ "[code=c]",           "[/code]",      pbtC,         pbstSourceCode },
		{ "[code=csharp]",      "[/code]",      pbtCSharp,    pbstSourceCode },
		{ "[code=erlang]",      "[/code]",      pbtErlang,    pbstSourceCode },
		{ "[code=haskell]",     "[/code]",      pbtHaskell,   pbstSourceCode },
		{ "[code=idl]",         "[/code]",      pbtIDL,       pbstSourceCode },
		{ "[code=java]",        "[/code]",      pbtJava,      pbstSourceCode },
		{ "[code=lisp]",        "[/code]",      pbtLisp,      pbstSourceCode },
		{ "[code=msil]",        "[/code]",      pbtMSIL,      pbstSourceCode },
		{ "[code=ocaml]",       "[/code]",      pbtOCaml,     pbstSourceCode },
		{ "[code=pascal]",      "[/code]",      pbtPascal,    pbstSourceCode },
		{ "[code=perl]",        "[/code]",      pbtPerl,      pbstSourceCode },
		{ "[code=php]",         "[/code]",      pbtPHP,       pbstSourceCode },
		{ "[code=prolog]",      "[/code]",      pbtProlog,    pbstSourceCode },
		{ "[code=python]",      "[/code]",      pbtPython,    pbstSourceCode },
		{ "[code=ruby]",        "[/code]",      pbtRuby,      pbstSourceCode },
		{ "[code=sql]",         "[/code]",      pbtSQL,       pbstSourceCode },
		{ "[code=visualbasic]", "[/code]",      pbtBasic,     pbstSourceCode },
		{ "[code=xsl]",         "[/code]",      pbtXML,       pbstSourceCode },

		// https://rsdn.ru/forum/cpp/3482377.1
		{ "[code=cpp]",         "[/code]",      pbtCPP,       pbstSourceCode },

		// тэги, которые не обрабатываются подсветкой кода
		{ "[tagline]",          "[/tagline]",   pbtTagline,   pbstText  },
		{ "[moderator]",        "[/moderator]", pbtModerator, pbstText  },
		{ "[q]",                "[/q]",         pbtQuote,     pbstText  },
		{ "[t]",                "[/t]",         pbtTable,     pbstText  },
	};

	AParsedBlockList result;

	// проверка на вхождения тэгов cut - тэг "жадный"
	QRegExp cut_rx("\\[cut=?([^\\]]*)\\](.*)\\[/cut\\]", Qt::CaseInsensitive, QRegExp::RegExp2 /* greedy */);

	int cut_pos = cut_rx.indexIn(source);

	if (cut_pos != -1)
	{
		//qDebug() << cut_rx.capturedTexts();

		// TODO: Если кому-то в голову придет вставить большое количество вложенных тэгов cut в сообщение,
		// то грохнется с переполнением стека (но интересно же кому придет и выдержит ли форматтер RSDN :)

		int     cut_length  = cut_rx.cap(0).length();
		QString pre_source  = source.mid(0, cut_pos);
		QString post_source = source.mid(cut_pos + cut_length, source.length() - cut_pos - cut_length);

		if (pre_source.length() > 0)
			result += parseBlocks(pre_source);

		AParsedBlock block;

		block.Type      = pbtCut;
		block.SubType   = pbstBlock;
		block.Title     = cut_rx.cap(1);
		block.SubBlocks = parseBlocks(cut_rx.cap(2));

		result.append(block);

		if (post_source.length() > 0)
			result += parseBlocks(post_source);

		return result;
	}

	AParsedBlock current_block;

	current_block.Type    = pbtText;
	current_block.SubType = pbstText;

	AQuotedStringList source_quote_list = parseQuotes(source);

	for (int i = 0; i < source_quote_list.count(); i++)
	{
		AQuotedString current_quote;

		current_quote.QuoteLevel = source_quote_list[i].QuoteLevel;
		current_quote.QuoteText  = source_quote_list[i].QuoteText;

		QString data = source_quote_list[i].Data;

		// пустая строка на входе
		if (data.length() == 0)
		{
			// текст исходного кода оставляем без изменений
			if (current_block.Type != pbtText)
				current_block.Strings.append(current_quote);
			// игнорируем более одной пустой строки подряд в рамках квоты одного уровня
			else if (current_block.Strings.count() > 0 && current_block.Strings.last().QuoteLevel == current_quote.QuoteLevel && current_block.Strings.last().Data.length() > 0)
				current_block.Strings.append(current_quote);
		}

		while (data.length() > 0)
		{
			// поиск начала тэга
			int pos1 = data.indexOf('[');

			if (pos1 == -1)
			{
				// открывающий тэг не найден - добавляется вся строка
				current_quote.Data += data;
				current_block.Strings.append(current_quote);
				break;
			}

			// проверка того, что открывающая скобка не стоит в конце строки
			if (pos1 + 1 == data.length())
			{
				// открывающая скобка в конце строки - это не тэг
				current_quote.Data += data;
				current_block.Strings.append(current_quote);
				break;
			}

			// проверка на двойную открывающую скобку (экранирование "[")
			if (data[pos1 + 1] == '[')
			{
				current_quote.Data += data.mid(0, pos1) + "&#x5B;";

				// в исходном коде "[" не экранируется
				if (current_block.SubType == pbstSourceCode)
					data.remove(0, pos1 + 1);
				else
					data.remove(0, pos1 + 2);

				continue;
			}

			// найдено открытие начала тэга, поиск закрытия
			int pos2 = data.indexOf(']', pos1);

			if (pos2 == -1)
			{
				current_quote.Data += data;
				current_block.Strings.append(current_quote);
				break;
			}

			// получение тэга
			QString tag  = data.mid(pos1, pos2 - pos1 + 1);
			QString ltag = tag.toLower();

			current_quote.Data += data.mid(0, pos1);
			data.remove(0, pos2 + 1);

			//
			// проверка на то, что тэг в списке монолитных и требуется разрыв
			// или закрытие открытого монолитного тэга
			//

			// найденный описатель тэга
			const AStrongTag* found_tag = NULL;

			// флаг того, что ищется открывающий тэг
			bool search_open_tag = false;

			if (current_block.Type == pbtText)
				search_open_tag = true;

			if (search_open_tag == true)
			{
				// поиск открывающего тэга
				for (size_t i = 0; i < sizeof(strong_tags) / sizeof(AStrongTag); i++)
					if (ltag == strong_tags[i].OpenPart)
					{
						found_tag = &strong_tags[i];
						break;
					}
			}
			else
			{
				// поиск закрывающего тэга
				for (size_t i = 0; i < sizeof(strong_tags) / sizeof(AStrongTag); i++)
					if (ltag == strong_tags[i].ClosePart && strong_tags[i].Type == current_block.Type)
					{
						found_tag = &strong_tags[i];
						break;
					}
			}

			// найден монолитный тэг, однако, требуется проверить, что он где-то закрывается
			// подробнее поведение в тесте на сайте: https://rsdn.ru/forum/test/4212824.1
			if (found_tag != NULL && search_open_tag == true)
			{
				QString close_part = found_tag->ClosePart;

				// проверяем хвост разбираемой строки
				if (data.indexOf(close_part, Qt::CaseInsensitive) == -1)
				{
					// в хвосте строки закрывающий тэг не найден, проверяем остальные строки
					bool found = false;

					for (int j = i; j < source_quote_list.count(); j++)
						if (source_quote_list[j].Data.indexOf(close_part, Qt::CaseInsensitive) != -1)
						{
							found = true;
							break;
						}

					// если закрывающий тэг не найден, то считаем,
					// что открывающий тэг является частью предыдущего блока
					if (found == false)
						found_tag = NULL;
				}
			}

			// найден простой немонолитный тэг
			if (found_tag == NULL)
			{
				// двойной тэг перечеркивания - приводим к одному
				if (ltag == "[strike]")
					ltag = "[s]";
				else if (ltag == "[/strike]")
					ltag = "[/s]";

				// специальный случай ссылки - содержимое ссылки не должно менять регистр
				if (ltag.indexOf("[url=") == 0)
				{
					ltag = tag;
					ltag.replace(0, 5, "[url=");
				}

				current_quote.Data += ltag;

				// условие выполнится, когда строка заканчивается немонолитным тэгом
				if (data.length() == 0)
					current_block.Strings.append(current_quote);

				continue;
			}

			//
			// найден монолитный открывающий или закрывающий тэг
			//

			// добавление хвоста квоты в текущий блок
			if (current_quote.Data.length() > 0)
				current_block.Strings.append(current_quote);

			result.append(current_block);

			current_quote.Data = "";

			current_block.Strings.clear();

			if (search_open_tag == true)
			{
				current_block.Type    = found_tag->Type;
				current_block.SubType = found_tag->SubType;
			}
			else
			{
				current_block.Type    = pbtText;
				current_block.SubType = pbstText;
				current_block.Title   = "";
			}
		}
	}

	// добавление "хвоста"
	if (current_block.Strings.count() > 0)
		result.append(current_block);

	// зачистка от пустых строк в блоках сверху и снизу
	int index = 0;

	while (index < result.count())
	{
		// зачистка пустых строк сверху
		while (result[index].Strings.isEmpty() == false)
			if (result[index].Strings.first().Data.trimmed().length() == 0)
				result[index].Strings.removeFirst();
			else
				break;

		// зачистка пустых строк снизу
		while (result[index].Strings.isEmpty() == false)
			if (result[index].Strings.last().Data.trimmed().length() == 0)
				result[index].Strings.removeLast();
			else
				break;

		// зачистка блоков с пустым количеством строк / подблоков
		if (result[index].Strings.isEmpty() == true && result[index].SubBlocks.isEmpty() == true)
			result.removeAt(index);
		else
			index++;
	}

	return result;
}
//----------------------------------------------------------------------------------------------

int AParser::isURL (const QString& text)
{
	QUrl url = QUrl::fromUserInput(text);

	if (url.isValid() == false)
		return 0;

	if (url.scheme() == "data" || url.scheme() == "file")
		return 2;

	QString host = url.host();

	if (host.indexOf('.') == -1)
		return 0;

	return 1;
}
//----------------------------------------------------------------------------------------------
