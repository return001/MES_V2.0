package com.jimi.mes_server.entity;

import com.jfinal.plugin.activerecord.Record;

public class PlanDetail {

	private Record planUser;

	private Record order;

	
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
