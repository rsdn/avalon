/*!
 * \file
 * \brief Интерфейс для работы с деревом сообщений
 */

#ifndef _avalon_imessage_tree_h_
#define _avalon_imessage_tree_h_

#include "../model/all.h"

/*!
 * \brief Интерфейс для работы с деревом сообщений
 */
class IMessageTree
{
	public:

		virtual ~IMessageTree () {}

		/*!
		 * \brief Cмена форума, NULL - очистить список (указатель на группе)
		 * \param info Описатель форума
		 */
		virtual void changeForum (const AForumInfo* info) = 0;

		/*!
		 * \brief Отображение сырого текста выделенного сообщения
		 */
		virtual void showSource () = 0;

		/*!
		 * \brief Установка оценки/модерилки и обработка других спец-линков
		 * \param url Строка URL запроса из тела сообщения
		 */
		virtual void processUrl (const QString& url) = 0;

		/*!
		 * \brief Перейти к следующему непрочитанному сообщению
		 */
		virtual void gotoNextUnreadArticle () = 0;

		/*!
		 * \brief Перейти к следующей непрочитанной ветке
		 */
		virtual void gotoNextUnreadThread () = 0;

		/*!
		 * \brief Возвращает путь к текущему выделенному сообщению от корня (см. selectByPath)
		 * \param path Список, который заполняется ID сообщений от корня (корневой ID в начале списка)
		 */
		virtual void getSelectedPath (QList<int>& path) = 0;

		/*!
		 * \brief Переместить текущее выделение на сообщение с заданным путем
		 * \param path Путь к сообщению от корня (список ID сообщений, корневой элемент первый в списке)
		 * \return true, если есть выделение, false если сообщение с заданным путем не найдено
		 * или находится вне зоны загруженных сообщений - в этом случае будет выделено максимально близкое по дереву
		 */
		virtual bool selectByPath (const QList<int>* path) = 0;

		/*!
		 * \brief Вернуться назад
		 */
		virtual void gotoBackward () = 0;

		/*!
		 * \brief Перейти вперед
		 */
		virtual void gotoForward () = 0;

		/*!
		 * \brief Ответить на сообщение
		 */
		virtual void reply () = 0;
};

#endif
