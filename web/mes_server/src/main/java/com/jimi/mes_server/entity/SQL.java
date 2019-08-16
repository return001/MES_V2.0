package com.jimi.mes_server.entity;

public class SQL {

	public final static String SELECT_USER_BY_NAME_PASSWORD = "SELECT * FROM LUserAccount where Name = ? and Password = ?";

	public final static String SELECT_WEBUSERTYPE = "SELECT * FROM WebUserType where TypeId <> 4";

	public final static String SELECT_TYPENAME_BY_TYPEID = "SELECT * FROM WebUserType where TypeId = ?";

	public final static String SELECT_AUTOTEST_ANTIDUP_SQL_FRAGMENT = "SELECT * FROM Gps_AutoTest_AntiDup where ";
	
	public final static String SELECT_AUTOTEST_RESULT2_SQL_FRAGMENT = "SELECT * FROM Gps_AutoTest_Result2 where ";

	public final static String SELECT_TESTRESULT_SQL_FRAGMENT = "SELECT * FROM Gps_TestResult where ";

	public final static String SELECT_TABLE_DETAIL_SQL_FRAGMENT1 = "SELECT b.* from (SELECT * FROM Gps_AutoTest_AntiDup where ";

	public final static String SELECT_TABLE_DETAIL_SQL_FRAGMENT2 = " ) a INNER JOIN Gps_AutoTest_Result2 b on a.SN = b.SN and a.Version = b.Version and a.softModel = b.softModel";

	public final static String SELECT_TABLE_DETAIL_SQL_FRAGMENT3 = " ) a INNER JOIN Gps_TestResult b on a.SN = b.SN and a.Version = b.Version and a.softModel = b.softModel";

	public final static String DELETE_AUTOTEST_ANTIDUP_SQL_FRAGMENT = "DELETE from Gps_AutoTest_AntiDup where ";

	public final static String DELETE_AUTOTEST_RESULT2_SQL_FRAGMENT1 = "DELETE from Gps_AutoTest_Result2 where ";

	public final static String UPDATE_AUTOTEST_RESULT2_FUNCTIONRESULT_SQL_FRAGMENT1 = "UPDATE Gps_TestResult set FunctionResult = 0 where ";

	public final static String SELECT_DISTINCT_AUTOTEST_ANTIDUP_BY_MAC_SQL_FRAGMENT = "SELECT DISTINCT SN,Version,SoftModel FROM Gps_AutoTest_AntiDup where ";

	public final static String DELETE_AUTOTEST_RESULT2_SQL_FRAGMENT2 = "DELETE Gps_AutoTest_Result2 FROM Gps_AutoTest_AntiDup, Gps_AutoTest_Result2 WHERE Gps_AutoTest_AntiDup.SN = Gps_AutoTest_Result2.SN AND Gps_AutoTest_AntiDup.Version = Gps_AutoTest_Result2.Version AND Gps_AutoTest_AntiDup.softModel = Gps_AutoTest_Result2.softModel AND ";

	public final static String SELECT_IMEI_BEGIN_END_BY_ZHIDAN = "SELECT DISTINCT IMEIStart,IMEIEnd from Gps_ManuPrintParam where ZhiDan like ?";

	public final static String SELECT_SHORT_IMEI_BY_ZHIDAN = "SELECT SUBSTRING(IMEI,1,14) as realIMEI from Gps_ManuPrintParam where ZhiDan like ? ORDER BY realIMEI";

	public final static String SELECT_LONG_IMEI_BY_ZHIDAN = "SELECT IMEI as realIMEI from Gps_ManuPrintParam where ZhiDan like ? ORDER BY realIMEI";

	public final static String SELECT_ZHIDAN_IMEI_INFO_BY_ZHIDAN = "SELECT ZhiDan,IMEIStart,IMEIEnd from Gps_ManuPrintParam where ZhiDan like ? GROUP BY ZhiDan,IMEIStart,IMEIEnd";

	public final static String SELECT_DATARELATIVESHEET_SQL_FRAGMENT = "SELECT * from DataRelativeSheet where ";

