package com.jimi.mes_report.service;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

import com.jfinal.kit.PropKit;
import com.jfinal.kit.StrKit;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Record;
import com.jfinal.plugin.redis.Cache;
import com.jfinal.plugin.redis.Redis;
import com.jfplugin.mail.MailKit;
import com.jimi.mes_report.constant.Constant;
import com.jimi.mes_report.model.entity.DailyProductionReport;
import com.jimi.mes_report.model.entity.DailyProductionReportItem;
import com.jimi.mes_report.model.entity.HourStatisticsItem;
import com.jimi.mes_report.util.CommonUtil;
import com.jimi.mes_report.util.DateFormat;
import com.jimi.mes_report.util.ErrorLogWritter;

/**邮件服务类
 * @author   HCJ
 * @date     2019年4月29日 上午11:10:51
 */
public class EmailDocService {

	// 空数组
	public final static Document[] EMPTY_DOCUMENT_ARRAY = new Document[0];


	/**@author HCJ
	 * 根据数据生成产能和日报表的Document集合
	 * @param orderNumRecords 工单号集合
	 * @param orderCapacityRecords 工单、产能集合
	 * @param orderProductionRecord 所有订单的产量
	 * @param stationProductionRecords 各个工位的产量
	 * @param historyMaxProduct 最大的订单产量
	 * @param maxStationProductions 根据工位获取每天最大的产量
	 * @param timeType 时间类型：天、周、月
	 * @param startDate 起始时间
	 * @param endDate 结束时间
	 * @return Document[] 存储html的Document集合
	 * @date 2019年4月29日 上午11:11:07
	 */
	public Document[] getPaperCapacityDocument(List<Record> orderNumRecords, List<Record> orderCapacityRecords, Record orderProductionRecord, List<Record> stationProductionRecords, Integer historyMaxProduct, List<Record> maxStationProductions, Integer timeType, Date startDate, Date endDate) {
		Document paperDoc = null;
		Document capacityDoc = null;
		try {
			File paperTemplate = new File(EmailDocService.class.getClassLoader().getResource("JIMI-dailyPaper.html").toURI());
			File capacityTemplate = new File(EmailDocService.class.getClassLoader().getResource("JIMI-dailyCapacity.html").toURI());
			paperDoc = Jsoup.parse(paperTemplate, "UTF-8");
			capacityDoc = Jsoup.parse(capacityTemplate, "UTF-8");
		} catch (Exception e) {
			ErrorLogWritter.save(e.getClass().getSimpleName() + ":" + e.getMessage());
		}
		// 根据订单号保存集合
		List<List<Record>> orderCapacityLists = new ArrayList<>();
		for (Record orderNumRecord : orderNumRecords) {
			List<Record> orderCapacityList = new ArrayList<>();
			for (Record orderCapacityRecord : orderCapacityRecords) {
				if (orderCapacityRecord.getStr("order_number").equals(orderNumRecord.getStr("order_number"))) {
					orderCapacityList.add(orderCapacityRecord);
				}
			}
			orderCapacityLists.add(orderCapacityList);
		}
		// 根据时间类型和间隔修改显示的标题和时间
		switch (timeType) {
		case 1:
			SimpleDateFormat dayFormat = new SimpleDateFormat("yyyy年MM月dd日");
			paperDoc.getElementById("time").text(dayFormat.format(startDate));
			capacityDoc.getElementById("time").text(dayFormat.format(startDate));
			paperDoc.getElementById("title").text("几米智造已配置订单日报表");
			paperDoc.getElementById("dailyPlanName").text("日计划量");
			paperDoc.getElementById("dailyActualName").text("日完成量");
			capacityDoc.getElementById("title").text("几米智造日产能报表");
			break;
		case 2:
			SimpleDateFormat prefixFormat = new SimpleDateFormat("yyyy年MM月dd日 至 ");
			SimpleDateFormat suffixFormat = new SimpleDateFormat("dd日");
			String timeString = prefixFormat.format(startDate) + suffixFormat.format(endDate);
			paperDoc.getElementById("time").text(timeString);
			capacityDoc.getElementById("time").text(timeString);
			paperDoc.getElementById("title").text("几米智造已配置订单周报表");
			paperDoc.getElementById("dailyPlanName").text("周计划量");
			paperDoc.getElementById("dailyActualName").text("周完成量");
			capacityDoc.getElementById("title").text("几米智造周产能报表");
			break;
		case 3:
			SimpleDateFormat monthFormat = new SimpleDateFormat("yyyy年MM月");
			paperDoc.getElementById("time").text(monthFormat.format(startDate));
			capacityDoc.getElementById("time").text(monthFormat.format(startDate));
			paperDoc.getElementById("title").text("几米智造已配置订单月报表");
			paperDoc.getElementById("dailyPlanName").text("月计划量");
			paperDoc.getElementById("dailyActualName").text("月完成量");
			capacityDoc.getElementById("title").text("几米智造月产能报表");
			break;
		default:
			break;
		}
		// 根据list大小创建table
		StringBuffer orderBuffer = new StringBuffer();
		StringBuffer stationBuffer = new StringBuffer();
		String order = paperDoc.select("table#order").first().toString();
		String station = capacityDoc.select("table#station").first().toString();
		int size = orderCapacityLists.size();
		if (size > 1) {
			for (int i = 0; i < size - 1; i++) {
				orderBuffer.append(order);
				stationBuffer.append(station);
			}
		}
		paperDoc.select("body").append(orderBuffer.toString());
		capacityDoc.select("body").append(stationBuffer.toString());
		for (int i = 0; i < orderCapacityLists.size(); i++) {
			List<Record> orderCapacityList = orderCapacityLists.get(i);
			Element ordersTable = paperDoc.select("table#order").get(i);
			Element stationsTable = capacityDoc.select("table#station").get(i);
			for (Record orderCapacityRecord : orderCapacityList) {
				ordersTable.select("#orderNumber").first().text(orderCapacityRecord.get("order_number"));
				ordersTable.select("#softModel").first().text(orderCapacityRecord.get("soft_model"));
				ordersTable.select("#productionNumber").first().text("产品编号：" + orderCapacityRecord.get("production_number"));
				ordersTable.select("#softVersion").first().text("软件版本：" + orderCapacityRecord.get("soft_version"));
				ordersTable.select("#planProduct").first().text("订单数量：" + orderCapacityRecord.get("plan_product"));

				stationsTable.select("#orderNumber").first().text(orderCapacityRecord.get("order_number"));
				stationsTable.select("#planProduct").first().text("计划量：" + orderCapacityRecord.get("plan_product"));
				String hourlyPlanProduct = orderCapacityRecord.get("hourly_plan_product").toString();
				stationsTable.select("#SMTTestPlan").first().text(hourlyPlanProduct);
				stationsTable.select("#functionTestPlan").first().text(hourlyPlanProduct);
				stationsTable.select("#agingTestPlan").first().text(hourlyPlanProduct);
				stationsTable.select("#couplingTestPlan").first().text(hourlyPlanProduct);
				stationsTable.select("#JSTestPlan").first().text(hourlyPlanProduct);
				stationsTable.select("#CHTestPlan").first().text(hourlyPlanProduct);
				stationsTable.select("#IMEIContrastPlan").first().text(hourlyPlanProduct);
				stationsTable.select("#cartonBoxTestPlan").first().text(hourlyPlanProduct);

				String orderProduction = orderCapacityRecord.get("stationProduct").toString();
				String hourlyOrderProduction = orderCapacityRecord.get("hourly_actual_product").toString();
				switch (orderCapacityRecord.getInt("type")) {
				case 1:
					ordersTable.select("#SMTTest").first().text(orderProduction);
					stationsTable.select("#SMTTestActual").first().text(hourlyOrderProduction);
					break;
				case 2:
					ordersTable.select("#functionTest").first().text(orderProduction);
					stationsTable.select("#functionTestActual").first().text(hourlyOrderProduction);
					break;
				case 3:
					ordersTable.select("#agingTest").first().text(orderProduction);
					stationsTable.select("#agingTestActual").first().text(hourlyOrderProduction);
					break;
				case 4:
					ordersTable.select("#couplingTest").first().text(orderProduction);
					stationsTable.select("#couplingTestActual").first().text(hourlyOrderProduction);
					break;
				case 5:
					ordersTable.select("#JSTest").first().text(orderProduction);
					stationsTable.select("#JSTestActual").first().text(hourlyOrderProduction);
					break;
				case 6:
					ordersTable.select("#CHTest").first().text(orderProduction);
					stationsTable.select("#CHTestActual").first().text(hourlyOrderProduction);
					break;
				case 7:
					ordersTable.select("#IMEIContrast").first().text(orderProduction);
					stationsTable.select("#IMEIContrastActual").first().text(hourlyOrderProduction);
					break;
				case 8:
					ordersTable.select("#cartonBoxTest").first().text(orderProduction);
					ordersTable.select("#actualProduct").first().text("实际完成：" + orderProduction);
					stationsTable.select("#actualProduct").first().text("完成量：" + orderProduction);
					stationsTable.select("#cartonBoxTestActual").first().text(hourlyOrderProduction);
					break;
				default:
					break;
				}
			}
		}
		Element stationTable = paperDoc.select("table#station").first();
		Integer cartonProduct = 0;
		for (Record stationProductionRecord : stationProductionRecords) {
			String stationProduction = stationProductionRecord.getInt("product").toString();
			switch (stationProductionRecord.getInt("type")) {
			case 1:
				stationTable.select("#SMTTestActualTotal").first().text(stationProduction);
				break;
			case 2:
				stationTable.select("#functionTestActualTotal").first().text(stationProduction);
				break;
			case 3:
				stationTable.select("#agingTestActualTotal").first().text(stationProduction);
				break;
			case 4:
				stationTable.select("#couplingTestActualTotal").first().text(stationProduction);
				break;
			case 5:
				stationTable.select("#JSTestActualTotal").first().text(stationProduction);
				break;
			case 6:
				stationTable.select("#CHTestActualTotal").first().text(stationProduction);
				break;
			case 7:
				stationTable.select("#IMEIContrastActualTotal").first().text(stationProduction);
				break;
			case 8:
				stationTable.select("#cartonBoxTestActualTotal").first().text(stationProduction);
				cartonProduct = stationProductionRecord.getInt("product");
				break;
			default:
				break;
			}
		}

		Element productionTable = paperDoc.select("table#production").first();

		for (Record maxStationProduction : maxStationProductions) {
			String maxProduction = maxStationProduction.get("maxProduct").toString();
			switch (maxStationProduction.getInt("type")) {
			case 1:
				stationTable.select("#SMTTestMaximumTotal").first().text(maxProduction);
				break;
			case 2:
				stationTable.select("#functionTestMaximumTotal").first().text(maxProduction);
				break;
			case 3:
				stationTable.select("#agingTestMaximumTotal").first().text(maxProduction);
				break;
			case 4:
				stationTable.select("#couplingTestMaximumTotal").first().text(maxProduction);
				break;
			case 5:
				stationTable.select("#JSTestMaximumTotal").first().text(maxProduction);
				break;
			case 6:
				stationTable.select("#CHTestMaximumTotal").first().text(maxProduction);
				break;
			case 7:
				stationTable.select("#IMEIContrastMaximumTotal").first().text(maxProduction);
				break;
			case 8:
				stationTable.select("#cartonBoxTestMaximumTotal").first().text(maxProduction);
				productionTable.select("#maximumProduct").first().text(maxProduction);
				break;
			default:
				break;
			}
		}

		productionTable.select("#dailyPlanProduct").first().text(orderProductionRecord.get("plan_product").toString());
		productionTable.select("#dailyActualProduct").first().text(cartonProduct.toString());

		Element hourlyProductionTable = capacityDoc.select("table#hourlyProduction").first();
		switch (timeType) {
		case 1:
			hourlyProductionTable.select("#hourlyMaximumProduct").first().text(String.valueOf(historyMaxProduct / Constant.DAILY_WORKING_HOURS));
			hourlyProductionTable.select("#hourlyPlanProduct").first().text(String.valueOf(orderProductionRecord.getInt("plan_product") / Constant.DAILY_WORKING_HOURS));
			hourlyProductionTable.select("#hourlyActualProduct").first().text(String.valueOf(cartonProduct / Constant.DAILY_WORKING_HOURS));
			break;
		case 2:
			hourlyProductionTable.select("#hourlyMaximumProduct").first().text(String.valueOf(historyMaxProduct / Constant.DAILY_WORKING_HOURS / Constant.WEEKLY_DAYS));
			hourlyProductionTable.select("#hourlyPlanProduct").first().text(String.valueOf(orderProductionRecord.getInt("plan_product") / Constant.DAILY_WORKING_HOURS / Constant.WEEKLY_DAYS));
			hourlyProductionTable.select("#hourlyActualProduct").first().text(String.valueOf(cartonProduct / Constant.DAILY_WORKING_HOURS / Constant.WEEKLY_DAYS));
			break;
		case 3:
			// 获取前一个月的天数
			Calendar calendar = Calendar.getInstance();
			calendar.setTime(startDate);
			int monthDays = calendar.getActualMaximum(Calendar.DATE);
			hourlyProductionTable.select("#hourlyMaximumProduct").first().text(String.valueOf(historyMaxProduct / Constant.DAILY_WORKING_HOURS / monthDays));
			hourlyProductionTable.select("#hourlyPlanProduct").first().text(String.valueOf(orderProductionRecord.getInt("plan_product") / Constant.DAILY_WORKING_HOURS / monthDays));
			hourlyProductionTable.select("#hourlyActualProduct").first().text(String.valueOf(cartonProduct / Constant.DAILY_WORKING_HOURS / monthDays));
			break;
		default:
			break;
		}
		Document[] documents = new Document[] { paperDoc/*, capacityDoc*/ };
		return documents;
	}


