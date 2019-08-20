package com.jimi.mes_server.service.base;

import java.util.ArrayList;
import java.util.List;

import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.SqlPara;

public class DeleteService {

	private final static String[] OPERATORS = { "#in#", "#like#", "#=#", "#<#", "#>#", "#>=#", "#<=#", "#!=#" };


	public void delete(String table, String filter, Integer type) {
		StringBuffer sql = new StringBuffer();
		List<String> questionValues = new ArrayList<String>();
		createDeleteFrom(sql, table);
		createWhere(filter, questionValues, sql);
		SqlPara sqlPara = new SqlPara();
		sqlPara.setSql(sql.toString());
		for (String para : questionValues) {
			sqlPara.addPara(para);
		}
		if (type != null) {
			switch (type) {
			case 0:
				Db.use("db1").update(sqlPara);
				break;
			case 1:
				Db.use("db2").update(sqlPara);
				break;
			case 2:
				Db.use("db2").update(sqlPara);
				break;
			case 3:
				Db.use("db3").update(sqlPara);
				break;
			case 4:
				Db.use("db3").update(sqlPara);
			default:
				break;
			}
		} else {
			Db.update(sqlPara);
		}
	}


	public void delete(String sql) {
		Db.delete(sql);
	}


	public void createDeleteFrom(StringBuffer sql, String table) {
		sql.append("delete from ");
		sql.append(table);
	}


	private void createWhere(String filter, List<String> questionValues, StringBuffer sql) {
		// 判断filter存在与否
		if (filter != null) {
			sql.append(" WHERE ");
			String[] whereUnits = filter.split("#&#");

			int index = 0;
			for (String whereUnit : whereUnits) {
				// 分割键值与运算符
				int operatorStartIndex = -1;
				StringBuffer operator = new StringBuffer();
				int operationLength = 0;
				int i = 0;
				for (; i < OPERATORS.length; i++) {
					operatorStartIndex = whereUnit.indexOf(OPERATORS[i]);
					if (operatorStartIndex != -1) {
						operationLength = OPERATORS[i].length();
						operator.append(OPERATORS[i].substring(1, operationLength - 1));
						break;
					}
				}
				String key = whereUnit.substring(0, operatorStartIndex);
				String value = whereUnit.substring(operatorStartIndex + operationLength, whereUnit.length());
				if (operator.toString().equals("in")) {
					sql.append(key + " " + operator.toString() + " " + value + " AND ");
				} else {
					sql.append(key + " " + operator.toString() + " ? AND ");
				}
				if (OPERATORS[i].equals("#like#")) {
					questionValues.add("%" + value + "%");
				} else if (!OPERATORS[i].equals("#in#")) {
					questionValues.add(value);
				}
				if (index == whereUnits.length - 1) {
					sql.delete(sql.lastIndexOf("AND"), sql.length());
				}
				index++;
			}
			System.out.println(sql + "  " + questionValues);
		}
	}
}
