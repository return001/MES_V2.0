package com.jimi.mes_server.entity;

import java.util.List;

import com.jfinal.plugin.activerecord.Record;

/**订单文件信息类
 * @author   HCJ
 * @date     2019年8月20日 上午10:00:27
 */
public class OrderFileInfo {

	/**
	 * information : 信息表
	 */
	private List<Record> information;

	/**
	 * bom : BOM表
	 */
	private List<Record> bom;

	/**
	 * sop : SOP表
	 */
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


	public OrderFileInfo(List<Record> information, List<Record> bom, List<Record> sop) {
		super();
		this.information = information;
		this.bom = bom;
		this.sop = sop;
	}

}
