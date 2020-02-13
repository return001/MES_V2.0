package com.jimi.mes_server.controller;

import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jfinal.core.paragetter.Para;
import com.jimi.mes_server.annotation.Access;
import com.jimi.mes_server.model.TestSystemSetting;
import com.jimi.mes_server.model.TestSystemSettingFunc;
import com.jimi.mes_server.model.TestSystemSettingOqc;
import com.jimi.mes_server.service.TestService;
import com.jimi.mes_server.util.ResultUtil;

/**
 * 测试配置控制器
 * 
 * @author HCJ
 * @date 2018年9月28日 下午2:59:00
 */
public class TestController extends Controller {

	private static TestService testService = Enhancer.enhance(TestService.class);

	public static final String FUNCTION_TEST_TABLE_NAME = "TestSystemSettingFunc";

	public static final String COUPLE_TEST_TABLE_NAME = "TestSystemSetting";

	public static final String OQC_TEST_TABLE_NAME = "TestSystemSettingOqc";


	@Access({ "operator", "engineer", "oqcManager", "configurationManager", "developConfigurator" })
	public void select(Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter, Integer type) {
		if (type != null) {
			if (type.equals(0) || type.equals(1) || type.equals(3)) {
				renderJson(ResultUtil.succeed(testService.select(FUNCTION_TEST_TABLE_NAME, pageNo, pageSize, ascBy, descBy, filter, type)));
			} else if (type.equals(2) || type.equals(4)) {
				renderJson(ResultUtil.succeed(testService.select(COUPLE_TEST_TABLE_NAME, pageNo, pageSize, ascBy, descBy, filter, type)));
			} else if (type.equals(5)) {
				renderJson(ResultUtil.succeed(testService.select(OQC_TEST_TABLE_NAME, pageNo, pageSize, ascBy, descBy, filter, type)));
			} else {
				renderJson(ResultUtil.failed(400, "此类型无法识别"));
			}
		} else {
			renderJson(ResultUtil.failed(400, "类型不能为空"));
		}
	}


	@Access({ "engineer", "oqcManager", "configurationManager", "developConfigurator" })
	public void copy(String oldKey, Integer type, String newKey) {
		if (testService.copy(oldKey, type, newKey)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "engineer", "oqcManager", "configurationManager", "developConfigurator" })
	public void create(@Para("") TestSystemSetting coupleSetTing, @Para("") TestSystemSettingFunc functionSetTing, @Para("") TestSystemSettingOqc oqcSetTing, Integer type) {
		if (testService.create(coupleSetTing, functionSetTing, oqcSetTing, type)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "engineer", "oqcManager", "configurationManager", "developConfigurator" })
	public void cancel(String key, Integer type) {
		if (testService.cancel(key, type)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "engineer", "oqcManager", "configurationManager", "developConfigurator" })
	public void update(@Para("") TestSystemSetting coupleSetTing, @Para("") TestSystemSettingFunc functionSetTing, @Para("") TestSystemSettingOqc oqcSetTing, Integer type) {
		if (testService.update(coupleSetTing, functionSetTing, oqcSetTing, type)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}
}
