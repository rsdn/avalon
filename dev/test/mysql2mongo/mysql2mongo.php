<?php

/*!
 * \file
 * \brief Конвертация базы MySQL в базу MongoDB
 *
 * $Date: 2010-07-30 15:27:33 +0400 (Птн, 30 Июл 2010) $
 * $Author: antonbatenev.ya.ru $
 * $Revision: 347 $
 * $URL: svn://opensvn.ru/avalon/test/mysql2mongo/mysql2mongo.php $
 */

//
// параметры подключения к MySQL
//

$mysql_host     = "localhost";
$mysql_database = "avalon";
$mysql_user     = "root";
$mysql_password = "";

//
// соединение с MongoDB
//

$mongo = new Mongo();
$mongo_db = $mongo->selectDB("avalon");
$mongo_db->drop();

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

$mongo_collection = $mongo_db->selectCollection("broken");

$sql = "SELECT `id_message` AS `_id`, `is_topic` FROM `broken`";
$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

$row = mysql_fetch_array($query, MYSQL_ASSOC);

while ($row !== false)
{
	$mongo_collection->insert($row);
	$row = mysql_fetch_array($query, MYSQL_ASSOC);
}

mysql_free_result($query);

//
// forum
//

echo "Process table `forum`\n";

$mongo_collection = $mongo_db->selectCollection("forum");

$sql = "SELECT `id` AS `_id`, `id_group`, `short_name`, `name`, `rated`, `in_top`, `rate_limit` FROM `forum`";
$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

$row = mysql_fetch_array($query, MYSQL_ASSOC);

while ($row !== false)
{
	$mongo_collection->insert($row);
	$row = mysql_fetch_array($query, MYSQL_ASSOC);
}

mysql_free_result($query);

//
// group
//

echo "Process table `group`\n";

$mongo_collection = $mongo_db->selectCollection("group");

$sql = "SELECT `id` AS `_id`, `name`, `sort_order` FROM `group`";
$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

$row = mysql_fetch_array($query, MYSQL_ASSOC);

while ($row !== false)
{
	$mongo_collection->insert($row);
	$row = mysql_fetch_array($query, MYSQL_ASSOC);
}

mysql_free_result($query);

//
// message2send
//

echo "Process table `message2send`\n";

$mongo_collection = $mongo_db->selectCollection("message2send");

$sql = "SELECT `id` AS `_id`, `id_parent`, `id_forum`, `subject`, `message`, `date`, `draft` FROM `message2send`";
$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

$row = mysql_fetch_array($query, MYSQL_ASSOC);

while ($row !== false)
{
	$mongo_collection->insert($row);
	$row = mysql_fetch_array($query, MYSQL_ASSOC);
}

mysql_free_result($query);

//
// moderate
//

echo "Process table `moderate`\n";

$mongo_collection = $mongo_db->selectCollection("moderate");

$index    = 0;
$page     = 10000;
$index_lo = 0;
$index_hi = $index_lo + $page;

while ($index_lo < $max_message_id)
{
	$sql = "SELECT `id_message`, `id_topic`, `id_user`, `id_forum`, `created` FROM `moderate` WHERE `id_message` >= " . strval($index_lo) . " AND `id_message` < " . strval($index_hi);
	$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

	$row = mysql_fetch_array($query, MYSQL_ASSOC);

	while ($row !== false)
	{
		$new_row = array
		(
			'_id'      => array ('id_message' => $row['id_message'], 'id_user' => $row['id_user']),
			'id_topic' => $row['id_topic'],
			'id_forum' => $row['id_forum'],
			'created'  => $row['created']
		);

		$mongo_collection->insert($new_row);

		$row = mysql_fetch_array($query, MYSQL_ASSOC);

		$index = $index + 1;

		if ($index % 100 == 0)
			echo "`moderate` rows added " . strval($index) . "\n";
	}

	mysql_free_result($query);

	$index_lo = $index_lo + $page;
	$index_hi = $index_lo + $page;
}

//
// moderate2send
//

echo "Process table `moderate2send`\n";

$mongo_collection = $mongo_db->selectCollection("moderate2send");

$sql = "SELECT `id` AS `_id`, `id_message`, `action`, `id_forum`, `description`, `as_moderator`, `date` FROM `moderate2send`";
$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

$row = mysql_fetch_array($query, MYSQL_ASSOC);

while ($row !== false)
{
	$mongo_collection->insert($row);
	$row = mysql_fetch_array($query, MYSQL_ASSOC);
}

mysql_free_result($query);

//
// rating
//

echo "Process table `rating`\n";

$mongo_collection = $mongo_db->selectCollection("rating");

$index    = 0;
$page     = 10000;
$index_lo = 0;
$index_hi = $index_lo + $page;

while ($index_lo < $max_message_id)
{
	$sql = "SELECT `id_message`, `id_topic`, `id_user`, `user_rating`, `rate`, `rate_date` FROM `rating` WHERE `id_message` >= " . strval($index_lo) . " AND `id_message` < " . strval($index_hi);
	$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

	$row = mysql_fetch_array($query, MYSQL_ASSOC);

	while ($row !== false)
	{
		$new_row = array
		(
			'_id'         => array ('id_message' => $row['id_message'], 'id_user' => $row['id_user']),
			'id_topic'    => $row['id_topic'],
			'user_rating' => $row['user_rating'],
			'rate'        => $row['rate'],
			'rate_date'   => $row['rate_date']
		);

		$mongo_collection->insert($new_row);

		$row = mysql_fetch_array($query, MYSQL_ASSOC);

		$index = $index + 1;

		if ($index % 100 == 0)
			echo "`rating` rows added " . strval($index) . "\n";
	}

	mysql_free_result($query);

	$index_lo = $index_lo + $page;
	$index_hi = $index_lo + $page;
}

