package com.jimi.mes_server.controller;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.servlet.ServletOutputStream;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang3.StringUtils;

import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jfinal.core.paragetter.Para;
import com.jfinal.kit.StrKit;
import com.jfinal.plugin.activerecord.Page;
import com.jfinal.plugin.activerecord.Record;
import com.jfinal.upload.UploadFile;
import com.jimi.mes_server.annotation.Access;
import com.jimi.mes_server.entity.Constant;
import com.jimi.mes_server.entity.PlanQueryCriteria;
import com.jimi.mes_server.entity.vo.LUserAccountVO;
import com.jimi.mes_server.exception.OperationException;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.model.Orders;
import com.jimi.mes_server.service.ProductionService;
import com.jimi.mes_server.util.ResultUtil;
import com.jimi.mes_server.util.TokenBox;

public class ProductionController extends Controller {

	private static ProductionService productionService = Enhancer.enhance(ProductionService.class);


	@Access({ "schedulingSZPC", "schedulingJMPMC", "engineer", "operator" })
	public void getProcessGroup() {
		renderJson(ResultUtil.succeed(productionService.getProcessGroup()));
	}


	@Access({ "schedulingSZPC", "schedulingJMPMC", "engineer", "operator" })
	public void getProcess() {
		renderJson(ResultUtil.succeed(productionService.getProcess()));
	}


	@Access({ "schedulingSZPC", "schedulingJMPMC", "engineer", "operator" })
	public void getLine() {
		renderJson(ResultUtil.succeed(productionService.getLine()));
	}


	@Access({ "schedulingSZPC", "schedulingJMPMC", "engineer", "operator" })
	public void getOrderStatus() {
		renderJson(ResultUtil.succeed(productionService.getOrderStatus()));
	}


	@Access({ "schedulingSZPC", "schedulingJMPMC", "engineer", "operator" })
	public void getSchedulingPlanStatus() {
		renderJson(ResultUtil.succeed(productionService.getSchedulingPlanStatus()));
	}


