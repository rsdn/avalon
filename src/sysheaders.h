/*!
 * \file
 * \brief Файл для включения всех системных и библиотечных заголовочных файлов.
 */

#ifndef _avalon_sysheaders_h_
#define _avalon_sysheaders_h_

//
// стандартные заголовочные файлы Qt
//

#include <QDir>
#include <QHash>
#include <QHttp>
#include <QIcon>
#include <QFrame>
#include <QTimer>
#include <QLabel>
#include <QDialog>
#include <QRegExp>
#include <QSysInfo>
#include <QtGlobal>
#include <QWebView>
#include <QMenuBar>
#include <QToolBar>
#include <QPainter>
#include <QProcess>
#include <QLineEdit>
#include <QSplitter>
#include <QSettings>
#include <QComboBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QTimeEdit>
#include <QGroupBox>
#include <QCheckBox>
#include <QTabWidget>
#include <QScrollBar>
#include <QStatusBar>
#include <QValidator>
#include <QTextCodec>
#include <QTcpSocket>
#include <QSslSocket>
#include <QTranslator>
#include <QStringList>
#include <QTreeWidget>
#include <QPushButton>
#include <QCloseEvent>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QHeaderView>
#include <QMessageBox>
#include <QListWidget>
#include <QFileDialog>
#include <QProgressBar>
#include <QSqlDatabase>
#include <QApplication>
#include <QTextBrowser>
#include <QLibraryInfo>
#include <QIntValidator>
#include <QNetworkProxy>
#include <QDesktopWidget>
#include <QTemporaryFile>
#include <QCalendarWidget>
#include <QDesktopServices>

//
// системные заголовочные файлы
//

#include <memory>

//
// заголовочные файлы дополнительных библиотек
//

#ifdef Q_WS_WIN
	#ifdef _MSC_VER
		// warning C4996: 'sprintf' was declared deprecated
		#pragma warning (disable : 4996)
	#endif
#endif

#ifndef _MSC_VER
	// возможность сжатия тел сообщений
	#define AVALON_USE_ZLIB
	#ifdef AVALON_USE_ZLIB
		// for windows use http://www.winimage.com/zLibDll/index.html
		#include <zlib.h>
	#endif

	// проверка орфографии
	#define AVALON_USE_ASPELL
	#ifdef AVALON_USE_ASPELL
		// for windows use http://aspell.net/win32/
		#include <aspell.h>
		#include <QSyntaxHighlighter>
	#endif
#endif

/*!
 * \brief Получение номера билда (см. version.cpp)
 * \return Номер билда, на котором основана текущая сборка
 */
int getBuildNumber ();

/*!
 * \brief Получение строки версии (см. version.cpp)
 * \return Строка версии вида "1.0 build NNN"
 */
QString getVersionString ();

/*!
 * \brief Получение строки имени агента (см. version.cpp)
 * \return Строка агента вида "avalon 1.0 build NNN"
 */
QString getAgentString ();

/*!
 * \brief Получение tagline (см. version.cpp)
 * \return Строка таглайна
 */
QString getTagline ();

/*!
 * \brief Макрос для отладки и легкого вывода алерта, в релизе использоваться не должен.
 * \param x сообщение для вывода.
 */
#define DEBUG_MSG(x) QMessageBox::critical(0, QString::fromUtf8("Ошибка!"), x)

/*!
 * \brief Макрос для отладки и легкого вывода алерта (числа), в релизе использоваться не должен.
 * \param x число для вывода.
 */
#define DEBUG_INT(x) DEBUG_MSG(QString::number(x))

#ifndef Q_WS_WIN

#include <sys/time.h>

/*!
 * \brief Класс "профайлера" для отметок времени выполнения кусков кода при отладке, в релизе использоваться не должен.
 */
class AProfiler
{
	public:

		/*!
		 * \brief При создании экземпляра создается первая отсечка времени.
		 */
		AProfiler ()
		{
			start();
		}

		/*!
		 * \brief Создание первой отсечки времени, при создании экземпляра класса создается автоматически,
		 * при повторном вызове заменяет старую отсечку времени текущей.
		 */
		void start ()
		{
			gettimeofday(&m_start, 0);
		}

		/*!
		 * \brief Создание второй отсечки времени и вывод разницы между текущей отсечкой и стартовой в миллисекундах.
		 * \param ittr Количество итераций
		 */
		void stop (int ittr = 1)
		{
			gettimeofday(&m_stop, 0);

			long long int delta = (m_stop.tv_sec - m_start.tv_sec) * 1000 + (m_stop.tv_usec - m_start.tv_usec) / 1000;

			QString msg;

			msg += QString::fromUtf8("Общее время : ") + QString::number(delta) + " ms\n";
			msg += QString::fromUtf8("Среднее время : ") + QString::number(delta / ittr) + " ms";

			QMessageBox::information(0, QString::fromUtf8("Таймер!"), msg);
		}

	private:

		/*!
		 * \brief Время вызова функции Start().
		 */
		timeval m_start;

		/*!
		 * \brief Время вызова функции Stop().
		 */
		timeval m_stop;
};

#endif // #ifndef Q_WS_WIN

#endif
