/*!
 * \file
 * \brief Обертка для работы с MySQL
 *
 * $Date: 2011-04-20 17:27:00 +0400 (Срд, 20 Апр 2011) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 419 $
 * $URL: svn://opensvn.ru/avalon/trunk/storage/mysql_database.h $
 */

#ifndef _avalon_mysql_database_h_
#define _avalon_mysql_database_h_

#include "database.h"

/*!
 * \brief Обертка для работы с MySQL
 */
class AMySQLDatabase : public ADatabase
{
	public:

		AMySQLDatabase  ();
		~AMySQLDatabase ();

		/*!
		 * \brief Проверяет открыто ли соединение, если нет, то пытается его открыть
		 * \return true - успешное открытие, иначе см. описание ADatabaseError
		 */
		bool checkDatabase ();

		/*!
		 * \brief Начало транзакции с автооткрытием соединения
		 * \return true - успешное, иначе см. описание ADatabaseError
		 */
		bool transaction ();

		/*!
		 * \brief Принятие транзакции
		 * Важно! В случае неудачи операции commit внутри вызывается автоматический rollback
		 * (т.е. дополнительный rollback в случае неудачи commit не требуется).
		 * \return true - успешное, иначе см. описание ADatabaseError
		 */
		bool commit ();

		/*!
		 * \brief Откат транзакции
		 * \return true - успешный, иначе см. описание ADatabaseError
		 */
		bool rollback ();

		/*!
		 * \brief Cоздание объекта запроса
		 * \param sql SQL текст запроса
		 * \param prepared true, если запрос с параметрами и false для простых запросов
		 * \return объект или NULL в случае ошибки (см. описание ADatabaseError)
		 */
		AQuery* createQuery (const QString& sql, bool prepared = true);

	private:

		/*!
		 * \brief Флаг наличия транзакции (true - транзакция открыта)
		 */
		bool m_transaction;

		/*!
		 * \brief Открытие соединения
		 * \return true - успешное, иначе см. описание ADatabaseError
		 */
		bool openDatabase  ();
};

#endif
