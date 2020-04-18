package com.jimi.mes_standard_capacity.model.entity;

/**根据传入的参数生成起始时间
 * @author   HCJ
 * @date     2020年4月16日 下午5:00:38
 */
public class DateFormat {

	private String testDate;

	private String imeiDate;

	private String printDate;

	private String startTestDate;

	private String endTestDate;

	private String startIMEIDate;

	private String endIMEIDate;

	private String startPrintDate;

	private String endPrintDate;


	public String getTestDate() {
		return testDate;
	}


	public String getImeiDate() {
		return imeiDate;
	}


	public String getPrintDate() {
		return printDate;
	}


	public String getStartTestDate() {
		return startTestDate;
	}


	public String getEndTestDate() {
		return endTestDate;
	}


	public String getStartIMEIDate() {
		return startIMEIDate;
	}


	public String getEndIMEIDate() {
		return endIMEIDate;
	}


	public String getStartPrintDate() {
		return startPrintDate;
	}


	public String getEndPrintDate() {
		return endPrintDate;
	}


	public DateFormat(String date) {
		this.imeiDate = date.replace("-0", "/").replace("-", "/");
		this.printDate = date.replace("-", ".");
		this.testDate = date;
		this.startIMEIDate = imeiDate + " 00:00:00";
		this.endIMEIDate = imeiDate + " 23:59:59";
		this.startPrintDate = printDate + " 00:00:00.000";
		this.endPrintDate = printDate + " 23:59:59.999";
		this.startTestDate = testDate + " 00:00:00";
		this.endTestDate = testDate + " 23:59:59";
	}

}
