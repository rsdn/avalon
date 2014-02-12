###
#
# схема хранилища MySQL
#
###

CREATE TABLE `broken`
(
	`id_message` INT        NOT NULL COMMENT 'id топика/сообщения',
	`is_topic`   TINYINT(1) NOT NULL COMMENT 'флаг того, что это топик, а не сообщение',

	PRIMARY KEY (`id_message`)

) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='список поломанных топиков/сообщений';

CREATE TABLE `forum`
(
	`id`         INT          NOT NULL COMMENT 'id',
	`id_group`   INT          NOT NULL COMMENT 'id группы форумов',
	`short_name` VARCHAR(64)  NOT NULL COMMENT 'короткое имя форума',
	`name`       VARCHAR(128) NOT NULL COMMENT 'полное имя форума',
	`rated`      INT          NOT NULL COMMENT 'оценивается ли форум',
	`in_top`     INT          NOT NULL COMMENT 'участвует ли оценки этого форума в топе',
	`rate_limit` INT          NOT NULL COMMENT 'лимит оценки в форуме',

	PRIMARY KEY (`id`)

) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='список форумов';

CREATE TABLE `group`
(
	`id`         INT          NOT NULL COMMENT 'id',
	`name`       VARCHAR(100) NOT NULL COMMENT 'наименование группы',
	`sort_order` INT          NOT NULL COMMENT 'порядок сортировки',

	PRIMARY KEY (`id`)

) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='список групп форумов';

CREATE TABLE `message`
(
	`id`               INT          NOT NULL COMMENT 'id сообщения',
	`id_topic`         INT          NOT NULL COMMENT 'id темы',
	`id_parent`        INT          NOT NULL COMMENT 'id родительского сообщения',
	`id_user`          INT          NOT NULL COMMENT 'id автора',
	`id_forum`         INT          NOT NULL COMMENT 'id форума',
	`subject`          VARCHAR(128) NOT NULL COMMENT 'тема сообщения',
	`message_name`     VARCHAR(160) NOT NULL COMMENT 'имя сообщения',
	`user_nick`        VARCHAR(100) NOT NULL COMMENT 'имя автора сообщения',
	`message`          LONGBLOB     NOT NULL COMMENT 'текст сообщения',
	`id_article`       INT          NOT NULL COMMENT 'id статьи, если сообщение является статьей или 0',
	`message_date`     DATETIME     NOT NULL COMMENT 'дата создания сообщения',
	`update_date`      DATETIME     NOT NULL COMMENT 'дата обновления сообщения или 0',
	`user_role`        VARCHAR(50)  NOT NULL COMMENT 'статус автора сообщения',
	`user_title`       VARCHAR(100) NOT NULL COMMENT 'повязка пользователя',
	`user_title_color` INT          NOT NULL COMMENT 'цвет повязки пользователя',
	`last_moderated`   DATETIME     NOT NULL COMMENT 'дата последнего переноса сообщения',
	`closed`           TINYINT(1)   NOT NULL COMMENT 'флаг закрытия темы',
	`has_child`        TINYINT(1)   NOT NULL COMMENT 'флаг наличия дочерних сообщений',
	`compressed`       TINYINT(1)   NOT NULL COMMENT 'флаг сжатия тела сообщения',

	PRIMARY KEY (`id`),

	KEY `ix_id_topic`     (`id_topic`),
	KEY `ix_id_parent`    (`id_parent`),
	KEY `ix_id_user`      (`id_user`),
	KEY `ix_id_forum`     (`id_forum`),
	KEY `ix_message_date` (`message_date`)

) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='сообщения';

CREATE TABLE `message2send`
(
	`id`        INT          NOT NULL COMMENT 'id сообщения (локальный)',
	`id_parent` INT          NOT NULL COMMENT 'id родителя',
	`id_forum`  INT          NOT NULL COMMENT 'id форума',
	`subject`   VARCHAR(128) NOT NULL COMMENT 'тема',
	`message`   LONGTEXT     NOT NULL COMMENT 'текст сообщения',
	`date`      DATETIME     NOT NULL COMMENT 'дата и время занесения',
	`draft`     TINYINT(1)   NOT NULL COMMENT 'флаг черновика',

	PRIMARY KEY (`id`)

) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='сообщения к отправке';

CREATE TABLE `message_topic`
(
	`id_message`   INT      NOT NULL COMMENT 'id сообщения',
	`id_forum`     INT      NOT NULL COMMENT 'id форума',
	`message_date` DATETIME NOT NULL COMMENT 'дата создания сообщения',

	PRIMARY KEY (`id_message`),

	KEY `ix_id_forum` (`id_forum`),

	CONSTRAINT `fk_message_topic_message` FOREIGN KEY (`id_message`) REFERENCES `message` (`id`) ON DELETE CASCADE ON UPDATE CASCADE

) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='список топиков';

