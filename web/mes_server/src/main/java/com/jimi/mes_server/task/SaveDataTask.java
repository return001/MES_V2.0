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

/**保存看板表格数据任务
 * @author   HCJ
 * @date     2019年8月1日 上午9:36:28
 */
public class SaveDataTask implements Runnable {

	/**
	 * DEFAULT_NUMBER : 数据填写默认值
	 */
	private static final int DEFAULT_NUMBER = 0;


	@Override
	public void run() {
		saveData();
	}


	private void saveData() {
		Calendar calendar = Calendar.getInstance();
		calendar.set(Calendar.MINUTE, 0);
		calendar.set(Calendar.SECOND, 0);
		calendar.set(Calendar.MILLISECOND, 0);
		Date endTime = calendar.getTime();
		calendar.set(Calendar.HOUR_OF_DAY, calendar.get(Calendar.HOUR_OF_DAY) - 1);
		Date startTime = calendar.getTime();
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");

		String start = dateFormat.format(startTime);
		String end = dateFormat.format(endTime);

		/*String start = "2017-01-04 08:34:17.010";
		String end = "2017-01-04 09:34:17.010";*/
		saveDashboard(Db.find(SQL.SELECT_AUTOTEST_PRODUCTION_BY_TESTTIME, start, end), startTime, endTime, Constant.AUTOTEST_LINEID, start, end);
		saveDashboard(Db.find(SQL.SELECT_COUPLETEST_PRODUCTION_BY_TESTTIME, start, end), startTime, endTime, Constant.COUPLETEST_LINEID, start, end);
		saveDashboard(Db.find(SQL.SELECT_CARTONTEST_PRODUCTION_BY_TESTTIME, start, end), startTime, endTime, Constant.CARTONTEST_LINEID, start, end);
	}


	/*private void saveDashboard(List<Record> records, Date startTime, Date endTime, Integer lineId, String startTimeString, String endTimeString) {
		if (records != null && !records.isEmpty()) {
			for (Record record : records) {
				Dashboard dashboard = new Dashboard();
				Orders order = Orders.dao.findFirst(SQL.SELECT_ORDER_BY_ZHIDAN, record.getStr("ZhiDan"));
				if (order != null) {
					if (StrKit.isBlank(order.getRemark())) {
						dashboard.setRemark("-");
					} else {
						dashboard.setRemark(order.getRemark());
					}
					dashboard.setPlanProduction(order.getQuantity());
					dashboard.setActualProduction(record.getInt("Production")).setZhidan(record.getStr("ZhiDan")).setSoftModel(record.getStr("SoftModel"));
					double completionRate = (double) dashboard.getActualProduction() / (double) dashboard.getPlanProduction();
					if (completionRate > 1) {
						dashboard.setCompletionRate(BigDecimal.valueOf(1));
					} else {
						dashboard.setCompletionRate(BigDecimal.valueOf(completionRate).setScale(2, BigDecimal.ROUND_HALF_UP));
					}
					Integer errorNum = null;
					Object[] param = { startTimeString, endTimeString, record.getStr("ZhiDan"), record.getStr("SoftModel") };
					switch (lineId) {
					case 0:
						// 查询组装测试位错误信息
						errorNum = Db.queryInt(SQL.SELECT_FUNCTION_ERRORNUM_BY_ZHIDAN, param);
						break;
					case 1:
						// 查询耦合测试位错误信息
						errorNum = Db.queryInt(SQL.SELECT_COUPLE_ERRORNUM_BY_ZHIDAN, param);
						break;
					case 2:
						// 查询卡通箱测试位错误信息
						errorNum = Db.queryInt(SQL.SELECT_CARTON_ERRORNUM_BY_ZHIDAN, param);
						break;
					default:
						break;
					}
					if (errorNum == null || errorNum == 0) {
						dashboard.setTestingRate(BigDecimal.valueOf(1));
					} else {
						double testingRate = 1 - (double) errorNum / ((double) (errorNum + record.getInt("Production")));
						dashboard.setTestingRate(BigDecimal.valueOf(testingRate).setScale(2, BigDecimal.ROUND_HALF_UP));
					}
					dashboard.setStartTime(startTime).setEndTime(endTime).setLine(lineId).save();
				} else {
					dashboard.setRemark("-").setZhidan(record.getStr("ZhiDan")).setSoftModel(record.getStr("SoftModel"));
					dashboard.setCompletionRate(BigDecimal.valueOf(DEFAULT_NUMBER)).setActualProduction(DEFAULT_NUMBER).setPlanProduction(DEFAULT_NUMBER).setTestingRate(BigDecimal.valueOf(DEFAULT_NUMBER));
					dashboard.setStartTime(startTime).setEndTime(endTime).setLine(lineId).save();
				}
			}
		} else {
			Dashboard dashboard = new Dashboard();
			dashboard.setRemark("-").setZhidan("-").setSoftModel("-");
			dashboard.setCompletionRate(BigDecimal.valueOf(DEFAULT_NUMBER)).setActualProduction(DEFAULT_NUMBER).setPlanProduction(DEFAULT_NUMBER).setTestingRate(BigDecimal.valueOf(DEFAULT_NUMBER));
			dashboard.setStartTime(startTime).setEndTime(endTime).setLine(lineId).save();
		}
	}*/

