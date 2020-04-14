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

	public final static String SELECT_DAILY_ORDERS = "SELECT r.ZhiDan,o.Version,o.SoftModel,o.ProductNo,o.IMEIStart,o.IMEIEnd from (SELECT ZhiDan from Gps_ManuCpParam where CPTIME > ? and CPTIME < ? and CPRESULT = 1 and LEN(ZhiDan) > 0 GROUP BY ZhiDan ) r LEFT JOIN Gps_ManuOrderParam o on o.ZhiDan = r.ZhiDan and o.Status != 3 UNION SELECT r.ZhiDan,r.Version,r.SoftModel,o.ProductNo,o.IMEIStart,o.IMEIEnd from (SELECT ZhiDan,Version,SoftModel from Gps_AutoTest_Result where TestTime > ? and TestTime < ? and LEN(ZhiDan) > 0 GROUP BY ZhiDan,SoftModel,Version) r LEFT JOIN Gps_ManuOrderParam o on o.ZhiDan = r.ZhiDan and o.Status != 3 AND o.SoftModel = r.SoftModel AND o.Version = r.Version UNION SELECT r.ZhiDan,r.Version,r.SoftModel,o.ProductNo,o.IMEIStart,o.IMEIEnd from (SELECT ZhiDan,Version,SoftModel from Gps_AutoTest_Result2 where TestTime > ? and TestTime < ? and LEN(ZhiDan) > 0 GROUP BY ZhiDan,SoftModel,Version) r LEFT JOIN Gps_ManuOrderParam o on o.ZhiDan = r.ZhiDan and o.Status != 3 AND o.SoftModel = r.SoftModel AND o.Version = r.Version UNION SELECT r.ZhiDan,r.Version,r.SoftModel,o.ProductNo,o.IMEIStart,o.IMEIEnd from (SELECT ZhiDan,Version,SoftModel from Gps_AutoTest_Result3 where TestTime > ? and TestTime < ? and LEN(ZhiDan) > 0 GROUP BY ZhiDan,SoftModel,Version) r LEFT JOIN Gps_ManuOrderParam o on o.ZhiDan = r.ZhiDan and o.Status != 3 AND o.SoftModel = r.SoftModel AND o.Version = r.Version UNION SELECT r.ZhiDan,r.Version,r.SoftModel,o.ProductNo,o.IMEIStart,o.IMEIEnd from (SELECT ZhiDan,Version,SoftModel from Gps_CoupleTest_Result where TestTime > ? and TestTime < ? and LEN(ZhiDan) > 0 GROUP BY ZhiDan,SoftModel,Version) r LEFT JOIN Gps_ManuOrderParam o on o.ZhiDan = r.ZhiDan and o.Status != 3 AND o.SoftModel = r.SoftModel AND o.Version = r.Version UNION SELECT r.ZhiDan,r.Version,r.SoftModel,o.ProductNo,o.IMEIStart,o.IMEIEnd from (SELECT ZhiDan,Version,SoftModel from Gps_CartonBoxTwenty_Result where TestTime > ? and TestTime < ? and LEN(ZhiDan) > 0 GROUP BY ZhiDan,SoftModel,Version) r LEFT JOIN Gps_ManuOrderParam o on o.ZhiDan = r.ZhiDan and o.Status != 3 AND o.SoftModel = r.SoftModel AND o.Version = r.Version UNION SELECT r.ZhiDan,r.Version,r.SoftModel,o.ProductNo,o.IMEIStart,o.IMEIEnd from (SELECT ZhiDan,Version,SoftModel from Gps_ManuPrintParam where CH_PrintTime > ? and CH_PrintTime < ? and LEN(ZhiDan) > 0 GROUP BY ZhiDan,SoftModel,Version) r LEFT JOIN Gps_ManuOrderParam o on o.ZhiDan = r.ZhiDan and o.Status != 3 AND o.SoftModel = r.SoftModel AND o.Version = r.Version UNION SELECT r.ZhiDan,r.Version,r.SoftModel,o.ProductNo,o.IMEIStart,o.IMEIEnd from (SELECT ZhiDan,Version,SoftModel from Gps_ManuPrintParam where JS_PrintTime > ? and JS_PrintTime < ? and LEN(ZhiDan) > 0 GROUP BY ZhiDan,SoftModel,Version) r LEFT JOIN Gps_ManuOrderParam o on o.ZhiDan = r.ZhiDan and o.Status != 3 AND o.SoftModel = r.SoftModel AND o.Version = r.Version";

	public final static String SELECT_COST_TIME_BY_ORDERS = "SELECT max(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as production from Gps_AutoTest_Result2 where TestTime > ? and TestTime < ? and ZhiDan = ? AND Version = ? and SoftModel = ? UNION ALL SELECT max(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as production from Gps_AutoTest_Result where TestTime > ? and TestTime < ? and ZhiDan = ? AND Version = ? and SoftModel = ? UNION ALL SELECT max(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as production from Gps_AutoTest_Result3 where TestTime > ? and TestTime < ? and ZhiDan = ? AND Version = ? and SoftModel = ? UNION ALL SELECT max(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as production from Gps_CoupleTest_Result where TestTime > ? and TestTime < ? and ZhiDan = ? AND Version = ? and SoftModel = ? UNION ALL SELECT max(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*)as production from Gps_ManuPrintParam where JS_PrintTime > ? and JS_PrintTime < ? and ZhiDan = ? AND Version = ? and SoftModel = ? UNION ALL SELECT max(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*)as production from Gps_ManuPrintParam where CH_PrintTime > ? and CH_PrintTime < ? and ZhiDan = ? AND Version = ? and SoftModel = ? UNION ALL SELECT max(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*)as production from Gps_ManuCpParam where CPTIME > ? and CPTIME < ? and CPRESULT = 1 and ZhiDan = ? UNION ALL SELECT max(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as production from Gps_CartonBoxTwenty_Result where TestTime > ? and TestTime < ? and ZhiDan = ? AND Version = ? and SoftModel = ?";

	public final static String SELECT_WORKSTATIONSORDERS = "SELECT id,total_use_time FROM workstations_orders WHERE order_number = ? and soft_model = ? and soft_version = ?";

	public final static String SELECT_MIN_AVERAGE_USE_TIME = "SELECT min(t.average_use_time) as minAverageUseTime FROM workstations_use_time t INNER JOIN (SELECT id FROM workstations_orders WHERE soft_model = ? AND soft_version = ?) r ON r.id = t.workstations_orders_id WHERE t.type = ?";

	public final static String SELECT_WORKSTATIONS_USE_TIMES_BY_ORDER = "SELECT id,workstations_orders_id,type,total_product,actual_use_time, average_use_time, min_used_time FROM workstations_use_time WHERE workstations_orders_id = ?";

	public final static String SELECT_WORKSTATIONS_ORDERS_BY_TIME = "SELECT o.order_number, o.soft_model, o.soft_version, o.production_number, o.total_use_time, o.status, t.type, t.actual_use_time, t.average_use_time, t.min_used_time FROM workstations_orders AS o INNER JOIN workstations_use_time AS t ON o.id = t.workstations_orders_id WHERE o.[time] > ? and o.[time] < ?";

	public final static String SELECT_MIN_TIME_BY_ORDERS = "SELECT min(TestTime) as startTime  from Gps_AutoTest_Result2 where TestTime > ? and TestTime < ? and ZhiDan = ? AND Version = ? and SoftModel = ? UNION ALL SELECT min(TestTime) as startTime from Gps_AutoTest_Result where TestTime > ? and TestTime < ? and ZhiDan = ? AND Version = ? and SoftModel = ? UNION ALL SELECT min(TestTime) as startTime from Gps_AutoTest_Result3 where TestTime > ? and TestTime < ? and ZhiDan = ? AND Version = ? and SoftModel = ? UNION ALL SELECT min(TestTime) as startTime from Gps_CoupleTest_Result where TestTime > ? and TestTime < ? and ZhiDan = ? AND Version = ? and SoftModel = ? UNION ALL SELECT min(JS_PrintTime) as startTime from Gps_ManuPrintParam where JS_PrintTime > ? and JS_PrintTime < ? and ZhiDan = ? AND Version = ? and SoftModel = ? UNION ALL SELECT min(CH_PrintTime) as startTime from Gps_ManuPrintParam where CH_PrintTime > ? and CH_PrintTime < ? and ZhiDan = ? AND Version = ? and SoftModel = ? UNION ALL SELECT MIN(CPTIME) as startTime from Gps_ManuCpParam where CPTIME > ? and CPTIME < ? and CPRESULT = 1 and ZhiDan = ? UNION ALL SELECT min(TestTime) as startTime from Gps_CartonBoxTwenty_Result where TestTime > ? and TestTime < ? and ZhiDan = ? AND Version = ? and SoftModel = ?";

}
