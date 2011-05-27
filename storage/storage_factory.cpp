//----------------------------------------------------------------------------------------------
// $Date: 2009-04-06 02:06:38 +0400 (Пнд, 06 Апр 2009) $
// $Author: antonbatenev.ya.ru $
// $Revision: 210 $
// $URL: svn://opensvn.ru/avalon/trunk/storage/storage_factory.cpp $
//----------------------------------------------------------------------------------------------
#include "storage_factory.h"
//----------------------------------------------------------------------------------------------
#include "mysql_storage.h"
#include "sqlite_storage.h"
//----------------------------------------------------------------------------------------------

IAStorage* AStorageFactory::getStorage ()
{
	// чтение типа хранилища из конфига
	QSettings settings;

	QString type = settings.value("storage/type", "MySQL").toString();

	if (type == "MySQL")
		return new AMySQLStorage();
	else if (type == "SQLite")
		return new ASQLiteStorage();

	return NULL;
}
//----------------------------------------------------------------------------------------------
