package com.jimi.mes_server.entity;

import java.util.List;

import com.jfinal.plugin.activerecord.Record;

public class DashboardInfo {

	private List<Record> table;

	private List<Record> error;

	public List<Record> getTable() {
		return table;
	}

	public void setTable(List<Record> table) {
		this.table = table;
	}

	public List<Record> getError() {
		return error;
	}

	public void setError(List<Record> error) {
		this.error = error;
	}
}
