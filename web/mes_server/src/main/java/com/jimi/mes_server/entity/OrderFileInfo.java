package com.jimi.mes_server.entity;

import java.util.List;

import com.jfinal.plugin.activerecord.Record;

public class OrderFileInfo {

	private List<Record> information;

	public OrderFileInfo(List<Record> information, List<Record> bom, List<Record> sop) {
		super();
		this.information = information;
		this.bom = bom;
		this.sop = sop;
	}

	private List<Record> bom;

	private List<Record> sop;

	public List<Record> getInformation() {
		return information;
	}

	public void setInformation(List<Record> information) {
		this.information = information;
	}

	public List<Record> getBom() {
		return bom;
	}

	public void setBom(List<Record> bom) {
		this.bom = bom;
	}

	public List<Record> getSop() {
		return sop;
	}

	public void setSop(List<Record> sop) {
		this.sop = sop;
	}

}
