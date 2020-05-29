package com.jimi.mes_server.entity;

import java.util.List;

/**排产计划计算预计时间的工作时间类
 * @author   HCJ
 * @date     2020年5月28日 下午6:22:14
 */
public class WorkTimes {

	/**
	 * executorId : 产线ID
	 */
	private Integer executorId;

	/**
	 * workTime : 工作时间的集合
	 */
	private List<WorkTime> workTime;


	public Integer getExecutorId() {
		return executorId;
	}


	public void setExecutorId(Integer executorId) {
		this.executorId = executorId;
	}


	public List<WorkTime> getWorkTime() {
		return workTime;
	}


	public void setWorkTime(List<WorkTime> workTime) {
		this.workTime = workTime;
	}
}
