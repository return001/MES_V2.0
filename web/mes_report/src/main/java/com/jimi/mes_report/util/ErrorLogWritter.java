package com.jimi.mes_report.util;

import java.util.Date;
import org.apache.log4j.LogManager;

import com.jimi.mes_report.model.ErrorLog;

public class ErrorLogWritter {


	public static boolean save(String message) {
		LogManager.getRootLogger().error(message);
		ErrorLog errorLog = new ErrorLog();
		System.out.println("record: " + errorLog);
		return errorLog.set("message", message).set("time", new Date()).save();
	}

}
