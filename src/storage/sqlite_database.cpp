#include "sqlite_database.h"
//----------------------------------------------------------------------------------------------

ASQLiteDatabase::ASQLiteDatabase () : ADatabase ("QSQLITE")
{
	m_transaction = false;
}
//----------------------------------------------------------------------------------------------

ASQLiteDatabase::~ASQLiteDatabase ()
{
}
//----------------------------------------------------------------------------------------------

bool ASQLiteDatabase::checkDatabase ()
{
	if (ADatabase::isOpen() == true)
		return true;

	return openDatabase();
}
//----------------------------------------------------------------------------------------------

bool ASQLiteDatabase::openDatabase ()
{
	if (ADatabase::isOpen() == true)
		return true;

	// получение настроек соединения из конфига
	QSettings settings;

	QString database_file = settings.value("sqlite/file").toString();

	ADatabase::setDatabaseName(database_file);

	m_transaction = false;

	if (ADatabase::open() == false)
	{
		setLastError();
		return false;
	}

	return true;
}
//----------------------------------------------------------------------------------------------

bool ASQLiteDatabase::transaction ()
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

bool ASQLiteDatabase::commit ()
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

bool ASQLiteDatabase::rollback ()
{
	if (ADatabase::isOpen() == false || m_transaction == false)
		return false;

	if (ADatabase::rollback() == false)
		setLastError();

	m_transaction = false;

	return true;
}
//----------------------------------------------------------------------------------------------

AQuery* ASQLiteDatabase::createQuery (const QString& sql, bool prepared)
{
	if (checkDatabase() == false)
		return NULL;

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
