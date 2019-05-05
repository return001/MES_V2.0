package com.jimi.mes_report.task;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Record;
import com.jimi.mes_report.constant.Constant;
import com.jimi.mes_report.constant.SQL;
import com.jimi.mes_report.model.Capacity;
import com.jimi.mes_report.model.OrderProduction;
import com.jimi.mes_report.model.Orders;
import com.jimi.mes_report.model.WorkstationProduction;
import com.jimi.mes_report.util.ErrorLogWritter;

import cc.darhao.dautils.api.UuidUtil;

/**保存每天的数据
 * @author   HCJ
 * @date     2019年4月29日 上午11:23:22
 */
public class SaveDataTask implements Runnable {


	@Override
	public void run() {
		saveData();
	}


	public void saveData() {
		Calendar calendar = Calendar.getInstance();
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
		// 获取前一天时间
		calendar.add(Calendar.DAY_OF_MONTH, -1);
		String startTime = dateFormat.format(calendar.getTime()) + " 00:00:00";
		String endTime = dateFormat.format(calendar.getTime()) + " 23:59:59";
		SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
		// 设定当天记录的时间
		Date recordDate = new Date();
		try {
			recordDate = format.parse(dateFormat.format(calendar.getTime()) + " 23:00:00");
		} catch (ParseException e) {
			ErrorLogWritter.save(e.getClass().getSimpleName() + e.getMessage());
			e.printStackTrace();
		}
		List<Record> orderRecords = Db.use("db1").find(SQL.SELECT_ORDER_BY_TIME, startTime, endTime);
		// 格式化需要使用到打印日期和IMEI日期
		SimpleDateFormat printDateFormat = new SimpleDateFormat("yyyy.MM.dd");
		SimpleDateFormat IMEIDateFormat = new SimpleDateFormat("yyyy/M/d");
		String printTime = printDateFormat.format(recordDate);
		String IMEITime = IMEIDateFormat.format(recordDate);
		if (orderRecords != null && !orderRecords.isEmpty()) {
			for (Record orderRecord : orderRecords) {
				String zhiDan = orderRecord.getStr("ZhiDan");
				String version = orderRecord.getStr("Version");
				int cartonActualProduct = Db.use("db1").queryInt(SQL.SELECT_CARTONNUM_BY_ZHIDAN_VERSION, zhiDan, version, startTime, endTime);
				saveOrders(startTime, endTime, cartonActualProduct, orderRecord, printTime, IMEITime, recordDate);
			}
			saveOrderProduction(startTime, endTime, recordDate);
		} else {
			ErrorLogWritter.save("前一天不存在配置的工单和版本，无需保存数据");
			return;
		}
		saveWorkStationProductions(startTime, endTime, printTime, IMEITime, recordDate);
	}


	/**@author HCJ
	 * 保存订单产量
	 * @method saveOrderProduction
	 * @param startTime 开始时间
	 * @param endTime 结束时间
	 * @param recordDate 记录的时间
	 * @date 2019年5月5日 上午11:38:19
	 */
	private void saveOrderProduction(String startTime, String endTime, Date recordDate) {
		List<Record> orderRecords = Db.find(SQL.SELECT_ORDERS_BY_TIME, startTime, endTime);
		if (orderRecords != null && !orderRecords.isEmpty()) {
			int planProduct = 0;
			int actualProduct = 0;
			for (Record orderRecord : orderRecords) {
				planProduct += orderRecord.getInt("plan_product");
				actualProduct += orderRecord.getInt("actual_product");
			}
			OrderProduction orderProduction = new OrderProduction();
			orderProduction.setActualProduct(actualProduct);
			orderProduction.setPlanProduct(planProduct);
			orderProduction.setHourlyActualProduct(actualProduct / Constant.DAILY_WORKING_HOURS);
			orderProduction.setHourlyPlanProduct(planProduct / Constant.DAILY_WORKING_HOURS);
			orderProduction.setTimeType(Constant.TIME_TYPE_DAY);
			orderProduction.setTime(recordDate).save();
		} else {
			ErrorLogWritter.save("orders表格不存在前一天配置好的工单和版本");
		}
	}


