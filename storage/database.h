/*!
 * \file
 * \brief Обертка для работы с БД
 */

#ifndef _avalon_database_h_
#define _avalon_database_h_

#include "query.h"
#include "database_error.h"

/*!
 * \brief Класс-обертка для работы с БД
 */
class ADatabase :
	public ADatabaseError, // общий класс контроля ошибок
	public QSqlDatabase    // Qt класс для работы с БД
{
	public:

		/*!
		 * \brief Конструктор объекта для работы с БД
		 * \param type Тип БД, для MySQL type = "QMYSQL" (см. документацию Qt)
		 */
		ADatabase  (const QString& type);
		~ADatabase ();

		/*!
		 * \brief Создание объекта для запроса данных
		 * \param sql Строка запроса
		 * \return Объект для запроса данных
		 */
		AQuery* createQuery (const QString& sql = QString());

		/*!
		 * \brief Создание объекта для запроса данных с параметрами
		 * \param sql Строка запроса с параметрами, параметры задаются в виде ":имя_параметра"
		 * \return Объект для запроса данных
		 */
		AQuery* createPreparedQuery (const QString& sql);

	protected:

		/*!
		 * \brief Формирует строку с последней ошибкой на основании информации из драйвера БД (см. ADatabaseError)
		 */
		void setLastError ();

	private:

		/*!
		 * \brief Перегрузка соответствующего метода QSqlDatabase::close
		 * Запрещено к вызову вследствии того, что при наличии prepared запросов
		 * закрытие соединения приводит к неопределенному поведению (в частности,
		 * утечкам памяти). Вызывается из деструктора - к этому моменту экземпляров
		 * AQuery оставаться в теории не должно.
		 */
		void close () { QSqlDatabase::close(); }
};

#endif
