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
}
