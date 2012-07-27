/*!
 * \file
 * Точка входа
 */

#include "form_main.h"
#include "global.h"
#include "logger.h"
#include <memory>

Logger g_logger;

static void log_helper(QtMsgType type, const char *msg)
{
	g_logger.logMessage(type, msg);
}

void init_logging()
{
	QSettings settings;
	int loggingLevel = settings.value("other/logging_level", 2).toInt();
	g_logger.setDebugLevel((QtMsgType)loggingLevel);
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

	qInstallMsgHandler(log_helper);

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

	init_logging();

	return app.exec();
}
