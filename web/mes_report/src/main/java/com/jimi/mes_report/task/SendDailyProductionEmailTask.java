package com.jimi.mes_report.task;

import java.util.Calendar;

import org.apache.log4j.LogManager;
import org.jsoup.nodes.Document;

import com.jfinal.aop.Enhancer;
import com.jimi.mes_report.service.EmailDocService;
import com.jimi.mes_report.util.ErrorLogWritter;
/**发送每日产量邮件任务
 * @author   HCJ
 * @date     2020年4月8日 上午11:57:00
 */
public class SendDailyProductionEmailTask implements Runnable {

	private static org.apache.log4j.Logger logger = LogManager.getRootLogger();
;
	
	private static EmailDocService emailDocService = Enhancer.enhance(EmailDocService.class);


	@Override
	public void run() {
		sendDailyProductionEmail();
	}


	public static void sendDailyProductionEmail() {
		Calendar calendar = Calendar.getInstance();
		// 获取前一天时间W
		calendar.add(Calendar.DAY_OF_MONTH, -1);
		calendar.set(2019, 3, 1, 13, 13, 13);
		Document[] documents = emailDocService.getDailyProductionReportDocument(calendar.getTime());
		if (documents.length < 1) {
			ErrorLogWritter.save("前一天不存在订单产量，无需发送邮件");
		} else {
			for (Document document : documents) {
				logger.error(document);
			}
			
			emailDocService.sendEmail(documents);
			
		}

	}
}