	public final static String SELECT_DATARELATIVEUNIQUE_SQL_FRAGMENT = "SELECT * FROM DataRelativeUnique where ";

	public final static String SELECT_AUTOTEST_RESULT_SQL_FRAGMENT = "SELECT * FROM Gps_AutoTest_Result where ";

	public final static String SELECT_AUTOTEST_RESULT3_SQL_FRAGMENT = "SELECT * FROM Gps_AutoTest_Result3 where ";

	public final static String SELECT_CARTONBOX_RESULT_SQL_FRAGMENT = "SELECT * FROM Gps_CartonBoxTwenty_Result where ";

	public final static String SELECT_COUPLE_RESULT_SQL_FRAGMENT = "SELECT * FROM Gps_CoupleTest_Result where ";

	public final static String SELECT_MANUCP_RESULT_SQL_FRAGMENT = "SELECT * FROM Gps_ManuCpParam where ";

	public final static String SELECT_MANUPRINT_RESULT_SQL_FRAGMENT = "SELECT * FROM Gps_ManuPrintParam where ";

	public final static String SELECT_TEST_RESULT_SQL_FRAGMENT = "SELECT * FROM Gps_TestResult where ";

	public final static String SELECT_NETMARKIMEI_RESULT_SQL_FRAGMENT = "SELECT * FROM NetMarkIMEI where ";

	public final static String SELECT_MANUSIM_RESULT_SQL_FRAGMENT = "SELECT * FROM Gps_ManuSimDataParam where ";

	public final static String DELETE_DATARELATIVESHEET_SQL_FRAGMENT = "DELETE from DataRelativeSheet where ";

	public final static String DELETE_DATARELATIVEUNIQUE_SQL_FRAGMENT = "DELETE FROM DataRelativeUnique where ";

	public final static String DELETE_AUTOTEST_RESULT_SQL_FRAGMENT = "DELETE FROM Gps_AutoTest_Result where ";

	public final static String DELETE_AUTOTEST_RESULT3_SQL_FRAGMENT = "DELETE FROM Gps_AutoTest_Result3 where ";

	public final static String DELETE_CARTONBOX_RESULT_SQL_FRAGMENT = "DELETE FROM Gps_CartonBoxTwenty_Result where ";

	public final static String DELETE_COUPLE_RESULT_SQL_FRAGMENT = "DELETE FROM Gps_CoupleTest_Result where ";

	public final static String DELETE_MANUPRINT_RESULT_SQL_FRAGMENT = "DELETE FROM Gps_ManuPrintParam where ";

	public final static String DELETE_TEST_RESULT_SQL_FRAGMENT = "DELETE FROM Gps_TestResult where ";

	public final static String DELETE_NETMARKIMEI_RESULT_SQL_FRAGMENT = "DELETE FROM NetMarkIMEI where ";

	public final static String DELETE_MANUSIM_RESULT_SQL_FRAGMENT = "DELETE FROM Gps_ManuSimDataParam where ";

