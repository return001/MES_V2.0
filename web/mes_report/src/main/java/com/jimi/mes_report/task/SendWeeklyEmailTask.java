package com.jimi.mes_report.task;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

import org.jsoup.nodes.Document;

import com.jfinal.aop.Enhancer;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Record;
import com.jimi.mes_report.constant.Constant;
import com.jimi.mes_report.constant.SQL;
import com.jimi.mes_report.model.OrderProduction;
import com.jimi.mes_report.model.WorkstationProduction;
import com.jimi.mes_report.service.EmailDocService;
import com.jimi.mes_report.util.ErrorLogWritter;

/**发送每周邮件
 * @author   HCJ
 * @date     2019年4月29日 上午11:19:58
 */
public class SendWeeklyEmailTask implements Runnable {

	private static EmailDocService emailDocService = Enhancer.enhance(EmailDocService.class);


	@Override
	public void run() {
		sendWeeklyEmail();
	}


	private void sendWeeklyEmail() {
		Calendar calendar = Calendar.getInstance();
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
		// 获取前一天时间(即周日)
		calendar.add(Calendar.DAY_OF_MONTH, -1);
		Date endDate = calendar.getTime();
		String endTime = dateFormat.format(calendar.getTime()) + " 23:59:59";
		// 设定当周记录的时间
		SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
		Date recordDate = new Date();
		try {
			recordDate = format.parse(dateFormat.format(calendar.getTime()) + " 23:00:00");
		} catch (ParseException e) {
			ErrorLogWritter.save(e.getClass().getSimpleName() + e.getMessage());
			e.printStackTrace();
		}
		// 获取前六天时间(即周一)
		calendar.add(Calendar.DAY_OF_MONTH, -6);
		Date startDate = calendar.getTime();
		String startTime = dateFormat.format(calendar.getTime()) + " 00:00:00";
		// 获取工单号集合
		List<Record> orderNumRecords = Db.find(SQL.SELECT_DISTINCT_ORDERNUM_BY_TIME, startTime, endTime);
		if (orderNumRecords != null && !orderNumRecords.isEmpty()) {
			// 获取工单和产能集合
			List<Record> orderCapacityRecords = Db.find(SQL.SELECT_ORDERS_CAPATICY_BY_TIME, startTime, endTime);
			// 获取时间范围内所有订单的产量
			Record productionRecord = Db.findFirst(SQL.SELECT_PRODUCTION_BY_TIME, Constant.TIME_TYPE_DAY, startTime, endTime);
			// 保存每周工单记录
			OrderProduction orderProduction = new OrderProduction();
			orderProduction.setTimeType(Constant.TIME_TYPE_WEEK).setPlanProduct(productionRecord.getInt("plan_product")).setActualProduct(productionRecord.getInt("actual_product"));
			orderProduction.setHourlyActualProduct(productionRecord.getInt("actual_product") / Constant.DAILY_WORKING_HOURS / Constant.WEEKLY_DAYS).setHourlyPlanProduct(productionRecord.getInt("plan_product") / Constant.DAILY_WORKING_HOURS / Constant.WEEKLY_DAYS);
			orderProduction.setTime(recordDate).save();
			// 获取当周所有订单的产量
			Record orderProductionRecord = Db.findFirst(SQL.SELECT_ORDERPRODUCTION_BY_TIME, Constant.TIME_TYPE_WEEK, startTime, endTime);
			// 获取当周所有工位的产量
			List<Record> stationProductionRecords = Db.find(SQL.SELECT_SUM_STATIONRODUCTION_BY_TIME, Constant.TIME_TYPE_DAY, startTime, endTime);
			// 保存每周工位记录
			for (Record record : stationProductionRecords) {
				WorkstationProduction workstationProduction = new WorkstationProduction();
				workstationProduction.setProduct(record.getInt("product")).setTimeType(Constant.TIME_TYPE_WEEK).setType(record.getInt("type"));
				workstationProduction.setTime(recordDate).save();
			}
			// 获取历史工单最大周产量
			Integer historyMaxProduct = Db.queryInt(SQL.SELECT_MAXRODUCTION_BY_TIMETYPE, Constant.TIME_TYPE_WEEK);
			// 获取历史工位最大周产量
			List<Record> maxStationProductions = Db.find(SQL.SELECT_MAXSTATIONRODUCTION_BY_TIMETYPE, Constant.TIME_TYPE_WEEK);
			Document[] documents = emailDocService.getPaperCapacityDocument(orderNumRecords, orderCapacityRecords, orderProductionRecord, stationProductionRecords, historyMaxProduct, maxStationProductions, Constant.TIME_TYPE_WEEK, startDate, endDate);
			emailDocService.sendEmail(documents);
		} else {
			ErrorLogWritter.save("前一周不存在配置的工单和版本");
		}
	}
}
