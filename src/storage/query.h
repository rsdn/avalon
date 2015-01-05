/*!
 * \file
 * \brief Обертка для работы с SQL запросами
 */

#ifndef _avalon_query_h_
#define _avalon_query_h_

#include "sysheaders.h"

/*!
 * \brief Обертка для работы с SQL запросами
 */
class AQuery : public QSqlQuery
{
	public:

		/*!
		 * \brief Создание объекта запроса
		 * \param database Qt объект работы с БД
		 * \param sql текст SQL запроса
		 */
		AQuery  (QSqlDatabase* database, const QString& sql = QString());
		~AQuery ();

		/*!
		 * \brief Получение текста последней ошибки
		 * \return возвращает строку с последней ошибкой на основе информации из драйвера
		 */
		QString getLastError ();

		/*!
		 * \brief Отображение текста последней ошибки в диалоговом окне
		 * \param parent родительский виджет для диалогового окна
		 */
		void showError (QWidget* parent);

		/*!
		 * \brief Перегрузка QSqlQuery::exec для возможности вызова не prepared запросов
		 */
		bool exec ();

	private:

		/*!
		 * \brief Текст запроса для не prepared запросов (текст из конструктора)
		 */
		QString m_sql;
};

#endif   // _avalon_query_h_
