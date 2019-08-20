package com.jimi.mes_server.service;

import java.util.Date;

import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.model.TestSystemSetting;
import com.jimi.mes_server.model.TestSystemSettingFunc;
import com.jimi.mes_server.model.TestSystemSettingOqc;
import com.jimi.mes_server.service.base.SelectService;

/**测试配置项业务层
 * @author   HCJ
 * @date     2018年10月11日 下午6:01:22
 */
public class TestService extends SelectService {

	public boolean copy(String oldKey, Integer type, String newKey) {
		TestSystemSetting coupleSetTing = null;
		TestSystemSettingFunc functionSetTing = null;
		TestSystemSettingOqc oqcSetTing = null;
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
		case 5:
			oqcSetTing = TestSystemSettingOqc.dao.use("db2").findById(oldKey);
			oqcSetTing.setSoftWare(newKey);
			oqcSetTing.setRecordTime(new Date());
			return oqcSetTing.use("db2").save();
		default:
			return false;
		}
	}


	public boolean create(TestSystemSetting coupleSetTing, TestSystemSettingFunc functionSetTing, TestSystemSettingOqc oqcSetTing, Integer type) {
		TestSystemSettingFunc functionTestSetTing = null;
		TestSystemSetting coupleTestSetTing = null;
		TestSystemSettingOqc oqcTestSetTing = null;
		switch (type) {
		case 0:
			if (functionSetTing.getSoftWare() == null || functionSetTing.getMachineName() == null || functionSetTing.getStation() == null) {
				throw new ParameterException("机型名、版本号和配置不能为空");
			}
			functionTestSetTing = TestSystemSettingFunc.dao.use("db1").findById(functionSetTing.getSoftWare());
			if (functionTestSetTing != null) {
				throw new ParameterException("已存在此配置");
			}
			return functionSetTing.use("db1").save();
		case 1:
			if (functionSetTing.getSoftWare() == null || functionSetTing.getMachineName() == null || functionSetTing.getStation() == null) {
				throw new ParameterException("机型名、版本号和配置不能为空");
			}
			functionTestSetTing = TestSystemSettingFunc.dao.use("db2").findById(functionSetTing.getSoftWare());
			if (functionTestSetTing != null) {
				throw new ParameterException("已存在此配置");
			}
			return functionSetTing.use("db2").save();
		case 2:
			if (coupleSetTing.getSoftWare() == null || coupleSetTing.getMachineName() == null || coupleSetTing.getStation() == null) {
				throw new ParameterException("机型名、版本号和配置不能为空");
			}
			coupleTestSetTing = TestSystemSetting.dao.use("db2").findById(coupleSetTing.getSoftWare());
			if (coupleTestSetTing != null) {
				throw new ParameterException("已存在此配置");
			}
			return coupleSetTing.use("db2").save();
		case 3:
			if (functionSetTing.getSoftWare() == null || functionSetTing.getMachineName() == null || functionSetTing.getStation() == null) {
				throw new ParameterException("机型名、版本号和配置不能为空");
			}
			functionTestSetTing = TestSystemSettingFunc.dao.use("db3").findById(functionSetTing.getSoftWare());
			if (functionTestSetTing != null) {
				throw new ParameterException("已存在此配置");
			}
			return functionSetTing.use("db3").save();
		case 4:
			if (coupleSetTing.getSoftWare() == null || coupleSetTing.getMachineName() == null || coupleSetTing.getStation() == null) {
				throw new ParameterException("机型名、版本号和配置不能为空");
			}
			coupleTestSetTing = TestSystemSetting.dao.use("db3").findById(coupleSetTing.getSoftWare());
			if (coupleTestSetTing != null) {
				throw new ParameterException("已存在此配置");
			}
			return coupleSetTing.use("db3").save();
		case 5:
			if (oqcSetTing.getSoftWare() == null || oqcSetTing.getMachineName() == null || oqcSetTing.getStation() == null) {
				throw new ParameterException("机型名、版本号和配置不能为空");
			}
			oqcTestSetTing = TestSystemSettingOqc.dao.use("db2").findById(oqcSetTing.getSoftWare());
			if (oqcTestSetTing != null) {
				throw new ParameterException("已存在此配置");
			}
			return oqcSetTing.use("db2").save();
		default:
			return false;
		}
	}


	public boolean cancel(String key, Integer type) {
		TestSystemSetting coupleSetTing = null;
		TestSystemSettingFunc functionSetTing = null;
		TestSystemSettingOqc oqcSetTing = null;
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
		case 5:
			oqcSetTing = TestSystemSettingOqc.dao.use("db2").findById(key);
			return oqcSetTing.use("db2").delete();
		default:
			return false;
		}
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
}
