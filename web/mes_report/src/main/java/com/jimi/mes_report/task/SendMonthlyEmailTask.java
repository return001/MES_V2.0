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

/**发送每月邮件
 * @author   HCJ
 * @date     2019年4月29日 上午11:17:18
 */
public class SendMonthlyEmailTask implements Runnable {

	private static EmailDocService emailDocService = Enhancer.enhance(EmailDocService.class);


	@Override
	public void run() {
		sendMonthlyEmail();
	}


	private void sendMonthlyEmail() {
		Calendar calendar = Calendar.getInstance();
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
		// 获取前一个月第一天的时间
		calendar.add(Calendar.MONTH, -1);
		String startTime = dateFormat.format(calendar.getTime()) + " 00:00:00";
		Date startDate = calendar.getTime();
		// 获取前一个月的天数
		int monthDays = calendar.getActualMaximum(Calendar.DATE);
		// 获取前一个月最后一天的时间(即昨天)
		calendar.setTime(new Date());
		calendar.add(Calendar.DAY_OF_MONTH, -1);
		String endTime = dateFormat.format(calendar.getTime()) + " 23:59:59";
		// 设定当月记录的时间
		SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
		Date recordDate = new Date();
		try {
			recordDate = format.parse(dateFormat.format(calendar.getTime()) + " 23:00:00");
		} catch (ParseException e) {
			ErrorLogWritter.save(e.getClass().getSimpleName() + e.getMessage());
		}
		// 获取工单号集合
		List<Record> orderNumRecords = Db.find(SQL.SELECT_DISTINCT_ORDERNUM_BY_TIME, startTime, endTime);
		if (orderNumRecords != null && !orderNumRecords.isEmpty()) {
			// 获取工单和产能集合
			List<Record> orderCapacityRecords = Db.find(SQL.SELECT_ORDERS_CAPATICY_BY_TIME, startTime, endTime);
			// 获取时间范围内所有订单的产量
			Record productionRecord = Db.findFirst(SQL.SELECT_PRODUCTION_BY_TIME, Constant.TIME_TYPE_DAY, startTime, endTime);
			// 保存每月工单记录
			OrderProduction orderProduction = new OrderProduction();
			orderProduction.setTimeType(Constant.TIME_TYPE_MONTH).setPlanProduct(productionRecord.getInt("plan_product")).setActualProduct(productionRecord.getInt("actual_product"));
			orderProduction.setHourlyActualProduct(productionRecord.getInt("actual_product") / Constant.DAILY_WORKING_HOURS / monthDays).setHourlyPlanProduct(productionRecord.getInt("plan_product") / Constant.DAILY_WORKING_HOURS / monthDays);
			orderProduction.setTime(recordDate).save();
			// 获取当月所有订单的产量
			Record orderProductionRecord = Db.findFirst(SQL.SELECT_ORDERPRODUCTION_BY_TIME, Constant.TIME_TYPE_MONTH, startTime, endTime);
			// 获取当月所有工位的产量
			List<Record> stationProductionRecords = Db.find(SQL.SELECT_SUM_STATIONRODUCTION_BY_TIME, Constant.TIME_TYPE_DAY, startTime, endTime);
			// 保存每月工位记录
			for (Record record : stationProductionRecords) {
				WorkstationProduction workstationProduction = new WorkstationProduction();
				workstationProduction.setProduct(record.getInt("product")).setTimeType(Constant.TIME_TYPE_MONTH).setType(record.getInt("type"));
				workstationProduction.setTime(recordDate).save();
			}
			// 获取历史工单最大月产量
			Integer historyMaxProduct = Db.queryInt(SQL.SELECT_MAXRODUCTION_BY_TIMETYPE, Constant.TIME_TYPE_MONTH);
			// 获取历史工位最大月产量
			List<Record> maxStationProductions = Db.find(SQL.SELECT_MAXSTATIONRODUCTION_BY_TIMETYPE, Constant.TIME_TYPE_MONTH);
			Document[] documents = emailDocService.getPaperCapacityDocument(orderNumRecords, orderCapacityRecords, orderProductionRecord, stationProductionRecords, historyMaxProduct, maxStationProductions, Constant.TIME_TYPE_MONTH, startDate, null);
			emailDocService.sendEmail(documents);
		} else {
			ErrorLogWritter.save("前一月不存在配置的工单和版本");
		}
	}
}
