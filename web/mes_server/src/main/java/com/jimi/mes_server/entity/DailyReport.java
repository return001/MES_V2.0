package com.jimi.mes_server.entity;

import java.util.List;

import com.jimi.mes_server.model.DailyCompletion;

public class DailyReport {

	private List<DailyOrderProduction> orderProductions;
	private Production production;
	private List<DailyCompletion> dailyCompletions;

	public List<DailyOrderProduction> getOrderProductions() {
		return orderProductions;
	}

	public void setOrderProductions(List<DailyOrderProduction> orderProductions) {
		this.orderProductions = orderProductions;
	}

	public Production getProduction() {
		return production;
	}

	public void setProduction(Production production) {
		this.production = production;
	}

	public List<DailyCompletion> getDailyCompletions() {
		return dailyCompletions;
	}

	public void setDailyCompletions(List<DailyCompletion> dailyCompletions) {
		this.dailyCompletions = dailyCompletions;
	}

	public DailyReport(List<DailyOrderProduction> orderProductions, Production production, List<DailyCompletion> dailyCompletions) {
		super();
		this.orderProductions = orderProductions;
		this.production = production;
		this.dailyCompletions = dailyCompletions;
	}
}
