#include "webservice.h"
//----------------------------------------------------------------------------------------------

AWebservice::AWebservice (QWidget* parent, IProgress* progress) : QObject (parent)
{
	m_progress = progress;

	QSettings settings;

	m_rsdn_login    = settings.value("rsdn/login",    "").toString();
	m_rsdn_password = settings.value("rsdn/password", "").toString();
	m_rsdn_proto    = settings.value("rsdn/proto",    "https").toString();
}
//----------------------------------------------------------------------------------------------

AWebservice::~AWebservice ()
{
}
//----------------------------------------------------------------------------------------------

bool AWebservice::getForumList (AForumGroupInfoList& list)
{
	QString data;
	data += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n";
	data += "<soap:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">\r\n";
	data += "  <soap:Body>\r\n";
	data += "    <GetForumList xmlns=\"http://rsdn.ru/Janus/\">\r\n";
	data += "      <forumRequest>\r\n";
	data += (QString)"        <userName>" + m_rsdn_login    + "</userName>\r\n";
	data += (QString)"        <password>" + m_rsdn_password + "</password>\r\n";
	data += "        <forumsRowVersion>AAAAAAAAAAA=</forumsRowVersion>\r\n";
	data += "      </forumRequest>\r\n";
	data += "    </GetForumList>\r\n";
	data += "  </soap:Body>\r\n";
	data += "</soap:Envelope>\r\n";

	QNetworkRequest request;
	prepareRequest(request, m_rsdn_proto, "GetForumList", data.toUtf8().size());

	if (makeRequest(request, data) == false)
		return false;

	parseForumList(m_body, list);

	return true;
}
//----------------------------------------------------------------------------------------------

bool AWebservice::getUserList (const QString& last_row_version, AUserInfoList& list, QString& row_version)
{
	QString data;
	data += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n";
	data += "<soap:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">\r\n";
	data += "  <soap:Body>\r\n";
	data += "    <GetNewUsers xmlns=\"http://rsdn.ru/Janus/\">\r\n";
	data += "      <userRequest>\r\n";
	data += (QString)"        <userName>"       + m_rsdn_login     + "</userName>\r\n";
	data += (QString)"        <password>"       + m_rsdn_password  + "</password>\r\n";
	data += (QString)"        <lastRowVersion>" + last_row_version + "</lastRowVersion>\r\n";
	data += "        <maxOutput>0</maxOutput>\r\n";
	data += "      </userRequest>\r\n";
	data += "    </GetNewUsers>\r\n";
	data += "  </soap:Body>\r\n";
	data += "</soap:Envelope>\r\n";

	QNetworkRequest request;
	prepareRequest(request, m_rsdn_proto, "GetNewUsers", data.toUtf8().size());

	if (makeRequest(request, data) == false)
		return false;

	QString result = parseUserList(m_body, list, row_version);
	if (result.length() > 0)
	{
		m_error = result;
		return false;
	}

	return true;
}
//----------------------------------------------------------------------------------------------

