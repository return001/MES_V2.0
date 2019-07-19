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

	public final static String SELECT_ORDERFILE_BY_ORDER = "SELECT order_file.id, file_name AS fileName, file_type AS fileType, file_type.type_name AS typeName,LUserAccount.Name as uploaderName,upload_time as uploadTime FROM order_file, file_type ,LUserAccount WHERE file_type.id = order_file.file_type and LUserAccount.Id = order_file.uploader ";

	public final static String SELECT_ORDERFILE_BY_ORDER_FILETYPE = "SELECT order_file.id, file_name AS fileName, file_type AS fileType, file_type.type_name AS typeName,LUserAccount.Name as uploaderName,upload_time as uploadTime FROM order_file, file_type ,LUserAccount WHERE orders = ? and file_type = ? ";

	public final static String SELECT_MODELCAPACITY_BY_MODEL_PROCESS = "SELECT * from model_capacity WHERE soft_model = ? and process = ? and process_group = ?";

	public final static String SELECT_CARTONNUM_BY_TESTTIME = "SELECT count(*) FROM Gps_CartonBoxTwenty_Result WHERE TestTime > ? AND TestTime < ?";

	public final static String SELECT_ZHIDAN_VERSION_SOFTMODEL_BY_TESTTIME = "SELECT ZhiDan,Version,SoftModel from Gps_AutoTest_Result where TestTime > ? and TestTime < ? GROUP BY ZhiDan,SoftModel,Version UNION SELECT ZhiDan,Version,SoftModel from Gps_AutoTest_Result2 where TestTime > ? and TestTime < ? GROUP BY ZhiDan,SoftModel,Version UNION SELECT ZhiDan,Version,SoftModel from Gps_AutoTest_Result3 where TestTime > ? and TestTime < ?  GROUP BY ZhiDan,SoftModel,Version UNION SELECT ZhiDan,Version,SoftModel from Gps_CoupleTest_Result where TestTime > ? and TestTime < ? GROUP BY ZhiDan,SoftModel,Version UNION SELECT ZhiDan,Version,SoftModel from Gps_CartonBoxTwenty_Result where TestTime > ? and TestTime < ? GROUP BY ZhiDan,Version,SoftModel UNION SELECT ZhiDan,Version,SoftModel from Gps_ManuPrintParam WHERE CH_PrintTime > ? and CH_PrintTime < ? GROUP BY ZhiDan,Version,SoftModel UNION SELECT ZhiDan,Version,SoftModel from Gps_ManuPrintParam WHERE JS_PrintTime > ? and JS_PrintTime < ? GROUP BY ZhiDan,Version,SoftModel";

	public final static String SELECT_PRODUCTION_BY_ZHIDAN_VERSION_SOFTMODEL_TESTTIME = "SELECT (SELECT count(*) from Gps_AutoTest_Result WHERE ZhiDan = ? and Version = ? and SoftModel = ? and TestTime > ? and TestTime < ?) as FunctionProduct, (SELECT count(*) from Gps_AutoTest_Result2 WHERE ZhiDan = ? and Version = ? and SoftModel = ? and TestTime > ? and TestTime < ?) as SMTProduct, (SELECT count(*) from Gps_AutoTest_Result3 WHERE ZhiDan = ? and Version = ? and SoftModel = ? and TestTime > ? and TestTime < ?) as AgedProduct, (SELECT count(*) from Gps_CoupleTest_Result WHERE ZhiDan = ? and Version = ? and SoftModel = ? and TestTime > ? and TestTime < ?) as CouplingProduct, (SELECT count(*) from Gps_CartonBoxTwenty_Result WHERE ZhiDan = ? and Version = ? and SoftModel = ? and TestTime > ? and TestTime < ?) as CartonProduct, (SELECT count(*) from Gps_ManuPrintParam WHERE ZhiDan = ? and Version = ? and SoftModel = ? and CH_PrintTime > ? and CH_PrintTime < ?) as CHPrintProduct, (SELECT count(*) from Gps_ManuPrintParam WHERE ZhiDan = ? and Version = ? and SoftModel = ? and  JS_PrintTime > ? and JS_PrintTime  < ?) as JSPrintProduct";

	public final static String SELECT_RENCENT_SIXDAYS_PRODUCTION = "SELECT top 6 quantity,[time] from daily_completion ORDER BY Id DESC";

	public final static String SELECT_PLAN_GANT_INFORMATION = "SELECT scheduling_plan.id, orders.ZhiDan as zhidan, plan_start_time as planStartTime, scheduling_plan.plan_complete_time as planEndTime, '' as interval from scheduling_plan INNER JOIN orders on orders.id = scheduling_plan.orders WHERE scheduling_plan.id = ? ";

	public final static String SELECT_ORDER_BY_STATUS = "SELECT * from orders WHERE orders_status = ?";

	public final static String SELECT_PROCESS_PROCESSGROUP = "SELECT process.id,process_no as processNo,process_name as processName,process_remark as processRemark,process_group as processGroup,process_group.group_no as groupNo,process_group.group_name as groupName from process, process_group where process.process_group = process_group.id ";

	public final static String SELECT_LINE_PROCESSGROUP = "SELECT line.id, line_no AS 'lineNo', line_name AS lineName, line_remark AS lineRemark, line_director AS lineDirector, line_engineer AS lineEngineer, line_qc AS lineQc, process_group AS processGroup, process_group.group_name AS groupName, a.Name AS directorName, b.Name AS engineerName, c.Name AS qcName FROM line, process_group, LUserAccount a, LUserAccount b, LUserAccount c WHERE line.process_group = process_group.id AND a.Id = line.line_director AND b.Id = line.line_engineer AND c.Id = line_qc ";

	public final static String UPDATE_MODELCAPACITY_POSITION = "UPDATE model_capacity SET [position] = ? where id = ?";

	public final static String SELECT_MODELCAPACITY = "SELECT model_capacity.id,soft_model as softModel,customer_model as customerModel,process,model_capacity.process_group as processGroup,process_people_quantity as processPeopleQuantity,capacity,remark,[position], process_group.group_name as groupName, process.process_name as processName FROM model_capacity, process, process_group WHERE model_capacity.process = process.id AND model_capacity.process_group = process_group.id ";

	public final static String SELECT_SCHEDULED_ORDER_QUANTITY = "SELECT orders, COUNT (scheduling_quantity) AS scheduled_quantity FROM scheduling_plan INNER JOIN orders on scheduling_plan.orders = orders.id WHERE process_group = ? and orders.orders_status <> 3 and orders.orders_status <> 4 GROUP BY orders ";

	public final static String SELECT_MODELCAPACITY_BY_ORDER_PROCESS = "SELECT * from model_capacity WHERE process_group = ? and process = ? AND soft_model LIKE ? ";

	public final static String SELECT_PEOPLE_CAPACITY_BY_SOFTMODEL_PROCESSGROUP = "SELECT COUNT(process_people_quantity) as people,count(capacity) as capacity FROM model_capacity WHERE process_group = ? AND soft_model LIKE ? ";

	public final static String SELECT_SCHEDULINGPLAN_LINE = "SELECT scheduling_plan.*, line.line_no, line.line_name FROM scheduling_plan, orders, line WHERE scheduling_plan.orders = orders.id AND scheduling_plan.line = line.id ";

	public final static String SELECT_USER_NAME_ID_BY_NAME = "SELECT Name as name, Id as id from LUserAccount WHERE InService = 1 and Name LIKE ?";

	public final static String SELECT_PROCESS_NAME_ID = "SELECT id, process_name as processName from process";

	public final static String SELECT_PROCESSGROUP_NAME_ID = "SELECT id, group_name as groupName from process_group";

	public final static String SELECT_LINE_NAME_ID = "SELECT id, line_name as lineName FROM line";

	public final static String SELECT_ORDER = "SELECT orders.id, zhidan, alias, soft_model AS softModel, version, product_no AS productNo, customer_name AS customerName, customer_number AS customerNumber, order_date AS orderDate, quantity, delivery_date AS deliveryDate, remark, order_status AS orderStatus, delete_reason AS deleteReason, order_creator AS orderCreator, order_create_time AS orderCreateTime, order_modifier AS orderModifier, order_modify_time AS orderModifyTime,a.Name as creatorName,b.Name as modifierName,order_status.status_name as statusName FROM orders , order_status , LUserAccount a , LUserAccount b WHERE orders.order_status = order_status.id AND b.Id = orders.order_modifier AND a.Id = orders.order_creator ";

	public final static String SELECT_PROCESSGROUP = "SELECT id,group_no as groupNo,group_name as groupName,group_remark as groupRemark FROM process_group";

	public final static String SELECT_LINECOMPUTER_BY_LINE = "SELECT id,computer_name as computerName,remark,line,ip from line_computer WHERE line = ? ";

	public final static String SELECT_LINECOMPUTER_BY_IP = "SELECT * from line_computer WHERE ip = ? ";
}
