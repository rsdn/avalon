<?php

/*!
 * \file
 * \brief Быстрый тест выбоки списка топиков для форума с включенным кэшем без сортировки
 *
 * $Date: 2009-03-28 10:17:43 +0300 (Сбт, 28 Мар 2009) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 194 $
 * $URL: svn://opensvn.ru/avalon/test/quick_mysql/topic_list_cache_no_sort.php $
 */

/*
Количество запросов        : 10000
Общее время запросов       : 67.414
Среднее время на запрос    : 0.00674
Общее время теста          : 68.306
Время дополнительного кода : 0.893
*/

$time_test  = microtime(true);

$connection = mysql_connect("localhost", "root", "") or die("mysql_connect: " . mysql_error());

mysql_select_db("avalon", $connection) or die ("mysql_select_db : " . mysql_error($connection));

$sql = "
SELECT
    `id_message`
FROM
    `message_topic`
WHERE
    `id_forum` = 8
";

$count = 10000;
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
