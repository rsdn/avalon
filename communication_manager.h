#ifndef COMMUNICATION_MANAGER_H
#define COMMUNICATION_MANAGER_H

#include "sysheaders.h"
#include <QWaitCondition>
#include <QMutex>

class CommunicationManager : public QObject
{
    Q_OBJECT
public:
    enum State {
        Ok,
        Error,
        NoStorage,
        InvalidStorage,
    };


    explicit CommunicationManager(QObject *parent = 0);

    void start();
    void stop();
    void startSync();

signals:
    void syncDone(int withState);

public slots:
    void svc();
//    void finished();

    void process_state_changed            (int state);                       /*!< \brief Смена состояния              */
    void process_data_read_progress       (int done, int total);             /*!< \brief Прогресс чтения данных       */
    void process_data_send_progress       (int done, int total);             /*!< \brief Прогресс отправки данных     */
    void process_response_header_received (const QHttpResponseHeader& resp); /*!< \brief Получен заголовок ответа     */
    void process_request_started          (int id);                          /*!< \brief Начало выполнения запроса    */
    void process_request_finished         (int id, bool error);              /*!< \brief Окончание выполнения запроса */
    void process_ssl_errors               (const QList<QSslError> &errors);  /*!< \brief Ошибка SSL                   */

private:
    void sync();

private:
    QThread m_syncThread;
    QHttp m_http;
    QWaitCondition m_waitForSync;
    QMutex m_sync;
    bool m_active;
};

#endif // COMMUNICATION_MANAGER_H
