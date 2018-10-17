package com.jimi.mes_server.service;

import java.util.Date;

import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.model.TestSystemSetting;
import com.jimi.mes_server.model.TestSystemSettingFunc;
import com.jimi.mes_server.service.base.SelectService;

/**测试配置项业务层
 * @author   HCJ
 * @date     2018年10月11日 下午6:01:22
 */
public class TestService extends SelectService {

	
	public boolean copy(String oldKey, Integer type, String newKey) {
		TestSystemSetting coupleSetTing = null;
		TestSystemSettingFunc functionSetTing = null;
		switch (type) {
		case 0:
			functionSetTing = TestSystemSettingFunc.dao.use("db1").findById(oldKey);
			functionSetTing.setSoftWare(newKey);
			functionSetTing.setRecordTime(new Date());
			return functionSetTing.use("db1").save();
		case 1:
			functionSetTing = TestSystemSettingFunc.dao.use("db2").findById(oldKey);
			functionSetTing.setSoftWare(newKey);
			functionSetTing.setRecordTime(new Date());
			return functionSetTing.use("db2").save();
		case 2:
			coupleSetTing = TestSystemSetting.dao.use("db2").findById(oldKey);
			coupleSetTing.setSoftWare(newKey);
			coupleSetTing.setRecordTime(new Date());
			return coupleSetTing.use("db2").save();
		case 3:
			functionSetTing = TestSystemSettingFunc.dao.use("db3").findById(oldKey);
			functionSetTing.setSoftWare(newKey);
			functionSetTing.setRecordTime(new Date());
			return functionSetTing.use("db3").save();
		case 4:
			coupleSetTing = TestSystemSetting.dao.use("db3").findById(oldKey);
			coupleSetTing.setSoftWare(newKey);
			coupleSetTing.setRecordTime(new Date());
			return coupleSetTing.use("db3").save();
		default:
			return false;
		}
	}

	
	public boolean create(TestSystemSetting coupleSetTing, TestSystemSettingFunc functionSetTing, Integer type) {
		TestSystemSettingFunc functionTestSetTing = null;
		TestSystemSetting coupleTestSetTing = null;
		switch (type) {
		case 0:
			if(functionSetTing.getSoftWare() == null || functionSetTing.getMachineName() == null || functionSetTing.getStation() == null) {
				throw new ParameterException("only create the setTing have the SoftWare、MachineName and Station");
			}
			functionTestSetTing = TestSystemSettingFunc.dao.use("db1").findById(functionSetTing.getSoftWare());
			if (functionTestSetTing != null) {
				throw new ParameterException("only create the setTing once");
			}		
			return functionSetTing.use("db1").save();
		case 1:
			if(functionSetTing.getSoftWare() == null || functionSetTing.getMachineName() == null || functionSetTing.getStation() == null) {
				throw new ParameterException("only create the setTing have the SoftWare、MachineName and Station");
			}
			functionTestSetTing = TestSystemSettingFunc.dao.use("db2").findById(functionSetTing.getSoftWare());
			if (functionTestSetTing != null) {
				throw new ParameterException("only create the setTing once");
			}			
			return functionSetTing.use("db2").save();
		case 2:
			if(coupleSetTing.getSoftWare() == null || coupleSetTing.getMachineName() == null || coupleSetTing.getStation() == null) {
				throw new ParameterException("only create the setTing have the SoftWare、MachineName and Station");
			}
			coupleTestSetTing = TestSystemSetting.dao.use("db2").findById(coupleSetTing.getSoftWare());
			if (coupleTestSetTing != null) {
				throw new ParameterException("only create the setTing once");
			}		
			return coupleSetTing.use("db2").save();
		case 3:
			if(functionSetTing.getSoftWare() == null || functionSetTing.getMachineName() == null || functionSetTing.getStation() == null) {
				throw new ParameterException("only create the setTing have the SoftWare、MachineName and Station");
			}
			functionTestSetTing = TestSystemSettingFunc.dao.use("db3").findById(functionSetTing.getSoftWare());
			if (functionTestSetTing != null) {
				throw new ParameterException("only create the setTing once");
			}
			return functionSetTing.use("db3").save();
		case 4:
			if(coupleSetTing.getSoftWare() == null || coupleSetTing.getMachineName() == null || coupleSetTing.getStation() == null) {
				throw new ParameterException("only create the setTing have the SoftWare、MachineName and Station");
			}
			coupleTestSetTing = TestSystemSetting.dao.use("db3").findById(coupleSetTing.getSoftWare());
			if (coupleTestSetTing != null) {
				throw new ParameterException("only create the setTing once");
			}
			return coupleSetTing.use("db3").save();
		default:
			return false;
		}
	}

	
	public boolean cancel(String key, Integer type) {
		TestSystemSetting coupleSetTing = null;
		TestSystemSettingFunc functionSetTing = null;
		switch (type) {
		case 0:
			functionSetTing = TestSystemSettingFunc.dao.use("db1").findById(key);
			return functionSetTing.use("db1").delete();
		case 1:
			functionSetTing = TestSystemSettingFunc.dao.use("db2").findById(key);
			return functionSetTing.use("db2").delete();
		case 2:
			coupleSetTing = TestSystemSetting.dao.use("db2").findById(key);
			return coupleSetTing.use("db2").delete();
		case 3:
			functionSetTing = TestSystemSettingFunc.dao.use("db3").findById(key);
			return functionSetTing.use("db3").delete();
		case 4:
			coupleSetTing = TestSystemSetting.dao.use("db3").findById(key);
			return coupleSetTing.use("db3").delete();
		default:
			return false;
		}
	}

	
	public boolean update(TestSystemSetting coupleSetTing, TestSystemSettingFunc functionSetTing, Integer type) {
		TestSystemSettingFunc functionTestSetTing = null;
		TestSystemSetting coupleTestSetTing = null;
		switch (type) {
		case 0:
			if(functionSetTing.getSoftWare() == null || functionSetTing.getMachineName() == null || functionSetTing.getStation() == null) {
				throw new ParameterException("only update the setTing have the SoftWare、MachineName and Station");
			}
			functionTestSetTing = TestSystemSettingFunc.dao.use("db1").findById(functionSetTing.getSoftWare());
			if (functionTestSetTing == null) {
				throw new ParameterException("only update the setTing exist");
			}
			return functionSetTing.use("db1").update();
		case 1:
			if(functionSetTing.getSoftWare() == null || functionSetTing.getMachineName() == null || functionSetTing.getStation() == null) {
				throw new ParameterException("only update the setTing have the SoftWare、MachineName and Station");
			}
			functionTestSetTing = TestSystemSettingFunc.dao.use("db2").findById(functionSetTing.getSoftWare());
			if (functionTestSetTing == null) {
				throw new ParameterException("only update the setTing exist");
			}
			return functionSetTing.use("db2").update();
		case 2:
			if(coupleSetTing.getSoftWare() == null || coupleSetTing.getMachineName() == null || coupleSetTing.getStation() == null) {
				throw new ParameterException("only update the setTing have the SoftWare、MachineName and Station");
			}
			coupleTestSetTing = TestSystemSetting.dao.use("db2").findById(coupleSetTing.getSoftWare());
			if (coupleTestSetTing != null) {
				throw new ParameterException("only update the setTing exist");
			}
			return coupleSetTing.use("db2").update();
		case 3:
			if(functionSetTing.getSoftWare() == null || functionSetTing.getMachineName() == null || functionSetTing.getStation() == null) {
				throw new ParameterException("only update the setTing have the SoftWare、MachineName and Station");
			}
			functionTestSetTing = TestSystemSettingFunc.dao.use("db3").findById(functionSetTing.getSoftWare());
			if (functionTestSetTing != null) {
				throw new ParameterException("only update the setTing exist");
			}
			return functionSetTing.use("db3").update();
		case 4:
			if(coupleSetTing.getSoftWare() == null || coupleSetTing.getMachineName() == null || coupleSetTing.getStation() == null) {
				throw new ParameterException("only update the setTing have the SoftWare、MachineName and Station");
			}
			coupleTestSetTing = TestSystemSetting.dao.use("db3").findById(coupleSetTing.getSoftWare());
			if (coupleTestSetTing != null) {
				throw new ParameterException("only update the setTing exist");
			}
			return coupleSetTing.use("db3").update();
		default:
			return false;
		}
	}
}