bool AWebservice::getMessageList (const ARowVersion& last_row_version, const ADataQuery& query, ADataList& list, ARowVersion& row_version)
{
	QString data;
	data += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n";
	data += "<soap:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">\r\n";
	data += "  <soap:Body>\r\n";
	data += "    <GetNewData xmlns=\"http://rsdn.ru/Janus/\">\r\n";
	data += "      <changeRequest>\r\n";
	data += (QString)"        <userName>" + m_rsdn_login    + "</userName>\r\n";
	data += (QString)"        <password>" + m_rsdn_password + "</password>\r\n";

	// подписанные форумы
	if (query.Forum.count() > 0)
	{
		data += "        <subscribedForums>\r\n";

		for (int i = 0; i < query.Forum.count(); i++)
		{
			data += "          <RequestForumInfo>\r\n";
			data += (QString)"            <forumId>"        + QString::number(query.Forum[i].IDForum)     + "</forumId>\r\n";
			data += (QString)"            <isFirstRequest>" + (query.Forum[i].IsFirst ? "true" : "false") + "</isFirstRequest>\r\n";
			data += "          </RequestForumInfo>\r\n";
		}

		data += "        </subscribedForums>\r\n";
	}
	else
		data += "        <subscribedForums />\r\n";

	// версии
	data += (QString)"        <ratingRowVersion>"   + last_row_version.Rating   + "</ratingRowVersion>\r\n";
	data += (QString)"        <messageRowVersion>"  + last_row_version.Message  + "</messageRowVersion>\r\n";
	data += (QString)"        <moderateRowVersion>" + last_row_version.Moderate + "</moderateRowVersion>\r\n";

	// ID оборванных сообщений, то есть сообщений без родителя.
	if (query.BrokenMessage.count() > 0)
	{
		data += "        <breakMsgIds>\r\n";

		for (int i = 0; i < query.BrokenMessage.count(); i++)
			data += (QString)"          <int>" + QString::number(query.BrokenMessage[i]) + "</int>\r\n";

		data += "        </breakMsgIds>\r\n";
	}
	else
		data += "        <breakMsgIds />\r\n";

	// ID топика не имеющего корневого сообщения или просто ID топика, который хочется выкачать целиком
	// topicId == messageId первого сообщения топика
	if (query.BrokenTopic.count() > 0)
	{
		data += "        <breakTopicIds>\r\n";

		for (int i = 0; i < query.BrokenTopic.count(); i++)
			data += (QString)"          <int>" + QString::number(query.BrokenTopic[i]) + "</int>\r\n";

		data += "        </breakTopicIds>\r\n";
	}
	else
		data += "        <breakTopicIds />\r\n";

	data += "        <maxOutput>0</maxOutput>\r\n";
	data += "      </changeRequest>\r\n";
	data += "    </GetNewData>\r\n";
	data += "  </soap:Body>\r\n";
	data += "</soap:Envelope>\r\n";

	QNetworkRequest request;
	prepareRequest(request, m_rsdn_proto, "GetNewData", data.toUtf8().size());

	if (makeRequest(request, data) == false)
		return false;

	QString result = parseMessageList(m_body, list, row_version);
	if (result.length() > 0)
	{
		m_error = result;
		return false;
	}

	return true;
}
//----------------------------------------------------------------------------------------------

bool AWebservice::postChange (const AMessage2SendList& list_messages, const ARating2SendList& list_rating, const AModerate2SendList& list_moderate, ACommitInfo& commit_info)
{
	QString data;
	data += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n";
	data += "<soap:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">\r\n";
	data += "  <soap:Body>\r\n";
	data += "    <PostChange xmlns=\"http://rsdn.ru/Janus/\">\r\n";
	data += "      <postRequest>\r\n";
	data += (QString)"        <userName>" + m_rsdn_login    + "</userName>\r\n";
	data += (QString)"        <password>" + m_rsdn_password + "</password>\r\n";

	// сообщения
	if (list_messages.count() > 0)
	{
		data += "        <writedMessages>\r\n";

		for (int i = 0; i < list_messages.count(); i++)
		{
			QString message = list_messages[i].Message;

			QString subject = list_messages[i].Subject;

			subject.replace("&", "&amp;");
			subject.replace("<", "&lt;");
			subject.replace(">", "&gt;");

			data += "          <PostMessageInfo>\r\n";
			data += (QString)"            <localMessageId>" + QString::number(list_messages[i].ID)       + "</localMessageId>\r\n";
			data += (QString)"            <parentId>"       + QString::number(list_messages[i].IDParent) + "</parentId>\r\n";
			data += (QString)"            <forumId>"        + QString::number(list_messages[i].IDForum)  + "</forumId>\r\n";
			data += (QString)"            <subject>"        + subject                                    + "</subject>\r\n";
			data += (QString)"            <message>"        + message                                    + "</message>\r\n";
			data += "          </PostMessageInfo>\r\n";
		}

		data += "        </writedMessages>\r\n";
	}
	else
		data += "        <writedMessages />\r\n";

	// рейтинги
	if (list_rating.count() > 0)
	{
		data += "        <rates>\r\n";

		for (int i = 0; i < list_rating.count(); i++)
		{
			data += "          <PostRatingInfo>\r\n";
			data += (QString)"            <localRatingId>" + QString::number(list_rating[i].ID)        + "</localRatingId>\r\n";
			data += (QString)"            <messageId>"     + QString::number(list_rating[i].IDMessage) + "</messageId>\r\n";
			data += (QString)"            <rate>"          + QString::number(list_rating[i].Rate)      + "</rate>\r\n";
			data += "          </PostRatingInfo>\r\n";
		}

		data += "        </rates>\r\n";
	}
	else
		data += "        <rates />\r\n";

	// модерилки
	if (list_moderate.count() > 0)
	{
		data += "        <moderates>\r\n";

		for (int i = 0; i < list_moderate.count(); i++)
		{
			QString description = list_moderate[i].Description;

			description.replace("&", "&amp;");
			description.replace("<", "&lt;");
			description.replace(">", "&gt;");

			data += "          <PostModerateInfo>\r\n";
			data += (QString)"            <LocalModerateId>"   + QString::number(list_moderate[i].ID)          + "</LocalModerateId>\r\n";
			data += (QString)"            <MessageId>"         + QString::number(list_moderate[i].IDMessage)   + "</MessageId>\r\n";
			data += (QString)"            <ModerateAction>"    + list_moderate[i].Action                       + "</ModerateAction>\r\n";
			data += (QString)"            <ModerateToForumId>" + QString::number(list_moderate[i].IDForum)     + "</ModerateToForumId>\r\n";
			data += (QString)"            <Description>"       + description                                   + "</Description>\r\n";
			data += (QString)"            <AsModerator>"       + QString::number(list_moderate[i].AsModerator) + "</AsModerator>\r\n";
			data += "          </PostModerateInfo>\r\n";
		}

		data += "        </moderates>\r\n";
	}
	else
		data += "        <moderates />\r\n";

	data += "      </postRequest>\r\n";
	data += "    </PostChange>\r\n";
	data += "  </soap:Body>\r\n";
	data += "</soap:Envelope>\r\n";

	QNetworkRequest request;
	prepareRequest(request, m_rsdn_proto, "PostChange", data.toUtf8().size());

	if (makeRequest(request, data) == false)
		return false;

	QString cookie;
	QString result = parsePostChange(m_headers, cookie);

	if (result.length() > 0)
	{
		m_error = result;
		return false;
	}

	data  = "";
	data += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n";
	data += "<soap:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">\r\n";
	data += "  <soap:Body>\r\n";
	data += "    <PostChangeCommit xmlns=\"http://rsdn.ru/Janus/\" />\r\n";
	data += "  </soap:Body>\r\n";
	data += "</soap:Envelope>\r\n";

	prepareRequest(request, m_rsdn_proto, "PostChangeCommit", data.toUtf8().size());

	if (cookie.length() > 0)
		request.setRawHeader("Cookie", cookie.toUtf8());

	if (makeRequest(request, data) == false)
		return false;

	result = parsePostChangeCommit(m_body, commit_info);

	if (result.length() > 0)
	{
		m_error = result;
		return false;
	}

	return true;
}
//----------------------------------------------------------------------------------------------

