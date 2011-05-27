//----------------------------------------------------------------------------------------------
// $Date: 2011-04-07 11:52:43 +0400 (Чтв, 07 Апр 2011) $
// $Author: antonbatenev.ya.ru $
// $Revision: 415 $
// $URL: svn://opensvn.ru/avalon/trunk/global.cpp $
//----------------------------------------------------------------------------------------------
#include "global.h"
//----------------------------------------------------------------------------------------------
#include "storage/storage_factory.h"
//----------------------------------------------------------------------------------------------
AGlobal* g_global = NULL;
//----------------------------------------------------------------------------------------------

AGlobal* AGlobal::getInstance ()
{
	if (g_global != NULL)
		return g_global;

	return new AGlobal();
}
//----------------------------------------------------------------------------------------------

AGlobal::AGlobal ()
{
	g_global = this;

	AnonymousName = QString::fromUtf8("Аноним");
	DateFormat    = "dd.MM.yyyy hh:mm:ss";

	Me.ID = 0;

	reload();
}
//----------------------------------------------------------------------------------------------

AGlobal::~AGlobal ()
{
	clearTempList();

	g_global = NULL;
}
//----------------------------------------------------------------------------------------------

void AGlobal::reload ()
{
	QSettings settings;

#ifdef AVALON_USE_ZLIB
	Compression = settings.value("storage/compression", false).toInt();
#endif

	// установка колорера для подсветки синтаксиса
	QString colorer = settings.value("ui/colorer", COLORER_DEFAULT_NAME).toString();

	if (colorer == COLORER_ENSCRIPT_NAME)
		Colorer = acEnscript;
	else if (colorer == COLORER_COLORER_TAKE5_NAME)
		Colorer = acColorerTake5;
	else if (colorer == COLORER_GNU_SOURCE_HIGHLIGHT_NAME)
		Colorer = acGNUSourceHighlight;
	else if (colorer == COLORER_HIGHLIGHT_JS_NAME)
		Colorer = acHighlightJS;
	else
		Colorer = COLORER_DEFAULT;

	// информация о текущем пользователе avalon
	Me.ID             = -1;
	Me.Name           = "(n/a)";
	Me.Nick           = "(n/a)";
	Me.RealName       = "(n/a)";
	Me.Email          = "(n/a)";
	Me.Homepage       = "(n/a)";
	Me.Specialization = "(n/a)";
	Me.WhereFrom      = "(n/a)";
	Me.Origin         = "(n/a)";

	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() != NULL)
	{
		Me.Name = settings.value("rsdn/login", "").toString();
		storage->whoAmI(Me);
	}
}
//----------------------------------------------------------------------------------------------

void AGlobal::clearTempList ()
{
	while (TempFileList.count() > 0)
	{
		QTemporaryFile* file = TempFileList.takeLast();

		delete file;
	}
}
//----------------------------------------------------------------------------------------------
