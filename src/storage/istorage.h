/*!
 * \file
 * \brief Интерфейс для работы с хранилищем данных
 */

#ifndef _avalon_istorage_h_
#define _avalon_istorage_h_

#include "model/all.h"
#include "iprogress.h"
#include "database_error.h"

/*!
 * \brief Типы множеств для групповой обработки (см. IAStorage::setIDsAsRead и IAStorage::unsubscribe).
 */
typedef enum AIDSet
{
	idsMessage,   /*!< \brief Сообщение      */
	idsTopic,     /*!< \brief Топик          */
	idsForum,     /*!< \brief Форум          */
	idsGroup,     /*!< \brief Группа форумов */
	idsAll        /*!< \brief Все            */
} AIDSet;

/*!
 * \brief Общий интерфейс для всех хранилищ (см. AStorageFactory)
 * Все методы возвращают true или false в зависимости от успеха выполнения операции.
 * Если метод вернул false, то сообщение об ошибке можно получить вызовом ADatabaseError::getLastError().
 * Последним параметром всегда передается указатель на интерфейс IProgress для отображения прогресса выполнения операции или NULL, если отображение не требуется.
 */
class IAStorage : public ADatabaseError
{
	public:

		IAStorage () : ADatabaseError () {}
		virtual ~IAStorage ()  {}

		/*!
		 * \brief Проверка соединения с БД.
		 */
		virtual bool ping () = 0;

		/*!
		 * \brief Создание новой БД.
		 */
		virtual bool createDatabase () = 0;

		/*!
		 * \brief Получение информации о текущем пользователе.
		 * \param info Информация о пользователе, перед вызовом должно быть заполнено поле Name (логин на RSDN).
		 */
		virtual bool whoAmI (AUserInfo& info) = 0;

		/*!
		 * \brief Получение версий строк (снимков).
		 * \param list Версии строк.
		 */
		virtual bool getRowVersion (ARowVersion& list) = 0;

		/*!
		 * \brief Возвращает дерево форумов.
		 * \param list Список групп/форумов.
		 * \param subscribed_only true, для того, чтобы получить только дерево подписаных форумов, false - все форумы.
		 */
		virtual bool getForumList (AForumGroupInfoList& list, bool subscribed_only) = 0;

		/*!
		 * \brief Устанавливает (сохраняет) дерево форумов.
		 * \param list Заполненый список описателей форумов.
		 */
		virtual bool setForumList (const AForumGroupInfoList& list) = 0;

		/*!
		 * \brief Возвращает список описателей подписаных форумов.
		 * \param list Список описателей.
		 */
		virtual bool getSubscribedForumList (ASubscribedForumInfoList& list) = 0;

		/*!
		 * \brief Устанавливает (сохраняет) список описателей подписаных форумов.
		 * \param list Заполненый список описателей подписаных форумов.
		 */
		virtual bool setSubscribedForumList (const ASubscribedForumInfoList& list) = 0;

		/*!
		 * \brief Получение описателя форума
		 * \param id_forum ID форума
		 * \param info Описатель форума
		 */
		virtual bool getForumInfo (int id_forum, AForumInfo& info) = 0;

		/*!
		 * \brief Сохраняет список пользователей добавляя новых и обновляя информацию о существующих.
		 * \param list Заполненый список описателей пользователей.
		 * \param row_version Версия снимка.
		 */
		virtual bool setUserList (const AUserInfoList& list, const QString& row_version) = 0;

		/*!
		 * \brief Возвращает максимальный id для списка сообщений
		 * Функция полезна для работы с полной таблицей по диапазонам id
		 * \param max_id - максимальный id для таблицы со списком сообщений
		 */
		virtual bool getMaxIDMessage (int& max_id) = 0;

		/*!
		 * \brief Возвращает минимальный id для списка сообщений
		 * Функция полезна для работы с полной таблицей по диапазонам id
		 * \param min_id - минимальный id для таблицы со списком сообщений
		 */
		virtual bool getMinIDMessage (int& min_id) = 0;