	public final static String SELECT_DATARELATIVESHEET_AND_CARTONBOX ="SELECT Gps_CartonBoxTwenty_Result.Id AS GpsCartonBoxTwentyResult_Id,Gps_CartonBoxTwenty_Result.BoxNo AS GpsCartonBoxTwentyResult_BoxNo,Gps_CartonBoxTwenty_Result.IMEI AS GpsCartonBoxTwentyResult_IMEI,Gps_CartonBoxTwenty_Result.ZhiDan AS GpsCartonBoxTwentyResult_ZhiDan,Gps_CartonBoxTwenty_Result.SoftModel AS GpsCartonBoxTwentyResult_SoftModel,Gps_CartonBoxTwenty_Result.Version AS GpsCartonBoxTwentyResult_Version,Gps_CartonBoxTwenty_Result.ProductCode AS GpsCartonBoxTwentyResult_ProductCode,Gps_CartonBoxTwenty_Result.Color AS GpsCartonBoxTwentyResult_Color,Gps_CartonBoxTwenty_Result.Qty AS GpsCartonBoxTwentyResult_Qty,Gps_CartonBoxTwenty_Result.Weight AS GpsCartonBoxTwentyResult_Weight,Gps_CartonBoxTwenty_Result.[Date] AS GpsCartonBoxTwentyResult_Date,Gps_CartonBoxTwenty_Result.TACInfo AS GpsCartonBoxTwentyResult_TACInfo,Gps_CartonBoxTwenty_Result.CompanyName AS GpsCartonBoxTwentyResult_CompanyName,Gps_CartonBoxTwenty_Result.TesterId AS GpsCartonBoxTwentyResult_TesterId,Gps_CartonBoxTwenty_Result.TestTime AS GpsCartonBoxTwentyResult_TestTime,Gps_CartonBoxTwenty_Result.Remark1 AS GpsCartonBoxTwentyResult_Remark1,Gps_CartonBoxTwenty_Result.Remark2 AS GpsCartonBoxTwentyResult_Remark2,Gps_CartonBoxTwenty_Result.Remark3 AS GpsCartonBoxTwentyResult_Remark3,Gps_CartonBoxTwenty_Result.Remark4 AS GpsCartonBoxTwentyResult_Remark4,Gps_CartonBoxTwenty_Result.Remark5 AS GpsCartonBoxTwentyResult_Remark5,Gps_CartonBoxTwenty_Result.Computer AS GpsCartonBoxTwentyResult_Computer,DataRelativeSheet.sN AS DataRelativeSheet_SN,DataRelativeSheet.iMEI1 AS DataRelativeSheet_IMEI1,DataRelativeSheet.iMEI2 AS DataRelativeSheet_IMEI2,DataRelativeSheet.iMEI3 AS DataRelativeSheet_IMEI3,DataRelativeSheet.iMEI4 AS DataRelativeSheet_IMEI4,DataRelativeSheet.iMEI5 AS DataRelativeSheet_IMEI5,DataRelativeSheet.iMEI6 AS DataRelativeSheet_IMEI6,DataRelativeSheet.iMEI7 AS DataRelativeSheet_IMEI7,DataRelativeSheet.iMEI8 AS DataRelativeSheet_IMEI8,DataRelativeSheet.iMEI9 AS DataRelativeSheet_IMEI9,DataRelativeSheet.iMEI10 AS DataRelativeSheet_IMEI10,DataRelativeSheet.iMEI11 AS DataRelativeSheet_IMEI11,DataRelativeSheet.iMEI12 AS DataRelativeSheet_IMEI12,DataRelativeSheet.iMEI13 AS DataRelativeSheet_IMEI13,DataRelativeSheet.zhiDan AS DataRelativeSheet_ZhiDan,DataRelativeSheet.testTime AS DataRelativeSheet_TestTime,DataRelativeSheet.simEffectiveDate AS DataRelativeSheet_SimEffectiveDate FROM Gps_CartonBoxTwenty_Result JOIN DataRelativeSheet ON DataRelativeSheet.IMEI1 = Gps_CartonBoxTwenty_Result.IMEI ";

	public final static String SELECT_PROCESSGROUP_BY_GROUPNAME = "SELECT * from process_group WHERE group_name = ?";

	public final static String SELECT_PROCESSGROUP_BY_GROUPNO = "SELECT * from process_group WHERE group_no = ?";

	public final static String SELECT_LINE_BY_LINENAME = "SELECT * from line WHERE line_name = ?";

	public final static String SELECT_LINE_BY_LINENO = "SELECT * from line WHERE line_no = ?";

	public final static String SELECT_PROCESS_BY_PROCESSNAME = "SELECT * from process WHERE process_name = ?";

	public final static String SELECT_PROCESS_BY_PROCESSNO = "SELECT * from process WHERE process_no = ?";

	public final static String SELECT_ORDER_BY_ZHIDAN = "SELECT * from orders WHERE zhidan = ?";

	public final static String SELECT_ORDERFILE_BY_ORDER = "SELECT order_file.id, file_name AS fileName, file_type AS fileType, file_type.type_name AS typeName, LUserAccount.Name AS uploaderName, upload_time AS uploadTime FROM order_file, file_type, LUserAccount, orders WHERE file_type.id = order_file.file_type AND LUserAccount.Id = order_file.uploader AND orders.id = order_file.orders AND order_file.orders = ? ORDER BY file_type";

