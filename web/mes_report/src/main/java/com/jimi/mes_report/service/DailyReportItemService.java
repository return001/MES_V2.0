package com.jimi.mes_report.service;

import cc.darhao.dautils.api.DateUtil;

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

import org.apache.commons.lang3.math.NumberUtils;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;
import java.util.TreeSet;
import java.util.stream.Collectors;

public class DailyReportItemService {

	public static final String GET_START_DATE = "SELECT MIN(TestTime) AS startDate FROM Gps_AutoTest_Result where ZhiDan = ?";


	public static DailyProductionReport getDailyProductionReport(String day) {
		DateFormat date = new DateFormat(day);
		List<Record> tempOrders = Db.use("db1").find(DailyProductionSql.SELECT_ALL_ZHIDAN_BY_TIME, date.getTime());
		List<String> orders = new ArrayList<>();
		for (Record record : tempOrders) {
			String zhidan = record.getStr("ZhiDan");
			if (zhidan.contains("-")) {
				record.set("ZhiDan", zhidan.substring(0, zhidan.lastIndexOf("-")));
			}
		}
		for (Record record : tempOrders) {
			if (!orders.contains(record.getStr("ZhiDan"))) {
				orders.add(record.getStr("ZhiDan"));
			}
		}
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
		List<DailyProductionReportItem> itemList = new ArrayList<>();
		DailyProductionReportItem item;
		int production = 0;
		for (String zhidan : orders) {
			Record record = Db.use("db1").findFirst(DailyProductionSql.SELECT_ALL_ORDER_BY_ZHIDAN,zhidan);
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
			// 总日产量
			int totalProductionQuantity = 0;

			for (int type = 0; type < 8; type++) {
				int workTotalOutput = 0; // 各工位总完成量
				int workTotalConsumingTime = 0; // 各工位总完成时间
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
				totalProductionQuantity += workTotalOutput;
				item.addHourStatisticsItem(type, -1, workTotalOutput, workTotalConsumingTime);
			}
			item.setOrderNo(zhidan);
			item.setModel(softModel);
			item.setVersion(version);
			item.setProductNo(productNo);

			List<HourStatisticsItem> items = item.getHourStatisticsItems(WorkstationType.CARTON);
			HourStatisticsItem hourStatisticsItem = items.get(items.size() - 1);
			// 已完成数量
			int productionQuantity = Db.use("db1").queryInt(DailyProductionSql.SELECT_CARTON_NUMBER_BY_ZHIDAN, zhidan);
			production = production + hourStatisticsItem.getOutput();
			item.setProductionQuantity(productionQuantity);
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
			if (item.getCompletionRate()>100) {
				item.setPlanProductionQuantity(productionQuantity);
				item.setCompletionRate(100f);
			}
			// 每小时产能
			int totalConsumingTime = 0;
			for (HourStatisticsItem temp : items) {
				totalConsumingTime = totalConsumingTime+temp.getConsumingTime();
			}
			if (totalConsumingTime > 0) {
				int hour = totalConsumingTime % 3600;
				if (hour < 1) {
					hour = 1;
				}
				float capacity = productionQuantity / hour;
				item.setCapacity(capacity);
			}
			// 上线日期
			Record startDateRecord = Db.use("db1").findFirst(GET_START_DATE, zhidan);
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
		}
		Collections.sort(itemList, new Comparator<DailyProductionReportItem>() {

			@Override
			public int compare(DailyProductionReportItem o1, DailyProductionReportItem o2) {
				return o2.getPlanProductionQuantity() - o1.getPlanProductionQuantity();
			}
		});
		DailyProductionReport dailyProductionReport = new DailyProductionReport(production, 0, 0, 0);
		dailyProductionReport.setItems(itemList);
		return dailyProductionReport;
	}


	private static int getDateDiff(String startTime, String endTime, int type) throws Exception {
		String format = "yyyy-MM-dd HH:mm:ss";
		if (type == WorkstationType.COLOR_BOX_STICKING || type == WorkstationType.BODY_STICKING) {
			format = "yyyy.MM.dd HH:mm:ss:SSS";
		} else if (type == WorkstationType.IMEI_COMPARISON) {
			format = "yyyy/M/d HH:mm:ss";
		}
		return CommonUtil.dateDiff(startTime, endTime, format);
	}

}
