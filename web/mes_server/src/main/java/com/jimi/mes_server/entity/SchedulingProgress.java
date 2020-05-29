package com.jimi.mes_server.entity;

import java.util.Date;
import java.util.List;

/**排产进度类
 * @author   HCJ
 * @date     2020年5月28日 下午6:19:25
 */
public class SchedulingProgress {

	/**
	 * previousOrderPlanEndTime : 上一个订单计划完成时间
	 */
	private Date previousOrderPlanEndTime = null;

	/**
	 * previousOrderPlanEndTimeIndex : 上一个订单计划完成时间对应时间表上的位置
	 */
	private Integer previousOrderPlanEndTimeIndex = 0;

	/**
	 * workSchedule : 工作计划
	 */
	private List<WorkTime> workSchedule;


	public Date getPreviousOrderPlanEndTime() {
		return previousOrderPlanEndTime;
	}


	public void setPreviousOrderPlanEndTime(Date previousOrderPlanEndTime) {
		this.previousOrderPlanEndTime = previousOrderPlanEndTime;
	}


	public Integer getPreviousOrderPlanEndTimeIndex() {
		return previousOrderPlanEndTimeIndex;
	}


	public void setPreviousOrderPlanEndTimeIndex(Integer previousOrderPlanEndTimeIndex) {
		this.previousOrderPlanEndTimeIndex = previousOrderPlanEndTimeIndex;
	}


	public List<WorkTime> getWorkSchedule() {
		return workSchedule;
	}


	public void setWorkSchedule(List<WorkTime> workSchedule) {
		this.workSchedule = workSchedule;
	}
}
