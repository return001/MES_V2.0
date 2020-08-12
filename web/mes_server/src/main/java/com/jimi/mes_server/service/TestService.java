package com.jimi.mes_server.service;

import com.jfinal.kit.StrKit;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Page;
import com.jfinal.plugin.activerecord.Record;
import com.jfinal.plugin.activerecord.SqlPara;
import com.jimi.mes_server.entity.SQL;
import com.jimi.mes_server.exception.OperationException;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.model.TestSystemSetting;
import com.jimi.mes_server.model.TestSystemSettingFunc;
import com.jimi.mes_server.model.TestSystemSettingLog;
import com.jimi.mes_server.model.TestSystemSettingOqc;
import com.jimi.mes_server.service.base.SelectService;

import java.io.File;
import java.io.IOException;
import java.util.Date;
import java.util.List;

import org.apache.commons.io.FileUtils;

/**测试配置项业务层
 * @author   HCJ
 * @date     2018年10月11日 下午6:01:22
 */
public class TestService extends SelectService {

	private static final String GuideFileLink = "/home/mes_document/guide_document";
	
	private static final String SELECT_TEST_SYSTEM_SETTING_FUNC_SQL = "SELECT * FROM TestSystemSettingFunc";
	
	private static final String SELECT_TEST_SYSTEM_SETTING_SQL = "SELECT * FROM TestSystemSetting";

	private static final String SELECT_TEST_SYSTEM_SETTING_OQC_SQL = "SELECT * FROM TestSystemSettingOqc";

	public boolean copy(String operator,String oldKey, Integer type, String newKey) {
		TestSystemSetting coupleSetTing = null;
		TestSystemSettingFunc functionSetTing = null;
		TestSystemSettingOqc oqcSetTing = null;
		Boolean result;

		//操作日志记录
		TestSystemSettingLog testSystemSettingLog = new TestSystemSettingLog();
		testSystemSettingLog.setOperator(operator);
		testSystemSettingLog.setOperationType(3);
		testSystemSettingLog.setTime(new Date());
		testSystemSettingLog.setSettingType(type);

		switch (type) {
		case 0:
			functionSetTing = TestSystemSettingFunc.dao.use("db1").findById(oldKey);
			functionSetTing.setSoftWare(newKey);
			functionSetTing.setRecordTime(new Date());
			result =  functionSetTing.use("db1").save();
			testSystemSettingLog.setSoftVersion(functionSetTing.getSoftVersion());
			testSystemSettingLog.setModel(functionSetTing.getMachineName());
			testSystemSettingLog.setStation(functionSetTing.getStation());
			testSystemSettingLog.setOrderName(functionSetTing.getOrderName());
			break;
		case 1:
			functionSetTing = TestSystemSettingFunc.dao.use("db2").findById(oldKey);
			functionSetTing.setSoftWare(newKey);
			functionSetTing.setRecordTime(new Date());
			result =  functionSetTing.use("db2").save();
			testSystemSettingLog.setSoftVersion(functionSetTing.getSoftVersion());
			testSystemSettingLog.setModel(functionSetTing.getMachineName());
			testSystemSettingLog.setStation(functionSetTing.getStation());
			testSystemSettingLog.setOrderName(functionSetTing.getOrderName());
			break;
		case 2:
			coupleSetTing = TestSystemSetting.dao.use("db2").findById(oldKey);
			coupleSetTing.setSoftWare(newKey);
			coupleSetTing.setRecordTime(new Date());
			result =  coupleSetTing.use("db2").save();
			testSystemSettingLog.setSoftVersion(coupleSetTing.getSoftVersion());
			testSystemSettingLog.setModel(coupleSetTing.getMachineName());
			testSystemSettingLog.setStation(coupleSetTing.getStation());
			testSystemSettingLog.setOrderName(coupleSetTing.getOrderName());
			break;
		case 3:
			functionSetTing = TestSystemSettingFunc.dao.use("db3").findById(oldKey);
			functionSetTing.setSoftWare(newKey);
			functionSetTing.setRecordTime(new Date());
			result =  functionSetTing.use("db3").save();
			testSystemSettingLog.setSoftVersion(functionSetTing.getSoftVersion());
			testSystemSettingLog.setModel(functionSetTing.getMachineName());
			testSystemSettingLog.setStation(functionSetTing.getStation());
			testSystemSettingLog.setOrderName(functionSetTing.getOrderName());
			break;
		case 4:
			coupleSetTing = TestSystemSetting.dao.use("db3").findById(oldKey);
			coupleSetTing.setSoftWare(newKey);
			coupleSetTing.setRecordTime(new Date());
			result =  coupleSetTing.use("db3").save();
			testSystemSettingLog.setSoftVersion(coupleSetTing.getSoftVersion());
			testSystemSettingLog.setModel(coupleSetTing.getMachineName());
			testSystemSettingLog.setStation(coupleSetTing.getStation());
			testSystemSettingLog.setOrderName(coupleSetTing.getOrderName());
			break;
		case 5:
			oqcSetTing = TestSystemSettingOqc.dao.use("db2").findById(oldKey);
			oqcSetTing.setSoftWare(newKey);
			oqcSetTing.setRecordTime(new Date());
			result =  oqcSetTing.use("db2").save();
			testSystemSettingLog.setSoftVersion(oqcSetTing.getSoftVersion());
			testSystemSettingLog.setModel(oqcSetTing.getMachineName());
			testSystemSettingLog.setStation(oqcSetTing.getStation());
			testSystemSettingLog.setOrderName(oqcSetTing.getOrderName());
			break;
		default:
			return false;
		}
		testSystemSettingLog.setResult(result);
		testSystemSettingLog.save();
		return result;
	}


