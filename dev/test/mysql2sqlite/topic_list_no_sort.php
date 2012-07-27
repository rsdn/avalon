<?php

/*!
 * \file
 * \brief Быстрый тест выбоки списка топиков для форума с включенным кэшем и сортировкой
 */

/*
Количество запросов        : 100000
Общее время запросов       : 6.032
Среднее время на запрос    : 0.00006
Общее время теста          : 7.195
Время дополнительного кода : 1.163
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
	`id_message` DESC
";

$count = 1000000;
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
