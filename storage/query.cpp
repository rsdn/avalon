#include "query.h"
//----------------------------------------------------------------------------------------------

AQuery::AQuery (QSqlDatabase* database, const QString& sql) : QSqlQuery (QString(), *database)
{
	m_sql = sql;
}
//----------------------------------------------------------------------------------------------

AQuery::~AQuery ()
{
}
//----------------------------------------------------------------------------------------------

QString AQuery::getLastError ()
{
	return lastError().databaseText() + "\n" + lastError().driverText() + "\n" + QString::fromUtf8("Ошибка: ") + QString::number(lastError().number());
}
//----------------------------------------------------------------------------------------------

void AQuery::showError (QWidget* parent)
{
	QMessageBox::critical(parent, QString::fromUtf8("Ошибка!"), getLastError());
}
//----------------------------------------------------------------------------------------------

bool AQuery::exec ()
{
	if (m_sql.length() != 0)
		return QSqlQuery::exec(m_sql);

	return QSqlQuery::exec();
}
//----------------------------------------------------------------------------------------------