QNetworkProxy AWebservice::defaultProxy (bool webkit)
{
	QSettings settings;

	QNetworkProxy proxy;

	if (settings.value("proxy/enabled", false).toInt() != false)
	{
		if (webkit == true && settings.value("proxy/enabled_webkit", true).toInt() == false)
			return proxy;

		QString proxy_type = settings.value("proxy/type", "HTTP").toString();

		if (proxy_type == "HTTP")
			proxy.setType(QNetworkProxy::HttpCachingProxy);
		else if (proxy_type == "SOCKS 5")
			proxy.setType(QNetworkProxy::Socks5Proxy);
		else if (proxy_type == "CONNECT")
			proxy.setType(QNetworkProxy::HttpProxy);
		else
			proxy.setType(QNetworkProxy::DefaultProxy);

		if (proxy.type() != QNetworkProxy::DefaultProxy)
		{
			proxy.setHostName (settings.value("proxy/host",     "").toString());
			proxy.setPort     (settings.value("proxy/port",     0 ).toInt());
			proxy.setUser     (settings.value("proxy/login",    "").toString());
			proxy.setPassword (settings.value("proxy/password", "").toString());
		}
	}

	return proxy;
}
//----------------------------------------------------------------------------------------------

void AWebservice::parseForumList (const QString& data, AForumGroupInfoList& list)
{
	list.clear();

	//
	// список групп форумов
	//

	AGroupInfoList group_list;

	int seed = 0;

	QString group_info = getNextBlock(&data, "<JanusForumGroupInfo>", "</JanusForumGroupInfo>", seed);

	while (group_info.length())
	{
		AGroupInfo info;

		info.ID        = getTextBetween(&group_info, "<forumGroupId>",   "</forumGroupId>").toInt();
		info.Name      = getTextBetween(&group_info, "<forumGroupName>", "</forumGroupName>");
		info.SortOrder = getTextBetween(&group_info, "<sortOrder>",      "</sortOrder>").toInt();

		group_list.append(info);

		group_info = getNextBlock(&data, "<JanusForumGroupInfo>", "</JanusForumGroupInfo>", seed);
	}

	//
	// список форумов
	//

	AForumInfoList forum_list;

	seed = 0;

	QString forum_info = getNextBlock(&data, "<JanusForumInfo>", "</JanusForumInfo>", seed);

	while (forum_info.length())
	{
		AForumInfo info;

		info.ID        = getTextBetween(&forum_info, "<forumId>",        "</forumId>").toInt();
		info.IDGroup   = getTextBetween(&forum_info, "<forumGroupId>",   "</forumGroupId>").toInt();
		info.ShortName = getTextBetween(&forum_info, "<shortForumName>", "</shortForumName>");
		info.Name      = getTextBetween(&forum_info, "<forumName>",      "</forumName>");
		info.Rated     = getTextBetween(&forum_info, "<rated>",          "</rated>").toInt();
		info.InTop     = getTextBetween(&forum_info, "<inTop>",          "</inTop>").toInt();
		info.RateLimit = getTextBetween(&forum_info, "<rateLimit>",      "</rateLimit>").toInt();

		forum_list.append(info);

		forum_info = getNextBlock(&data, "<JanusForumInfo>", "</JanusForumInfo>", seed);
	}

	//
	// заполнение результата
	//

	for (int group_index = 0; group_index < group_list.count(); group_index++)
	{
		AForumGroupInfo group_info;

		group_info.Group = group_list[group_index];

		for (int forum_index = 0; forum_index < forum_list.count(); forum_index++)
			if (forum_list[forum_index].IDGroup == group_info.Group.ID)
				group_info.Forums.append(forum_list[forum_index]);

		list.append(group_info);
	}
}
//----------------------------------------------------------------------------------------------

