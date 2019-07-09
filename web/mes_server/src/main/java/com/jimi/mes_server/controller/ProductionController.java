package com.jimi.mes_server.controller;

import org.apache.commons.lang3.StringUtils;

import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jfinal.core.paragetter.Para;
import com.jfinal.kit.StrKit;
import com.jfinal.upload.UploadFile;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.model.Orders;
import com.jimi.mes_server.service.ProductionService;
import com.jimi.mes_server.util.ResultUtil;

public class ProductionController extends Controller {
	
	private static ProductionService productionService = Enhancer.enhance(ProductionService.class);

	public void addProcessGroup(String groupNo,String groupName,String groupRemark) {
		if (StringUtils.isAnyBlank(groupNo,groupName)) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.addProcessGroup(groupNo,groupName,groupRemark)) {
			renderJson(ResultUtil.succeed());
		}else {
			renderJson(ResultUtil.failed());
		}
	}
	
	
	public void deleteProcessGroup(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.deleteProcessGroup(id)) {
			renderJson(ResultUtil.succeed());
		}else {
			renderJson(ResultUtil.failed());
		}
		
	}
	

	public void editProcessGroup(Integer id,String groupNo,String groupName,String groupRemark) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.editProcessGroup(id,groupNo,groupName,groupRemark)) {
			renderJson(ResultUtil.succeed());
		}else {
			renderJson(ResultUtil.failed());
		}
		
	}
	
	public void addLine(String lineNo,String lineName,String lineRemark,Integer lineDirector,Integer processGroup) {
		if (StringUtils.isAnyBlank(lineNo,lineName)) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.addLine(lineNo,lineName,lineRemark,lineDirector,processGroup)) {
			renderJson(ResultUtil.succeed());
		}else {
			renderJson(ResultUtil.failed());
		}
	}
	
	public void deleteLine(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.deleteLine(id)) {
			renderJson(ResultUtil.succeed());
		}else {
			renderJson(ResultUtil.failed());
		}
		
	}
	public void editLine(Integer id,String lineNo,String lineName,String lineRemark,Integer lineDirector,Integer processGroup) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.editLine(id,lineNo,lineName,lineRemark,lineDirector, processGroup)) {
			renderJson(ResultUtil.succeed());
		}else {
			renderJson(ResultUtil.failed());
		}
		
	}
	
	public void addProcess(String processNo,String processName,String processRemark,Integer processGroup) {
		if (StringUtils.isAnyBlank(processNo,processName)) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.addProcess(processNo,processName,processRemark,processGroup)) {
			renderJson(ResultUtil.succeed());
		}else {
			renderJson(ResultUtil.failed());
		}
	}
	public void deleteProcess(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.deleteProcess(id)) {
			renderJson(ResultUtil.succeed());
		}else {
			renderJson(ResultUtil.failed());
		}
		
	}
	public void editProcess(Integer id,String processNo,String processName,String processRemark,Integer processGroup) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.editProcess(id,processNo,processName,processRemark,processGroup)) {
			renderJson(ResultUtil.succeed());
		}else {
			renderJson(ResultUtil.failed());
		}
		
	}
	
	public void addOrder(@Para("") Orders order) {
		if (order==null) {
			throw new ParameterException("参数不能为空");
		}
		boolean isOrderInformationExist = StringUtils.isAnyBlank(order.getZhidan(),order.getSoftModel(),order.getVersion());
		if (isOrderInformationExist||order.getQuantity()==null) {
			throw new ParameterException("订单号、机型、版本和数量不能为空");
		}
		if (productionService.addOrder(order)) {
			renderJson(ResultUtil.succeed());
		}else {
			renderJson(ResultUtil.failed());
		}
	}
	
	public void deleteOrder(Integer id,String deleteReason) {
		if (id == null || StrKit.isBlank(deleteReason)) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.deleteOrder(id,deleteReason)) {
			renderJson(ResultUtil.succeed());
		}else {
			renderJson(ResultUtil.failed());
		}
		
	}
	public void editOrder(@Para("") Orders order) {
		if (order==null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.editOrder(order)) {
			renderJson(ResultUtil.succeed());
		}else {
			renderJson(ResultUtil.failed());
		}
		
	}

	public synchronized void importOrder(UploadFile file) {
		if (file == null) {
			throw new ParameterException("参数不能为空");
		}
		String result = productionService.importOrder(file.getFile());
		if ("导入成功".equals(result)) {
			renderJson(ResultUtil.succeed());
		}else {
			renderJson(ResultUtil.failed(result));
		}
	}
	
}
