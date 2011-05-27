//----------------------------------------------------------------------------------------------
// $Date: 2009-03-23 03:03:38 +0300 (Пнд, 23 Мар 2009) $
// $Author: antonbatenev.ya.ru $
// $Revision: 175 $
// $URL: svn://opensvn.ru/avalon/trunk/icon_effect.cpp $
//----------------------------------------------------------------------------------------------
#include "icon_effect.h"
//----------------------------------------------------------------------------------------------

QIcon AIconEffect::unionIcons (const QString& file1, const QString& file2)
{
	QImage img_1 = QImage(file1);
	QImage img_2 = QImage(file2);

	QPainter painter(&img_1);

	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

	painter.drawImage(0, 0, img_2);

	painter.end();

	return QIcon(QPixmap::fromImage(img_1));
}
//----------------------------------------------------------------------------------------------