	public final static String SELECT_MODELCAPACITY_BY_MODEL_PROCESS = "SELECT * from model_capacity WHERE soft_model = ? and process = ? and process_group = ?";

	public final static String SELECT_CARTONNUM_BY_TESTTIME = "SELECT count(*) FROM Gps_CartonBoxTwenty_Result WHERE TestTime > ? AND TestTime < ?";

	public final static String SELECT_ORDER_BY_STATUS_ISREWORK = "SELECT * FROM orders WHERE is_rework = ? and (order_status = ? or order_status = ? ) ";

	public final static String SELECT_PROCESS_PROCESSGROUP = "SELECT process.id,process_no as processNo,process_name as processName,process_remark as processRemark,process_group as processGroup,process_group.group_no as groupNo,process_group.group_name as groupName from process, process_group where process.process_group = process_group.id ";

	public final static String SELECT_LINE_PROCESSGROUP = "SELECT line.id, line_no AS 'lineNo', line_name AS lineName, line_remark AS lineRemark, line_director AS lineDirector, line_engineer AS lineEngineer, line_qc AS lineQc, process_group AS processGroup, process_group.group_name AS groupName, a.Name AS directorName, b.Name AS engineerName, c.Name AS qcName FROM line, process_group, LUserAccount a, LUserAccount b, LUserAccount c WHERE line.process_group = process_group.id AND a.Id = line.line_director AND b.Id = line.line_engineer AND c.Id = line_qc ";

	public final static String UPDATE_MODELCAPACITY_POSITION = "UPDATE model_capacity SET [position] = ? where id = ?";

	public final static String SELECT_MODELCAPACITY = "SELECT model_capacity.id,soft_model as softModel,customer_model as customerModel,process,model_capacity.process_group as processGroup,process_people_quantity as processPeopleQuantity,capacity,remark,model_capacity.[position], process_group.group_name as groupName, process.process_name as processName FROM model_capacity, process, process_group WHERE model_capacity.process = process.id AND model_capacity.process_group = process_group.id ";

	public final static String SELECT_SCHEDULED_ORDER_AND_QUANTITY = "SELECT orders, SUM(scheduling_quantity) AS scheduled_quantity FROM scheduling_plan WHERE process_group = ? AND ( scheduling_plan_status = ? OR scheduling_plan_status = ? OR scheduling_plan_status = ? ) GROUP BY orders ";

	public final static String SELECT_MODELCAPACITY_BY_ORDER_PROCESS = "SELECT * from model_capacity WHERE process_group = ? and process = ? AND soft_model LIKE ? ";

	public final static String SELECT_PEOPLE_CAPACITY_BY_SOFTMODEL_PROCESSGROUP = "SELECT sum(process_people_quantity) as people,sum(capacity) as capacity FROM model_capacity WHERE process_group = ? AND soft_model = ? ";

	public final static String SELECT_SCHEDULINGPLAN = "SELECT scheduling_plan.id, scheduling_plan.orders, is_urgent AS isUrgent, scheduling_plan.process_group AS processGroup, line, scheduling_quantity AS schedulingQuantity, capacity, production_planning_number AS productionPlanningNumber, plan_start_time AS planStartTime, plan_complete_time AS planCompleteTime, start_time AS startTime, complete_time AS completeTime, produced_quantity producedQuantity, remaining_quantity AS remainingQuantity, is_timeout AS isTimeout, scheduling_plan_status AS schedulingPlanStatus, scheduling_plan.remark, remaining_reason AS remainingReason, line.line_no AS 'lineNo', line.line_name AS lineName, scheduling_plan_status.status_name AS statusName, zhidan,alias,soft_model as softModel,product_no as productNo,customer_number as customerNumber,customer_name as customerName,order_date as orderDate,delivery_date as deliveryDate,line_change_time as lineChangeTime FROM scheduling_plan, orders, line, scheduling_plan_status WHERE scheduling_plan.orders = orders.id AND scheduling_plan.line = line.id AND scheduling_plan.scheduling_plan_status = scheduling_plan_status.id ";

