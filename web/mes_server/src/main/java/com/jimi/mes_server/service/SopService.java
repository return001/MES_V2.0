package com.jimi.mes_server.service;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import javax.websocket.Session;

import org.apache.commons.io.FileUtils;
import org.apache.commons.lang3.StringUtils;

import com.alibaba.fastjson.JSONObject;
import com.aspose.cells.ImageOrPrintOptions;
import com.aspose.cells.ImageType;
import com.aspose.cells.SheetRender;
import com.aspose.cells.Workbook;
import com.aspose.cells.Worksheet;
import com.jfinal.kit.PropKit;
import com.jfinal.kit.StrKit;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Page;
import com.jfinal.plugin.activerecord.Record;
import com.jfinal.plugin.activerecord.SqlPara;
import com.jfinal.upload.UploadFile;
import com.jimi.mes_server.entity.Constant;
import com.jimi.mes_server.entity.FileHistoryDetail;
import com.jimi.mes_server.entity.PreviewInfo;
import com.jimi.mes_server.entity.SopFileState;
import com.jimi.mes_server.entity.SopSQL;
import com.jimi.mes_server.entity.sendMessageInfo;
import com.jimi.mes_server.entity.vo.LUserAccountVO;
import com.jimi.mes_server.entity.vo.PictureVO;
import com.jimi.mes_server.exception.OperationException;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.model.Line;
import com.jimi.mes_server.model.SopCustomer;
import com.jimi.mes_server.model.SopFaceInformation;
import com.jimi.mes_server.model.SopFactory;
import com.jimi.mes_server.model.SopFile;
import com.jimi.mes_server.model.SopFileHistory;
import com.jimi.mes_server.model.SopFilePicture;
import com.jimi.mes_server.model.SopLoginLog;
import com.jimi.mes_server.model.SopNotice;
import com.jimi.mes_server.model.SopNoticeHistory;
import com.jimi.mes_server.model.SopPictureHistory;
import com.jimi.mes_server.model.SopProductModel;
import com.jimi.mes_server.model.SopSeriesModel;
import com.jimi.mes_server.model.SopSite;
import com.jimi.mes_server.model.SopSiteDisplay;
import com.jimi.mes_server.model.SopWorkshop;
import com.jimi.mes_server.service.base.SelectService;
import com.jimi.mes_server.util.CommonUtil;
import com.jimi.mes_server.util.ExcelHelper;
import com.jimi.mes_server.util.ResultUtil;
import com.jimi.mes_server.websocket.container.SessionBox;
import com.jimi.mes_server.websocket.entity.Notice;
import com.jimi.mes_server.websocket.entity.Picture;
import com.jimi.mes_server.websocket.entity.RequestType;

import cc.darhao.pasta.Pasta;

public class SopService extends SelectService {

	public boolean addFactory(String factoryAlias, String abbreviation, String fullName) {
		SopFactory sopFactory = SopFactory.dao.findFirst(SopSQL.SELECT_FACTORY_ID_BY_COLUMN, factoryAlias, abbreviation, fullName);
		if (sopFactory != null) {
			throw new OperationException("数据项已存在,请重新输入");
		}
		sopFactory = new SopFactory();
		sopFactory.setAbbreviation(abbreviation).setFactoryAlias(factoryAlias).setFullName(fullName);
		return sopFactory.save();
	}


	public boolean deleteFactory(Integer id) {
		SopFactory sopFactory = SopFactory.dao.findById(id);
		if (sopFactory == null) {
			throw new OperationException("不存在当前工厂");
		}
		SopWorkshop sopWorkshop = SopWorkshop.dao.findFirst(SopSQL.SELECT_WORKSHOP_BY_FACTORY, id);
		if (sopWorkshop != null) {
			throw new OperationException("当前工厂被车间所引用，请先删除车间，再进行操作");
		}
		SopCustomer sopCustomer = SopCustomer.dao.findFirst(SopSQL.SELECT_CUSTOMER_BY_FACTORY, id);
		if (sopCustomer != null) {
			throw new OperationException("当前工厂被客户所引用，请先删除客户，再进行操作");
		}
		Line line = Line.dao.findFirst(SopSQL.SELECT_LINE_BY_FACTORY, id);
		if (line != null) {
			throw new OperationException("当前工厂被产线所引用，请先删除产线，再进行操作");
		}
		return sopFactory.delete();
	}


	public Page<Record> selectFactory(Integer pageNo, Integer pageSize, String factoryAlias, String abbreviation, String fullName) {
		SqlPara sqlPara = new SqlPara();
		StringBuilder sql = new StringBuilder(SopSQL.SELECT_FACTORY);
		sql.append(" WHERE 1 = 1 ");
		if (!StrKit.isBlank(factoryAlias)) {
			sql.append(" AND factory_alias like '%").append(factoryAlias).append("%'");
		}
		if (!StrKit.isBlank(abbreviation)) {
			sql.append(" AND abbreviation like '%").append(abbreviation).append("%'");
		}
		if (!StrKit.isBlank(fullName)) {
			sql.append(" AND full_name like '%").append(fullName).append("%'");
		}
		if (StringUtils.endsWith(sql, "1 = 1 ")) {
			sql.delete(sql.lastIndexOf("WHERE"), sql.length());
		}
		sqlPara.setSql(sql.toString());
		return Db.paginate(pageNo, pageSize, sqlPara);
	}


	public boolean editFactory(Integer id, String factoryAlias, String abbreviation, String fullName) {
		SopFactory sopFactory = SopFactory.dao.findById(id);
		if (sopFactory == null) {
			throw new OperationException("不存在当前工厂");
		}
		if (!StrKit.isBlank(factoryAlias)) {
			SopFactory temp = SopFactory.dao.findFirst(SopSQL.SELECT_FACTORY_ID_BY_COLUMN, factoryAlias, null, null);
			if (temp != null && !temp.getId().equals(id)) {
				throw new OperationException("工厂别名已存在,请重新输入");
			}
			sopFactory.setFactoryAlias(factoryAlias);
		}
		if (!StrKit.isBlank(abbreviation)) {
			SopFactory temp = SopFactory.dao.findFirst(SopSQL.SELECT_FACTORY_ID_BY_COLUMN, null, abbreviation, null);
			if (temp != null && !temp.getId().equals(id)) {
				throw new OperationException("工厂简称已存在,请重新输入");
			}
			sopFactory.setAbbreviation(abbreviation);
		}
		if (!StrKit.isBlank(fullName)) {
			SopFactory temp = SopFactory.dao.findFirst(SopSQL.SELECT_FACTORY_ID_BY_COLUMN, null, null, fullName);
			if (temp != null && !temp.getId().equals(id)) {
				throw new OperationException("工厂全称已存在,请重新输入");
			}
			sopFactory.setFullName(fullName);
		}
		return sopFactory.update();
	}


