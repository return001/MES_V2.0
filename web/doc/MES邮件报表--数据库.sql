/*
Navicat SQL Server Data Transfer

Source Server         : 10.10.11.80（MES）
Source Server Version : 105000
Source Host           : 10.10.11.80:1433
Source Database       : mes_report
Source Schema         : dbo

Target Server Type    : SQL Server
Target Server Version : 105000
File Encoding         : 65001

Date: 2019-04-29 18:00:57
*/


-- ----------------------------
-- Table structure for action_log
-- ----------------------------
DROP TABLE [dbo].[action_log]
GO
CREATE TABLE [dbo].[action_log] (
[id] int NOT NULL IDENTITY(1,1) ,
[ip] varchar(16) NOT NULL ,
[url] varchar(128) NOT NULL ,
[parameters] varchar(2048) NOT NULL ,
[action] varchar(2048) NOT NULL ,
[time] datetime NOT NULL ,
[result_code] int NOT NULL 
)


GO

-- ----------------------------
-- Records of action_log
-- ----------------------------
SET IDENTITY_INSERT [dbo].[action_log] ON
GO
SET IDENTITY_INSERT [dbo].[action_log] OFF
GO

-- ----------------------------
-- Table structure for capacity
-- ----------------------------
DROP TABLE [dbo].[capacity]
GO
CREATE TABLE [dbo].[capacity] (
[id] int NOT NULL IDENTITY(1,1) ,
[orders] varchar(32) NOT NULL ,
[type] int NOT NULL ,
[actual_product] int NOT NULL ,
[hourly_plan_product] int NOT NULL ,
[hourly_actual_product] int NOT NULL 
)


GO
DBCC CHECKIDENT(N'[dbo].[capacity]', RESEED, 1982)
GO

-- ----------------------------
-- Records of capacity
-- ----------------------------
SET IDENTITY_INSERT [dbo].[capacity] ON
GO
SET IDENTITY_INSERT [dbo].[capacity] OFF
GO

-- ----------------------------
-- Table structure for error_log
-- ----------------------------
DROP TABLE [dbo].[error_log]
GO
CREATE TABLE [dbo].[error_log] (
[id] int NOT NULL IDENTITY(1,1) ,
[message] varchar(4096) NOT NULL ,
[time] datetime NOT NULL 
)


GO
DBCC CHECKIDENT(N'[dbo].[error_log]', RESEED, 25)
GO

-- ----------------------------
-- Records of error_log
-- ----------------------------
SET IDENTITY_INSERT [dbo].[error_log] ON
GO
SET IDENTITY_INSERT [dbo].[error_log] OFF
GO

-- ----------------------------
-- Table structure for order_production
-- ----------------------------
DROP TABLE [dbo].[order_production]
GO
CREATE TABLE [dbo].[order_production] (
[id] int NOT NULL IDENTITY(1,1) ,
[time_type] int NOT NULL ,
[plan_product] int NOT NULL ,
[actual_product] int NOT NULL ,
[hourly_plan_product] int NOT NULL ,
[hourly_actual_product] int NOT NULL ,
[time] datetime NOT NULL 
)


GO
DBCC CHECKIDENT(N'[dbo].[order_production]', RESEED, 49)
GO

-- ----------------------------
-- Records of order_production
-- ----------------------------
SET IDENTITY_INSERT [dbo].[order_production] ON
GO
SET IDENTITY_INSERT [dbo].[order_production] OFF
GO

-- ----------------------------
-- Table structure for orders
-- ----------------------------
DROP TABLE [dbo].[orders]
GO
CREATE TABLE [dbo].[orders] (
[id] varchar(32) NOT NULL ,
[order_number] varchar(64) NOT NULL ,
[soft_model] varchar(64) NOT NULL ,
[soft_version] varchar(64) NOT NULL ,
[production_number] varchar(64) NOT NULL ,
[status] int NOT NULL ,
[completion_rate] varchar(8) NULL ,
[plan_product] int NOT NULL ,
[actual_product] int NOT NULL ,
[diffenrence] int NOT NULL ,
[time] datetime NOT NULL 
)


GO

-- ----------------------------
-- Records of orders
-- ----------------------------

-- ----------------------------
-- Table structure for status
-- ----------------------------
DROP TABLE [dbo].[status]
GO
CREATE TABLE [dbo].[status] (
[id] int NOT NULL IDENTITY(1,1) ,
[name] varchar(8) NOT NULL 
)


GO
DBCC CHECKIDENT(N'[dbo].[status]', RESEED, 4)
GO

-- ----------------------------
-- Records of status
-- ----------------------------
SET IDENTITY_INSERT [dbo].[status] ON
GO
INSERT INTO [dbo].[status] ([id], [name]) VALUES (N'1', N'未开始')
GO
GO
INSERT INTO [dbo].[status] ([id], [name]) VALUES (N'2', N'进行中')
GO
GO
INSERT INTO [dbo].[status] ([id], [name]) VALUES (N'3', N'已完成')
GO
GO
INSERT INTO [dbo].[status] ([id], [name]) VALUES (N'4', N'已作废')
GO
GO
SET IDENTITY_INSERT [dbo].[status] OFF
GO

