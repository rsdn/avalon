#include "storage_factory.h"
//----------------------------------------------------------------------------------------------
#include "mysql_storage.h"
#include "sqlite_storage.h"
//----------------------------------------------------------------------------------------------

IAStorage* AStorageFactory::getStorage ()
{
	// чтение типа хранилища из конфига
	QSettings settings;

	QString type = settings.value("storage/type", "SQLite").toString();

	if (type == "MySQL")
		return new AMySQLStorage();
	else if (type == "SQLite")
		return new ASQLiteStorage();

	return NULL;
}
//----------------------------------------------------------------------------------------------
