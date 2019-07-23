package com.jimi.mes_server.entity;

import java.util.List;

import com.jfinal.plugin.activerecord.Record;

public class OrderDetail {

	private List<Record> orderUser;

	private OrderFileInfo orderFileInfo;

	public List<Record> getOrderUser() {
		return orderUser;
	}

	public void setOrderUser(List<Record> orderUser) {
		this.orderUser = orderUser;
	}

	public OrderFileInfo getOrderFileInfo() {
		return orderFileInfo;
	}

	public void setOrderFileInfo(OrderFileInfo orderFileInfo) {
		this.orderFileInfo = orderFileInfo;
	}

}
