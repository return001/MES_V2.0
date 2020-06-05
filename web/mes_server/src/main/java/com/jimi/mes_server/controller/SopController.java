package com.jimi.mes_server.controller;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import org.apache.commons.lang3.StringUtils;
import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jfinal.kit.StrKit;
import com.jfinal.upload.UploadFile;
import com.jimi.mes_server.annotation.Log;
import com.jimi.mes_server.annotation.PermissionPass;
import com.jimi.mes_server.entity.Constant;
import com.jimi.mes_server.entity.SopFileState;
import com.jimi.mes_server.entity.SopRecycleFileType;
import com.jimi.mes_server.entity.SopSiteState;
import com.jimi.mes_server.entity.vo.LUserAccountVO;
import com.jimi.mes_server.exception.OperationException;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.service.SopService;
import com.jimi.mes_server.util.CommonUtil;
import com.jimi.mes_server.util.ResultUtil;
import com.jimi.mes_server.util.TokenBox;

public class SopController extends Controller {

	private static SopService sopService = Enhancer.enhance(SopService.class);


	/**@author HCJ
	 * 添加工厂
	 * @param factoryAlias 别名
	 * @param abbreviation 简称
	 * @param fullName 全称
	 * @date 2019年10月24日 下午2:14:42
	 */
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


	/**@author HCJ
	 * 删除工厂
	 * @param id 工厂ID
	 * @date 2019年10月24日 下午2:15:14
	 */
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


	/**@author HCJ
	 * 查询工厂
	 * @param pageNo 页码
	 * @param pageSize 页大小
	 * @param factoryAlias 别名
	 * @param abbreviation 简称
	 * @param fullName 全称
	 * @date 2019年10月24日 下午2:15:31
	 */
	public void selectFactory(Integer pageNo, Integer pageSize, String factoryAlias, String abbreviation, String fullName) {
		renderJson(ResultUtil.succeed(sopService.selectFactory(pageNo, pageSize, factoryAlias, abbreviation, fullName)));
	}


	/**@author HCJ
	 * 编辑工厂
	 * @param id 工厂ID
	 * @param factoryAlias 别名
	 * @param abbreviation 简称
	 * @param fullName 全称
	 * @date 2019年10月24日 下午2:16:03
	 */
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


	/**@author HCJ
	 * 增加车间
	 * @param factoryId 工厂ID
	 * @param workshopNumber 车间编号
	 * @param workshopName 车间名称
	 * @date 2019年10月24日 下午2:16:28
	 */
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


	/**@author HCJ
	 * 删除车间
	 * @param id 车间ID
	 * @date 2019年10月24日 下午2:17:09
	 */
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