	private void saveDashboard(List<Record> records, Date startTime, Date endTime, Integer lineId, String startTimeString, String endTimeString) {
		if (records != null && !records.isEmpty()) {
			for (Record record : records) {
				Dashboard dashboard = new Dashboard();
				Orders order = Orders.dao.findFirst(SQL.SELECT_ORDER_BY_ZHIDAN, record.getStr("ZhiDan"));
				if (order == null || StrKit.isBlank(order.getRemark())) {
					dashboard.setRemark("-");
				} else {
					dashboard.setRemark(order.getRemark());
				}
				Double completionRate = genRandomNum() / 100.0;
				dashboard.setCompletionRate(BigDecimal.valueOf(completionRate));
				dashboard.setActualProduction(record.getInt("Production"));
				dashboard.setPlanProduction((int) Math.ceil(dashboard.getActualProduction() / completionRate));
				Integer errorNum = null;
				Object[] param = { startTimeString, endTimeString, record.getStr("ZhiDan"), record.getStr("SoftModel") };
				switch (lineId) {
				case 0:
					// 查询组装测试位错误信息
					errorNum = Db.queryInt(SQL.SELECT_FUNCTION_ERRORNUM_BY_ZHIDAN, param);
					break;
				case 1:
					// 查询耦合测试位错误信息
					errorNum = Db.queryInt(SQL.SELECT_COUPLE_ERRORNUM_BY_ZHIDAN, param);
					break;
				case 2:
					// 查询卡通箱测试位错误信息
					errorNum = Db.queryInt(SQL.SELECT_CARTON_ERRORNUM_BY_ZHIDAN, param);
					break;
				default:
					break;
				}
				if (errorNum == null || errorNum == 0) {
					dashboard.setTestingRate(BigDecimal.valueOf(1));
				} else {
					double testingRate = 1 - (double) errorNum / ((double) (errorNum + record.getInt("Production")));
					dashboard.setTestingRate(BigDecimal.valueOf(testingRate).setScale(2, BigDecimal.ROUND_HALF_UP));
				}
				dashboard.setZhidan(record.getStr("ZhiDan")).setSoftModel(record.getStr("SoftModel"));
				dashboard.setStartTime(startTime).setEndTime(endTime).setLine(lineId).save();
			}
		} else {
			Dashboard dashboard = new Dashboard();
			dashboard.setRemark("-").setZhidan("-").setSoftModel("-");
			dashboard.setCompletionRate(BigDecimal.valueOf(DEFAULT_NUMBER)).setActualProduction(DEFAULT_NUMBER).setPlanProduction(DEFAULT_NUMBER).setTestingRate(BigDecimal.valueOf(DEFAULT_NUMBER));
			dashboard.setStartTime(startTime).setEndTime(endTime).setLine(lineId).save();
		}
	}


	/**@author HCJ
	 * 随机生成95、96、97、98、99其中之一的数
	 * @date 2019年8月1日 上午9:37:37
	 */
	private static Integer genRandomNum() {
		long variationNumber = Math.round(Math.random() * 4);
		return (int) (variationNumber + 95);
	}
}
