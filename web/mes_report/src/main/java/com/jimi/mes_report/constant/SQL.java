package com.jimi.mes_report.constant;

public class SQL {

	public final static String SELECT_ORDER_BY_TIME = "SELECT result.*,IMEIStart,IMEIEnd,SoftModel,ProductNo,status FROM (SELECT DISTINCT Gps_ManuOrderParam.ZhiDan, Gps_ManuOrderParam.Version FROM Gps_ManuOrderParam INNER JOIN Gps_CartonBoxTwenty_Result ON Gps_ManuOrderParam.ZhiDan = Gps_CartonBoxTwenty_Result.ZhiDan AND Gps_ManuOrderParam.Version = Gps_CartonBoxTwenty_Result.Version WHERE Gps_CartonBoxTwenty_Result.TestTime > ? AND Gps_CartonBoxTwenty_Result.TestTime < ? ) result INNER JOIN Gps_ManuOrderParam ON Gps_ManuOrderParam.ZhiDan = result.ZhiDan AND Gps_ManuOrderParam.Version = result.Version and Gps_ManuOrderParam.status != 3";

	public final static String SELECT_INFO_BY_ZHIDAN_VERSION = "SELECT ZhiDan, Version, ProductNo, SoftModel, IMEIEnd, IMEIStart, status from Gps_ManuOrderParam where ZhiDan = ? and Version = ?";

	public final static String SELECT_FUNCTIONNUM_BY_ZHIDAN_VERSION = "SELECT count(*) from Gps_AutoTest_Result where ZhiDan = ? and Version = ?  and TestTime > ? and TestTime < ?";

	public final static String SELECT_SMTNUM_BY_ZHIDAN_VERSION = "SELECT count(*) from Gps_AutoTest_Result2 where ZhiDan = ? and Version = ?  and TestTime > ? and TestTime < ?";

	public final static String SELECT_AGINGNUM_BY_ZHIDAN_VERSION = "SELECT count(*) from Gps_AutoTest_Result3 where ZhiDan = ? and Version = ?  and TestTime > ? and TestTime < ?";

	public final static String SELECT_COUPLINGNUM_BY_ZHIDAN_VERSION = "SELECT count(*) from Gps_CoupleTest_Result  where ZhiDan = ? and Version = ?  and TestTime > ? and TestTime < ?";

	public final static String SELECT_JSPRINTNUM_BY_ZHIDAN_VERSION = "SELECT count(*) from Gps_ManuPrintParam where ZhiDan = ? and Version = ?  and JS_PrintTime like ?";

	public final static String SELECT_CHPRINTNUM_BY_ZHIDAN_VERSION = "SELECT count(*) from Gps_ManuPrintParam where ZhiDan = ? and Version = ?  and CH_PrintTime like ?";

	public final static String SELECT_IMEINUM_BY_ZHIDAN = "SELECT COUNT (*) FROM Gps_ManuCpParam WHERE ZhiDan = ? AND CPTIME like ? AND CPRESULT = ?";

	public final static String SELECT_CARTONNUM_BY_ZHIDAN_VERSION = "SELECT count(*) FROM Gps_CartonBoxTwenty_Result WHERE ZhiDan = ? AND Version = ? AND TestTime > ? AND TestTime < ?";

	public final static String SELECT_ORDERS_BY_TIME = "SELECT * FROM orders WHERE time > ? AND time < ?";

	public final static String SELECT_MAX_PRODUCT_BY_TIMETYPE = "SELECT max(actual_product) FROM order_production WHERE time_type = 1";

	public final static String SELECT_FUNCTIONNUM_BY_TESTTIME = "SELECT count(*) from Gps_AutoTest_Result where TestTime > ? and TestTime < ?";

	public final static String SELECT_SMTNUM_BY_TESTTIME = "SELECT count(*) from Gps_AutoTest_Result2 where TestTime > ? and TestTime < ?";

	public final static String SELECT_AGINGNUM_BY_TESTTIME = "SELECT count(*) from Gps_AutoTest_Result3 where TestTime > ? and TestTime < ?";

	public final static String SELECT_COUPLINGNUM_BY_TESTTIME = "SELECT count(*) from Gps_CoupleTest_Result  where TestTime > ? and TestTime < ?";

	public final static String SELECT_JSPRINTNUM_BY_TESTTIME = "SELECT count(*) from Gps_ManuPrintParam where JS_PrintTime like ?";

	public final static String SELECT_CHPRINTNUM_BY_TESTTIME = "SELECT count(*) from Gps_ManuPrintParam where CH_PrintTime like ?";

	public final static String SELECT_IMEINUM_BY_TESTTIME = "SELECT COUNT (*) FROM Gps_ManuCpParam WHERE CPTIME like ? AND CPRESULT = ?";

	public final static String SELECT_CARTONNUM_BY_TESTTIME = "SELECT count(*) FROM Gps_CartonBoxTwenty_Result WHERE TestTime > ? AND TestTime < ?";

	public final static String SELECT_ORDERS_CAPATICY_BY_TIME = "SELECT o.order_number,o.soft_model,o.soft_version,o.production_number,o.status,o.completion_rate,o.diffenrence,o.plan_product,o.actual_product,c.type,c.actual_product as 'stationProduct',c.hourly_plan_product,c.hourly_actual_product from orders o INNER JOIN capacity c on o.id = c.orders where time > ? and time < ?";

	public final static String SELECT_ORDERPRODUCTION_BY_TIME = "SELECT * from order_production WHERE time_type = ? and time > ? and time < ?";

	public final static String SELECT_STATIONRODUCTION_BY_TIME = "SELECT * from workstation_production where time_type = ? and time > ? and time < ?";

	public final static String SELECT_MAXRODUCTION_BY_TIMETYPE = "SELECT max(actual_product) as maxProduct from order_production where time_type = ?";

	public final static String SELECT_MAXSTATIONRODUCTION_BY_TIMETYPE = "SELECT type,max(product) as maxProduct from workstation_production WHERE time_type = ? GROUP BY type";

	public final static String SELECT_DISTINCT_ORDERNUM_BY_TIME = "SELECT DISTINCT order_number FROM orders where TIME > ? AND TIME < ?";

	public final static String SELECT_PRODUCTION_BY_TIME = "SELECT sum(plan_product) as plan_product,SUM(actual_product) as actual_product from order_production WHERE time_type = ? and time > ? and time < ?";
	
	public final static String SELECT_SUM_STATIONRODUCTION_BY_TIME = "SELECT SUM(product) as product,type from workstation_production WHERE time_type = ? and time > ? and time < ? GROUP BY type";
}
