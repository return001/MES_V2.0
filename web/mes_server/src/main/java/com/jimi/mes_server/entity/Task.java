package com.jimi.mes_server.entity;

/**排产计划计算预计时间的任务类
 * @author   HCJ
 * @date     2020年5月28日 下午6:20:29
 */
public class Task {

	/**
	 * orderId : 订单ID
	 */
	private Integer orderId;

	/**
	 * executorId : 产线ID
	 */
	private Integer executorId;

	/**
	 * switchConsumingTime : 换线时长
	 */
	private Integer switchConsumingTime;

	/**
	 * planQuantity : 计划产量
	 */
	private Integer planQuantity;

	/**
	 * standardCapacity : 标准产能
	 */
	private Integer standardCapacity;


	public Integer getOrderId() {
		return orderId;
	}


	public void setOrderId(Integer orderId) {
		this.orderId = orderId;
	}


	public Integer getExecutorId() {
		return executorId;
	}


	public void setExecutorId(Integer executorId) {
		this.executorId = executorId;
	}


	public Integer getSwitchConsumingTime() {
		return switchConsumingTime;
	}


	public void setSwitchConsumingTime(Integer switchConsumingTime) {
		this.switchConsumingTime = switchConsumingTime;
	}


	public Integer getPlanQuantity() {
		return planQuantity;
	}


	public void setPlanQuantity(Integer planQuantity) {
		this.planQuantity = planQuantity;
	}


	public Integer getStandardCapacity() {
		return standardCapacity;
	}


	public void setStandardCapacity(Integer standardCapacity) {
		this.standardCapacity = standardCapacity;
	}
}
