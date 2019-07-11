package com.jimi.mes_server.entity;

public class DailyOrderProduction extends Production {

	private String zhidan;
	private String version;
	private String softModel;

	public String getZhidan() {
		return zhidan;
	}

	public void setZhidan(String zhidan) {
		this.zhidan = zhidan;
	}

	public String getVersion() {
		return version;
	}

	public void setVersion(String version) {
		this.version = version;
	}

	public String getSoftModel() {
		return softModel;
	}

	public void setSoftModel(String softModel) {
		this.softModel = softModel;
	}

	public DailyOrderProduction(String zhidan, String version, String softModel) {
		super();
		this.zhidan = zhidan;
		this.version = version;
		this.softModel = softModel;
	}

}