	/**@author HCJ
	 * 根据数据生成订单已用时间的Document集合
	 * @param ordersRecords 订单信息和各个工位花费时长集合
	 * @date 2020年4月3日 下午4:07:37
	 */
	public Document[] getUsedTimeDocument(List<Record> ordersRecords) {
		Document usedTimeDoc = null;
		try {
			File usedTimeTemplate = new File(EmailDocService.class.getClassLoader().getResource("JIMI-dailyUsedTime.html").toURI());
			usedTimeDoc = Jsoup.parse(usedTimeTemplate, "UTF-8");
		} catch (Exception e) {
			ErrorLogWritter.save(e.getClass().getSimpleName() + ":" + e.getMessage());
		}
		// 根据数据信息进行归类
		Map<String, List<Record>> map = new HashMap<>();
		for (Record record : ordersRecords) {
			String key = record.getStr("order_number") + record.getStr("soft_model") + record.getStr("soft_version") + record.getStr("production_number");
			if (map.containsKey(key)) {
				map.get(key).add(record);
			} else {
				List<Record> records = new ArrayList<>();
				records.add(record);
				map.put(key, records);
			}
		}
		// 根据list大小创建table
		StringBuffer usedTimeBuffer = new StringBuffer();
		String usedTimeTable = usedTimeDoc.select("table#report-table").first().toString();
		int size = map.size();
		if (size > 1) {
			for (int i = 0; i < size - 1; i++) {
				usedTimeBuffer.append(usedTimeTable);
			}
		}
		usedTimeDoc.select("div").append(usedTimeBuffer.toString());
		// 填充数据
		int index = 0;
		for (List<Record> usedTimes : map.values()) {
			Element usedTimesTable = usedTimeDoc.select("table#report-table").get(index);
			for (Record record : usedTimes) {
				switch (record.getInt("type")) {
				case 1:
					usedTimesTable.select("#reoprt-smt-used-time").first().text(record.get("actual_use_time"));
					usedTimesTable.select("#reoprt-smt-avg-time").first().text(record.get("average_use_time"));
					usedTimesTable.select("#reoprt-smt-min-time").first().text(record.get("min_used_time"));
					break;
				case 2:
					usedTimesTable.select("#report-zz-used-time").first().text(record.get("actual_use_time"));
					usedTimesTable.select("#report-zz-avg-time").first().text(record.get("average_use_time"));
					usedTimesTable.select("#report-zz-min-time").first().text(record.get("min_used_time"));
					break;
				case 3:
					usedTimesTable.select("#reoprt-lh-used-time").first().text(record.get("actual_use_time"));
					usedTimesTable.select("#reoprt-lh-avg-time").first().text(record.get("average_use_time"));
					usedTimesTable.select("#reoprt-lh-min-time").first().text(record.get("min_used_time"));
					break;
				case 4:
					usedTimesTable.select("#report-oh-used-time").first().text(record.get("actual_use_time"));
					usedTimesTable.select("#report-oh-avg-time").first().text(record.get("average_use_time"));
					usedTimesTable.select("#report-oh-min-time").first().text(record.get("min_used_time"));
					break;
				case 5:
					usedTimesTable.select("#report-jst-used-time").first().text(record.get("actual_use_time"));
					usedTimesTable.select("#report-jst-avg-time").first().text(record.get("average_use_time"));
					usedTimesTable.select("#reoprt-jst-min-time").first().text(record.get("min_used_time"));
					break;
				case 6:
					usedTimesTable.select("#reoprt-cht-used-time").first().text(record.get("actual_use_time"));
					usedTimesTable.select("#reoprt-cht-avg-time").first().text(record.get("average_use_time"));
					usedTimesTable.select("#reoprt-cht-min-time").first().text(record.get("min_used_time"));
					break;
				case 7:
					usedTimesTable.select("#reoprt-imei-used-time").first().text(record.get("actual_use_time"));
					usedTimesTable.select("#reoprt-imei-avg-time").first().text(record.get("average_use_time"));
					usedTimesTable.select("#reoprt-imei-min-time").first().text(record.get("min_used_time"));
					break;
				case 8:
					usedTimesTable.select("#reoprt-carton-used-time").first().text(record.get("actual_use_time"));
					usedTimesTable.select("#reoprt-carton-avg-time").first().text(record.get("average_use_time"));
					usedTimesTable.select("#reoprt-carton-min-time").first().text(record.get("min_used_time"));
					break;
				default:
					break;
				}
			}
			usedTimesTable.select("#report-order").first().text(usedTimes.get(5).get("order_number"));
			usedTimesTable.select("#report-machine").first().text(usedTimes.get(5).get("soft_model"));
			usedTimesTable.select("#report-product").first().text(usedTimes.get(5).get("production_number"));
			usedTimesTable.select("#report-version").first().text(usedTimes.get(5).get("soft_version"));
			usedTimesTable.select("#report-used-time").first().text(usedTimes.get(5).get("total_use_time"));
			usedTimesTable.select("#report-status").first().text(usedTimes.get(5).get("status"));
			index++;
		}
		Document[] documents = new Document[] { usedTimeDoc };
		return documents;
	}