		/*!
		 * \brief Возвращает параметры для получения списка сообщений.
		 * \param query Список форумов, поломанных веток и id сообщений для загрузки.
		 */
		virtual bool getMessageQuery (ADataQuery& query) = 0;

		/*!
		 * \brief Сохраняет список сообщений добавляя новые и обновляя полученную информацию о существующих.
		 * \param list Список сообщений, рейтингов, модерилок.
		 * \param row_version Версия строки (снимка).
		 * \param save_row_version true, для того, чтобы сохранить версии строк,
		 * false, для того, чтобы игнорировать список и не считать сохраненные сообщения за новые,
		 * добавлен для реализации workaround при вытягивании всей базы RSDN (см. IAStorage::GetMessageIds).
		 */
		virtual bool setMessageList (const ADataList& list, const ARowVersion& row_version, bool save_row_version) = 0;

		/*!
		 * \brief Возвращает количество непрочитанных сообщений для форумов
		 * в т.ч. и для спец-группы "Локальные" (см. константы SPECIAL_ID_FORUM_xxx в model/forum.h).
		 * \param list Список с количеством непрочитанных сообщений для id форумов.
		 * \param id_me ID текущего пользователя avalon.
		 */
		virtual bool getUnreadCount (AUnreadForumCountInfoList& list, int id_me) = 0;

		/*!
		 * \brief Возвращает список id топиков (родительских веток) для форума.
		 * \param id_forum ID форума.
		 * \param count Количество топиков (0 - все).
		 * \param list Список id родительских сообщений (топиков).
		 */
		virtual bool getForumTopicList (int id_forum, int count, QList<int>& list) = 0;

		/*!
		 * \brief Заполняет информацию о топиках в форуме
		 *
		 * Поля списка (кроме ID и Item) предполагаются заполненными значениями полей по умолчанию (см. AMessageInfoGUI::AMessageInfoGUI)
		 * и не меняются без необходимости.
		 *
		 * Функция ДОЛЖНА заполнить все поля AMessageInfo плюс установить флаги AMessageInfoGUI::IsRead, AMessageInfoGUI::HasUnreadChild,
		 * AMessageInfoGUI::HasUnreadChildMy в актуальные значения.
		 *
		 * Функция НЕ должна устанавливать флаг AMessageInfoGUI::IsInfoLoaded (устанавливается вызвающим кодом).
		 *
		 * Функция МОЖЕТ оставить некоторые поля AMessageInfo, которые не используются в текущем GUI, незаполненными в целях оптимизации
		 * скорости работы - в этом случае они должны иметь значения по умолчанию (к таким полям, например, может относиться поле
		 * AMessageInfo::LastModerated и т.д.)
		 *
		 * \param id_forum ID форума, для которого запрашивается список топиков
		 * (для ненулевого значения, будет произведено исключение одного лишнего преобразования из QVariant на каждый элемент,
		 * что положительно влияет на скорость)
		 * \param list Список топиков (с установленным полем ID, по которому будет производиться дальнейшее заполнение информацией).
		 * \param id_me ID текущего пользователя avalon.
		 */
		virtual bool getTopicInfoList (int id_forum, AMessageInfoGUIPtrList& list, int id_me) = 0;