	/**@author HCJ
	 * 查询车间
	 * @param pageNo 页码
	 * @param pageSize 页大小
	 * @param factoryId 工厂ID
	 * @param workshopNumber 车间编号
	 * @param workshopName 车间名称
	 * @date 2019年10月24日 下午2:17:26
	 */
	public void selectWorkshop(Integer pageNo, Integer pageSize, Integer factoryId, String workshopNumber, String workshopName) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		renderJson(ResultUtil.succeed(sopService.selectWorkshop(pageNo, pageSize, factoryId, workshopNumber, workshopName)));
	}


	/**@author HCJ
	 * 编辑车间
	 * @param id 车间ID
	 * @param factoryId 工厂ID
	 * @param workshopNumber 车间编号
	 * @param workshopName 车间名称
	 * @date 2019年10月24日 下午2:17:54
	 */
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


	/**@author HCJ
	 * 添加站点
	 * @param siteNumber 站点编号
	 * @param siteName 站点名称
	 * @param processOrder 工序顺序
	 * @param lineId 产线ID
	 * @param playTimes 播放次数
	 * @param switchInterval 切换间隔
	 * @param mac MAC地址
	 * @date 2019年10月24日 下午2:18:16
	 */
	public void addSite(String siteNumber, String siteName, Integer processOrder, Integer lineId, Integer playTimes, Integer switchInterval, String mac, String secondMac) {
		if (processOrder == null || lineId == null || switchInterval == null || StringUtils.isAnyBlank(siteNumber, siteName, mac)) {
			throw new ParameterException("参数不能为空");
		}
		if (!CommonUtil.isMac(mac)) {
			throw new ParameterException("MAC地址无效");
		}
		if (!StrKit.isBlank(secondMac)) {
			if (!CommonUtil.isMac(secondMac)) {
				throw new ParameterException("第二个MAC地址无效");
			}
			if (secondMac.equals(mac)) {
				throw new ParameterException("两个MAC地址不能相同");
			}
		}
		if (sopService.addSite(siteNumber, siteName, processOrder, lineId, playTimes, switchInterval, mac, secondMac)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 删除站点
	 * @param id 站点ID
	 * @date 2019年10月24日 下午2:21:27
	 */
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


	/**@author HCJ
	 * 查询站点
	 * @param pageNo 页码
	 * @param pageSize 页大小
	 * @param siteNumber 站点编号
	 * @param siteName 站点名称
	 * @param processOrder 工序顺序
	 * @param lineId 产线ID
	 * @date 2019年10月24日 下午2:21:44
	 */
	public void selectSite(Integer pageNo, Integer pageSize, String siteNumber, String siteName, Integer processOrder, Integer lineId, Boolean orderByProcessOrder) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		renderJson(ResultUtil.succeed(sopService.selectSite(pageNo, pageSize, siteNumber, siteName, processOrder, lineId, orderByProcessOrder)));
	}


	/**@author HCJ
	 * 编辑站点
	 * @param id 站点ID
	 * @param siteNumber 站点编号
	 * @param siteName 站点名称
	 * @param processOrder 工序顺序
	 * @param lineId 产线ID
	 * @param playTimes 播放次数
	 * @param switchInterval 切换间隔
	 * @param mac MAC地址
	 * @date 2019年10月24日 下午2:22:27
	 */
	public void editSite(Integer id, String siteNumber, String siteName, Integer processOrder, Integer lineId, Integer playTimes, Integer switchInterval, String mac, String secondMac) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.editSite(id, siteNumber, siteName, processOrder, lineId, playTimes, switchInterval, mac, secondMac)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Log("品质检查站点播放信息，站点的ID是：{id}，结果：{state}")
	public void editSiteState(Integer id, String state) {
		if (id == null || StrKit.isBlank(state)) {
			throw new ParameterException("参数不能为空");
		}
		if (!state.equals(SopSiteState.PASS.getName()) && !state.equals(SopSiteState.FAIL.getName())) {
			throw new OperationException("无效的状态修改操作");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		if (sopService.editSiteState(id, state, userVO)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 增加客户
	 * @param customerNumber 客户编号
	 * @param customerName 客户名称
	 * @param factoryId 工厂ID
	 * @date 2019年10月24日 下午2:23:14
	 */
	public void addCustomer(String customerNumber, String customerName, Integer factoryId) {
		if (factoryId == null || StringUtils.isAnyBlank(customerNumber, customerName)) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.addCustomer(customerNumber, customerName, factoryId)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 删除客户
	 * @param id 客户ID
	 * @date 2019年10月24日 下午2:23:35
	 */
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


	/**@author HCJ
	 * 查询客户
	 * @param pageNo 页码
	 * @param pageSize 页大小
	 * @param customerNumber 客户编号
	 * @param customerName 客户名称
	 * @param factoryId 工厂ID
	 * @date 2019年10月24日 下午2:23:48
	 */
	public void selectCustomer(Integer pageNo, Integer pageSize, String customerNumber, String customerName, Integer factoryId) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		renderJson(ResultUtil.succeed(sopService.selectCustomer(pageNo, pageSize, customerNumber, customerName, factoryId)));
	}


	/**@author HCJ
	 * 编辑客户
	 * @param id 客户ID
	 * @param customerNumber 客户编号
	 * @param customerName 客户名称
	 * @param factoryId 工厂ID
	 * @date 2019年10月24日 下午2:24:14
	 */
	public void editCustomer(Integer id, String customerNumber, String customerName, Integer factoryId) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.editCustomer(id, customerNumber, customerName, factoryId)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 添加系列机型
	 * @param seriesModelName 名称
	 * @date 2019年10月24日 下午2:24:32
	 */
	public void addSeriesModel(String seriesModelName) {
		if (StrKit.isBlank(seriesModelName)) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.addSeriesModel(seriesModelName)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 删除系列机型
	 * @param id 机型ID
	 * @date 2019年10月24日 下午2:24:47
	 */
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


	/**@author HCJ
	 * 查询系列机型
	 * @param pageNo 页码
	 * @param pageSize 页大小
	 * @param seriesModelName 系列机型名称
	 * @date 2019年10月24日 下午2:25:05
	 */
	public void selectSeriesModel(Integer pageNo, Integer pageSize, String seriesModelName) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		renderJson(ResultUtil.succeed(sopService.selectSeriesModel(pageNo, pageSize, seriesModelName)));
	}


	/**@author HCJ
	 * 编辑系列机型
	 * @param id 机型ID
	 * @param seriesModelName 机型名称
	 * @date 2019年10月24日 下午2:25:24
	 */
	public void editSeriesModel(Integer id, String seriesModelName) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.editSeriesModel(id, seriesModelName)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 增加产品机型
	 * @param productModelName 机型名
	 * @param seriesModelId 系列机型ID
	 * @date 2019年10月24日 下午2:25:42
	 */
	public void addProductModel(String productModelName, Integer seriesModelId) {
		if (StrKit.isBlank(productModelName) || seriesModelId == null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.addProductModel(productModelName, seriesModelId)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 删除产品机型
	 * @param id 产品机型ID
	 * @date 2019年10月24日 下午2:26:03
	 */
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


	/**@author HCJ
	 * 查询产品机型
	 * @param pageNo 页码
	 * @param pageSize 页大小
	 * @param productModelName 机型名
	 * @param seriesModelId 系列机型ID
	 * @date 2019年10月24日 下午2:26:22
	 */
	public void selectProductModel(Integer pageNo, Integer pageSize, String productModelName, Integer seriesModelId) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		renderJson(ResultUtil.succeed(sopService.selectProductModel(pageNo, pageSize, productModelName, seriesModelId)));
	}


	/**@author HCJ
	 * 编辑产品机型
	 * @param id 产品机型ID
	 * @param productModelName 机型名
	 * @param seriesModelId 系列机型ID
	 * @date 2019年10月24日 下午2:26:51
	 */
	public void editProductModel(Integer id, String productModelName, Integer seriesModelId) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.editProductModel(id, productModelName, seriesModelId)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 上传文件
	 * @date 2019年10月24日 下午2:27:17
	 */
	public void importFiles() {
		List<UploadFile> uploadFiles = getFiles();
		if (uploadFiles == null || uploadFiles.isEmpty()) {
			throw new ParameterException("请添加文件");
		}
		for (UploadFile uploadFile : uploadFiles) {
			if (!uploadFile.getOriginalFileName().endsWith(".xls") && !uploadFile.getOriginalFileName().endsWith(".xlsx")) {
				throw new ParameterException("只能上传Excel文件");
			}
		}
		if (sopService.importFiles(uploadFiles)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 删除文件
	 * @param id 文件ID
	 * @date 2019年10月24日 下午2:27:27
	 */
	@Log("删除文件，文件的ID是：{id}")
	public void deleteFile(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.deleteFile(id)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 查询文件
	 * @param pageNo 页码
	 * @param pageSize 页大小
	 * @param fileNumber 文件编号
	 * @param fileName 文件名称
	 * @param version 版本
	 * @param customer 客户
	 * @param seriesModel 系列机型
	 * @param productModel 产品机型
	 * @param reviewer 审核者
	 * @param state 状态
	 * @param reviewStartTime 起始审核时间
	 * @param reviewEndTime 结束审核时间
	 * @date 2019年10月24日 下午2:27:41
	 */
	public void selectFiles(Integer pageNo, Integer pageSize, String fileNumber, String fileName, String version, String customer, String seriesModel, String productModel, String reviewer, String state, String reviewStartTime, String reviewEndTime) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		renderJson(ResultUtil.succeed(sopService.selectFiles(pageNo, pageSize, fileNumber, fileName, version, customer, seriesModel, productModel, reviewer, state, reviewStartTime, reviewEndTime)));
	}


	/**@author HCJ
	 * 查询文件的图片信息
	 * @param pageNo 页码
	 * @param pageSize 页大小
	 * @param fileId 文件ID
	 * @date 2019年10月24日 下午2:28:46
	 */
	public void selectFilePictures(Integer pageNo, Integer pageSize, Integer fileId) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		if (fileId == null) {
			throw new ParameterException("参数不能为空");
		}
		renderJson(ResultUtil.succeed(sopService.selectFilePictures(pageNo, pageSize, fileId)));
	}


	/**@author HCJ
	 * 编辑文件状态
	 * @param id 文件ID
	 * @param state 状态
	 * @date 2019年10月24日 下午2:29:07
	 */
	@Log("修改文件状态，文件的ID是：{id}，修改的状态：{state}")
	public void editFileState(Integer id, String state) {
		if (id == null || StrKit.isBlank(state)) {
			throw new ParameterException("参数不能为空");
		}
		if (!state.equals(SopFileState.REVIEWED_STATE.getName()) && !state.equals(SopFileState.INVALID_STATE.getName())) {
			throw new OperationException("无效的状态修改操作");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		if (sopService.editFileState(id, state, userVO)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 下载文件
	 * @param id 文件ID
	 * @date 2019年10月24日 下午2:29:24
	 */
	public void downloadFile(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		CommonUtil.downloadFile(sopService.downloadFile(id), getResponse());
		renderNull();
	}


	/**@author HCJ
	 * 下载图片
	 * @param id 图片ID
	 * @date 2019年10月24日 下午2:29:37
	 */
	public void downloadPicture(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		CommonUtil.downloadFile(sopService.downloadPicture(id), getResponse());
		renderNull();
	}


	/**@author HCJ
	 * 添加通知
	 * @param title 标题
	 * @param content 内容
	 * @param startTime 开始播放时间
	 * @param endTime 结束播放时间
	 * @date 2019年10月24日 下午2:29:50
	 */
	public void addNotice(String title, String content, String startTime, String endTime) {
		if (StringUtils.isAnyBlank(title, content)) {
			throw new ParameterException("参数不能为空");
		}
		Map<String, Date> timeMap = getStartAndEndTime(startTime, endTime);
		if (sopService.addNotice(title, content, timeMap.get("start"), timeMap.get("end"))) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 删除通知
	 * @param id 通知ID
	 * @date 2019年10月24日 下午2:30:17
	 */
	public void deleteNotice(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.deleteNotice(id)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 查询通知
	 * @param pageNo 页码
	 * @param pageSize 页大小
	 * @param title 标题
	 * @param content 内容
	 * @param startTimeFrom 开始播放时间起始
	 * @param startTimeTo 开始播放时间终止
	 * @param endTimeFrom 结束播放时间起始
	 * @param endTimeTo 结束播放时间终止
	 * @date 2019年10月24日 下午2:30:32
	 */
	public void selectNotice(Integer pageNo, Integer pageSize, String title, String content, String startTimeFrom, String startTimeTo, String endTimeFrom, String endTimeTo) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		renderJson(ResultUtil.succeed(sopService.selectNotice(pageNo, pageSize, title, content, startTimeFrom, startTimeTo, endTimeFrom, endTimeTo)));
	}


	/**@author HCJ
	 * 编辑通知
	 * @param id 通知ID
	 * @param title 标题
	 * @param content 内容
	 * @param startTime 开始播放时间
	 * @param endTime 结束播放时间
	 * @date 2019年10月24日 下午2:33:24
	 */
	public void editNotice(Integer id, String title, String content, String startTime, String endTime) {
		if (id == null || StringUtils.isAnyBlank(title, content)) {
			throw new ParameterException("参数不能为空");
		}
		Map<String, Date> timeMap = getStartAndEndTime(startTime, endTime);
		if (sopService.editNotice(id, title, content, timeMap.get("start"), timeMap.get("end"))) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 查询接口日志
	 * @param currentPage 当前页
	 * @param pageSize 页大小
	 * @param ascBy 以哪个字段增序
	 * @param descBy 以哪个字段降序
	 * @param filter 查询条件
	 * @date 2019年9月24日 下午3:26:34
	 */
	public void selectActionLog(Integer pageNo, Integer pageSize, String timeFrom, String timeTo, String uid, String resultCode) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		renderJson(ResultUtil.succeed(sopService.selectActionLog(pageNo, pageSize, timeFrom, timeTo, uid, resultCode)));
	}


	/**@author HCJ
	 * 将字符串类型的开始和结束时间转换为date类型，存储到map
	 * @param startTime 开始时间
	 * @param endTime 结束时间
	 * @date 2019年10月24日 下午2:33:58
	 */
	private Map<String, Date> getStartAndEndTime(String startTime, String endTime) {
		Map<String, Date> map = Collections.synchronizedMap(new HashMap<String, Date>());
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		Date start = null;
		Date end = null;
		if (startTime != null && endTime != null) {
			try {
				start = dateFormat.parse(startTime);
				end = dateFormat.parse(endTime);
			} catch (ParseException e) {
				throw new ParameterException("时间格式出错");
			}
			if (start.compareTo(end) >= Constant.INTEGER_ZERO) {
				throw new ParameterException("开始时间不能晚于或者等于结束时间");
			}
			if (((end.getTime() - start.getTime()) / (1000 * 3600 * 24)) > 20) {
				throw new ParameterException("开始时间和结束时间的间隔不能超过20天");
			}
		}
		map.put("start", start);
		map.put("end", end);
		return map;
	}


	/**@author HCJ
	 * 增加人脸信息，客户端调用
	 * @param userName 用户名
	 * @param feature 人脸特征
	 * @date 2019年10月24日 下午2:35:29
	 */
	@PermissionPass
	public void addFaceInformation(String userName, String feature) {
		if (StringUtils.isAnyBlank(userName, feature)) {
			throw new OperationException("用户名和人脸特征不能为空");
		}
		if (sopService.addFaceInformation(userName, feature)) {
			renderJson(ResultUtil.succeed("保存成功"));
		} else {
			renderJson(ResultUtil.failed(412, "保存失败"));
		}
	}


	/**@author HCJ
	 * 获取人脸信息，客户端调用
	 * @date 2019年10月24日 下午2:35:54
	 */
	@PermissionPass
	public void getFaceInformation() {
		renderJson(ResultUtil.succeed(sopService.getFaceInformation()));
	}


	/**@author HCJ
	 * 查询客户端登录、登出日志
	 * @param pageNo 页码
	 * @param pageSize 页大小
	 * @param startTime 开始时间
	 * @param endTime 结束时间
	 * @param userName 用户名
	 * @param siteNumber 站点编号
	 * @param type 日志类型
	 * @date 2019年10月24日 下午2:36:09
	 */
	public void selectLoginLog(Integer pageNo, Integer pageSize, String timeFrom, String timeTo, String userName, String siteNumber, String type) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		renderJson(ResultUtil.succeed(sopService.selectLoginLog(pageNo, pageSize, timeFrom, timeTo, userName, siteNumber, type)));
	}


	/**@author HCJ
	 * 发布作业到站点进行播放
	 * @param list 存储需要进行播放内容的json串
	 * @date 2019年10月24日 下午2:37:11
	 */
	@Log("发放文件，JSON信息是：{list}")
	public void dispatchFile(String list) throws Exception {
		if (StrKit.isBlank(list)) {
			throw new ParameterException("参数不能为空");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		renderJson(sopService.dispatchFile(list, userVO));
	}


	/**@author HCJ
	 * 停止站点的播放内容
	 * @param id 站点ID
	 * @param type 类型
	 * @date 2019年10月24日 下午2:37:50
	 */
	@Log("取消文件的播放，站点ID是：{id}，类型是：{type}")
	public void recycleFile(String id, Integer type) throws Exception {
		if (StrKit.isBlank(id) || type == null) {
			throw new ParameterException("参数不能为空");
		}
		String typeName = SopRecycleFileType.getNameById(type);
		if (typeName == null) {
			throw new ParameterException("当前类型不存在");
		}
		renderJson(sopService.recycleFile(id, typeName));
	}


	/**@author HCJ
	 * 预览站点正在播放的内容
	 * @param id 站点ID
	 * @date 2019年10月24日 下午2:38:13
	 */
	public void previewSite(Integer id) throws Exception {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		renderJson(ResultUtil.succeed(sopService.previewSite(id)));
	}


	/**@author HCJ
	 * 预览正在发放的内容
	 * @param list 存储需要进行预览内容的json串
	 * @date 2019年10月24日 下午2:38:42
	 */
	public void previewDispatchingFile(String list) throws Exception {
		if (StrKit.isBlank(list)) {
			throw new ParameterException("参数不能为空");
		}
		renderJson(ResultUtil.succeed(sopService.previewDispatchingFile(list)));
	}


	/**@author HCJ
	 * 查询文件历史发放记录
	 * @param pageNo 页码
	 * @param pageSize 页大小
	 * @param fileId 文件ID
	 * @param startTime 开始时间
	 * @param endTime 结束时间
	 * @param pushPerson 推送人
	 * @date 2019年10月24日 下午2:39:37
	 */
	public void selectFileHistory(Integer pageNo, Integer pageSize, Integer fileId, String startTime, String endTime, String pushPerson) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		if (fileId == null) {
			throw new ParameterException("文件ID不能为空");
		}
		renderJson(ResultUtil.succeed(sopService.selectFileHistory(pageNo, pageSize, fileId, startTime, endTime, pushPerson)));
	}


	/**@author HCJ
	 * 查询文件发放历史记录详情
	 * @param fileHistoryId 文件发放历史ID
	 * @date 2019年10月24日 下午2:40:49
	 */
	public void selectFileHistoryDetail(Integer fileHistoryId) {
		if (fileHistoryId == null) {
			throw new ParameterException("文件历史ID不能为空");
		}
		renderJson(ResultUtil.succeed(sopService.selectFileHistoryDetail(fileHistoryId)));
	}


	/**@author HCJ
	 * 查询通知发放历史记录
	 * @param pageNo 页码
	 * @param pageSize 页大小
	 * @param siteNumber 站点编号
	 * @param siteName 站点名称
	 * @param line 产线
	 * @param workshop 车间
	 * @param factory 工厂
	 * @param timeFrom 开始播放时间
	 * @param timeTo 结束播放时间
	 * @param title 标题
	 * @param content 内容
	 * @param pushPerson 推送人
	 * @date 2019年10月24日 下午2:41:22
	 */
	public void selectNoticeHistory(Integer pageNo, Integer pageSize, /*String siteNumber, String siteName,*/ String line, String workshop, String factory, String timeFrom, String timeTo, String title, String content, String pushPerson) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		renderJson(ResultUtil.succeed(sopService.selectNoticeHistory(pageNo, pageSize, /*siteNumber, siteName, */line, workshop, factory, timeFrom, timeTo, title, content, pushPerson)));
	}


	/**@author HCJ
	 * 查询确认日志
	 * @param pageNo 页码
	 * @param pageSize 页大小
	 * @param timeFrom 起始时间
	 * @param timeTo 结束时间
	 * @param userName 用户名
	 * @param siteNumber 站点编号
	 * @param lineName 产线名称
	 * @param type 类型
	 * @date 2019年11月4日 下午5:46:18
	 */
	public void selectConfirmLog(Integer pageNo, Integer pageSize, String timeFrom, String timeTo, String userName, String siteNumber, String lineName, String type) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		renderJson(ResultUtil.succeed(sopService.selectConfirmLog(pageNo, pageSize, timeFrom, timeTo, userName, siteNumber, lineName, type)));
	}


	/**@author HCJ
	 * 查询计数日志
	 * @param pageNo 页码
	 * @param pageSize 页大小
	 * @param startTimeFrom 起始的开始时间
	 * @param startTimeTo 起始的终止时间
	 * @param endTimeFrom 结束的开始时间
	 * @param endTimeTo 结束的终止时间
	 * @param userName 用户名
	 * @param siteNumber 站点编号
	 * @date 2019年11月7日 上午10:20:53
	 */
	public void selectCountLog(Integer pageNo, Integer pageSize, String startTimeFrom, String startTimeTo, String endTimeFrom, String endTimeTo, String userName, String siteNumber) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		renderJson(ResultUtil.succeed(sopService.selectCountLog(pageNo, pageSize, startTimeFrom, startTimeTo, endTimeFrom, endTimeTo, userName, siteNumber)));
	}


	/**@author HCJ
	 * 根据产线查询岗位信息
	 * @param pageNo 页码
	 * @param pageSize 页大小
	 * @param lineId 产线ID
	 * @date 2020年3月16日 上午9:27:49
	 */
	public void selectPost(Integer pageNo, Integer pageSize, Integer lineId) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		if (lineId == null) {
			throw new ParameterException("产线ID不能为空");
		}
		renderJson(ResultUtil.succeed(sopService.selectPost(pageNo, pageSize, lineId)));
	}


	/**@author HCJ
	 * 查询可分配用户
	 * @param pageNo 页码
	 * @param pageSize 页大小
	 * @param postId 岗位信息ID
	 * @param processId 工序ID
	 * @param userDes 用户描述
	 * @param userName 用户名称
	 * @date 2020年3月16日 上午9:28:18
	 */
	public void selectAssignableUser(Integer pageNo, Integer pageSize, Integer postId, Integer processId, String userDes, String userName) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		if (!(postId == null ^ processId == null)) {
			throw new ParameterException("参数不能同时为空或者同时存在");
		}
		renderJson(ResultUtil.succeed(sopService.selectAssignableUser(pageNo, pageSize, postId, processId, userDes, userName)));
	}


	/**@author HCJ
	 * 分配用户
	 * @param userId 用户ID
	 * @param lineId 产线ID
	 * @param processId 工序ID
	 * @param siteId 站点ID
	 * @param postId 岗位信息ID
	 * @date 2020年3月16日 上午9:29:13
	 */
	public void assignUser(Integer userId, Integer lineId, Integer processId, Integer siteId, Integer postId) {
		if (userId == null) {
			throw new ParameterException("用户不能为空");
		}
		if (!(postId == null ^ (lineId == null || processId == null || siteId == null))) {
			throw new ParameterException("参数不能同时为空或者同时存在");
		}
		if (sopService.assignUser(userId, lineId, processId, siteId, postId)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}
}
