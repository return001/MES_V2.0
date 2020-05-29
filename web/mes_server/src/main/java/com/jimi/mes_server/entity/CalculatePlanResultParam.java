package com.jimi.mes_server.entity;

import java.util.List;

/**计算排产计划预计时间的参数类
 * @author   HCJ
 * @date     2020年5月28日 下午6:18:37
 */
public class CalculatePlanResultParam {

	/**
	 * workTimes : 工作时间的集合
	 */
	private List<WorkTimes> workTimes;

	/**
	 * tasks : 任务的集合
	 */
	private List<Task> tasks;


	public List<WorkTimes> getWorkTimes() {
		return workTimes;
	}


	public void setWorkTimes(List<WorkTimes> workTimes) {
		this.workTimes = workTimes;
	}


	public List<Task> getTasks() {
		return tasks;
	}


	public void setTasks(List<Task> tasks) {
		this.tasks = tasks;
	}
}
