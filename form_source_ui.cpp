//----------------------------------------------------------------------------------------------
// $Date: 2010-12-04 20:27:36 +0300 (Сбт, 04 Дек 2010) $
// $Author: antonbatenev.ya.ru $
// $Revision: 372 $
// $URL: svn://opensvn.ru/avalon/trunk/form_source_ui.cpp $
//----------------------------------------------------------------------------------------------
#include "form_source_ui.h"
//----------------------------------------------------------------------------------------------

FormSourceUI::FormSourceUI (QWidget* parent) : QDialog (parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint
#if QT_VERSION >= 0x040500
 | Qt::WindowCloseButtonHint
#endif
)
{
	setWindowTitle(QString::fromUtf8("просмотр источника"));

	QIcon icon;
	icon.addFile(":/icons/viewsource16.png", QSize(16,  16));
	icon.addFile(":/icons/viewsource22.png", QSize(22,  22));
	icon.addFile(":/icons/viewsource32.png", QSize(32,  32));
	icon.addFile(":/icons/viewsource48.png", QSize(48,  48));
	icon.addFile(":/icons/viewsource64.png", QSize(64, 64));
	setWindowIcon(icon);

	resize(400, 300);
	setMinimumSize(QSize(400, 300));

	m_layout = new QVBoxLayout(this);

	m_text_source = new QTextEdit(this);

	#ifndef Q_WS_WIN
		m_text_source->setFrameShadow(QFrame::Plain);
	#endif

	m_text_source->setReadOnly(true);

	m_layout->addWidget(m_text_source);

	// восстановление layout
	restore();
}
//----------------------------------------------------------------------------------------------

FormSourceUI::~FormSourceUI ()
{
}
//----------------------------------------------------------------------------------------------

void FormSourceUI::save ()
{
	QSettings settings;

	if (windowState() == Qt::WindowNoState)
	{
		QRect rect = geometry();

		settings.setValue("form_source/left",   rect.left());
		settings.setValue("form_source/top",    rect.top());
		settings.setValue("form_source/width",  rect.width());
		settings.setValue("form_source/height", rect.height());
	}

	settings.setValue("form_source/state", static_cast<int>(windowState()));
}
//----------------------------------------------------------------------------------------------

void FormSourceUI::restore ()
{
	QSettings settings;

	QRect screen = QDesktopWidget().screenGeometry(this);

	int w = settings.value("form_source/width",  width()).toInt();
	int h = settings.value("form_source/height", height()).toInt();
	int x = settings.value("form_source/left",   (screen.width()  - w) / 2).toInt();
	int y = settings.value("form_source/top",    (screen.height() - h) / 2).toInt();

	// setGeometry как-то некорректно работает :(
	resize(w, h);
	move(x, y);

	Qt::WindowStates state = static_cast<Qt::WindowStates>(settings.value("form_source/state", Qt::WindowNoState).toInt());

	setWindowState(state);
}
//----------------------------------------------------------------------------------------------
