package com.jimi.mes_server.entity;

import java.util.Date;

public class AddPlanInfo {

	private Integer order;

	private String remark;

	private String schedulingQuantity;

	private String line;

	private String capacity;

	private Integer processGroup;

	private Date planStartTime;

	private Date planCompleteTime;


	public Integer getOrder() {
		return order;
	}


	public void setOrder(Integer order) {
		this.order = order;
	}


	public String getRemark() {
		return remark;
	}


	public void setRemark(String remark) {
		this.remark = remark;
	}


	public String getSchedulingQuantity() {
		return schedulingQuantity;
	}


	public void setSchedulingQuantity(String schedulingQuantity) {
		this.schedulingQuantity = schedulingQuantity;
	}


	public String getLine() {
		return line;
	}


	public void setLine(String line) {
		this.line = line;
	}


	public String getCapacity() {
		return capacity;
	}


	public void setCapacity(String capacity) {
		this.capacity = capacity;
	}


	public Integer getProcessGroup() {
		return processGroup;
	}


	public void setProcessGroup(Integer processGroup) {
		this.processGroup = processGroup;
	}


	public Date getPlanStartTime() {
		return planStartTime;
	}


	public void setPlanStartTime(Date planStartTime) {
		this.planStartTime = planStartTime;
	}


	public Date getPlanCompleteTime() {
		return planCompleteTime;
	}


	public void setPlanCompleteTime(Date planCompleteTime) {
		this.planCompleteTime = planCompleteTime;
	}

}
