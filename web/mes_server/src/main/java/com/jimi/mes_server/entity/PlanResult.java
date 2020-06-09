package com.jimi.mes_server.entity;

public class PlanResult {

	private Integer orderId;

	private Integer executorId;

	private Double estimatedProductionTime;

	private String estimatedStartTime;

	private String estimatedEndTime;

	private String tagId;


	public String getTagId() {
		return tagId;
	}


	public void setTagId(String tagId) {
		this.tagId = tagId;
	}


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


	public Double getEstimatedProductionTime() {
		return estimatedProductionTime;
	}


	public void setEstimatedProductionTime(Double estimatedProductionTime) {
		this.estimatedProductionTime = estimatedProductionTime;
	}


	public String getEstimatedStartTime() {
		return estimatedStartTime;
	}


	public void setEstimatedStartTime(String estimatedStartTime) {
		this.estimatedStartTime = estimatedStartTime;
	}


	public String getEstimatedEndTime() {
		return estimatedEndTime;
	}


	public void setEstimatedEndTime(String estimatedEndTime) {
		this.estimatedEndTime = estimatedEndTime;
	}
}