QString AWebservice::parseUserList (const QString& data, AUserInfoList& list, QString& row_version)
{
	list.clear();

	row_version = "";

	int seed = 0;

	QString user_info = getNextBlock(&data, "<JanusUserInfo>", "</JanusUserInfo>", seed);

	while (user_info.length())
	{
		AUserInfo info;

		info.ID             = getTextBetween(&user_info, "<userId>",         "</userId>").toInt();
		info.Name           = getTextBetween(&user_info, "<userName>",       "</userName>");
		info.Nick           = getTextBetween(&user_info, "<userNick>",       "</userNick>");
		info.RealName       = getTextBetween(&user_info, "<realName>",       "</realName>");
		info.Email          = getTextBetween(&user_info, "<publicEmail>",    "</publicEmail>");
		info.Homepage       = getTextBetween(&user_info, "<homePage>",       "</homePage>");
		info.Specialization = getTextBetween(&user_info, "<specialization>", "</specialization>");
		info.WhereFrom      = getTextBetween(&user_info, "<whereFrom>",      "</whereFrom>");
		info.Origin         = getTextBetween(&user_info, "<origin>",         "</origin>");

		list.append(info);

		user_info = getNextBlock(&data, "<JanusUserInfo>", "</JanusUserInfo>", seed);
	}

	seed = 0;
	row_version = getNextBlock(&data, "<lastRowVersion>", "</lastRowVersion>", seed);

	// проверка ошибок получения версий строк
	// подавлять ошибки сбросом в значение по умолчанию ("AAAAAAAAAAA=") оказалось неправильно
	// подробнее см. https://rsdn.ru/forum/janus/3449147.1
	if (row_version.length() == 0)
		return QString::fromUtf8("В ответе вебсервиса не найдено поле <lastRowVersion>");

	return "";
}
//----------------------------------------------------------------------------------------------

