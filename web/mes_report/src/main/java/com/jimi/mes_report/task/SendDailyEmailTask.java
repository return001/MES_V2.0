package com.jimi.mes_report.task;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.List;

import org.jsoup.nodes.Document;

import com.jfinal.aop.Enhancer;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Record;
import com.jimi.mes_report.constant.Constant;
import com.jimi.mes_report.constant.SQL;
import com.jimi.mes_report.service.EmailDocService;
import com.jimi.mes_report.util.ErrorLogWritter;

/**发送每日邮件
 * @author   HCJ
 * @date     2019年4月29日 上午11:05:29
 */
public class SendDailyEmailTask implements Runnable {

	private static EmailDocService emailDocService = Enhancer.enhance(EmailDocService.class);


	@Override
	public void run() {
		sendDailyEmail();
	}


	private void sendDailyEmail() {
		Calendar calendar = Calendar.getInstance();
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
		// 获取前一天时间
		calendar.add(Calendar.DAY_OF_MONTH, -1);
		String startTime = dateFormat.format(calendar.getTime()) + " 00:00:00";
		String endTime = dateFormat.format(calendar.getTime()) + " 23:59:59";
		// 获取工单号集合
		List<Record> orderNumRecords = Db.find(SQL.SELECT_DISTINCT_ORDERNUM_BY_TIME, startTime, endTime);
		if (orderNumRecords != null && !orderNumRecords.isEmpty()) {
			// 获取工单和产能集合
			List<Record> orderCapacityRecords = Db.find(SQL.SELECT_ORDERS_CAPATICY_BY_TIME, startTime, endTime);
			// 获取当天所有订单的产量
			Record orderProductionRecord = Db.findFirst(SQL.SELECT_ORDERPRODUCTION_BY_TIME, Constant.TIME_TYPE_DAY, startTime, endTime);
			// 获取当天各个工位的产量
			List<Record> stationProductionRecords = Db.find(SQL.SELECT_STATIONRODUCTION_BY_TIME, Constant.TIME_TYPE_DAY, startTime, endTime);
			// 获取每天最大的订单产量
			Integer dailyMaxProduct = Db.queryInt(SQL.SELECT_MAXRODUCTION_BY_TIMETYPE, Constant.TIME_TYPE_DAY);
			// 根据工位获取每天最大的产量
			List<Record> maxStationProductions = Db.find(SQL.SELECT_MAXSTATIONRODUCTION_BY_TIMETYPE, Constant.TIME_TYPE_DAY);
			Document[] documents = emailDocService.getPaperCapacityDocument(orderNumRecords, orderCapacityRecords, orderProductionRecord, stationProductionRecords, dailyMaxProduct, maxStationProductions, Constant.TIME_TYPE_DAY, calendar.getTime(), null);
			emailDocService.sendEmail(documents);
		} else {
			ErrorLogWritter.save("前一天不存在配置的工单和版本");
		}
	}
}
