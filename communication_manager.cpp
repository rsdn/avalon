#include "communication_manager.h"

#include "global.h"
#include "storage/storage_factory.h"

CommunicationManager::CommunicationManager(QObject *parent) :
    QObject(parent)
{

    moveToThread(m_syncThread);
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
        QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), QString::fromUtf8("Не выбрано хранилище данных"));
        return;
    }

    //
    // отправка сообщений/модерилок/рейтингов
    //

    QString header;
    QString data;

    AMessageInfoList   messages_temp;
    AMessage2SendList  messages;
    ARating2SendList   ratings;
    AModerate2SendList moderates;

    if (storage->getMessage2SendList(messages_temp, false) == false)
    {
        storage->showError(this);
        return;
    }

    if (storage->getRating2SendList(ratings) == false)
    {
        storage->showError(this);
        return;
    }

    if (storage->getModerate2SendList(moderates) == false)
    {
        storage->showError(this);
        return;
    }

    for (int i = 0; i < messages_temp.count(); i++)
    {
        AMessage2Send info;

        info.ID       = messages_temp[i].ID;
        info.IDParent = messages_temp[i].IDParent;
        info.IDForum  = messages_temp[i].IDForum;
        info.Subject  = messages_temp[i].Subject;
        info.Message  = messages_temp[i].Message;
        info.Date     = messages_temp[i].MessageDate;

        messages.append(info);
    }

    messages_temp.clear();

    // отправка данных
    QString cookie = "";

    if (messages.count() != 0 || ratings.count() != 0 || moderates.count() != 0)
    {
        AWebservice::postChange_WebserviceQuery(header, data, messages, ratings, moderates, NULL);

        std::auto_ptr<FormRequest> form(new FormRequest(this, rsdn_host, rsdn_port, header, data));

        if (form->exec() == QDialog::Accepted)
        {
            QString answer = form->getResponseHeader();

            QString result = AWebservice::postChange_WebserviceParse(answer, cookie, NULL);

            if (result.length() > 0)
            {
                QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), result);
                return;
            }
        }
        else
            return;

    }

    // коммит данных
    if (messages.count() != 0 || ratings.count() != 0 || moderates.count() != 0)
    {
        AWebservice::postChangeCommit_WebserviceQuery(header, data, cookie, NULL);

        std::auto_ptr<FormRequest> form(new FormRequest(this, rsdn_host, rsdn_port, header, data));

        if (form->exec() == QDialog::Accepted)
        {
            // получение ответа
            bool error;
            QString answer = form->getResponse(error);

            if (error == true)
            {
                QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), answer);
                return;
            }

            // парсинг ответа
            ACommitInfo commit_info;

            QString result = AWebservice::postChangeCommit_WebserviceParse(answer, commit_info, NULL);

            if (result.length() > 0)
            {
                QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), result);
                return;
            }

            // сохранение ответа
            if (storage->setCommitResult(commit_info) == false)
            {
                storage->showError(this);
                return;
            }

            m_forum_tree->reloadUnread(true);

            // проверка исключений
            if (commit_info.MessagesExceptions.count() != 0 || commit_info.RatingExceptions.count() != 0 || commit_info.ModerateExceptions.count() != 0)
            {
                QString warning = "";

                if (commit_info.MessagesExceptions.count() != 0)
                {
                    warning += QString::fromUtf8("<b>Отправка сообщений<b>:\n<pre>");

                    for (int i = 0; i < commit_info.MessagesExceptions.count(); i++)
                    {
                        warning += "ID        : " + QString::number(commit_info.MessagesExceptions[i].ID) + "\n";
                        warning += "Exception : " + commit_info.MessagesExceptions[i].Exception + "\n";
                        warning += "Info      : " + commit_info.MessagesExceptions[i].Info + "\n";
                        warning += "\n";
                    }

                    warning += "</pre>\n";
                }

                if (commit_info.RatingExceptions.count() != 0)
                {
                    warning += QString::fromUtf8("<b>Отправка рейтингов</b>:\n<pre>");

                    for (int i = 0; i < commit_info.RatingExceptions.count(); i++)
                    {
                        warning += "ID        : " + QString::number(commit_info.RatingExceptions[i].ID) + "\n";
                        warning += "Exception : " + commit_info.RatingExceptions[i].Exception + "\n";
                        warning += "Info      : " + commit_info.RatingExceptions[i].Info + "\n";
                        warning += "\n";
                    }

                    warning += "</pre>\n";
                }

                if (commit_info.ModerateExceptions.count() != 0)
                {
                    warning += QString::fromUtf8("<b>Отправка рейтингов</b>:\n<pre>");

                    for (int i = 0; i < commit_info.ModerateExceptions.count(); i++)
                    {
                        warning += "ID        : " + QString::number(commit_info.ModerateExceptions[i].ID) + "\n";
                        warning += "Exception : " + commit_info.ModerateExceptions[i].Exception + "\n";
                        warning += "Info      : " + commit_info.ModerateExceptions[i].Info + "\n";
                        warning += "\n";
                    }

                    warning += "</pre>\n";
                }

                QMessageBox::warning(this, QString::fromUtf8("Ошибки при отправке данных!"), warning, QMessageBox::Ok);
            }
        }
        else
            return;
    }

    //
    // получение версий строк
    //

    ARowVersion row_version;

    if (storage->getRowVersion(row_version) == false)
    {
        storage->showError(this);
        return;
    }

    //
    // синхронизация списка пользователей
    //

    // для того, чтобы загрузить весь список пользователей
    // необходимо повторить операцию N раз, т.к. команда RSDN
    // ограничила максимальное кол-во записей в 1000

    QString user_row_version = "";

    while (user_row_version != row_version.User)
    {
        user_row_version = row_version.User;

        // получение текста запроса
        AWebservice::getUserList_WebserviceQuery(header, data, row_version.User, NULL);

        // запрос к вебсервису
        std::auto_ptr<FormRequest> form(new FormRequest(this, rsdn_host, rsdn_port, header, data));

        if (form->exec() == QDialog::Accepted)
        {
            // получение ответа
            bool error;
            QString answer = form->getResponse(error);

            if (error == true)
            {
                QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), answer);
                return;
            }

            // парсинг ответа
            AUserInfoList list;

            QString result = AWebservice::getUserList_WebserviceParse(answer, list, row_version.User, NULL);

            if (result.length() > 0)
            {
                QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), result);
                return;
            }

            // сохранение ответа
            if (storage->setUserList(list, row_version.User) == false)
            {
                storage->showError(this);
                return;
            }
        }
        else
            return;
    }

    // после первой синхронизации списка пользователей может быть полезно перегрузить текущего пользователя,
    // т.к. он может быть неизвестен (см. #48)
    AGlobal::getInstance()->reload();

    //
    // синхронизация сообщений
    //

    // для того, чтобы загрузить весь список сообщений
    // необходимо повторить операцию N раз, т.к. могут быть, например, оборваные ветки

    QString message_row_version  = "";
    QString rating_row_version   = "";
    QString moderate_row_version = "";
    bool    has_broken           = true;

    while (!(
        message_row_version  == row_version.Message  &&
        rating_row_version   == row_version.Rating   &&
        moderate_row_version == row_version.Moderate &&
        has_broken           == false
    ))
    {
        message_row_version  = row_version.Message;
        rating_row_version   = row_version.Rating;
        moderate_row_version = row_version.Moderate;


        ADataQuery query;

        // получение параметров запроса
        if (storage->getMessageQuery(query) == false)
        {
            storage->showError(this);
            return;
        }

        // получение текста запроса
        AWebservice::getMessageList_WebserviceQuery(header, data, row_version, query, NULL);

        // запрос к вебсервису
        std::auto_ptr<FormRequest> form(new FormRequest(this, rsdn_host, rsdn_port, header, data));

        if (form->exec() == QDialog::Accepted)
        {
            bool error;
            QString answer = form->getResponse(error);

            if (error == true)
            {
                QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), answer);
                return;
            }

            // парсинг ответа
            ADataList list;

            QString result = AWebservice::getMessageList_WebserviceParse(answer, list, row_version, NULL);

            if (result.length() > 0)
            {
                QMessageBox::critical(this, QString::fromUtf8("Ошибка!"), result);
                return;
            }

            // сохранение ответа
            if (storage->setMessageList(list, row_version, true) == false)
            {
                storage->showError(this);
                return;
            }

            // получение количества поломанных топиков
            if (storage->hasBroken(has_broken) == false)
            {
                storage->showError(this);
                return;
            }
        }
        else
            return;

        // перезагрузка количества непрочитаных сообщений
        m_forum_tree->reloadUnread(true);
    }
}
