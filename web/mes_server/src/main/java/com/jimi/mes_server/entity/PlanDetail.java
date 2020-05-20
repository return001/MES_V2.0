package com.jimi.mes_server.entity;

import java.util.Map;

import com.jfinal.plugin.activerecord.Record;

/**计划详情类
 * @author   HCJ
 * @date     2019年8月27日 下午3:17:14
 */
public class PlanDetail {

	/**
	 * planUser : 计划使用者
	 */
	private Record planUser;

	/**
	 * order : 订单详情
	 */
	private Record order;
	
	private Map<String, String> picture;


	
	public Map<String, String> getPicture() {
		return picture;
	}


	
	public void setPicture(Map<String, String> picture) {
		this.picture = picture;
	}


	public Record getPlanUser() {
		return planUser;
	}


	public void setPlanUser(Record planUser) {
		this.planUser = planUser;
	}


	public Record getOrder() {
		return order;
	}


	public void setOrder(Record order) {
		this.order = order;
	}

}
