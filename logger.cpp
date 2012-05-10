#include "logger.h"

Logger::Logger() : m_debugLevel(QtWarningMsg)
{
    m_logfileName = QDir::tempPath() + "/avalon_" +
            QDateTime::currentDateTime().toString(Qt::ISODate) + ".log";
}

void Logger::setDebugLevel(QtMsgType level)
{
    m_debugLevel = level;
    logMessage(QtSystemMsg, QString("Setting up new logging level: %1").arg(level).toStdString().c_str());
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
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}
