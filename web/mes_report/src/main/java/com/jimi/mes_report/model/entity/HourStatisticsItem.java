package com.jimi.mes_report.model.entity;

/**
 * 小时统计条目
 * <br>
 * <b>2020年4月7日</b>
 * @author <a href="https://github.com/darhao">鲁智深</a>
 */
public class HourStatisticsItem {

	private int hour;
	private int output;
	private int consumingTime;

	/**
	 * 获取小时
	 */
	public int getHour() {
		return hour;
	}

	/**
	 * 获取产量
	 */
	public int getOutput() {
		return output;
	}

	/**
	 * 获取耗时（秒）
	 */
	public int getConsumingTime() {
		return consumingTime;
	}

	/**
	 * @param hour 小时
	 * @param output 产量
	 * @param consumingTime 耗时（秒）
	 */
	HourStatisticsItem(int hour, int output, int consumingTime) {
		this.hour = hour;
		this.output = output;
		this.consumingTime = consumingTime;
	}
}
