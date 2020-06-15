/*
SQLyog Professional v12.08 (64 bit)
MySQL - 5.6.37 : Database - qtqq
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`qtqq` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `qtqq`;

/*Table structure for table `tab_accounts` */

DROP TABLE IF EXISTS `tab_accounts`;

CREATE TABLE `tab_accounts` (
  `employeeID` int(11) NOT NULL COMMENT '员工工号',
  `account` char(20) NOT NULL COMMENT '账号',
  `code` char(20) NOT NULL COMMENT '密码',
  PRIMARY KEY (`account`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `tab_accounts` */

insert  into `tab_accounts`(`employeeID`,`account`,`code`) values (10002,'rener','rener'),(10003,'rensan','rensan'),(10001,'renyi','renyi'),(10009,'sersan','shisan'),(10008,'shier','shier'),(10007,'shiyi','shiyi'),(10005,'yaner','yaner'),(10006,'yansan','yansan'),(10004,'yanyi','yanyi');

/*Table structure for table `tab_department` */

DROP TABLE IF EXISTS `tab_department`;

CREATE TABLE `tab_department` (
  `departmentID` int(11) NOT NULL AUTO_INCREMENT COMMENT '部门编号',
  `department_name` char(20) NOT NULL COMMENT '部门名称',
  `picture` char(50) NOT NULL COMMENT '头像路径',
  `sign` char(50) NOT NULL COMMENT '部门口号',
  PRIMARY KEY (`departmentID`)
) ENGINE=InnoDB AUTO_INCREMENT=2004 DEFAULT CHARSET=utf8;

/*Data for the table `tab_department` */

insert  into `tab_department`(`departmentID`,`department_name`,`picture`,`sign`) values (2000,'公司群','D:\\qtqq_images\\gsq.png','越分享月拥有'),(2001,'人事部','D:\\qtqq_images\\rsb.png','公心、民心、爱心、事事在心'),(2002,'研发部','D:\\qtqq_images\\yfb.png','只有两种编程语言:一种是天天挨骂的,另一种是没人用的'),(2003,'市场部','D:\\qtqq_images\\scb.png','今天工作不努力,明天努力找工作');

/*Table structure for table `tab_employees` */

DROP TABLE IF EXISTS `tab_employees`;

CREATE TABLE `tab_employees` (
  `departmentID` int(11) NOT NULL COMMENT '部门编号',
  `employeeID` int(11) NOT NULL AUTO_INCREMENT COMMENT '员工ID',
  `employee_name` char(20) NOT NULL COMMENT '员工姓名',
  `employee_sign` char(200) DEFAULT NULL COMMENT '员工签名',
  `status` tinyint(4) NOT NULL DEFAULT '1' COMMENT '1  正常   0 注销',
  `picture` char(250) NOT NULL COMMENT '头像路径',
  `online` tinyint(4) NOT NULL DEFAULT '1' COMMENT '是否在线  1 离线 2 在线',
  PRIMARY KEY (`employeeID`)
) ENGINE=InnoDB AUTO_INCREMENT=10012 DEFAULT CHARSET=utf8;

/*Data for the table `tab_employees` */

insert  into `tab_employees`(`departmentID`,`employeeID`,`employee_name`,`employee_sign`,`status`,`picture`,`online`) values (2001,10001,'renyi','风一样的女子',0,'D:\\qtqq_images\\r1.png',1),(2001,10002,'renwer','奋斗中...',1,'D:\\qtqq_images\\r2.png',1),(2001,10003,'rensan','加油...',1,'D:\\qtqq_images\\r3.png',1),(2002,10004,'yanyi','调bug中...',1,'D:\\qtqq_images\\y1.png',1),(2002,10005,'yaner','改bug中...',1,'D:\\qtqq_images\\y2.png',1),(2002,10006,'yansan','写bug 中...',1,'D:\\qtqq_images\\y3.png',1),(2003,10007,'shiyi','跑业务中',1,'D:\\qtqq_images\\s1.png',1),(2003,10008,'shier','目标明确,鉴定不易',1,'D:\\qtqq_images\\s2.png',1),(2003,10009,'shisan','周末看海去',0,'D:\\qtqq_images\\s3.png',1),(2001,10010,'小白',NULL,1,'D:/Code/VS2017/source/repos/QtQQ_Server/QtQQ_Server/fly2.png',1),(2001,10011,'小白兔',NULL,1,'E:/Download/Picture/picsartforkids_128px_1091451_easyicon.net.png',1);

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
