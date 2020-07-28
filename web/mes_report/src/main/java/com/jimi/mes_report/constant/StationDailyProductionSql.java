/**  
*  
*/  
package com.jimi.mes_report.constant;

/**  
 * <p>Title: StationDailyProductionSql</p>  
 * <p>Description: </p>  
 * <p>Copyright: Copyright (c) 2019</p>  
 * <p>Company: 惠州市几米物联技术有限公司</p>  
 * @author trjie  
 * @date 2020年7月31日
 *
 */
public class StationDailyProductionSql {
	
	public static final String SMT_COMPUTER_DAILAY_PRODUCTION_SQL = "SELECT Computer AS computer, MAX (TestTime) AS endTime, MIN (TestTime) AS startTime, COUNT (*) AS OUTPUT FROM Gps_AutoTest_Result2 WHERE ZhiDan = ? AND TestTime >= ? AND TestTime <= ? GROUP BY Computer";
	
	public static final String ASSEMBLE_COMPUTER_DAILAY_PRODUCTION_SQL = "SELECT Computer AS computer, MAX(TestTime) as endTime,MIN(TestTime) as startTime,count(*) as output from Gps_AutoTest_Result where  ZhiDan = ? and TestTime >= ? and TestTime <= ? GROUP BY Computer";
	
	public static final String AGEING_COMPUTER_DAILAY_PRODUCTION_SQL = "SELECT Computer AS computer, MAX (TestTime) AS endTime, MIN (TestTime) AS startTime, COUNT (*) AS OUTPUT FROM Gps_AutoTest_Result3 WHERE ZhiDan = ? AND TestTime >= ? AND TestTime <= ? GROUP BY Computer";
	
	public static final String COUPLING_COMPUTER_DAILAY_PRODUCTION_SQL = "SELECT Computer AS computer, MAX (TestTime) AS endTime, MIN (TestTime) AS startTime, COUNT (*) AS OUTPUT FROM Gps_CoupleTest_Result WHERE ZhiDan = ? AND TestTime >= ? AND TestTime <= ? GROUP BY Computer";
	
	public static final String CARTON_COMPUTER_DAILAY_PRODUCTION_SQL = "SELECT Computer AS computer, MAX (TestTime) AS endTime, MIN (TestTime) AS startTime, COUNT (*) AS OUTPUT FROM Gps_CartonBoxTwenty_Result WHERE ZhiDan = ? AND TestTime >= ? AND TestTime <= ? GROUP BY Computer";
	
}
