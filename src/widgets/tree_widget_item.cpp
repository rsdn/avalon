#include "tree_widget_item.h"
//----------------------------------------------------------------------------------------------

QTreeWidgetItem* ATreeWidgetItem::prevItem (QTreeWidgetItem* current)
{
	QTreeWidgetItem* item = prevSibling(current);

	if (item != NULL)
		return lastLeaf(item);

	item = current->parent();

	if (item != NULL)
		return item;

	return NULL;
}
//----------------------------------------------------------------------------------------------

QTreeWidgetItem* ATreeWidgetItem::nextItem (QTreeWidgetItem* current)
{
	QTreeWidgetItem* item = firstChild(current);

	if (item != NULL)
		return item;

	item = nextSibling(current);

	if (item != NULL)
		return item;

	item = current->parent();

	if (item != NULL)
	{
		QTreeWidgetItem* next_item = nextSibling(item);

		while (next_item == NULL)
		{
			item = item->parent();

			if (item == NULL)
				return NULL;

			next_item = nextSibling(item);
		}

		return next_item;
	}

	return NULL;
}
//----------------------------------------------------------------------------------------------

QTreeWidgetItem* ATreeWidgetItem::rootItem (QTreeWidgetItem* current)
{
	QTreeWidgetItem* current_item = current;
	QTreeWidgetItem* parent_item  = current_item->parent();

	while (parent_item != NULL)
	{
		current_item = parent_item;
		parent_item = current_item->parent();
	}

	return current_item;
}
//----------------------------------------------------------------------------------------------

QTreeWidgetItem* ATreeWidgetItem::firstLeaf (QTreeWidgetItem* current)
{
	QTreeWidgetItem* current_item = current;

	while (current_item->childCount() != 0)
		current_item = current_item->child(0);

	return current_item;
}
//----------------------------------------------------------------------------------------------

QTreeWidgetItem* ATreeWidgetItem::lastLeaf (QTreeWidgetItem* current)
{
	QTreeWidgetItem* current_item = current;

	int child_count = current_item->childCount();

	while (child_count != 0)
	{
		current_item = current_item->child(child_count - 1);
		child_count = current_item->childCount();
	}

	return current_item;
}
//----------------------------------------------------------------------------------------------

QTreeWidgetItem* ATreeWidgetItem::firstChild (QTreeWidgetItem* current)
{
	if (current->childCount() == 0)
		return NULL;

	return current->child(0);
}
//----------------------------------------------------------------------------------------------

QTreeWidgetItem* ATreeWidgetItem::lastChild (QTreeWidgetItem* current)
{
	int child_count = current->childCount();

	if (child_count == 0)
		return NULL;

	return current->child(child_count - 1);
}
//----------------------------------------------------------------------------------------------

QTreeWidgetItem* ATreeWidgetItem::firstSibling (QTreeWidgetItem* current)
{
	QTreeWidgetItem* parent_item = current->parent();

	if (parent_item == NULL)
		return current->treeWidget()->topLevelItem(0);

	return parent_item->child(0);
}
//----------------------------------------------------------------------------------------------

QTreeWidgetItem* ATreeWidgetItem::lastSibling (QTreeWidgetItem* current)
{
	QTreeWidgetItem* parent_item = current->parent();

	if (parent_item == NULL)
	{
		QTreeWidget* widget = current->treeWidget();

		return widget->topLevelItem(widget->topLevelItemCount() - 1);
	}

	return parent_item->child(parent_item->childCount() - 1);
}
//----------------------------------------------------------------------------------------------

QTreeWidgetItem* ATreeWidgetItem::prevSibling (QTreeWidgetItem* current)
{
	QTreeWidgetItem* parent_item = current->parent();

	if (parent_item == NULL)
	{
		QTreeWidget* widget = current->treeWidget();

		int index = widget->indexOfTopLevelItem(current);

		if (index == 0)
			return NULL;

		index--;

		return widget->topLevelItem(index);
	}

	int index = parent_item->indexOfChild(current);

	if (index == 0)
		return NULL;

	index--;

	return parent_item->child(index);
}
//----------------------------------------------------------------------------------------------

QTreeWidgetItem* ATreeWidgetItem::nextSibling (QTreeWidgetItem* current)
{
	QTreeWidgetItem* parent_item = current->parent();

	if (parent_item == NULL)
	{
		QTreeWidget* widget = current->treeWidget();

		int index = widget->indexOfTopLevelItem(current);

		if (index == widget->topLevelItemCount() - 1)
			return NULL;

		index++;

		return widget->topLevelItem(index);
	}

	int index = parent_item->indexOfChild(current);

	if (index == parent_item->childCount() - 1)
		return NULL;

	index++;

	return parent_item->child(index);
}
//----------------------------------------------------------------------------------------------
