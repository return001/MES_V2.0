package com.jimi.mes_server.entity;

import java.util.Date;

/**添加计划信息类
 * @author   HCJ
 * @date     2019年8月27日 下午3:12:09
 */
public class AddPlanInfo {

	/**
	 * order : 订单ID
	 */
	private Integer order;

	/**
	 * remark : 备注
	 */
	private String remark;

	/**
	 * schedulingQuantity : 所有的排产数量，以逗号分隔
	 */
	private String schedulingQuantity;

	/**
	 * line : 所有的产线ID，以逗号分隔
	 */
	private String line;

	/**
	 * capacity : 所有的产能，以逗号分隔
	 */
	private String capacity;

	/**
	 * processGroup : 工序组ID
	 */
	private Integer processGroup;

	/**
	 * planStartTime : 计划开始时间
	 */
	private Date planStartTime;

	/**
	 * planCompleteTime : 计划结束时间
	 */
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
