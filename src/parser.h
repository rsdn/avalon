#ifndef _avalon_parser_h_
#define _avalon_parser_h_

/*!
 * \file parser.h
 * \brief Медоды для парсинга текста сообщений
 *
 * <b>Общее описание алгоритма парсинга</b>
 *
 * 1) Входной текст разбивается на строки, а каждая строка на
 * две части - квоту и строку после квотирования. Это необходимо для того
 * чтобы часть цитирования можно было вынести за пределы монолитного блока
 * (напрмер, тэга кода) и не включать в подсветку синтаксиса кода. (см. parseQuotes)
 *
 * 2) Набор квотированных строк разбивается на монолитные блоки, каждый из
 * которых содержит свой поднабор квотированных строк и тип/подтип моноблока.
 * Подтип моноблока используется для корректного определения способа экранирования "]"
 * (в исходном коде "]]" так и остается, а в остальных случаях заменяется одиночным "]")
 * (см. parseBlocks). Дополнительно производится "схлопывание" пустых блоков, которые
 * не несут никакой полезной информации.
 *
 * Каждый блок, который не является исходным кодом, проверяется на корректное закрытие
 * всех тэгов внешнего вида текста [b], [i], [u], [s]/[strike] и т.д. для того, чтобы форматирование
 * текста не переходило к следующему блоку, который может оказаться исходным кодом
 * У исходного же кода тэги форматирования являются частью кода и никак не должны обрабатываться.
 */

#include "sysheaders.h"

/*!
 * \brief Описатель квотированной строки
 */
typedef struct AQuotedString
{
	int     QuoteLevel; /*!< \brief Уровень квотирования (0 - нет) */
	QString QuoteText;  /*!< \brief Текст квотирования             */
	QString Data;       /*!< \brief Текст строки                   */
} AQuotedString;

/*!
 * \brief Список квотированных строк
 */
typedef QList<AQuotedString> AQuotedStringList;

/*!
 * \brief Тип монолитного блока сообщения
 */
typedef enum
{
	pbtText,      /*!< \brief Блок с простым текстом    */
	pbtCode,      /*!< \brief Блок с исходным кодом     */
	pbtAssembler, /*!< \brief Блок ассемблера           */
	pbtC,         /*!< \brief Блок си                   */
	pbtCPP,       /*!< \brief Блок c++                  */
	pbtCSharp,    /*!< \brief Блок c#                   */
	pbtMSIL,      /*!< \brief Блок MSIL                 */
	pbtIDL,       /*!< \brief Блок IDL/MIDL             */
	pbtPascal,    /*!< \brief Блок Pascal/Delphi        */
	pbtBasic,     /*!< \brief Блок Basic/Visual Basic   */
	pbtSQL,       /*!< \brief Блок SQL/TSQL/PSQL        */
	pbtPerl,      /*!< \brief Блок Perl                 */
	pbtPHP,       /*!< \brief Блок PHP                  */
	pbtJava,      /*!< \brief Блок Java                 */
	pbtXML,       /*!< \brief Блок XML/XSL              */
	pbtLisp,      /*!< \brief Блок Lisp                 */
	pbtHaskell,   /*!< \brief Блок Haskell              */
	pbtErlang,    /*!< \brief Блок Erlang               */
	pbtOCaml,     /*!< \brief Блок OCaml                */
	pbtProlog,    /*!< \brief Блок Prolog               */
	pbtPython,    /*!< \brief Блок Python               */
	pbtRuby,      /*!< \brief Блок Ruby                 */
	pbtNemerle,   /*!< \brief Блок Nemerle              */
	pbtTagline,   /*!< \brief Блок подписи              */
	pbtModerator, /*!< \brief Блок сообщения модератора */
	pbtQuote,     /*!< \brief Блок цитаты               */
	pbtTable,     /*!< \brief Блок таблицы              */
	pbtCut        /*!< \brief Блок cut                  */
} AParsedBlockType;

/*
 * \brief Подтип монолитного блока сообщения
 * Флаг необходим из за разных алгоритмов обработки экранирующего [[
 * (см. http://www.rsdn.ru/forum/test/4217775.1.aspx - в блоке исходного кода экранирование работать не должно),
 * а так же, возможно, для новых тэгов, требующих специальной обработки
 */
typedef enum
{
	pbstText,         /*!< \brief Простой текст - "]" экранируется    */
	pbstSourceCode,   /*!< \brief Исходный код  - "]" не экранируется */
	pbstBlock         /*!< \brief Блок блоков                         */
} AParsedBlockSubType;

/*!
 * \brief Описатель монолитного блока сообщения (см. ниже)
 */
struct AParsedBlock;

/*!
 * \brief Список разобранных монолитных блоков
 */
typedef QList<AParsedBlock> AParsedBlockList;

/*!
 * \brief Описатель монолитного блока сообщения
 */
typedef struct AParsedBlock
{
	AParsedBlockType    Type;      /*!< \brief Тип монолитного блока          */
	AParsedBlockSubType SubType;   /*!< \brief Подтип монолитного блока       */
	QString             Title;     /*!< \brief Заголовок блока (для тэга cut) */
	AQuotedStringList   Strings;   /*!< \brief Список квотированных строк     */
	AParsedBlockList    SubBlocks; /*!< \brief Список подблоков               */
} AParsedBlock;

/*!
 * \brief Класс парсера BB сообщений
 */
class AParser
{
	public:

		/*!
		 * \brief Парсинг одиночной строки на квотированную часть и данные
		 * \param line Исходная строка
		 * \return Строка разделенная на квоту и данные
		 * из квоты удаляются пробелы, данные не изменяются
		 */
		static AQuotedString parseQuote (const QString& line);

		/*!
		 * \brief Парсинг списка строк на квотированную часть и данные
		 * \param source Исходный текст
		 * \return Строки разделенные на квоту и данные
		 * (список строк после обработки каждой вызовом parseQuote),
		 */
		static AQuotedStringList parseQuotes (const QString& source);

		/*!
		 * \brief Парсинг списка строк на монолитные блоки
		 * \param source Исходный текст
		 * \return Список описателей монолитных блоков
		 */
		static AParsedBlockList parseBlocks (const QString& source);
};

#endif   // _avalon_parser_h_
