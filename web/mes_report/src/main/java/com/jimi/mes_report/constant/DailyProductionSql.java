package com.jimi.mes_report.constant;

public class DailyProductionSql {

    public  static final String SELECT_ALL_ZHIDAN = "SELECT r.ZhiDan,o.Version,o.SoftModel,o.ProductNo,o.IMEIStart,o.IMEIEnd from " +
            "(SELECT ZhiDan from Gps_ManuCpParam where CPTIME > ? and CPTIME < ? and CPRESULT = 1 and LEN(ZhiDan) > 0 GROUP BY ZhiDan ) r LEFT JOIN Gps_ManuOrderParam o " +
            "on o.ZhiDan = r.ZhiDan and o.Status != 3 " +
            "UNION " +
            "SELECT r.ZhiDan,o.Version,o.SoftModel,o.ProductNo,o.IMEIStart,o.IMEIEnd from " +
            "(SELECT ZhiDan,Version,SoftModel from Gps_AutoTest_Result where TestTime > ? and TestTime < ? and LEN(ZhiDan) > 0 " +
            "GROUP BY ZhiDan,SoftModel,Version) r LEFT JOIN Gps_ManuOrderParam o " +
            "on o.ZhiDan = r.ZhiDan and o.Status != 3 " +
            "UNION " +
            "SELECT r.ZhiDan,o.Version,o.SoftModel,o.ProductNo,o.IMEIStart,o.IMEIEnd from " +
            "(SELECT ZhiDan,Version,SoftModel from Gps_AutoTest_Result2 where TestTime > ? and TestTime < ? and LEN(ZhiDan) > 0 " +
            "GROUP BY ZhiDan,SoftModel,Version) r LEFT JOIN Gps_ManuOrderParam o " +
            "on o.ZhiDan = r.ZhiDan and o.Status != 3 " +
            "UNION " +
            "SELECT r.ZhiDan,o.Version,o.SoftModel,o.ProductNo,o.IMEIStart,o.IMEIEnd from " +
            "(SELECT ZhiDan,Version,SoftModel from Gps_AutoTest_Result3 where TestTime > ? and TestTime < ? and LEN(ZhiDan) > 0 " +
            "GROUP BY ZhiDan,SoftModel,Version) r LEFT JOIN Gps_ManuOrderParam o " +
            "on o.ZhiDan = r.ZhiDan and o.Status != 3 " +
            "UNION " +
            "SELECT r.ZhiDan,o.Version,o.SoftModel,o.ProductNo,o.IMEIStart,o.IMEIEnd from " +
            "(SELECT ZhiDan,Version,SoftModel from Gps_CoupleTest_Result where TestTime > ? and TestTime < ? and LEN(ZhiDan) > 0 " +
            "GROUP BY ZhiDan,SoftModel,Version) r LEFT JOIN Gps_ManuOrderParam o " +
            "on o.ZhiDan = r.ZhiDan and o.Status != 3 " +
            "UNION " +
            "SELECT r.ZhiDan,o.Version,o.SoftModel,o.ProductNo,o.IMEIStart,o.IMEIEnd from " +
            "(SELECT ZhiDan,Version,SoftModel from Gps_CartonBoxTwenty_Result where TestTime > ? and TestTime < ? and LEN(ZhiDan) > 0 " +
            "GROUP BY ZhiDan,SoftModel,Version) r LEFT JOIN Gps_ManuOrderParam o " +
            "on o.ZhiDan = r.ZhiDan and o.Status != 3 " +
            "UNION " +
            "SELECT r.ZhiDan,o.Version,o.SoftModel,o.ProductNo,o.IMEIStart,o.IMEIEnd from " +
            "(SELECT ZhiDan,Version,SoftModel from Gps_ManuPrintParam where CH_PrintTime > ? and CH_PrintTime < ? and LEN(ZhiDan) > 0 " +
            "GROUP BY ZhiDan,SoftModel,Version) r LEFT JOIN Gps_ManuOrderParam o " +
            "on o.ZhiDan = r.ZhiDan and o.Status != 3 " +
            "UNION " +
            "SELECT r.ZhiDan,o.Version,o.SoftModel,o.ProductNo,o.IMEIStart,o.IMEIEnd from " +
            "(SELECT ZhiDan,Version,SoftModel from Gps_ManuPrintParam where JS_PrintTime > ? and JS_PrintTime < ? and LEN(ZhiDan) > 0 " +
            "GROUP BY ZhiDan,SoftModel,Version) r LEFT JOIN Gps_ManuOrderParam o " +
            "on o.ZhiDan = r.ZhiDan and o.Status != 3 ";

