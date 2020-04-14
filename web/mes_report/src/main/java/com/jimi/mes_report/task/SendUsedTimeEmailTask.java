package com.jimi.mes_report.task;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.List;

import org.jsoup.nodes.Document;

import com.jfinal.aop.Enhancer;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Record;
import com.jimi.mes_report.constant.SQL;
import com.jimi.mes_report.service.EmailDocService;
import com.jimi.mes_report.util.ErrorLogWritter;

public class SendUsedTimeEmailTask implements Runnable {

	private static EmailDocService emailDocService = Enhancer.enhance(EmailDocService.class);


	@Override
	public void run() {
		sendUsedTimeEmail();
	}


	private void sendUsedTimeEmail() {
		Calendar calendar = Calendar.getInstance();
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
		// 获取前一天时间
		calendar.add(Calendar.DAY_OF_MONTH, -1);
		String startTime = dateFormat.format(calendar.getTime()) + " 00:00:00";
		String endTime = dateFormat.format(calendar.getTime()) + " 23:59:59";
		// 获取工单和使用时间集合
		List<Record> ordersRecords = Db.find(SQL.SELECT_WORKSTATIONS_ORDERS_BY_TIME, startTime, endTime);
		if (ordersRecords != null && !ordersRecords.isEmpty()) {
			Document[] documents = emailDocService.getUsedTimeDocument(ordersRecords);
			emailDocService.sendEmail(documents);
		} else {
			ErrorLogWritter.save("前一天不存在订单");
		}
	}

}
