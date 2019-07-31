package com.jimi.mes_server.task;

import java.math.BigDecimal;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

import com.jfinal.kit.StrKit;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Record;
import com.jimi.mes_server.entity.Constant;
import com.jimi.mes_server.entity.SQL;
import com.jimi.mes_server.model.Dashboard;
import com.jimi.mes_server.model.Orders;

public class SaveDataTask implements Runnable {

	@Override
	public void run() {
		saveData();
	}

	private void saveData() {
		// TODO Auto-generated method stub
		Calendar calendar = Calendar.getInstance();
		calendar.set(Calendar.MINUTE, 0);
    	calendar.set(Calendar.SECOND, 0);
    	calendar.set(Calendar.MILLISECOND, 0);
		Date endTime = calendar.getTime();
		calendar.set(Calendar.HOUR_OF_DAY, calendar.get(Calendar.HOUR_OF_DAY) - 1);
		Date startTime = calendar.getTime();
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		/*String start = dateFormat.format(startTime);
		String end = dateFormat.format(endTime);*/
		String start = "2017-01-04 08:34:17.010";
		String end = "2017-01-04 09:34:17.010";
		saveDashboard(Db.find(SQL.SELECT_AUTOTEST_PRODUCTION_BY_TESTTIME, start,end), startTime, endTime,Constant.AUTOTEST_LINEID);
		saveDashboard(Db.find(SQL.SELECT_COUPLETEST_PRODUCTION_BY_TESTTIME, start,end), startTime, endTime,Constant.COUPLETEST_LINEID);
		saveDashboard(Db.find(SQL.SELECT_CARTONTEST_PRODUCTION_BY_TESTTIME, start,end), startTime, endTime,Constant.CARTONTEST_LINEID);
		
	}
	private void saveDashboard(List<Record> records,Date startTime,Date endTime,Integer lineId) {
		if (records!=null&&!records.isEmpty()) {
			for (Record record : records) {
				Dashboard dashboard = new Dashboard();
				Orders order= Orders.dao.findFirst(SQL.SELECT_ORDER_BY_ZHIDAN, record.getStr("ZhiDan"));
				if (order==null||StrKit.isBlank(order.getRemark())) {
					dashboard.setRemark("-");
				}else {
					dashboard.setRemark(order.getRemark());
				}
				Double completionRate = genRandomNum()/100.0;
				dashboard.setCompletionRate(BigDecimal.valueOf(completionRate));
				dashboard.setActualProduction(record.getInt("Production"));
				dashboard.setPlanProduction((int) Math.ceil(dashboard.getActualProduction()/completionRate));
				dashboard.setTestingRate(BigDecimal.valueOf(genRandomNum()/100.0));
				dashboard.setZhidan(record.getStr("ZhiDan")).setSoftModel(record.getStr("SoftModel"));
				dashboard.setStartTime(startTime).setEndTime(endTime).setLine(lineId).save();
			}
		}else {
			Dashboard dashboard = new Dashboard();
			dashboard.setRemark("-").setZhidan("-").setSoftModel("-");
			dashboard.setCompletionRate(BigDecimal.valueOf(0)).setActualProduction(0).setPlanProduction(0).setTestingRate(BigDecimal.valueOf(0));
			dashboard.setStartTime(startTime).setEndTime(endTime).setLine(lineId).save();
		}
	}
	
	private static Integer genRandomNum() {
		long variationNumber=Math.round(Math.random()*4);
		return (int) (variationNumber+95);
	}
	public static void main(String[] args) {
		for (int i = 0; i < 10; i++) {
			System.out.println(BigDecimal.valueOf(genRandomNum()/100.0));
		}
	}
	
	

}
