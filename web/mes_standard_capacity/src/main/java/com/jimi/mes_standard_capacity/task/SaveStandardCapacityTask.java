package com.jimi.mes_standard_capacity.task;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;

import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Record;
import com.jimi.mes_standard_capacity.model.StandardCapacity;
import com.jimi.mes_standard_capacity.model.entity.DateFormat;
import com.jimi.mes_standard_capacity.model.entity.SQL;

/**保存标准产能任务
 * @author   HCJ
 * @date     2020年4月16日 下午2:54:07
 */
public class SaveStandardCapacityTask implements Runnable {

	private static Logger logger = LogManager.getRootLogger();


	@Override
	public void run() {
		saveStandardCapacity("2019-01-01", "2019-12-31");
	}


	/**@author HCJ
	 * 根据当前机型和当前工位以及当前时刻生产的数量和天数计算标准产能
	 * @param startDate 开始日期
	 * @param endDate 结束日期
	 * @date 2020年4月16日 下午2:55:19
	 */
	private void saveStandardCapacity(String startDate, String endDate) {
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
		try {
			dateFormat.parse(startDate);
			dateFormat.parse(endDate);
		} catch (Exception e) {
			logger.error("保存标准产能失败时间参数格式需为yyyy-MM-dd " + e.getClass().getSimpleName() + " : " + e.getMessage());
			return;
		}
		DateFormat startDateFormat = new DateFormat(startDate);
		DateFormat endDateFormat = new DateFormat(endDate);
		String imeiStartDate = startDateFormat.getStartIMEIDate();
		String printStartDate = startDateFormat.getStartPrintDate();
		String testStartDate = startDateFormat.getStartTestDate();
		String imeiEndDate = endDateFormat.getEndIMEIDate();
		String printEndDate = endDateFormat.getEndPrintDate();
		String testEndDate = endDateFormat.getEndTestDate();
		Object[] params = { testStartDate, testEndDate, testStartDate, testEndDate, testStartDate, testEndDate, testStartDate, testEndDate, printStartDate, printEndDate, printStartDate, printEndDate, imeiStartDate, imeiEndDate, testStartDate, testEndDate };
		List<Record> outputRecords = Db.use("db1").find(SQL.SELECT_OUTPUT_BY_TIME, params);
		if (outputRecords != null && !outputRecords.isEmpty()) {
			List<StandardCapacity> standardCapacities = new ArrayList<StandardCapacity>();
			Map<String, Integer> outputMap = new HashMap<String, Integer>();
			Map<String, Integer> dayMap = new HashMap<String, Integer>();
			// 保存当前机型和当前工位以及当前时刻生产的数量
			for (Record record : outputRecords) {
				StringBuilder stringBuilder = new StringBuilder();
				stringBuilder.append(record.getStr("model"));
				stringBuilder.append(",");
				stringBuilder.append(record.getInt("time"));
				stringBuilder.append(",");
				stringBuilder.append(record.getStr("workstation"));
				String key = stringBuilder.toString();
				if (outputMap.containsKey(key)) {
					outputMap.put(key, outputMap.get(key) + record.getInt("output"));
				} else {
					outputMap.put(key, record.getInt("output"));
				}
			}
			// 保存当前机型和当前工位以及当前时刻有进行生产的天数
			List<Record> daysRecords = Db.use("db1").find(SQL.SELECT_DAYS_BY_TIME, params);
			for (Record record : daysRecords) {
				StringBuilder stringBuilder = new StringBuilder();
				stringBuilder.append(record.getStr("model"));
				stringBuilder.append(",");
				stringBuilder.append(record.getInt("time"));
				stringBuilder.append(",");
				stringBuilder.append(record.getStr("workstation"));
				String key = stringBuilder.toString();
				dayMap.put(key, record.getInt("days"));
			}
			// 保存标准产能
			for (Entry<String, Integer> entry : outputMap.entrySet()) {
				String key = entry.getKey();
				Integer days = dayMap.get(key);
				if (days == null) {
					days = 1;
				}
				String[] keyArray = key.split(",");
				StandardCapacity standardCapacity = new StandardCapacity();
				standardCapacity.setOutput(entry.getValue() / days);
				standardCapacity.setModel(keyArray[0]).setTime(Integer.parseInt(keyArray[1])).setWorkstation(Integer.parseInt(keyArray[2]));
				standardCapacities.add(standardCapacity);
			}
			Db.batchSave(standardCapacities, standardCapacities.size());
			logger.info("从 " + startDate + " 到 " + endDate + " 的标准产能保存完成");
		} else {
			logger.info("从 " + startDate + " 到 " + endDate + " 没有进行生产");
		}
	}
}