	public boolean create(TestSystemSetting coupleSetTing, TestSystemSettingFunc functionSetTing, TestSystemSettingOqc oqcSetTing,Integer type) {
		TestSystemSettingFunc functionTestSetTing = null;
		TestSystemSetting coupleTestSetTing = null;
		TestSystemSettingOqc oqcTestSetTing = null;
		String softWare;
		switch (type) {
		case 0:
			if (functionSetTing.getOrderName() == null || functionSetTing.getSoftVersion() == null  || functionSetTing.getMachineName() == null || functionSetTing.getStation() == null) {
				throw new ParameterException("机型名、版本号、订单号和配置不能为空");
			}
			softWare = functionSetTing.getSoftVersion() + functionSetTing.getOrderName();
			functionTestSetTing = TestSystemSettingFunc.dao.use("db1").findById(softWare);
			if (functionTestSetTing != null) {
				throw new ParameterException("已存在此配置");
			}
			functionSetTing.setSoftWare(softWare);
			return functionSetTing.use("db1").save();
		case 1:
			if (functionSetTing.getOrderName() == null || functionSetTing.getSoftVersion() == null || functionSetTing.getMachineName() == null || functionSetTing.getStation() == null) {
				throw new ParameterException("机型名、版本号、订单号和配置不能为空");
			}
			softWare = functionSetTing.getSoftVersion() + functionSetTing.getOrderName();
			functionTestSetTing = TestSystemSettingFunc.dao.use("db2").findById(softWare);
			if (functionTestSetTing != null) {
				throw new ParameterException("已存在此配置");
			}
			functionSetTing.setSoftWare(softWare);
			return functionSetTing.use("db2").save();
		case 2:
			if (coupleSetTing.getOrderName() == null || coupleSetTing.getSoftVersion() == null || coupleSetTing.getMachineName() == null || coupleSetTing.getStation() == null) {
				throw new ParameterException("机型名、版本号、订单号和配置不能为空");
			}
			softWare = coupleSetTing.getSoftVersion() + coupleSetTing.getOrderName();
			coupleTestSetTing = TestSystemSetting.dao.use("db2").findById(softWare);
			if (coupleTestSetTing != null) {
				throw new ParameterException("已存在此配置");
			}
			coupleSetTing.setSoftWare(softWare);
			return coupleSetTing.use("db2").save();
		case 3:
			if (functionSetTing.getOrderName() == null || functionSetTing.getSoftVersion() == null || functionSetTing.getMachineName() == null || functionSetTing.getStation() == null) {
				throw new ParameterException("机型名、版本号、订单号和配置不能为空");
			}
			softWare = functionSetTing.getSoftVersion() + functionSetTing.getOrderName();
			functionTestSetTing = TestSystemSettingFunc.dao.use("db3").findById(softWare);
			if (functionTestSetTing != null) {
				throw new ParameterException("已存在此配置");
			}
			if (functionSetTing.getFileLink() != null && !functionSetTing.getFileLink().trim().equals("")) {
				File oldFile = new File(functionSetTing.getFileLink());
				String path = saveGuideFile(oldFile, functionSetTing.getFileName());
				coupleSetTing.setFileLink(path).setFileName(functionSetTing.getFileName());
			}
			functionSetTing.setSoftWare(softWare);
			return functionSetTing.use("db3").save();
		case 4:
			if (coupleSetTing.getOrderName() == null || coupleSetTing.getSoftVersion() == null || coupleSetTing.getMachineName() == null || coupleSetTing.getStation() == null) {
				throw new ParameterException("机型名、版本号、订单号和配置不能为空");
			}
			
			softWare = coupleSetTing.getSoftVersion() + coupleSetTing.getOrderName();
			coupleTestSetTing = TestSystemSetting.dao.use("db3").findById(softWare);
			if (coupleTestSetTing != null) {
				throw new ParameterException("已存在此配置");
			}
			if (coupleSetTing.getFileLink() != null && !coupleSetTing.getFileLink().trim().equals("")) {
				File oldFile = new File(coupleSetTing.getFileLink());
				String path = saveGuideFile(oldFile, coupleSetTing.getFileName());
				coupleSetTing.setFileLink(path).setFileName(coupleSetTing.getFileName());
			}
			coupleSetTing.setSoftWare(softWare);
			return coupleSetTing.use("db3").save();
		case 5:
			if (oqcSetTing.getOrderName() == null || oqcSetTing.getSoftVersion() == null|| oqcSetTing.getMachineName() == null || oqcSetTing.getStation() == null) {
				throw new ParameterException("机型名、版本号和配置不能为空");
			}
			softWare = oqcSetTing.getSoftVersion() + oqcSetTing.getOrderName();
			oqcTestSetTing = TestSystemSettingOqc.dao.use("db2").findById(softWare);
			if (oqcTestSetTing != null) {
				throw new ParameterException("已存在此配置");
			}
			oqcSetTing.setSoftWare(softWare);
			return oqcSetTing.use("db2").save();
		default:
			return false;
		}
	}