	public Document[] getDailyProductionReportDocument(Date date) {
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
		Document dailyProductionDoc = null;
		try {
			File dailyProductionTemplate = new File(EmailDocService.class.getClassLoader().getResource("JIMI-dailyProduction.html").toURI());
			dailyProductionDoc = Jsoup.parse(dailyProductionTemplate, "UTF-8");
		} catch (Exception e) {
			ErrorLogWritter.save(e.getClass().getSimpleName() + ":" + e.getMessage());
		}
		String dateString = dateFormat.format(date);
		// 根据时间获取每日报表信息dailyProductionReport
		DailyProductionReport dailyProductionReport = DailyReportItemService.getDailyProductionReport(dateString);
		DateFormat dateFomatUtil = new DateFormat(dateString);
		int output = dailyProductionReport.getOutput();
		if (output <= 0) {
			return EMPTY_DOCUMENT_ARRAY;
		}
		// 填充时间范围和日产量以及历史情况
		dailyProductionDoc.getElementById("time-range").text(dateString + " 00:00:00 ~ " + dateString + " 23:59:59");
		dailyProductionDoc.getElementById("produce-time-current").text(CommonUtil.secondToTime(dailyProductionReport.getConsumingTime()));

		List<DailyProductionReportItem> dailyProductionList = dailyProductionReport.getItems();
		// 根据集合大小拼接表格
		int size = dailyProductionList.size();
		StringBuffer summarySb = new StringBuffer();
		StringBuffer detailsSb = new StringBuffer();
		String summaryTableString = dailyProductionDoc.select("tr#capacity-total-content").first().toString();
		String detailsTableString = dailyProductionDoc.select("div#detailDiv").first().toString();
		if (size > 1) {
			for (int i = 0; i < size - 1; i++) {
				summarySb.append(summaryTableString);
				detailsSb.append(detailsTableString);
			}
		}
		dailyProductionDoc.select("table#summary").append(summarySb.toString());
		dailyProductionDoc.select("div#detailDiv").append(detailsSb.toString());
		// 填充数据
		for (int i = 0; i < dailyProductionList.size(); i++) {
			DailyProductionReportItem item = dailyProductionList.get(i);
			Element summaryTable = dailyProductionDoc.select("tr#capacity-total-content").get(i);
			Element detailsTable = dailyProductionDoc.select("div#detailDiv").get(i);
			// 填充汇总表格
			summaryTable.select("#order").first().text(item.getOrderNo());
			if (!StrKit.isBlank(item.getModel())) {
				summaryTable.select("#model").first().text(item.getModel());
			} else {
				summaryTable.select("#model").first().text("-");
			}
			summaryTable.select("#number").first().text(String.valueOf(item.getPlanProductionQuantity()));
			if (item.getStartDate() != null) {
				summaryTable.select("#startDate").first().text(dateFormat.format(item.getStartDate()));
			} else {
				summaryTable.select("#startDate").first().text("-");
			}
			if (item.getPlanDate() != null) {
				summaryTable.select("#planDate").first().text(dateFormat.format(item.getPlanDate()));
			} else {
				summaryTable.select("#planDate").first().text("-");
			}
			summaryTable.select("#production").first().text(String.valueOf(item.getProductionQuantity()));
			summaryTable.select("#completeRate").first().text(item.getCompletionRate() + "%");
			summaryTable.select("#dailyProduction").first().text(String.valueOf(item.getDailyProductionQuantity()));
			// 填充详情表格表头
			detailsTable.select("#capacity-order").first().text(item.getOrderNo());
			if (!StrKit.isBlank(item.getModel())) {
				detailsTable.select("#capacity-machine-name").first().text(item.getModel());
			} else {
				detailsTable.select("#capacity-machine-name").first().text("-");
			}
			if (!StrKit.isBlank(item.getProductNo())) {
				detailsTable.select("#capacity-product-number").first().text(item.getProductNo());
			} else {
				detailsTable.select("#capacity-product-number").first().text("-");
			}
			if (!StrKit.isBlank(item.getVersion())) {
				detailsTable.select("#capacity-software-version").first().text(item.getVersion());
			} else {
				detailsTable.select("#capacity-software-version").first().text("-");
			}
			detailsTable.select("#capacity-order-count").first().text(String.valueOf(item.getPlanProductionQuantity()));
			detailsTable.select("#capacity-finished-count").first().text(String.valueOf(item.getProductionQuantity()));
			detailsTable.select("#capacity-finished-rate").first().text(item.getCompletionRate() + "%");
			detailsTable.select("#on-produce-time").first().text(String.valueOf(item.getProductionDays()));
			/*detailsTable.select("#actual-capacity-hour").first().text(String.valueOf(item.getCapacity()));*/
			// 填充详情表格内容
			for (int j = 0; j < 8; j++) {
				List<HourStatisticsItem> hourStatisticsItems = item.getHourStatisticsItems(j);
				int lineNum = 0;
				for (HourStatisticsItem hourStatisticsItem : hourStatisticsItems) {
					if (hourStatisticsItem.getOutput() < 1 && hourStatisticsItem.getHour() >= 0) {
						detailsTable.select("#" + j + "-time-" + hourStatisticsItem.getHour()).first().empty();
						continue;
					}
					if (hourStatisticsItem.getHour() == -1 && hourStatisticsItem.getOutput() == 0) {
						detailsTable.select("#" + j + "-name").first().empty();
						detailsTable.select("#" + j + "-summary").first().empty();
						continue;
					}
					detailsTable.select("#" + j + "-finished-count-" + hourStatisticsItem.getHour()).first().text(String.valueOf(hourStatisticsItem.getOutput()));
					int minutes = hourStatisticsItem.getConsumingTime() / 60;
					int remainingSeconds = hourStatisticsItem.getConsumingTime() % 60;
					detailsTable.select("#" + j + "-actual-used-time-" + hourStatisticsItem.getHour()).first().text(String.valueOf(minutes + "分" + remainingSeconds + "秒"));
					if (hourStatisticsItem.getHour() < 0) {
						continue;
					}
					try {
						Object[] column = dateFomatUtil.getSingleTimeColumn(j, item.getOrderNo(), hourStatisticsItem.getHour());
						List<Record> records = HourlyProductionCounter.countStation(j, column);
						lineNum += addStationDailyProductionHtml(detailsTable.select("#" + j + "-time-" + hourStatisticsItem.getHour()), j, hourStatisticsItem.getHour(), records, j);
					} catch (Exception e) {
						e.printStackTrace();
					}
					
				}
				if (detailsTable.select("#" + j + "-name").select("td.tg-uzvj").attr("rowspan").equals("")) {
					System.out.println("j:" + j);
				}
				detailsTable.select("#" + j + "-name").select("td.tg-uzvj").attr("rowspan", String.valueOf(lineNum + Integer.valueOf(detailsTable.select("#" + j + "-name").select("td.tg-uzvj").attr("rowspan").equals("") ? "25" : detailsTable.select("#" + j + "-name").select("td.tg-uzvj").attr("rowspan"))));
			}
		}
		// 去除分单的订单
		// 存储分单前的订单号
		List<String> orders = new ArrayList<>();
		// 存储分单前的订单号在表格的位置
		List<Integer> indexs = new ArrayList<>();
		Elements elements = dailyProductionDoc.select("tr#capacity-total-content");
		for (int i = 0; i < elements.size(); i++) {
			String order = elements.get(i).select("td#order").first().text();
			if (order.contains("-")) {
				order = order.substring(0, order.lastIndexOf("-"));
				// 将分单前的订单号写入表格
				elements.get(i).select("td#order").first().text(order);
			}
			if (!orders.contains(order)) {
				orders.add(order);
			} else {
				indexs.add(i);
			}
		}
		for (Integer index : indexs) {
			elements.get(index).empty();
		}
		// 填充新的日出货量
		int totalDailyProduction = 0;
		Elements newElements = dailyProductionDoc.select("tr#capacity-total-content");
		for (int i = 0; i < newElements.size(); i++) {
			String dailyProduction = null;
			try {
				dailyProduction = newElements.get(i).select("td#dailyProduction").first().text();
			} catch (Exception e) {
				continue;
			}
			if (StrKit.isBlank(dailyProduction)) {
				continue;
			}
			totalDailyProduction = totalDailyProduction + Integer.parseInt(dailyProduction);
		}
		// 更新最大日产量和花费的时间
		int dailyMaxProduction = 0;
		int dailyMaxProductionTime = 0;
		Cache cache = Redis.use();
		if (cache.exists(Constant.DAILY_MAX_CAPACITY_KEY)) {
			int dailyMaxCapacityTemp = Integer.parseInt(cache.get(Constant.DAILY_MAX_CAPACITY_KEY));
			int dailyMaxCapacityTimeTemp = Integer.parseInt(cache.get(Constant.DAILY_MAX_CAPACITY_COST_TIME_KEY));
			if (totalDailyProduction > dailyMaxCapacityTemp) {
				dailyMaxProduction = totalDailyProduction;
				dailyMaxProductionTime = dailyProductionReport.getConsumingTime();
				cache.set(Constant.DAILY_MAX_CAPACITY_KEY, dailyMaxProduction);
				cache.set(Constant.DAILY_MAX_CAPACITY_COST_TIME_KEY, dailyMaxProductionTime);
			} else {
				dailyMaxProduction = dailyMaxCapacityTemp;
				dailyMaxProductionTime = dailyMaxCapacityTimeTemp;
			}
		} else {
			dailyMaxProduction = totalDailyProduction;
			dailyMaxProductionTime = dailyProductionReport.getConsumingTime();
			cache.set(Constant.DAILY_MAX_CAPACITY_KEY, dailyMaxProduction);
			cache.set(Constant.DAILY_MAX_CAPACITY_COST_TIME_KEY, dailyMaxProductionTime);
		}
		dailyProductionDoc.getElementById("capacity-total-daily").text(String.valueOf(totalDailyProduction));
		dailyProductionDoc.getElementById("capacity-total-history-max-daily").text(String.valueOf(dailyMaxProduction));
		dailyProductionDoc.getElementById("produce-time-history").text(CommonUtil.secondToTime(dailyMaxProductionTime));
		Document[] documents = new Document[] { dailyProductionDoc };
		return documents;
	}


