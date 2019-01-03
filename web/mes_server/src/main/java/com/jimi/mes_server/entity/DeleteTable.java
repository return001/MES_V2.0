package com.jimi.mes_server.entity;

public enum DeleteTable {
	
	DataRelativeSheet("DataRelativeSheet",0),
	DataRelativeUnique("DataRelativeUnique", 1),
	GpsAutoTestResult("Gps_AutoTest_Result", 2),
	GpsAutoTestResult3("Gps_AutoTest_Result3", 3),
	GpsCartonBoxTwentyResult("Gps_CartonBoxTwenty_Result", 4),
	GpsCoupleTestResult("Gps_CoupleTest_Result", 5),
	GpsManuPrintParam("Gps_ManuPrintParam", 6),
	GpsTestResult("Gps_TestResult", 7),
	NetMarkIMEI("NetMarkIMEI", 8),
	GpsManuSimDataParam("Gps_ManuSimDataParam", 9);
	
	private String name;
	private Integer id;
	
	private DeleteTable(String name, Integer id){
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
