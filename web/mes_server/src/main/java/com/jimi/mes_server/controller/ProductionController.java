package com.jimi.mes_server.controller;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;

import org.apache.commons.lang3.StringUtils;

import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jfinal.core.paragetter.Para;
import com.jfinal.kit.StrKit;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Record;
import com.jfinal.upload.UploadFile;
import com.jimi.mes_server.entity.DailyOrderProduction;
import com.jimi.mes_server.entity.SQL;
import com.jimi.mes_server.exception.OperationException;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.model.Orders;
import com.jimi.mes_server.service.ProductionService;
import com.jimi.mes_server.util.ResultUtil;

public class ProductionController extends Controller {

	private static ProductionService productionService = Enhancer.enhance(ProductionService.class);

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

	public void addLine(String lineNo, String lineName, String lineRemark, Integer lineDirector, Integer processGroup) {
		if (StringUtils.isAnyBlank(lineNo, lineName)) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.addLine(lineNo, lineName, lineRemark, lineDirector, processGroup)) {
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

	public void editLine(Integer id, String lineNo, String lineName, String lineRemark, Integer lineDirector,
			Integer processGroup) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.editLine(id, lineNo, lineName, lineRemark, lineDirector, processGroup)) {
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

	public void addOrder(@Para("") Orders order) {
		if (order == null) {
			throw new ParameterException("参数不能为空");
		}
		boolean isOrderInformationExist = StringUtils.isAnyBlank(order.getZhidan(), order.getSoftModel(),
				order.getVersion());
		if (isOrderInformationExist || order.getQuantity() == null) {
			throw new ParameterException("订单号、机型、版本和数量不能为空");
		}
		if (productionService.addOrder(order)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}

	public void deleteOrder(Integer id, String deleteReason) {
		if (id == null || StrKit.isBlank(deleteReason)) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.deleteOrder(id, deleteReason)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}

	}

	public void editOrder(@Para("") Orders order) {
		if (order == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.editOrder(order)) {
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
		if (productionService.importOrderTable(uploadFiles, type, id)) {
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
			Integer processPeopleQuantity, Integer capacity) {
		if (StrKit.isBlank(softModel) || process == null || processGroup == null || processPeopleQuantity == null
				|| capacity == null) {
			throw new ParameterException("参数不能为空");
		}
		if (processPeopleQuantity < 0 || capacity < 0) {
			throw new ParameterException("产能和人数不合理");
		}
		if (productionService.addCapacity(softModel, customerModel, process, processGroup, processPeopleQuantity,
				capacity)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}
public void dailyReport() {
	Calendar calendar = Calendar.getInstance();
	SimpleDateFormat testDateFormat = new SimpleDateFormat("yyyy-MM-dd");
	SimpleDateFormat printDateFormat = new SimpleDateFormat("yyyy.MM.dd");
	// 设置起始和结束时间
	String startTestTime = testDateFormat.format(calendar.getTime()) + " 00:00:00";
	String endTestTime = testDateFormat.format(calendar.getTime()) + " 23:59:59";
	String startPrintTime = printDateFormat.format(calendar.getTime()) + " 00:00:00:000";
	String endPrintTime = printDateFormat.format(calendar.getTime()) + " 23:59:59:999";
	Object[] timeParas = {startTestTime,endTestTime,
			startTestTime,endTestTime,
			startTestTime,endTestTime,
			startTestTime,endTestTime,
			startTestTime,endTestTime,
			startPrintTime,endPrintTime,
			startPrintTime,endPrintTime};
	List<Record> records = Db.find(SQL.SELECT_ZHIDAN_VERSION_SOFTMODEL_BY_TESTTIME, timeParas);
	List<DailyOrderProduction> productions = new ArrayList<>();
	if (records!=null&&!records.isEmpty()) {
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
			Object[] paras = {zhidan,version,softModel,startTestTime,endTestTime,
					zhidan,version,softModel,startTestTime,endTestTime,
					zhidan,version,softModel,startTestTime,endTestTime,
					zhidan,version,softModel,startTestTime,endTestTime,
					zhidan,version,softModel,startTestTime,endTestTime,
					zhidan,version,softModel,startPrintTime,endPrintTime,
					zhidan,version,softModel,startPrintTime,endPrintTime};
			Record productionRecord =  Db.findFirst(SQL.SELECT_PRODUCTION_BY_ZHIDAN_VERSION_SOFTMODEL_TESTTIME, paras);
			DailyOrderProduction production = new DailyOrderProduction(zhidan, version, softModel, productionRecord.getInt("FunctionProduct"), productionRecord.getInt("SMTProduct"), productionRecord.getInt("AgedProduct"), productionRecord.getInt("CouplingProduct"), productionRecord.getInt("CartonProduct"), productionRecord.getInt("CHPrintProduct"), productionRecord.getInt("JSPrintProduct"));
			productions.add(production);
		
		}
	}
	
	
}
}
