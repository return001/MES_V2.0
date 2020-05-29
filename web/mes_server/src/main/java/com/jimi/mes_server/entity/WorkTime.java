package com.jimi.mes_server.entity;

import java.util.Date;

/**工作时间类
 * @author   HCJ
 * @date     2020年5月28日 下午6:21:36
 */
public class WorkTime {

	/**
	 * date : 日期
	 */
	private Date date;

	/**
	 * startTime : 开始时间
	 */
	private String startTime;

	/**
	 * endTime : 结束时间
	 */
	private String endTime;


	public Date getDate() {
		return date;
	}


	public void setDate(Date date) {
		this.date = date;
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
}
