#include "communication_manager.h"

#include "global.h"
#include "storage/storage_factory.h"
#include "webservice.h"

CommunicationManager::CommunicationManager(QObject *parent) :
    QObject(parent),
    m_active(false)
{
    connect(&m_syncThread, SIGNAL(started()), this, SLOT(svc()));
    moveToThread(&m_syncThread);
}

void CommunicationManager::start()
{
    m_active = true;
    m_syncThread.start();
}

void CommunicationManager::stop()
{
    if(m_active)
    {
        m_active = false;
        m_waitForSync.wakeAll();
        m_syncThread.wait();
    }
}

void CommunicationManager::svc()
{
    while(m_active)
    {
        m_sync.lock();
        m_waitForSync.wait(&m_sync);
        sync();
//        emit syncDone(Error);
    }
}

void CommunicationManager::sync()
{
    // основные настройки
    QSettings settings;

    QString rsdn_host = settings.value("rsdn/host", "www.rsdn.ru").toString();
    int     rsdn_port = settings.value("rsdn/port", "80").toInt();

    // получение хранилища
    std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

    if (storage.get() == NULL)
    {
        emit syncDone(NoStorage);
        return;
    }


}

void CommunicationManager::startSync()
{
    m_waitForSync.wakeAll();
}

void CommunicationManager::process_state_changed(int state)
{
}

void CommunicationManager::process_data_read_progress(int done, int total)
{
}

void CommunicationManager::process_data_send_progress(int done, int total)
{
}

void CommunicationManager::process_response_header_received(const QHttpResponseHeader& resp)
{
}

void CommunicationManager::process_request_started(int id)
{
}

void CommunicationManager::process_request_finished(int id, bool error)
{
}

void CommunicationManager::process_ssl_errors(const QList<QSslError> &errors)
{
}
