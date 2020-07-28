package com.jimi.mes_report.service;

import cc.darhao.dautils.api.DateUtil;

import com.alibaba.druid.sql.visitor.functions.If;
import com.jfinal.json.Jackson;
import com.jfinal.json.Json;
import com.jfinal.kit.StrKit;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Record;
import com.jimi.mes_report.constant.DailyProductionSql;
import com.jimi.mes_report.model.entity.DailyProductionReport;
import com.jimi.mes_report.model.entity.DailyProductionReportItem;
import com.jimi.mes_report.model.entity.HourStatisticsItem;
import com.jimi.mes_report.model.entity.WorkstationType;
import com.jimi.mes_report.util.CommonUtil;
import com.jimi.mes_report.util.DateFormat;
import com.jimi.mes_report.util.ErrorLogWritter;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.List;

public class DailyReportItemService {

	public static DailyProductionReport getDailyProductionReport(String day) {
		DateFormat date = new DateFormat(day);
		List<Record> orders = Db.use("db1").find(DailyProductionSql.SELECT_ALL_ZHIDAN_BY_TIME, date.getTime());
		/*List<Record> list = new ArrayList<>();
		list.add(orders.get(19));
		list.add(orders.get(22));
		list.add(orders.get(35));
		list.add(orders.get(27));
		list.add(orders.get(18));
		list.add(orders.get(23));
		list.add(orders.get(45));
		list.add(orders.get(67));
		list.add(orders.get(15));
		list.add(orders.get(34));*/
		System.out.println(orders.size());
		List<DailyProductionReportItem> itemList = new ArrayList<>();
		DailyProductionReportItem item;
		// 日可出货量
		int production = 0;
		// 日可出货量花费时间
		int dailyProductionTime = 0;
		int flag = 1;
		for (Record order : orders) {
			String zhidan = order.getStr("ZhiDan");
			Record record = Db.use("db1").findFirst(DailyProductionSql.SELECT_ALL_ORDER_BY_ZHIDAN, zhidan);
			if (record == null) {
				if (zhidan.contains("-")) {
					String tempZhiDan = zhidan.substring(0, zhidan.lastIndexOf("-"));
					record = Db.use("db1").findFirst(DailyProductionSql.SELECT_ALL_ORDER_BY_ZHIDAN, tempZhiDan);
				}
			}
			String version = null;
			String softModel = null;
			String productNo = null;
			String imeiStart = null;
			String imeiEnd = null;
			if (record != null) {
				version = record.getStr("Version");
				softModel = record.getStr("SoftModel");
				productNo = record.getStr("ProductNo");
				imeiStart = record.getStr("IMEIStart");
				imeiEnd = record.getStr("IMEIEnd");
			}
			item = new DailyProductionReportItem();

			for (int type = 0; type < 8; type++) {
				// 各工位总完成量
				int workTotalOutput = 0;
				// 各工位总完成时间
				int workTotalConsumingTime = 0;
				String[] column = date.getTimeColumn(type, zhidan);
				List<Record> records = HourlyProductionCounter.count(type, column);
				for (int i = 0; i < records.size(); i++) {
					String endTime = records.get(i).getStr("endTime");
					String startTime = records.get(i).getStr("startTime");
					int output = records.get(i).getInt("output");
					int consumingTime = 0;
					try {
						consumingTime = getDateDiff(startTime, endTime, type);
					} catch (Exception e) {
					}
					workTotalOutput += output;
					workTotalConsumingTime += consumingTime;
					item.addHourStatisticsItem(type, i, output, consumingTime);
				}
				dailyProductionTime = dailyProductionTime + workTotalConsumingTime;
				item.addHourStatisticsItem(type, -1, workTotalOutput, workTotalConsumingTime);
			}
			item.setOrderNo(zhidan);
			item.setModel(softModel);
			item.setVersion(version);
			item.setProductNo(productNo);

			List<HourStatisticsItem> items = item.getHourStatisticsItems(WorkstationType.CARTON);

			// 还原订单分单前的订单号
			String tempZhiDan = new String(zhidan);
			if (tempZhiDan.contains("-")) {
				tempZhiDan = tempZhiDan.substring(0, tempZhiDan.lastIndexOf("-"));
			}
			// 订单已完成数量
			String productionQuantitySql = DailyProductionSql.SELECT_CARTON_NUMBER_BY_ZHIDAN + "'" + tempZhiDan + "%'";
			int productionQuantity = Db.use("db1").queryInt(productionQuantitySql);
			item.setProductionQuantity(productionQuantity);
			// 订单当天已完成数量
			StringBuilder dailyProductionQuantitySb = new StringBuilder(DailyProductionSql.SELECT_CARTON_NUMBER_BY_ZHIDAN_TESTTIME);
			dailyProductionQuantitySb.append("'").append(tempZhiDan).append("%'");
			dailyProductionQuantitySb.append(" AND TestTime >= '").append(date.getTestTimeStart());
			dailyProductionQuantitySb.append("' AND TestTime <= '").append(date.getTestTimeEnd()).append("'");
			int dailyProductionQuantity = Db.use("db1").queryInt(dailyProductionQuantitySb.toString());
			item.setDailyProductionQuantity(dailyProductionQuantity);
			// 日可出货总量
			production = production + dailyProductionQuantity;
			// 订单数量
			int planProductionQuantity = 0;
			if (!StrKit.isBlank(imeiEnd) && !StrKit.isBlank(imeiStart)) {
				try {
					long IMEIStart = Long.parseLong(imeiStart);
					long IMEIEnd = Long.parseLong(imeiEnd);
					planProductionQuantity = (int) (IMEIEnd - IMEIStart + 1);
				} catch (Exception e) {
					ErrorLogWritter.save("格式化IMEI参数出错" + e.getClass().getSimpleName() + ":" + e.getMessage());
					e.printStackTrace();
				}
			}
			if (planProductionQuantity > 0) {
				item.setPlanProductionQuantity(planProductionQuantity);
				// 完成率
				float completionRate = BigDecimal.valueOf(((double) productionQuantity / (double) planProductionQuantity) * 100).setScale(2, BigDecimal.ROUND_HALF_UP).floatValue();
				item.setCompletionRate(completionRate);
			} else {
				item.setPlanProductionQuantity(0);
				item.setCompletionRate(0);
			}
			if (item.getCompletionRate() > 100) {
				item.setPlanProductionQuantity(productionQuantity);
				item.setCompletionRate(100f);
			}
			// 每小时产能
			/*int totalConsumingTime = 0;
			for (HourStatisticsItem temp : items) {
				totalConsumingTime = totalConsumingTime + temp.getConsumingTime();
			}
			if (totalConsumingTime > 0) {
				int hour = totalConsumingTime % 3600;
				if (hour < 1) {
					hour = 1;
				}
				float capacity = productionQuantity / hour;
				item.setCapacity(capacity);
			}*/
			// 上线日期
			Record startDateRecord = Db.use("db1").findFirst(DailyProductionSql.GET_START_DATE + "'" + tempZhiDan + "%'");
			String startTime = startDateRecord.getStr("startDate");
			try {
				item.setStartDate(DateUtil.yyyyMMdd(startTime));
			} catch (Exception e) {
				item.setStartDate(null);
			}
			// 在制时间
			item.setProductionDays(1);
			try {
				int productionDays = (int) CommonUtil.daysBetween(startTime, day);
				item.setProductionDays(productionDays);
			} catch (Exception e) {

			}
			itemList.add(item);
			System.out.println( "flag =" + flag);
			flag ++;
		}
		ErrorLogWritter.record(Json.getJson().toJson(itemList));
		Collections.sort(itemList, new Comparator<DailyProductionReportItem>() {

			@Override
			public int compare(DailyProductionReportItem o1, DailyProductionReportItem o2) {
				long time2 = (o2.getStartDate() == null ? 0 : o2.getStartDate().getTime());
				long time1 = (o1.getStartDate() == null ? 0 : o1.getStartDate().getTime());

				long flag =  (time2 - time1);
				if (flag == 0) {
					//起始时间相同，对比完成率
					return ((int) (o1.getCompletionRate() * 100) - (int) (o2.getCompletionRate() *100));
				}
				if (flag > 0) {
					return 1;
				}else {
					return -1;
				}
			}
		});
		ErrorLogWritter.record(Json.getJson().toJson(itemList));
		DailyProductionReport dailyProductionReport = new DailyProductionReport(production, 0, dailyProductionTime, 0);
		dailyProductionReport.setItems(itemList);
		return dailyProductionReport;
	}


	public static int getDateDiff(String startTime, String endTime, int type) throws Exception {
		String format = "yyyy-MM-dd HH:mm:ss";
		if (type == WorkstationType.COLOR_BOX_STICKING || type == WorkstationType.BODY_STICKING) {
			format = "yyyy.MM.dd HH:mm:ss:SSS";
		} else if (type == WorkstationType.IMEI_COMPARISON) {
			format = "yyyy/M/d HH:mm:ss";
		}
		return CommonUtil.dateDiff(startTime, endTime, format);
	}
}
