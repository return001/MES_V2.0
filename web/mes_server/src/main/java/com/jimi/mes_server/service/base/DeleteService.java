package com.jimi.mes_server.service.base;

import java.util.ArrayList;
import java.util.List;

import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.SqlPara;

public class DeleteService {

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
		if(type != null) {
			switch(type)
			{
			case 0:
				Db.use("db1").update(sqlPara);
			case 1:
				Db.use("db2").update(sqlPara);
			case 2:
				Db.use("db2").update(sqlPara);
			case 3:
				Db.use("db3").update(sqlPara);
			case 4:
				Db.use("db3").update(sqlPara);
			default:
				break;
			}
		}else {
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
		//判断filter存在与否
		if(filter != null) {
			sql.append(" WHERE ");
			String[] whereUnits = filter.split("&");
			int index = 0;
			for (String whereUnit: whereUnits) {
				//分割键值与运算符
				int operatorStartIndex = -1;
				StringBuffer operator = new StringBuffer();
				for (int i = 0; i < whereUnit.length(); i++) {
					char c = whereUnit.charAt(i);
					if(c == '>' || c == '<' || c == '=' || c == '!') {
						operator.append(c);
						if(operatorStartIndex == -1) {
							operatorStartIndex = i;
						}
					}
				}
				String key = whereUnit.substring(0, operatorStartIndex);
				String value = whereUnit.substring(operatorStartIndex + operator.length(), whereUnit.length());
				sql.append(key + operator.toString() +"? AND ");
				questionValues.add(value);
				if(index == whereUnits.length - 1) {
					sql.delete(sql.lastIndexOf("AND"), sql.length());
				}
				index++;
			}
		}
	}
}
