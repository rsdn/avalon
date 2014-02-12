/*!
 * \file
 * \brief Форматтер текста сообщения для отображения
 */

#ifndef _avalon_formatter_h_
#define _avalon_formatter_h_

#include "parser.h"
#include "model/all.h"

/*!
 * \brief Форматтер текста сообщения для отображения
 */
class AFormatter
{
	public:

		/*!
		 * \brief Форматтер HTML сообщения из текстового
		 * \param message Дескриптор сообщения
		 * \param special Флаг спец-форума
		 * \param rated Флаг того, что форум оценивается
		 * \param rating_list Список рейтингов (если есть)
		 * \return Строка с HTML кодом для отображения
		 */
		static QString formatMessage (const AMessageInfo& message, bool special = false, bool rated = false, const AMessageRatingList* rating_list = NULL);

		/*!
		 * \brief Нормализация тела сообщения - выправление криворукого квотинга, ссылок, приветствий
		 * \param body Тело сообщения
		 * \param nick Ник автора
		 * \return Строка с нормализованным телом сообщения
		 */
		static QString normalizeBody (const QString& body, const QString& nick);

	private:

		/*!
		 * \brief Блок заголовочного html / head / body
		 * \return Строка html для вставки
		 */
		static QString headHTML ();

		/*!
		 * \brief Блок темы сообщения
		 * \param id ID сообщения
		 * \param special Флаг того, что сообщение в спец-форуме
		 * \param subject Тема сообщения
		 * \return Строка html для вставки
		 */
		static QString subjectHTML (int id, bool special, const QString subject);

		/*!
		 * \brief Блок возможности выставлять оценки
		 * \param special Флаг того, что сообщение в спец-форуме
		 * \param rated Флаг того, что на форуме разрешены оценки
		 * \return Строка html для вставки
		 */
		static QString rateHTML (bool special, bool rated);

		/*!
		 * \brief Блок информации об авторе сообщения
		 * \param id ID пользователя
		 * \param nick Ник пользователя
		 * \return Строка html для вставки
		 */
		static QString authorHTML (int id, const QString nick);

		/*!
		 * \brief Блок поставленных оценок
		 * \param id ID сообщения
		 * \param rating_list Список оценок
		 * \return Строка html для вставки
		 */
		static QString ratingHTML (int id, const AMessageRatingList* rating_list);

		/*!
		 * \brief Блок подвала html
		 * \return Строка html для вставки
		 */
		static QString footerHTML ();

		/*!
		 * \brief Форматирование блока текста
		 * \param block Описатель блока
		 * \return Строка html для вставки
		 */
		static QString formatParsedBlock (const AParsedBlock& block);

		/*!
		 * \brief Форматирование списка квотированных строк
		 * \param list Список строк
		 * \param type Тип блока строк
		 * \param sub_type Подтип блока строк
		 * \return Строка html для вставки
		 */
		static QString formatQuotedStringList (const AQuotedStringList& list, AParsedBlockType type, AParsedBlockSubType sub_type);

		/*!
		 * \brief Форматирование блока текста
		 * \param text Текст
		 * \return Строка html для вставки
		 */
		static QString formatSimpleText (const QString& text);

		/*!
		 * \brief Форматирование гиперссылок в блоке текста
		 * \param text Текст
		 * \return Строка html для вставки
		 */
		static QString formatHyperlinks (const QString& text);

		/*!
		 * \brief Создание аббриветауры ника из полного имени
		 * \param nick Ник
		 * \return 1-3 символа
		 */
		static QString nick3 (const QString& nick);

		/*!
		 * \brief Форматтер HTML сообщения из куска простого текста (без блоков кода)
		 * \param text Текст блока
		 * \param message Описатель сообщения
		 * \return Строка с HTML кодом для отображения
		 */
		static QString processSimpleText (const QString& text, const AMessageInfo& message);
};

#endif   // _avalon_formatter_h_