    public static final String SELECT_SMT_PRODUCTION = "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?  and TestTime >= ? and TestTime <= ?  " +
            "UNION ALL " +
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?    and TestTime >= ? and TestTime <= ?  " +
            "UNION ALL " +
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?    and TestTime >= ? and TestTime <= ?  " +
            "UNION ALL " +
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?    and TestTime >= ? and TestTime <= ?  " +
            "UNION ALL " +
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?    and TestTime >= ? and TestTime <= ?  " +
            "UNION ALL " +
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?    and TestTime >= ? and TestTime <= ?  " +
            "UNION ALL " +
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?    and TestTime >= ? and TestTime <= ?  " +
            "UNION ALL " +
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?    and TestTime >= ? and TestTime <= ?  " +
            "UNION ALL " +
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?    and TestTime >= ? and TestTime <= ?  " +
            "UNION ALL " +
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?    and TestTime >= ? and TestTime <= ?  " +
            "UNION ALL " +
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?    and TestTime >= ? and TestTime <= ?  " +
            "UNION ALL " +
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?    and TestTime >= ? and TestTime <= ?  " +
            "UNION ALL " +
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?    and TestTime >= ? and TestTime <= ?  " +
            "UNION ALL " +
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?    and TestTime >= ? and TestTime <= ?  " +
            "UNION ALL " +
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?    and TestTime >= ? and TestTime <= ?  " +
            "UNION ALL " +
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?    and TestTime >= ? and TestTime <= ?  " +
            "UNION ALL " +
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?    and TestTime >= ? and TestTime <= ?  " +
            "UNION ALL " +
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?    and TestTime >= ? and TestTime <= ?  " +
            "UNION ALL " +
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?    and TestTime >= ? and TestTime <= ?  " +
            "UNION ALL " +
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?    and TestTime >= ? and TestTime <= ?  " +
            "UNION ALL " +
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?    and TestTime >= ? and TestTime <= ?  " +
            "UNION ALL " +
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?    and TestTime >= ? and TestTime <= ?  " +
            "UNION ALL " +
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?    and TestTime >= ? and TestTime <= ?  " +
            "UNION ALL " +
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*)as output from Gps_AutoTest_Result2 where ZhiDan = ?    and TestTime >= ? and TestTime <= ?  " ;

    public static final String SELECT_ASSEMBLE_PRODUCTION = "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? ";

    public static final String SELECT_AGEING_PRODUCTION = "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result3 where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? ";


    public static final String SELECT_COUPLING_PRODUCTION = "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CoupleTest_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? ";

    public static final String SELECT_BODY_STICKING_PRODUCTION = "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(JS_PrintTime) as endTime,MIN(JS_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where  ZhiDan = ?    and JS_PrintTime >= ? and JS_PrintTime <= ? ";

    public static final String SELECT_COLOR_BOX_PRODUCTION = "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(CH_PrintTime) as endTime,MIN(CH_PrintTime) as startTime,count(*) as output from Gps_ManuPrintParam where ZhiDan = ?    and CH_PrintTime >= ? and CH_PrintTime <= ? ";

    public static final String SELECT_IMEI_PRODUCTION = "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? "+
            "UNION ALL "+
            "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? "+
            "UNION ALL "+
            "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? "+
            "UNION ALL "+
            "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? "+
            "UNION ALL "+
            "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? "+
            "UNION ALL "+
            "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? "+
            "UNION ALL "+
            "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? "+
            "UNION ALL "+
            "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? "+
            "UNION ALL "+
            "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? "+
            "UNION ALL "+
            "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? "+
            "UNION ALL "+
            "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? "+
            "UNION ALL "+
            "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? "+
            "UNION ALL "+
            "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? "+
            "UNION ALL "+
            "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? "+
            "UNION ALL "+
            "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? "+
            "UNION ALL "+
            "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? "+
            "UNION ALL "+
            "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? "+
            "UNION ALL "+
            "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? "+
            "UNION ALL "+
            "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? "+
            "UNION ALL "+
            "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? "+
            "UNION ALL "+
            "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? "+
            "UNION ALL "+
            "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? "+
            "UNION ALL "+
            "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? "+
            "UNION ALL "+
            "SELECT MAX(CPTIME) as endTime,MIN(CPTIME) as startTime,count(*) as output from Gps_ManuCpParam where CPRESULT = 1 and ZhiDan = ? and CPTIME >= ? and CPTIME <= ? ";


    public static final String SELECT_CARTON_PRODUCTION = "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? "+
            "UNION ALL "+
            "SELECT MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_CartonBoxTwenty_Result where  ZhiDan = ?    and TestTime >= ? and TestTime <= ? ";

    public static final String SELECT_ALL_ZHIDAN_BY_TIME = "SELECT ZhiDan from Gps_ManuCpParam where CPTIME > ? and CPTIME < ? and CPRESULT = 1 and LEN(ZhiDan) > 0 GROUP BY ZhiDan UNION SELECT ZhiDan from Gps_AutoTest_Result where TestTime > ? and TestTime < ? and LEN(ZhiDan) > 0 GROUP BY ZhiDan UNION SELECT ZhiDan from Gps_AutoTest_Result2 where TestTime > ? and TestTime < ? and LEN(ZhiDan) > 0 GROUP BY ZhiDan UNION SELECT ZhiDan from Gps_AutoTest_Result3 where TestTime > ? and TestTime < ? and LEN(ZhiDan) > 0 GROUP BY ZhiDan UNION SELECT ZhiDan from Gps_CoupleTest_Result where TestTime > ? and TestTime < ? and LEN(ZhiDan) > 0 GROUP BY ZhiDan UNION SELECT ZhiDan from Gps_CartonBoxTwenty_Result where TestTime > ? and TestTime < ? and LEN(ZhiDan) > 0 GROUP BY ZhiDan UNION SELECT ZhiDan from Gps_ManuPrintParam where CH_PrintTime > ? and CH_PrintTime < ? and LEN(ZhiDan) > 0 GROUP BY ZhiDan UNION SELECT ZhiDan from Gps_ManuPrintParam where JS_PrintTime > ? and JS_PrintTime < ? and LEN(ZhiDan) > 0 GROUP BY ZhiDan";

    public static final String SELECT_ALL_ORDER_BY_ZHIDAN = "SELECT * FROM Gps_ManuOrderParam WHERE ZhiDan = ?";
    
    		public static final String SELECT_CARTON_NUMBER_BY_ZHIDAN = "SELECT count(*) FROM Gps_CartonBoxTwenty_Result WHERE ZhiDan = ?";

}