	public boolean cancel(String operator,String key, Integer type) {
		TestSystemSetting coupleSetTing = null;
		TestSystemSettingFunc functionSetTing = null;
		TestSystemSettingOqc oqcSetTing = null;
		//操作日志记录
		TestSystemSettingLog testSystemSettingLog = new TestSystemSettingLog();
		testSystemSettingLog.setOperator(operator);
		testSystemSettingLog.setOperationType(2);
		testSystemSettingLog.setTime(new Date());
		testSystemSettingLog.setSettingType(type);

		Boolean result;
		switch (type) {
		case 0:
			functionSetTing = TestSystemSettingFunc.dao.use("db1").findById(key);
			testSystemSettingLog.setSoftVersion(functionSetTing.getSoftVersion());
			testSystemSettingLog.setModel(functionSetTing.getMachineName());
			testSystemSettingLog.setStation(functionSetTing.getStation());
			testSystemSettingLog.setOrderName(functionSetTing.getOrderName());
			result = functionSetTing.use("db1").delete();
			break;
		case 1:
			functionSetTing = TestSystemSettingFunc.dao.use("db2").findById(key);
			testSystemSettingLog.setSoftVersion(functionSetTing.getSoftVersion());
			testSystemSettingLog.setModel(functionSetTing.getMachineName());
			testSystemSettingLog.setStation(functionSetTing.getStation());
			testSystemSettingLog.setOrderName(functionSetTing.getOrderName());
			result = functionSetTing.use("db2").delete();
			break;
		case 2:
			coupleSetTing = TestSystemSetting.dao.use("db2").findById(key);
			testSystemSettingLog.setSoftVersion(coupleSetTing.getSoftVersion());
			testSystemSettingLog.setModel(coupleSetTing.getMachineName());
			testSystemSettingLog.setStation(coupleSetTing.getStation());
			testSystemSettingLog.setOrderName(coupleSetTing.getOrderName());
			result =  coupleSetTing.use("db2").delete();
			break;
		case 3:
			functionSetTing = TestSystemSettingFunc.dao.use("db3").findById(key);
			testSystemSettingLog.setSoftVersion(functionSetTing.getSoftVersion());
			testSystemSettingLog.setModel(functionSetTing.getMachineName());
			testSystemSettingLog.setStation(functionSetTing.getStation());
			testSystemSettingLog.setOrderName(functionSetTing.getOrderName());
			result =  functionSetTing.use("db3").delete();
			break;
		case 4:
			coupleSetTing = TestSystemSetting.dao.use("db3").findById(key);
			testSystemSettingLog.setSoftVersion(coupleSetTing.getSoftVersion());
			testSystemSettingLog.setModel(coupleSetTing.getMachineName());
			testSystemSettingLog.setStation(coupleSetTing.getStation());
			testSystemSettingLog.setOrderName(coupleSetTing.getOrderName());
			result =  coupleSetTing.use("db3").delete();
			break;
		case 5:
			oqcSetTing = TestSystemSettingOqc.dao.use("db2").findById(key);
			testSystemSettingLog.setSoftVersion(oqcSetTing.getSoftVersion());
			testSystemSettingLog.setModel(oqcSetTing.getMachineName());
			testSystemSettingLog.setStation(oqcSetTing.getStation());
			testSystemSettingLog.setOrderName(oqcSetTing.getOrderName());
			result =  oqcSetTing.use("db2").delete();
			break;
		default:
			result = false;
		}
		testSystemSettingLog.setResult(result);
		testSystemSettingLog.save();
		return result;
	}


