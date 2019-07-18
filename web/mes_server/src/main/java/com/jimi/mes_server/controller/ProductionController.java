package com.jimi.mes_server.controller;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.commons.lang3.StringUtils;

import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jfinal.core.paragetter.Para;
import com.jfinal.kit.StrKit;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Record;
import com.jfinal.upload.UploadFile;
import com.jimi.mes_server.entity.DailyOrderProduction;
import com.jimi.mes_server.entity.DailyReport;
import com.jimi.mes_server.entity.Production;
import com.jimi.mes_server.entity.SQL;
import com.jimi.mes_server.entity.vo.LUserAccountVO;
import com.jimi.mes_server.exception.OperationException;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.model.DailyCompletion;
import com.jimi.mes_server.model.Orders;
import com.jimi.mes_server.service.ProductionService;
import com.jimi.mes_server.util.ResultUtil;
import com.jimi.mes_server.util.TokenBox;

public class ProductionController extends Controller {

	private static ProductionService productionService = Enhancer.enhance(ProductionService.class);
	
	public void getProcessGroup() {
		renderJson(ResultUtil.succeed(productionService.getProcessGroup()));
	}
	
	public void getProcess() {
		renderJson(ResultUtil.succeed(productionService.getProcess()));
	}
	
	public void getLine() {
		renderJson(ResultUtil.succeed(productionService.getLine()));
	}

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

	public void selectProcessGroup(String filter) {
		ResultUtil result = ResultUtil.succeed(productionService.selectProcessGroup(filter));
		renderJson(result);
	}

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

	public void addLine(String lineNo, String lineName, String lineRemark, Integer lineDirector, Integer lineEngineer ,Integer lineQc ,Integer processGroup) {
		if (StringUtils.isAnyBlank(lineNo, lineName)) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.addLine(lineNo, lineName, lineRemark, lineDirector,  lineEngineer , lineQc,processGroup)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}

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

	public void selectLine(String lineNo, String lineName, Integer processGroup) {

		renderJson(ResultUtil.succeed(productionService.selectLine(lineNo, lineName, processGroup)));
	}

