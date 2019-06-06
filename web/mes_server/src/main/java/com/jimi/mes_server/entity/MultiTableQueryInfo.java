package com.jimi.mes_server.entity;

import java.util.List;

import com.jfinal.plugin.activerecord.Record;

/**多表查询信息类
 * @author   HCJ
 * @date     2019年5月30日 下午3:26:39
 */
public class MultiTableQueryInfo {

	private List<Record> dataRelativeSheets;

	private List<Record> dataRelativeUniques;

	private List<Record> gpsAutotestResults;

	private List<Record> gpsAutotestResult3s;

	private List<Record> gpsCartonboxtwentyResults;

	private List<Record> gpsCoupletestResults;

	private List<Record> gpsManucpparams;

	private List<Record> gpsManuprintparams;

	private List<Record> gpsTestresults;

	private List<Record> netMarkIMEIs;

	private List<Record> gpsManusimdataparams;

	public List<Record> getDataRelativeSheets() {
		return dataRelativeSheets;
	}

	public void setDataRelativeSheets(List<Record> dataRelativeSheets) {
		this.dataRelativeSheets = dataRelativeSheets;
	}

	public List<Record> getGpsAutotestResults() {
		return gpsAutotestResults;
	}

	public void setGpsAutotestResults(List<Record> gpsAutotestResults) {
		this.gpsAutotestResults = gpsAutotestResults;
	}

	public List<Record> getGpsAutotestResult3s() {
		return gpsAutotestResult3s;
	}

	public void setGpsAutotestResult3s(List<Record> gpsAutotestResult3s) {
		this.gpsAutotestResult3s = gpsAutotestResult3s;
	}

	public List<Record> getGpsCartonboxtwentyResults() {
		return gpsCartonboxtwentyResults;
	}

	public void setGpsCartonboxtwentyResults(List<Record> gpsCartonboxtwentyResults) {
		this.gpsCartonboxtwentyResults = gpsCartonboxtwentyResults;
	}

	public List<Record> getGpsCoupletestResults() {
		return gpsCoupletestResults;
	}

	public void setGpsCoupletestResults(List<Record> gpsCoupletestResults) {
		this.gpsCoupletestResults = gpsCoupletestResults;
	}

	public List<Record> getGpsManucpparams() {
		return gpsManucpparams;
	}

	public void setGpsManucpparams(List<Record> gpsManucpparams) {
		this.gpsManucpparams = gpsManucpparams;
	}

	public List<Record> getGpsManuprintparams() {
		return gpsManuprintparams;
	}

	public void setGpsManuprintparams(List<Record> gpsManuprintparams) {
		this.gpsManuprintparams = gpsManuprintparams;
	}

	public List<Record> getGpsTestresults() {
		return gpsTestresults;
	}

	public void setGpsTestresults(List<Record> gpsTestresults) {
		this.gpsTestresults = gpsTestresults;
	}

	public List<Record> getNetMarkIMEIs() {
		return netMarkIMEIs;
	}

	public void setNetMarkIMEIs(List<Record> netMarkIMEIs) {
		this.netMarkIMEIs = netMarkIMEIs;
	}

	public List<Record> getGpsManusimdataparams() {
		return gpsManusimdataparams;
	}

	public void setGpsManusimdataparams(List<Record> gpsManusimdataparams) {
		this.gpsManusimdataparams = gpsManusimdataparams;
	}

	public List<Record> getDataRelativeUniques() {
		return dataRelativeUniques;
	}

	public void setDataRelativeUniques(List<Record> dataRelativeUniques) {
		this.dataRelativeUniques = dataRelativeUniques;
	}

	public MultiTableQueryInfo(List<Record> dataRelativeSheets, List<Record> dataRelativeUniques,
			List<Record> gpsAutotestResults, List<Record> gpsAutotestResult3s, List<Record> gpsCartonboxtwentyResults,
			List<Record> gpsCoupletestResults, List<Record> gpsManucpparams, List<Record> gpsManuprintparams,
			List<Record> gpsTestresults, List<Record> netMarkIMEIs, List<Record> gpsManusimdataparams) {
		super();
		this.dataRelativeSheets = dataRelativeSheets;
		this.dataRelativeUniques = dataRelativeUniques;
		this.gpsAutotestResults = gpsAutotestResults;
		this.gpsAutotestResult3s = gpsAutotestResult3s;
		this.gpsCartonboxtwentyResults = gpsCartonboxtwentyResults;
		this.gpsCoupletestResults = gpsCoupletestResults;
		this.gpsManucpparams = gpsManucpparams;
		this.gpsManuprintparams = gpsManuprintparams;
		this.gpsTestresults = gpsTestresults;
		this.netMarkIMEIs = netMarkIMEIs;
		this.gpsManusimdataparams = gpsManusimdataparams;
	}
}