CREATE TABLE `moderate`
(
	`id_message` INT      NOT NULL COMMENT 'id сообщения',
	`id_topic`   INT      NOT NULL COMMENT 'id топика',
	`id_user`    INT      NOT NULL COMMENT 'id пользователя',
	`id_forum`   INT      NOT NULL COMMENT 'id форума',
	`created`    DATETIME NOT NULL COMMENT 'дата',

	PRIMARY KEY (`id_message`, `id_user`)

) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='таблица бомбочек';

CREATE TABLE `moderate2send`
(
	`id`           INT         NOT NULL COMMENT 'id (локальный)',
	`id_message`   INT         NOT NULL COMMENT 'id сообщения',
	`action`       VARCHAR(32) NOT NULL COMMENT 'действие {MoveMessage, DeleteMessage, DeleteThread, DeleteErrorMessage, SplitThread, CloseTopic, OpenTopic}',
	`id_forum`     INT         NOT NULL COMMENT 'id форума для перемещения',
	`description`  LONGTEXT    NOT NULL COMMENT 'описание',
	`as_moderator` TINYINT(1)  NOT NULL COMMENT 'флаг "как модератор"',
	`date`         DATETIME    NOT NULL COMMENT 'дата добавления',

	PRIMARY KEY (`id`)

) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='модерилки к отправке';

CREATE TABLE `rating`
(
	`id_message`  INT        NOT NULL COMMENT 'id сообщения',
	`id_topic`    INT        NOT NULL COMMENT 'id топика',
	`id_user`     INT        NOT NULL COMMENT 'id пользователя',
	`user_rating` INT        NOT NULL COMMENT 'рейтинг пользователя',
	`rate`        INT        NOT NULL COMMENT 'оценка',
	`rate_date`   DATETIME   NOT NULL COMMENT 'дата',
	`rate_type`   TINYINT(1) NOT NULL COMMENT 'тип оценки (0 - смайл, 1 - +/-, 2 - балл)',

	PRIMARY KEY (`id_message`, `id_user`, `rate_type`)

) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='таблица рейтинга';


CREATE TABLE `rating2send`
(
	`id`         INT      NOT NULL COMMENT 'id (локальный)',
	`id_message` INT      NOT NULL COMMENT 'id сообщения',
	`rate`       INT      NOT NULL COMMENT 'рейтинг',
	`date`       DATETIME NOT NULL COMMENT 'дата добавления',

	PRIMARY KEY (`id`)

) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='рейтинг к отправке';

CREATE TABLE `row_version`
(
	`key`   VARCHAR(100) NOT NULL COMMENT 'ключ версии',
	`value` VARCHAR(100) NOT NULL COMMENT 'значение версии',

	PRIMARY KEY (`key`)

) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='версии данных';

CREATE TABLE `subscribed`
(
	`id_forum` INT        NOT NULL COMMENT 'id форума',
	`is_first` TINYINT(1) NOT NULL COMMENT 'была ли раньше подписка на форум true - нет, false - да',

	PRIMARY KEY (`id_forum`)

) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='список форумов, на которые подписан пользователь';

CREATE TABLE `unread`
(
	`id_message`     INT      NOT NULL COMMENT 'id непрочитанного сообщения',
	`id_parent`      INT      NOT NULL COMMENT 'id родительского сообщения',
	`id_forum`       INT      NOT NULL COMMENT 'id форума',
	`id_topic`       INT      NOT NULL COMMENT 'id топика',
	`id_parent_user` INT      NOT NULL COMMENT 'id пользователя, которому идет ответ',
	`message_date`   DATETIME NOT NULL COMMENT 'дата отправки сообщения',

	PRIMARY KEY (`id_message`)

) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='список непрочитанных сообщений';

CREATE TABLE `user`
(
	`id`             INT          NOT NULL COMMENT 'id пользователя',
	`name`           VARCHAR(60)  NOT NULL COMMENT 'логин пользователя',
	`nick`           VARCHAR(100) NOT NULL COMMENT 'псевдоним пользователя',
	`real_name`      VARCHAR(80)  NOT NULL COMMENT 'реальное имя пользователя',
	`email`          VARCHAR(60)  NOT NULL COMMENT 'публичный email пользователя',
	`homepage`       VARCHAR(120) NOT NULL COMMENT 'домашняя страница',
	`specialization` VARCHAR(100) NOT NULL COMMENT 'специализация',
	`where_from`     VARCHAR(100) NOT NULL COMMENT 'откуда',
	`origin`         VARCHAR(258) NOT NULL COMMENT 'подпись',

	PRIMARY KEY (`id`),

	KEY `ix_name` (`name`)

) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='список пользователей';

CREATE TABLE `version`
(
	`key`   VARCHAR(100) NOT NULL COMMENT 'ключ версии',
	`value` VARCHAR(100) NOT NULL COMMENT 'значение версии',

	PRIMARY KEY  (`key`)

) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='версии хранилища';