	/**@author HCJ
	 * 保存所有工位的产量
	 * @param startTime 开始时间
	 * @param endTime 结束时间
	 * @param printTime 当天打印的时间
	 * @param IMEITime 当天IMEI对比的时间
	 * @param recordDate 记录的时间
	 * @date 2019年5月5日 上午11:39:38
	 */
	private void saveWorkStationProductions(String startTime, String endTime, String printTime, String IMEITime, Date recordDate) {
		Integer SMTProduction = Db.use("db1").queryInt(SQL.SELECT_SMTNUM_BY_TESTTIME, startTime, endTime);
		saveWorkStationProduction(SMTProduction, Constant.SMT_TEST_TYPE, recordDate);

		Integer functionProduction = Db.use("db1").queryInt(SQL.SELECT_FUNCTIONNUM_BY_TESTTIME, startTime, endTime);
		saveWorkStationProduction(functionProduction, Constant.FUNCTION_TEST_TYPE, recordDate);

		Integer agingProduction = Db.use("db1").queryInt(SQL.SELECT_AGINGNUM_BY_TESTTIME, startTime, endTime);
		saveWorkStationProduction(agingProduction, Constant.AGING_TEST_TYPE, recordDate);

		Integer couplingProduction = Db.use("db1").queryInt(SQL.SELECT_COUPLINGNUM_BY_TESTTIME, startTime, endTime);
		saveWorkStationProduction(couplingProduction, Constant.COUPLING_TEST_TYPE, recordDate);

		Integer JSTestProduction = Db.use("db1").queryInt(SQL.SELECT_JSPRINTNUM_BY_TESTTIME, printTime + "%");
		saveWorkStationProduction(JSTestProduction, Constant.JS_TEST_TYPE, recordDate);

		Integer CHTestProduction = Db.use("db1").queryInt(SQL.SELECT_CHPRINTNUM_BY_TESTTIME, printTime + "%");
		saveWorkStationProduction(CHTestProduction, Constant.CH_TEST_TYPE, recordDate);

		Integer IMEITestProduction = Db.use("db1").queryInt(SQL.SELECT_IMEINUM_BY_TESTTIME, IMEITime + " %",
				Constant.SUCCESS_CPRESULT);
		saveWorkStationProduction(IMEITestProduction, Constant.IMEI_TEST_TYPE, recordDate);

		Integer cartonTestProduction = Db.use("db1").queryInt(SQL.SELECT_CARTONNUM_BY_TESTTIME, startTime, endTime);
		saveWorkStationProduction(cartonTestProduction, Constant.CARTON_TEST_TYPE, recordDate);
	}


	/**@author HCJ
	 * 保存工位的产量
	 * @param workStationProduction 产量
	 * @param workStationType 工位类型
	 * @param recordDate 记录的时间
	 * @date 2019年4月29日 上午11:25:35
	 */
	private void saveWorkStationProduction(Integer workStationProduction, Integer workStationType, Date recordDate) {
		WorkstationProduction workstationProduction = new WorkstationProduction();
		if (workStationProduction != null) {
			workstationProduction.setProduct(workStationProduction);
			workstationProduction.setTimeType(Constant.TIME_TYPE_DAY);
			workstationProduction.setType(workStationType);
			workstationProduction.setTime(recordDate);
			workstationProduction.save();
		}
	}


	/**@author HCJ
	 * 保存所有订单
	 * @param startTime 开始时间
	 * @param endTime 结束时间
	 * @param cartonActualProduct 卡通箱实际产量
	 * @param order 订单信息记录
	 * @param printTime 当天打印的时间
	 * @param IMEITime 当天IMEI对比的时间
	 * @param recordDate 记录的时间
	 * @date 2019年5月5日 上午11:42:31
	 */
	private void saveOrders(String startTime, String endTime, int cartonActualProduct, Record order, String printTime, String IMEITime, Date recordDate) {
		Orders orders = new Orders();
		// 获取当前订单计划产量
		int planProduct = 0;
		try {
			long IMEIStart = Long.parseLong(order.getStr("IMEIStart"));
			long IMEIEnd = Long.parseLong(order.getStr("IMEIEnd"));
			planProduct = (int) (IMEIEnd - IMEIStart + 1);
		} catch (Exception e) {
			ErrorLogWritter.save(e.getClass().getSimpleName() + ":" + e.getMessage());
			e.printStackTrace();
		}
		orders.setId(UuidUtil.get32UUID());
		orders.setPlanProduct(planProduct);
		orders.setOrderNumber(order.getStr("ZhiDan"));
		orders.setSoftModel(order.getStr("SoftModel"));
		orders.setSoftVersion(order.getStr("Version"));
		orders.setProductionNumber(order.getStr("ProductNo"));
		orders.setStatus(order.getInt("status") + 1);
		orders.setActualProduct(cartonActualProduct);
		if (planProduct - cartonActualProduct > 0) {
			orders.setDiffenrence(planProduct - cartonActualProduct);
			String completionRate = String.valueOf((float) cartonActualProduct / (float) planProduct * 100);
			if (completionRate.length() > 5) {
				orders.setCompletionRate(completionRate.substring(0, 5));
			} else {
				orders.setCompletionRate(completionRate);
			}
		} else {
			orders.setDiffenrence(0);
			orders.setCompletionRate("100");
		}
		orders.setTime(recordDate);
		orders.save();
		saveCapacitys(orders.getId(), startTime, endTime, planProduct, cartonActualProduct, order.getStr("ZhiDan"), order.getStr("Version"), printTime, IMEITime);
	}


