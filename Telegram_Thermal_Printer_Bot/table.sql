CREATE TABLE `entries`(
   command varchar(255) PRIMARY KEY NOT NULL,
   content varchar(255) NOT NULL,
   `status` varchar(255) NOT NULL
);

INSERT INTO `entries`(`command`, `content`, `status`) VALUES ('default' , 'default', 'default')