//
// rating2send
//

echo "Process table `rating2send`\n";

$mongo_collection = $mongo_db->selectCollection("rating2send");

$sql = "SELECT `id` AS `_id`, `id_message`, `rate`, `date` FROM `rating2send`";
$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

$row = mysql_fetch_array($query, MYSQL_ASSOC);

while ($row !== false)
{
	$mongo_collection->insert($row);
	$row = mysql_fetch_array($query, MYSQL_ASSOC);
}

mysql_free_result($query);

//
// row_version
//

echo "Process table `row_version`\n";

$mongo_collection = $mongo_db->selectCollection("row_version");

$sql = "SELECT `key` AS `_id`, `value` FROM `row_version`";
$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

$row = mysql_fetch_array($query, MYSQL_ASSOC);

while ($row !== false)
{
	$mongo_collection->insert($row);
	$row = mysql_fetch_array($query, MYSQL_ASSOC);
}

mysql_free_result($query);

//
// subscribed
//

echo "Process table `subscribed`\n";

$mongo_collection = $mongo_db->selectCollection("subscribed");

$sql = "SELECT `id_forum` AS `_id`, `is_first` FROM `subscribed`";
$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

$row = mysql_fetch_array($query, MYSQL_ASSOC);

while ($row !== false)
{
	$mongo_collection->insert($row);
	$row = mysql_fetch_array($query, MYSQL_ASSOC);
}

mysql_free_result($query);

//
// unread
//

echo "Process table `unread`\n";

$mongo_collection = $mongo_db->selectCollection("unread");

$sql = "SELECT `id_message` AS `_id`, `id_parent`, `id_forum`, `id_topic`, `id_parent_user`, `message_date` FROM `unread`";
$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

$row = mysql_fetch_array($query, MYSQL_ASSOC);

while ($row !== false)
{
	$mongo_collection->insert($row);
	$row = mysql_fetch_array($query, MYSQL_ASSOC);
}

mysql_free_result($query);

//
// user
//

echo "Process table `user`\n";

$mongo_collection = $mongo_db->selectCollection("user");

$index    = 0;
$page     = 1000;
$index_lo = 0;
$index_hi = $index_lo + $page;

while ($index_lo < 100000)
{
	$sql = "SELECT `id` AS `_id`, `name`, `nick`, `real_name`, `email`, `homepage`, `specialization`, `where_from`, `origin` FROM `user` WHERE `id` >= " . strval($index_lo) . " AND `id` < " . strval($index_hi);
	$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

	$row = mysql_fetch_array($query, MYSQL_ASSOC);

	while ($row !== false)
	{
		$mongo_collection->insert($row);

		$row = mysql_fetch_array($query, MYSQL_ASSOC);

		$index = $index + 1;

		if ($index % 100 == 0)
			echo "`user` row " . strval($index) . "\n";
	}

	mysql_free_result($query);

	$index_lo = $index_lo + $page;
	$index_hi = $index_lo + $page;
}

//
// version
//

echo "Process table `version`\n";

$mongo_collection = $mongo_db->selectCollection("version");

$sql = "SELECT `key` AS `_id`, `value` FROM `version`";
$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

$row = mysql_fetch_array($query, MYSQL_ASSOC);

while ($row !== false)
{
	$mongo_collection->insert($row);
	$row = mysql_fetch_array($query, MYSQL_ASSOC);
}

mysql_free_result($query);

//
// message
//

echo "Process table `message`\n";

$mongo_collection = $mongo_db->selectCollection("message");

$index    = 0;
$page     = 1000;
$index_lo = 0;
$index_hi = $index_lo + $page;

while ($index_lo < $max_message_id)
{
	$sql = "SELECT `id` AS `_id`, `id_topic`, `id_parent`, `id_user`, `id_forum`, `subject`, `message_name`, `user_nick`, `message`, `id_article`, `message_date`, `update_date`, `user_role`, `user_title`, `user_title_color`, `last_moderated`, `closed`, `has_child`, `compressed`  FROM `message` WHERE `id` >= " . strval($index_lo) . " AND `id` < " . strval($index_hi);
	$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

	$row = mysql_fetch_array($query, MYSQL_ASSOC);

	while ($row !== false)
	{
		$mongo_collection->insert($row);

		$row = mysql_fetch_array($query, MYSQL_ASSOC);

		$index = $index + 1;

		if ($index % 100 == 0)
			echo "`message` row " . strval($index) . "\n";
	}

	mysql_free_result($query);

	$index_lo = $index_lo + $page;
	$index_hi = $index_lo + $page;
}

//
// message_topic
//

echo "Process table `message_topic`\n";

$mongo_collection = $mongo_db->selectCollection("message_topic");

$index    = 0;
$page     = 1000;
$index_lo = 0;
$index_hi = $index_lo + $page;

while ($index_lo < $max_message_id)
{
	$sql = "SELECT `id_message` AS `_id`, `id_forum`, `message_date` FROM `message_topic` WHERE `id_message` >= " . strval($index_lo) . " AND `id_message` < " . strval($index_hi);
	$query = mysql_unbuffered_query($sql, $mysql_connection) or die ("mysql_query: " . mysql_error($mysql_connection) . "\n");

	$row = mysql_fetch_array($query, MYSQL_ASSOC);

	while ($row !== false)
	{
		$mongo_collection->insert($row);

		$row = mysql_fetch_array($query, MYSQL_ASSOC);

		$index = $index + 1;

		if ($index % 100 == 0)
			echo "`message_topic` row " . strval($index) . "\n";
	}

	mysql_free_result($query);

	$index_lo = $index_lo + $page;
	$index_hi = $index_lo + $page;
}

echo "DONE!!!";

?>