		/*!
		 * \brief Заполняет информацию о всех сообщениях в топике (родительской ветке).
		 *
		 * Функция ДОЛЖНА заполнить все поля AMessageInfo плюс установить флаги AMessageInfoGUI::IsRead, AMessageInfo::HasChild в актуальные
		 * значения.
		 *
		 * Функция НЕ должна устанавливать флаг AMessageInfoGUI::IsInfoLoaded (устанавливается вызвающим кодом).
		 *
		 * Функция МОЖЕТ оставить некоторые поля AMessageInfo, которые не используются в текущем GUI, незаполненными в целях оптимизации
		 * скорости работы - в этом случае они должны иметь значения по умолчанию (к таким полям, например, может относиться поле
		 * AMessageInfo::LastModerated и т.д.)
		 *
		 * \param id_forum ID форума, для которого запрашивается список топиков
		 * (для ненулевого значения, будет произведено исключение одного лишнего преобразования из QVariant на каждый элемент,
		 * что положительно влияет на скорость)
		 * \param id_topic ID топика (темы, родительской ветки).
		 * \param list Список сообщений (изначально пустой, на выходе заполненый _не_ включая сообщение id_topic).
		 * \param factory Фабрика для создания элементов (каждый элемент списка создается вызовом IMessageInfoGUIFactory::createItem),
		 * поля (кроме Item) предполагаются заполненными значениями полей по умолчанию (см. AMessageInfoGUI::AMessageInfoGUI)
		 * и не меняются без необходимости.
		 */
		virtual bool getTopicMessageList (int id_forum, int id_topic, AMessageInfoGUIPtrList& list, IMessageInfoGUIFactory* factory) = 0;

		/*!
		 * \brief Возвращает тело сообщения.
		 * \param id_message ID сообщения.
		 * \param body Тело сообщения.
		 */
		virtual bool getMessageBody (int id_message, QString& body) = 0;

		/*!
		 * \brief Пометить группу сущностей (сообщений, форумов, групп форумов) как прочитанное/непрочитанное.
		 * Если установлена дата (.isValid() == true), то она учитывается соответственно логике:
		 * read = true - до даты как прочитанные;
		 * read = false - после даты как непрочитанные.
		 * \param list Список id сущностей.
		 * \param type Тип сущностей.
		 * \param read Флаг прочитано (true) или нет (false).
		 * \param date До даты / После даты или все, если дата не валидна.
		 */
		virtual bool setIDsAsRead (const QList<int>& list, AIDSet type, bool read, QDateTime date) = 0;

		/*!
		 * \brief Отписка от форума / группы форумов.
		 * \param list Список ID форумов, групп.
		 * \param type  {idsForum | idsGroup}.
		 * \param clean true - очистить базу от "лишних" сообщений, false не удалять сообщения.
		 */
		virtual bool unsubscribe (const QList<int>& list, AIDSet type, bool clean) = 0;

		/*!
		 * \brief Добавление сообщения в очередь на отправку.
		 * \param info Описатель сообщения на отправку.
		 */
		virtual bool addMessage2Send (const AMessage2Send& info) = 0;

		/*!
		 * \brief Возвращает список всех сообщений к отправке.
		 * \param list Список сообщений к отправке.
		 * \param drafts Флаг выбора черновиков.
		 */
		virtual bool getMessage2SendList (AMessageInfoList& list, bool drafts) = 0;

		/*!
		 * \brief Добавление рейтинга в очередь на отправку.
		 * \param info Описатель рейтинга на отправку.
		 */
		virtual bool addRating2Send (const ARating2Send& info) = 0;

		/*!
		 * \brief Возвращает список всех рейтингов к отправке со связанными сообщениями.
		 * Индексы в списках сообщений и рейтингов соответствуют.
		 * \param message_list Список оцененых сообщений.
		 * \param rating_list Список оценок.
		 */
		virtual bool getRating2SendList (AMessageInfoList& message_list, ARating2SendList& rating_list) = 0;

		/*!
		 * \brief Возвращает список всех рейтингов к отправке.
		 * \param list Список рейтингов.
		 */
		virtual bool getRating2SendList (ARating2SendList& list) = 0;

		/*!
		 * \brief Смена оценки
		 * \param id ID оценки (локальный)
		 * \param new_rate Новая оценка
		 */
		virtual bool changeRating (int id, int new_rate) = 0;

		/*!
		 * \brief Добавление модерилки в очередь на отправку.
		 * \param info Описатель модерилки.
		 */
		virtual bool addModerate2Send (const AModerate2Send& info) = 0;

		/*!
		 * \brief Возвращает список всех модерилок к отправке со связанными сообщениями.
		 * Индексы в списках сообщений и рейтингов соответствуют.
		 * \param message_list Список модерируемых сообщений.
		 * \param moderate_list Список модерилок.
		 */
		virtual bool getModerate2SendList (AMessageInfoList& message_list, AModerate2SendList& moderate_list) = 0;

