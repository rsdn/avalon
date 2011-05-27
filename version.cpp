//----------------------------------------------------------------------------------------------
// $Date: 2010-11-03 23:45:09 +0300 (Срд, 03 Ноя 2010) $
// $Author: antonbatenev.ya.ru $
// $Revision: 366 $
// $URL: svn://opensvn.ru/avalon/trunk/version.cpp $
//----------------------------------------------------------------------------------------------
#include "version.h"
#include "sysheaders.h"
//----------------------------------------------------------------------------------------------

int getRevision ()
{
	QString revision_str = AVALON_REVISION;

	revision_str.replace("$Revision: ", "");
	revision_str.replace(" $", "");

	bool ok = false;
	int revision = revision_str.toInt(&ok);

	return revision;
}
//----------------------------------------------------------------------------------------------

QString getVersionString ()
{
	QString result = QString("avalon 1.0rc3 rev ") + QString::number(getRevision());

#ifdef AVALON_USE_ZLIB
	result += ", zlib " + QString(zlibVersion());
#endif

	return result;
}
//----------------------------------------------------------------------------------------------

QString getTagline ()
{
	QSettings settings;

	QString tagline = settings.value("ui/tagline", "%%version%%").toString();

	tagline = tagline.replace("%%version%%", "[url=http://trac.opensvn.ru/avalon/wiki/screenshot]" + getVersionString() + "[/url]");

	return tagline;
}
//----------------------------------------------------------------------------------------------
