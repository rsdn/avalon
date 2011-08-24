<?php

/*!
 * \file
 * \brief Конвертация базы MySQL в базу SQLite
 *
 * $Date: 2010-07-30 15:27:33 +0400 (Птн, 30 Июл 2010) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 347 $
 * $URL: svn://opensvn.ru/avalon/test/mysql2sqlite/mysql2sqlite.php $
 */

//
// параметры подключения к MySQL
//

$mysql_host     = "localhost";
$mysql_database = "avalon";
$mysql_user     = "root";
$mysql_password = "";

//
// параметры базы SQLite
//

$sqlite_filename = "avalon.db";

//
// соединение с SQLite
//

if (file_exists($sqlite_filename) == true)
	unlink($sqlite_filename) or die ("can't delete file " . $sqlite_filename . "\n");

$sqlite_connection = null;

try
{
	$sqlite_connection = new PDO("sqlite:" . $sqlite_filename);
}
catch (PDOException $e)
{
	die ("SQLite connection error\n");
}

function quoteString ($param)
{
	global $sqlite_connection;
	return $sqlite_connection->quote($param);
}

//
// создание схемы БД
//

$sql = file_get_contents("../../trunk/dev/avalon.sqlite.sql");

if ($sqlite_connection->exec($sql) === false)
{
	$error = $sqlite_connection->errorInfo();
	die ($error[2] . "\n");
}

//
// соединение с MySQL
//

$mysql_connection = mysql_connect($mysql_host, $mysql_user, $mysql_password) or die ("mysql_connect: " . mysql_error() . "\n");
mysql_select_db($mysql_database, $mysql_connection) or die ("mysql_select_db : " . mysql_error($mysql_connection) . "\n");
mysql_set_charset("utf8", $mysql_connection) or die ("mysql_set_charset : " . mysql_error($mysql_connection) . "\n");

//
// получение максимального id сообщения
//

