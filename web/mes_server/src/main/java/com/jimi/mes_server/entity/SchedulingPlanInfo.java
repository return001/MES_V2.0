package com.jimi.mes_server.entity;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import com.jimi.mes_server.model.SchedulingPlan;

public class SchedulingPlanInfo {

	private Integer id;
	
	private Integer orders;
	private Boolean isUrgent;
	private Integer processGroup;
	private Integer line;
	private Integer schedulingQuantity;
	private Integer capacity;
	private String productionPlanningNumber;
	private Date planStartTime;
	private Date planCompleteTime;
	private Date startTime;
	private Date completeTime;
	private Integer producedQuantity;
	private Integer remainingQuantity;
	private String lineChangeTime;
	private Boolean isTimeout;
	private Integer schedulingPlanStatus;
	private String remark;
	private String remainingReason;
	private Integer scheduler;
	private Date schedulingTime;
	private Integer planModifier;
	private Date planModifyTime;
	private Integer productionConfirmer;
	private Integer personNumber;
	private Double rhythm;
	
	public Integer getId() {
		return id;
	}
	
	public void setId(Integer id) {
		this.id = id;
	}
	
	public Integer getOrders() {
		return orders;
	}
	
	public void setOrders(Integer orders) {
		this.orders = orders;
	}
	
	public Boolean getIsUrgent() {
		return isUrgent;
	}
	
	public void setIsUrgent(Boolean isUrgent) {
		this.isUrgent = isUrgent;
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
	
	public Integer getSchedulingQuantity() {
		return schedulingQuantity;
	}
	
	public void setSchedulingQuantity(Integer schedulingQuantity) {
		this.schedulingQuantity = schedulingQuantity;
	}
	
	public Integer getCapacity() {
		return capacity;
	}
	
	public void setCapacity(Integer capacity) {
		this.capacity = capacity;
	}
	
	public String getProductionPlanningNumber() {
		return productionPlanningNumber;
	}
	
	public void setProductionPlanningNumber(String productionPlanningNumber) {
		this.productionPlanningNumber = productionPlanningNumber;
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
	
	public Date getStartTime() {
		return startTime;
	}
	
	public void setStartTime(Date startTime) {
		this.startTime = startTime;
	}
	
	public Date getCompleteTime() {
		return completeTime;
	}
	
	public void setCompleteTime(Date completeTime) {
		this.completeTime = completeTime;
	}
	
	public Integer getProducedQuantity() {
		return producedQuantity;
	}
	
	public void setProducedQuantity(Integer producedQuantity) {
		this.producedQuantity = producedQuantity;
	}
	
	public Integer getRemainingQuantity() {
		return remainingQuantity;
	}
	
	public void setRemainingQuantity(Integer remainingQuantity) {
		this.remainingQuantity = remainingQuantity;
	}
	
	public String getLineChangeTime() {
		return lineChangeTime;
	}
	
	public void setLineChangeTime(String lineChangeTime) {
		this.lineChangeTime = lineChangeTime;
	}
	
	public Boolean getIsTimeout() {
		return isTimeout;
	}
	
	public void setIsTimeout(Boolean isTimeout) {
		this.isTimeout = isTimeout;
	}
	
	public Integer getSchedulingPlanStatus() {
		return schedulingPlanStatus;
	}
	
	public void setSchedulingPlanStatus(Integer schedulingPlanStatus) {
		this.schedulingPlanStatus = schedulingPlanStatus;
	}
	
	public String getRemark() {
		return remark;
	}
	
	public void setRemark(String remark) {
		this.remark = remark;
	}
	
	public String getRemainingReason() {
		return remainingReason;
	}
	
	public void setRemainingReason(String remainingReason) {
		this.remainingReason = remainingReason;
	}
	
	public Integer getScheduler() {
		return scheduler;
	}
	
	public void setScheduler(Integer scheduler) {
		this.scheduler = scheduler;
	}
	
	public Date getSchedulingTime() {
		return schedulingTime;
	}
	
	public void setSchedulingTime(Date schedulingTime) {
		this.schedulingTime = schedulingTime;
	}
	
	public Integer getPlanModifier() {
		return planModifier;
	}
	
	public void setPlanModifier(Integer planModifier) {
		this.planModifier = planModifier;
	}
	
	public Date getPlanModifyTime() {
		return planModifyTime;
	}
	
	public void setPlanModifyTime(Date planModifyTime) {
		this.planModifyTime = planModifyTime;
	}
	
	public Integer getProductionConfirmer() {
		return productionConfirmer;
	}
	
	public void setProductionConfirmer(Integer productionConfirmer) {
		this.productionConfirmer = productionConfirmer;
	}
	
	public Integer getPersonNumber() {
		return personNumber;
	}
	
	public void setPersonNumber(Integer personNumber) {
		this.personNumber = personNumber;
	}
	
	public Double getRhythm() {
		return rhythm;
	}
	
	public void setRhythm(Double rhythm) {
		this.rhythm = rhythm;
	}
	
	public static List<SchedulingPlan> fill(List<SchedulingPlanInfo> infos){
		List<SchedulingPlan> list = new ArrayList<SchedulingPlan>();
		for (SchedulingPlanInfo info : infos) {
			SchedulingPlan schedulingPlan = new SchedulingPlan();
			schedulingPlan.setCapacity(info.getCapacity());
			schedulingPlan.setCompleteTime(info.getCompleteTime());
			schedulingPlan.setIsUrgent(info.getIsUrgent());
			schedulingPlan.setLine(info.getLine());
			schedulingPlan.setLineChangeTime(info.getLineChangeTime());
			schedulingPlan.setOrders(info.getOrders());
			schedulingPlan.setPersonNumber(info.getPersonNumber());
			schedulingPlan.setPlanCompleteTime(info.getPlanCompleteTime());
			schedulingPlan.setPlanStartTime(info.getPlanStartTime());
			schedulingPlan.setProcessGroup(info.getProcessGroup());
			schedulingPlan.setProducedQuantity(info.getProducedQuantity());
			schedulingPlan.setProductionPlanningNumber(info.getProductionPlanningNumber());
			schedulingPlan.setRemainingQuantity(info.getRemainingQuantity());
			schedulingPlan.setRemainingReason(info.getRemainingReason());
			schedulingPlan.setRemark(info.getRemark());
			schedulingPlan.setRhythm(BigDecimal.valueOf(info.getRhythm()));
			schedulingPlan.setSchedulingPlanStatus(info.getSchedulingPlanStatus());
			schedulingPlan.setSchedulingQuantity(info.getSchedulingQuantity());
			schedulingPlan.setSchedulingTime(info.getSchedulingTime());
			if (info.getId() != null) {
				schedulingPlan.setId(info.getId());
			}
			list.add(schedulingPlan);
		}
		return list;
		
	}
}