		/*!
		 * \brief Возвращает список всех модерилок к отправке.
		 * \param list Список описателей модерилок.
		 */
		virtual bool getModerate2SendList (AModerate2SendList& list) = 0;

		/*!
		 * \brief Возвращает описатель модерилки к отправке
		 * \param id Локальный ID модерилки
		 * \param info Описатель модерилки
		 */
		virtual bool getModerate2SendInfo (int id, AModerate2Send& info) = 0;

		/*!
		 * \brief Коммит результата отправки.
		 * \param info Описатель с информацией о коммите.
		 */
		virtual bool setCommitResult (const ACommitInfo& info) = 0;

		/*!
		 * \brief Удаление элементов из спец-папок.
		 * \param ids Список id элементов.
		 * \param id_special ID специального форума (см. константы SPECIAL_ID_FORUM_ххх в model/forum.h).
		 */
		virtual bool deleteSpecial (const QList<int>& ids, int id_special) = 0;

		/*!
		 * \deprecated Использовался для тестирования работы MySQL с большой (~5GB) базой сообщений, для чего пришлось выкачать все сообщения RSDN
		 * \brief Возвращает существующие id сообщений в заданном интервале.
		 * Используется для вытягивания всей базы RSDN.
		 * В реализациях хранилищ, отличных от стандартного MySQL, может смело выдавать ошибку
		 * с сообщением "не реализованно", т.к. данная функциональность, скорее всего, будет скрыта от пользователя.
		 * \param from_id Начальный ID сообщения.
		 * \param to_id Конечный ID сообщения.
		 * \param list Список существующих id сообщений в заданном интервале.
		 */
		virtual bool getMessageIds (int from_id, int to_id, QList<int>& list) = 0;

		/*!
		 * \brief Получение списка рейтингов для сообщения
		 * Дополнительно см. примечания к ARatingInfo
		 * \param id_message ID сообщения
		 * \param list Список рейтингов
		 */
		virtual bool getMessageRatingList (int id_message, AMessageRatingList& list) = 0;

		/*!
		 * \brief Сжатие тел сообщений в хранилище
		 * \param progress Прогресс выполнения операции.
		 */
		virtual bool compressStorage (IProgress* progress) = 0;

		/*!
		 * \brief Распаковка тел сообщений в хранилище
		 * \param progress Прогресс выполнения операции.
		 */
		virtual bool uncompressStorage (IProgress* progress) = 0;

		/*!
		 * \brief Поиск пути сообщения
		 * \param id_message ID сообщения для поиска
		 * \param id_forum ID форума, в котором находится сообщение
		 * \param path Список, который заполняется ID сообщений от корня (корневой ID в начале списка).
		 * В случае, если сообщение не найдено, id_forum = 0 и список будет пуст
		 */
		virtual bool getMessagePath (int id_message, int& id_forum, QList<int>& path) = 0;

		/*!
		 * \brief Добавление сообщений на дозагрузку
		 * \param id_message ID сообщения
		 * \param is_topic Является ли сообщение топиком
		 */
		virtual bool addBroken (int id_message, bool is_topic) = 0;

		/*!
		 * \brief Проверка наличия поломанных веток
		 * \param result true, если есть поломанные ветки / сообщения
		 */
		virtual bool hasBroken (bool& result) = 0;

		/*!
		 * \brief Получение ID своих сообщений
		 * \param id_me ID текущего пользователя avalon
		 * \param count Количество сообщений
		 * \param list Список ID сообщений
		 */
		virtual bool getMyMessageList (int id_me, int count, QList<int>& list) = 0;

		/*!
		 * \brief Получение ID сообщений ответов себе
		 * \param id_me ID текущего пользователя avalon
		 * \param count Количество сообщений
		 * \param list Список ID сообщений
		 */
		virtual bool getAnswers2MeList (int id_me, int count, QList<int>& list) = 0;
};

#endif   // _avalon_istorage_h_
