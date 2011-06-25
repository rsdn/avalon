<?php

/*!
 * \file
 * \brief Быстрый тест выбоки списка топиков для форума с включенным кэшем и сортировкой
 *
 * $Date: 2009-04-06 05:58:55 +0400 (Пнд, 06 Апр 2009) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 211 $
 * $URL: svn://opensvn.ru/avalon/test/mysql2sqlite/topic_list.php $
 */

/*
Количество запросов        : 100
Общее время запросов       : 33.987
Среднее время на запрос    : 0.33987
Общее время теста          : 34.079
Время дополнительного кода : 0.092
*/

$time_test  = microtime(true);

$connection = null;

try
{
	$connection = new PDO("sqlite:avalon.db");
}
catch (PDOException $e)
{
	die ("SQLite connection error\n");
}

$sql = "
SELECT
	`id_message`
FROM
	`message_topic`
WHERE
	`id_forum` = 8
ORDER BY
	`message_date` DESC
";

$count = 100;
$time_query = 0;

for ($i = 0; $i < $count; $i++)
{
	$time_start = microtime(true);

	$query = $connection->query($sql) or die ("sqlite_query: " . $connection->errorInfo());

	$time_query = $time_query + (microtime(true) - $time_start);

	$query->closeCursor();
}

$time_test = microtime(true) - $time_test;

echo "Количество запросов        : " . strval($count) . "\n";
echo "Общее время запросов       : " . sprintf("%.3f", $time_query) . "\n";
echo "Среднее время на запрос    : " . sprintf("%.5f", $time_query / $count) . "\n";
echo "Общее время теста          : " . sprintf("%.3f", $time_test) . "\n";
echo "Время дополнительного кода : " . sprintf("%.3f", $time_test - $time_query) . "\n";

?>