	public boolean addWorkshop(Integer factoryId, String workshopNumber, String workshopName) {
		SopWorkshop sopWorkshop = SopWorkshop.dao.findFirst(SopSQL.SELECT_WORKSHOP_BY_COLUMN, workshopName, workshopNumber);
		if (sopWorkshop != null) {
			throw new OperationException("数据项已存在,请重新输入");
		}
		if (SopFactory.dao.findById(factoryId) == null) {
			throw new OperationException("不存在当前工厂");
		}
		sopWorkshop = new SopWorkshop();
		sopWorkshop.setFactoryId(factoryId).setWorkshopName(workshopName).setWorkshopNumber(workshopNumber);
		return sopWorkshop.save();
	}


	public boolean deleteWorkshop(Integer id) {
		SopWorkshop sopWorkshop = SopWorkshop.dao.findById(id);
		if (sopWorkshop == null) {
			throw new OperationException("不存在当前车间");
		}
		Line line = Line.dao.findFirst(SopSQL.SELECT_LINE_BY_WORKSHOP, id);
		if (line != null) {
			throw new OperationException("当前车间被产线所引用，请先删除产线，再进行操作");
		}
		return sopWorkshop.delete();
	}


	public Page<Record> selectWorkshop(Integer pageNo, Integer pageSize, Integer factoryId, String workshopNumber, String workshopName) {
		SqlPara sqlPara = new SqlPara();
		StringBuilder sql = new StringBuilder(SopSQL.SELECT_WORKSHOP_AND_FACTORY_ABBREVIATION);
		if (factoryId != null) {
			sql.append(" AND ").append("w.factory_id = ").append(factoryId);
		}
		if (!StringUtils.isBlank(workshopNumber)) {
			sql.append(" AND ").append("w.workshop_number like '%").append(workshopNumber).append("%'");
		}
		if (!StringUtils.isBlank(workshopName)) {
			sql.append(" AND ").append("w.workshop_name like '%").append(workshopName).append("%'");
		}
		sqlPara.setSql(sql.toString());
		return Db.paginate(pageNo, pageSize, sqlPara);
	}


	public boolean editWorkshop(Integer id, Integer factoryId, String workshopNumber, String workshopName) {
		SopWorkshop sopWorkshop = SopWorkshop.dao.findById(id);
		if (sopWorkshop == null) {
			throw new OperationException("不存在当前车间");
		}
		if (factoryId != null && !factoryId.equals(sopWorkshop.getFactoryId())) {
			if (SopFactory.dao.findById(factoryId) == null) {
				throw new OperationException("不存在当前工厂");
			}
			sopWorkshop.setFactoryId(factoryId);
		}
		if (!StringUtils.isBlank(workshopNumber)) {
			SopWorkshop temp = SopWorkshop.dao.findFirst(SopSQL.SELECT_WORKSHOP_BY_COLUMN, null, workshopNumber);
			if (temp != null && !temp.getId().equals(id)) {
				throw new OperationException("车间编号已存在,请重新输入");
			}
			sopWorkshop.setWorkshopNumber(workshopNumber);
		}
		if (!StringUtils.isBlank(workshopName)) {
			SopWorkshop temp = SopWorkshop.dao.findFirst(SopSQL.SELECT_WORKSHOP_BY_COLUMN, workshopName, null);
			if (temp != null && !temp.getId().equals(id)) {
				throw new OperationException("车间名称已存在,请重新输入");
			}
			sopWorkshop.setWorkshopName(workshopName);
		}
		return sopWorkshop.update();
	}


	public boolean addSite(String siteNumber, String siteName, Integer processOrder, Integer lineId, Integer playTimes, Integer switchInterval, String mac) {
		if (SopSite.dao.findFirst(SopSQL.SELECT_SITE_BY_SITENUMBER, siteNumber) != null) {
			throw new OperationException("站点编号已存在,请重新输入");
		}
		if (SopSite.dao.findFirst(SopSQL.SELECT_SITE_BY_MAC, mac) != null) {
			throw new OperationException("MAC地址已存在,请重新输入");
		}
		if (Line.dao.findById(lineId) == null) {
			throw new OperationException("当前产线不存在");
		}
		SopSite sopSite = new SopSite();
		if (playTimes != null) {
			sopSite.setPlayTimes(playTimes);
		}
		sopSite.setLineId(lineId).setMac(mac).setProcessOrder(processOrder).setSiteName(siteName).setSiteNumber(siteNumber).setSwitchInterval(switchInterval);
		return sopSite.save();
	}


	public boolean deleteSite(Integer id) {
		SopSite sopSite = SopSite.dao.findById(id);
		if (sopSite == null) {
			throw new OperationException("当前站点不存在");
		}
		return sopSite.delete();
	}


	public Page<Record> selectSite(Integer pageNo, Integer pageSize, String siteNumber, String siteName, Integer processOrder, Integer lineId) {
		SqlPara sqlPara = new SqlPara();
		StringBuilder sql = new StringBuilder(SopSQL.SELECT_SITE_JOIN_LINE);
		if (!StrKit.isBlank(siteName)) {
			sql.append(" and s.site_name like '%").append(siteName).append("%'");
		}
		if (!StrKit.isBlank(siteNumber)) {
			sql.append(" and s.site_number like '%").append(siteNumber).append("%'");
		}
		if (processOrder != null) {
			sql.append(" and s.process_order = ").append(processOrder);
		}
		if (lineId != null) {
			sql.append(" and s.line_id = ").append(lineId);
		}
		sqlPara.setSql(sql.toString());
		return Db.paginate(pageNo, pageSize, sqlPara);
	}


	public boolean editSite(Integer id, String siteNumber, String siteName, Integer processOrder, Integer lineId, Integer playTimes, Integer switchInterval, String mac) {
		SopSite sopSite = SopSite.dao.findById(id);
		if (sopSite == null) {
			throw new OperationException("当前站点不存在");
		}
		if (!StrKit.isBlank(siteNumber)) {
			SopSite temp = SopSite.dao.findFirst(SopSQL.SELECT_SITE_BY_SITENUMBER, siteNumber);
			if (temp != null && !temp.getId().equals(id)) {
				throw new OperationException("站点编号已存在,请重新输入");
			}
			sopSite.setSiteNumber(siteNumber);
		}
		if (!StrKit.isBlank(siteName)) {
			sopSite.setSiteName(siteName);
		}
		if (processOrder != null) {
			sopSite.setProcessOrder(processOrder);
		}
		if (lineId != null) {
			if (Line.dao.findById(lineId) == null) {
				throw new OperationException("当前产线不存在");
			}
			sopSite.setLineId(lineId);
		}
		if (playTimes != null) {
			sopSite.setPlayTimes(playTimes);
		}
		if (switchInterval != null) {
			sopSite.setSwitchInterval(switchInterval);
		}
		if (!StrKit.isBlank(mac)) {
			if (!CommonUtil.isMac(mac)) {
				throw new ParameterException("MAC地址无效");
			}
			SopSite temp = SopSite.dao.findFirst(SopSQL.SELECT_SITE_BY_MAC, mac);
			if (temp != null && !temp.getId().equals(id)) {
				throw new OperationException("MAC地址已存在,请重新输入");
			}
			sopSite.setMac(mac);
		}
		return sopSite.update();
	}


