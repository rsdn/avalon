/*!
 * \file
 * Точка входа
 *
 * $Date: 2010-07-06 05:08:24 +0400 (Втр, 06 Июл 2010) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 337 $
 * $URL: svn://opensvn.ru/avalon/trunk/main.cpp $
 */

#include "form_main.h"
#include "global.h"

void customMessageHandler(QtMsgType type, const char *msg)
{
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg);
        break;

    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
    break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
    break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
        abort();
    }

    QFile outFile("debuglog.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}

/*!
 * (no comments)
 * \param argc (no comments)
 * \param argv (no comments)
 * \return (no comments)
 */
int main (int argc, char* argv[])
{
	// инициализация ресурсов (необязательный пункт)
	Q_INIT_RESOURCE(resource);

	QApplication app(argc, argv);

    qInstallMsgHandler(customMessageHandler);

#ifndef Q_WS_MAC
	// отображать иконки в меню для всех, кроме MacOS
	app.setAttribute(Qt::AA_DontShowIconsInMenus, false);
#endif

	// установка разделов для хранения конфигурации приложения
	QCoreApplication::setOrganizationName("www.rsdn.ru");
	QCoreApplication::setApplicationName("avalon");

	// переводчик стандартных сообщений и диалогов
	QString tr_file = "qt_" + QLocale::system().name();
	QString tr_path = QLibraryInfo::location(QLibraryInfo::TranslationsPath);

	if (tr_file == "qt_ru_RU" && QFileInfo (tr_path + "/" + tr_file + ".qm").exists() == false)
	{
		tr_file = ":translations/qt_ru_RU.qm";
		tr_path = "";
	}

	QTranslator qt_translator;
	qt_translator.load(tr_file, tr_path);
	app.installTranslator(&qt_translator);

	// глобальные настройки
	std::auto_ptr<AGlobal> global(AGlobal::getInstance());

	// главная форма
	AFormMain* form = new AFormMain();

	form->show();

	app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));

	return app.exec();
}