	public final static String SELECT_USER_NAME_ID_BY_NAME = "SELECT Name as name, Id as id, UserDes as userDes from LUserAccount WHERE InService = 1 and Name LIKE ?";

	public final static String SELECT_PROCESS_NAME_ID = "SELECT id, process_name as processName, process_group as processGroup from process";

	public final static String SELECT_PROCESSGROUP_NAME_ID = "SELECT id, group_name as groupName from process_group";

	public final static String SELECT_LINE_NAME_ID = "SELECT id, line_name as lineName, process_group as processGroup FROM line";

	public final static String SELECT_ORDERSTATUS_NAME_ID = "SELECT id, status_name as statusName FROM order_status";

	public final static String SELECT_SCHEDULINGPLANSTATUS_NAME_ID = "SELECT id, status_name as statusName from scheduling_plan_status";

	public final static String SELECT_ORDER = "SELECT orders.id, zhidan, alias, soft_model AS softModel, version, product_no AS productNo, customer_name AS customerName, customer_number AS customerNumber, order_date AS orderDate, quantity, delivery_date AS deliveryDate, remark, order_status AS orderStatus, delete_reason AS deleteReason, order_status.status_name AS statusName FROM orders, order_status WHERE orders.order_status = order_status.id ";

	public final static String SELECT_PROCESSGROUP = "SELECT id,group_no as groupNo,group_name as groupName,group_remark as groupRemark FROM process_group";

	public final static String SELECT_LINECOMPUTER_BY_LINE = "SELECT id,computer_name as computerName,remark,line,ip from line_computer WHERE line = ? ";

	public final static String SELECT_LINECOMPUTER_BY_IP = "SELECT * from line_computer WHERE ip = ? ";

	public final static String SELECT_ORDERDETAIL_BY_ID = "SELECT order_creator as orderCreator,LUserAccount.UserDes as creatorName,order_create_time as orderCreateTime,order_modifier as orderModifier,NULL as modifierName,order_modify_time as orderModifyTime,delete_person as deletePerson,NULL as deletePersonName,delete_time as deleteTime from orders JOIN LUserAccount on LUserAccount.Id = orders.order_creator WHERE orders.id = ?";

	public final static String SELECT_SCHEDULINGPLAN_BY_ORDERID = "SELECT COUNT(*) FROM scheduling_plan WHERE orders = ?";

	public final static String SELECT_SCHEDULINGPLANDETAIL_BY_ID = "SELECT scheduler, LUserAccount.UserDes AS schedulerName, scheduling_time AS schedulingTime, plan_modifier AS planModifier, NULL AS modifierName, plan_modify_time as planModifyTime, production_confirmer AS productionConfirmer, NULL AS confirmerName, line_change_time as lineChangeTime FROM scheduling_plan JOIN LUserAccount ON LUserAccount.Id = scheduling_plan.scheduler WHERE scheduling_plan.id = ?";

	public final static String SELECT_ASSEMBLING_PROCESS_GROUP_PRUDUCEDQUANTITY = "SELECT count(*) FROM Gps_AutoTest_Result WHERE ZhiDan = ? and SoftModel = ? and Computer like ? ";

	public final static String SELECT_TESTING_PROCESS_GROUP_PRUDUCEDQUANTITY = "SELECT count(*) FROM Gps_CoupleTest_Result WHERE ZhiDan = ? and SoftModel = ? and Computer like ? ";

	public final static String SELECT_PACKING_PROCESS_GROUP_PRUDUCEDQUANTITY = "SELECT count(*) FROM Gps_CartonBoxTwenty_Result WHERE ZhiDan = ? and SoftModel = ? and Computer like ? ";

	public final static String SELECT_REWORK_ORDER = "SELECT * FROM orders WHERE is_rework = 1 and order_status = 1 or order_status = 2";

	public final static String SELECT_PRODUCEDQUANTITY_BY_ORDER = "SELECT * FROM scheduling_plan WHERE scheduling_plan_status = 3 AND orders = ?";

