//----------------------------------------------------------------------------------------------
// $Date: 2011-04-20 17:27:00 +0400 (Срд, 20 Апр 2011) $
// $Author: antonbatenev.ya.ru $
// $Revision: 419 $
// $URL: svn://opensvn.ru/avalon/trunk/storage/mysql_database.cpp $
//----------------------------------------------------------------------------------------------
#include "mysql_database.h"
//----------------------------------------------------------------------------------------------

AMySQLDatabase::AMySQLDatabase () : ADatabase
(
#ifdef Q_WS_WIN
	"QODBC"
#else
	"QMYSQL"
#endif
)
{
	m_transaction = false;
}
//----------------------------------------------------------------------------------------------

AMySQLDatabase::~AMySQLDatabase ()
{
}
//----------------------------------------------------------------------------------------------

bool AMySQLDatabase::checkDatabase ()
{
	if (ADatabase::isOpen() == true)
		return true;

	return openDatabase();
}
//----------------------------------------------------------------------------------------------

bool AMySQLDatabase::openDatabase ()
{
	if (ADatabase::isOpen() == true)
		return true;

	// получение настроек соединения из конфига
	QSettings settings;

	QString database_host     = settings.value("mysql/host"    ).toString();
	QString database_port     = settings.value("mysql/port"    ).toString();
	QString database_name     = settings.value("mysql/name"    ).toString();
	QString database_login    = settings.value("mysql/login"   ).toString();
	QString database_password = settings.value("mysql/password").toString();

	bool is_port_number    = false;
	int  database_port_int = database_port.toInt(&is_port_number);

	#ifdef Q_WS_WIN
		// http://dev.mysql.com/doc/refman/5.1/en/connector-odbc-configuration-connection-parameters.html
		QString connection_string;

		connection_string += "driver={MySQL ODBC 5.1 Driver};";
		connection_string += "charset=UTF8;";
		connection_string += "option=" + QString(8 /* FLAG_BIG_PACKETS */ + 2048 /* FLAG_COMPRESSED_PROTO */) + ";";
		connection_string += "server="   + database_host     + ";";
		connection_string += "database=" + database_name     + ";";
		connection_string += "user="     + database_login    + ";";
		connection_string += "password=" + database_password + ";";

		if (is_port_number == true)
		{
			if (database_port_int != 0)
				connection_string += "port=" + QString(database_port_int) + ";";
		}
		else if (database_port.length() != 0)
			connection_string += "socket=" + database_port + ";";

		ADatabase::setDatabaseName(connection_string);
	#else
		ADatabase::setHostName(database_host);
		ADatabase::setDatabaseName(database_name);
		ADatabase::setUserName(database_login);
		ADatabase::setPassword(database_password);

		if (is_port_number == true)
		{
			if (database_port_int != 0)
				ADatabase::setPort(database_port_int);

			ADatabase::setConnectOptions("CLIENT_COMPRESS=1");
		}
		else if (database_port.length() != 0)
			ADatabase::setConnectOptions("UNIX_SOCKET=" + database_port);
	#endif

	m_transaction = false;

	if (ADatabase::open() == false)
	{
		setLastError();
		return false;
	}

	return true;
}
//----------------------------------------------------------------------------------------------

bool AMySQLDatabase::transaction ()
{
	if (m_transaction == true || checkDatabase() == false)
		return false;

	if (ADatabase::transaction() == false)
	{
		setLastError();
		return false;
	}

	m_transaction = true;

	return true;
}
//----------------------------------------------------------------------------------------------

bool AMySQLDatabase::commit ()
{
	if (ADatabase::isOpen() == false || m_transaction == false)
		return false;

	if (ADatabase::commit() == false)
	{
		setLastError();

		ADatabase::rollback();

		m_transaction = false;

		return false;
	}

	m_transaction = false;

	return true;
}
//----------------------------------------------------------------------------------------------

bool AMySQLDatabase::rollback ()
{
	if (ADatabase::isOpen() == false || m_transaction == false)
		return false;

	if (ADatabase::rollback() == false)
		setLastError();

	m_transaction = false;

	return true;
}
//----------------------------------------------------------------------------------------------

AQuery* AMySQLDatabase::createQuery (const QString& sql, bool prepared)
{
	if (checkDatabase() == false)
		return false;

	AQuery* query = 0;

	if (prepared == true)
		query = ADatabase::createPreparedQuery(sql);
	else
		query = ADatabase::createQuery(sql);

	if (query == NULL)
	{
		if (m_transaction == true)
			rollback();
	}

	return query;
}
//----------------------------------------------------------------------------------------------
