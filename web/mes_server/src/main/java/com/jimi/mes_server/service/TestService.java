package com.jimi.mes_server.service;

import com.jfinal.kit.StrKit;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Page;
import com.jfinal.plugin.activerecord.Record;
import com.jfinal.plugin.activerecord.SqlPara;
import com.jimi.mes_server.entity.SQL;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.model.TestSystemSetting;
import com.jimi.mes_server.model.TestSystemSettingFunc;
import com.jimi.mes_server.model.TestSystemSettingLog;
import com.jimi.mes_server.model.TestSystemSettingOqc;
import com.jimi.mes_server.service.base.SelectService;

import java.util.Date;
import java.util.List;

/**测试配置项业务层
 * @author   HCJ
 * @date     2018年10月11日 下午6:01:22
 */
public class TestService extends SelectService {

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
			softWare = functionSetTing.getSoftVersion() + "_"+ functionSetTing.getOrderName();
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
			softWare = functionSetTing.getSoftVersion() + "_"+ functionSetTing.getOrderName();
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
			softWare = coupleSetTing.getSoftVersion() + "_"+ coupleSetTing.getOrderName();
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
			softWare = functionSetTing.getSoftVersion() + "_"+ functionSetTing.getOrderName();
			functionTestSetTing = TestSystemSettingFunc.dao.use("db3").findById(softWare);
			if (functionTestSetTing != null) {
				throw new ParameterException("已存在此配置");
			}
			functionSetTing.setSoftWare(softWare);
			return functionSetTing.use("db3").save();
		case 4:
			if (coupleSetTing.getOrderName() == null || coupleSetTing.getSoftVersion() == null || coupleSetTing.getMachineName() == null || coupleSetTing.getStation() == null) {
				throw new ParameterException("机型名、版本号、订单号和配置不能为空");
			}
			softWare = coupleSetTing.getSoftVersion() + "_"+ coupleSetTing.getOrderName();
			coupleTestSetTing = TestSystemSetting.dao.use("db3").findById(softWare);
			if (coupleTestSetTing != null) {
				throw new ParameterException("已存在此配置");
			}
			coupleSetTing.setSoftWare(softWare);
			return coupleSetTing.use("db3").save();
		case 5:
			if (oqcSetTing.getOrderName() == null || oqcSetTing.getSoftVersion() == null|| oqcSetTing.getMachineName() == null || oqcSetTing.getStation() == null) {
				throw new ParameterException("机型名、版本号和配置不能为空");
			}
			softWare = oqcSetTing.getSoftVersion() + "_"+ oqcSetTing.getOrderName();
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

	public Page<Record> getLogs(Integer pageNo, Integer pageSize,String startTime, String endTime,String operator,String model,Integer settingType,String softVersion) {
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
		if (!StrKit.isBlank(startTime)) {
			sql.append(" AND time >= '").append(startTime).append(" 00:00:00'");
		}
		if (!StrKit.isBlank(endTime)) {
			sql.append(" AND time <= '").append(endTime).append(" 23:59:59'");
		}
		sql.append(" order by id desc");
		sqlPara.setSql(sql.toString());
		Page<Record> page = Db.paginate(pageNo, pageSize, sqlPara);
		List<Record> records = page.getList();
		for (Record record : records) {
			int type = record.get("operationType");
			Boolean result = record.get("result");
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
}