	public boolean update(TestSystemSetting coupleSetTing, TestSystemSettingFunc functionSetTing, TestSystemSettingOqc oqcSetTing, Integer type) {
		TestSystemSettingFunc functionTestSetTing = null;
		TestSystemSetting coupleTestSetTing = null;
		TestSystemSettingOqc oqcTestSetTing = null;
		switch (type) {
		case 0:
			if (functionSetTing.getSoftWare() == null || functionSetTing.getMachineName() == null || functionSetTing.getStation() == null) {
				throw new ParameterException("机型名、版本号和配置不能为空");
			}
			functionTestSetTing = TestSystemSettingFunc.dao.use("db1").findById(functionSetTing.getSoftWare());
			if (functionTestSetTing == null) {
				throw new ParameterException("不存在此配置，无法更新");
			}
			return functionSetTing.use("db1").update();
		case 1:
			if (functionSetTing.getSoftWare() == null || functionSetTing.getMachineName() == null || functionSetTing.getStation() == null) {
				throw new ParameterException("机型名、版本号和配置不能为空");
			}
			functionTestSetTing = TestSystemSettingFunc.dao.use("db2").findById(functionSetTing.getSoftWare());
			if (functionTestSetTing == null) {
				throw new ParameterException("不存在此配置，无法更新");
			}
			return functionSetTing.use("db2").update();
		case 2:
			if (coupleSetTing.getSoftWare() == null || coupleSetTing.getMachineName() == null || coupleSetTing.getStation() == null) {
				throw new ParameterException("机型名、版本号和配置不能为空");
			}
			coupleTestSetTing = TestSystemSetting.dao.use("db2").findById(coupleSetTing.getSoftWare());
			if (coupleTestSetTing == null) {
				throw new ParameterException("不存在此配置，无法更新");
			}
			return coupleSetTing.use("db2").update();
		case 3:
			if (functionSetTing.getSoftWare() == null || functionSetTing.getMachineName() == null || functionSetTing.getStation() == null) {
				throw new ParameterException("机型名、版本号和配置不能为空");
			}
			functionTestSetTing = TestSystemSettingFunc.dao.use("db3").findById(functionSetTing.getSoftWare());
			if (functionTestSetTing == null) {
				throw new ParameterException("不存在此配置，无法更新");
			}
			return functionSetTing.use("db3").update();
		case 4:
			if (coupleSetTing.getSoftWare() == null || coupleSetTing.getMachineName() == null || coupleSetTing.getStation() == null) {
				throw new ParameterException("机型名、版本号和配置不能为空");
			}
			coupleTestSetTing = TestSystemSetting.dao.use("db3").findById(coupleSetTing.getSoftWare());
			if (coupleTestSetTing == null) {
				throw new ParameterException("不存在此配置，无法更新");
			}
			return coupleSetTing.use("db3").update();
		case 5:
			if (oqcSetTing.getSoftWare() == null || oqcSetTing.getMachineName() == null || oqcSetTing.getStation() == null) {
				throw new ParameterException("机型名、版本号和配置不能为空");
			}
			oqcTestSetTing = TestSystemSettingOqc.dao.use("db2").findById(oqcSetTing.getSoftWare());
			if (oqcTestSetTing == null) {
				throw new ParameterException("不存在此配置，无法更新");
			}
			return oqcSetTing.use("db2").update();
		default:
			return false;
		}
	}

