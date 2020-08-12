package com.jimi.mes_server.controller;

import java.io.File;

import javax.servlet.http.HttpServletResponse;

import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jfinal.core.paragetter.Para;
import com.jfinal.upload.UploadFile;
import com.jimi.mes_server.annotation.PermissionPass;
import com.jimi.mes_server.annotation.TestLog;
import com.jimi.mes_server.entity.vo.LUserAccountVO;
import com.jimi.mes_server.exception.OperationException;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.model.TestSystemSetting;
import com.jimi.mes_server.model.TestSystemSettingFunc;
import com.jimi.mes_server.model.TestSystemSettingOqc;
import com.jimi.mes_server.service.TestService;
import com.jimi.mes_server.util.ResultUtil;
import com.jimi.mes_server.util.TokenBox;

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


	public void copy(String oldKey, Integer type, String newKey) {
		LUserAccountVO user = TokenBox.get(getPara(TokenBox.TOKEN_ID_KEY_NAME), UserController.SESSION_KEY_LOGIN_USER);
		if (testService.copy(user.getName(),oldKey, type, newKey)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@TestLog
	public void create(@Para("") TestSystemSetting coupleSetTing, @Para("") TestSystemSettingFunc functionSetTing, @Para("") TestSystemSettingOqc oqcSetTing,String softVersion,String orderName, Integer type) {
		if (testService.create(coupleSetTing, functionSetTing, oqcSetTing,type)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	public void cancel(String key, Integer type) {
		LUserAccountVO user = TokenBox.get(getPara(TokenBox.TOKEN_ID_KEY_NAME), UserController.SESSION_KEY_LOGIN_USER);
		if (testService.cancel(user.getName(),key, type)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}

	@TestLog
	public void update(@Para("") TestSystemSetting coupleSetTing, @Para("") TestSystemSettingFunc functionSetTing, @Para("") TestSystemSettingOqc oqcSetTing, Integer type) {
		if (testService.update(coupleSetTing, functionSetTing, oqcSetTing, type)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}

	/**
	 * 查询测试项配置日志
	 * @param pageNo
	 * @param pageSize
	 * @param startTime
	 * @param endTime
	 * @param operator
	 * @param model
	 * @param settingType
	 * @param softVersion
	 */
	public void getLog(Integer pageNo, Integer pageSize,String startTime, String endTime, String operator, String model, Integer settingType, String softVersion, String orderName, Boolean result){
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		renderJson(ResultUtil.succeed(testService.getLogs(pageNo, pageSize, startTime, endTime, operator, model, settingType, softVersion, orderName, result)));
	}
	
	
	
	public void uploadGuideFile(UploadFile uploadFile, String id, Integer type) {
		try {
			if (uploadFile == null || uploadFile.getFile() == null) {
				throw new ParameterException("文件上传失败，文件不存在");
			}
			if (id == null || type == null) {
				throw new ParameterException("文件上传失败，参数不能为空");
			}
			testService.uploadGuideFile(uploadFile.getFile(), id, type);
			renderJson(ResultUtil.succeed());
		} finally {
			if (uploadFile != null && uploadFile.getFile() != null) {
				uploadFile.getFile().delete();
			}
		}
	}
	
	
	public void downloadGuideFile(String id, Integer type) {
		if (id == null || type == null) {
			throw new OperationException("文件下载失败，参数不能为空");
		}
		try {
			HttpServletResponse response = getResponse();
			File guideFile = testService.downloadGuideFile(id, type);
			response.addHeader("Access-Control-Expose-Headers", "Content-disposition");
			renderFile(guideFile, guideFile.getName());
		} catch (Exception e) {
			e.printStackTrace();
			renderNull();
		}
	}
	
	
	@PermissionPass
	public void initSBData() {
		
		testService.initSBData();
		renderJson(ResultUtil.succeed());
	}

}
