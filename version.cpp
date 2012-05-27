/*!
 * \file
 * \brief Обрпаботка шаблона файла версии, который меняется при каждом билде
 */
//----------------------------------------------------------------------------------------------
#include "version.h"
#include "sysheaders.h"
//----------------------------------------------------------------------------------------------

int getBuildNumber ()
{
	return AVALON_BUILD;
}
//----------------------------------------------------------------------------------------------

QString getVersionString ()
{
	QString result = QString("avalon 1.0rc3 build ") + QString::number(getBuildNumber());

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

	tagline = tagline.replace("%%version%%", "[url=https://github.com/rsdn/avalon/wiki]" + getVersionString() + "[/url]");

	return tagline;
}
//----------------------------------------------------------------------------------------------