	public final static String SELECT_PRODUCEDQUANTITY_BY_ORDER_PROCESSGROUP = "SELECT * FROM scheduling_plan WHERE scheduling_plan_status = 3 AND orders = ? AND process_group = ?";

	public final static String SELECT_AUTOTEST_PRODUCTION_BY_TESTTIME = "SELECT ZhiDan,SoftModel,count(*) as Production FROM Gps_AutoTest_Result WHERE TestTime > ? and TestTime < ? GROUP BY ZhiDan,SoftModel ";

	public final static String SELECT_COUPLETEST_PRODUCTION_BY_TESTTIME = "SELECT ZhiDan,SoftModel,count(*) as Production FROM Gps_CoupleTest_Result WHERE TestTime > ? and TestTime < ? GROUP BY ZhiDan,SoftModel ";

	public final static String SELECT_CARTONTEST_PRODUCTION_BY_TESTTIME = "SELECT ZhiDan,SoftModel,count(*) as Production FROM Gps_CartonBoxTwenty_Result WHERE TestTime > ? and TestTime < ? GROUP BY ZhiDan,SoftModel ";

	public final static String SELECT_DASHBOARD = "SELECT id,zhidan,soft_model as softModel,plan_production as planProduction,actual_production as actualProduction,cast( convert (decimal(4,0),100*completion_rate ) as varchar)+'%' as completionRate,cast( convert (decimal(4,0),100*testing_rate ) as varchar)+'%' as testingRate,start_time as startTime,end_time as endTime,remark,'' as time FROM dashboard WHERE line = ? and end_time > ? and end_time < ? order by start_time";

	public final static String SELECT_PLAN_INFORMATION = "SELECT orders.ZhiDan,orders.soft_model,scheduling_quantity,line_computer.ip FROM scheduling_plan,orders,line_computer WHERE orders.id = scheduling_plan.orders and line_computer.line= scheduling_plan.line and scheduling_plan.id = ? ";

	public final static String SELECT_FUNCTION_ERROR = "SELECT '设备信息' AS errorName1, num1 + num2 AS errorNum1, '电源' AS errorName2, num3 + num4 + num8 + num12 AS errorNum2, '通信' AS errorName3, num5 + num9 + num10 AS errorNum3, 'GPS' AS errorName4, num6 AS errorNum4, '其他' AS errorName5, num7 + num11 AS errorNum5, num1 + num2 + num3 + num4 + num5 + num6 + num8 + num9 + num10 + num12 AS errorNumSum FROM ( SELECT SUM ( CASE WHEN A.errorName LIKE '%芯片ID%' THEN 1 ELSE 0 END ) AS num1, SUM ( CASE WHEN A.errorName LIKE '%软件版本%' THEN 1 ELSE 0 END ) AS num2, SUM ( CASE WHEN A.errorName LIKE '%只有电池%' THEN 1 ELSE 0 END ) AS num3, SUM ( CASE WHEN A.errorName LIKE '%只有电压%' THEN 1 ELSE 0 END ) AS num4, SUM ( CASE WHEN A.errorName LIKE '%WIFI%' THEN 1 ELSE 0 END ) AS num5, SUM ( CASE WHEN A.errorName LIKE '%GPS%' THEN 1 ELSE 0 END ) AS num6, SUM ( CASE WHEN A.errorName LIKE '%光敏%' THEN 1 ELSE 0 END ) AS num7, SUM ( CASE WHEN A.errorName LIKE '%外电%' THEN 1 ELSE 0 END ) AS num8, SUM ( CASE WHEN A.errorName LIKE '%信号强度%' THEN 1 ELSE 0 END ) AS num9, SUM ( CASE WHEN A.errorName LIKE '%电话%' THEN 1 ELSE 0 END ) AS num10, SUM ( CASE WHEN A.errorName LIKE '%蜂鸣器%' THEN 1 ELSE 0 END ) AS num11, SUM ( CASE WHEN A.errorName LIKE '%电池电压%' THEN 1 ELSE 0 END ) AS num12 FROM ( SELECT SUBSTRING ( ErrorMessage1, charindex('+++', ErrorMessage1) + 3, charindex('->', ErrorMessage1) - 6 ) AS errorName FROM LTestLogMessage WHERE TestTime >= ? AND TestTime < ? AND ErrorMessage1 IS NOT NULL ) A ) B";

