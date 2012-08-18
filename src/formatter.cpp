#include "formatter.h"
//----------------------------------------------------------------------------------------------
#include "global.h"
//----------------------------------------------------------------------------------------------
/*!
 * \brief Регексп для проверки URL
 * http://web.archive.org/web/20070807114147/http://www.foad.org/~abigail/Perl/url2.html
 * http://web.archive.org/web/20070705044149/www.foad.org/~abigail/Perl/url3.regex
 */
const QString g_url_regex = "(?:http://(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?)(?:/(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;:@&=])*)(?:/(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;:@&=])*))*)(?:\\?(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;:@&=])*))?)?)|(?:ftp://(?:(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;?&=])*)(?::(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;?&=])*))?@)?(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?))(?:/(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[?:@&=])*)(?:/(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[?:@&=])*))*)(?:;type=[AIDaid])?)?)|(?:news:(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;/?:&=])+@(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3})))|(?:[a-zA-Z](?:[a-zA-Z\\d]|[_.+-])*)|\\*))|(?:nntp://(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?)/(?:[a-zA-Z](?:[a-zA-Z\\d]|[_.+-])*)(?:/(?:\\d+))?)|(?:telnet://(?:(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;?&=])*)(?::(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;?&=])*))?@)?(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?))/?)|(?:gopher://(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?)(?:/(?:[a-zA-Z\\d$\\-_.+!*'(),;/?:@&=]|(?:%[a-fA-F\\d]{2}))(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),;/?:@&=]|(?:%[a-fA-F\\d]{2}))*)(?:%09(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;:@&=])*)(?:%09(?:(?:[a-zA-Z\\d$\\-_.+!*'(),;/?:@&=]|(?:%[a-fA-F\\d]{2}))*))?)?)?)?)|(?:wais://(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?)/(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))*)(?:(?:/(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))*)/(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))*))|\\?(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;:@&=])*))?)|(?:mailto:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),;/?:@&=]|(?:%[a-fA-F\\d]{2}))+))|(?:file://(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))|localhost)?/(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[?:@&=])*)(?:/(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[?:@&=])*))*))|(?:prospero://(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?)/(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[?:@&=])*)(?:/(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[?:@&=])*))*)(?:(?:;(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[?:@&])*)=(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[?:@&])*)))*)|(?:ldap://(?:(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?))?/(?:(?:(?:(?:(?:(?:(?:[a-zA-Z\\d]|%(?:3\\d|[46][a-fA-F\\d]|[57][Aa\\d]))|(?:%20))+|(?:OID|oid)\\.(?:(?:\\d+)(?:\\.(?:\\d+))*))(?:(?:%0[Aa])?(?:%20)*)=(?:(?:%0[Aa])?(?:%20)*))?(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))*))(?:(?:(?:%0[Aa])?(?:%20)*)\\+(?:(?:%0[Aa])?(?:%20)*)(?:(?:(?:(?:(?:[a-zA-Z\\d]|%(?:3\\d|[46][a-fA-F\\d]|[57][Aa\\d]))|(?:%20))+|(?:OID|oid)\\.(?:(?:\\d+)(?:\\.(?:\\d+))*))(?:(?:%0[Aa])?(?:%20)*)=(?:(?:%0[Aa])?(?:%20)*))?(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))*)))*)(?:(?:(?:(?:%0[Aa])?(?:%20)*)(?:[;,])(?:(?:%0[Aa])?(?:%20)*))(?:(?:(?:(?:(?:(?:[a-zA-Z\\d]|%(?:3\\d|[46][a-fA-F\\d]|[57][Aa\\d]))|(?:%20))+|(?:OID|oid)\\.(?:(?:\\d+)(?:\\.(?:\\d+))*))(?:(?:%0[Aa])?(?:%20)*)=(?:(?:%0[Aa])?(?:%20)*))?(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))*))(?:(?:(?:%0[Aa])?(?:%20)*)\\+(?:(?:%0[Aa])?(?:%20)*)(?:(?:(?:(?:(?:[a-zA-Z\\d]|%(?:3\\d|[46][a-fA-F\\d]|[57][Aa\\d]))|(?:%20))+|(?:OID|oid)\\.(?:(?:\\d+)(?:\\.(?:\\d+))*))(?:(?:%0[Aa])?(?:%20)*)=(?:(?:%0[Aa])?(?:%20)*))?(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))*)))*))*(?:(?:(?:%0[Aa])?(?:%20)*)(?:[;,])(?:(?:%0[Aa])?(?:%20)*))?)(?:\\?(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))+)(?:,(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))+))*)?)(?:\\?(?:base|one|sub)(?:\\?(?:((?:[a-zA-Z\\d$\\-_.+!*'(),;/?:@&=]|(?:%[a-fA-F\\d]{2}))+)))?)?)?)|(?:(?:z39\\.50[rs])://(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?)(?:/(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))+)(?:\\+(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))+))*(?:\\?(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))+))?)?(?:;esn=(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))+))?(?:;rs=(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))+)(?:\\+(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))+))*)?))|(?:cid:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;?:@&=])*))|(?:mid:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;?:@&=])*)(?:/(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[;?:@&=])*))?)|(?:vemmi://(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?)(?:/(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[/?:@&=])*)(?:(?:;(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[/?:@&])*)=(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[/?:@&])*))*))?)|(?:imap://(?:(?:(?:(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[&=~])+)(?:(?:;[Aa][Uu][Tt][Hh]=(?:\\*|(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[&=~])+))))?)|(?:(?:;[Aa][Uu][Tt][Hh]=(?:\\*|(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[&=~])+)))(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[&=~])+))?))@)?(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?))/(?:(?:(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[&=~:@/])+)?;[Tt][Yy][Pp][Ee]=(?:[Ll](?:[Ii][Ss][Tt]|[Ss][Uu][Bb])))|(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[&=~:@/])+)(?:\\?(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[&=~:@/])+))?(?:(?:;[Uu][Ii][Dd][Vv][Aa][Ll][Ii][Dd][Ii][Tt][Yy]=(?:[1-9]\\d*)))?)|(?:(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[&=~:@/])+)(?:(?:;[Uu][Ii][Dd][Vv][Aa][Ll][Ii][Dd][Ii][Tt][Yy]=(?:[1-9]\\d*)))?(?:/;[Uu][Ii][Dd]=(?:[1-9]\\d*))(?:(?:/;[Ss][Ee][Cc][Tt][Ii][Oo][Nn]=(?:(?:(?:[a-zA-Z\\d$\\-_.+!*'(),]|(?:%[a-fA-F\\d]{2}))|[&=~:@/])+)))?)))?)|(?:nfs:(?:(?://(?:(?:(?:(?:(?:[a-zA-Z\\d](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?)\\.)*(?:[a-zA-Z](?:(?:[a-zA-Z\\d]|-)*[a-zA-Z\\d])?))|(?:(?:\\d+)(?:\\.(?:\\d+)){3}))(?::(?:\\d+))?)(?:(?:/(?:(?:(?:(?:(?:[a-zA-Z\\d\\$\\-_.!~*'(),])|(?:%[a-fA-F\\d]{2})|[:@&=+])*)(?:/(?:(?:(?:[a-zA-Z\\d\\$\\-_.!~*'(),])|(?:%[a-fA-F\\d]{2})|[:@&=+])*))*)?)))?)|(?:/(?:(?:(?:(?:(?:[a-zA-Z\\d\\$\\-_.!~*'(),])|(?:%[a-fA-F\\d]{2})|[:@&=+])*)(?:/(?:(?:(?:[a-zA-Z\\d\\$\\-_.!~*'(),])|(?:%[a-fA-F\\d]{2})|[:@&=+])*))*)?))|(?:(?:(?:(?:(?:[a-zA-Z\\d\\$\\-_.!~*'(),])|(?:%[a-fA-F\\d]{2})|[:@&=+])*)(?:/(?:(?:(?:[a-zA-Z\\d\\$\\-_.!~*'(),])|(?:%[a-fA-F\\d]{2})|[:@&=+])*))*)?)))";
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

	//
	// формирование html
	//

	QString result;

	result += "<html>";
	result += "<head>";
	result += "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>";

	// подключение highlight.js
	QString path = QCoreApplication::applicationDirPath();

	#if QT_VERSION >= 0x040500
	result += "<style type='text/css'>pre { font-family: 'Courier New', courier; font-size: 11pt; }</style>";
	#endif

	#ifdef Q_WS_WIN
		result += "<link rel='stylesheet' title='Magula' href='file:///" + path + "/highlight/styles/magula.css'>";
		result += "<script src='file:///" + path + "/highlight/highlight.pack.js'></script>";
	#else
		result += "<link rel='stylesheet' title='Magula' href='file://" + path + "/highlight/styles/magula.css'>";
		result += "<script src='file://" + path + "/highlight/highlight.pack.js'></script>";
	#endif

	result += "<script>hljs.tabReplace = '    '; hljs.initHighlightingOnLoad();</script>";
	result += "<style type='text/css'>table { font-size: 11pt; }</style>";
	result += "</head>";

	// тело html
	result += "<body topmargin='0' leftmargin='0' rightmargin='0' bottommargin='0'>";
	result += "<table width='100%' cellpadding='1' cellspacing='0' border='0'>";
	result += "<tr style='background-color: #E6F2E6' valign='center'>";

	// тема сообщения + ссылка на сообщение на сайте
	result += "<td><a href='http://www.rsdn.ru/forum/message/" + QString::number(message.ID) + ".aspx'>" + QString::fromUtf8("<img src='qrc:/icons/show_topic.png' title='показать положение в теме' alt='показать положение в теме'>") + "</a><b>&nbsp;";

	if (message.ID != 0 && special == false)
		result += "<a style='color: black; text-decoration: none;' href='http://www.rsdn.ru/forum/message/" + QString::number(message.ID) + ".1.aspx'>" + subject + "</a>";
	else
		result += subject;

	result += "</b></td>";

	// иконки для выставления оценок
	result += "<td align='right'>";

	if (rated == true)
	{
		result += "<a href='avalon:rate_plus_1'><img src='qrc:/icons/rate_plus_1.png' title='+1' alt='+1'></a>";
		result += "&nbsp;&nbsp;";
		result += "<a href='avalon:rate_1'>" + QString::fromUtf8("<img src='qrc:/icons/rate_1.png' title='интересно' alt='интересно'>") + "</a>";
		result += "<a href='avalon:rate_2'>" + QString::fromUtf8("<img src='qrc:/icons/rate_2.png' title='спасибо' alt='спасибо'>") + "</a>";
		result += "<a href='avalon:rate_3'>" + QString::fromUtf8("<img src='qrc:/icons/rate_3.png' title='супер' alt='супер'>") + "</a>";
		result += "&nbsp;&nbsp;";
		result += "<a href='avalon:rate_cross'>" + QString::fromUtf8("<img src='qrc:/icons/rate_cross.png' title='удалить оценку' alt='удалить оценку'>") + "</a>";
		result += "&nbsp;&nbsp;";
		result += "<a href='avalon:rate_smile'>" + QString::fromUtf8("<img src='qrc:/icons/rate_smile.png' title='смешно' alt='смешно'>") + "</a>";
		result += "&nbsp;&nbsp;";
		result += "<a href='avalon:rate_plus'>" + QString::fromUtf8("<img src='qrc:/icons/rate_plus.png' title='согласен' alt='согласен'>") + "</a>";
		result += "<a href='avalon:rate_minus'>" + QString::fromUtf8("<img src='qrc:/icons/rate_minus.png' title='не согласен' alt='не согласен'>") + "</a>";
		result += "&nbsp;&nbsp;";
	}

	if (special == false)
		result += "<a href='avalon:moderate'>" + QString::fromUtf8("<img src='qrc:/icons/moderate.png' title='модерирование' alt='модерирование'>") + "</a>";

	result += "&nbsp;&nbsp;";
	result += "</td></tr>";

	// имя пользователя + ссылка на профиль
	if (message.IDUser == 0 || message.IDUser == -1)
	{
		if (message.UserNick.length() > 0)
			result += "<tr style='background-color: #FFFFF6'><td colspan='2'><b>&nbsp;" + QString::fromUtf8("От:") + "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</b>" + message.UserNick + "</td></tr>";
		else
			result += "<tr style='background-color: #FFFFF6'><td colspan='2'><b>&nbsp;" + QString::fromUtf8("От:") + "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</b>" + QString::fromUtf8("Аноним") + "</td></tr>";
	}
	else
		result += "<tr style='background-color: #FFFFF6'><td colspan='2'><b>&nbsp;" + QString::fromUtf8("От:") + "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</b><a href='http://www.rsdn.ru/Users/" + QString::number(message.IDUser) + ".aspx'>" + message.UserNick + "</a></td></tr>";

	result += "<tr style='background-color: #FFFFF6'><td colspan='2'><b>&nbsp;" + QString::fromUtf8("Дата:") + "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</b>" + message.MessageDate.toString("dd.MM.yyyy HH:mm") + "</td></tr>";

	// формирование строки рейтинга сообщения
	if (rating_list != NULL && rating_list->count() > 0)
	{
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
			result += "<tr style='background-color: #FFFFF6'><td colspan='2'><b>&nbsp;<a style='color: black; text-decoration: none;' href='http://www.rsdn.ru/forum/RateList.aspx?mid=" + QString::number(message.ID) + QString::fromUtf8("'>Оценки</a>:") + "&nbsp;&nbsp;</b>";

			if (rate_smile > 0)
				result += QString::fromUtf8("<img src='qrc:/icons/rate_smile.png' align='top' title='смешно' alt='смешно'>&nbsp;") + QString::number(rate_smile) + "&nbsp;&nbsp;";
			if (rate_plus > 0)
				result += QString::fromUtf8("<img src='qrc:/icons/rate_plus.png' align='top' title='согласен' alt='согласен'>&nbsp;") + QString::number(rate_plus) + "&nbsp;&nbsp;";
			if (rate_minus > 0)
				result += QString::fromUtf8("<img src='qrc:/icons/rate_minus.png' align='top' title='не согласен' alt='не согласен'>&nbsp;") + QString::number(rate_minus) + "&nbsp;&nbsp;";
			if (rate_plus_one > 0)
				result += QString::fromUtf8("<img src='qrc:/icons/rate_plus_1.png' align='top' title='+1' alt='+1'>&nbsp;") + QString::number(rate_plus_one) + "&nbsp;&nbsp;";
			if (rate_one > 0)
				result += QString::fromUtf8("<img src='qrc:/icons/rate_1.png' align='top' title='интересно' alt='интересно'>&nbsp;") + QString::number(rate_one) + "&nbsp;&nbsp;";
			if (rate_two > 0)
				result += QString::fromUtf8("<img src='qrc:/icons/rate_2.png' align='top' title='спасибо' alt='спасибо'>&nbsp;") + QString::number(rate_two) + "&nbsp;&nbsp;";
			if (rate_three > 0)
				result += QString::fromUtf8("<img src='qrc:/icons/rate_3.png' align='top' title='супер' alt='супер'>&nbsp;") + QString::number(rate_three) + "&nbsp;&nbsp;";

			result += "</td></tr>";
		}
	}

	result += "<tr><td colspan='2'><table width='99%' align='center'><tr><td><br />";

	// формирование тела сообщения
	for (int i = 0; i < list.count(); i++)
        result += formatParsedBlock(list.at(i));

	// хвост html
	result += "</td></tr></table></td></tr>";
	result += "<tr style='background-color: #E6F2E6'><td colspan='2'><b>&nbsp;</b></td></tr>";

	result += "</table>";
	result += "</body>";
	result += "</html>";

	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::formatParsedBlock (const AParsedBlock& block)
{
	QString result;

	if (block.Type == pbtText)
	{
		result += formatQuotedStringList(block.Strings, block.Type, block.SubType);
		result += "<br />";
	}

	else if (block.Type == pbtQuote)
	{
		result += "<table style='background-color: #FFFFE0;' width='98%' align='center'><tr><td>";
		result += formatQuotedStringList(block.Strings, block.Type, block.SubType);
		result += "</td></tr></table><br />";
	}

	else if (block.Type == pbtModerator)
	{
		result += "<table style='background-color: #FFC0C0;' width='100%'><tr><td>";
		result += formatQuotedStringList(block.Strings, block.Type, block.SubType);
		result += "</td></tr></table>";
	}

	else if (block.Type == pbtTagline)
	{
		result += "<font color='#A52A2A' size=-1>";
		result += formatQuotedStringList(block.Strings, block.Type, block.SubType);
		result += "</font><br />";
	}

	else if (block.Type == pbtTable)
	{
		result += "<table align='center'>";
		result += formatQuotedStringList(block.Strings, block.Type, block.SubType);
		result += "</table><br />";
	}

	else if (block.Type == pbtImage)
	{
		if (block.Strings.count() == 1)
			result += "<p align='center'><img src='" + block.Strings[0].Data + "'></p>";
		else
		{
			formatQuotedStringList(block.Strings, block.Type, block.SubType);
			result += "<br />";
		}
	}

	else
	{
		/*!
		 * Описатель соответствий языку и имени для подсветки
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

		result += "<table width='98%' align='center'><tr><td><pre>";

		if (code_class.length() > 0)
			result += "<code class='" + code_class + "'>";
		else
			result += "<code>";

		result += formatQuotedStringList(block.Strings, block.Type, block.SubType) + "</code>";
		result += "</pre></tr></td></table>";
	}

	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::formatQuotedStringList (const AQuotedStringList& list, AParsedBlockType type, AParsedBlockSubType sub_type)
{
	QString result;

	int last_quote_level = -1;

	for (int i = 0; i < list.count(); i++)
	{
		AQuotedString string = list.at(i);

		QString line = formatQuotedString(string, type, sub_type);

		if (sub_type != pbstSourceCode)
		{
			if (i != 0 && string.QuoteLevel != last_quote_level)
				// дополнительная пустая строка между квотами
				result += "<br>";

			if (type == pbtText && string.QuoteLevel == 0 && last_quote_level == 0)
				// дополнительная пустая строка в рамках нулевой квоты (разделение абзацев)
				result += "<br>";

			if (string.QuoteLevel != 0)
			{
				if (last_quote_level % 3 == 0)
					result += "<font color=darkblue>" + line + "</font>";
				else if (last_quote_level % 2 == 0)
					result += "<font color=darkred>" + line + "</font>";
				else
					result += "<font color=darkgreen>" + line + "</font>";
			}
			else
				result += line;

			result += "<br>";

			last_quote_level = string.QuoteLevel;
		}
		else
			result += line + "<br>";
	}

	return result;
}
//----------------------------------------------------------------------------------------------

QString AFormatter::formatQuotedString (const AQuotedString& string, AParsedBlockType /*type*/, AParsedBlockSubType sub_type)
{
	if (sub_type == pbstSourceCode)
		// строка исходного кода никак не форматируется далее
		return string.QuoteText + " " + string.Data;

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
		{ "[th]",      "<td align='center' style='background-color: #DCDCDC;'>" },
		{ "[/th]",     "</td>"     },
		{ "[hr]",      "<hr>"      },
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
		{ "[*]",       "<li />"    },
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

	QString data = string.Data;

	for (size_t i = 0; i < sizeof(replace_map) / sizeof(ASimpleTag); i++)
		data.replace(QString::fromUtf8(replace_map[i].Source), QString::fromUtf8(replace_map[i].Replace), Qt::CaseInsensitive);

	return string.QuoteText + " " + data;
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

int AFormatter::isURL (const QString& text)
{
	QRegExp url(g_url_regex, Qt::CaseInsensitive);

	if (url.exactMatch(text) == true)
		return 1;

	if (url.exactMatch("http://" + text) == true)
		return 2;

	return 0;
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
