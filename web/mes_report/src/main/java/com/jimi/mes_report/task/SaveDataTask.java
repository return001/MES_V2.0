package com.jimi.mes_report.task;

import java.math.BigDecimal;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

import com.jfinal.kit.StrKit;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Record;
import com.jimi.mes_report.constant.Constant;
import com.jimi.mes_report.constant.SQL;
import com.jimi.mes_report.model.Capacity;
import com.jimi.mes_report.model.OrderProduction;
import com.jimi.mes_report.model.Orders;
import com.jimi.mes_report.model.WorkstationProduction;
import com.jimi.mes_report.model.WorkstationsOrders;
import com.jimi.mes_report.model.WorkstationsUseTime;
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
		/*calendar.set(2019, 3, 1, 13, 13, 13);*/
		String startTime = dateFormat.format(calendar.getTime()) + " 00:00:00";
		String endTime = dateFormat.format(calendar.getTime()) + " 23:59:59";
		SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		// 设定当天记录的时间
		Date recordDate = new Date();
		try {
			recordDate = format.parse(dateFormat.format(calendar.getTime()) + " 23:00:00");
		} catch (ParseException e) {
			ErrorLogWritter.save(e.getClass().getSimpleName() + e.getMessage());
			e.printStackTrace();
		}
		// 格式化需要使用到的打印日期和IMEI日期
		SimpleDateFormat printDateFormat = new SimpleDateFormat("yyyy.MM.dd");
		SimpleDateFormat IMEIDateFormat = new SimpleDateFormat("yyyy/M/d");
		String printTime = printDateFormat.format(recordDate);
		String IMEITime = IMEIDateFormat.format(recordDate);

		// 保存每日产能和日报数据
		List<Record> orderRecords = Db.use("db1").find(SQL.SELECT_ORDER_BY_TIME, startTime, endTime);
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

		/*// 保存每日订单各个工位所用时间
		String printStartTime = printDateFormat.format(calendar.getTime()) + " 00:00:00";
		String printEndTime = printDateFormat.format(recordDate) + " 23:59:59";
		String IMEIStartTime = IMEIDateFormat.format(recordDate) + " 00:00:00";
		String IMEIEndTime = IMEIDateFormat.format(recordDate) + " 23:59:59";
		// 查询工位的所有订单
		Object[] params = { IMEIStartTime, IMEIEndTime, startTime, endTime, startTime, endTime, startTime, endTime, startTime, endTime, startTime, endTime, printStartTime, printEndTime, printStartTime, printEndTime };
		List<Record> dailyOrders = Db.use("db1").find(SQL.SELECT_DAILY_ORDERS, params);
		if (dailyOrders != null && !dailyOrders.isEmpty()) {
			for (Record dailyOrder : dailyOrders) {
				saveOrders(dailyOrder, startTime, endTime, IMEIStartTime, IMEIEndTime, printStartTime, printEndTime, recordDate);
			}
		}*/
	}


	/**@author HCJ
	 * 保存订单信息
	 * @param dailyOrder 每日的订单信息
	 * @param startTime 开始时间
	 * @param endTime 结束时间
	 * @param IMEIStartTime IMEI开始时间
	 * @param IMEIEndTime IMEI结束时间
	 * @param printStartTime 打印开始时间
	 * @param printEndTime 打印结束时间
	 * @param recordDate 记录的时间
	 * @date 2020年4月3日 下午2:19:02
	 */
	private void saveOrders(Record dailyOrder, String startTime, String endTime, String IMEIStartTime, String IMEIEndTime, String printStartTime, String printEndTime, Date recordDate) {
		String zhiDan = dailyOrder.getStr("ZhiDan");
		String version = dailyOrder.getStr("Version");
		String softModel = dailyOrder.getStr("SoftModel");
		// 查询当前订单在各个工位所花时间
		Object[] params = { startTime, endTime, zhiDan, version, softModel, startTime, endTime, zhiDan, version, softModel, startTime, endTime, zhiDan, version, softModel, startTime, endTime, zhiDan, version, softModel, printStartTime, printEndTime, zhiDan, version, softModel, printStartTime, printEndTime, zhiDan, version, softModel, IMEIStartTime, IMEIEndTime, zhiDan, startTime, endTime, zhiDan, version, softModel };
		List<Record> orderWorkstationsUseTimes = Db.use("db1").find(SQL.SELECT_COST_TIME_BY_ORDERS, params);
		// 查询是否已存在当前订单
		WorkstationsOrders workstationsOrders = WorkstationsOrders.dao.findFirst(SQL.SELECT_WORKSTATIONSORDERS, zhiDan, softModel, version);
		if (workstationsOrders == null || workstationsOrders.getId() == null) {
			saveWorkstationsOrdersAndUseTime(dailyOrder, orderWorkstationsUseTimes, recordDate, workstationsOrders, false);
		} else {
			saveWorkstationsOrdersAndUseTime(dailyOrder, orderWorkstationsUseTimes, recordDate, workstationsOrders, true);
		}
	}


	/**@author HCJ
	 * 保存和更新订单信息和各个工位的时长信息
	 * @param dailyOrder 当日订单的信息
	 * @param orderWorkstationsUseTimes 当日订单对应的各个工位所用的时长信息和产量
	 * @param recordDate 记录的时间
	 * @param workstationsOrders 已存在的订单
	 * @param isOrderExist 订单是否已存在
	 * @date 2020年4月3日 下午2:20:02
	 */
	private void saveWorkstationsOrdersAndUseTime(Record dailyOrder, List<Record> orderWorkstationsUseTimes, Date recordDate, WorkstationsOrders workstationsOrders, boolean isOrderExist) {
		// 获取最大时间和最小时间得到当天订单已用时间
		Date minDate = null;
		Date maxDate = null;
		for (Record orderWorkstationsUseTime : orderWorkstationsUseTimes) {
			Integer production = orderWorkstationsUseTime.getInt("production");
			if (production == 0) {
				continue;
			}
			Date startTime = orderWorkstationsUseTime.getDate("startTime");
			Date endTime = orderWorkstationsUseTime.getDate("endTime");
			if (minDate == null) {
				minDate = startTime;
			}
			if (maxDate == null) {
				maxDate = endTime;
			}
			if (minDate.after(startTime)) {
				minDate = startTime;
			}
			if (maxDate.before(endTime)) {
				maxDate = endTime;
			}
		}
		// 是否存在计划产量
		boolean isPlanProductExist = false;
		int planProduct = 0;
		if (!StrKit.isBlank(dailyOrder.getStr("IMEIStart")) && !StrKit.isBlank(dailyOrder.getStr("IMEIEnd"))) {
			try {
				long IMEIStart = Long.parseLong(dailyOrder.getStr("IMEIStart"));
				long IMEIEnd = Long.parseLong(dailyOrder.getStr("IMEIEnd"));
				planProduct = (int) (IMEIEnd - IMEIStart + 1);
				isPlanProductExist = true;
			} catch (Exception e) {
				ErrorLogWritter.save("格式化IMEI参数出错" + e.getClass().getSimpleName() + ":" + e.getMessage());
				e.printStackTrace();
			}
		}
		// 是否存在订单
		if (!isOrderExist) {
			// 保存订单信息
			WorkstationsOrders newWorkstationsOrders = new WorkstationsOrders();
			newWorkstationsOrders.setOrderNumber(dailyOrder.getStr("ZhiDan")).setProductionNumber(dailyOrder.getStr("ProductNo"));
			newWorkstationsOrders.setSoftModel(dailyOrder.getStr("SoftModel")).setSoftVersion(dailyOrder.getStr("Version"));
			// 设置间隔时长四舍五入保留三位小数
			newWorkstationsOrders.setTotalUseTime(BigDecimal.valueOf(((double) (maxDate.getTime() - minDate.getTime())) / 3600 / 1000).setScale(3, BigDecimal.ROUND_HALF_UP).toString());
			newWorkstationsOrders.setTime(recordDate);
			// 设置订单状态
			if (isPlanProductExist) {
				// 卡通箱数量与计划数量比较
				if (orderWorkstationsUseTimes.get(orderWorkstationsUseTimes.size() - 1).getInt("production") >= planProduct) {
					newWorkstationsOrders.setStatus("已完成");
				} else {
					newWorkstationsOrders.setStatus("进行中");
				}
			} else {
				newWorkstationsOrders.setStatus("进行中");
			}
			newWorkstationsOrders.save();
			// 保存订单对应工位所用时间
			for (int i = 0; i < orderWorkstationsUseTimes.size(); i++) {
				WorkstationsUseTime useTime = new WorkstationsUseTime();
				Record orderWorkstationsUseTime = orderWorkstationsUseTimes.get(i);
				Integer production = orderWorkstationsUseTime.getInt("production");
				// 保存生产时长和每台生产时长以及产量
				if (production == 0) {
					useTime.setActualUseTime("0").setAverageUseTime("0").setTotalProduct(0);
				} else {
					Date startTime = orderWorkstationsUseTime.getDate("startTime");
					Date endTime = orderWorkstationsUseTime.getDate("endTime");
					BigDecimal actualUseTime = BigDecimal.valueOf(((double) (endTime.getTime() - startTime.getTime())) / 3600 / 1000).setScale(3, BigDecimal.ROUND_HALF_UP);
					// 如果实际生产小时小于此数字则设定默认值
					if (actualUseTime.doubleValue() < 0.001) {
						actualUseTime = BigDecimal.valueOf(0.001);
					}
					useTime.setActualUseTime(actualUseTime.toString());
					double actualUseMinute = actualUseTime.doubleValue() * Constant.HOUR_TO_MINUTE;
					// 如果实际生产分钟小于一分钟则设定为一分钟
					if (actualUseMinute < 1) {
						actualUseMinute = 1;
					}
					useTime.setAverageUseTime(String.valueOf(production / (int) actualUseMinute));
					useTime.setTotalProduct(production);
				}
				// 根据机型版本工位获取最小平均时间
				String minAverageUseTime = null;
				Record record = Db.findFirst(SQL.SELECT_MIN_AVERAGE_USE_TIME, dailyOrder.getStr("SoftModel"), dailyOrder.getStr("Version"), i + 1);
				if (record == null || StrKit.isBlank(record.getStr("minAverageUseTime"))) {
					minAverageUseTime = "0";
				} else {
					minAverageUseTime = record.getStr("minAverageUseTime");
				}
				useTime.setWorkstationsOrdersId(newWorkstationsOrders.getId()).setType(i + 1).setMinUsedTime(minAverageUseTime).save();
			}
		} else {
			// 更新订单信息
			// 获取当前订单已存在的工位时长信息
			List<WorkstationsUseTime> workstationsUseTimes = WorkstationsUseTime.dao.find(SQL.SELECT_WORKSTATIONS_USE_TIMES_BY_ORDER, workstationsOrders.getId());
			// 更新订单总时长
			BigDecimal newTotalUseTime = BigDecimal.valueOf(((double) (maxDate.getTime() - minDate.getTime())) / 3600 / 1000).setScale(3, BigDecimal.ROUND_HALF_UP);
			workstationsOrders.setTotalUseTime(String.valueOf(Double.parseDouble(workstationsOrders.getTotalUseTime()) + newTotalUseTime.doubleValue()));
			workstationsOrders.setTime(recordDate);
			// 更新订单状态
			if (isPlanProductExist) {
				// 获取订单已生产的卡通箱生产数量
				Integer originalProduction = 0;
				for (WorkstationsUseTime workstationsUseTime : workstationsUseTimes) {
					if (Constant.CARTON_TEST_TYPE.equals(workstationsUseTime.getType())) {
						originalProduction = workstationsUseTime.getTotalProduct();
					}
				}
				// 卡通箱数量与计划数量比较
				if (orderWorkstationsUseTimes.get(orderWorkstationsUseTimes.size() - 1).getInt("production") + originalProduction >= planProduct) {
					workstationsOrders.setStatus("已完成");
				}
			}
			workstationsOrders.update();
			// 更新订单的工位时长信息
			for (int i = 0; i < orderWorkstationsUseTimes.size(); i++) {
				Record orderWorkstationsUseTime = orderWorkstationsUseTimes.get(i);
				for (WorkstationsUseTime workstationsUseTime : workstationsUseTimes) {
					if (i + 1 == workstationsUseTime.getType()) {
						Integer newProduction = orderWorkstationsUseTime.getInt("production");
						// 更新生产时长和每台生产时长以及产量
						if (newProduction > 0) {
							Date startTime = orderWorkstationsUseTime.getDate("startTime");
							Date endTime = orderWorkstationsUseTime.getDate("endTime");
							BigDecimal newUseTime = BigDecimal.valueOf(((double) (endTime.getTime() - startTime.getTime())) / 3600 / 1000).setScale(3, BigDecimal.ROUND_HALF_UP);
							double actualUseTime = Double.parseDouble(workstationsUseTime.getActualUseTime()) + newUseTime.doubleValue();
							workstationsUseTime.setActualUseTime(String.valueOf(actualUseTime));
							workstationsUseTime.setTotalProduct(newProduction + workstationsUseTime.getTotalProduct());
							workstationsUseTime.setAverageUseTime(BigDecimal.valueOf(workstationsUseTime.getTotalProduct() / actualUseTime).setScale(3, BigDecimal.ROUND_HALF_UP).toString());
							// 根据机型版本工位获取最小平均时间
							String minAverageUseTime = null;
							Record record = Db.findFirst(SQL.SELECT_MIN_AVERAGE_USE_TIME, dailyOrder.getStr("SoftModel"), dailyOrder.getStr("Version"), workstationsUseTime.getType());
							if (record == null || StrKit.isBlank(record.getStr("minAverageUseTime"))) {
								minAverageUseTime = "0";
							} else {
								minAverageUseTime = record.getStr("minAverageUseTime");
							}
							workstationsUseTime.setMinUsedTime(minAverageUseTime).update();
						}
					}
				}
			}
		}
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

		Integer IMEITestProduction = Db.use("db1").queryInt(SQL.SELECT_IMEINUM_BY_TESTTIME, IMEITime + " %", Constant.SUCCESS_CPRESULT);
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
