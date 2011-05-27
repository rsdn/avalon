/*!
 * \file
 * \brief Элемент (лист) древовидных виджетов
 *
 * $Date: 2008-11-16 00:09:58 +0300 (Вск, 16 Ноя 2008) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 130 $
 * $URL: svn://opensvn.ru/avalon/trunk/tree_widget_item.h $
 */

#ifndef _avalon_tree_widget_item_h_
#define _avalon_tree_widget_item_h_

#include "sysheaders.h"

/*!
 * \brief Обертка для QTreeWidgetItem упрощающая навигацию по элементам дерева
 * используется в дереве форумов, сообщений.
 * Изначально класс разрабатывался для дерева форумов, и при попытке его использования для дерева сообщений
 * выявились некоторые проблемы в связи с тем, что дерево может быть не загружено полностью - требуется адаптация
 */
class ATreeWidgetItem : public QTreeWidgetItem
{
	public:

		/*!
		 * \brief Стандартный конструктор QTreeWidgetItem (см. документацию к Qt)
		 */
		ATreeWidgetItem () : QTreeWidgetItem (0) {}

		/*!
		 * \brief Стандартный конструктор QTreeWidgetItem (см. документацию к Qt)
		 */
		ATreeWidgetItem (QTreeWidget* parent) : QTreeWidgetItem (parent) {}

		/*!
		 * \brief Стандартный конструктор QTreeWidgetItem (см. документацию к Qt)
		 */
		ATreeWidgetItem (QTreeWidgetItem* parent) : QTreeWidgetItem (parent) {}

	//static:

		/*!
		 * \brief Получение предыдущего элемента в порядке обхода дерева
		 * \param current указатель на элемент относительно которого производится поиск
		 * \return Указатель на предыдущий элемент или NULL, если это первый корень дерева
		 */
		static QTreeWidgetItem* prevItem (QTreeWidgetItem* current);

		/*!
		 * \brief Получение следующего элемента в порядке обхода дерева
		 * \param current указатель на элемент относительно которого производится поиск
		 * \return Указатель на следующий элемент или NULL, если это последний лист дерева
		 */
		static QTreeWidgetItem* nextItem (QTreeWidgetItem* current);

		/*!
		 * \brief Получение корневого родительского элемента
		 * \param current указатель на элемент относительно которого производится поиск
		 * \return Указатель на корневой родительский элемент (или самого себя, если это корневой элемент)
		 */
		static QTreeWidgetItem* rootItem (QTreeWidgetItem* current);

		/*!
		 * \brief Получение первого (самого высокого) листа (элемента без дочерних)
		 * \param current указатель на элемент относительно которого производится поиск
		 * \return Указатель первый лист
		 */
		static QTreeWidgetItem* firstLeaf (QTreeWidgetItem* current);

		/*!
		 * \brief Получение последнего (самого нижнего) листа (элемента без дочерних)
		 * \param current указатель на элемент относительно которого производится поиск
		 * \return Указатель последний лист
		 */
		static QTreeWidgetItem* lastLeaf (QTreeWidgetItem* current);

		/*!
		 * \brief Получение первого дочернего элемента
		 * \param current указатель на элемент относительно которого производится поиск
		 * \return Указатель на первый дочерний элемент или NULL
		 */
		static QTreeWidgetItem* firstChild (QTreeWidgetItem* current);

		/*!
		 * \brief Получение последнего дочернего элемента
		 * \param current указатель на элемент относительно которого производится поиск
		 * \return Указатель на последний дочерний элемент или NULL
		 */
		static QTreeWidgetItem* lastChild (QTreeWidgetItem* current);

		/*!
		 * \brief Получение первого "брата"
		 * \param current указатель на элемент относительно которого производится поиск
		 * \return Указатель на первого "брата" (может быть сам текущий элемент)
		 */
		static QTreeWidgetItem* firstSibling (QTreeWidgetItem* current);

		/*!
		 * \brief Получение последнего "брата"
		 * \param current указатель на элемент относительно которого производится поиск
		 * \return Указатель на последнего "брата" (может быть сам текущий элемент)
		 */
		static QTreeWidgetItem* lastSibling (QTreeWidgetItem* current);

		/*!
		 * \brief Получение предыдущего "брата"
		 * \param current указатель на элемент относительно которого производится поиск
		 * \return Указатель на предыдущего "брата" или NULL
		 */
		static QTreeWidgetItem* prevSibling (QTreeWidgetItem* current);

		/*!
		 * \brief Получение следующего "брата"
		 * \param current указатель на элемент относительно которого производится поиск
		 * \return Указатель на следующего "брата" или NULL
		 */
		static QTreeWidgetItem* nextSibling (QTreeWidgetItem* current);
};

/*!
 * \brief Расширенный элемент дерева, хранящий дополнительную информацию,
 * используется в дереве форумов, сообщений
 */
template <class T> class ATreeWidgetItemT : public ATreeWidgetItem
{
	public:

		/*!
		 * \brief Стандартный конструктор QTreeWidgetItem (см. документацию к Qt)
		 */
		ATreeWidgetItemT () : ATreeWidgetItem () {}

		/*!
		 * \brief Стандартный конструктор QTreeWidgetItem (см. документацию к Qt)
		 */
		ATreeWidgetItemT (QTreeWidget* parent) : ATreeWidgetItem (parent) {}

		/*!
		 * \brief Стандартный конструктор QTreeWidgetItem (см. документацию к Qt)
		 */
		ATreeWidgetItemT (QTreeWidgetItem* parent) : ATreeWidgetItem (parent) {}

		/*!
		 * \brief Возвращает копию экземпляра обернутого класса
		 */
		T tag ()
		{
			return m_tag;
		}

		/*!
		 * \brief Возвращает указатель на экземпляр обернутого класса
		 */
		T* pag ()
		{
			return &m_tag;
		}

		/*!
		 * \brief Устанавливает экземпляр обернутого класса
		 * \param value Экземпляр класса
		 */
		void setTag (T value)
		{
			m_tag = value;
		}

	private:

		/*!
		 * \brief Экземпляр обернутого класса
		 */
		T m_tag;
};

#endif
