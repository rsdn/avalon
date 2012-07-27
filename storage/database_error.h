/*!
 * \file
 * \brief Обертка для контроля ошибок при работе с БД
 */

#ifndef _avalon_database_error_h_
#define _avalon_database_error_h_

#include "../sysheaders.h"

/*!
 * \brief Обертка для контроля и отображения ошибок при работе с БД
 */
class ADatabaseError
{
	public:

		ADatabaseError  ();
		~ADatabaseError ();

		/*!
		 * \brief Возвращает текст последней ошибки
		 * \return Форматированный текст последней ошибки
		 */
		QString getLastError () const;

		/*!
		 * \brief Отображает диалоговое окно с информацией о последней ошибке
		 * \param parent Родительский виджет для диалогового окна
		 */
		void showError (QWidget* parent);

	protected:

		/*!
		 * \brief Устанавливает текст последней ошибки
		 * \param message Текст ошибки
		 */
		void setLastError (const QString& message);

	private:

		/*!
		 * \brief Текст последней ошибки
		 */
		QString m_error_message;
};

#endif
