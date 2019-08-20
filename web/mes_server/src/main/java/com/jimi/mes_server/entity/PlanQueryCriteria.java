package com.jimi.mes_server.entity;

/**排产计划查询条件类
 * @author   HCJ
 * @date     2019年8月20日 上午10:05:49
 */
public class PlanQueryCriteria {

	private Integer pageNo;

	private Integer pageSize;

	private Integer schedulingPlanStatus;

	private String zhidan;

	private String customerName;

	private String orderDateFrom;

	private String orderDateTo;

	private String planStartTimeFrom;

	private String planStartTimeTo;

	private String planCompleteTimeFrom;

	private String planCompleteTimeTo;

	private String startTimeFrom;

	private String startTimeTo;

	private String completeTimeFrom;

	private String completeTimeTo;

	private Integer processGroup;

	private Integer line;

	private String productionPlanningNumber;

	private String softModel;

	private String productNo;


	public Integer getPageNo() {
		return pageNo;
	}


	public void setPageNo(Integer pageNo) {
		this.pageNo = pageNo;
	}


	public Integer getPageSize() {
		return pageSize;
	}


	public void setPageSize(Integer pageSize) {
		this.pageSize = pageSize;
	}


	public Integer getSchedulingPlanStatus() {
		return schedulingPlanStatus;
	}


	public void setSchedulingPlanStatus(Integer schedulingPlanStatus) {
		this.schedulingPlanStatus = schedulingPlanStatus;
	}


	public String getZhidan() {
		return zhidan;
	}


	public void setZhidan(String zhidan) {
		this.zhidan = zhidan;
	}


	public String getCustomerName() {
		return customerName;
	}


	public void setCustomerName(String customerName) {
		this.customerName = customerName;
	}


	public String getOrderDateFrom() {
		return orderDateFrom;
	}


	public void setOrderDateFrom(String orderDateFrom) {
		this.orderDateFrom = orderDateFrom;
	}


	public String getOrderDateTo() {
		return orderDateTo;
	}


	public void setOrderDateTo(String orderDateTo) {
		this.orderDateTo = orderDateTo;
	}


	public String getPlanStartTimeFrom() {
		return planStartTimeFrom;
	}


	public void setPlanStartTimeFrom(String planStartTimeFrom) {
		this.planStartTimeFrom = planStartTimeFrom;
	}


	public String getPlanStartTimeTo() {
		return planStartTimeTo;
	}


	public void setPlanStartTimeTo(String planStartTimeTo) {
		this.planStartTimeTo = planStartTimeTo;
	}


	public String getPlanCompleteTimeFrom() {
		return planCompleteTimeFrom;
	}


	public void setPlanCompleteTimeFrom(String planCompleteTimeFrom) {
		this.planCompleteTimeFrom = planCompleteTimeFrom;
	}


	public String getPlanCompleteTimeTo() {
		return planCompleteTimeTo;
	}


	public void setPlanCompleteTimeTo(String planCompleteTimeTo) {
		this.planCompleteTimeTo = planCompleteTimeTo;
	}


	public String getStartTimeFrom() {
		return startTimeFrom;
	}


	public void setStartTimeFrom(String startTimeFrom) {
		this.startTimeFrom = startTimeFrom;
	}


	public String getStartTimeTo() {
		return startTimeTo;
	}


	public void setStartTimeTo(String startTimeTo) {
		this.startTimeTo = startTimeTo;
	}


	public String getCompleteTimeFrom() {
		return completeTimeFrom;
	}


	public void setCompleteTimeFrom(String completeTimeFrom) {
		this.completeTimeFrom = completeTimeFrom;
	}


	public String getCompleteTimeTo() {
		return completeTimeTo;
	}


	public void setCompleteTimeTo(String completeTimeTo) {
		this.completeTimeTo = completeTimeTo;
	}


	public Integer getProcessGroup() {
		return processGroup;
	}


	public void setProcessGroup(Integer processGroup) {
		this.processGroup = processGroup;
	}


	public Integer getLine() {
		return line;
	}


	public void setLine(Integer line) {
		this.line = line;
	}


	public String getProductionPlanningNumber() {
		return productionPlanningNumber;
	}


	public void setProductionPlanningNumber(String productionPlanningNumber) {
		this.productionPlanningNumber = productionPlanningNumber;
	}


	public String getSoftModel() {
		return softModel;
	}


	public void setSoftModel(String softModel) {
		this.softModel = softModel;
	}


	public String getProductNo() {
		return productNo;
	}


	public void setProductNo(String productNo) {
		this.productNo = productNo;
	}


	@Override
	public String toString() {
		return "PlanQueryCriteria [pageNo=" + pageNo + ", pageSize=" + pageSize + ", schedulingPlanStatus=" + schedulingPlanStatus + ", zhidan=" + zhidan + ", customerName=" + customerName + ", orderDateFrom=" + orderDateFrom + ", orderDateTo=" + orderDateTo + ", planStartTimeFrom=" + planStartTimeFrom + ", planStartTimeTo=" + planStartTimeTo + ", planCompleteTimeFrom=" + planCompleteTimeFrom + ", planCompleteTimeTo=" + planCompleteTimeTo + ", startTimeFrom=" + startTimeFrom + ", startTimeTo=" + startTimeTo + ", completeTimeFrom=" + completeTimeFrom + ", completeTimeTo=" + completeTimeTo + ", processGroup=" + processGroup + ", line=" + line + ", productionPlanningNumber=" + productionPlanningNumber + ", softModel=" + softModel + ", productNo=" + productNo + "]";
	}
}
