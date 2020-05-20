package com.jimi.mes_server.entity;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import com.jimi.mes_server.model.SchedulingPlan;

/**添加计划信息类
 * @author   HCJ
 * @date     2019年8月27日 下午3:12:09
 */
public class AddPlanInfo {

	/**
	 * id : 排产计划ID
	 */
	private Integer id;

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
	private Integer schedulingQuantity;

	/**
	 * line : 所有的产线ID，以逗号分隔
	 */
	private Integer line;

	/**
	 * capacity : 所有的产能，以逗号分隔
	 */
	private Integer capacity;

	/**
	 * processGroup : 工序组ID
	 */
	private Integer processGroup;

	/**
	 * planStartTime : 所有的计划开始时间，以逗号分隔
	 */
	private Date planStartTime;

	/**
	 * planCompleteTime : 所有的计划结束时间，以逗号分隔
	 */
	private Date planCompleteTime;

	/**
	 * peopleNumber : 所有的人数，以逗号分隔
	 */
	private Integer personNumber;

	/**
	 * rhythm : 所有的节拍，以逗号分隔
	 */
	private Double rhythm;

	/**
	 * lineChangeTime : 产线换线时长，以逗号分隔
	 */
	private String lineChangeTime;

	/**
	 * isUrgent : 是否紧急
	 */
	private Boolean isUrgent;


	public Boolean getIsUrgent() {
		return isUrgent;
	}


	public void setIsUrgent(Boolean isUrgent) {
		this.isUrgent = isUrgent;
	}


	public Integer getId() {
		return id;
	}


	public void setId(Integer id) {
		this.id = id;
	}


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


	public Integer getSchedulingQuantity() {
		return schedulingQuantity;
	}


	public void setSchedulingQuantity(Integer schedulingQuantity) {
		this.schedulingQuantity = schedulingQuantity;
	}


	public Integer getLine() {
		return line;
	}


	public void setLine(Integer line) {
		this.line = line;
	}


	public Integer getCapacity() {
		return capacity;
	}


	public void setCapacity(Integer capacity) {
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


	public String getLineChangeTime() {
		return lineChangeTime;
	}


	public void setLineChangeTime(String lineChangeTime) {
		this.lineChangeTime = lineChangeTime;
	}


	public static List<SchedulingPlan> fill(List<AddPlanInfo> infos) {
		List<SchedulingPlan> list = new ArrayList<SchedulingPlan>();
		for (AddPlanInfo info : infos) {
			SchedulingPlan schedulingPlan = new SchedulingPlan();
			schedulingPlan.setOrders(info.getOrder());
			schedulingPlan.setCapacity(info.getCapacity());
			schedulingPlan.setLine(info.getLine());
			schedulingPlan.setLineChangeTime(info.getLineChangeTime());
			schedulingPlan.setPersonNumber(info.getPersonNumber());
			schedulingPlan.setPlanCompleteTime(info.getPlanCompleteTime());
			schedulingPlan.setPlanStartTime(info.getPlanStartTime());
			schedulingPlan.setProcessGroup(info.getProcessGroup());
			schedulingPlan.setRemark(info.getRemark());
			schedulingPlan.setRhythm(BigDecimal.valueOf(info.getRhythm()));
			schedulingPlan.setSchedulingQuantity(info.getSchedulingQuantity());
			schedulingPlan.setIsUrgent(info.getIsUrgent());
			if (info.getId() != null) {
				schedulingPlan.setId(info.getId());
			}
			list.add(schedulingPlan);
		}
		return list;

	}

}
