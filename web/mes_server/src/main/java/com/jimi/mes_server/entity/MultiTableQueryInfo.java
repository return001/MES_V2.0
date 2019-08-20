package com.jimi.mes_server.entity;

import java.util.List;

import com.jfinal.plugin.activerecord.Record;

/**多表查询信息类
 * @author   HCJ
 * @date     2019年5月30日 下午3:26:39
 */
public class MultiTableQueryInfo {

	private List<Record> DataRelativeSheet;

	private List<Record> DataRelativeUnique;

	private List<Record> Gps_AutoTest_Result;

	private List<Record> Gps_AutoTest_Result3;

	private List<Record> Gps_CartonBoxTwenty_Result;

	private List<Record> Gps_CoupleTest_Result;

	private List<Record> Gps_ManuCpParam;

	private List<Record> Gps_ManuPrintParam;

	private List<Record> Gps_TestResult;

	private List<Record> NetMarkIMEI;

	private List<Record> Gps_ManuSimDataParam;


	public List<Record> getDataRelativeSheet() {
		return DataRelativeSheet;
	}


	public void setDataRelativeSheet(List<Record> dataRelativeSheet) {
		DataRelativeSheet = dataRelativeSheet;
	}


	public List<Record> getDataRelativeUnique() {
		return DataRelativeUnique;
	}


	public void setDataRelativeUnique(List<Record> dataRelativeUnique) {
		DataRelativeUnique = dataRelativeUnique;
	}


	public List<Record> getGps_AutoTest_Result() {
		return Gps_AutoTest_Result;
	}


	public void setGps_AutoTest_Result(List<Record> gps_AutoTest_Result) {
		Gps_AutoTest_Result = gps_AutoTest_Result;
	}


	public List<Record> getGps_AutoTest_Result3() {
		return Gps_AutoTest_Result3;
	}


	public void setGps_AutoTest_Result3(List<Record> gps_AutoTest_Result3) {
		Gps_AutoTest_Result3 = gps_AutoTest_Result3;
	}


	public List<Record> getGps_CartonBoxTwenty_Result() {
		return Gps_CartonBoxTwenty_Result;
	}


	public void setGps_CartonBoxTwenty_Result(List<Record> gps_CartonBoxTwenty_Result) {
		Gps_CartonBoxTwenty_Result = gps_CartonBoxTwenty_Result;
	}


	public List<Record> getGps_CoupleTest_Result() {
		return Gps_CoupleTest_Result;
	}


	public void setGps_CoupleTest_Result(List<Record> gps_CoupleTest_Result) {
		Gps_CoupleTest_Result = gps_CoupleTest_Result;
	}


	public List<Record> getGps_ManuCpParam() {
		return Gps_ManuCpParam;
	}


	public void setGps_ManuCpParam(List<Record> gps_ManuCpParam) {
		Gps_ManuCpParam = gps_ManuCpParam;
	}


	public List<Record> getGps_ManuPrintParam() {
		return Gps_ManuPrintParam;
	}


	public void setGps_ManuPrintParam(List<Record> gps_ManuPrintParam) {
		Gps_ManuPrintParam = gps_ManuPrintParam;
	}


	public List<Record> getGps_TestResult() {
		return Gps_TestResult;
	}


	public void setGps_TestResult(List<Record> gps_TestResult) {
		Gps_TestResult = gps_TestResult;
	}


	public List<Record> getNetMarkIMEI() {
		return NetMarkIMEI;
	}


	public void setNetMarkIMEI(List<Record> netMarkIMEI) {
		NetMarkIMEI = netMarkIMEI;
	}


	public List<Record> getGps_ManuSimDataParam() {
		return Gps_ManuSimDataParam;
	}


	public void setGps_ManuSimDataParam(List<Record> gps_ManuSimDataParam) {
		Gps_ManuSimDataParam = gps_ManuSimDataParam;
	}


	public MultiTableQueryInfo(List<Record> dataRelativeSheet, List<Record> dataRelativeUnique, List<Record> gps_AutoTest_Result, List<Record> gps_AutoTest_Result3, List<Record> gps_CartonBoxTwenty_Result, List<Record> gps_CoupleTest_Result, List<Record> gps_ManuCpParam, List<Record> gps_ManuPrintParam, List<Record> gps_TestResult, List<Record> netMarkIMEI, List<Record> gps_ManuSimDataParam) {
		super();
		DataRelativeSheet = dataRelativeSheet;
		DataRelativeUnique = dataRelativeUnique;
		Gps_AutoTest_Result = gps_AutoTest_Result;
		Gps_AutoTest_Result3 = gps_AutoTest_Result3;
		Gps_CartonBoxTwenty_Result = gps_CartonBoxTwenty_Result;
		Gps_CoupleTest_Result = gps_CoupleTest_Result;
		Gps_ManuCpParam = gps_ManuCpParam;
		Gps_ManuPrintParam = gps_ManuPrintParam;
		Gps_TestResult = gps_TestResult;
		NetMarkIMEI = netMarkIMEI;
		Gps_ManuSimDataParam = gps_ManuSimDataParam;
	}
}