	public boolean addCustomer(String customerNumber, String customerName, Integer factoryId) {
		SopCustomer sopCustomer = SopCustomer.dao.findFirst(SopSQL.SELECT_CUSTOMER_ID_BY_COLUMN, customerName, factoryId, customerNumber, factoryId);
		if (sopCustomer != null) {
			throw new OperationException("同一工厂下已存在当前数据项，请重新输入");
		}
		if (SopFactory.dao.findById(factoryId) == null) {
			throw new OperationException("当前工厂不存在");
		}
		sopCustomer = new SopCustomer();
		sopCustomer.setFactoryId(factoryId).setCustomerName(customerName).setCustomerNumber(customerNumber);
		return sopCustomer.save();
	}


	public boolean deleteCustomer(Integer id) {
		SopCustomer sopCustomer = SopCustomer.dao.findById(id);
		if (sopCustomer == null) {
			throw new OperationException("当前客户不存在");
		}
		return sopCustomer.delete();
	}


	public Page<Record> selectCustomer(Integer pageNo, Integer pageSize, String customerNumber, String customerName, Integer factoryId) {
		SqlPara sqlPara = new SqlPara();
		StringBuilder sql = new StringBuilder(SopSQL.SELECT_CUSTOMER_JOIN_FACTORY);
		if (!StrKit.isBlank(customerName)) {
			sql.append(" AND c.customer_name LIKE '%").append(customerName).append("%'");
		}
		if (!StrKit.isBlank(customerNumber)) {
			sql.append(" AND c.customer_number LIKE '%").append(customerNumber).append("%'");
		}
		if (factoryId != null) {
			sql.append(" AND c.factory_id = ").append(factoryId);
		}
		sqlPara.setSql(sql.toString());
		return Db.paginate(pageNo, pageSize, sqlPara);
	}


	public boolean editCustomer(Integer id, String customerNumber, String customerName, Integer factoryId) {
		SopCustomer sopCustomer = SopCustomer.dao.findById(id);
		if (sopCustomer == null) {
			throw new OperationException("当前客户不存在");
		}
		if (factoryId != null && !StringUtils.isAnyBlank(customerNumber, customerName)) {
			SopCustomer temp;
			if (!factoryId.equals(sopCustomer.getFactoryId())) {
				temp = SopCustomer.dao.findFirst(SopSQL.SELECT_CUSTOMER_ID_BY_COLUMN, customerName, factoryId, customerNumber, factoryId);
			} else {
				temp = SopCustomer.dao.findFirst(SopSQL.SELECT_CUSTOMER_ID_BY_COLUMN, customerName, sopCustomer.getFactoryId(), customerNumber, sopCustomer.getFactoryId());
			}
			if (temp != null && !id.equals(temp.getId())) {
				throw new OperationException("同一工厂下已存在当前数据项，请重新输入");
			}
			sopCustomer.setFactoryId(factoryId).setCustomerName(customerName).setCustomerNumber(customerNumber);
		}
		return sopCustomer.update();
	}


	public boolean addSeriesModel(String seriesModelName) {
		SopSeriesModel sopSeriesModel = SopSeriesModel.dao.findFirst(SopSQL.SELECT_SERIESMODEL_BY_SERIESMODELNAME, seriesModelName);
		if (sopSeriesModel != null) {
			throw new OperationException("当前数据项已存在，请重新输入");
		}
		sopSeriesModel = new SopSeriesModel();
		sopSeriesModel.setSeriesModelName(seriesModelName);
		return sopSeriesModel.save();
	}


	public boolean deleteSeriesModel(Integer id) {
		SopSeriesModel sopSeriesModel = SopSeriesModel.dao.findById(id);
		if (sopSeriesModel == null) {
			throw new OperationException("当前系列机型不存在");
		}
		SopProductModel sopProductModel = SopProductModel.dao.findFirst(SopSQL.SELECT_PRODUCTMODEL_BY_SERIESMODEL, id);
		if (sopProductModel != null) {
			throw new OperationException("当前系列机型已被产品机型所引用，请先删除产品机型，再进行操作");
		}
		return sopSeriesModel.delete();
	}


	public Page<Record> selectSeriesModel(Integer pageNo, Integer pageSize, String seriesModelName) {
		SqlPara sqlPara = new SqlPara();
		StringBuilder sql = new StringBuilder(SopSQL.SELECT_SERIESMODEL);
		if (!StrKit.isBlank(seriesModelName)) {
			sql.append(" WHERE series_model_name LIKE '%").append(seriesModelName).append("%'");
		}
		sqlPara.setSql(sql.toString());
		return Db.paginate(pageNo, pageSize, sqlPara);
	}


	public boolean editSeriesModel(Integer id, String seriesModelName) {
		SopSeriesModel sopSeriesModel = SopSeriesModel.dao.findById(id);
		if (sopSeriesModel == null) {
			throw new OperationException("当前系列机型不存在");
		}
		SopSeriesModel temp = SopSeriesModel.dao.findFirst(SopSQL.SELECT_SERIESMODEL_BY_SERIESMODELNAME, seriesModelName);
		if (temp != null && !id.equals(temp.getId())) {
			throw new OperationException("当前数据项已存在，请重新输入");
		}
		if (!StrKit.isBlank(seriesModelName)) {
			if (!seriesModelName.equals(sopSeriesModel.getSeriesModelName())) {
				sopSeriesModel.setSeriesModelName(seriesModelName);
				return sopSeriesModel.update();
			}
		}
		return true;
	}


	public boolean addProductModel(String productModelName, Integer seriesModelId) {
		if (SopSeriesModel.dao.findById(seriesModelId) == null) {
			throw new OperationException("当前系列机型不存在");
		}
		SopProductModel sopProductModel = SopProductModel.dao.findFirst(SopSQL.SELECT_PRODUCTMODEL_BY_NAME_ID, productModelName, seriesModelId);
		if (sopProductModel != null) {
			throw new OperationException("当前系列机型此数据项已存在，请重新输入");
		}
		sopProductModel = new SopProductModel();
		sopProductModel.setProductModelName(productModelName).setSeriesModelId(seriesModelId);
		return sopProductModel.save();
	}


	public boolean deleteProductModel(Integer id) {
		SopProductModel sopProductModel = SopProductModel.dao.findById(id);
		if (sopProductModel == null) {
			throw new OperationException("当前产品机型不存在");
		}
		return sopProductModel.delete();
	}


	public Page<Record> selectProductModel(Integer pageNo, Integer pageSize, String productModelName, Integer seriesModelId) {
		SqlPara sqlPara = new SqlPara();
		StringBuilder sql = new StringBuilder(SopSQL.SELECT_PRODUCTMODEL_JOIN_SERIESMODEL);
		if (!StrKit.isBlank(productModelName)) {
			sql.append(" AND p.product_model_name like '%").append(productModelName).append("%'");
		}
		if (seriesModelId != null) {
			sql.append(" AND p.series_model_id = ").append(seriesModelId);
		}
		sqlPara.setSql(sql.toString());
		return Db.paginate(pageNo, pageSize, sqlPara);
	}