QString AWebservice::parseMessageList (const QString& data, ADataList& list, ARowVersion& row_version)
{
	list.Rating.clear();
	list.Message.clear();
	list.Moderate.clear();

	int seed = 0;

	QString message_info = getNextBlock(&data, "<JanusMessageInfo>", "</JanusMessageInfo>", seed);

	while (message_info.length())
	{
		AMessageInfo info;

		info.ID             = getTextBetween(&message_info, "<messageId>",      "</messageId>").toInt();
		info.IDTopic        = getTextBetween(&message_info, "<topicId>",        "</topicId>").toInt();
		info.IDParent       = getTextBetween(&message_info, "<parentId>",       "</parentId>").toInt();
		info.IDUser         = getTextBetween(&message_info, "<userId>",         "</userId>").toInt();
		info.IDForum        = getTextBetween(&message_info, "<forumId>",        "</forumId>").toInt();
		info.Subject        = getTextBetween(&message_info, "<subject>",        "</subject>");
		info.MessageName    = getTextBetween(&message_info, "<messageName>",    "</messageName>");
		info.UserNick       = getTextBetween(&message_info, "<userNick>",       "</userNick>");
		info.Message        = getTextBetween(&message_info, "<message>",        "</message>");
		info.IDArticle      = getTextBetween(&message_info, "<articleId>",      "</articleId>").toInt();
		info.MessageDate    = getDateTimeFromString(getTextBetween(&message_info, "<messageDate>", "</messageDate>"));
		info.UpdateDate     = getDateTimeFromString(getTextBetween(&message_info, "<updateDate>", "</updateDate>"));
		info.UserRole       = getTextBetween(&message_info, "<userRole>",       "</userRole>");
		info.UserTitle      = getTextBetween(&message_info, "<userTitle>",      "</userTitle>");
		info.UserTitleColor = getTextBetween(&message_info, "<userTitleColor>", "</userTitleColor>").toInt();
		info.LastModerated  = getDateTimeFromString(getTextBetween(&message_info, "<lastModerated>", "</lastModerated>"));
		info.Closed         = getBooleanFromString(getTextBetween(&message_info, "<closed>", "</closed>"));

		if (info.MessageDate.isValid() == false)
			return QString::fromUtf8("Некорректное значение даты <messageDate>: ") + getTextBetween(&message_info, "<messageDate>", "</messageDate>");

		if (info.UpdateDate.isValid() == false)
			return QString::fromUtf8("Некорректное значение даты <updateDate>: ") + getTextBetween(&message_info, "<updateDate>", "</updateDate>");

		if (info.LastModerated.isValid() == false)
			return QString::fromUtf8("Некорректное значение даты <lastModerated>: ") + getTextBetween(&message_info, "<lastModerated>", "</lastModerated>");

		list.Message.append(info);

		message_info = getNextBlock(&data, "<JanusMessageInfo>", "</JanusMessageInfo>", seed);
	}

	seed = 0;

	QString rating_info = getNextBlock(&data, "<JanusRatingInfo>", "</JanusRatingInfo>", seed);

	while (rating_info.length())
	{
		ARatingInfo info;

		info.IDMessage  = getTextBetween(&rating_info, "<messageId>",  "</messageId>").toInt();
		info.IDTopic    = getTextBetween(&rating_info, "<topicId>",    "</topicId>").toInt();
		info.IDUser     = getTextBetween(&rating_info, "<userId>",     "</userId>").toInt();
		info.UserRating = getTextBetween(&rating_info, "<userRating>", "</userRating>").toInt();
		info.Rate       = getTextBetween(&rating_info, "<rate>",       "</rate>").toInt();
		info.RateDate   = getDateTimeFromString(getTextBetween(&rating_info, "<rateDate>", "</rateDate>"));

		if (info.RateDate.isValid() == false)
			return QString::fromUtf8("Некорректное значение даты <rateDate>: ") + getTextBetween(&rating_info, "<rateDate>", "</rateDate>");

		list.Rating.append(info);

		rating_info = getNextBlock(&data, "<JanusRatingInfo>", "</JanusRatingInfo>", seed);
	}

	seed = 0;

	QString moderate_info = getNextBlock(&data, "<JanusModerateInfo>", "</JanusModerateInfo>", seed);

	while (moderate_info.length())
	{
		AModerateInfo info;

		info.IDMessage = getTextBetween(&moderate_info, "<messageId>", "</messageId>").toInt();
		info.IDTopic   = getTextBetween(&moderate_info, "<topicId>",   "</topicId>").toInt();
		info.IDUser    = getTextBetween(&moderate_info, "<userId>",    "</userId>").toInt();
		info.IDForum   = getTextBetween(&moderate_info, "<forumId>",   "</forumId>").toInt();
		info.Created   = getDateTimeFromString(getTextBetween(&moderate_info, "<create>", "</create>"));

		if (info.Created.isValid() == false)
			return QString::fromUtf8("Некорректное значение даты <create>: ") + getTextBetween(&moderate_info, "<create>", "</create>");

		list.Moderate.append(info);

		moderate_info = getNextBlock(&data, "<JanusModerateInfo>", "</JanusModerateInfo>", seed);
	}

	seed = 0;
        QString lastRatingRowVersion   = getNextBlock(&data, "<lastRatingRowVersion>",   "</lastRatingRowVersion>",   seed);
	seed = 0;
        QString lastForumRowVersion    = getNextBlock(&data, "<lastForumRowVersion>",    "</lastForumRowVersion>",    seed);
	seed = 0;
        QString lastModerateRowVersion = getNextBlock(&data, "<lastModerateRowVersion>", "</lastModerateRowVersion>", seed);

	// проверка ошибок получения версий строк
	// подавлять ошибки сбросом в значение по умолчанию ("AAAAAAAAAAA=") оказалось неправильно
	// подробнее см. https://rsdn.ru/forum/janus/3449147.1
	// выбрасывать ошибку (r253) тоже оказалось неправильно - для некоторых форумов не приходят ни рейтинги ни модерилки
	if (lastRatingRowVersion.length() != 0)
		row_version.Rating = lastRatingRowVersion;
	if (lastForumRowVersion.length() != 0)
		row_version.Message = lastForumRowVersion;
	if (lastModerateRowVersion.length() != 0)
		row_version.Moderate = lastModerateRowVersion;

	return "";
}
//----------------------------------------------------------------------------------------------

