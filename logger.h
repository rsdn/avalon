#ifndef LOGGER_H
#define LOGGER_H

#include "sysheaders.h"

class Logger
{
public:
    Logger();
    void setDebugLevel(QtMsgType level);
    void logMessage(QtMsgType type, const char *msg);
private:
    QString m_logfileName;
    QtMsgType m_debugLevel;
};

#endif // LOGGER_H
