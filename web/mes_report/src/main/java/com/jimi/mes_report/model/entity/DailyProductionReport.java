package com.jimi.mes_report.model.entity;

import java.util.LinkedList;
import java.util.List;

/**
 * 日产能报表
 * <br>
 * <b>2020年4月7日</b>
 * @author <a href="https://github.com/darhao">鲁智深</a>
 */
public class DailyProductionReport {
	
	private int output;
	
	private int highestOutputInHistory;
	
	private int consumingTime;
	
	private int highestConsumingTimeInHistory;
	
	private List items;
	
	
	/**
	 * 添加报表条目
	 * @param item 日产量报表条目
	 */
	public void addItem(DailyProductionReportItem item) {
		items.add(item);
	}
	
	public void setItems(List<DailyProductionReportItem> items) {
		this.items = items;
	}
	
	
	/**
	 * 获取报表条目
	 * @return 日产量报表条目列表
	 */
	public List getItems() {
		return new LinkedList<DailyProductionReportItem>(items);
	}
	
	
	/**
	 * 获取实际日产量
	 */
	public int getOutput() {
		return output;
	}
	
	
	/**
	 * 获取历史最高日产量
	 */
	public int getHighestOutputInHistory() {
		return highestOutputInHistory;
	}
	
	
	/**
	 * 获取实际日生产总耗时（分钟）
	 */
	public int getConsumingTime() {
		return consumingTime;
	}
	
	
	/**
	 * 获取历史最高日生产总耗时（分钟）
	 */
	public int getHighestConsumingTimeInHistory() {
		return highestConsumingTimeInHistory;
	}
	
	
	/**
	 * @param output 实际日产量
	 * @param highestOutputInHistory 历史最高日产量
	 * @param consumingTime 实际日生产总耗时（分钟）
	 * @param highestConsumingTimeInHistory 历史最高日生产总耗时（分钟）
	 */
	public DailyProductionReport(int output, int highestOutputInHistory, int consumingTime,
			int highestConsumingTimeInHistory) {
		this.output = output;
		this.highestOutputInHistory = highestOutputInHistory;
		this.consumingTime = consumingTime;
		this.highestConsumingTimeInHistory = highestConsumingTimeInHistory;
		this.items = new LinkedList<DailyProductionReportItem>();
	}
	
}
