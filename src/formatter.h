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

		/*!
		 * \brief Проверка является ли текст URL
		 * \param text Текст для проверки
		 * \return 0, если текст не является ссылкой, 1 - если текст является ссылкой, 2 - если текст является "кривой" ссылкой и требует добавления "http://"
		 */
		static int isURL (const QString& text);

	private:

		/*!
		 * \brief Форматирование блока текста
		 * \param block Описатель блока
		 * \return Строка html для вставки
		 */
		static QString formatParsedBlock (const AParsedBlock& block);

		/*!
		 * \brief Форматирование списка квотированных строк
		 * \param list Список строк
		 * \param sub_type Подтип блока строк
		 * \return Строка html для вставки
		 */
		static QString formatQuotedStringList (const AQuotedStringList& list, AParsedBlockSubType sub_type);

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
