<?php

/*!
 * \file
 * \brief Быстрый тест выбоки списка топиков для форума с выключенным кэшем и сортировкой
 */

/*
Количество запросов        : 1000
Общее время запросов       : 102.570
Среднее время на запрос    : 0.103
Общее время теста          : 102.678
Время дополнительного кода : 0.108
*/

$time_test  = microtime(true);

$connection = mysql_connect("localhost", "root", "") or die("mysql_connect: " . mysql_error());

mysql_select_db("avalon", $connection) or die ("mysql_select_db : " . mysql_error($connection));

$sql = "
SELECT SQL_NO_CACHE
    `id_message`
FROM
    `message_topic`
WHERE
    `id_forum` = 8
ORDER BY
    `message_date` DESC
";

$count = 1000;
$time_query = 0;

for ($i = 0; $i < $count; $i++)
{
    $time_start = microtime(true);

    $query = mysql_query($sql, $connection) or die("mysql_query: " . mysql_error($connection));

    $time_query = $time_query + (microtime(true) - $time_start);

    mysql_free_result($query) or die("mysql_free_result: " . mysql_error($connection));
}

mysql_close($connection);

$time_test = microtime(true) - $time_test;

echo "Количество запросов        : " . strval($count) . "\n";
echo "Общее время запросов       : " . sprintf("%.3f", $time_query) . "\n";
echo "Среднее время на запрос    : " . sprintf("%.5f", $time_query / $count) . "\n";
echo "Общее время теста          : " . sprintf("%.3f", $time_test) . "\n";
echo "Время дополнительного кода : " . sprintf("%.3f", $time_test - $time_query) . "\n";

?>
