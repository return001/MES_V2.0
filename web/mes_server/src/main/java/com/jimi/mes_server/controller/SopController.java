package com.jimi.mes_server.controller;

import java.util.List;

import org.apache.commons.lang3.StringUtils;

import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jfinal.kit.StrKit;
import com.jfinal.upload.UploadFile;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.service.SopService;
import com.jimi.mes_server.util.ResultUtil;

public class SopController extends Controller {

	private static SopService sopService = Enhancer.enhance(SopService.class);


	public void addFactory(String factoryAlias, String abbreviation, String fullName) {
		if (StringUtils.isAnyBlank(factoryAlias, abbreviation, fullName)) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.addFactory(factoryAlias, abbreviation, fullName)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	public void deleteFactory(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.deleteFactory(id)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	public void selectFactory(Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter) {
		renderJson(ResultUtil.succeed(sopService.selectFactory(pageNo, pageSize, ascBy, descBy, filter)));
	}


	public void editFactory(Integer id, String factoryAlias, String abbreviation, String fullName) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.editFactory(id, factoryAlias, abbreviation, fullName)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	public void addWorkshop(Integer factoryId, String workshopNumber, String workshopName) {
		if (factoryId == null || StringUtils.isAnyBlank(workshopNumber, workshopName)) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.addWorkshop(factoryId, workshopNumber, workshopName)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	public void deleteWorkshop(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.deleteWorkshop(id)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	public void selectWorkshop(Integer pageNo, Integer pageSize, Integer factoryId, String workshopNumber, String workshopName) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		renderJson(ResultUtil.succeed(sopService.selectWorkshop(pageNo, pageSize, factoryId, workshopNumber, workshopName)));
	}


	public void editWorkshop(Integer id, Integer factoryId, String workshopNumber, String workshopName) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.editWorkshop(id, factoryId, workshopNumber, workshopName)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}
	
	public void addSite(String siteNumber,String siteName,Integer processOrder,Integer lineId,Integer playTimes,Integer switchInterval,String mac) {
		if (processOrder == null ||lineId==null|| switchInterval==null||StringUtils.isAnyBlank(siteNumber, siteName,mac)) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.addSite( siteNumber, siteName, processOrder, lineId, playTimes, switchInterval, mac)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}
	
	public void deleteSite(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.deleteSite(id)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}
	
	public void selectSite(Integer pageNo, Integer pageSize, String siteNumber,String siteName,Integer processOrder,Integer lineId) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		renderJson(ResultUtil.succeed(sopService.selectWorkshop(pageNo, pageSize,  siteNumber, siteName, processOrder, lineId)));
	}
	
	public void editSite(Integer id,String siteNumber,String siteName,Integer processOrder,Integer lineId,Integer playTimes,Integer switchInterval,String mac) {
		if (id==null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.editSite( id,siteNumber, siteName, processOrder, lineId, playTimes, switchInterval, mac)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}
	
	public void addCustomer(String customerNumber ,String customerName ,Integer factoryId ) {
		if (factoryId == null ||StringUtils.isAnyBlank(customerNumber, customerName)) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.addCustomer(  customerNumber , customerName , factoryId)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}
	
	public void deleteCustomer(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.deleteCustomer(id)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}
	
	public void selectCustomer(Integer pageNo, Integer pageSize, String customerNumber ,String customerName ,Integer factoryId) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		renderJson(ResultUtil.succeed(sopService.selectCustomer(pageNo, pageSize,   customerNumber , customerName , factoryId)));
	}
	
	
	public void editCustomer(Integer id,String customerNumber ,String customerName ,Integer factoryId ) {
		if (id==null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.editCustomer(  id, customerNumber , customerName , factoryId)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}
	
	public void addSeriesModel(String seriesModelName  ) {
		if (StrKit.isBlank(seriesModelName)) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.addSeriesModel( seriesModelName)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}
	
	public void deleteSeriesModel(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.deleteSeriesModel(id)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}
	
	
	public void selectSeriesModel(Integer pageNo, Integer pageSize, String seriesModelName ) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		renderJson(ResultUtil.succeed(sopService.selectSeriesModel(pageNo, pageSize,   seriesModelName)));
	}
	
	
	public void editSeriesModel(Integer id,String seriesModelName ) {
		if (id==null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.editSeriesModel(  id, seriesModelName)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}
	
	
	public void addProductModel(String productModelName ,Integer seriesModelId   ) {
		if (StrKit.isBlank(productModelName)||seriesModelId==null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.addProductModel( productModelName , seriesModelId)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}
	
	
	public void deleteProductModel(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.deleteProductModel(id)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}
	
	
	public void selectProductModel(Integer pageNo, Integer pageSize, String productModelName ,Integer seriesModelId ) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		renderJson(ResultUtil.succeed(sopService.selectProductModel(pageNo, pageSize,    productModelName , seriesModelId)));
	}
	
	public void editProductModel(Integer id,String productModelName ,Integer seriesModelId  ) {
		if (id==null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.editProductModel(  id,  productModelName , seriesModelId )) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}
	
	public void importFile() {
		List<UploadFile> uploadFiles = getFiles();
		if (uploadFiles == null || uploadFiles.isEmpty()) {
			throw new ParameterException("请添加文件");
		}
		for (UploadFile uploadFile : uploadFiles) {
			if (!uploadFile.getOriginalFileName().endsWith(".xls") && !uploadFile.getOriginalFileName().endsWith(".xlsx")) {
				throw new ParameterException("只能上传Excel文件");
			}
		}
		if (sopService.importFile(  uploadFiles )) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}
}
