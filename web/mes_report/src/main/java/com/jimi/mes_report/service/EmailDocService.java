package com.jimi.mes_report.service;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;

import com.jfinal.kit.PropKit;
import com.jfinal.plugin.activerecord.Record;
import com.jfplugin.mail.MailKit;
import com.jimi.mes_report.constant.Constant;
import com.jimi.mes_report.util.ErrorLogWritter;

/**邮件服务类
 * @author   HCJ
 * @date     2019年4月29日 上午11:10:51
 */
public class EmailDocService {


	/**@author HCJ
	 * 根据数据生成Document集合
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
			paperDoc.getElementById("title").text("几米智造日报表");
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
			paperDoc.getElementById("title").text("几米智造周报表");
			paperDoc.getElementById("dailyPlanName").text("周计划量");
			paperDoc.getElementById("dailyActualName").text("周完成量");
			capacityDoc.getElementById("title").text("几米智造周产能报表");
			break;
		case 3:
			SimpleDateFormat monthFormat = new SimpleDateFormat("yyyy年MM月");
			paperDoc.getElementById("time").text(monthFormat.format(startDate));
			capacityDoc.getElementById("time").text(monthFormat.format(startDate));
			paperDoc.getElementById("title").text("几米智造月报表");
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
				break;
			default:
				break;
			}
		}
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
				break;
			default:
				break;
			}
		}
		Element productionTable = paperDoc.select("table#production").first();
		productionTable.select("#dailyPlanProduct").first().text(orderProductionRecord.get("plan_product").toString());
		productionTable.select("#dailyActualProduct").first().text(orderProductionRecord.get("actual_product").toString());
		productionTable.select("#maximumProduct").first().text(historyMaxProduct.toString());

		Element hourlyProductionTable = capacityDoc.select("table#hourlyProduction").first();
		switch (timeType) {
		case 1:
			hourlyProductionTable.select("#hourlyMaximumProduct").first().text(String.valueOf(historyMaxProduct / Constant.DAILY_WORKING_HOURS));
			hourlyProductionTable.select("#hourlyPlanProduct").first().text(String.valueOf(orderProductionRecord.getInt("plan_product") / Constant.DAILY_WORKING_HOURS));
			hourlyProductionTable.select("#hourlyActualProduct").first().text(String.valueOf(orderProductionRecord.getInt("actual_product") / Constant.DAILY_WORKING_HOURS));
			break;
		case 2:
			hourlyProductionTable.select("#hourlyMaximumProduct").first().text(String.valueOf(historyMaxProduct / Constant.DAILY_WORKING_HOURS / Constant.WEEKLY_DAYS));
			hourlyProductionTable.select("#hourlyPlanProduct").first().text(String.valueOf(orderProductionRecord.getInt("plan_product") / Constant.DAILY_WORKING_HOURS / Constant.WEEKLY_DAYS));
			hourlyProductionTable.select("#hourlyActualProduct").first().text(String.valueOf(orderProductionRecord.getInt("actual_product") / Constant.DAILY_WORKING_HOURS / Constant.WEEKLY_DAYS));
			break;
		case 3:
			// 获取前一个月的天数
			Calendar calendar = Calendar.getInstance();
			calendar.setTime(startDate);
			int monthDays = calendar.getActualMaximum(Calendar.DATE);
			hourlyProductionTable.select("#hourlyMaximumProduct").first().text(String.valueOf(historyMaxProduct / Constant.DAILY_WORKING_HOURS / monthDays));
			hourlyProductionTable.select("#hourlyPlanProduct").first().text(String.valueOf(orderProductionRecord.getInt("plan_product") / Constant.DAILY_WORKING_HOURS / monthDays));
			hourlyProductionTable.select("#hourlyActualProduct").first().text(String.valueOf(orderProductionRecord.getInt("actual_product") / Constant.DAILY_WORKING_HOURS / monthDays));
			break;
		default:
			break;
		}
		Document[] documents = new Document[] { paperDoc, capacityDoc };
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
}
