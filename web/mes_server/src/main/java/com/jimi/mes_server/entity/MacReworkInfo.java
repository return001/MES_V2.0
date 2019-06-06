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

	private List<GpsAutotestAntidup> gpsAutotestAntidups;

	private List<GpsAutotestResult2> gpsAutotestResult2s;

	private List<GpsTestresult> gpsTestresults;

	public List<GpsAutotestAntidup> getGpsAutotestAntidups() {
		return gpsAutotestAntidups;
	}

	public void setGpsAutotestAntidups(List<GpsAutotestAntidup> gpsAutotestAntidups) {
		this.gpsAutotestAntidups = gpsAutotestAntidups;
	}

	public List<GpsAutotestResult2> getGpsAutotestResult2s() {
		return gpsAutotestResult2s;
	}

	public void setGpsAutotestResult2s(List<GpsAutotestResult2> gpsAutotestResult2s) {
		this.gpsAutotestResult2s = gpsAutotestResult2s;
	}

	public List<GpsTestresult> getGpsTestresults() {
		return gpsTestresults;
	}

	public void setGpsTestresults(List<GpsTestresult> gpsTestresults) {
		this.gpsTestresults = gpsTestresults;
	}

	public MacReworkInfo(List<GpsAutotestAntidup> gpsAutotestAntidups, List<GpsAutotestResult2> gpsAutotestResult2s, List<GpsTestresult> gpsTestresults) {
		super();
		this.gpsAutotestAntidups = gpsAutotestAntidups;
		this.gpsAutotestResult2s = gpsAutotestResult2s;
		this.gpsTestresults = gpsTestresults;
	}
}