	/**@author HCJ
	 * 发送邮件
	 * @param documents 需要进行发送的Document[]
	 * @date 2019年4月29日 上午11:14:09
	 */
	public void sendEmail(Document[] documents) {
		String toAddress = PropKit.use("mail.properties").get("mail.to.address");
		String[] address = toAddress.split(",");
		for (String to : address) {
			for (Document document : documents) {
				try {
					MailKit.send(to, null, document.getElementById("title").text(), document.html());
				} catch (Exception e) {
					ErrorLogWritter.save(e.getClass().getSimpleName() + e.getMessage() + "用户名为 " + to);
				}
			}
		}
	}
	
	
	
	private int addStationDailyProductionHtml(Elements node, int firstIndex, int hour, List<Record> records, int type) {
		if (records == null || records.isEmpty()) {
			return 0;
		}
		StringBuilder sb = new StringBuilder();
		int x = 0;
		for (int i = 0; i < records.size(); i++) {
			int cusumeTime = 0;
			try {
				cusumeTime = DailyReportItemService.getDateDiff(records.get(i).getStr("startTime"), records.get(i).getStr("endTime"), type);
			} catch (Exception e) {
			}
			int minutes = cusumeTime / 60;
			int remainingSeconds = cusumeTime % 60;
			if (records.get(i).getInt("output") == null) {
				System.err.println(records.get(i).toJson());
				continue;
			}
			splicingStationDailyProductionHtml(sb, firstIndex, hour, i, records.get(i).getStr("computer"), records.get(i).getInt("output"), String.valueOf(minutes + "分" + remainingSeconds + "秒"));
			x++;
		}
		node.after(sb.toString());
		return x;
	}
	
	
	private void splicingStationDailyProductionHtml(StringBuilder sb, int firstIndex, int hour, Integer secondIndex, String computer, Integer quantity, String time) {
		sb.append("<tr id=\"" + firstIndex + "-time-" + hour + "-" + secondIndex + "\">");
		sb.append("<td style=\"padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:center;vertical-align:middle;\" class=\"tg-9wq8\" id=\"time-range-" + hour + "-" + secondIndex +"\">" + computer +"</td>");
		sb.append("<td style=\"padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:center;vertical-align:middle;\" class=\"tg-9wq8\" id=\"" + firstIndex + "-finished-count-" + hour + "-" + secondIndex + "\">" + quantity +"</td>");
		sb.append("<td style=\"padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:center;vertical-align:middle;\" class=\"tg-9wq8\" id=\"" + firstIndex + "-actual-used-time-" + hour + "-" + secondIndex + "\">" + time +"</td>");
		sb.append("</tr>");
	}
}