	public final static String SELECT_COUPLE_ERROR = "SELECT B.*, errorNum1 + errorNum2 + errorNum3 AS errorNumSum FROM (SELECT 'RF耦合' as errorName1,sum(case WHEN A.errorName like '%RF耦合指标测试失败%' then 1 ELSE 0 END) as errorNum1 , 'IMEI对比' as errorName2,sum(case WHEN A.errorName like '%IMEI 不一致%' then 1 ELSE 0 END) as errorNum2, '其他' as errorName3,sum(case WHEN A.errorName like '%数据库上传失败%' then 1 ELSE 0 END) as errorNum3, '返回值有误' as errorName4,0 as errorNum4 FROM (SELECT SUBSTRING (ErrorMessage2, charindex('+++', ErrorMessage2) + 3, charindex('->', ErrorMessage2) - 6 ) AS errorName FROM LTestLogMessage WHERE TestTime >= ? AND TestTime < ? AND ErrorMessage2 IS NOT NULL) A ) B";

	public final static String SELECT_CARTON_ERROR = "SELECT '号码重复' as errorName1,sum(case WHEN A.errorName like '%号码重复%' then 1 ELSE 0 END) as errorNum1 , '工位漏测' as errorName2,sum(case WHEN A.errorName like '%工位漏测%' then 1 ELSE 0 END) as errorNum2, '箱号重复' as errorName3,sum(case WHEN A.errorName like '%箱号重复%' then 1 ELSE 0 END) as errorNum3 FROM ( SELECT SUBSTRING ( ErrorMessage4, charindex('+++', ErrorMessage4) + 3, charindex('->', ErrorMessage4) - 6 ) AS errorName FROM LTestLogMessage WHERE TestTime >= ? AND TestTime < ? AND ErrorMessage4 IS NOT NULL ) A ";

	public final static String SELECT_FUNCTION_ERRORNUM_BY_TESTTIME = "SELECT count(*) FROM LTestLogMessage WHERE TestTime >= ? AND TestTime < ? AND ErrorMessage1 IS NOT NULL ";

	public final static String SELECT_COUPLE_ERRORNUM_BY_TESTTIME = "SELECT count(*) FROM LTestLogMessage WHERE TestTime >= ? AND TestTime < ? AND ErrorMessage2 IS NOT NULL ";

	public final static String SELECT_FUNCTION_ERRORNUM_BY_ZHIDAN = "SELECT count(*) FROM LTestLogMessage WHERE TestTime >= ? AND TestTime < ? AND ZhiDan = ? AND SoftModel = ? AND ErrorMessage1 IS NOT NULL ";

	public final static String SELECT_COUPLE_ERRORNUM_BY_ZHIDAN = "SELECT count(*) FROM LTestLogMessage WHERE TestTime >= ? AND TestTime < ? AND ZhiDan = ? AND SoftModel = ? AND ErrorMessage2 IS NOT NULL ";

	public final static String SELECT_CARTON_ERRORNUM_BY_ZHIDAN = "SELECT count(*) FROM LTestLogMessage WHERE TestTime >= ? AND TestTime < ? AND ZhiDan = ? AND SoftModel = ? AND ErrorMessage4 IS NOT NULL ";

	public final static String SELECT_LAST_TEST_FRAGMENT_TWO = " ORDER BY TestTime ASC) TEST ORDER BY TEST.TestTime DESC";

	public final static String SELECT_SMTTEST_NUMBER_BY_ZHIDAN_COMPUTER = "SELECT count(*) from Gps_AutoTest_Result2 WHERE ZhiDan = ? ";

	public final static String SELECT_FIRST_SMTTEST = "SELECT TOP 1 * FROM Gps_AutoTest_Result2 WHERE ZhiDan = ? ";

