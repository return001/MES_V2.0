package com.jimi.mes_server.service.base;

import java.util.ArrayList;
import java.util.List;

import com.jfinal.kit.PropKit;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Page;
import com.jfinal.plugin.activerecord.Record;
import com.jimi.mes_server.entity.Constant;
import com.jimi.mes_server.exception.ParameterException;

/**
 * 通用查询业务层
 * <br>
 * <b>2018年5月23日</b>
 * @author 沫熊工作室 <a href="http://www.darhao.cc">www.darhao.cc</a>
 */
public class SelectService {

	private final static String[] OPERATORS = { "#in#", "#like#", "#=#", "#<#", "#>#", "#>=#", "#<=#", "#!=#" };


	/**
	 * 分页查询，支持筛选和排序
	 * @param table 提供可读的表名
	 * @param pageNo 页码，从1开始
	 * @param pageSize 每页的条目数
	 * @param ascBy 按指定字段升序，不可和descBy同时使用
	 * @param descBy 按指定字段降序，不可和ascBy同时使用
	 * @param filter 按字段筛选，支持<, >, >,=, <=, !=, =，多个字段请用&隔开
	 * @return Page对象
	 */
	public Page<Record> select(String table, Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter, Integer type) {
		StringBuffer sql = new StringBuffer();
		List<String> questionValues = new ArrayList<>();
		createFrom(table, sql);
		createWhere(filter, questionValues, sql);
		createOrderBy(ascBy, descBy, sql);
		return paginateAndFillWhereValues(pageNo, pageSize, sql, questionValues, type);
	}


	public Page<Record> select(String resultSet, Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter) {
		StringBuffer sql = new StringBuffer();
		List<String> questionValues = new ArrayList<>();
		sql.append(resultSet.substring(resultSet.indexOf("FROM")));
		createWhere(filter, questionValues, sql);
		createOrderBy(ascBy, descBy, sql);
		return paginateAndFillWhereValues(resultSet, pageNo, pageSize, sql, questionValues);
	}


	private void createFrom(String table, StringBuffer sql) {
		// 表名非空判断
		if (table == null) {
			throw new ParameterException("table name must be provided");
		}
		// 表是否在可读范围内
		String[] reportTables = PropKit.use("properties.ini").get("readableTables").split(",");
		for (String tablePara : reportTables) {
			// 按冒号分割表名和主键名（多个主键用&隔开）
			if (tablePara.equals(table)) {
				sql.append(" FROM " + table);
				return;
			}
		}
		throw new ParameterException("not a readable table");
	}


	private void createWhere(String filter, List<String> questionValues, StringBuffer sql) {
		// 判断filter存在与否
		if (filter != null) {
			if (!sql.toString().contains(" where ") && !sql.toString().contains(" WHERE ")) {
				sql.append(" WHERE ");
			}
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


	private void createOrderBy(String ascBy, String descBy, StringBuffer sql) {
		if (ascBy != null && descBy != null) {
			throw new ParameterException("ascBy and descBy can not be provided at the same time");
		} else if (ascBy != null) {
			sql.append(" ORDER BY " + ascBy + " ASC ");
		} else if (descBy != null) {
			sql.append(" ORDER BY " + descBy + " DESC ");
		}
	}


	private Page<Record> paginateAndFillWhereValues(Integer pageNo, Integer pageSize, StringBuffer sql, List<String> questionValues, Integer type) {
		if ((pageNo != null && pageSize == null) || (pageNo == null && pageSize != null)) {
			throw new ParameterException("ascBy and descBy must be provided at the same time");
		}
		if (pageNo == null && pageSize == null) {
			if (type != null) {
				switch (type) {
				case 0:
					return Db.use("db1").paginate(1, PropKit.use("properties.ini").getInt("defaultPageSize"), "SELECT *", sql.toString(), questionValues.toArray());
				case 1:
					return Db.use("db2").paginate(1, PropKit.use("properties.ini").getInt("defaultPageSize"), "SELECT *", sql.toString(), questionValues.toArray());
				case 2:
					return Db.use("db2").paginate(1, PropKit.use("properties.ini").getInt("defaultPageSize"), "SELECT *", sql.toString(), questionValues.toArray());
				case 3:
					return Db.use("db3").paginate(1, PropKit.use("properties.ini").getInt("defaultPageSize"), "SELECT *", sql.toString(), questionValues.toArray());
				case 4:
					return Db.use("db3").paginate(1, PropKit.use("properties.ini").getInt("defaultPageSize"), "SELECT *", sql.toString(), questionValues.toArray());
				case 5:
					return Db.use("db2").paginate(1, PropKit.use("properties.ini").getInt("defaultPageSize"), "SELECT *", sql.toString(), questionValues.toArray());
				default:
					break;
				}
			} else {
				try {
					return Db.paginate(1, PropKit.use("properties.ini").getInt("defaultPageSize"), "SELECT *", sql.toString(), questionValues.toArray());
				} catch (Exception e) {
					e.printStackTrace();
				}

			}
		} else {
			if (type != null) {
				switch (type) {
				case 0:
					return Db.use("db1").paginate(pageNo, pageSize, "SELECT *", sql.toString(), questionValues.toArray());
				case 1:
					return Db.use("db2").paginate(pageNo, pageSize, "SELECT *", sql.toString(), questionValues.toArray());
				case 2:
					return Db.use("db2").paginate(pageNo, pageSize, "SELECT *", sql.toString(), questionValues.toArray());
				case 3:
					return Db.use("db3").paginate(pageNo, pageSize, "SELECT *", sql.toString(), questionValues.toArray());
				case 4:
					return Db.use("db3").paginate(pageNo, pageSize, "SELECT *", sql.toString(), questionValues.toArray());
				case 5:
					return Db.use("db2").paginate(pageNo, pageSize, "SELECT *", sql.toString(), questionValues.toArray());
				default:
					break;
				}
			} else {
				return Db.paginate(pageNo, pageSize, "SELECT *", sql.toString(), questionValues.toArray());
			}
		}
		return null;
	}


	private Page<Record> paginateAndFillWhereValues(String resultSet, Integer pageNo, Integer pageSize, StringBuffer sql, List<String> questionValues) {
		if ((pageNo != null && pageSize == null) || (pageNo == null && pageSize != null)) {
			throw new ParameterException("pageNo and pageSize must be provided at the same time");
		}
		if (pageNo == null && pageSize == null) {
			return Db.paginate(Constant.DEFAULT_PAGE_NUM, Constant.DEFAULT_PAGE_SIZE, resultSet.substring(0, resultSet.indexOf("FROM")), sql.toString(), questionValues.toArray());
		} else {
			System.out.println(sql.toString());
			return Db.paginate(pageNo, pageSize, resultSet.substring(0, resultSet.indexOf("FROM")), sql.toString(), questionValues.toArray());
		}
	}

}