	public boolean editProductModel(Integer id, String productModelName, Integer seriesModelId) {
		SopProductModel sopProductModel = SopProductModel.dao.findById(id);
		if (sopProductModel == null) {
			throw new OperationException("当前产品机型不存在");
		}
		if (SopSeriesModel.dao.findById(seriesModelId) == null) {
			throw new OperationException("当前系列机型不存在");
		}
		sopProductModel.setSeriesModelId(seriesModelId);
		if (!StrKit.isBlank(productModelName)) {
			SopProductModel temp = SopProductModel.dao.findFirst(SopSQL.SELECT_PRODUCTMODEL_BY_NAME_ID, productModelName, seriesModelId);
			if (temp != null && !id.equals(temp.getId())) {
				throw new OperationException("当前系列机型此数据项已存在，请重新输入");
			}
			sopProductModel.setProductModelName(productModelName);
		}
		return sopProductModel.update();
	}


	public boolean importFiles(List<UploadFile> uploadFiles) {
		PropKit.use("properties.ini");
		String filePath = null;
		if (CommonUtil.isWindows()) {
			filePath = PropKit.get("windowsFlagPath");
		} else {
			filePath = PropKit.get("linuxFlagPath");
		}
		String path = filePath + Constant.SOP_FILE_PATH;
		File dir = new File(path);
		if (!dir.exists()) {
			dir.mkdirs();
		}
		for (UploadFile uploadFile : uploadFiles) {
			saveFile(uploadFile, path);
		}
		return true;
	}


	private void saveFile(UploadFile uploadFile, String path) {
		File previousFile = new File(path + uploadFile.getOriginalFileName());
		if (previousFile.exists()) {
			throw new OperationException("已存在文件名为：" + uploadFile.getOriginalFileName() + " 的文件");
		}
		File destFile = new File(path + uploadFile.getOriginalFileName());
		try {
			FileUtils.moveFile(uploadFile.getFile(), destFile);
		} catch (Exception e) {
			throw new OperationException("存储文件失败");
		}
		saveFileInfo(destFile);
	}


	private void saveFileInfo(File file) {
		ExcelHelper excelHelper;
		try {
			excelHelper = ExcelHelper.from(file);
		} catch (Exception e) {
			throw new OperationException("读取文件：" + file.getName() + " 出错，请确认文件内容");
		}
		String fileName = excelHelper.getString(10, 2);
		String fileNumber = excelHelper.getString(11, 2);
		String version = excelHelper.getString(12, 2);
		excelHelper.switchSheet(2);
		String customer = excelHelper.getString(2, 2);
		String productModel = excelHelper.getString(2, 6);
		String seriesModel = excelHelper.getString(2, 11);
		SopFile sopFile = new SopFile();
		sopFile.setCustomer(customer).setFileName(fileName).setFileNumber(fileNumber).setPath(file.getAbsolutePath());
		sopFile.setProductModel(productModel).setSeriesModel(seriesModel).setVersion(version).setState(SopFileState.WAITREVIEW_STATE.getName());
		sopFile.save();

	}


	public boolean deleteFile(Integer id) {
		SopFile sopFile = SopFile.dao.findById(id);
		if (sopFile == null) {
			throw new OperationException("当前文件信息不存在");
		}
		List<SopSiteDisplay> sopSiteDisplays = SopSiteDisplay.dao.find(SopSQL.SELECT_SOPSITEDISPLAY);
		if (sopSiteDisplays != null && !sopSiteDisplays.isEmpty()) {
			for (SopSiteDisplay sopSiteDisplay : sopSiteDisplays) {
				if (StrKit.isBlank(sopSiteDisplay.getFiles())) {
					continue;
				}
				String[] fileIds = sopSiteDisplay.getFiles().split(",");
				for (String fileId : fileIds) {
					if (!StrKit.isBlank(fileId) && id.equals(Integer.parseInt(fileId))) {
						throw new OperationException("当前文件正在被播放中");
					}
				}
			}
		}
		File file = new File(sopFile.getPath());
		if (file.exists()) {
			file.delete();
		}
		List<SopFilePicture> sopFilePictures = SopFilePicture.dao.find(SopSQL.SELECT_SOPFILEPICTURE_BY_FILEID, id);
		if (sopFilePictures != null && !sopFilePictures.isEmpty()) {
			for (SopFilePicture sopFilePicture : sopFilePictures) {
				File picture = new File(sopFilePicture.getPicturePath());
				if (picture.exists()) {
					picture.delete();
				}
			}
			Db.delete(SopSQL.DELETE_SOPFILEPICTURE_BY_FILEID, id);
		}
		sopFile.setState(SopFileState.INVALID_STATE.getName());
		return sopFile.update();
	}


	public Page<Record> selectFiles(Integer pageNo, Integer pageSize, String fileNumber, String fileName, String version, String customer, String seriesModel, String productModel, String reviewer, String state, String startTime, String endTime) {
		SqlPara sqlPara = new SqlPara();
		StringBuilder sql = new StringBuilder(SopSQL.SELECT_SOPFILE);
		sql.append(" WHERE 1 = 1 ");
		if (!StrKit.isBlank(fileNumber)) {
			sql.append(" AND file_number like '%").append(fileNumber).append("%'");
		}
		if (!StrKit.isBlank(fileName)) {
			sql.append(" AND file_name like '%").append(fileName).append("%'");
		}
		if (!StrKit.isBlank(version)) {
			sql.append(" AND version like '%").append(version).append("%'");
		}
		if (!StrKit.isBlank(customer)) {
			sql.append(" AND customer like '%").append(customer).append("%'");
		}
		if (!StrKit.isBlank(seriesModel)) {
			sql.append(" AND series_model like '%").append(seriesModel).append("%'");
		}
		if (!StrKit.isBlank(productModel)) {
			sql.append(" AND product_model like '%").append(productModel).append("%'");
		}
		if (!StrKit.isBlank(reviewer)) {
			sql.append(" AND reviewer like '%").append(reviewer).append("%'");
		}
		if (!StrKit.isBlank(state)) {
			sql.append(" AND state = ").append(state);
		}
		if (!StrKit.isBlank(startTime)) {
			sql.append(" AND review_time >= '").append(startTime).append("'");
		}
		if (!StrKit.isBlank(endTime)) {
			sql.append(" AND review_time <= '").append(endTime).append("'");
		}
		if (StringUtils.endsWith(sql, "1 ")) {
			sql.delete(sql.lastIndexOf("WHERE"), sql.length());
		}
		sqlPara.setSql(sql.toString());
		Page<Record> records = Db.paginate(pageNo, pageSize, sqlPara);
		for (Record record : records.getList()) {
			Integer fileHistoryNumber = Db.queryInt(SopSQL.SELECT_FILEHISTORY_NUMBER, record.getInt("id"));
			record.set("fileHistoryNumber", fileHistoryNumber);
		}
		return records;
	}


	public Page<Record> selectFilePictures(Integer pageNo, Integer pageSize, Integer fileId) {
		SqlPara sqlPara = new SqlPara();
		sqlPara.setSql(SopSQL.SELECT_SOPPICTURE_BY_FILEID);
		sqlPara.addPara(fileId);
		return Db.paginate(pageNo, pageSize, sqlPara);
	}


