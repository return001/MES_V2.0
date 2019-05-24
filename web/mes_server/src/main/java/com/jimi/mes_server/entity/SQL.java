package com.jimi.mes_server.entity;

public class SQL {

	public final static String SELECT_USER_BY_NAME_PASSWORD = "SELECT * FROM LUserAccount where Name = ? and Password = ?";

	public final static String SELECT_WEBUSERTYPE = "SELECT * FROM WebUserType where TypeId <> 4";

	public final static String SELECT_TYPENAME_BY_TYPEID = "SELECT * FROM WebUserType where TypeId = ?";

	public final static String SELECT_AUTOTEST_ANTIDUP_BY_SN = "SELECT * FROM Gps_AutoTest_AntiDup where SN = ?";

	public final static String SELECT_AUTOTEST_RESULT2_BY_SN = "SELECT * FROM Gps_AutoTest_Result2 where SN = ?";

	public final static String SELECT_TESTRESULT_BY_SN = "SELECT * FROM Gps_TestResult where SN = ?";

	public final static String DELETE_AUTOTEST_ANTIDUP_BY_SN = "DELETE from Gps_AutoTest_AntiDup where SN = ?";

	public final static String DELETE_AUTOTEST_RESULT2_BY_SN = "DELETE from Gps_AutoTest_Result2 where SN = ?";

	public final static String UPDATE_AUTOTEST_RESULT2_FUNCTIONRESULT_BY_SN = "UPDATE Gps_TestResult set FunctionResult = 0 where SN = ?";
}
