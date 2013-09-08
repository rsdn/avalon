#include "formatter.h"
//----------------------------------------------------------------------------------------------
#include "global.h"
//----------------------------------------------------------------------------------------------

QString AFormatter::headHTML ()
{
	QString path = QCoreApplication::applicationDirPath();

	QString custom_css;
	if (QFileInfo(path + "/avalon.css").exists() == true)
#ifdef Q_WS_WIN
		custom_css = "	<link rel='stylesheet' href='file:///" + path + "/avalon.css' type='text/css' media='screen' />\n";
#else
		custom_css = "	<link rel='stylesheet' href='file://" + path + "/avalon.css' type='text/css' media='screen' />\n";
#endif

	QString result =
		"<!DOCTYPE html>\n"
		"<html xmlns='http://www.w3.org/1999/xhtml'>\n"
		"<head>\n"
		"	<meta charset='utf-8' />\n"
		"	<link rel='stylesheet' href='qrc:/style.css' type='text/css' media='screen' />\n"
		+ custom_css +
#ifdef Q_WS_WIN
		"	<link rel='stylesheet' title='Magula' href='file:///" + path + "/highlight/styles/magula.css' />\n"
		"	<script src='file:///" + path + "/highlight/highlight.pack.js'></script>\n"
#else
		"	<link rel='stylesheet' title='Magula' href='file://" + path + "/highlight/styles/magula.css' />\n"
		"	<script src='file://" + path + "/highlight/highlight.pack.js'></script>\n"
#endif
		"	<script>hljs.tabReplace = '    '; hljs.initHighlightingOnLoad();</script>\n"
		"</head>\n"
		"<body>\n";

	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::subjectHTML (int id, bool special, const QString subject)
{
	QString pad = "			";
	QString result = pad + "<div id='title'>\n";

	if (special == false)
	{
		result +=
			pad + "	<a id='move_to_thread' href='http://www.rsdn.ru/forum/message/" + QString::number(id) + QString::fromUtf8(".aspx'><img src='qrc:/icons/show_topic.png' title='показать положение в теме' /></a>\n") +
			pad + "	<a id='subject' href='http://www.rsdn.ru/forum/message/" + QString::number(id) + ".1.aspx'>" + subject + "</a>\n";
	}
	else
		result += pad + "<p id='subject'>" + subject + "</p>\n";

	result += pad + "</div>\n";

	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::rateHTML (bool special, bool rated)
{
	QString pad = "				";
	QString result = pad + "<div id='rate'>\n";

	if (rated == true)
	{
		result +=
			pad + QString::fromUtf8("<a id='rate_plus_1' href='avalon:rate_plus_1'><img src='qrc:/icons/rate_plus_1.png' title='+1' /></a>\n") +
			pad + QString::fromUtf8("<a id='rate_1' href='avalon:rate_1'><img src='qrc:/icons/rate_1.png' title='интересно' /></a>\n") +
			pad + QString::fromUtf8("<a id='rate_2' href='avalon:rate_2'><img src='qrc:/icons/rate_2.png' title='спасибо' /></a>\n") +
			pad + QString::fromUtf8("<a id='rate_3' href='avalon:rate_3'><img src='qrc:/icons/rate_3.png' title='супер' /></a>\n") +
			pad + QString::fromUtf8("<a id='rate_cross' href='avalon:rate_cross'><img src='qrc:/icons/rate_cross.png' title='удалить оценку' /></a>\n") +
			pad + QString::fromUtf8("<a id='rate_smile' href='avalon:rate_smile'><img src='qrc:/icons/rate_smile.png' title='смешно' /></a>\n") +
			pad + QString::fromUtf8("<a id='rate_plus' href='avalon:rate_plus'><img src='qrc:/icons/rate_plus.png' title='согласен' /></a>\n") +
			pad + QString::fromUtf8("<a id='rate_minus' href='avalon:rate_minus'><img src='qrc:/icons/rate_minus.png' title='не согласен' /></a>\n");
	}

	if (special == false)
		result += pad + QString::fromUtf8("<a id='moderate' href='avalon:moderate'><img src='qrc:/icons/moderate.png' title='модерирование' /></a>\n");

	result += pad + "</div>\n";

	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::authorHTML (int id, const QString nick)
{
	QString pad = "			";
	QString result;

	if (id == 0 || id == -1)
		result += pad + QString::fromUtf8("<div class='info_left'>От:</div><div class='info_right'>") + nick + "</div><br />\n";
	else
		result += pad + QString::fromUtf8("<div class='info_left'>От:</div><div class='info_right'><a href='http://www.rsdn.ru/Users/") + QString::number(id) + ".aspx'>" + nick + "</a></div><br />\n";

	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::ratingHTML (int id, const AMessageRatingList* rating_list)
{
	if (rating_list == NULL || rating_list->count() == 0)
		return "";

	QString result = "";

	/* "+1" = -3, "1" = 1, "2" = 2, "3" = 3, "+" = -4, "-" = 0, ";)" = -2 */
	int rate_plus_one = 0;
	int rate_one      = 0;
	int rate_two      = 0;
	int rate_three    = 0;
	int rate_plus     = 0;
	int rate_minus    = 0;
	int rate_smile    = 0;

	for (int i = 0; i < rating_list->count(); i++)
	{
		AMessageRating info = rating_list->at(i);

		int rate = info.Rate;

		if (rate == 1)
			rate_one++;
		else if (rate == 2)
			rate_two++;
		else if (rate == 3)
			rate_three++;
		else if (rate == 0)
			rate_minus++;
		else if (rate == -2)
			rate_smile++;
		else if (rate == -3)
			rate_plus_one++;
		else if (rate == -4)
			rate_plus++;
	}

	// может оказаться, что есть всего одна оценка и она не входит в диапазон допустимых - получится пустое пространство
	if (rate_plus_one > 0 || rate_one > 0 || rate_two > 0 || rate_three > 0 || rate_plus > 0 || rate_minus > 0 || rate_smile > 0)
	{
		QString pad = "			";

		result +=
			pad + "<div class='info_left'><a id='rating' href='http://www.rsdn.ru/forum/RateList.aspx?mid=" + QString::number(id) + QString::fromUtf8("'>Оценки</a>:</div>\n") +
			pad + "<div class='info_right'>\n";

		if (rate_smile > 0)
			result += pad + QString::fromUtf8("	<img id='rating_plus_1' src='qrc:/icons/rate_smile.png' title='смешно' />") + QString::number(rate_smile) + "\n";
		if (rate_plus > 0)
			result += pad + QString::fromUtf8("	<img id='rating_1' src='qrc:/icons/rate_plus.png' title='согласен' />") + QString::number(rate_plus) + "\n";
		if (rate_minus > 0)
			result += pad + QString::fromUtf8("	<img id='rating_2' src='qrc:/icons/rate_minus.png' title='не согласен' />") + QString::number(rate_minus) + "\n";
		if (rate_plus_one > 0)
			result += pad + QString::fromUtf8("	<img id='rating_3' src='qrc:/icons/rate_plus_1.png' title='+1' />") + QString::number(rate_plus_one) + "\n";
		if (rate_one > 0)
			result += pad + QString::fromUtf8("	<img id='rating_smile' src='qrc:/icons/rate_1.png' title='интересно' />") + QString::number(rate_one) + "\n";
		if (rate_two > 0)
			result += pad + QString::fromUtf8("	<img id='rating_plus' src='qrc:/icons/rate_2.png' title='спасибо' />") + QString::number(rate_two) + "\n";
		if (rate_three > 0)
			result += pad + QString::fromUtf8("	<img id='rating_minus' src='qrc:/icons/rate_3.png' title='супер' />") + QString::number(rate_three) + "\n";

		result += pad + "</div><br />\n";
	}

	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::footerHTML ()
{
	QString result =
		"	<div id='footer'></div>\n"

		// Web Of Trust (experemental)
		// http://www.mywot.com/en/blog/adding-ratings-to-your-website
		// http://www.mywot.com/wiki/API
		"	<script type='text/javascript'>\n"
		"		var wot_rating_options = {\n"
		"			exclude: /^(.+\\.){0,1}(rsdn\\.ru|yandex\\.ru|google\\.com|youtube\\.com|github\\.com|wikipedia\\.org)$/i\n"
		"		};\n"
		"	</script>\n"
		"	<script type='text/javascript' src='http://api.mywot.com/widgets/ratings.js'></script>\n"

		"</body>\n"
		"</html>\n";

	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::formatMessage (const AMessageInfo& message, bool special, bool rated, const AMessageRatingList* rating_list)
{
	// парсинг сообщения
	AParsedBlockList list = AParser::parseBlocks(message.Message);

	// тема письма хранится без html замен
	QString subject = message.Subject;

	subject.replace("&", "&amp;");
	subject.replace("<", "&lt;");
	subject.replace(">", "&gt;");

	// шапка html
	QString result = headHTML() +
	"	<div id='message'>\n"
	"		<div id='header'>\n";

	// заголовок
	result += subjectHTML(message.ID, special, message.Subject);

	// иконки для выставления оценок
	result += rateHTML(special, rated);

	// закрытие div.id = header
	result +=
	"		</div>\n"
	"		<div id='info'>\n";

	// автор сообщения + ссылка на профиль
	result += authorHTML(message.IDUser, message.UserNick.length() > 0 ? message.UserNick : QString::fromUtf8("Аноним"));

	// дата сообщения
	result += QString::fromUtf8("			<div class='info_left'>Дата:</div><div class='info_right'>") + message.MessageDate.toString("dd.MM.yyyy HH:mm") + "</div><br />\n";

	// рейтинг сообщения
	result += ratingHTML(message.ID, rating_list);

	// закрытие div.id = info
	result += "		</div>\n";

	// формирование тела сообщения
	result += "		<div id='text'>\n";

	for (int i = 0; i < list.count(); i++)
        result += formatParsedBlock(list.at(i));

	result += "		</div>\n";

	// закрытие div.id = message
	result += "	</div>\n";

	// хвост html
	result += footerHTML();

	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::formatParsedBlock (const AParsedBlock& block)
{
	QString pad = "			";
	QString result;

	if (block.Type == pbtText)
	{
		result += formatQuotedStringList(block.Strings, block.Type, block.SubType);
	}

	else if (block.Type == pbtQuote)
	{
		result += pad + "<blockquote>\n";
		result += formatQuotedStringList(block.Strings, block.Type, block.SubType);
		result += pad + "</blockquote>\n";
	}

	else if (block.Type == pbtModerator)
	{
		result += pad + "<div class='moderator'>\n";
		result += formatQuotedStringList(block.Strings, block.Type, block.SubType);
		result += pad + "</div>\n";
	}

	else if (block.Type == pbtTagline)
	{
		result += pad + "<div class='tagline'>\n";
		result += formatQuotedStringList(block.Strings, block.Type, block.SubType);
		result += pad + "</div>\n";
	}

	else if (block.Type == pbtTable)
	{
		result += pad + "<table><tbody>\n";
		result += formatQuotedStringList(block.Strings, block.Type, block.SubType);
		result += pad + "</tbody></table>\n";
	}

	else if (block.Type == pbtCut)
	{
		for (int i = 0; i < block.SubBlocks.count(); i++)
			result += formatParsedBlock(block.SubBlocks.at(i));
	}

	else
	{
		/*!
		 * \brief Описатель соответствий языку и имени для подсветки
		 */
		typedef struct AHighlightMap
		{
			AParsedBlockType Type;      /*!< \brief Тип кода для подсветки    */
			const char*      CodeClass; /*!< \brief Класс кода в Highlight.js */
		} AHighlightMap;

		// Список замены для подсветки синтаксиса Highlight.js
		// см. http://softwaremaniacs.org/soft/highlight/
		const AHighlightMap highlight_map [] =
		{
			{ pbtCode,      ""         },
			{ pbtAssembler, "avrasm"   },
			{ pbtC,         "cpp"      },
			{ pbtCPP,       "cpp"      },
			{ pbtCSharp,    "cs"       },
			{ pbtMSIL,      "cpp"      },
			{ pbtIDL,       "cpp"      },
			{ pbtPascal,    "delphi"   },
			{ pbtBasic,     "vbscript" },
			{ pbtSQL,       "sql"      },
			{ pbtPerl,      "perl"     },
			{ pbtPHP,       "php"      },
			{ pbtJava,      "java"     },
			{ pbtXML,       "xml"      },
			{ pbtLisp,      "lisp"     },
			{ pbtHaskell,   "haskell"  },
			{ pbtErlang,    "erlang"   },
			{ pbtOCaml,     "lisp"     },
			{ pbtProlog,    "lisp"     },
			{ pbtPython,    "python"   },
			{ pbtRuby,      "ruby"     },
			{ pbtNemerle,   "cs"       },
			{ pbtText,      NULL       }  // last
		};

		// массив тэгов для подсветки кода
		const AHighlightMap* map = highlight_map;

		QString code_class;

		while (map->CodeClass != NULL)
		{
			if (block.Type == map->Type)
			{
				code_class = QString::fromUtf8(map->CodeClass);
				break;
			}

			map++;
		}

		if (code_class.length() > 0)
			result += pad + "<pre><code class='" + code_class + "'>";
		else
			result += pad + "<pre><code>";

		result += formatQuotedStringList(block.Strings, block.Type, block.SubType);

		result += "</code></pre>\n";
	}

	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::formatQuotedStringList (const AQuotedStringList& list, AParsedBlockType type, AParsedBlockSubType sub_type)
{
	QString result;

	int last_quote_level = -1;

	int count = list.count();

	for (int i = 0; i < count; i++)
	{
		AQuotedString string = list.at(i);

		QString line = string.Data;

		if (string.QuoteText.length() > 0)
			line = string.QuoteText + " " + line;

		if (type != pbtTable && sub_type != pbstSourceCode)
		{
			QString pad = "			";

			if (string.QuoteLevel != 0)
			{
				if (string.QuoteLevel % 3 == 0)
					result += pad + "<p class='quote_blue'> " + line + " </p>\n";
				else if (string.QuoteLevel % 2 == 0)
					result += pad + "<p class='quote_red'> " + line + " </p>\n";
				else
					result += pad + "<p class='quote_green'> " + line + " </p>\n";
			}
			else
				result += pad + "<p class='quote_black'> " + line + " </p>\n";

			last_quote_level = string.QuoteLevel;
		}
		else
		{
			if (i != count - 1)
				result += line + "\n";
			else
				result += line;
		}
	}

	if (sub_type != pbstSourceCode)
		result = formatSimpleText(result);

	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::formatSimpleText (const QString& text)
{
	/*!
	 * \brief Описатель соответствий простых BB тэгов и их html замен
	 */
	typedef struct ASimpleTag
	{
		const char* Source;  /*!< \brief Имя тэга (со скобками) */
		const char* Replace; /*!< \brief Текст замены           */
	} ASimpleTag;

	// Список замены BB кодов
	const ASimpleTag replace_map [] =
	{
		{ "[b]",       "<b>"       },
		{ "[/b]",      "</b>"      },
		{ "[i]",       "<i>"       },
		{ "[/i]",      "</i>"      },
		{ "[u]",       "<u>"       },
		{ "[/u]",      "</u>"      },
		{ "[s]",       "<strike>"  },
		{ "[/s]",      "</strike>" },
		{ "[tr]",      "<tr>"      },
		{ "[/tr]",     "</tr>"     },
		{ "[td]",      "<td>"      },
		{ "[/td]",     "</td>"     },
		{ "[th]",      "<th>"      },
		{ "[/th]",     "</th>"     },
		{ "[hr]",      "<hr />"    },
		{ "[h1]",      "<h1>"      },
		{ "[/h1]",     "</h1>"     },
		{ "[h2]",      "<h2>"      },
		{ "[/h2]",     "</h2>"     },
		{ "[h3]",      "<h3>"      },
		{ "[/h3]",     "</h3>"     },
		{ "[h4]",      "<h4>"      },
		{ "[/h4]",     "</h4>"     },
		{ "[h5]",      "<h5>"      },
		{ "[/h5]",     "</h5>"     },
		{ "[h6]",      "<h6>"      },
		{ "[/h6]",     "</h6>"     },
		{ "[*]",       "<li>"      },
		{ ":)))",      "<img src='qrc:/smiles/sideways.png'>"   },
		{ ":))",       "<img src='qrc:/smiles/biggrin.png'>"    },
		{ ":)",        "<img src='qrc:/smiles/smile.png'>"      },
		{ ":(",        "<img src='qrc:/smiles/sad.png'>"        },
		{ ";)",        "<img src='qrc:/smiles/wink.png'>"       },
		{ ":-\\",      "<img src='qrc:/smiles/pouty.png'>"      },
		{ ":???:",     "<img src='qrc:/smiles/wacko.png'>"      },
		{ ":no:",      "<img src='qrc:/smiles/stop.png'>"       },
		{ ":up:",      "<img src='qrc:/smiles/thumbs_up.png'>"  },
		{ ":down:",    "<img src='qrc:/smiles/thumb_down.png'>" },
		{ ":super:",   "<img src='qrc:/smiles/cool.png'>"       },
		{ ":shuffle:", "<img src='qrc:/smiles/blush.png'>"      },
		{ ":wow:",     "<img src='qrc:/smiles/shock.png'>"      },
		{ ":crash:",   "<img src='qrc:/smiles/angry.png'>"      },
		{ ":user:",    "<img src='qrc:/smiles/music.png'>"      },
		{ ":maniac:",  "<img src='qrc:/smiles/devil.png'>"      },
		{ ":xz:",      "<img src='qrc:/smiles/wrong.png'>"      },
		{ ":beer:",    "<img src='qrc:/smiles/beer.png'>"       }
	};

	QString result = text;

	for (size_t i = 0; i < sizeof(replace_map) / sizeof(ASimpleTag); i++)
		result.replace(QString::fromUtf8(replace_map[i].Source), QString::fromUtf8(replace_map[i].Replace), Qt::CaseInsensitive);

	//
	// [img] ... [/img]
	//

	QRegExp img("\\[img\\](.+)\\[/img\\]", Qt::CaseInsensitive, QRegExp::RegExp);

	img.setMinimal(true);

	int index = 0;

	while ((index = img.indexIn(result, index)) != -1)
	{
		QString html;
		QString lstr = img.cap(1).trimmed();
		int     lval = AParser::isURL(lstr);

		if (lval == 1)
			html = "<center><img src='" + lstr + "'></center>";
		else if (lval == 2) // опасная ссылка
			html = "<span class='alert'>" + lstr + "</span>";
		else // невалидная ссылка
			html = lstr;

		result.replace(img.cap(0), html);

		index += std::min(img.matchedLength(), html.length());
	}

	//
	// списки
	//

	QRegExp list1("\\[list=1\\](.+)\\[/list\\]", Qt::CaseInsensitive, QRegExp::RegExp);
	QRegExp list2("\\[list=a\\](.+)\\[/list\\]", Qt::CaseInsensitive, QRegExp::RegExp);
	QRegExp list3("\\[list\\](.+)\\[/list\\]",   Qt::CaseInsensitive, QRegExp::RegExp);

	list1.setMinimal(true);
	list2.setMinimal(true);
	list3.setMinimal(true);

	result.replace(list1, "<ol type='1' start='1'>\\1</ol>");
	result.replace(list2, "<ol type='a'>\\1</ol>");
	result.replace(list3, "<ul>\\1</ul>");

	// гиперссылки
	result = formatHyperlinks(result);

	result = "			" + result;

	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::formatHyperlinks (const QString& text)
{
	QString result = text;

	QRegExp url1("\\[url\\](.+)\\[/url\\]",        Qt::CaseInsensitive, QRegExp::RegExp);
	QRegExp url2("\\[url=(\\S+)\\](.+)\\[/url\\]", Qt::CaseInsensitive, QRegExp::RegExp);

	url1.setMinimal(true);
	url2.setMinimal(true);

	int index = 0;

	while ((index = url1.indexIn(result, index)) != -1)
	{
		QString html;
		QString lstr = url1.cap(1);
		int     lval = AParser::isURL(lstr);

		if (lval == 1)
			html = QString::fromUtf8("<a href='") + lstr + "'>" + lstr + "</a>";
		else if (lval == 2) // опасная ссылка
			html = "<span class='alert'>" + lstr + "</span>";
		else // невалидная ссылка
			html = lstr;

		result.replace(url1.cap(0), html);

		index += std::min(url1.matchedLength(), html.length());
	}

	index = 0;

	while ((index = url2.indexIn(result, index)) != -1)
	{
		QString html;

		QString lstr = url2.cap(1);
		QString rstr = url2.cap(2);

		int lval = AParser::isURL(lstr);
		int rval = AParser::isURL(rstr);

		if (lval == 1)
			html = "<a href='" + lstr + "'>" + rstr + "</a>";
		else if (rval == 1)
			html = "<a href='" + rstr + "'>" + lstr + "</a>";
		else if (lval == 2)
			html = "<span class='alert'>" + rstr + "</span>";
		else if (rval == 2)
			html = "<span class='alert'>" + lstr + "</span>";
		else // невалидная ссылка
			html = rstr + " (" + lstr + ")";

		result.replace(url2.cap(0), html);

		index += std::min(url2.matchedLength(), html.length());
	}

	// ссылки без тэгов
	QRegExp url3("[^'>]((http://|https://|ftp://|[^/]www\\.)[^<\\s]+)", Qt::CaseInsensitive, QRegExp::RegExp);

	index = 0;

	while ((index = url3.indexIn(result, index)) != -1)
	{
		QString html;
		QString lstr = url3.cap(1);
		int     lval = AParser::isURL(lstr);

		if (lval == 1)
		{
			html = QString::fromUtf8("<a href='") + lstr + "'>" + QUrl::fromPercentEncoding(lstr.toUtf8()) + "</a>";
			result.replace(lstr, html);
			index += std::min(url3.matchedLength(), html.length());
		}
		else // невалидная или опасная ссылка
			index += url3.matchedLength();
	}

	// email url
	QRegExp email("\\[email\\](\\S+)\\[/email\\]", Qt::CaseInsensitive, QRegExp::RegExp);

	email.setMinimal(true);

	index = 0;

	while ((index = email.indexIn(result, index)) != -1)
	{
		QString html;
		QString lstr = email.cap(1);
		int     lval = AParser::isURL(lstr);

		if (lval == 1)
			html = QString::fromUtf8("<a href='mailto:") + lstr + "'>" + lstr + "</a>";
		else if (lval == 2)
			html = lstr;
		else // невалидная ссылка
			html = lstr;

		result.replace(email.cap(0), html);

		index += std::min(email.matchedLength(), html.length());
	}

	// msdn url
	QRegExp msdn("\\[msdn\\](\\S+)\\[/msdn\\]", Qt::CaseInsensitive, QRegExp::RegExp);
	msdn.setMinimal(true);
	result.replace(msdn, "<a href='http://search.msdn.microsoft.com/Default.aspx?brand=Msdn&query=\\1'>\\1</a>");

	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::normalizeBody (const QString& body, const QString& nick)
{
	QString data = body;

	// отбрасываем "[tagline]" и "[moderator]"
	QRegExp tagline("\\[tagline\\](.+)\\[/tagline\\]",       Qt::CaseInsensitive);
	QRegExp moderator("\\[moderator\\](.+)\\[/moderator\\]", Qt::CaseInsensitive);

	tagline.setMinimal(true);
	moderator.setMinimal(true);

	data.replace(tagline,   "");
	data.replace(moderator, "");

	// удаляем img с даными вместо ссылки (например, см. http://www.rsdn.ru/forum/flame.comp/4077971.1.aspx)
	QRegExp img1("\\[img\\]data:(\\S+)\\[/img\\]", Qt::CaseInsensitive);
	img1.setMinimal(true);
	data.replace(img1, "");

	// заменяем [img]...[/img] на ссылку
	QRegExp img2("\\[img\\](\\S+)\\[/img\\]", Qt::CaseInsensitive);
	img2.setMinimal(true);
	data.replace(img2, QString::fromUtf8("[url=\\1]\\1[/url]"));

	// укорачивание длинных ссылок (например, см. http://www.rsdn.ru/forum/web/4086359.1.aspx)
	QRegExp url1("\\[url=data:(\\S+)\\](.+)\\[/url\\]", Qt::CaseInsensitive);
	url1.setMinimal(true);
	data.replace(url1, "[url=bad%20link]\\2[/url]");

	// удаление тэга цитирования
	QRegExp q1("(^|[^\\[])\\[q\\]", Qt::CaseInsensitive);
	q1.setMinimal(true);
	data.replace(q1, "\\1");

	QRegExp q2("(^|[^\\[])\\[/q\\]", Qt::CaseInsensitive);
	q2.setMinimal(true);
	data.replace(q2, "\\1");

	// удаление таблиц из цитирования
	QRegExp table("(^|[^\\[])\\[t\\](.+)\\[/t\\]", Qt::CaseInsensitive);
	table.setMinimal(true);
	data.replace(table, "\\1");

	// удаление тэгов [h1]..[h6] из цитирования
	for (int i = 1; i < 7; i++)
	{
		QRegExp h1("(^|[^\\[])\\[h" + QString::number(i) + "\\]", Qt::CaseInsensitive);
		h1.setMinimal(true);
		data.replace(h1, "\\1");

		QRegExp h2("(^|[^\\[])\\[/h" + QString::number(i) + "\\]", Qt::CaseInsensitive);
		h2.setMinimal(true);
		data.replace(h2, "\\1");
	}

	data = data.trimmed();

	// разбиение
	QStringList source = data.split("\n");

	QString nick_3 = AFormatter::nick3(nick);

	// добавление квотинга к строкам
	for (int i = 0; i < source.size(); i++)
		if (source[i].trimmed().length())
			source[i] = nick_3 + "> " + source[i];

	// удаление дублирующихся пустых строк
	int index = 0;

	// регексп для приветствий в квотинге
	QRegExp hello(QString::fromUtf8(">\\s{0,}Здравствуйте,\\s.+,\\sВы писали:"));

	while (index < source.size() - 1)
	{
		if (source.at(index).trimmed().length() == 0 && source.at(index + 1).trimmed().length() == 0)
		{
			source.removeAt(index);
			continue;
		}
		else if (source.at(index).indexOf(hello) != -1)
		{
			source.removeAt(index);

			if (index > 0)
				index--;

			continue;
		}

		index++;
	}

	// вычисление уровня квотинга
	QList<int> quoting_level;

	int size = source.size();

	for (int i = 0; i < size; i++)
	{
		QString temp = source.at(i).trimmed();

		int level = 0;

		for (int j = 0; j < temp.length() - 3; j++)
		{
			if (temp[j] == ' ' && !(temp[j + 1] == '&' || temp[j + 2] == '&' || temp[j + 3] == '&'))
				break;
			else if (temp[j] == '&' && temp[j + 1] == 'g' && temp[j + 2] == 't' && temp[j + 3] == ';')
			{
				level++;
				j += 3;
			}
			else if (temp[j] == '&' && temp[j + 1] == 'l' && temp[j + 2] == 't' && temp[j + 3] == ';')
				break;
		}

		quoting_level.append(level);

		source[i] = temp;
	}

	// вставка пустых строк между разными уровнями квотинга
	for (int i = 0; i < size - 1; i++)
	{
		if (source[i].length() == 0)
			continue;

		if (quoting_level.at(i) != quoting_level.at(i + 1) && source[i + 1].length() /*&& (quoting_level.at(i) == 0 || quoting_level.at(i + 1) == 0)*/)
			source[i] = source[i] + "\r\n";
	}

	data = source.join("\r\n").trimmed();

	// замена HTML спец-символов
	data.replace("&gt;",  ">");
	data.replace("&lt;",  "<");
	data.replace("&amp;", "&");

	return data;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::nick3 (const QString& nick)
{
	if (nick.length() < 4)
		return nick;

	QString result;

	for (int i = 0; i < nick.length(); i++)
		if (nick[i].isUpper() == true)
			result += nick[i];

	if (result.length() == 0)
		result = nick[0];

	if (result.length() > 3)
		result.resize(3);

	return result;
}
//----------------------------------------------------------------------------------------------