	/**@author HCJ
	 * 保存所有产能信息
	 * @param id 工单id
	 * @param startTime 开始时间
	 * @param endTime 结束时间
	 * @param cartonPlanProduct 卡通箱计划产量
	 * @param cartonActualProduct 卡通箱实际产量
	 * @param zhiDan 订单号
	 * @param version 版本
	 * @param printTime 当天打印的时间
	 * @param IMEITime 当天IMEI对比的时间
	 * @date 2019年5月5日 上午11:45:53
	 */
	private void saveCapacitys(String id, String startTime, String endTime, int cartonPlanProduct, int cartonActualProduct, String zhiDan, String version, String printTime, String IMEITime) {
		// 保存当前工单的卡通箱测试产量
		saveCapacity(id, cartonPlanProduct, Constant.CARTON_TEST_TYPE, cartonActualProduct);

		// 保存当前工单的SMT测试产量
		Integer SMTActualProduct = Db.use("db1").queryInt(SQL.SELECT_SMTNUM_BY_ZHIDAN_VERSION, zhiDan, version, startTime, endTime);
		saveCapacity(id, cartonPlanProduct, Constant.SMT_TEST_TYPE, SMTActualProduct);

		// 保存当前工单的功能测试产量
		Integer functionActualProduct = Db.use("db1").queryInt(SQL.SELECT_FUNCTIONNUM_BY_ZHIDAN_VERSION, zhiDan, version, startTime, endTime);
		saveCapacity(id, cartonPlanProduct, Constant.FUNCTION_TEST_TYPE, functionActualProduct);

		// 保存当前工单的老化后测试产量
		Integer agingActualProduct = Db.use("db1").queryInt(SQL.SELECT_AGINGNUM_BY_ZHIDAN_VERSION, zhiDan, version, startTime, endTime);
		saveCapacity(id, cartonPlanProduct, Constant.AGING_TEST_TYPE, agingActualProduct);

		// 保存当前工单的耦合测试产量
		Integer couplingActualProduct = Db.use("db1").queryInt(SQL.SELECT_COUPLINGNUM_BY_ZHIDAN_VERSION, zhiDan, version, startTime, endTime);
		saveCapacity(id, cartonPlanProduct, Constant.COUPLING_TEST_TYPE, couplingActualProduct);

		// 保存当前工单的机身贴测试产量
		Integer JSTestActualProduct = Db.use("db1").queryInt(SQL.SELECT_JSPRINTNUM_BY_ZHIDAN_VERSION, zhiDan, version, printTime + "%");
		saveCapacity(id, cartonPlanProduct, Constant.JS_TEST_TYPE, JSTestActualProduct);

		// 保存当前工单的彩盒贴测试产量
		Integer CHTestActualProduct = Db.use("db1").queryInt(SQL.SELECT_CHPRINTNUM_BY_ZHIDAN_VERSION, zhiDan, version, printTime + "%");
		saveCapacity(id, cartonPlanProduct, Constant.CH_TEST_TYPE, CHTestActualProduct);

		// 保存当前工单的IMEI测试产量
		Integer IMEITestActualProduct = Db.use("db1").queryInt(SQL.SELECT_IMEINUM_BY_ZHIDAN, zhiDan, IMEITime + " %", Constant.SUCCESS_CPRESULT);
		saveCapacity(id, cartonPlanProduct, Constant.IMEI_TEST_TYPE, IMEITestActualProduct);
	}


	/**@author HCJ
	 * 保存产能信息
	 * @param id 工单id
	 * @param cartonPlanProduct 卡通箱计划产量
	 * @param workStationType 工位类型
	 * @param actualProduct 实际生产数量
	 * @date 2019年4月29日 上午11:28:49
	 */
	private void saveCapacity(String id, Integer cartonPlanProduct, Integer workStationType, Integer actualProduct) {
		Capacity capacity = new Capacity();
		capacity.setOrders(id);
		capacity.setType(workStationType);
		if (actualProduct != null) {
			capacity.setActualProduct(actualProduct);
			capacity.setHourlyActualProduct(actualProduct / Constant.DAILY_WORKING_HOURS);
			capacity.setHourlyPlanProduct(cartonPlanProduct / Constant.DAILY_WORKING_HOURS);
		} else {
			capacity.setActualProduct(0);
			capacity.setHourlyActualProduct(0);
			capacity.setHourlyPlanProduct(0);
		}
		capacity.save();
	}
}
