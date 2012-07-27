/**
 *
 * Схема хранилища SQLite
 *
 **/

BEGIN TRANSACTION;

PRAGMA page_size = 32768;
PRAGMA encoding = "UTF-8";
PRAGMA auto_vacuum = 2;
PRAGMA incremental_vacuum(10);

CREATE TABLE `broken`
(
	`id_message` INTEGER NOT NULL PRIMARY KEY,
	`is_topic`   INTEGER NOT NULL
);

CREATE TABLE `forum`
(
	`id`         INTEGER      NOT NULL PRIMARY KEY,
	`id_group`   INTEGER      NOT NULL,
	`short_name` VARCHAR(64)  NOT NULL,
	`name`       VARCHAR(128) NOT NULL,
	`rated`      INTEGER      NOT NULL,
	`in_top`     INTEGER      NOT NULL,
	`rate_limit` INTEGER      NOT NULL
);

CREATE TABLE `group`
(
	`id`         INTEGER      NOT NULL PRIMARY KEY,
	`name`       VARCHAR(100) NOT NULL,
	`sort_order` INTEGER      NOT NULL
);

CREATE TABLE `message`
(
	`id`               INTEGER      NOT NULL PRIMARY KEY,
	`id_topic`         INTEGER      NOT NULL,
	`id_parent`        INTEGER      NOT NULL,
	`id_user`          INTEGER      NOT NULL,
	`id_forum`         INTEGER      NOT NULL,
	`subject`          VARCHAR(128) NOT NULL,
	`message_name`     VARCHAR(160) NOT NULL,
	`user_nick`        VARCHAR(100) NOT NULL,
	`message`          BLOB         NOT NULL,
	`id_article`       INTEGER      NOT NULL,
	`message_date`     INTEGER      NOT NULL,
	`update_date`      INTEGER      NOT NULL,
	`user_role`        VARCHAR(50)  NOT NULL,
	`user_title`       VARCHAR(100) NOT NULL,
	`user_title_color` INTEGER      NOT NULL,
	`last_moderated`   INTEGER      NOT NULL,
	`closed`           INTEGER      NOT NULL,
	`has_child`        INTEGER      NOT NULL,
	`compressed`       INTEGER      NOT NULL
);

CREATE INDEX `message_ix_id_topic`              ON `message` (`id_topic`);
CREATE INDEX `message_ix_id_parent`             ON `message` (`id_parent`);
CREATE INDEX `message_ix_id_user`               ON `message` (`id_user`);
CREATE INDEX `message_ix_id_forum`              ON `message` (`id_forum`);
CREATE INDEX `message_ix_message_date`          ON `message` (`message_date`);
CREATE INDEX `message_ix_id_forum_message_date` ON `message` (`id_forum`, `message_date`);

CREATE TABLE `message2send`
(
	`id`        INTEGER      NOT NULL PRIMARY KEY,
	`id_parent` INTEGER      NOT NULL,
	`id_forum`  INTEGER      NOT NULL,
	`subject`   VARCHAR(128) NOT NULL,
	`message`   TEXT         NOT NULL,
	`date`      INTEGER      NOT NULL,
	`draft`     INTEGER      NOT NULL
);

CREATE TABLE `message_topic`
(
	`id_message`   INTEGER NOT NULL PRIMARY KEY,
	`id_forum`     INTEGER NOT NULL,
	`message_date` INTEGER NOT NULL,
	FOREIGN KEY (`id_message`) REFERENCES `message` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE INDEX `message_topic_ix_id_forum` ON `message_topic` (`id_forum`);

CREATE TABLE `moderate`
(
	`id_message` INTEGER NOT NULL,
	`id_topic`   INTEGER NOT NULL,
	`id_user`    INTEGER NOT NULL,
	`id_forum`   INTEGER NOT NULL,
	`created`    INTEGER NOT NULL,
	PRIMARY KEY (`id_message`, `id_user`)
);

CREATE TABLE `moderate2send`
(
	`id`           INTEGER     NOT NULL PRIMARY KEY,
	`id_message`   INTEGER     NOT NULL,
	`action`       VARCHAR(32) NOT NULL,
	`id_forum`     INTEGER     NOT NULL,
	`description`  TEXT        NOT NULL,
	`as_moderator` INTEGER     NOT NULL,
	`date`         INTEGER     NOT NULL
);

CREATE TABLE `rating`
(
	`id_message`  INTEGER NOT NULL,
	`id_topic`    INTEGER NOT NULL,
	`id_user`     INTEGER NOT NULL,
	`user_rating` INTEGER NOT NULL,
	`rate`        INTEGER NOT NULL,
	`rate_date`   INTEGER NOT NULL,
	`rate_type`   INTEGER NOT NULL,
	PRIMARY KEY (`id_message`, `id_user`, `rate_type`)
);

CREATE TABLE `rating2send`
(
	`id`         INTEGER NOT NULL PRIMARY KEY,
	`id_message` INTEGER NOT NULL,
	`rate`       INTEGER NOT NULL,
	`date`       INTEGER NOT NULL
);

CREATE TABLE `row_version`
(
	`key`   VARCHAR(100) NOT NULL PRIMARY KEY,
	`value` VARCHAR(100) NOT NULL
);

CREATE TABLE `subscribed`
(
	`id_forum` INTEGER NOT NULL PRIMARY KEY,
	`is_first` INTEGER NOT NULL
);

CREATE TABLE `unread`
(
	`id_message`     INTEGER NOT NULL PRIMARY KEY,
	`id_parent`      INTEGER NOT NULL,
	`id_forum`       INTEGER NOT NULL,
	`id_topic`       INTEGER NOT NULL,
	`id_parent_user` INTEGER NOT NULL,
	`message_date`   INTEGER NOT NULL
);

CREATE TABLE `user`
(
	`id`             INTEGER      NOT NULL PRIMARY KEY,
	`name`           VARCHAR(60)  NOT NULL,
	`nick`           VARCHAR(100) NOT NULL,
	`real_name`      VARCHAR(80)  NOT NULL,
	`email`          VARCHAR(60)  NOT NULL,
	`homepage`       VARCHAR(120) NOT NULL,
	`specialization` VARCHAR(100) NOT NULL,
	`where_from`     VARCHAR(100) NOT NULL,
	`origin`         VARCHAR(258) NOT NULL
);

CREATE INDEX `user_ix_name` ON `user` (`name`);

CREATE TABLE `version`
(
	`key`   VARCHAR(100) NOT NULL PRIMARY KEY,
	`value` VARCHAR(100) NOT NULL
);

ANALYZE;

COMMIT;
