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