QString AWebservice::parsePostChange (const QString& header, QString& cookie)
{
	QStringList list = header.split("\r\n");

	for (int i = 0; i < list.count(); i++)
		if (list[i].indexOf("Set-Cookie: ") == 0)
		{
			QString cookie_string = list[i];

			cookie_string.replace("Set-Cookie: ", "");

			QStringList cookie_list = cookie_string.split(";");

			if (cookie_list.count() > 1)
			{
				QStringList key_value = cookie_list[0].split("=");

				if (key_value.count() != 2)
					continue;

				QString key   = key_value[0];
				QString value = key_value[1];

				cookie += key + "=" + value + "; ";
			}
		}

	// убираем "; "
	if (cookie.length() > 0)
		cookie.chop(2);

	return "";
}
//----------------------------------------------------------------------------------------------

QString AWebservice::parsePostChangeCommit (const QString& data, ACommitInfo& commit_info)
{
	int seed = 0;

	QString messages = getTextBetween(&data, "<commitedIds>", "</commitedIds>");

	QString id_info = getNextBlock(&messages, "<int>", "</int>", seed);

	while (id_info.length())
	{
		commit_info.Messages.append(id_info.toInt());

		id_info = getNextBlock(&messages, "<int>", "</int>", seed);
	}

	seed = 0;

	QString message_exceptions = getTextBetween(&data, "<exceptions>", "</exceptions>");

	QString message_exception_info = getNextBlock(&message_exceptions, "<PostExceptionInfo>", "</PostExceptionInfo>", seed);

	while (message_exception_info.length())
	{
		ACommitExceptionInfo info;

		info.Exception = getTextBetween(&message_exception_info, "<exception>",      "</exception>");
		info.ID        = getTextBetween(&message_exception_info, "<localMessageId>", "</localMessageId>").toInt();
		info.Info      = getTextBetween(&message_exception_info, "<info>",           "</info>");

		commit_info.MessagesExceptions.append(info);

		message_exception_info = getNextBlock(&message_exceptions, "<PostExceptionInfo>", "</PostExceptionInfo>", seed);
	}

	seed = 0;

	QString ratings = getTextBetween(&data, "<commitedRatingIds>", "</commitedRatingIds>");

	id_info = getNextBlock(&ratings, "<int>", "</int>", seed);

	while (id_info.length())
	{
		commit_info.Rating.append(id_info.toInt());

		id_info = getNextBlock(&ratings, "<int>", "</int>", seed);
	}

	seed = 0;

	QString rating_exceptions = getTextBetween(&data, "<ratingExceptions>", "</ratingExceptions>");

	QString rating_exception_info = getNextBlock(&rating_exceptions, "<RatingExceptionInfo>", "</RatingExceptionInfo>", seed);

	while (rating_exception_info.length())
	{
		ACommitExceptionInfo info;

		info.Exception = getTextBetween(&rating_exception_info, "<exception>",     "</exception>");
		info.ID        = getTextBetween(&rating_exception_info, "<localRatingId>", "</localRatingId>").toInt();
		info.Info      = getTextBetween(&rating_exception_info, "<info>",          "</info>");

		commit_info.RatingExceptions.append(info);

		rating_exception_info = getNextBlock(&rating_exceptions, "<RatingExceptionInfo>", "</RatingExceptionInfo>", seed);
	}

	seed = 0;

	QString moderates = getTextBetween(&data, "<commitedModerateIds>", "</commitedModerateIds>");

	id_info = getNextBlock(&moderates, "<int>", "</int>", seed);

	while (id_info.length())
	{
		commit_info.Moderate.append(id_info.toInt());

		id_info = getNextBlock(&moderates, "<int>", "</int>", seed);
	}

	seed = 0;

	QString moderate_exceptions = getTextBetween(&data, "<moderateExceptions>", "</moderateExceptions>");

	QString moderate_exception_info = getNextBlock(&moderate_exceptions, "<ModerateExceptionInfo>", "</ModerateExceptionInfo>", seed);

	while (moderate_exception_info.length())
	{
		ACommitExceptionInfo info;

		info.Exception = getTextBetween(&moderate_exception_info, "<ExceptionMessage>", "</ExceptionMessage>");
		info.ID        = getTextBetween(&moderate_exception_info, "<LocalModerateId>",  "</LocalModerateId>").toInt();
		info.Info      = getTextBetween(&moderate_exception_info, "<Info>",             "</Info>");

		commit_info.MessagesExceptions.append(info);

		moderate_exception_info = getNextBlock(&moderate_exceptions, "<ModerateExceptionInfo>", "</ModerateExceptionInfo>", seed);
	}

	return "";
}
//----------------------------------------------------------------------------------------------

