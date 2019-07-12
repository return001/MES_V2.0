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

	public final static String SELECT_ORDERFILE_BY_ORDER = "SELECT id,orders,file_name,file_type from order_file WHERE orders = ?";

	public final static String SELECT_ORDERFILE_BY_ORDER_FILETYPE = "SELECT id,orders,file_name,file_type from order_file WHERE orders = ? and file_type = ?";

	public final static String SELECT_MODELCAPACITY_BY_MODEL_PROCESS = "SELECT * from model_capacity WHERE soft_model = ? and process = ? and process_group = ?";

	public final static String SELECT_CARTONNUM_BY_TESTTIME = "SELECT count(*) FROM Gps_CartonBoxTwenty_Result WHERE TestTime > ? AND TestTime < ?";

	public final static String SELECT_ZHIDAN_VERSION_SOFTMODEL_BY_TESTTIME = "SELECT ZhiDan,Version,SoftModel from Gps_AutoTest_Result where TestTime > ? and TestTime < ? GROUP BY ZhiDan,SoftModel,Version UNION SELECT ZhiDan,Version,SoftModel from Gps_AutoTest_Result2 where TestTime > ? and TestTime < ? GROUP BY ZhiDan,SoftModel,Version UNION SELECT ZhiDan,Version,SoftModel from Gps_AutoTest_Result3 where TestTime > ? and TestTime < ?  GROUP BY ZhiDan,SoftModel,Version UNION SELECT ZhiDan,Version,SoftModel from Gps_CoupleTest_Result where TestTime > ? and TestTime < ? GROUP BY ZhiDan,SoftModel,Version UNION SELECT ZhiDan,Version,SoftModel from Gps_CartonBoxTwenty_Result where TestTime > ? and TestTime < ? GROUP BY ZhiDan,Version,SoftModel UNION SELECT ZhiDan,Version,SoftModel from Gps_ManuPrintParam WHERE CH_PrintTime > ? and CH_PrintTime < ? GROUP BY ZhiDan,Version,SoftModel UNION SELECT ZhiDan,Version,SoftModel from Gps_ManuPrintParam WHERE JS_PrintTime > ? and JS_PrintTime < ? GROUP BY ZhiDan,Version,SoftModel";

	public final static String SELECT_PRODUCTION_BY_ZHIDAN_VERSION_SOFTMODEL_TESTTIME = "SELECT (SELECT count(*) from Gps_AutoTest_Result WHERE ZhiDan = ? and Version = ? and SoftModel = ? and TestTime > ? and TestTime < ?) as FunctionProduct, (SELECT count(*) from Gps_AutoTest_Result2 WHERE ZhiDan = ? and Version = ? and SoftModel = ? and TestTime > ? and TestTime < ?) as SMTProduct, (SELECT count(*) from Gps_AutoTest_Result3 WHERE ZhiDan = ? and Version = ? and SoftModel = ? and TestTime > ? and TestTime < ?) as AgedProduct, (SELECT count(*) from Gps_CoupleTest_Result WHERE ZhiDan = ? and Version = ? and SoftModel = ? and TestTime > ? and TestTime < ?) as CouplingProduct, (SELECT count(*) from Gps_CartonBoxTwenty_Result WHERE ZhiDan = ? and Version = ? and SoftModel = ? and TestTime > ? and TestTime < ?) as CartonProduct, (SELECT count(*) from Gps_ManuPrintParam WHERE ZhiDan = ? and Version = ? and SoftModel = ? and CH_PrintTime > ? and CH_PrintTime < ?) as CHPrintProduct, (SELECT count(*) from Gps_ManuPrintParam WHERE ZhiDan = ? and Version = ? and SoftModel = ? and  JS_PrintTime > ? and JS_PrintTime  < ?) as JSPrintProduct";

	public final static String SELECT_RENCENT_SIXDAYS_PRODUCTION = "SELECT top 6 quantity,[time] from daily_completion ORDER BY Id DESC";

	public final static String SELECT_PLAN_GANT_INFORMATION = "SELECT scheduling_plan.id, orders.ZhiDan as zhidan, plan_start_time as startTime, scheduling_plan.plan_complete_time as endTime, '' as interval from scheduling_plan INNER JOIN orders on orders.id = scheduling_plan.orders WHERE scheduling_plan.id = ? ";

	public final static String SELECT_UNSCHEDULED_ORDER = "SELECT * from orders WHERE orders_status = ?";
}
