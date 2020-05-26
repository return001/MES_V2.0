package com.jimi.mes_server.entity;

import java.util.Map;

/**排产计划甘特图类
 * @author   HCJ
 * @date     2019年8月20日 上午10:01:50
 */
public class PlanGantt {

	private Integer id;

	private String name;

	/**
	 * planProduction : 计划产量
	 */
	private String planProduction;

	/**
	 * intervalDay : 工期
	 */
	private String intervalDay;

	/**
	 * startTime : 开始时间
	 */
	private String startTime;

	/**
	 * endTime : 结束时间
	 */
	private String endTime;

	/**
	 * completionRate : 完成率
	 */
	private String completionRate;

	/**
	 * completionQuantity : 完成数量
	 */
	private String completionQuantity;

	private Map<String, Integer> dailyCapacity;


	public Map<String, Integer> getDailyCapacity() {
		return dailyCapacity;
	}


	public void setDailyCapacity(Map<String, Integer> dailyCapacity) {
		this.dailyCapacity = dailyCapacity;
	}


	public Integer getId() {
		return id;
	}


	public void setId(Integer id) {
		this.id = id;
	}


	public String getName() {
		return name;
	}


	public void setName(String name) {
		this.name = name;
	}


	public String getPlanProduction() {
		return planProduction;
	}


	public void setPlanProduction(String planProduction) {
		this.planProduction = planProduction;
	}


	public String getIntervalDay() {
		return intervalDay;
	}


	public void setIntervalDay(String intervalDay) {
		this.intervalDay = intervalDay;
	}


	public String getStartTime() {
		return startTime;
	}


	public void setStartTime(String startTime) {
		this.startTime = startTime;
	}


	public String getEndTime() {
		return endTime;
	}


	public void setEndTime(String endTime) {
		this.endTime = endTime;
	}


	public String getCompletionRate() {
		return completionRate;
	}


	public void setCompletionRate(String completionRate) {
		this.completionRate = completionRate;
	}


	public String getCompletionQuantity() {
		return completionQuantity;
	}


	public void setCompletionQuantity(String completionQuantity) {
		this.completionQuantity = completionQuantity;
	}

}
