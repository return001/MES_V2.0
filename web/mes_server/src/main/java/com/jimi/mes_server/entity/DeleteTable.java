package com.jimi.mes_server.entity;

/**
 * 可删除表枚举类
 * @type DeleteTable
 * @Company 几米物联技术有限公司-自动化部
 * @author 汤如杰
 * @date 2019年1月8日
 */
public enum DeleteTable {

	DataRelativeSheet("DataRelativeSheet", 0), 
	DataRelativeUnique("DataRelativeUnique", 1), 
	DataRelativeUpdate("DataRelativeUpdate", 2), 
	GpsAutoTestResult("Gps_AutoTest_Result", 3), 
	GpsAutoTestResult2("Gps_AutoTest_Result2", 4), 
	GpsAutoTestResult3("Gps_AutoTest_Result3", 5), 
	GpsCartonBoxTwentyResult("Gps_CartonBoxTwenty_Result", 6), 
	GpsCoupleTestResult("Gps_CoupleTest_Result", 7), 
	GpsManuPrintParam("Gps_ManuPrintParam", 8), 
	GpsTestResult("Gps_TestResult", 9), 
	NetMarkIMEI("NetMarkIMEI", 10), 
	GpsManuSimDataParam("Gps_ManuSimDataParam", 11);

	private String name;

	private Integer id;


	private DeleteTable(String name, Integer id) {
		this.name = name;
		this.id = id;
	}


	public String getName() {
		return name;
	}


	public Integer getId() {
		return id;
	}


	public static String getNameById(Integer id) {
		for (DeleteTable deleteTable : DeleteTable.values()) {
			if (deleteTable.getId().equals(id)) {
				return deleteTable.getName();
			}
		}
		return null;
	}

}
