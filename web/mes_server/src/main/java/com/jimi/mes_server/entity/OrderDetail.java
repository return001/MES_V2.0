package com.jimi.mes_server.entity;

import com.jfinal.plugin.activerecord.Record;

/**订单详情类
 * @author   HCJ
 * @date     2019年8月20日 上午9:59:42
 */
public class OrderDetail {

	/**
	 * orderUser : 订单操作者
	 */
	private Record orderUser;

	/**
	 * orderFileInfo : 订单文件信息
	 */
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
