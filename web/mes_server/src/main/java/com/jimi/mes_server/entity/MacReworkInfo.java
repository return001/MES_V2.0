package com.jimi.mes_server.entity;

import java.util.List;

import com.jimi.mes_server.model.GpsAutotestAntidup;
import com.jimi.mes_server.model.GpsAutotestResult2;
import com.jimi.mes_server.model.GpsTestresult;

/**Mac返工信息类
 * @author   HCJ
 * @date     2019年5月30日 下午3:12:04
 */
public class MacReworkInfo {

	private List<GpsAutotestAntidup> Gps_AutoTest_AntiDup;

	private List<GpsAutotestResult2> Gps_AutoTest_Result2;

	private List<GpsTestresult> Gps_TestResult;

	public List<GpsAutotestAntidup> getGps_AutoTest_AntiDup() {
		return Gps_AutoTest_AntiDup;
	}

	public void setGps_AutoTest_AntiDup(List<GpsAutotestAntidup> gps_AutoTest_AntiDup) {
		Gps_AutoTest_AntiDup = gps_AutoTest_AntiDup;
	}

	public List<GpsAutotestResult2> getGps_AutoTest_Result2() {
		return Gps_AutoTest_Result2;
	}

	public void setGps_AutoTest_Result2(List<GpsAutotestResult2> gps_AutoTest_Result2) {
		Gps_AutoTest_Result2 = gps_AutoTest_Result2;
	}

	public List<GpsTestresult> getGps_TestResult() {
		return Gps_TestResult;
	}

	public void setGps_TestResult(List<GpsTestresult> gps_TestResult) {
		Gps_TestResult = gps_TestResult;
	}

	public MacReworkInfo(List<GpsAutotestAntidup> gps_AutoTest_AntiDup, List<GpsAutotestResult2> gps_AutoTest_Result2, List<GpsTestresult> gps_TestResult) {
		super();
		Gps_AutoTest_AntiDup = gps_AutoTest_AntiDup;
		Gps_AutoTest_Result2 = gps_AutoTest_Result2;
		Gps_TestResult = gps_TestResult;
	}
}
