###
# $Date: 2009-07-13 20:46:26 +0400 (Пнд, 13 Июл 2009) $
# $Author: antonbatenev.ya.ru $
# $Revision: 262 $
# $URL: svn://opensvn.ru/avalon/trunk/dev/avalon.mysql.000.sql $
###

###
#
# патч для установки начального значения версии после создания пустой базы
#
###

START TRANSACTION;

USE `avalon`;

###
# если патч установлен, то транзакция даст ошибку
###

INSERT INTO `version`
(
	`key`,
	`value`
)
VALUES
(
	'avalon.000.sql',
	NOW()
);

###
# обновление версии
###

REPLACE INTO `version`
(
	`key`,
	`value`
)
VALUES
(
	'version',
	'v1.0rc2, patch avalon.000.sql'
);

###
# выборка результата
###

SELECT
	CONCAT(`value`, ' - OK') AS `result`
FROM
	`version`
WHERE
	`key` = 'version';

COMMIT;
