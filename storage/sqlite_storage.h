/*!
 * \file
 * \brief Хранилище на базе SQLite
 *
 * $Date: 2011-04-07 11:52:43 +0400 (Чтв, 07 Апр 2011) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 415 $
 * $URL: svn://opensvn.ru/avalon/trunk/storage/sqlite_storage.h $
 */

#ifndef _avalon_sqlite_storage_h_
#define _avalon_sqlite_storage_h_

#include "istorage.h"
#include "sqlite_database.h"

/*!
 * \brief Хранилище на базе SQLite
 */
class ASQLiteStorage :
	public ASQLiteDatabase, // обертка для работы с базами SQLite
	public IAStorage        // интерфейс, который возвращает фабрика хранилищ при работе
{
	public:

		ASQLiteStorage  ();
		~ASQLiteStorage ();

	private:

		/*!
		 * \brief Устанавливает IAStorage::SetLastError("OK")
		 * \return Всегда true
		 */
		bool returnSuccess ();

		/*!
		 * \brief Устанавливает IAStorage::SetLastError(message)
		 * \param message Сообщение об ошибке
		 * \return Всегда false
		 */
		bool returnError (const QString& message);

	// IAStorage (см. istorage.h)
	private:

		bool ping                   ();
		bool whoAmI                 (AUserInfo& info);
		bool getRowVersion          (ARowVersion& list);
		bool getForumList           (AForumGroupInfoList& list, bool subscribed_only);
		bool setForumList           (const AForumGroupInfoList& list);
		bool getSubscribedForumList (ASubscribedForumInfoList& list);
		bool setSubscribedForumList (const ASubscribedForumInfoList& list);
		bool getForumInfo           (int id_forum, AForumInfo& info);
		bool setUserList            (const AUserInfoList& list, const QString& row_version);
		bool getMaxIDMessage        (int& max_id);
		bool getMinIDMessage        (int& min_id);
		bool getMessageQuery        (ADataQuery& query);
		bool setMessageList         (const ADataList& list, const ARowVersion& row_version, bool save_row_version);
		bool getUnreadCount         (AUnreadForumCountInfoList& list, int id_me);
		bool getForumTopicList      (int id_forum, int count, QList<int>& list);
		bool getTopicInfoList       (int id_forum, AMessageInfoGUIPtrList& list, int id_me);
		bool getTopicMessageList    (int id_forum, int id_topic, AMessageInfoGUIPtrList& list, IMessageInfoGUIFactory* factory);
		bool getMessageBody         (int id_message, QString& body);
		bool setIDsAsRead           (const QList<int>& list, AIDSet type, bool read, QDateTime date);
		bool unsubscribe            (const QList<int>& list, AIDSet type, bool clean);
		bool getMessageIds          (int from_id, int to_id, QList<int>& list);
		bool addMessage2Send        (const AMessage2Send& info);
		bool getMessage2SendList    (AMessageInfoList& list, bool drafts);
		bool getModerate2SendInfo   (int id, AModerate2Send& info);
		bool addRating2Send         (const ARating2Send& info);
		bool getRating2SendList     (AMessageInfoList& message_list, ARating2SendList& rating_list);
		bool getRating2SendList     (ARating2SendList& list);
		bool changeRating           (int id, int new_rate);
		bool addModerate2Send       (const AModerate2Send& info);
		bool getModerate2SendList   (AMessageInfoList& message_list, AModerate2SendList& moderate_list);
		bool getModerate2SendList   (AModerate2SendList& list);
		bool setCommitResult        (const ACommitInfo& info);
		bool deleteSpecial          (const QList<int>& ids, int id_special);
		bool getMessageRatingList   (int id_message, AMessageRatingList& list);
		bool compressStorage        (IProgress* progress);
		bool uncompressStorage      (IProgress* progress);
		bool getMessagePath         (int id_message, int& id_forum, QList<int>& path);
		bool addBroken              (int id_message, bool is_topic);
		bool hasBroken              (bool& result);
		bool getMyMessageList       (int id_me, int count, QList<int>& list);
		bool getAnswers2MeList      (int id_me, int count, QList<int>& list);
};

#endif
