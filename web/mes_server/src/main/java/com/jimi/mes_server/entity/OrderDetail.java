package com.jimi.mes_server.entity;

import com.jfinal.plugin.activerecord.Record;

public class OrderDetail {

	private Record orderUser;

	private OrderFileInfo orderFileInfo;

	public Record getOrderUser() {
		return orderUser;
	}

	public void setOrderUser(Record orderUser) {
		this.orderUser = orderUser;
	}

	public OrderFileInfo getOrderFileInfo() {
		return orderFileInfo;
	}

	public void setOrderFileInfo(OrderFileInfo orderFileInfo) {
		this.orderFileInfo = orderFileInfo;
	}

}
