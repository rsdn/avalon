/*!
 * \file
 * \brief Тест возможной степени сжатия тел сообщений при помощи zlib [http://zlib.net/]
 *
 * $Date: 2009-04-05 00:23:04 +0400 (Вск, 05 Апр 2009) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 206 $
 * $URL: svn://opensvn.ru/avalon/test/zip_raito/main.cpp $
 */

#include "../../trunk/storage/mysql_database.h"
#include "zlib.h"

/*!
 * (no comments)
 * \param argc (no comments)
 * \param argv (no comments)
 * \return (no comments)
 */
int main (int argc, char* argv[])
{
	// стандартная инициализация для avalon
	QApplication app(argc, argv);

	QCoreApplication::setOrganizationName("www.rsdn.ru");
	QCoreApplication::setApplicationName("avalon");

	// подготовка запросов
	QString sql;

	AMySQLDatabase database;

	// запрос максимального ID
	sql += "SELECT\n";
	sql += "	MAX(`id`)\n";
	sql += "FROM\n";
	sql += "	`message`\n";

	std::auto_ptr<AQuery> query_max_id(database.createQuery(sql));

	if (query_max_id->exec() == false)
	{
		printf("%s\n", query_max_id->getLastError().toStdString().c_str());
		return 1;
	}

	int max_id = 0;

	while (query_max_id->next())
		max_id = query_max_id->value(0).toInt();

	query_max_id->finish();

	// запрос страницы с телами сообщений
	sql  = "";
	sql += "SELECT\n";
	sql += "	`message`\n";
	sql += "FROM\n";
	sql += "	`message`\n";
	sql += "WHERE\n";
	sql += "	`id` >= :id_lo AND `id` < :id_hi\n";

	std::auto_ptr<AQuery> query(database.createQuery(sql));

	query->setForwardOnly(true);

	// начальные значения счетчиков
	int page  = 10000;        // размер страницы
	int id_lo = 0;            // нижний ID
	int id_hi = id_lo + page; // верхний ID
	int index = 0;            // количесто обработанных строк

	uLongf dst_size = 3 * 1024 * 1024;     // максимальный размер сжатых данных (3 MB согласно предварительным оценкам хватит)
	Bytef* dst_buf  = new Bytef[dst_size]; // буфер для сжатых данных

	uLongf total_old_size   = 0; // суммарный объем несжатых данных
	uLongf total_new_size   = 0; // суммарный объем сжатых данных
	int    total_errors     = 0; // общее количество ошибок
	int    total_faults     = 0; // общее количество несжатых строк (по причине невозможности сжатия)

	// постраничный проход по таблице
	while (id_lo < max_id + 1)
	{
		// выборка страницы
		query->bindValue(":id_lo", id_lo);
		query->bindValue(":id_hi", id_hi);

		if (query->exec() == false)
		{
			printf("%s\n", query->getLastError().toStdString().c_str());
			return 1;
		}

		// проход по строкам выбранной страницы
		while (query->next())
		{
			index++;

			// если сразу конвертить в QByteArray, то слетит кодировка строки
			QByteArray msg = query->value(0).toByteArray();

			uLongf       src_size = msg.count();                   // размер исходных данных строки
			const Bytef* src_buf  = (const Bytef*)msg.constData(); // массив исходных данных строки

			uLongf new_size = dst_size; // размер сжатых данных строки

			// сжатие (см. документацию по zlib)
			int z_result = compress2(dst_buf, &new_size, src_buf, src_size, 9);

			// анализ результата
			if (z_result != Z_OK)
				total_errors++;
			else if (new_size >= src_size)
			{
				total_faults++;
				total_new_size += src_size;
			}
			else
				total_new_size += new_size;

			total_old_size += src_size;

			// TODO: добавить тест обратной распаковки
		}

		// вычисление индексов новой страницы
		id_lo = id_hi;
		id_hi = id_lo + page;

		// вывод промежуточного результата
		printf("----------------------------------------------------\n");
		printf("всего строк           : %i\n",                    index);
		printf("ошибок сжатия         : %i (%.2f%%)\n",           total_errors, (double)total_errors / (double)index * (double)100);
		printf("несжатых строк        : %i (%.2f%%)\n",           total_faults, (double)total_faults / (double)index * (double)100);
		printf("размер несжатых строк : %lu (~%lu MB)\n",         total_old_size, total_old_size / 1024 / 1024);
		printf("размер сжатых строк   : %lu (~%lu MB, %.2f%%)\n", total_new_size, total_new_size / 1024 / 1024,  (double)total_new_size / (double)total_old_size  * (double)100);
	}

	delete [] dst_buf;

	return 0;
}
