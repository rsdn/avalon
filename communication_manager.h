#ifndef COMMUNICATION_MANAGER_H
#define COMMUNICATION_MANAGER_H

#include <QObject>


class CommunicationManager : public QObject
{
    Q_OBJECT
public:
    enum State {
        Ok,
        InvalidStorage,
    };


    explicit CommunicationManager(QObject *parent = 0);

signals:
    void syncDone(bool success);

public slots:
    void sync();

private:
    QThread m_syncThread;
};

#endif // COMMUNICATION_MANAGER_H
