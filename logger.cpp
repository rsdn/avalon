#include "logger.h"
#include <QDebug>

Logger::Logger() : m_debugLevel(QtCriticalMsg)
{
#ifndef __APPLE__
    m_logfileName = QDir::tempPath();
#else
    m_logfileName = QDir::homePath () + "/Library/Logs/Avalon";
    QDir logDir(m_logfileName);
    qDebug() << logDir.absolutePath();
    logDir.mkpath(logDir.absolutePath());
#endif
    m_logfileName += "/avalon_" +
            QDateTime::currentDateTime().toString(Qt::ISODate) + ".log";
}

void Logger::setDebugLevel(QtMsgType level)
{
    m_debugLevel = level;
}

void Logger::logMessage(QtMsgType type, const char *msg)
{
    if(type < m_debugLevel)
        return;

    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("%1 : debug: %2").arg(QTime::currentTime().toString(Qt::ISODate), msg);
        break;

    case QtWarningMsg:
        txt = QString("%1 : warning: %2").arg(QTime::currentTime().toString(Qt::ISODate), msg);
    break;
    case QtCriticalMsg:
        txt = QString("%1 : critical: %2").arg(QTime::currentTime().toString(Qt::ISODate), msg);
    break;
    case QtFatalMsg:
        txt = QString("%1 : fatal: %2").arg(QTime::currentTime().toString(Qt::ISODate), msg);
        abort();
    }

    QFile outFile(m_logfileName);
    bool res = outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}