QString AWebservice::getTextBetween (const QString* source, const QString& from, const QString& to)
{
	int idx1 = source->indexOf(from);

	if (idx1 == -1)
		return "";

	int idx2 = source->indexOf(to, idx1 + 1);

	if (idx2 == -1)
		return "";

	return source->mid(idx1 + from.length(), idx2 - idx1 - from.length());
}
//----------------------------------------------------------------------------------------------

QString AWebservice::getNextBlock (const QString* source, const QString& from, const QString& to, int& seed)
{
	int idx1 = source->indexOf(from, seed);

	if (idx1 == -1)
		return "";

	int idx2 = source->indexOf(to, idx1 + 1);

	if (idx2 == -1)
		return "";

	QString result = source->mid(idx1 + from.length(), idx2 - idx1 - from.length());

	seed = idx2 + to.length();

	return result;
}
//----------------------------------------------------------------------------------------------

QDateTime AWebservice::getDateTimeFromString (const QString& value)
{
	if (value == "0001-01-01T00:00:00")
		return QDateTime::fromString("1970-01-01T00:00:00", Qt::ISODate);

	return QDateTime::fromString(value, Qt::ISODate);
}
//----------------------------------------------------------------------------------------------

bool AWebservice::getBooleanFromString (const QString& value)
{
	QString tmp = value.toLower();

	if (tmp == "0" || tmp == "false" || tmp == "no" || tmp == "f" || tmp == "n")
		return false;

	return true;
}
//----------------------------------------------------------------------------------------------

QString AWebservice::formatPrettyBytes (qint64 size)
{
	char buf[64];

	if (size < (qint64)1024)
	{
		sprintf(buf, "%0dB", (int)size);
		return QString::fromUtf8(buf);
	}
	else if (size < (qint64)1024 * 1024)
	{
		sprintf(buf, "%0.2fKB", (double)size / 1024);
		return QString::fromUtf8(buf);
	}
	else if (size < (qint64)1024 * 1024 * 1024)
	{
		sprintf(buf, "%0.2fMB", (double)size / 1024 / 1024);
		return QString::fromUtf8(buf);
	}
	else
	{
		sprintf(buf, "%0.2fGB", (double)size / 1024 / 1024 / 1024);
		return QString::fromUtf8(buf);
	}
}
//----------------------------------------------------------------------------------------------

void AWebservice::prepareRequest (QNetworkRequest& request, const QString& proto, const QString& action, qint64 length)
{
	request.setUrl(proto.toLower() + "://rsdn.ru/ws/janusAT.asmx");

	request.setHeader(QNetworkRequest::CookieHeader, QVariant());

	request.setRawHeader("Host",           "rsdn.ru");
	request.setRawHeader("Connection",     "close");
	request.setRawHeader("User-Agent",     getAgentString().toUtf8());
	request.setRawHeader("Content-Type",   "text/xml; charset=utf-8");
	request.setRawHeader("Content-Length", QString::number(length).toUtf8());
	request.setRawHeader("SOAPAction",     (QString("\"http://rsdn.ru/Janus/") + action + "\"").toUtf8());

#ifdef AVALON_USE_ZLIB
	request.setRawHeader("Accept-Encoding", "gzip");
#endif
}
//----------------------------------------------------------------------------------------------

