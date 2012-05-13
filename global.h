/*!
 * \file
 * \brief Заголовочный файл для глобальных настроек
 *
 * $Date: 2010-08-26 06:32:00 +0400 (Чтв, 26 Авг 2010) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 359 $
 * $URL: svn://opensvn.ru/avalon/trunk/global.h $
 */

#ifndef _avalon_global_h_
#define _avalon_global_h_

#include "colorer.h"
#include "model/user.h"
#include "logger.h"

extern Logger g_logger;

/*!
 * \brief Глобальные настройки
 */
class AGlobal
{
	private:

		AGlobal ();

	public:

		/*!
		 * \brief Стандартный singleton
		 */
		static AGlobal* getInstance ();

		~AGlobal ();

		/*!
		 * \brief Перезагрузка настроек
		 */
		void reload ();

		//
		// Общие переменные
		//

		AUserInfo Me;          /*!< \brief Текущий пользователь avalon */
		AColorer  Colorer;     /*!< \brief Движок подсветки синтаксиса */
#ifdef AVALON_USE_ZLIB
		bool      Compression; /*!< \brief Сжатие тел сообщений        */
#endif

		//
		// Константы форматирования
		//

		QString AnonymousName; /*!< \brief Отображаемое имя анонима */
		QString DateFormat;    /*!< \brief Формат даты/времени      */

		/*!
		 * \brief Список временных файлов для работы (пока только для файлов graphviz)
		 */
		QList<QTemporaryFile*> TempFileList;

		/*!
		 * \brief Очистка (удаление) временных файлов
		 */
		void clearTempList ();
};

#endif   // _avalon_global_h_