	public void editLine(Integer id, String lineNo, String lineName, String lineRemark, Integer lineDirector, Integer lineEngineer ,Integer lineQc ,
			Integer processGroup) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.editLine(id, lineNo, lineName, lineRemark, lineDirector,  lineEngineer , lineQc , processGroup)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}

	}

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

	public void selectProcess(String processNo, String processName, Integer processGroup) {

		renderJson(ResultUtil.succeed(productionService.selectProcess(processNo, processName, processGroup)));
	}

	public void editProcess(Integer id, String processNo, String processName, String processRemark,
			Integer processGroup) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.editProcess(id, processNo, processName, processRemark, processGroup)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}

	}
	
	public void addComputer(String ip,String computerName, String remark, Integer line) {
		if (line==null||StrKit.isBlank(ip)) {
			throw new ParameterException("参数不能为空");
		}
		if (!validateIP(ip)) {
			throw new ParameterException("无效的IP地址");
		}
		if (productionService.addComputer( ip, computerName,  remark,  line)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());   
		}
	}
	
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
	
	public void selectComputer(Integer lineId) {
		if (lineId==null) {
			throw new ParameterException("参数不能为空");
		}
		renderJson(ResultUtil.succeed(productionService.selectComputer(lineId)));
	}
	
	public void editComputer(Integer id, String ip,String computerName, String remark) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (!StrKit.isBlank(ip)&&!validateIP(ip)) {
			throw new ParameterException("无效的IP地址");
		}
		if (productionService.editComputer(id, ip, computerName,  remark)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}

	}

	public void addOrder(@Para("") Orders order) {
		if (order == null) {
			throw new ParameterException("参数不能为空");
		}
		boolean isOrderInformationExist = StringUtils.isAnyBlank(order.getZhidan(), order.getSoftModel(),
				order.getVersion());
		if (isOrderInformationExist || order.getQuantity() == null) {
			throw new ParameterException("订单号、机型、版本和数量不能为空");
		}
		if (order.getQuantity()<0) {
			throw new ParameterException("订单数量格式错误");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		if (productionService.addOrder(order,userVO)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}

	public void deleteOrder(Integer id, String deleteReason) {
		if (id == null || StrKit.isBlank(deleteReason)) {
			throw new ParameterException("参数不能为空");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		if (productionService.deleteOrder(id, deleteReason,userVO)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}

	}

	public void selectOrder(Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter) {
		ResultUtil result = ResultUtil
				.succeed(productionService.selectOrder(pageNo, pageSize, ascBy, descBy, filter));
		renderJson(result);
	}

	public void editOrder(@Para("") Orders order) {
		if (order == null) {
			throw new ParameterException("参数不能为空");
		}
		boolean isOrderInformationExist = StringUtils.isAnyBlank(order.getZhidan(), order.getSoftModel(),
				order.getVersion());
		if (isOrderInformationExist || order.getQuantity() == null) {
			throw new ParameterException("订单号、机型、版本和数量不能为空");
		}
		if (order.getQuantity()<0) {
			throw new ParameterException("订单数量格式错误");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		if (productionService.editOrder(order,userVO)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}

	}

	public synchronized void importOrder() {
		UploadFile file = getFile();
		if (file == null) {
			throw new ParameterException("请添加文件");
		}
		String result = productionService.importOrder(file.getFile());
		if ("导入成功".equals(result)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed(result));
		}
	}

	public synchronized void importOrderTable(Integer type, Integer id) {
		if (type == null || id == null) {
			throw new ParameterException("参数不能为空");
		}

		List<UploadFile> uploadFiles = getFiles();
		if (uploadFiles == null || uploadFiles.isEmpty()) {
			throw new ParameterException("请添加文件");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		if (productionService.importOrderTable(uploadFiles, type, id,userVO)) {
			renderJson(ResultUtil.succeed());
		}

	}

	public void selectOrderTable(Integer type, Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (type != null) {
			switch (type) {
			case 0:
			case 1:
			case 2:
				break;
			default:
				throw new OperationException("无法识别的类型");
			}
		}
		renderJson(ResultUtil.succeed(productionService.selectOrderTable(type, id)));

	}

	public synchronized void downloadOrderTable(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		renderFile(productionService.downloadOrderTable(id));

	}

	public void addCapacity(String softModel, String customerModel, Integer process, Integer processGroup,
			Integer processPeopleQuantity, Integer capacity, String remark) {
		if (StrKit.isBlank(softModel) || process == null || processGroup == null || processPeopleQuantity == null
				|| capacity == null) {
			throw new ParameterException("参数不能为空");
		}
		if (processPeopleQuantity < 0 || capacity < 0) {
			throw new ParameterException("产能和人数不合理");
		}
		if (productionService.addCapacity(softModel, customerModel, process, processGroup, processPeopleQuantity,
				capacity, remark)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}

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

	public void selectCapacity(Integer pageNo, Integer pageSize, String ascBy, String descBy, String softModel,
			String customerModel, Integer process) {
		renderJson(ResultUtil.succeed(
				productionService.selectCapacity(pageNo, pageSize, ascBy, descBy, softModel, customerModel, process)));
	}

	public void editCapacity(Integer id, String softModel, String customerModel, Integer process, Integer processGroup,
			Integer processPeopleQuantity, Integer capacity, String remark, Integer position) {
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


	public void getPlanGannt(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		renderJson(ResultUtil.succeed(productionService.getPlanGannt(id)));

	}

	// 查询未排产
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

	// 添加排产计划
	public void addPlan(Integer order, String remark, Integer schedulingQuantity, Integer line, Integer processGroup,
			Integer capacity) {
		if (order == null || schedulingQuantity == null || line == null || capacity == null) {
			throw new ParameterException("参数不能为空");
		}
		if (schedulingQuantity < 0) {
			throw new ParameterException("排产数量不合理");
		}
		if (productionService.addPlan(order, remark, schedulingQuantity, line, processGroup, capacity)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}

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
		if (productionService.editPlanStatus(id, type, userVO)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}

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

	public void selectPlan(Integer pageNo, Integer pageSize, Integer schedulingPlanStatus, String zhidan,
			String customerName, String orderDateFrom, String orderDateTo, String planStartTimeFrom,
			String planStartTimeTo, String planCompleteTimeFrom, String planCompleteTimeTo, String startTimeFrom,
			String startTimeTo, String completeTimeFrom, String completeTimeTo, Integer processGroup, Integer line,
			String productionPlanningNumber, String softModel, String productNo) {
		StringBuilder filter = new StringBuilder();
		filter.append(concatEqualSqlFilter("scheduling_plan_status", schedulingPlanStatus));
		filter.append(concatEqualSqlFilter("scheduling_plan.process_group", processGroup));
		filter.append(concatEqualSqlFilter("line", line));
		filter.append(concatSqlFilter("zhidan", zhidan, true, false));
		filter.append(concatSqlFilter("customer_name", customerName, true, false));
		filter.append(concatSqlFilter("production_planning_number", productionPlanningNumber, true, false));
		filter.append(concatSqlFilter("soft_model", softModel, true, false));
		filter.append(concatSqlFilter("product_no", productNo, true, false));
		filter.append(concatSqlFilter("create_time", orderDateFrom, false, true));
		filter.append(concatSqlFilter("create_time", orderDateTo, false, false));
		filter.append(concatSqlFilter("plan_start_time", planStartTimeFrom, false, true));
		filter.append(concatSqlFilter("plan_start_time", planStartTimeTo, false, false));
		filter.append(concatSqlFilter("plan_complete_time", planCompleteTimeFrom, false, true));
		filter.append(concatSqlFilter("plan_complete_time", planCompleteTimeTo, false, false));
		filter.append(concatSqlFilter("start_time", startTimeFrom, false, true));
		filter.append(concatSqlFilter("start_time", startTimeTo, false, false));
		filter.append(concatSqlFilter("complete_time", completeTimeFrom, false, true));
		filter.append(concatSqlFilter("complete_time", completeTimeTo, false, false));

		renderJson(ResultUtil.succeed(productionService.selectPlan(filter.toString(), pageNo, pageSize)));

	}

	private StringBuilder concatSqlFilter(String key, String value, Boolean isLike, Boolean isGreater) {
		StringBuilder filter = new StringBuilder();
		if (!StrKit.isBlank(value)) {
			if (isLike) {
				filter.append(" AND " + key + " like '%" + value + "%'");
			} else {
				if (isGreater) {
					filter.append(" AND " + key + " > '" + value + "'");
				} else {
					filter.append(" AND " + key + " < '" + value + "'");
				}
			}
		}
		return filter;

	}

	private StringBuilder concatEqualSqlFilter(String key, Integer value) {
		StringBuilder filter = new StringBuilder();
		if (value != null) {
			filter.append(" AND " + key + " = " + value);
		}

		return filter;

	}

	public void editPlan(Integer id, Boolean isUrgent, String remark, Integer schedulingQuantity, Integer line,
			String planStartTime, String planCompleteTime, String lineChangeTime, Integer capacity, Boolean isCompleted,
			Integer producedQuantity, String remainingReason, String productionPlanningNumber) {
		if (StringUtils.isAnyBlank(planStartTime, planCompleteTime, lineChangeTime)
				|| id == null && schedulingQuantity == null || capacity == null || line == null) {
			throw new ParameterException("参数不能为空");
		}
		if (schedulingQuantity < 0 || capacity < 0) {
			throw new ParameterException("排产数量或产能不合理");
		}
		if (producedQuantity != null && producedQuantity < 0) {
			throw new ParameterException("已生产数量不合理");
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
		if (productionService.editPlan(id, isUrgent, remark, schedulingQuantity, line, start, end, lineChangeTime,
				capacity, isCompleted, producedQuantity, remainingReason, productionPlanningNumber)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}

	public void checkCompleteTime(Integer schedulingQuantity, String planStartTime, String planCompleteTime,
			String lineChangeTime, Integer capacity) {
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

	public void dailyReport() {
		Calendar calendar = Calendar.getInstance();
		SimpleDateFormat testDateFormat = new SimpleDateFormat("yyyy-MM-dd");
		SimpleDateFormat printDateFormat = new SimpleDateFormat("yyyy.MM.dd");
		// 设置起始和结束时间
		/*
		 * String startTestTime = testDateFormat.format(calendar.getTime()) +
		 * " 00:00:00"; String endTestTime = testDateFormat.format(calendar.getTime()) +
		 * " 23:59:59"; String startPrintTime =
		 * printDateFormat.format(calendar.getTime()) + " 00:00:00:000"; String
		 * endPrintTime = printDateFormat.format(calendar.getTime()) + " 23:59:59:999";
		 */

		String startTestTime = "2017-01-05 00:00:00";
		String endTestTime = "2017-01-05 23:59:59";
		String startPrintTime = "2017.01.05 00:00:00:000";
		String endPrintTime = "2017.01.05 23:59:59:999";
		Object[] timeParas = { startTestTime, endTestTime, startTestTime, endTestTime, startTestTime, endTestTime,
				startTestTime, endTestTime, startTestTime, endTestTime, startPrintTime, endPrintTime, startPrintTime,
				endPrintTime };
		long t1 = System.currentTimeMillis();
		List<Record> records = Db.find(SQL.SELECT_ZHIDAN_VERSION_SOFTMODEL_BY_TESTTIME, timeParas);
		System.out.println("t1" + (System.currentTimeMillis() - t1));
		long t2 = System.currentTimeMillis();
		List<DailyOrderProduction> orderProductions = new ArrayList<>();
		if (records != null && !records.isEmpty()) {
			int dailyFunctionProduct = 0, dailySmtProduct = 0, dailyAgedProduct = 0, dailyCouplingProduct = 0,
					dailyCartonProduct = 0, dailyChPrintProduct = 0, dailyJsPrintProduct = 0;
			for (Record record : records) {
				String zhidan = record.get("ZhiDan");
				String version = record.get("Version");
				String softModel = record.get("SoftModel");
				if (StrKit.isBlank(zhidan)) {
					zhidan = "";
				}
				if (StrKit.isBlank(version)) {
					version = "";
				}
				if (StrKit.isBlank(softModel)) {
					softModel = "";
				}
				Object[] paras = { zhidan, version, softModel, startTestTime, endTestTime, zhidan, version, softModel,
						startTestTime, endTestTime, zhidan, version, softModel, startTestTime, endTestTime, zhidan,
						version, softModel, startTestTime, endTestTime, zhidan, version, softModel, startTestTime,
						endTestTime, zhidan, version, softModel, startPrintTime, endPrintTime, zhidan, version,
						softModel, startPrintTime, endPrintTime };
				Record productionRecord = Db.findFirst(SQL.SELECT_PRODUCTION_BY_ZHIDAN_VERSION_SOFTMODEL_TESTTIME,
						paras);
				DailyOrderProduction orderproduction = new DailyOrderProduction(zhidan, version, softModel);
				orderproduction.setFunctionProduct(productionRecord.getInt("FunctionProduct"));
				orderproduction.setSmtProduct(productionRecord.getInt("SMTProduct"));
				orderproduction.setAgedProduct(productionRecord.getInt("AgedProduct"));
				orderproduction.setCouplingProduct(productionRecord.getInt("CouplingProduct"));
				orderproduction.setCartonProduct(productionRecord.getInt("CartonProduct"));
				orderproduction.setChPrintProduct(productionRecord.getInt("CHPrintProduct"));
				orderproduction.setJsPrintProduct(productionRecord.getInt("JSPrintProduct"));
				orderProductions.add(orderproduction);

				dailyFunctionProduct += orderproduction.getFunctionProduct();
				dailySmtProduct += orderproduction.getSmtProduct();
				dailyAgedProduct += orderproduction.getAgedProduct();
				dailyCouplingProduct += orderproduction.getCouplingProduct();
				dailyCartonProduct += orderproduction.getCartonProduct();
				dailyChPrintProduct += orderproduction.getChPrintProduct();
				dailyJsPrintProduct += orderproduction.getJsPrintProduct();

			}
			Production production = new Production();
			production.setFunctionProduct(dailyFunctionProduct);
			production.setSmtProduct(dailySmtProduct);
			production.setAgedProduct(dailyAgedProduct);
			production.setCouplingProduct(dailyCouplingProduct);
			production.setCartonProduct(dailyCartonProduct);
			production.setChPrintProduct(dailyChPrintProduct);
			production.setJsPrintProduct(dailyJsPrintProduct);

			List<DailyCompletion> dailyCompletions = DailyCompletion.dao.find(SQL.SELECT_RENCENT_SIXDAYS_PRODUCTION);
			DailyCompletion dailyCompletion = new DailyCompletion();

			Integer quantity = production.getFunctionProduct() + production.getSmtProduct()
					+ production.getAgedProduct() + production.getCouplingProduct() + production.getCartonProduct()
					+ production.getChPrintProduct() + production.getJsPrintProduct();
			dailyCompletion.setTime(new Date());
			dailyCompletion.setQuantity(quantity);
			dailyCompletions.add(dailyCompletion);
			System.out.println();
			DailyReport dailyReport = new DailyReport(orderProductions, production, dailyCompletions);
			System.out.println("t2" + (System.currentTimeMillis() - t2));
			renderJson(ResultUtil.succeed(dailyReport));
		} else {
			renderJson(ResultUtil.failed("当天没有进行中的工单"));
		}

	}
	public static boolean validateIP(String ipAddress) {  
        String regex = "([1-9]|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])(\\.(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])){3}";   
        Pattern pattern = Pattern.compile(regex);  
        Matcher matcher = pattern.matcher(ipAddress);  
        return matcher.matches();  
    }  
}