	public final static String SELECT_LAST_SMTTEST_FRAGMENT_ONE = "SELECT TOP 1 * FROM (SELECT TOP ? * FROM Gps_AutoTest_Result2 WHERE ZhiDan = ? ";

	public final static String SELECT_FUNCTIONTEST_NUMBER_BY_ZHIDAN_COMPUTER = "SELECT count(*) from Gps_AutoTest_Result WHERE ZhiDan = ? ";

	public final static String SELECT_FIRST_FUNCTIONTEST = "SELECT TOP 1 * FROM Gps_AutoTest_Result WHERE ZhiDan = ? ";

	public final static String SELECT_LAST_FUNCTIONTEST_FRAGMENT_ONE = "SELECT TOP 1 * FROM (SELECT TOP ? * FROM Gps_AutoTest_Result WHERE ZhiDan = ? ";

	public final static String SELECT_AGEDTEST_NUMBER_BY_ZHIDAN_COMPUTER = "SELECT count(*) from Gps_AutoTest_Result3 WHERE ZhiDan = ? ";

	public final static String SELECT_FIRST_AGEDTEST = "SELECT TOP 1 * FROM Gps_AutoTest_Result3 WHERE ZhiDan = ? ";

	public final static String SELECT_LAST_AGEDTEST_FRAGMENT_ONE = "SELECT TOP 1 * FROM (SELECT TOP ? * FROM Gps_AutoTest_Result3 WHERE ZhiDan = ? ";

	public final static String SELECT_COUPLETEST_NUMBER_BY_ZHIDAN_COMPUTER = "SELECT count(*) from Gps_CoupleTest_Result WHERE ZhiDan = ? ";

	public final static String SELECT_FIRST_COUPLETEST = "SELECT TOP 1 * FROM Gps_CoupleTest_Result WHERE ZhiDan = ? ";

	public final static String SELECT_LAST_COUPLETEST_FRAGMENT_ONE = "SELECT TOP 1 * FROM (SELECT TOP ? * FROM Gps_CoupleTest_Result WHERE ZhiDan = ? ";

	public final static String SELECT_CARTONTEST_NUMBER_BY_ZHIDAN_COMPUTER = "SELECT count(*) from Gps_CartonBoxTwenty_Result WHERE ZhiDan = ? ";

	public final static String SELECT_FIRST_CARTONTEST = "SELECT TOP 1 * FROM Gps_CartonBoxTwenty_Result WHERE ZhiDan = ? ";

	public final static String SELECT_LAST_CARTONTEST_FRAGMENT_ONE = "SELECT TOP 1 * FROM (SELECT TOP ? * FROM Gps_CartonBoxTwenty_Result WHERE ZhiDan = ? ";

	public final static String SELECT_SCHEDULED_REWORK_ORDER_QUANTITY = "SELECT SUM (scheduling_quantity) AS scheduled_quantity FROM scheduling_plan INNER JOIN orders ON scheduling_plan.orders = orders.id WHERE process_group = ? AND orders.order_status = 2 AND is_rework = 1 AND orders = ? ";

	public final static String SELECT_SCHEDULED_ORDER_QUANTITY = "SELECT SUM (scheduling_quantity) AS scheduled_quantity FROM scheduling_plan WHERE process_group = ? AND orders = ? ";

	public final static String SELECT_DISTINCT_ORDER_BY_PROCESSGROUP_ORDERSTATUS = "SELECT scheduling_plan.orders FROM scheduling_plan INNER JOIN orders on scheduling_plan.orders = orders.id WHERE process_group = ? and is_rework = 0 and (order_status = ? or order_status = ? ) GROUP BY orders";

	public final static String UPDATE_PROCESSGROUP_POSITION = "UPDATE process_group SET [position] = ? where id = ?";

	public final static String UPDATE_PROCESS_POSITION = "UPDATE process SET [position] = ? where id = ?";

	public final static String SELECT_CARTONTEST_NUMBER_BY_ZHIDAN_SOFTMODEL = "SELECT count(*) FROM Gps_CartonBoxTwenty_Result WHERE ZhiDan = ? and SoftModel = ?";
}