	public Page<Record> getLogs(Integer pageNo, Integer pageSize,String startTime, String endTime, String operator, String model, Integer settingType, String softVersion, String orderName, Boolean result) {
		SqlPara sqlPara = new SqlPara();
		StringBuilder sql = new StringBuilder(SQL.SELECT_TEST_SYSTeM_SETTING_LOG);
		sql.append(" WHERE 1 = 1 ");
		if (!StrKit.isBlank(operator)) {
			sql.append(" AND operator ='").append(operator).append("'");
		}
		if (!StrKit.isBlank(model)) {
			sql.append(" AND model ='").append(model).append("'");
		}
		if (settingType != null) {
			sql.append(" AND setting_type ='").append(settingType).append("'");
		}
		if (!StrKit.isBlank(softVersion)) {
			sql.append(" AND soft_version ='").append(softVersion).append("'");
		}
		if (!StrKit.isBlank(orderName)) {
			sql.append(" AND order_name ='").append(orderName).append("'");
		}
		if (!StrKit.isBlank(startTime)) {
			sql.append(" AND time >= '").append(startTime).append("'");
		}
		if (!StrKit.isBlank(endTime)) {
			sql.append(" AND time <= '").append(endTime).append("'");
		}
		if (result != null){
			  sql.append(" AND result = '").append(result).append("'");
		}
		sql.append(" order by id desc");
		sqlPara.setSql(sql.toString());
		Page<Record> page = Db.paginate(pageNo, pageSize, sqlPara);
		List<Record> records = page.getList();
		for (Record record : records) {
			int type = record.get("operationType");
			result = record.get("result");
			if (type == 0){
				record.set("operationType","新增");
			}else if (type == 1){
				record.set("operationType","修改");
			}else if (type == 2){
				record.set("operationType","删除");
			}else if (type == 3){
				record.set("operationType","另存为");
			}
			record.set("result",result ?"成功":"失败");
		}
		return page;
	}
	
	
	
	public void uploadGuideFile(File file, String id, Integer type) {
		String fileName = file.getName();
		String path = null;
		String oldPath = null;
		if (type == null) {
			throw new OperationException("上传失败，仅研发测试可以上传");
		}
		switch (type) {
		case 3:
			TestSystemSettingFunc functionTestSetTing = TestSystemSettingFunc.dao.use("db3").findById(id);
			if (functionTestSetTing == null) {
				throw new OperationException("上传失败， 组装研发配置表不存在该记录");
			}
			oldPath = functionTestSetTing.getFileLink();
			path = saveGuideFile(file, fileName);
			functionTestSetTing.setFileName(fileName);
			functionTestSetTing.setFileLink(path);
			functionTestSetTing.update();
			if (oldPath != null && !oldPath.trim().equals("")) {
				File oldFile = new File(oldPath);
				if (oldFile.exists()) {
					oldFile.delete();
				}
			}
			break;
		case 4:
			TestSystemSetting coupleTestSetTing = TestSystemSetting.dao.use("db3").findById(id);
			if (coupleTestSetTing == null) {
				throw new OperationException("上传失败，耦合研发配置表不存在该记录");
			}
			oldPath = coupleTestSetTing.getFileLink();
			path = saveGuideFile(file, fileName);
			coupleTestSetTing.setFileName(fileName);
			coupleTestSetTing.setFileLink(path);
			coupleTestSetTing.update();
			if (oldPath != null && !oldPath.trim().equals("")) {
				File oldFile = new File(oldPath);
				if (oldFile.exists()) {
					oldFile.delete();
				}
			}
			break;
		default:
			throw new OperationException("上传失败，仅研发测试可以上传");
		}
	}
	
	
	public File downloadGuideFile(String id, Integer type) {
		if (type == null) {
			throw new OperationException("下载失败，仅研发测试可以下载");
		}
		File guideFile = null;
		switch (type) {
		case 3:
			TestSystemSettingFunc functionTestSetTing = TestSystemSettingFunc.dao.use("db3").findById(id);
			if (functionTestSetTing == null) {
				throw new OperationException("下载失败， 组装研发配置表不存在该记录");
			}
			if (functionTestSetTing.getFileLink() == null || functionTestSetTing.getFileLink().trim().equals("") || functionTestSetTing.getFileName() == null) {
				throw new OperationException("下载失败，该记录并未绑定文件");
			}
			guideFile = new File(functionTestSetTing.getFileLink());
			if (!guideFile.exists()) {
				throw new OperationException("下载失败，该记录绑定的文件不存在");
			}
			break;
		case 4:
			TestSystemSetting coupleTestSetTing = TestSystemSetting.dao.use("db3").findById(id);
			if (coupleTestSetTing == null) {
				throw new OperationException("下载失败，耦合研发配置表不存在该记录");
			}
			if (coupleTestSetTing.getFileLink() == null || coupleTestSetTing.getFileLink().trim().equals("") || coupleTestSetTing.getFileName() == null) {
				throw new OperationException("下载失败，该记录并未绑定文件");
			}
			guideFile = new File(coupleTestSetTing.getFileLink());
			if (!guideFile.exists()) {
				throw new OperationException("下载失败，该记录绑定的文件不存在");
			}
			break;
		default:
			throw new OperationException("下载失败，仅研发测试可以下载");
		}
		return guideFile;
	}
	
	
	private String saveGuideFile(File oldFile, String newFileName) {
		if (!oldFile.exists()) {
			throw new OperationException("文件不存在，无法生成生产指导指南");
		}
		File newFile = new File(GuideFileLink + File.separator + System.currentTimeMillis() + File.separator + newFileName);
		try {
			FileUtils.copyFile(oldFile, newFile);
			return newFile.getAbsolutePath();
		} catch (IOException e) {
			e.printStackTrace();
			throw new OperationException("无法生成生产指导指南");
		}
	}


