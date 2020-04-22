package com.jimi.mes_report.util;

import java.text.SimpleDateFormat;
import java.util.Date;

public class CommonUtil {

	// 计算两个日期之间相差天数
	public static long daysBetween(String smdate, String bdate) throws Exception {
		SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
		Date begin = sdf.parse(smdate);
		Date end = sdf.parse(bdate);
		long between = (end.getTime() - begin.getTime()) / 1000;// 除以1000是为了转换成秒
		long day = between / (24 * 3600);
		long hour = between % (24 * 3600) / 3600;
		if (hour >= 0 || day == 0) {
			return day + 1;
		}
		return day;
	}


	// 计算两日期相差秒数
	public static int dateDiff(String startTime, String endTime, String format) throws Exception {
		// 按照传入的格式生成一个simpledateformate对象
		SimpleDateFormat sd = new SimpleDateFormat(format);
		long ns = 1000;// 一秒钟的毫秒数
		long diff;
		// 获得两个时间的毫秒时间差异
		diff = sd.parse(endTime).getTime() - sd.parse(startTime).getTime();
		long sec = diff / ns;// 计算差多少秒//输出结果
		return (int) sec;
	}


	/**@author HCJ
	 * 秒数转时分秒
	 * @param number 秒数的大小
	 * @date 2020年4月22日 上午11:45:15
	 */
	public static String secondToTime(int number) {
		int hour = number / 3600;
		int minute = number / 60 % 60;
		int second = number % 60;
		return hour + "时" + minute + "分" + second + "秒";
	}


	public static void main(String[] args) {
		System.err.println(secondToTime(5260));
	}

}
