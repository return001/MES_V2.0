package com.jimi.mes_server.task;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;


import com.jfinal.plugin.activerecord.Db;

import com.jimi.mes_server.entity.SQL;
import com.jimi.mes_server.model.DailyCompletion;



public class SaveDailyProductionTask implements Runnable {


	@Override
	public void run() {
		Calendar calendar = Calendar.getInstance();
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
		// 获取前一天时间
		calendar.add(Calendar.DAY_OF_MONTH, -1);
		String startTime = dateFormat.format(calendar.getTime()) + " 00:00:00";
		String endTime = dateFormat.format(calendar.getTime()) + " 23:59:59";
		SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		// 设定当天记录的时间
		Date recordDate = new Date();
		try {
			recordDate = format.parse(dateFormat.format(calendar.getTime()) + " 10:00:00");
		} catch (ParseException e) {
			
			e.printStackTrace();
		}
		DailyCompletion dailyCompletion = new DailyCompletion();
		Integer quantity = Db.queryInt(SQL.SELECT_CARTONNUM_BY_TESTTIME, startTime,endTime);
		dailyCompletion.setQuantity(quantity).setTime(recordDate).save();
	}

	
}