	/**
	 * <p>Description: <p>
	 * @return
	 * @exception
	 * @author trjie
	 * @Time 2020年7月21日
	 */
	public void initSBData() {
		
		List<TestSystemSettingFunc> functionTestSetTing1s = TestSystemSettingFunc.dao.use("db1").find(SELECT_TEST_SYSTEM_SETTING_FUNC_SQL);
		List<TestSystemSettingFunc> functionTestSetTing2s = TestSystemSettingFunc.dao.use("db2").find(SELECT_TEST_SYSTEM_SETTING_FUNC_SQL);
		List<TestSystemSetting> functionTestSet2s = TestSystemSetting.dao.use("db2").find(SELECT_TEST_SYSTEM_SETTING_SQL);
		List<TestSystemSettingFunc> functionTestSetTing3s = TestSystemSettingFunc.dao.use("db3").find(SELECT_TEST_SYSTEM_SETTING_FUNC_SQL);
		List<TestSystemSetting> functionTestSet3s = TestSystemSetting.dao.use("db3").find(SELECT_TEST_SYSTEM_SETTING_SQL);
		List<TestSystemSettingOqc> testSystemSettingOqcs = TestSystemSettingOqc.dao.use("db2").find(SELECT_TEST_SYSTEM_SETTING_OQC_SQL);

		String[] a = new String[68];
		for (int i = 0; i < 68; i++) {
			a[i] = "Setting" + i;
		}
		int j = 0;
		//********************************功能测试***************************************
		for (TestSystemSettingFunc testSystemSettingFunc : functionTestSetTing1s) {
			
			int i = testSystemSettingFunc.getSoftWare().toUpperCase().indexOf("SMT");
			if (i != -1) {
				testSystemSettingFunc.setSoftVersion(testSystemSettingFunc.getSoftWare().substring(0, i));
				testSystemSettingFunc.setOrderName(testSystemSettingFunc.getSoftWare().substring(i, testSystemSettingFunc.getSoftWare().length()));
				testSystemSettingFunc.use("db1").update();
				j++;
				continue;
			}
			i = testSystemSettingFunc.getSoftWare().toUpperCase().indexOf("CPX");
			if (i != -1) {
				testSystemSettingFunc.setSoftVersion(testSystemSettingFunc.getSoftWare().substring(0, i));
				testSystemSettingFunc.setOrderName(testSystemSettingFunc.getSoftWare().substring(i, testSystemSettingFunc.getSoftWare().length()));
				testSystemSettingFunc.use("db1").update();
				j++;
				continue;
			}
		}
		System.out.println("NetMarkIMEI | TestSystemSettingFunc TOTAL: " + j);
		j = 0;
		for (TestSystemSettingFunc testSystemSettingFunc : functionTestSetTing2s) {
			int i = testSystemSettingFunc.getSoftWare().toUpperCase().indexOf("CPX");
			if (i != -1) {
				testSystemSettingFunc.setSoftVersion(testSystemSettingFunc.getSoftWare().substring(0, i));
				testSystemSettingFunc.setOrderName(testSystemSettingFunc.getSoftWare().substring(i, testSystemSettingFunc.getSoftWare().length()));
				testSystemSettingFunc.use("db2").update();
				j++;
				continue;
			}
			i = testSystemSettingFunc.getSoftWare().toUpperCase().indexOf("WPX");
			if (i != -1) {
				testSystemSettingFunc.setSoftVersion(testSystemSettingFunc.getSoftWare().substring(0, i));
				testSystemSettingFunc.setOrderName(testSystemSettingFunc.getSoftWare().substring(i, testSystemSettingFunc.getSoftWare().length()));
				testSystemSettingFunc.use("db2").update();
				j++;
				continue;
			}
			i = testSystemSettingFunc.getSoftWare().toUpperCase().indexOf("JIMI");
			if (i != -1) {
				testSystemSettingFunc.setSoftVersion(testSystemSettingFunc.getSoftWare().substring(0, i));
				testSystemSettingFunc.setOrderName(testSystemSettingFunc.getSoftWare().substring(i, testSystemSettingFunc.getSoftWare().length()));
				testSystemSettingFunc.use("db2").update();
				j++;
				continue;
			}
			
		}
		System.out.println("NetMarkIMEI2 | TestSystemSettingFunc TOTAL: " + j);
		
		j = 0;
		for (TestSystemSetting testSystemSet : functionTestSet2s) {
			int i = testSystemSet.getSoftWare().toUpperCase().indexOf("CPX");
			if (i != -1) {
				testSystemSet.setSoftVersion(testSystemSet.getSoftWare().substring(0, i));
				testSystemSet.setOrderName(testSystemSet.getSoftWare().substring(i, testSystemSet.getSoftWare().length()));
				testSystemSet.use("db2").update();
				j++;
				continue;
			}
			i = testSystemSet.getSoftWare().toUpperCase().indexOf("WPX");
			if (i != -1) {
				testSystemSet.setSoftVersion(testSystemSet.getSoftWare().substring(0, i));
				testSystemSet.setOrderName(testSystemSet.getSoftWare().substring(i, testSystemSet.getSoftWare().length()));
				testSystemSet.use("db2").update();
				j++;
				continue;
			}
			i = testSystemSet.getSoftWare().toUpperCase().indexOf("JIMI");
			if (i != -1) {
				testSystemSet.setSoftVersion(testSystemSet.getSoftWare().substring(0, i));
				testSystemSet.setOrderName(testSystemSet.getSoftWare().substring(i, testSystemSet.getSoftWare().length()));
				testSystemSet.use("db2").update();
				j++;
				continue;
			}
			
		}
		System.out.println("NetMarkIMEI2 | TestSystemSetting TOTAL: " + j);
		
		// **************************OQC测试*****************************
		j = 0;
		for (TestSystemSettingOqc testSystemSettingOqc : testSystemSettingOqcs) {
			
			int i = testSystemSettingOqc.getSoftWare().toUpperCase().indexOf("CPX");
			if (i != -1) {
				testSystemSettingOqc.setSoftVersion(testSystemSettingOqc.getSoftWare().substring(0, i));
				testSystemSettingOqc.setOrderName(testSystemSettingOqc.getSoftWare().substring(i, testSystemSettingOqc.getSoftWare().length()));
				testSystemSettingOqc.use("db2").update();
				j++;
				continue;
			}
			i = testSystemSettingOqc.getSoftWare().toUpperCase().indexOf("WPX");
			if (i != -1) {
				testSystemSettingOqc.setSoftVersion(testSystemSettingOqc.getSoftWare().substring(0, i));
				testSystemSettingOqc.setOrderName(testSystemSettingOqc.getSoftWare().substring(i, testSystemSettingOqc.getSoftWare().length()));
				testSystemSettingOqc.use("db2").update();
				j++;
				continue;
			}
			i = testSystemSettingOqc.getSoftWare().toUpperCase().indexOf("JIMI");
			if (i != -1) {
				testSystemSettingOqc.setSoftVersion(testSystemSettingOqc.getSoftWare().substring(0, i));
				testSystemSettingOqc.setOrderName(testSystemSettingOqc.getSoftWare().substring(i, testSystemSettingOqc.getSoftWare().length()));
				testSystemSettingOqc.use("db2").update();
				j++;
				continue;
			}
		}
		System.out.println("NetMarkIMEI2 | testSystemSettingOqc TOTAL: " + j);
		
		//***************************研发测试*****************************
		
		j = 0;
		for (TestSystemSettingFunc testSystemSettingFunc : functionTestSetTing3s) {
			boolean flag = true;
			for (String string : a) {
				String value = testSystemSettingFunc.getStr(string);
				if (value != null && !value.equals("") && value.startsWith("功能测试软件版本")) {
					String[] b = value.split("@@");
					if (b[1] != null && !b[1].trim().equals("")) {
						if (testSystemSettingFunc.getSoftWare().toUpperCase().startsWith(b[1].toUpperCase())) {
							testSystemSettingFunc.setSoftVersion(b[1]);
							testSystemSettingFunc.setOrderName(testSystemSettingFunc.getSoftWare().substring(b[1].length(), testSystemSettingFunc.getSoftWare().length()));
							testSystemSettingFunc.use("db3").update();
							j ++;
							flag = false;
						}
					}
					break;
				}
			}
			if (flag) {
				int i = testSystemSettingFunc.getSoftWare().toUpperCase().indexOf("SMT");
				if (i != -1) {
					testSystemSettingFunc.setSoftVersion(testSystemSettingFunc.getSoftWare().substring(0, i));
					testSystemSettingFunc.setOrderName(testSystemSettingFunc.getSoftWare().substring(i, testSystemSettingFunc.getSoftWare().length()));
					testSystemSettingFunc.use("db3").update();
					j++;
					continue;
				}
				i = testSystemSettingFunc.getSoftWare().toUpperCase().indexOf("CPX");
				if (i != -1) {
					testSystemSettingFunc.setSoftVersion(testSystemSettingFunc.getSoftWare().substring(0, i));
					testSystemSettingFunc.setOrderName(testSystemSettingFunc.getSoftWare().substring(i, testSystemSettingFunc.getSoftWare().length()));
					testSystemSettingFunc.use("db3").update();
					j++;
					continue;
				}
				i = testSystemSettingFunc.getSoftWare().toUpperCase().indexOf("XZX");
				if (i != -1) {
					testSystemSettingFunc.setSoftVersion(testSystemSettingFunc.getSoftWare().substring(0, i));
					testSystemSettingFunc.setOrderName(testSystemSettingFunc.getSoftWare().substring(i, testSystemSettingFunc.getSoftWare().length()));
					testSystemSettingFunc.use("db3").update();
					j++;
					continue;
				}
			}
		}
		System.out.println("NetMarkIMEI3 | TestSystemSettingFunc TOTAL: " + j);

		j = 0;
		for (TestSystemSetting testSystemSet : functionTestSet3s) {
			int i = testSystemSet.getSoftWare().toUpperCase().indexOf("SMT");
			if (i != -1) {
				testSystemSet.setSoftVersion(testSystemSet.getSoftWare().substring(0, i));
				testSystemSet.setOrderName(testSystemSet.getSoftWare().substring(i, testSystemSet.getSoftWare().length()));
				testSystemSet.use("db3").update();
				j++;
				continue;
			}
			i = testSystemSet.getSoftWare().toUpperCase().indexOf("CPX");
			if (i != -1) {
				testSystemSet.setSoftVersion(testSystemSet.getSoftWare().substring(0, i));
				testSystemSet.setOrderName(testSystemSet.getSoftWare().substring(i, testSystemSet.getSoftWare().length()));
				testSystemSet.use("db3").update();
				j++;
				continue;
			}
			i = testSystemSet.getSoftWare().toUpperCase().indexOf("XZX");
			if (i != -1) {
				testSystemSet.setSoftVersion(testSystemSet.getSoftWare().substring(0, i));
				testSystemSet.setOrderName(testSystemSet.getSoftWare().substring(i, testSystemSet.getSoftWare().length()));
				testSystemSet.use("db3").update();
				j++;
				continue;
			}
			
		}
		System.out.println("NetMarkIMEI3 | TestSystemSetting TOTAL: " + j);
	}
	
	
	
	
}