-- ----------------------------
-- Table structure for time_type
-- ----------------------------
DROP TABLE [dbo].[time_type]
GO
CREATE TABLE [dbo].[time_type] (
[id] int NOT NULL IDENTITY(1,1) ,
[name] varchar(8) NOT NULL 
)


GO
DBCC CHECKIDENT(N'[dbo].[time_type]', RESEED, 3)
GO

-- ----------------------------
-- Records of time_type
-- ----------------------------
SET IDENTITY_INSERT [dbo].[time_type] ON
GO
INSERT INTO [dbo].[time_type] ([id], [name]) VALUES (N'1', N'天')
GO
GO
INSERT INTO [dbo].[time_type] ([id], [name]) VALUES (N'2', N'周')
GO
GO
INSERT INTO [dbo].[time_type] ([id], [name]) VALUES (N'3', N'月')
GO
GO
SET IDENTITY_INSERT [dbo].[time_type] OFF
GO

-- ----------------------------
-- Table structure for workstation_production
-- ----------------------------
DROP TABLE [dbo].[workstation_production]
GO
CREATE TABLE [dbo].[workstation_production] (
[id] int NOT NULL IDENTITY(1,1) ,
[time_type] int NOT NULL ,
[type] int NOT NULL ,
[product] int NOT NULL ,
[time] datetime NOT NULL 
)


GO
DBCC CHECKIDENT(N'[dbo].[workstation_production]', RESEED, 367)
GO

-- ----------------------------
-- Records of workstation_production
-- ----------------------------
SET IDENTITY_INSERT [dbo].[workstation_production] ON
GO
SET IDENTITY_INSERT [dbo].[workstation_production] OFF
GO

-- ----------------------------
-- Table structure for workstation_type
-- ----------------------------
DROP TABLE [dbo].[workstation_type]
GO
CREATE TABLE [dbo].[workstation_type] (
[id] int NOT NULL IDENTITY(1,1) ,
[name] varchar(16) NOT NULL 
)


GO
DBCC CHECKIDENT(N'[dbo].[workstation_type]', RESEED, 8)
GO

-- ----------------------------
-- Records of workstation_type
-- ----------------------------
SET IDENTITY_INSERT [dbo].[workstation_type] ON
GO
INSERT INTO [dbo].[workstation_type] ([id], [name]) VALUES (N'1', N'SMT功能测试')
GO
GO
INSERT INTO [dbo].[workstation_type] ([id], [name]) VALUES (N'2', N'功能测试1')
GO
GO
INSERT INTO [dbo].[workstation_type] ([id], [name]) VALUES (N'3', N'老化后测试')
GO
GO
INSERT INTO [dbo].[workstation_type] ([id], [name]) VALUES (N'4', N'耦合测试')
GO
GO
INSERT INTO [dbo].[workstation_type] ([id], [name]) VALUES (N'5', N'机身贴测试')
GO
GO
INSERT INTO [dbo].[workstation_type] ([id], [name]) VALUES (N'6', N'彩盒贴测试')
GO
GO
INSERT INTO [dbo].[workstation_type] ([id], [name]) VALUES (N'7', N'IMEI对比')
GO
GO
INSERT INTO [dbo].[workstation_type] ([id], [name]) VALUES (N'8', N'卡通箱测试')
GO
GO
SET IDENTITY_INSERT [dbo].[workstation_type] OFF
GO

-- ----------------------------
-- Indexes structure for table action_log
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table action_log
-- ----------------------------
ALTER TABLE [dbo].[action_log] ADD PRIMARY KEY ([id])
GO

-- ----------------------------
-- Indexes structure for table capacity
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table capacity
-- ----------------------------
ALTER TABLE [dbo].[capacity] ADD PRIMARY KEY ([id])
GO

-- ----------------------------
-- Indexes structure for table error_log
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table error_log
-- ----------------------------
ALTER TABLE [dbo].[error_log] ADD PRIMARY KEY ([id])
GO

-- ----------------------------
-- Indexes structure for table order_production
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table order_production
-- ----------------------------
ALTER TABLE [dbo].[order_production] ADD PRIMARY KEY ([id])
GO

-- ----------------------------
-- Indexes structure for table orders
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table orders
-- ----------------------------
ALTER TABLE [dbo].[orders] ADD PRIMARY KEY ([id])
GO

-- ----------------------------
-- Indexes structure for table status
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table status
-- ----------------------------
ALTER TABLE [dbo].[status] ADD PRIMARY KEY ([id])
GO

-- ----------------------------
-- Indexes structure for table time_type
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table time_type
-- ----------------------------
ALTER TABLE [dbo].[time_type] ADD PRIMARY KEY ([id])
GO

-- ----------------------------
-- Indexes structure for table workstation_production
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table workstation_production
-- ----------------------------
ALTER TABLE [dbo].[workstation_production] ADD PRIMARY KEY ([id])
GO

-- ----------------------------
-- Indexes structure for table workstation_type
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table workstation_type
-- ----------------------------
ALTER TABLE [dbo].[workstation_type] ADD PRIMARY KEY ([id])
GO
