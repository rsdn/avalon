/*!
 * \file
 * \brief Модели для работы с рейтингом
 */

#ifndef _avalon_rating_info_h_
#define _avalon_rating_info_h_

#include "sysheaders.h"

/*!
 * \brief Описатель рейтинга
 *
 * Не смотря на то, что для поля Rate определено 7 возможных значений,
 * на практике оказывается, что веб-сервис далеко не всегда отдает значения в "корректном" диапазоне
 * и за имеющуюся историю оценок (на 10.03.2009) было выставлено 10 оценок не входящих в диапазон,
 * а так же, оценка "+1" отсутствует как класс :\
 *
 * Проектное решение состоит в сохранении в хранилище любой оценки вне зависимости от ее значения
 * (следуя принципу максимально длительного невмешательства в поток данных), при выборке оценок
 * (см. IAStorage::getMessageRatingList) хранилище так же обязуется вернуть значения рейтингов "как есть"
 * Фильтрация диапазона производится при формировании отображения сообщения (см. AFormatter::parse).
 */
typedef struct ARatingInfo
{
	int       IDMessage;  /*!< \brief ID сообщения         */
	int       IDTopic;    /*!< \brief ID топика            */
	int       IDUser;     /*!< \brief ID пользователя      */
	int       UserRating; /*!< \brief Рейтинг пользователя */
	int       Rate;       /*!< \brief "+1" = -3, "1" = 1, "2" = 2, "3" = 3, "+" = -4, "-" = 0, ";)" = -2 */
	QDateTime RateDate;   /*!< \brief Дата                 */
} ARatingInfo;

/*!
 * \brief Список рейтингов
 */
typedef QList<ARatingInfo> ARatingInfoList;

/*!
 * \brief Описатель рейтинга к отправке
 */
typedef struct ARating2Send
{
	int       ID;        /*!< \brief ID локальный */
	int       IDMessage; /*!< \brief ID сообщения */
	int       Rate;      /*!< \brief Оценка       */
	QDateTime Date;      /*!< \brief Дата         */
} ARating2Send;

/*!
 * \brief Список рейтингов к отправке
 */
typedef QList<ARating2Send> ARating2SendList;

/*!
 * \brief Описатель оценки сообщения/топика
 */
typedef struct AMessageRating
{
	int     IDUser;     /*!< \brief ID пользователя      */
	QString Name;       /*!< \brief Логин пользователя   */
	QString Nick;       /*!< \brief Ник                  */
	int     UserRating; /*!< \brief Рейтинг пользователя */
	int     Rate;       /*!< \brief "+1" = -3, "1" = 1, "2" = 2, "3" = 3, "+" = -4, "-" = 0, ";)" = -2 (дополнительно см. примечания к ARatingInfo) */
} AMessageRating;

/*!
 * \brief Список оценок для сообщения/топика
 */
typedef QList<AMessageRating> AMessageRatingList;

/*!
 * \brief Константа для типа оценки "Смайл"
 */
const int SPECIAL_RATE_TYPE_SMILE = 0;

/*!
 * \brief Константа для типа оценки "Согласен" / "Не согласен"
 */
const int SPECIAL_RATE_TYPE_PLUS_MINUS = 1;

/*!
 * \brief Константа для числового типа оценки
 */
const int SPECIAL_RATE_TYPE_NUMBER = 2;

#endif