bool AWebservice::makeRequest(const QNetworkRequest& request, const QString& data)
{
	QNetworkAccessManager http;

	http.setProxy(defaultProxy());

	QNetworkReply* reply = NULL;
	if (data.length() == 0)
		reply = http.get(request);
	else
		reply = http.post(request, data.toUtf8());

	QObject::connect(reply, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(process_ssl_errors(const QList<QSslError>&)));
	QObject::connect(reply, SIGNAL(downloadProgress(qint64, qint64)),   this, SLOT(process_download_progress(qint64, qint64)));
	QObject::connect(reply, SIGNAL(uploadProgress(qint64, qint64)),     this, SLOT(process_upload_progress(qint64, qint64)));

	QEventLoop loop;
	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

	bool result = parseReply(reply);

	delete reply;

	return result;
}
//----------------------------------------------------------------------------------------------

bool AWebservice::parseReply (QNetworkReply* reply)
{
	m_error = "";
	if (reply->error() != QNetworkReply::NoError)
	{
		m_error = reply->errorString();
		return false;
	}

	m_headers = "";
	const QList<QNetworkReply::RawHeaderPair> pairs = reply->rawHeaderPairs();
	for (int i = 0; i < pairs.count(); i++)
	{
		QNetworkReply::RawHeaderPair pair = pairs[i];
		QString     header = QString::fromUtf8(pair.first.constData());
		QStringList values = QString::fromUtf8(pair.second.constData()).split("\n");
		for (int j = 0; j < values.count(); j++)
			m_headers += header + ": " + values[j] + "\r\n";
	}

#ifndef AVALON_USE_ZLIB
	m_body = QString::fromUtf8(reply->readAll());
#else
	if (reply->hasRawHeader("Content-Encoding") == true)
	{
		QString content_encoding = QString::fromUtf8(reply->rawHeader("Content-Encoding")).toLower();
		if (content_encoding != "gzip")
		{
			m_body = QString::fromUtf8(reply->readAll());
			return true;
		}

		// ответ пришел закодированным в gzip
		QByteArray array = reply->readAll();

		QTemporaryFile tmp_file;
		if (tmp_file.open() == false)
		{
			m_error = tmp_file.errorString();
			return false;
		}
		if (tmp_file.write(array) == -1)
		{
			m_error = tmp_file.errorString();
			return false;
		}

		QString tmp_file_name = tmp_file.fileName();
		tmp_file.close();
		array.clear();

		gzFile zip_file = gzopen(tmp_file_name.toUtf8().constData(), "rb");
		if (zip_file == NULL)
		{
			// TODO: if errno is zero, the zlib error is Z_MEM_ERROR
			m_error = QString::fromUtf8("Ошибка открытия файла: ") + tmp_file_name;
			return false;
		}

		char buffer[1024];
		while (true)
		{
			int read = gzread(zip_file, buffer, 1024);
			if (read == -1)
			{
				// TODO: gzerror(gzFile file, int *errnum);
				gzclose(zip_file);
				m_error = QString::fromUtf8("Ошибка чтения файла: ") + tmp_file_name;
				return false;
			}
			else if (read == 0)
				break;

			array.append(QByteArray(buffer, read));
		}

		gzclose(zip_file);

		m_body = QString::fromUtf8(array.constData());
	}
	else
		m_body = QString::fromUtf8(reply->readAll());
#endif   // AVALON_USE_ZLIB

	return true;
}
//----------------------------------------------------------------------------------------------

void AWebservice::process_ssl_errors (const QList<QSslError> &errors)
{
	for (int i = 0; i < errors.count(); i++)
		m_error += errors[i].errorString() + "\n";
}
//----------------------------------------------------------------------------------------------

void AWebservice::process_download_progress (qint64 done, qint64 total)
{
	if (m_progress != NULL)
	{
		// при включенном сжатии, размер данных неопределен
		if (total > 0)
			m_progress->onProgress(0, total, done, QString::fromUtf8("чтение ") + formatPrettyBytes(done) + "/" + formatPrettyBytes(total));
		else
			m_progress->onProgress(0, 0, done, QString::fromUtf8("чтение ") + formatPrettyBytes(done));
	}
}
//----------------------------------------------------------------------------------------------

void AWebservice::process_upload_progress (qint64 done, qint64 total)
{
	if (m_progress != NULL)
		m_progress->onProgress(0, total, done, QString::fromUtf8("отправка ") + formatPrettyBytes(done) + "/" + formatPrettyBytes(total));
}
//----------------------------------------------------------------------------------------------