	public boolean editFileState(Integer id, String state, LUserAccountVO userVO) {
		SopFile sopFile = SopFile.dao.findById(id);
		if (sopFile == null) {
			throw new OperationException("当前文件信息不存在");
		}
		if (state.equals(SopFileState.INVALID_STATE.getName()) && !sopFile.getState().equals(SopFileState.PLAYING_STATE.getName())) {
			throw new OperationException("播放状态不能修改为作废状态");
		}
		if (SopFileState.REVIEWED_STATE.getName().equals(state) && sopFile.getState().equals(SopFileState.WAITREVIEW_STATE.getName())) {
			saveFilePicture(sopFile);
			sopFile.setReviewer(userVO.getName());
			sopFile.setReviewTime(new Date());
		}
		sopFile.setState(state);
		return sopFile.update();
	}


	private void saveFilePicture(SopFile sopFile) {
		PropKit.use("properties.ini");
		String filePath = null;
		if (CommonUtil.isWindows()) {
			filePath = PropKit.get("windowsFlagPath");
		} else {
			filePath = PropKit.get("linuxFlagPath");
		}
		String path = filePath + Constant.SOP_PICTURE_PATH + sopFile.getFileName().replace(" ", "") + File.separator;
		File dir = new File(path);
		if (!dir.exists()) {
			dir.mkdirs();
		}
		File file = new File(sopFile.getPath());
		if (file.exists() && CommonUtil.getLicense()) {
			Workbook book = null;
			try {
				book = new Workbook(sopFile.getPath());
				ImageOrPrintOptions imgOptions = new ImageOrPrintOptions();
				imgOptions.setImageType(ImageType.PNG);
				imgOptions.setDesiredSize(1920, 1080);

				for (int i = 0; i < book.getWorksheets().getCount(); i++) {
					Worksheet worksheet = book.getWorksheets().get(i);
					SheetRender sr = new SheetRender(worksheet, imgOptions);
					String pictureNumber = worksheet.getName();
					String pictureName = worksheet.getName();
					String picturePath;
					if (CommonUtil.isInteger(pictureNumber)) {
						pictureName = worksheet.getCells().get(4, 4).getStringValue();
						picturePath = path + pictureName + ".png";
					} else {
						picturePath = path + pictureNumber + ".png";
					}
					sr.toImage(0, picturePath);
					savePictureInfo(sopFile, pictureNumber, pictureName + ".png", picturePath);
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}


	private void savePictureInfo(SopFile sopFile, String pictureNumber, String pictureName, String picturePath) {
		SopFilePicture sopFilePicture = new SopFilePicture();
		sopFilePicture.setPictureName(pictureName).setPictureNumber(pictureNumber).setPicturePath(picturePath).setSopFileId(sopFile.getId()).save();
	}


	public File downloadFile(Integer id) {
		SopFile sopFile = SopFile.dao.findById(id);
		if (sopFile == null) {
			throw new OperationException("当前文件信息不存在");
		}
		File file = new File(sopFile.getPath());
		if (!file.exists()) {
			throw new OperationException("文件不存在,请重新上传");
		}
		return file;
	}


	public File downloadPicture(Integer id) {
		SopFilePicture sopFilePicture = SopFilePicture.dao.findById(id);
		if (sopFilePicture == null) {
			throw new OperationException("当前文件信息不存在");
		}
		File file = new File(sopFilePicture.getPicturePath());
		if (!file.exists()) {
			throw new OperationException("文件不存在,请重新上传");
		}
		return file;
	}


	public boolean addNotice(String title, String content, Date startTime, Date endTime) {
		SopNotice sopNotice = new SopNotice();
		sopNotice.setContent(content).setEndTime(endTime).setStartTime(startTime).setTitle(title);
		return sopNotice.save();
	}


	public boolean deleteNotice(Integer id) {
		SopNotice sopNotice = SopNotice.dao.findById(id);
		if (sopNotice == null) {
			throw new OperationException("当前通知/注意事项不存在");
		}
		List<SopSiteDisplay> sopSiteDisplays = SopSiteDisplay.dao.find(SopSQL.SELECT_SOPSITEDISPLAY);
		if (sopSiteDisplays != null && !sopSiteDisplays.isEmpty()) {
			for (SopSiteDisplay sopSiteDisplay : sopSiteDisplays) {
				if (StrKit.isBlank(sopSiteDisplay.getNotices())) {
					continue;
				}
				String[] noticeIds = sopSiteDisplay.getNotices().split(",");
				for (String noticeId : noticeIds) {
					if (!StrKit.isBlank(noticeId) && id.equals(Integer.parseInt(noticeId))) {
						throw new OperationException("当前通知正在被播放中");
					}
				}
			}
		}
		return sopNotice.delete();
	}


	public Page<Record> selectNotice(Integer pageNo, Integer pageSize, String title, String content, String startTimeFrom, String startTimeTo, String endTimeFrom, String endTimeTo, Boolean isAllSite) {
		SqlPara sqlPara = new SqlPara();
		StringBuilder sql = new StringBuilder(SopSQL.SELECT_SOPNOTICE);
		sql.append(" WHERE 1 = 1 ");
		if (!StrKit.isBlank(title)) {
			sql.append(" AND title like '%").append(title).append("%'");
		}
		if (!StrKit.isBlank(content)) {
			sql.append(" AND content like '%").append(content).append("%'");
		}
		if (!StrKit.isBlank(startTimeFrom)) {
			sql.append(" AND start_time >= '").append(startTimeFrom).append("'");
		}
		if (!StrKit.isBlank(startTimeTo)) {
			sql.append(" AND start_time <= '").append(startTimeTo).append("'");
		}
		if (!StrKit.isBlank(endTimeFrom)) {
			sql.append(" AND end_time >= '").append(endTimeFrom).append("'");
		}
		if (!StrKit.isBlank(endTimeTo)) {
			sql.append(" AND end_time <= '").append(endTimeTo).append("'");
		}
		if (isAllSite != null) {
			sql.append(" AND is_all_site = ").append(isAllSite);
		}
		if (StringUtils.endsWith(sql, "1 = 1 ")) {
			sql.delete(sql.lastIndexOf("WHERE"), sql.length());
		}
		sqlPara.setSql(sql.toString());
		return Db.paginate(pageNo, pageSize, sqlPara);
	}


	public boolean editNotice(Integer id, String title, String content, Date startTime, Date endTime) {
		SopNotice sopNotice = SopNotice.dao.findById(id);
		if (sopNotice == null) {
			throw new OperationException("当前通知/注意事项不存在");
		}
		if (!StrKit.isBlank(title)) {
			sopNotice.setTitle(title);
		}
		if (!StrKit.isBlank(content)) {
			sopNotice.setContent(content);
		}
		if (startTime != null) {
			sopNotice.setStartTime(startTime);
		}
		if (endTime != null) {
			sopNotice.setEndTime(endTime);
		}
		return sopNotice.update();
	}


	public Page<Record> selectActionLog(Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter) {
		return select(SopSQL.SELECT_ACTIONLOG, pageNo, pageSize, ascBy, descBy, filter);
	}


	public SopSite getSopSiteByMac(String mac) {
		return SopSite.dao.findFirst(SopSQL.SELECT_SITE_BY_MAC, mac);
	}


	public boolean addFaceInformation(String userName, String feature) {
		SopFaceInformation temp = SopFaceInformation.dao.findFirst(SopSQL.SELECT_FACEINFORMATION_BY_USERNAME, userName);
		if (temp != null) {
			throw new OperationException("用户名已存在");
		}
		SopFaceInformation faceInformation = new SopFaceInformation();
		faceInformation.setFeature(feature).setUserName(userName);
		return faceInformation.save();
	}


	public List<SopFaceInformation> getFaceInformation() {
		return SopFaceInformation.dao.find(SopSQL.SELECT_FACEINFORMATION);
	}


	public void addLoginLog(String userName, String time, String siteNumber, String type) {
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		Date loginTime;
		try {
			loginTime = dateFormat.parse(time);
		} catch (Exception e) {
			loginTime = new Date();
		}
		SopLoginLog sopLoginLog = new SopLoginLog();
		sopLoginLog.setTime(loginTime).setUserName(userName).setSiteNumber(siteNumber).setType(type).save();
	}


	public Page<Record> selectLoginLog(Integer pageNo, Integer pageSize, String startTime, String endTime, String userName, String siteNumber, String type) {
		SqlPara sqlPara = new SqlPara();
		StringBuilder sql = new StringBuilder(SopSQL.SELECT_LOGIN_LOG);
		sql.append(" WHERE 1 = 1 ");
		if (!StrKit.isBlank(userName)) {
			sql.append(" AND user_name like '%").append(userName).append("%'");
		}
		if (!StrKit.isBlank(siteNumber)) {
			sql.append(" AND site_number like '%").append(siteNumber).append("%'");
		}
		if (!StrKit.isBlank(type)) {
			sql.append(" AND type like '%").append(type).append("%'");
		}
		if (!StrKit.isBlank(startTime)) {
			sql.append(" AND time >= '").append(startTime).append("'");
		}
		if (!StrKit.isBlank(endTime)) {
			sql.append(" AND time <= '").append(endTime).append("'");
		}
		if (StringUtils.endsWith(sql, "1 = 1 ")) {
			sql.delete(sql.lastIndexOf("WHERE"), sql.length());
		}
		sqlPara.setSql(sql.toString());
		return Db.paginate(pageNo, pageSize, sqlPara);
	}


	public ResultUtil dispatchFile(String list, LUserAccountVO userVO) {
		List<sendMessageInfo> sendMessageInfos = JSONObject.parseArray(list, sendMessageInfo.class);
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		for (sendMessageInfo sendMessageInfo : sendMessageInfos) {
			StringBuilder noticeIdBuilder = new StringBuilder();
			StringBuilder pictureIdBuilder = new StringBuilder();
			StringBuilder fileIdBuilder = new StringBuilder();
			Session session = SessionBox.getSessionById(sendMessageInfo.getId());
			if (session == null) {
				throw new OperationException("客户端不在线或不存在");
			}
			SopSite sopSite = SopSite.dao.findById(sendMessageInfo.getId());
			if (sopSite == null) {
				throw new OperationException("当前站点记录不存在");
			}
			Record siteRecord = Db.findFirst(SopSQL.SELECT_SITE_JOIN_LINE_WORKSHOP_FACTORY, sendMessageInfo.getId());
			JSONObject requestBody = new JSONObject();
			List<SopNoticeHistory> sopNoticeHistories = new ArrayList<SopNoticeHistory>();
			List<SopPictureHistory> sopPictureHistories = new ArrayList<SopPictureHistory>();
			if (sendMessageInfo.getNoticeList() != null && sendMessageInfo.getNoticeList().length > 0) {
				List<Notice> notices = new ArrayList<Notice>();
				for (Integer noticeId : sendMessageInfo.getNoticeList()) {
					SopNotice sopNotice = SopNotice.dao.findById(noticeId);
					if (sopNotice == null) {
						continue;
					}
					SopNoticeHistory sopNoticeHistory = new SopNoticeHistory();
					sopNoticeHistory.setTitle(sopNotice.getTitle()).setContent(sopNotice.getContent());
					sopNoticeHistories.add(sopNoticeHistory);
					noticeIdBuilder.append(noticeId + ",");
					Notice notice = new Notice();
					notice.setTitle(sopNotice.getTitle());
					notice.setContent(sopNotice.getContent());
					notice.setStartTime(dateFormat.format(sopNotice.getStartTime()));
					notice.setEndTime(dateFormat.format(sopNotice.getEndTime()));
					notices.add(notice);
				}
				requestBody.put("notice", notices);
			}
			if (sendMessageInfo.getPictureList() != null && sendMessageInfo.getPictureList().length > 0) {
				List<Picture> pictures = new ArrayList<Picture>();
				for (Integer pictureId : sendMessageInfo.getPictureList()) {
					SopFilePicture sopFilePicture = SopFilePicture.dao.findById(pictureId);
					if (sopFilePicture == null) {
						continue;
					}
					pictureIdBuilder.append(pictureId + ",");
					fileIdBuilder.append(sopFilePicture.getSopFileId() + ",");
					String picturePath = sopFilePicture.getPicturePath();
					String urlPath = picturePath.substring(picturePath.indexOf("mes_document")).replace("\\", "/");
					Picture picture = new Picture();
					picture.setPath(Constant.SOP_PICTURE_URL + urlPath);
					pictures.add(picture);
					SopPictureHistory sopPictureHistory = new SopPictureHistory();
					sopPictureHistory.setPictureName(sopFilePicture.getPictureName());
					sopPictureHistory.setPictureNumber(sopFilePicture.getPictureNumber());
					sopPictureHistory.setPicturePath(Constant.SOP_PICTURE_URL + urlPath);
					sopPictureHistories.add(sopPictureHistory);
				}
				requestBody.put("picture", pictures);
			}
			requestBody.put("switchInterval", sopSite.getSwitchInterval());
			JSONObject response = null;
			Date time = new Date();
			try {
				response = Pasta.sendRequest(session, RequestType.SHOW, requestBody);
			} catch (Exception e) {
				throw new OperationException("发送播放指令到ID为 " + sopSite.getId() + " 的站点出错");
			}
			throwExceptionIfExistByResult(response);
			playingFile(fileIdBuilder);
			saveSopSiteDisplay(sendMessageInfo.getId(), noticeIdBuilder, pictureIdBuilder, fileIdBuilder);
			saveSopNoticeHistory(sopNoticeHistories, userVO, time, siteRecord);
			saveSopFileHistory(sopPictureHistories, fileIdBuilder, userVO, time, siteRecord);
		}
		return ResultUtil.succeed();
	}


	private void playingFile(StringBuilder fileIdBuilder) {
		for (String fileId : fileIdBuilder.toString().split(",")) {
			if (!StrKit.isBlank(fileId)) {
				editFileState(Integer.parseInt(fileId), SopFileState.PLAYING_STATE.getName(), null);
			}
		}
	}


	private void saveSopSiteDisplay(Integer siteId, StringBuilder noticeIdBuilder, StringBuilder pictureIdBuilder, StringBuilder fileIdBuilder) {
		SopSiteDisplay sopSiteDisplay = SopSiteDisplay.dao.findFirst(SopSQL.SELECT_SITEDISPLAY_BY_SITE, siteId);
		if (sopSiteDisplay == null) {
			sopSiteDisplay = new SopSiteDisplay();
			sopSiteDisplay.setSiteId(siteId).setFiles(fileIdBuilder.toString()).setNotices(noticeIdBuilder.toString()).setPictures(pictureIdBuilder.toString()).save();
		} else {
			if (StrKit.isBlank(sopSiteDisplay.getFiles())) {
				sopSiteDisplay.setFiles(fileIdBuilder.toString()).setNotices(noticeIdBuilder.toString()).setPictures(pictureIdBuilder.toString()).update();
			} else {
				sopSiteDisplay.setFiles(sopSiteDisplay.getFiles() + fileIdBuilder.toString());
				sopSiteDisplay.setNotices(sopSiteDisplay.getNotices() + noticeIdBuilder.toString());
				sopSiteDisplay.setPictures(sopSiteDisplay.getPictures() + pictureIdBuilder.toString()).update();
			}
		}
	}


	private void saveSopNoticeHistory(List<SopNoticeHistory> sopNoticeHistories, LUserAccountVO userVO, Date time, Record siteRecord) {
		for (SopNoticeHistory sopNoticeHistory : sopNoticeHistories) {
			sopNoticeHistory.setFactory(siteRecord.getStr("abbreviation")).setLine(siteRecord.getStr("line_name")).setSiteName("site_name");
			sopNoticeHistory.setSiteNumber(siteRecord.getStr("site_number")).setWorkshop(siteRecord.getStr("workshop_name"));
			sopNoticeHistory.setPushPerson(userVO.getName()).setPushTime(time);
		}
		Db.batchSave(sopNoticeHistories, sopNoticeHistories.size());
	}


	private void saveSopFileHistory(List<SopPictureHistory> sopPictureHistories, StringBuilder fileIdBuilder, LUserAccountVO userVO, Date time, Record siteRecord) {
		String[] fileIds = fileIdBuilder.toString().split(",");
		for (String fileId : fileIds) {
			if (!StrKit.isBlank(fileId)) {
				SopFileHistory sopFileHistory = new SopFileHistory();
				sopFileHistory.setFileId(Integer.parseInt(fileId)).setPushPerson(userVO.getName()).setPushTime(time).save();
				saveSopPictureHistory(sopPictureHistories, siteRecord, sopFileHistory.getId());

			}
		}
	}


	private void saveSopPictureHistory(List<SopPictureHistory> sopPictureHistories, Record siteRecord, Integer sopFileHistoryId) {
		for (SopPictureHistory sopPictureHistory : sopPictureHistories) {
			sopPictureHistory.setFactory(siteRecord.getStr("abbreviation")).setLine(siteRecord.getStr("line_name")).setSiteName("site_name");
			sopPictureHistory.setSiteNumber(siteRecord.getStr("site_number")).setWorkshop(siteRecord.getStr("workshop_name"));
			sopPictureHistory.setFileHistoryId(sopFileHistoryId);
		}
		Db.batchSave(sopPictureHistories, sopPictureHistories.size());
	}


	private void throwExceptionIfExistByResult(JSONObject response) {
		if (response.getInteger("result").toString().equals("200")) {
			return;
		} else {
			throw new OperationException(response.getString("data"));
		}
	}


	public ResultUtil recycleFile(String id) {
		for (String siteId : id.split(",")) {
			SopSite sopSite = SopSite.dao.findById(Integer.parseInt(siteId));
			if (sopSite == null) {
				throw new OperationException("ID为 " + siteId + " 的站点信息不存在");
			}
			Session session = SessionBox.getSessionById(Integer.parseInt(siteId));
			if (session == null) {
				throw new OperationException("ID为 " + siteId + " 的站点客户端不在线或不存在");
			}
			JSONObject response = null;
			try {
				response = Pasta.sendRequest(session, RequestType.CANCELSHOW, null);
			} catch (Exception e) {
				throw new OperationException("发送取消播放指令到ID为 " + siteId + " 的站点出错");
			}
			throwExceptionIfExistByResult(response);
			stopPlayFile(Integer.parseInt(siteId));
			removeSopSiteDisplay(sopSite);
		}
		return ResultUtil.succeed();
	}


	private void stopPlayFile(Integer siteId) {
		SopSiteDisplay sopSiteDisplay = SopSiteDisplay.dao.findFirst(SopSQL.SELECT_SITEDISPLAY_BY_SITE, siteId);
		if (sopSiteDisplay != null) {
			if (!StrKit.isBlank(sopSiteDisplay.getFiles())) {
				List<SopFile> sopFiles = new ArrayList<>();
				String[] fileIds = sopSiteDisplay.getFiles().split(",");
				for (String fileId : fileIds) {
					SopFile sopFile = SopFile.dao.findById(Integer.parseInt(fileId));
					if (sopFile != null) {
						sopFile.setState(SopFileState.REVIEWED_STATE.getName());
						sopFiles.add(sopFile);
					}
				}
				Db.batchUpdate(sopFiles, sopFiles.size());
			}
		}
	}


	private void removeSopSiteDisplay(SopSite sopSite) {
		SopSiteDisplay sopSiteDisplay = SopSiteDisplay.dao.findFirst(SopSQL.SELECT_SITEDISPLAY_BY_SITE, sopSite.getId());
		if (sopSiteDisplay != null) {
			sopSiteDisplay.setFiles(null).setNotices(null).setPictures(null).update();
		}
	}


	public PreviewInfo previewSite(Integer id) {
		SopSite sopSite = SopSite.dao.findById(id);
		if (sopSite == null) {
			throw new OperationException("当前站点信息不存在");
		}
		Record record = Db.findFirst(SopSQL.SELECT_SITEDISPLAY_BY_SITE, id);
		if (record != null) {
			PreviewInfo previewInfo = new PreviewInfo();
			String pictureIds = record.getStr("pictures");
			String noticeIds = record.getStr("notices");
			if (StringUtils.isAllBlank(pictureIds, noticeIds, record.getStr("files"))) {
				throw new OperationException("当前站点没有正在播放的内容");
			}
			if (!StrKit.isBlank(pictureIds)) {
				List<Picture> pictures = new ArrayList<>();
				for (String pictureId : pictureIds.split(",")) {
					if (StrKit.isBlank(pictureId)) {
						continue;
					}
					SopFilePicture sopFilePicture = SopFilePicture.dao.findById(Integer.parseInt(pictureId));
					if (sopFilePicture == null) {
						throw new OperationException("当前站点正在播放的图片不存在");
					}
					Picture picture = new Picture();
					String picturePath = sopFilePicture.getPicturePath();
					String urlPath = picturePath.substring(picturePath.indexOf("mes_document")).replace("\\", "/");
					picture.setPath(Constant.SOP_PICTURE_URL + urlPath);
					pictures.add(picture);
				}
				previewInfo.setPictures(pictures);
			}
			if (!StrKit.isBlank(noticeIds)) {
				List<Notice> notices = new ArrayList<>();
				for (String noticeId : noticeIds.split(",")) {
					if (StrKit.isBlank(noticeId)) {
						continue;
					}
					SopNotice sopNotice = SopNotice.dao.findById(Integer.parseInt(noticeId));
					if (sopNotice == null) {
						throw new OperationException("当前站点正在播放的通知不存在");
					}
					Notice notice = new Notice();
					notice.setContent(sopNotice.getContent());
					notice.setTitle(sopNotice.getTitle());
					notices.add(notice);
				}
				previewInfo.setNotices(notices);
			}
			return previewInfo;
		}
		return null;
	}


	public List<PreviewInfo> previewDispatchingFile(String list) {
		List<sendMessageInfo> sendMessageInfos = JSONObject.parseArray(list, sendMessageInfo.class);
		List<PreviewInfo> previewInfos = new ArrayList<>();
		for (sendMessageInfo sendMessageInfo : sendMessageInfos) {
			SopSite sopSite = SopSite.dao.findById(sendMessageInfo.getId());
			if (sopSite == null) {
				throw new OperationException("当前站点记录不存在");
			}
			PreviewInfo previewInfo = new PreviewInfo();
			if (sendMessageInfo.getNoticeList() != null && sendMessageInfo.getNoticeList().length > 0) {
				List<Notice> notices = new ArrayList<Notice>();
				for (Integer noticeId : sendMessageInfo.getNoticeList()) {
					SopNotice sopNotice = SopNotice.dao.findById(noticeId);
					if (sopNotice == null) {
						continue;
					}
					Notice notice = new Notice();
					notice.setTitle(sopNotice.getTitle());
					notice.setContent(sopNotice.getContent());
					notices.add(notice);
				}
				previewInfo.setNotices(notices);
			}
			if (sendMessageInfo.getPictureList() != null && sendMessageInfo.getPictureList().length > 0) {
				List<Picture> pictures = new ArrayList<Picture>();
				for (Integer pictureId : sendMessageInfo.getPictureList()) {
					SopFilePicture sopFilePicture = SopFilePicture.dao.findById(pictureId);
					if (sopFilePicture == null) {
						continue;
					}
					String picturePath = sopFilePicture.getPicturePath();
					String urlPath = picturePath.substring(picturePath.indexOf("mes_document")).replace("\\", "/");
					Picture picture = new Picture();
					picture.setPath(Constant.SOP_PICTURE_URL + urlPath);
					pictures.add(picture);
				}
				previewInfo.setPictures(pictures);
			}
			previewInfos.add(previewInfo);
		}
		return previewInfos;
	}


	public Page<Record> selectFileHistory(Integer pageNo, Integer pageSize, Integer fileId, String startTime, String endTime, String pushPerson) {
		SqlPara sqlPara = new SqlPara();
		StringBuilder sql = new StringBuilder(SopSQL.SELECT_FILEHISTORY_JOIN_FILE);
		if (!StrKit.isBlank(pushPerson)) {
			sql.append(" AND h.push_person like '%").append(pushPerson).append("%'");
		}
		if (!StrKit.isBlank(startTime)) {
			sql.append(" AND h.push_time >= '").append(startTime).append("'");
		}
		if (!StrKit.isBlank(endTime)) {
			sql.append(" AND h.push_time <= '").append(endTime).append("'");
		}
		sql.append(" ORDER BY h.id");
		sqlPara.setSql(sql.toString());
		sqlPara.addPara(fileId);
		return Db.paginate(pageNo, pageSize, sqlPara);
	}


	public List<FileHistoryDetail> selectFileHistoryDetail(Integer fileHistoryId) {
		List<SopPictureHistory> sopPictureHistories = SopPictureHistory.dao.find(SopSQL.SELECT_FILEPICTUREHISTORY_BY_FILEHISTORYID, fileHistoryId);
		if (sopPictureHistories != null && !sopPictureHistories.isEmpty()) {
			Map<String, FileHistoryDetail> detailMap = new HashMap<String, FileHistoryDetail>();
			for (SopPictureHistory sopPictureHistory : sopPictureHistories) {
				FileHistoryDetail detail = new FileHistoryDetail();
				detail.setFactory(sopPictureHistory.getFactory());
				detail.setLine(sopPictureHistory.getLine());
				detail.setSiteName(sopPictureHistory.getSiteName());
				detail.setSiteNumber(sopPictureHistory.getSiteNumber());
				detail.setWorkshop(sopPictureHistory.getFactory());

				PictureVO picture = new PictureVO();
				picture.setPictureName(sopPictureHistory.getPictureName());
				picture.setPictureNumber(sopPictureHistory.getPictureNumber());
				picture.setPicturePath(sopPictureHistory.getPicturePath());
				String key = detail.getSiteName() + detail.getSiteNumber() + detail.getLine() + detail.getWorkshop() + detail.getFactory();
				if (sopPictureHistory.get(key) == null) {
					List<PictureVO> pictures = new ArrayList<>();
					pictures.add(picture);
					detail.setPictures(pictures);
					detailMap.put(key, detail);
				} else {
					detailMap.get(key).getPictures().add(picture);
				}
			}
			List<FileHistoryDetail> details = new ArrayList<>();
			for (Entry<String, FileHistoryDetail> detailEntry : detailMap.entrySet()) {
				details.add(detailEntry.getValue());
			}
			return details;
		}
		return null;
	}


	public Page<Record> selectNoticeHistory(Integer pageNo, Integer pageSize, String siteNumber, String siteName, String line, String workshop, String factory, String startTime, String endTime, String title, String content, String pushPerson) {
		SqlPara sqlPara = new SqlPara();
		StringBuilder sql = new StringBuilder(SopSQL.SELECT_NOTICEHISTORY);
		if (!StrKit.isBlank(siteNumber)) {
			sql.append(" AND site_number like '%").append(siteNumber).append("%'");
		}
		if (!StrKit.isBlank(siteName)) {
			sql.append(" AND site_name like '%").append(siteName).append("%'");
		}
		if (!StrKit.isBlank(line)) {
			sql.append(" AND line like '%").append(line).append("%'");
		}
		if (!StrKit.isBlank(workshop)) {
			sql.append(" AND workshop like '%").append(workshop).append("%'");
		}
		if (!StrKit.isBlank(factory)) {
			sql.append(" AND factory like '%").append(factory).append("%'");
		}
		if (!StrKit.isBlank(title)) {
			sql.append(" AND title like '%").append(title).append("%'");
		}
		if (!StrKit.isBlank(content)) {
			sql.append(" AND content like '%").append(content).append("%'");
		}
		if (!StrKit.isBlank(pushPerson)) {
			sql.append(" AND push_person like '%").append(pushPerson).append("%'");
		}
		if (!StrKit.isBlank(startTime)) {
			sql.append(" AND push_time >= '").append(startTime).append("'");
		}
		if (!StrKit.isBlank(endTime)) {
			sql.append(" AND push_time <= '").append(endTime).append("'");
		}
		sql.append(" ORDER BY id desc");
		sqlPara.setSql(sql.toString());
		return Db.paginate(pageNo, pageSize, sqlPara);
	}

}
