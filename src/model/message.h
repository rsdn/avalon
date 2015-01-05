/*!
 * \file
 * \brief Модели для работы с сообщениями
 */

#ifndef _avalon_message_info_h_
#define _avalon_message_info_h_

#include "forum.h"
#include "rating.h"
#include "moderate.h"

/*!
 * \brief Описатель сообщения
 */
typedef struct AMessageInfo
{
	int       ID;               /*!< \brief ID сообщения                                                    */
	int       IDTopic;          /*!< \brief ID темы сообщения или 0, если сообщение начинает тему           */
	int       IDParent;         /*!< \brief ID родителя или 0, если сообщение начинает тему                 */
	int       IDUser;           /*!< \brief ID пользователя                                                 */
	int       IDForum;          /*!< \brief ID форума                                                       */
	QString   Subject;          /*!< \brief Тема                                                            */
	QString   MessageName;      /*!< \brief Название статьи                                                 */
	QString   UserNick;         /*!< \brief Ник пользователя                                                */
	QString   Message;          /*!< \brief Содержимое сообщения                                            */
	int       IDArticle;        /*!< \brief ID статьи или 0                                                 */
	QDateTime MessageDate;      /*!< \brief Дата сообщения                                                  */
	QDateTime UpdateDate;       /*!< \brief Дата обновления сообщения или "1970-01-01 00:00:00"             */
	QString   UserRole;         /*!< \brief Роль пользователя - Anonym, Expert, Moderator, TeamMember, User */
	QString   UserTitle;        /*!< \brief Повязка пользователя (число для анонима, или наименование)      */
	int       UserTitleColor;   /*!< \brief Цвет повязки                                                    */
	QDateTime LastModerated;    /*!< \brief Дата последней модерилки или "1970-01-01 00:00:00"              */
	bool      Closed;           /*!< \brief Флаг закрытия темы                                              */
} AMessageInfo;

/*!
 * \brief Список сообщений
 */
typedef QList<AMessageInfo> AMessageInfoList;

/*!
 * \brief Объединенная структура для запроса списка сообщений к вебсервису
 */
typedef struct ADataQuery
{
	ASubscribedForumInfoList Forum;           /*!< \brief Список форумов для синхронизации */
	QList<int>               BrokenMessage;   /*!< \brief Список ID сообщений для загрузки */
	QList<int>               BrokenTopic;     /*!< \brief Список ID оборванных топиков     */
} ADataQuery;

/*!
 * \brief Объединенная структура для синхронизации списка сообщений от вебсервиса
 */
typedef struct ADataList
{
	AMessageInfoList  Message;    /*!< \brief Сообщения */
	ARatingInfoList   Rating;     /*!< \brief Рейтинги  */
	AModerateInfoList Moderate;   /*!< \brief Модерилки */
} ADataList;

/*!
 * \brief Расширенная информация о сообщении для отображения в виджете дерева сообщений
 */
typedef struct AMessageInfoGUI : AMessageInfo
{
	bool IsInfoLoaded;         /*!< \brief Загружена ли какая-нибудь информация кроме ID */
	bool IsRead;               /*!< \brief Прочитано ли сообщение                        */
	bool HasChild;             /*!< \brief Имеются ли дочерние сообщения                 */
	bool IsBodyLoaded;         /*!< \brief Загружено ли тело сообщения                   */
	bool IsChildLoaded;        /*!< \brief Загружены ли дочерние сообщения               */
	bool HasUnreadChild;       /*!< \brief Наличие непрочитанных дочерних                */
	bool HasUnreadChildMy;     /*!< \brief Наличие непрочитанных дочерних мне            */
	int  UnreadChildCount;     /*!< \brief Количество непрочитанных дочерних             */
	int  UnreadChildCountMy;   /*!< \brief Количество непрочитанных дочерних мне         */
	int  Special;              /*!< \brief Спец-данные                                   */

	QTreeWidgetItem* Item;     /*!< \brief Слинкованный элемент дерева                   */

	AMessageInfoGUI ()
	{
		// AMessageInfo
		ID             = -1;
		IDTopic        = -1;
		IDParent       = -1;
		IDUser         = -1;
		IDForum        = -1;
		IDArticle      = -1;
		UserTitleColor = -1;

		// AMessageInfoGUI
		IsInfoLoaded       = false;
		IsRead             = true;
		HasChild           = false;
		IsBodyLoaded       = false;
		IsChildLoaded      = false;
		HasUnreadChild     = false;
		HasUnreadChildMy   = false;
		UnreadChildCount   = -1;
		UnreadChildCountMy = -1;
		Special            = -1;

		Item = NULL;
	}

} AMessageInfoEx;

/*!
 * \brief Список сообщений для отображения в дереве в GUI
 */
typedef QList<AMessageInfoGUI> AMessageInfoGUIList;

/*!
 * \brief Список сообщений для отображения в дереве в GUI
 */
typedef QList<AMessageInfoGUI*> AMessageInfoGUIPtrList;

/*!
 * \brief Интерфейс для фабрики элементов дерева отображения сообщений (см. IAStorage::getTopicMessageList)
 * Необходим для уменьшения расходов врмени на копирование информации при загрузке большого топика
 */
class IMessageInfoGUIFactory
{
	public:

		/*!
		 * \brief Метод для создания GUI-элемента дерева сообщений
		 * \return Указатель на готовый к заполнению данными элемент (поле Item устанавливается в валидное при создании)
		 */
		virtual AMessageInfoGUI* createItem () = 0;
};

/*!
 * \brief Описатель сообщения к отправке
 */
typedef struct AMessage2Send
{
	int       ID;         /*!< \brief Локальный id сообщения */
	int       IDParent;   /*!< \brief ID родителя            */
	int       IDForum;    /*!< \brief ID форума              */
	QString   Subject;    /*!< \brief Тема                   */
	QString   Message;    /*!< \brief Сообщение              */
	QDateTime Date;       /*!< \brief Дата                   */
	bool      Draft;      /*!< \brief Флаг черновика         */
} AMessage2Send;

/*!
 * \brief Список сообщений к отправке
 */
typedef QList<AMessage2Send> AMessage2SendList;

#endif   // _avalon_message_info_h_