	@Access({ "engineer" })
	public void addProcessGroup(String groupNo, String groupName, String groupRemark) {
		if (StringUtils.isAnyBlank(groupNo, groupName)) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.addProcessGroup(groupNo, groupName, groupRemark)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "engineer" })
	public void deleteProcessGroup(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.deleteProcessGroup(id)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "engineer", "operator" })
	public void selectProcessGroup(String filter) {
		ResultUtil result = ResultUtil.succeed(productionService.selectProcessGroup(filter));
		renderJson(result);
	}


	@Access({ "engineer" })
	public void editProcessGroup(Integer id, String groupNo, String groupName, String groupRemark) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.editProcessGroup(id, groupNo, groupName, groupRemark)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "engineer" })
	public void addLine(String lineNo, String lineName, String lineRemark, Integer lineDirector, Integer lineEngineer,
			Integer lineQc, Integer processGroup) {
		if (StringUtils.isAnyBlank(lineNo, lineName)) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.addLine(lineNo, lineName, lineRemark, lineDirector, lineEngineer, lineQc, processGroup)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "engineer" })
	public void deleteLine(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.deleteLine(id)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "engineer", "operator" })
	public void selectLine(String lineNo, String lineName, Integer processGroup) {
		renderJson(ResultUtil.succeed(productionService.selectLine(lineNo, lineName, processGroup)));
	}


	@Access({ "engineer" })
	public void editLine(Integer id, String lineNo, String lineName, String lineRemark, Integer lineDirector,
			Integer lineEngineer, Integer lineQc, Integer processGroup) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.editLine(id, lineNo, lineName, lineRemark, lineDirector, lineEngineer, lineQc,
				processGroup)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "engineer" })
	public void addProcess(String processNo, String processName, String processRemark, Integer processGroup) {
		if (StringUtils.isAnyBlank(processNo, processName)) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.addProcess(processNo, processName, processRemark, processGroup)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "engineer" })
	public void deleteProcess(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.deleteProcess(id)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "engineer", "operator" })
	public void selectProcess(String processNo, String processName, Integer processGroup) {
		renderJson(ResultUtil.succeed(productionService.selectProcess(processNo, processName, processGroup)));
	}


	@Access({ "engineer" })
	public void editProcess(Integer id, String processNo, String processName, String processRemark, Integer processGroup) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.editProcess(id, processNo, processName, processRemark, processGroup)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "engineer" })
	public void addComputer(String ip, String computerName, String remark, Integer line) {
		if (line == null || StrKit.isBlank(ip)) {
			throw new ParameterException("参数不能为空");
		}
		if (!validateIP(ip)) {
			throw new ParameterException("无效的IP地址");
		}
		if (productionService.addComputer(ip, computerName, remark, line)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "engineer" })
	public void deleteComputer(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.deleteComputer(id)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "engineer", "operator" })
	public void selectComputer(Integer lineId) {
		if (lineId == null) {
			throw new ParameterException("参数不能为空");
		}
		renderJson(ResultUtil.succeed(productionService.selectComputer(lineId)));
	}


	@Access({ "engineer" })
	public void editComputer(Integer id, String ip, String computerName, String remark) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (!StrKit.isBlank(ip) && !validateIP(ip)) {
			throw new ParameterException("无效的IP地址");
		}
		if (productionService.editComputer(id, ip, computerName, remark)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "engineer" })
	public void addCapacity(String softModel, String customerModel, Integer process, Integer processGroup, Integer processPeopleQuantity, Integer capacity, String remark) {
		if (StrKit.isBlank(softModel) || process == null || processGroup == null || processPeopleQuantity == null || capacity == null) {
			throw new ParameterException("参数不能为空");
		}
		if (processPeopleQuantity < 0 || capacity < 0) {
			throw new ParameterException("产能或人数不合理");
		}
		if (productionService.addCapacity(softModel, customerModel, process, processGroup, processPeopleQuantity, capacity, remark)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "engineer" })
	public void deleteCapacity(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.deleteCapacity(id)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "engineer", "operator" })
	public void selectCapacity(Integer pageNo, Integer pageSize, String softModel, String customerModel, Integer process) {
		renderJson(ResultUtil.succeed(productionService.selectCapacity(pageNo, pageSize, softModel, customerModel, process)));
	}


	@Access({ "engineer" })
	public void editCapacity(Integer id, String softModel, String customerModel, Integer process, Integer processGroup, Integer processPeopleQuantity, Integer capacity, String remark, Integer position) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.editCapacity(id, softModel, customerModel, process, processGroup, processPeopleQuantity,
				capacity, remark, position)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "schedulingSZPC" })
	public void addOrder(@Para("") Orders order) {
		if (order == null) {
			throw new ParameterException("参数不能为空");
		}
		boolean isOrderInformationExist = StringUtils.isAnyBlank(order.getZhidan(), order.getSoftModel());
		if (isOrderInformationExist || order.getQuantity() == null || order.getIsRework() == null) {
			throw new ParameterException("订单号、机型、数量和是否为返工订单不能为空");
		}
		if (order.getQuantity() < 0) {
			throw new ParameterException("订单数量格式错误");
		}
		if (order.getOrderDate().after(order.getDeliveryDate())) {
			throw new ParameterException("订单日期与交货日期冲突");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		if (productionService.addOrder(order, userVO)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "schedulingSZPC" })
	public void deleteOrder(Integer id, String deleteReason) {
		if (id == null || StrKit.isBlank(deleteReason)) {
			throw new ParameterException("参数不能为空");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		if (productionService.deleteOrder(id, deleteReason, userVO)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "schedulingSZPC", "schedulingJMPMC", "engineer", "operator" })
	public void selectOrder(Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter, Boolean isRework) {
		if (isRework == null) {
			throw new ParameterException("是否为返工订单的参数不能为空");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		renderJson(ResultUtil.succeed(productionService.selectOrder(pageNo, pageSize, ascBy, descBy, filter, isRework,userVO)));
	}


	@Access({ "schedulingSZPC", "schedulingJMPMC", "engineer", "operator" })
	public void selectOrderDetail(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		renderJson(ResultUtil.succeed(productionService.selectOrderDetail(id)));
	}


	@Access({ "schedulingSZPC" })
	public void editOrder(@Para("") Orders order) {
		if (order == null) {
			throw new ParameterException("参数不能为空");
		}
		boolean isOrderInformationExist = StringUtils.isAnyBlank(order.getZhidan(), order.getSoftModel());
		if (isOrderInformationExist || order.getQuantity() == null) {
			throw new ParameterException("订单号、机型和数量不能为空");
		}
		if (order.getQuantity() < 0) {
			throw new ParameterException("订单数量格式错误");
		}
		if (order.getOrderDate().after(order.getDeliveryDate())) {
			throw new ParameterException("订单日期与交货日期冲突");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		if (productionService.editOrder(order, userVO)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "schedulingSZPC" })
	public void importOrder(UploadFile uploadFile, Boolean isRework) {
		uploadFile = getFile();
		if (uploadFile == null || isRework == null) {
			throw new ParameterException("参数不能为空");
		}
		if (!uploadFile.getOriginalFileName().endsWith(".xls") && !uploadFile.getOriginalFileName().endsWith(".xlsx")) {
			throw new ParameterException("只能上传Excel文件");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		String result = productionService.importOrder(uploadFile.getFile(), userVO, isRework);
		if ("导入成功".equals(result)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed(result));
		}
	}


	@Access({ "schedulingSZPC", "engineer" })
	public void importOrderTable() {
		List<UploadFile> uploadFiles = getFiles();
		if (uploadFiles == null || uploadFiles.isEmpty()) {
			throw new ParameterException("请添加文件");
		}
		for (UploadFile uploadFile : uploadFiles) {
			if (!uploadFile.getOriginalFileName().endsWith(".xls") && !uploadFile.getOriginalFileName().endsWith(".xlsx")) {
				throw new ParameterException("只能上传Excel文件");
			}
		}
		Integer type;
		Integer id;
		try {
			type = getParaToInt("type");
			id = getParaToInt("id");
		} catch (Exception e) {
			throw new ParameterException("请输入正确格式的参数");
		}
		if (type == null || id == null) {
			throw new ParameterException("参数不能为空");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		if (Constant.SCHEDULING_SZPC_USERTYPE.equals(userVO.getWebUserType()) && type.equals(2)) {
			throw new OperationException("深圳PC无法上传SOP表");
		}
		if (Constant.ENGINEER_USERTYPE.equals(userVO.getWebUserType()) && !type.equals(2)) {
			throw new OperationException("工程及生产只能上传SOP表");
		}
		if (productionService.importOrderTable(uploadFiles, type, id, userVO)) {
			renderJson(ResultUtil.succeed());
		}
	}


	@Access({ "schedulingSZPC", "engineer", "schedulingJMPMC" })
	public void downloadOrderTable(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		File file = productionService.downloadOrderTable(id);
		HttpServletResponse response = getResponse();
		try {
			response.setHeader("Content-Disposition", "attachment; filename=" + URLEncoder.encode(file.getName(), "utf-8").replaceAll("%C2%A0", "%20"));
		} catch (UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		response.addHeader("Access-Control-Expose-Headers", "Content-Disposition");
		if(file.getName().contains(".xlsx")) {
			response.setContentType("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
		}else if(file.getName().contains(".xls")) {
			response.setContentType("application/vnd.ms-excel");
		}
		try(ServletOutputStream os = response.getOutputStream();FileInputStream input = new FileInputStream(file);) {
			byte[] buffer = new byte[1024];
			int i = 0;
			while( (i = input.read(buffer, 0, 1024)) != -1) {
				os.write(buffer, 0, i);
			}
			os.flush();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		renderNull();
	}


	@Access({ "schedulingJMPMC" })
	public void selectUnscheduledPlan(Integer type) {
		if (type == null) {
			throw new ParameterException("参数不能为空");
		}
		
			switch (type) {
			case 0:
			case 1:
			case 2:
				break;
			default:
				throw new OperationException("无法识别的类型");
			}
		
		renderJson(ResultUtil.succeed(productionService.selectUnscheduledPlan(type)));
	}

	
	@Access({ "schedulingJMPMC" })
	public void selectReworkPlan(Integer type) {
		if (type == null) {
			throw new ParameterException("参数不能为空");
		}
		
			switch (type) {
			case 0:
			case 1:
			case 2:
				break;
			default:
				throw new OperationException("无法识别的类型");
			}
		
		renderJson(ResultUtil.succeed(productionService.selectReworkPlan(type)));
	}


	@Access({ "schedulingJMPMC" })
	public void addPlan(Integer order, String remark, String schedulingQuantity, String line, Integer processGroup, String capacity) {
		if (order == null || processGroup == null || StringUtils.isAnyBlank(schedulingQuantity,line,capacity)) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.addPlan(order, remark, schedulingQuantity, line, processGroup, capacity)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "schedulingJMPMC" })
	public void deletePlan(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.deletePlan(id)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/*public void selectPlan(Integer pageNo, Integer pageSize, Integer schedulingPlanStatus, String zhidan,
			String customerName, String orderDateFrom, String orderDateTo, String planStartTimeFrom,
			String planStartTimeTo, String planCompleteTimeFrom, String planCompleteTimeTo, String startTimeFrom,
			String startTimeTo, String completeTimeFrom, String completeTimeTo, Integer processGroup, Integer line,
			String productionPlanningNumber, String softModel, String productNo) {
		renderJson(ResultUtil.succeed(productionService.selectPlan(pageNo, pageSize, schedulingPlanStatus, zhidan,
				customerName, orderDateFrom, orderDateTo, planStartTimeFrom, planStartTimeTo, planCompleteTimeFrom,
				planCompleteTimeTo, startTimeFrom, startTimeTo, completeTimeFrom, completeTimeTo, processGroup, line,
				productionPlanningNumber, softModel, productNo)));
	}*/
	
	@Access({"schedulingJMPMC", "engineer", "operator" })
	public void selectPlan(@Para("") PlanQueryCriteria planQueryCriteria) {
		renderJson(ResultUtil.succeed(productionService.selectPlan(planQueryCriteria)));
	}

	@Access({"schedulingJMPMC", "engineer", "operator" })
	public void selectPlanDetail(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		ResultUtil result = ResultUtil.succeed(productionService.selectPlanDetail(id));
		renderJson(result);
	}


	@Access({"schedulingJMPMC", "engineer"})
	public void editPlan(Integer id, Boolean isUrgent, String remark, Integer schedulingQuantity, Integer line,
			String planStartTime, String planCompleteTime, String lineChangeTime, Integer capacity, Boolean isCompleted,
			Integer producedQuantity, String remainingReason, String productionPlanningNumber, Boolean isStarting) {
		if (StringUtils.isAnyBlank(planStartTime, planCompleteTime, lineChangeTime)
				|| id == null && schedulingQuantity == null || capacity == null || line == null || isCompleted == null
				|| isUrgent == null || isStarting == null) {
			throw new ParameterException("参数不能为空");
		}
		if (schedulingQuantity < 0 || capacity < 0) {
			throw new ParameterException("排产数量或产能不合理");
		}
		if (producedQuantity != null && producedQuantity < 0) {
			throw new ParameterException("已生产数量不合理");
		}
		if (!isStarting && isCompleted) {
			throw new ParameterException("未开始生产的计划无法完成");
		}
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		Date start;
		Date end;
		try {
			start = dateFormat.parse(planStartTime);
			end = dateFormat.parse(planCompleteTime);
		} catch (ParseException e) {
			throw new ParameterException("时间格式出错");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		if (productionService.editPlan(id, isUrgent, remark, schedulingQuantity, line, start, end, lineChangeTime,
				capacity, isCompleted, producedQuantity, remainingReason, productionPlanningNumber, isStarting,
				userVO)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({"schedulingJMPMC", "engineer"})
	public void editPlanStatus(Integer id, Integer type) {
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		if (id == null || type == null) {
			throw new ParameterException("参数不能为空");
		}
		switch (type) {
		case 0:
		case 1:
		case 2:
		case 3:
			break;
		default:
			throw new ParameterException("无法识别的类型");
		}
		String typeName = userVO.getTypeName();
		if ("engineer".equals(typeName)&&type.equals(Constant.WAIT_NOTIFICATION_PLANSTATUS-1)) {
			throw new ParameterException("工程只能修改计划状态是否为开始或完成");
		}
		if ("schedulingJMPMC".equals(typeName)&&!type.equals(Constant.WAIT_NOTIFICATION_PLANSTATUS-1)) {
			throw new ParameterException("PMC只能修改计划状态是否为待通知");
		}
		
		if (productionService.editPlanStatus(id, type, userVO)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({"schedulingJMPMC" })
	public void selectPlanProducedQuantity(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		ResultUtil result = ResultUtil.succeed(productionService.selectPlanProducedQuantity(id));
		renderJson(result);
	}

	/*public void exportPlan(Integer schedulingPlanStatus, String zhidan, String customerName, String orderDateFrom,
			String orderDateTo, String planStartTimeFrom, String planStartTimeTo, String planCompleteTimeFrom,
			String planCompleteTimeTo, String startTimeFrom, String startTimeTo, String completeTimeFrom,
			String completeTimeTo, Integer processGroup, Integer line, String productionPlanningNumber,
			String softModel, String productNo) {
		OutputStream output = null;
		Page<Record> page = productionService.selectPlan(Constant.DEFAULT_PAGE_NUM, Constant.DEFAULT_PAGE_SIZE,
				schedulingPlanStatus, zhidan, customerName, orderDateFrom, orderDateTo, planStartTimeFrom,
				planStartTimeTo, planCompleteTimeFrom, planCompleteTimeTo, startTimeFrom, startTimeTo, completeTimeFrom,
				completeTimeTo, processGroup, line, productionPlanningNumber, softModel, productNo);
		try {
			// 设置响应
			HttpServletResponse response = getResponse();
			output = response.getOutputStream();
			productionService.exportPlan(page, response, output);
		} catch (Exception e) {
			renderJson(ResultUtil.failed());
		} finally {
			try {
				if (output != null) {
					output.close();
				}
			} catch (IOException e) {
				renderJson(ResultUtil.failed());
			}
		}
		renderNull();
	}*/


	@Access({ "schedulingJMPMC" })
	public void getPlanGannt(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		renderJson(ResultUtil.succeed(productionService.getPlanGannt(id)));
	}


	@Access({"schedulingJMPMC" })
	public void exportPlan(@Para("") PlanQueryCriteria planQueryCriteria) {
		OutputStream output = null;
		planQueryCriteria.setPageNo(Constant.DEFAULT_PAGE_NUM);
		planQueryCriteria.setPageSize(Constant.DEFAULT_PAGE_SIZE);
		Page<Record> page = productionService.selectPlan(planQueryCriteria);
		try {
			// 设置响应
			HttpServletResponse response = getResponse();
			output = response.getOutputStream();
			productionService.exportPlan(page, response, output);
		} catch (Exception e) {
			renderJson(ResultUtil.failed());
		} finally {
			try {
				if (output != null) {
					output.close();
				}
			} catch (IOException e) {
				renderJson(ResultUtil.failed());
			}
		}
		renderNull();
	}


	@Access({"schedulingJMPMC" })
	public void checkCompleteTime(Integer schedulingQuantity, String planStartTime, String planCompleteTime, String lineChangeTime, Integer capacity) {
		if (schedulingQuantity == null || planStartTime == null || planCompleteTime == null || capacity == null) {
			throw new ParameterException("参数不能为空");
		}
		if (schedulingQuantity < 0 || capacity < 0) {
			throw new ParameterException("排产数量或产能不合理");
		}
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		Date start;
		Date end;
		try {
			start = dateFormat.parse(planStartTime);
			end = dateFormat.parse(planCompleteTime);
		} catch (Exception e) {
			throw new ParameterException("时间格式出错");
		}
		if (productionService.checkCompleteTime(schedulingQuantity, start, end, lineChangeTime, capacity)) {
			renderJson(ResultUtil.succeed("计划时间内能够完成任务"));
		} else {
			renderJson(ResultUtil.failed("计划时间内无法完成任务"));
		}
	}


	public static boolean validateIP(String ipAddress) {
		String regex = "([1-9]|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])(\\.(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])){3}";
		Pattern pattern = Pattern.compile(regex);
		Matcher matcher = pattern.matcher(ipAddress);
		return matcher.matches();
	}
}
