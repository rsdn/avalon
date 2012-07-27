/*!
 * \file
 * \brief Константы для библиотек подсветки синтаксиса
 */

#ifndef _avalon_colorer_h_
#define _avalon_colorer_h_

/*!
 * \brief Enscript [http://www.gnu.org/software/enscript/enscript.html]
 */
#define COLORER_ENSCRIPT_NAME "Enscript"

/*!
 * \brief Colorer take 5 [http://colorer.sourceforge.net/]
 */
#define COLORER_COLORER_TAKE5_NAME "Colorer take 5"

/*!
 * \brief GNU Source-highlight [http://www.gnu.org/software/src-highlite/]
 */
#define COLORER_GNU_SOURCE_HIGHLIGHT_NAME "GNU Source-highlight"

/*!
 * \brief Highlight.js [http://softwaremaniacs.org/soft/highlight/]
 */
#define COLORER_HIGHLIGHT_JS_NAME "Highlight.js"

/*!
 * \brief Перечисление возможных движков подсветки синтаксиса
 */
typedef enum AColorer
{
	acFirst,              /*!< \brief  Первый (неопределенный)                                          */
	acEnscript,           /*!< \brief  Enscript [http://www.gnu.org/software/enscript/enscript.html]    */
	acColorerTake5,       /*!< \brief  Colorer take 5 [http://colorer.sourceforge.net/]                 */
	acGNUSourceHighlight, /*!< \brief  GNU Source-highlight [http://www.gnu.org/software/src-highlite/] */
	acHighlightJS,        /*!< \brief  Highlight.js [http://softwaremaniacs.org/soft/highlight/]        */
	acLast                /*!< \brief  Последний (по счету)                                             */
} AColorer;

/*!
 * \brief Движок подсветки по умолчанию
 */
#define COLORER_DEFAULT acHighlightJS

/*!
 * \brief Имя движка подсветки по умолчанию
 */
#define COLORER_DEFAULT_NAME COLORER_HIGHLIGHT_JS_NAME

#endif   // _avalon_colorer_h_