$query = mysql_query("SELECT MAX(`id`) FROM `message`", $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");
$row = mysql_fetch_row($query);
$max_message_id = $row[0];
$max_message_id = $max_message_id + 1;
mysql_free_result($query);

//
// broken
//

echo "Process table `broken`\n";

$sql = "SELECT `id_message`, `is_topic` FROM `broken`";
$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

$row = mysql_fetch_row($query);

while ($row !== false)
{
	$sql = "INSERT INTO `broken` VALUES (" . $row[0] . ", " . $row[1] . ")";

	if ($sqlite_connection->exec($sql) === false)
	{
		$error = $sqlite_connection->errorInfo();
		die ($error[2] . "\n" . $sql . "\n");
	}

	$row = mysql_fetch_row($query);
}

mysql_free_result($query);

//
// forum
//

echo "Process table `forum`\n";

$sql = "SELECT `id`, `id_group`, `short_name`, `name`, `rated`, `in_top`, `rate_limit` FROM `forum`";
$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

$row = mysql_fetch_row($query);

while ($row !== false)
{
	$sql = "INSERT INTO `forum` VALUES (" . $row[0] . ", " . $row[1] . ", " . quoteString($row[2]) . ", " . quoteString($row[3]) . ", " . $row[4] . ", " . $row[5] . ", " . $row[6] . ")";

	if ($sqlite_connection->exec($sql) === false)
	{
		$error = $sqlite_connection->errorInfo();
		die ($error[2] . "\n" . $sql . "\n");
	}

	$row = mysql_fetch_row($query);
}

mysql_free_result($query);

//
// group
//

echo "Process table `group`\n";

$sql = "SELECT `id`, `name`, `sort_order` FROM `group`";
$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

$row = mysql_fetch_row($query);

while ($row !== false)
{
	$sql = "INSERT INTO `group` VALUES (" . $row[0] . ", " . quoteString($row[1]) . ", " . $row[2] . ")";

	if ($sqlite_connection->exec($sql) === false)
	{
		$error = $sqlite_connection->errorInfo();
		die ($error[2] . "\n" . $sql . "\n");
	}

	$row = mysql_fetch_row($query);
}

mysql_free_result($query);

//
// message2send
//

echo "Process table `message2send`\n";

$sql = "SELECT `id`, `id_parent`, `id_forum`, `subject`, `message`, UNIX_TIMESTAMP(`date`), `draft` FROM `message2send`";
$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

$row = mysql_fetch_row($query);

while ($row !== false)
{
	$sql = "INSERT INTO `message2send` VALUES (" . $row[0] . ", " . $row[1] . ", " . $row[2] . ", " . quoteString($row[3]) . ", " . quoteString($row[4]) . ", " . $row[5] .  ", " . $row[6] . ")";

	if ($sqlite_connection->exec($sql) === false)
	{
		$error = $sqlite_connection->errorInfo();
		die ($error[2] . "\n" . $sql . "\n");
	}

	$row = mysql_fetch_row($query);
}

mysql_free_result($query);

//
// moderate
//

echo "Process table `moderate`\n";

$index    = 0;
$page     = 10000;
$index_lo = 0;
$index_hi = $index_lo + $page;

while ($index_lo < $max_message_id)
{
	$sql = "SELECT `id_message`, `id_topic`, `id_user`, `id_forum`, UNIX_TIMESTAMP(`created`) FROM `moderate` WHERE `id_message` >= " . strval($index_lo) . " AND `id_message` < " . strval($index_hi);
	$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

	$row = mysql_fetch_row($query);

	$sqlite_connection->beginTransaction();

	while ($row !== false)
	{
		$sql = "INSERT INTO `moderate` VALUES (" . $row[0] . ", " . $row[1] . ", " . $row[2] . ", " . $row[3] . ", " . $row[4] . ")";

		if ($sqlite_connection->exec($sql) === false)
		{
			$error = $sqlite_connection->errorInfo();
			die ($error[2] . "\n" . $sql . "\n");
		}

		$row = mysql_fetch_row($query);

		$index = $index + 1;

		if ($index % 100 == 0)
			echo "`moderate` rows added " . strval($index) . "\n";
	}

	mysql_free_result($query);

	$sqlite_connection->commit();

	$index_lo = $index_lo + $page;
	$index_hi = $index_lo + $page;
}

//
// moderate2send
//

echo "Process table `moderate2send`\n";

$sql = "SELECT `id`, `id_message`, `action`, `id_forum`, `description`, `as_moderator`, UNIX_TIMESTAMP(`date`) FROM `moderate2send`";
$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

$row = mysql_fetch_row($query);

while ($row !== false)
{
	$sql = "INSERT INTO `moderate2send` VALUES (" . $row[0] . ", " . $row[1] . ", " . quoteString($row[2]) . ", " . $row[3] . ", " . quoteString($row[4]) . ", " . $row[5] . ", " . $row[6] . ")";

	if ($sqlite_connection->exec($sql) === false)
	{
		$error = $sqlite_connection->errorInfo();
		die ($error[2] . "\n" . $sql . "\n");
	}

	$row = mysql_fetch_row($query);
}

mysql_free_result($query);

//
// rating
//

echo "Process table `rating`\n";

$index    = 0;
$page     = 10000;
$index_lo = 0;
$index_hi = $index_lo + $page;

while ($index_lo < $max_message_id)
{
	$sql = "SELECT `id_message`, `id_topic`, `id_user`, `user_rating`, `rate`, UNIX_TIMESTAMP(`rate_date`) FROM `rating` WHERE `id_message` >= " . strval($index_lo) . " AND `id_message` < " . strval($index_hi);
	$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

	$row = mysql_fetch_row($query);

	$sqlite_connection->beginTransaction();

	while ($row !== false)
	{
		$sql = "INSERT INTO `rating` VALUES (" . $row[0] . ", " . $row[1] . ", " . $row[2] . ", " . $row[3] . ", " . $row[4] . ", " . $row[5] . ")";

		if ($sqlite_connection->exec($sql) === false)
		{
			$error = $sqlite_connection->errorInfo();
			die ($error[2] . "\n" . $sql . "\n");
		}

		$row = mysql_fetch_row($query);

		$index = $index + 1;

		if ($index % 100 == 0)
			echo "`rating` rows added " . strval($index) . "\n";
	}

	mysql_free_result($query);

	$sqlite_connection->commit();

	$index_lo = $index_lo + $page;
	$index_hi = $index_lo + $page;
}

//
// rating2send
//

echo "Process table `rating2send`\n";

$sql = "SELECT `id`, `id_message`, `rate`, UNIX_TIMESTAMP(`date`) FROM `rating2send`";
$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

$row = mysql_fetch_row($query);

while ($row !== false)
{
	$sql = "INSERT INTO `rating2send` VALUES (" . $row[0] . ", " . $row[1] . ", " . $row[2] . ", " . $row[3] . ")";

	if ($sqlite_connection->exec($sql) === false)
	{
		$error = $sqlite_connection->errorInfo();
		die ($error[2] . "\n" . $sql . "\n");
	}

	$row = mysql_fetch_row($query);
}

mysql_free_result($query);

//
// row_version
//

echo "Process table `row_version`\n";

$sql = "SELECT `key`, `value` FROM `row_version`";
$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

$row = mysql_fetch_row($query);

while ($row !== false)
{
	$sql = "INSERT INTO `row_version` VALUES (" . quoteString($row[0]) . ", " . quoteString($row[1]) . ")";

	if ($sqlite_connection->exec($sql) === false)
	{
		$error = $sqlite_connection->errorInfo();
		die ($error[2] . "\n" . $sql . "\n");
	}

	$row = mysql_fetch_row($query);
}

mysql_free_result($query);

//
// subscribed
//

echo "Process table `subscribed`\n";

$sql = "SELECT `id_forum`, `is_first` FROM `subscribed`";
$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

$row = mysql_fetch_row($query);

while ($row !== false)
{
	$sql = "INSERT INTO `subscribed` VALUES (" . $row[0] . ", " . $row[1] . ")";

	if ($sqlite_connection->exec($sql) === false)
	{
		$error = $sqlite_connection->errorInfo();
		die ($error[2] . "\n" . $sql . "\n");
	}

	$row = mysql_fetch_row($query);
}

mysql_free_result($query);

//
// unread
//

echo "Process table `unread`\n";

$sql = "SELECT `id_message`, `id_parent`, `id_forum`, `id_topic`, `id_parent_user`, UNIX_TIMESTAMP(`message_date`) FROM `unread`";
$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

$row = mysql_fetch_row($query);

while ($row !== false)
{
	$sql = "INSERT INTO `unread` VALUES (" . $row[0] . ", " . $row[1] . ", " . $row[2] . ", " . $row[3] . ", " . $row[4] . ", " . $row[5] . ")";

	if ($sqlite_connection->exec($sql) === false)
	{
		$error = $sqlite_connection->errorInfo();
		die ($error[2] . "\n" . $sql . "\n");
	}

	$row = mysql_fetch_row($query);
}

mysql_free_result($query);

//
// user
//

echo "Process table `user`\n";

$index    = 0;
$page     = 1000;
$index_lo = 0;
$index_hi = $index_lo + $page;

while ($index_lo < 100000)
{
	$sql = "SELECT `id`, `name`, `nick`, `real_name`, `email`, `homepage`, `specialization`, `where_from`, `origin` FROM `user` WHERE `id` >= " . strval($index_lo) . " AND `id` < " . strval($index_hi);
	$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

	$row = mysql_fetch_row($query);

	$sqlite_connection->beginTransaction();

	while ($row !== false)
	{
		$sql = "INSERT INTO `user` VALUES (" . $row[0] . ", " . quoteString($row[1]) . ", " . quoteString($row[2]) . ", " . quoteString($row[3]) . ", " . quoteString($row[4]) . ", " . quoteString($row[5]) . ", " . quoteString($row[6]) . ", " . quoteString($row[7]) . ", " . quoteString($row[8]) . ")";

		if ($sqlite_connection->exec($sql) === false)
		{
			$error = $sqlite_connection->errorInfo();
			die ($error[2] . "\n" . $sql . "\n");
		}

		$row = mysql_fetch_row($query);

		$index = $index + 1;

		if ($index % 100 == 0)
			echo "`user` row " . strval($index) . "\n";
	}

	mysql_free_result($query);

	$sqlite_connection->commit();

	$index_lo = $index_lo + $page;
	$index_hi = $index_lo + $page;
}

//
// version
//

echo "Process table `version`\n";

$sql = "SELECT `key`, `value` FROM `version`";
$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

$row = mysql_fetch_row($query);

while ($row !== false)
{
	$sql = "INSERT INTO `version` VALUES (" . quoteString($row[0]) . ", " . quoteString($row[1]) . ")";

	if ($sqlite_connection->exec($sql) === false)
	{
		$error = $sqlite_connection->errorInfo();
		die ($error[2] . "\n" . $sql . "\n");
	}

	$row = mysql_fetch_row($query);
}

mysql_free_result($query);

//
// message
//

echo "Process table `message`\n";

$index    = 0;
$page     = 1000;
$index_lo = 0;
$index_hi = $index_lo + $page;

while ($index_lo < $max_message_id)
{
	$sql = "SELECT `id`, `id_topic`, `id_parent`, `id_user`, `id_forum`, `subject`, `message_name`, `user_nick`, `message`, `id_article`, UNIX_TIMESTAMP(`message_date`), UNIX_TIMESTAMP(`update_date`), `user_role`, `user_title`, `user_title_color`, UNIX_TIMESTAMP(`last_moderated`), `closed`, `has_child`, `compressed`  FROM `message` WHERE `id` >= " . strval($index_lo) . " AND `id` < " . strval($index_hi);
	$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

	$row = mysql_fetch_row($query);

	$sqlite_connection->beginTransaction();

	while ($row !== false)
	{
		$sql = "INSERT INTO `message` VALUES (" . $row[0] . ", " . $row[1] . ", " . $row[2] . ", " . $row[3] . ", " . $row[4] . ", " . quoteString($row[5]) . ", " . quoteString($row[6]) . ", " . quoteString($row[7]) . ", " . quoteString($row[8]) . ", " . $row[9] . ", " . $row[10] . ", " . $row[11] . ", " . quoteString($row[12]) . ", " . quoteString($row[13]) . ", " . $row[14] . ", " . $row[15] . ", " . $row[16] . ", " . $row[17] . ", " . $row[18] . ")";

		if ($sqlite_connection->exec($sql) === false)
		{
			$error = $sqlite_connection->errorInfo();
			die ($error[2] . "\n" . $sql . "\n");
		}

		$row = mysql_fetch_row($query);

		$index = $index + 1;

		if ($index % 100 == 0)
			echo "`message` row " . strval($index) . "\n";
	}

	mysql_free_result($query);

	$sqlite_connection->commit();

	$index_lo = $index_lo + $page;
	$index_hi = $index_lo + $page;
}

//
// message_topic
//

echo "Process table `message_topic`\n";

$index    = 0;
$page     = 1000;
$index_lo = 0;
$index_hi = $index_lo + $page;

while ($index_lo < $max_message_id)
{
	$sql = "SELECT `id_message`, `id_forum`, UNIX_TIMESTAMP(`message_date`) FROM `message_topic` WHERE `id_message` >= " . strval($index_lo) . " AND `id_message` < " . strval($index_hi);
	$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

	$row = mysql_fetch_row($query);

	$sqlite_connection->beginTransaction();

	while ($row !== false)
	{
		$sql = "INSERT INTO `message_topic` VALUES (" . $row[0] . ", " . $row[1] . ", " . $row[2] . ")";

		if ($sqlite_connection->exec($sql) === false)
		{
			$error = $sqlite_connection->errorInfo();
			die ($error[2] . "\n" . $sql . "\n");
		}

		$row = mysql_fetch_row($query);

		$index = $index + 1;

		if ($index % 100 == 0)
			echo "`message_topic` row " . strval($index) . "\n";
	}

	mysql_free_result($query);

	$sqlite_connection->commit();

	$index_lo = $index_lo + $page;
	$index_hi = $index_lo + $page;
}

echo "DONE!!!";

?>
