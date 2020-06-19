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
import javax.servlet.ServletOutputStream;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang3.StringUtils;

import com.alibaba.fastjson.JSONObject;
import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jfinal.core.paragetter.Para;
import com.jfinal.kit.StrKit;
import com.jfinal.plugin.activerecord.Page;
import com.jfinal.plugin.activerecord.Record;
import com.jfinal.upload.UploadFile;
import com.jimi.mes_server.annotation.ProductionLog;
import com.jimi.mes_server.entity.AddPlanInfo;
import com.jimi.mes_server.entity.Constant;
import com.jimi.mes_server.entity.ModelCapacityInfo;
import com.jimi.mes_server.entity.PlanQueryCriteria;
import com.jimi.mes_server.entity.CalculatePlanResultParam;
import com.jimi.mes_server.entity.WorkTime;
import com.jimi.mes_server.entity.vo.LUserAccountVO;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.model.Orders;
import com.jimi.mes_server.service.ProductionService;
import com.jimi.mes_server.util.CommonUtil;
import com.jimi.mes_server.util.ResultUtil;
import com.jimi.mes_server.util.TokenBox;

/**排产模块控制器
 * @author   HCJ
 * @date     2019年8月16日 上午11:52:31
 */
public class ProductionController extends Controller {

	private static ProductionService productionService = Enhancer.enhance(ProductionService.class);


	/**@author HCJ
	 * 获取工序组ID和名称
	 * @date 2019年8月8日 下午3:38:32
	 */
	public void getProcessGroup() {
		renderJson(ResultUtil.succeed(productionService.getProcessGroup()));
	}


	/**@author HCJ
	 * 获取工序ID和名称
	 * @date 2019年8月8日 下午3:39:02
	 */
	public void getProcess() {
		renderJson(ResultUtil.succeed(productionService.getProcess()));
	}


	/**@author HCJ
	 * 获取产线ID和名称
	 * @date 2019年8月8日 下午3:39:25
	 */
	public void getLine() {
		renderJson(ResultUtil.succeed(productionService.getLine()));
	}


	/**@author HCJ
	 * 获取订单状态ID和名称
	 * @date 2019年8月8日 下午3:39:41
	 */
	public void getOrderStatus() {
		renderJson(ResultUtil.succeed(productionService.getOrderStatus()));
	}


	/**@author HCJ
	 * 获取排产计划状态的ID和名称
	 * @date 2019年8月8日 下午3:40:29
	 */
	public void getSchedulingPlanStatus() {
		renderJson(ResultUtil.succeed(productionService.getSchedulingPlanStatus()));
	}


	/**@author HCJ
	 * 获取文件类型的ID和名称
	 * @date 2020年5月15日 下午2:46:59
	 */
	public void getFileType() {
		renderJson(ResultUtil.succeed(productionService.getFileType()));
	}


	/**@author HCJ
	 * 添加工序组
	 * @param groupNo 工序组编号
	 * @param groupName 工序组名称
	 * @param groupRemark 备注
	 * @param factory 工厂ID
	 * @date 2020年5月15日 下午2:47:31
	 */
	@ProductionLog("添加工序组")
	public void addProcessGroup(String groupNo, String groupName, String groupRemark, Integer factory, Integer parentGroup) {
		if (StringUtils.isAnyBlank(groupNo, groupName)) {
			throw new ParameterException("参数不能为空");
		}
		if (factory == null) {
			throw new ParameterException("所属工厂不能为空");
		}
		if (productionService.addProcessGroup(groupNo, groupName, groupRemark, factory, parentGroup)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 删除工序组
	 * @param id 工序组ID
	 * @date 2019年8月8日 下午3:41:54
	 */
	@ProductionLog("删除工序组")
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


	/**@author HCJ
	 * 查询工序组
	 * @param groupNo 工序组编号
	 * @param groupName 工序组名称
	 * @param factory 工厂ID
	 * @date 2020年5月15日 下午2:48:04
	 */
	public void selectProcessGroup(String groupNo, String groupName, Integer factory, Integer parentGroup) {
		ResultUtil result = ResultUtil.succeed(productionService.selectProcessGroup(groupNo, groupName, factory, parentGroup));
		renderJson(result);
	}


	/**@author HCJ
	 * 编辑工序组信息
	 * @param id 工序组ID
	 * @param groupNo 工序组编号
	 * @param groupName 工序组名称
	 * @param groupRemark 备注
	 * @param factory 工厂ID
	 * @date 2019年8月8日 下午3:43:47
	 */
	@ProductionLog("修改工序组")
	public void editProcessGroup(Integer id, String groupNo, String groupName, String groupRemark, Integer factory, Integer parentGroup) {
		if (id == null || factory == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.editProcessGroup(id, groupNo, groupName, groupRemark, factory, parentGroup)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 增加产线
	 * @param lineNo 产线编号
	 * @param lineName 产线名称
	 * @param lineRemark 备注
	 * @param lineDirector 产线负责人
	 * @param lineEngineer 产线工程
	 * @param lineQc 产线品质
	 * @param processGroup 工序组ID
	 * @param workshopId 车间ID
	 * @param timeLength 待确认超时时长
	 * @date 2020年5月15日 下午2:48:57
	 */
	@ProductionLog("添加产线")
	public void addLine(String lineNo, String lineName, String lineRemark, Integer lineDirector, Integer lineEngineer, Integer lineQc, Integer processGroup, Integer workshopId, Integer timeLength) {
		if (StringUtils.isAnyBlank(lineNo, lineName) || timeLength == null) {
			throw new ParameterException("参数不能为空");
		}
		if (timeLength > Constant.MINUTE_OF_TWENTY_ONE_DAYS) {
			throw new ParameterException("确认时间的长度超过限制");
		}
		if (productionService.addLine(lineNo, lineName, lineRemark, lineDirector, lineEngineer, lineQc, processGroup, workshopId, timeLength)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 删除产线
	 * @param id 产线ID
	 * @date 2019年8月8日 下午3:48:38
	 */
	@ProductionLog("删除产线")
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


	/**@author HCJ
	 * 查询产线
	 * @param lineNo 产线编号
	 * @param lineName 产线名称
	 * @param processGroup 工序组ID
	 * @param workshopId 车间ID
	 * @param factoryId 工厂ID
	 * @param lineDirector 产线负责人
	 * @param lineEngineer 产线工程
	 * @param lineQc 产线品质
	 * @date 2020年5月15日 下午2:50:08
	 */
	public void selectLine(String lineNo, String lineName, Integer processGroup, Integer workshopId, Integer factoryId, String lineDirector, String lineEngineer, String lineQc) {
		renderJson(ResultUtil.succeed(productionService.selectLine(lineNo, lineName, processGroup, workshopId, factoryId, lineDirector, lineEngineer, lineQc)));
	}


	/**@author HCJ
	 * 修改产线信息
	 * @param id 产线ID
	 * @param lineNo 产线编号
	 * @param lineName 产线名称
	 * @param lineRemark 备注
	 * @param lineDirector 产线负责人
	 * @param lineEngineer 产线工程
	 * @param lineQc 产线品质
	 * @param processGroup 工序组ID
	 * @param workshopId 车间ID
	 * @param factoryId 工厂ID
	 * @param timeLength 待确认超时时长
	 * @date 2020年5月15日 下午2:51:16
	 */
	@ProductionLog("修改产线")
	public void editLine(Integer id, String lineNo, String lineName, String lineRemark, Integer lineDirector, Integer lineEngineer, Integer lineQc, Integer processGroup, Integer workshopId, Integer factoryId, Integer timeLength) {
		if (id == null || timeLength == null) {
			throw new ParameterException("参数不能为空");
		}
		if (timeLength > Constant.MINUTE_OF_TWENTY_ONE_DAYS) {
			throw new ParameterException("确认时间的长度超过限制");
		}
		if (productionService.editLine(id, lineNo, lineName, lineRemark, lineDirector, lineEngineer, lineQc, processGroup, workshopId, factoryId, timeLength)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 添加工序
	 * @param processNo 工序编号
	 * @param processName 工序名称
	 * @param processRemark 备注
	 * @param processGroup 工序组ID
	 * @date 2019年8月8日 下午3:50:40
	 */
	@ProductionLog("添加工序")
	public void addProcess(Integer processNo, String processName, String processRemark, Integer processGroup) {
		if (StrKit.isBlank(processName) || processNo == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.addProcess(processNo, processName, processRemark, processGroup)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 删除工序
	 * @param id 工序ID
	 * @date 2019年8月8日 下午3:51:08
	 */
	@ProductionLog("删除工序")
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


	/**@author HCJ
	 * 查询工序
	 * @param processNo 工序编号
	 * @param processName 工序名称
	 * @param processGroup 工序组ID
	 * @date 2019年8月8日 下午3:51:20
	 */
	public void selectProcess(Integer processNo, String processName, Integer processGroup) {
		renderJson(ResultUtil.succeed(productionService.selectProcess(processNo, processName, processGroup)));
	}


	/**@author HCJ
	 * 修改工序信息
	 * @param id 工序ID
	 * @param processNo 工序编号
	 * @param processName 工序名称
	 * @param processRemark 备注
	 * @param processGroup 工序组ID
	 * @date 2019年8月8日 下午3:52:31
	 */
	@ProductionLog("修改工序")
	public void editProcess(Integer id, Integer processNo, String processName, String processRemark, Integer processGroup, Integer position) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.editProcess(id, processNo, processName, processRemark, processGroup, position)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 添加产线电脑
	 * @param ip 电脑IP
	 * @param computerName 电脑名称
	 * @param remark 备注
	 * @param line 产线ID
	 * @date 2019年8月8日 下午3:53:07
	 */
	public void addComputer(String ip, String computerName, String remark, Integer line) {
		if (line == null || StrKit.isBlank(ip)) {
			throw new ParameterException("参数不能为空");
		}
		if (!CommonUtil.isIPAddress(ip)) {
			throw new ParameterException("无效的IP地址");
		}
		if (productionService.addComputer(ip, computerName, remark, line)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 删除产线电脑
	 * @param id 产线电脑ID
	 * @date 2019年8月8日 下午3:53:37
	 */
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


	/**@author HCJ
	 * 查询产线电脑信息
	 * @param lineId 产线电脑ID
	 * @date 2019年8月8日 下午3:53:53
	 */
	public void selectComputer(Integer lineId) {
		if (lineId == null) {
			throw new ParameterException("参数不能为空");
		}
		renderJson(ResultUtil.succeed(productionService.selectComputer(lineId)));
	}


	/**@author HCJ
	 * 编辑产线电脑信息
	 * @param id 产线电脑ID
	 * @param ip 产线电脑IP
	 * @param computerName 产线电脑名称
	 * @param remark 备注
	 * @date 2019年8月8日 下午3:54:13
	 */
	public void editComputer(Integer id, String ip, String computerName, String remark) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (!StrKit.isBlank(ip) && !CommonUtil.isIPAddress(ip)) {
			throw new ParameterException("无效的IP地址");
		}
		if (productionService.editComputer(id, ip, computerName, remark)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 添加机型产能
	 * @param modelCapacityString 机型产能集合的json串
	 * @date 2020年5月15日 下午2:52:28
	 */
	@ProductionLog("添加机型产能")
	public void addCapacity(String modelCapacityString) {
		if (StringUtils.isBlank(modelCapacityString)) {
			throw new ParameterException("参数不能为空");
		}
		List<ModelCapacityInfo> modelCapacityInfos;
		try {
			modelCapacityInfos = JSONObject.parseArray(modelCapacityString, ModelCapacityInfo.class);
		} catch (Exception e) {
			throw new ParameterException("参数格式出错");
		}
		if (productionService.addCapacity(modelCapacityInfos)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}

	}


	/**@author HCJ
	 * 删除机型产能
	 * @param id 机型产能ID
	 * @date 2019年8月8日 下午3:55:32
	 */
	@ProductionLog("删除机型产能")
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


	/**@author HCJ
	 * 查询机型产能
	 * @param pageNo 页码
	 * @param pageSize 页大小
	 * @param softModel 机型
	 * @param customerModel 客户型号
	 * @param processGroup 工序组ID
	 * @date 2020年5月15日 下午2:52:58
	 */
	public void selectCapacity(Integer pageNo, Integer pageSize, String softModel, String customerModel, Integer processGroup, Integer factory, Integer statusId) {
		renderJson(ResultUtil.succeed(productionService.selectCapacity(pageNo, pageSize, softModel, customerModel, processGroup, factory, statusId)));
	}


	/**@author HCJ
	 * 编辑机型产能
	 * @param modelCapacityString 机型产能集合的json串
	 * @date 2020年5月15日 下午2:53:24
	 */
	@ProductionLog("修改机型产能")
	public void editCapacity(String modelCapacityString) {
		if (StringUtils.isBlank(modelCapacityString)) {
			throw new ParameterException("参数不能为空");
		}
		List<ModelCapacityInfo> modelCapacityInfos;
		try {
			modelCapacityInfos = JSONObject.parseArray(modelCapacityString, ModelCapacityInfo.class);
		} catch (Exception e) {
			throw new ParameterException("参数格式出错");
		}
		if (productionService.editCapacity(modelCapacityInfos)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 审核机型产能
	 * @param softModel 机型
	 * @param customerNumber 客户编号
	 * @param customerModel 客户型号
	 * @param reviewRemark 审核备注
	 * @param statusId 状态ID
	 * @param factoryId 工厂ID
	 * @date 2020年5月21日 上午11:29:18
	 */
	@ProductionLog("审核机型产能")
	public void reviewCapacity(String softModel, String customerNumber, String customerModel, String reviewRemark, Integer statusId, Integer factoryId) {
		if (StringUtils.isAnyBlank(softModel) || statusId == null || factoryId == null) {
			throw new ParameterException("参数不能为空");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		if (productionService.reviewCapacity(softModel, customerNumber, customerModel, reviewRemark, statusId, factoryId, userVO)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 添加文件类型
	 * @param typeName 名称
	 * @param typeRemarks 备注
	 * @date 2020年5月15日 下午2:53:54
	 */
	@ProductionLog("添加文件类型")
	public void addFileType(String typeName, String typeRemarks) {
		if (StrKit.isBlank(typeName)) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.addFileType(typeName, typeRemarks)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 删除文件类型
	 * @param id 文件类型ID
	 * @date 2020年5月15日 下午2:53:54
	 */
	@ProductionLog("删除文件类型")
	public void deleteFileType(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.deleteFileType(id)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 查询文件类型
	 * @param filter 查询条件
	 * @date 2020年5月15日 下午2:53:54
	 */
	public void selectFileType(String filter) {
		ResultUtil result = ResultUtil.succeed(productionService.selectFileType(filter));
		renderJson(result);
	}


	/**@author HCJ
	 * 编辑文件类型
	 * @param id 文件类型ID
	 * @param typeName 名称
	 * @param typeRemarks 备注
	 * @date 2020年5月15日 下午2:54:50
	 */
	@ProductionLog("修改文件类型")
	public void editFileType(Integer id, String typeName, String typeRemarks) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (productionService.editFileType(id, typeName, typeRemarks)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 添加订单
	 * @param order 订单信息
	 * @date 2019年8月8日 下午3:57:34
	 */
	@ProductionLog("添加订单")
	public void addOrder(@Para("") Orders order) {
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		if (order == null) {
			throw new ParameterException("参数不能为空");
		}
		boolean isOrderInformationExist = StringUtils.isAnyBlank(order.getZhidan(), order.getSoftModel());
		if (isOrderInformationExist || order.getQuantity() == null || order.getIsRework() == null) {
			throw new ParameterException("订单号、机型、数量和是否为返工订单不能为空");
		}
		if (order.getQuantity() < Constant.INTEGER_ZERO) {
			throw new ParameterException("订单数量格式错误");
		}
		if (order.getOrderDate() != null && order.getDeliveryDate() != null) {
			if (order.getOrderDate().after(order.getDeliveryDate())) {
				throw new ParameterException("订单日期与交货日期冲突");
			}
		}
		if (productionService.addOrder(order, userVO)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 订单异常结单
	 * @param id 订单ID
	 * @param abnormalEndReason 原因
	 * @date 2019年8月8日 下午3:57:52
	 */
	@ProductionLog("订单异常结单")
	public void abnormalEndOrder(Integer id, String abnormalEndReason) {
		if (id == null || StrKit.isBlank(abnormalEndReason)) {
			throw new ParameterException("参数不能为空");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		if (productionService.abnormalEndOrder(id, abnormalEndReason, userVO)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 查询订单
	 * @param pageNo 页码
	 * @param pageSize 页大小
	 * @param factoryId 工厂ID
	 * @param status 状态ID
	 * @param zhidan 订单号
	 * @param alias 内部替换号
	 * @param softModel 机型
	 * @param productNo 产品编码
	 * @param customerNumber 客户编号
	 * @param customerName 客户名称
	 * @param orderDateFrom 起始订单日期
	 * @param orderDateTo 结束订单日期
	 * @param deliveryDateFrom 起始交货日期
	 * @param deliveryDateTo 结束交货日期
	 * @param isRework 是否是返工订单
	 * @param customerMaterialNo 客户料号
	 * @param reworkZhidan 返工订单号
	 * @date 2020年5月15日 下午2:55:34
	 */
	public void selectOrder(Integer pageNo, Integer pageSize, Integer factoryId, Integer status, String zhidan, String alias, String softModel, String productNo, String customerNumber, String customerName, String orderDateFrom, String orderDateTo, String deliveryDateFrom, String deliveryDateTo, Boolean isRework, String customerMaterialNo, String reworkZhidan) {
		if (isRework == null) {
			throw new ParameterException("是否为返工订单的参数不能为空");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		renderJson(ResultUtil.succeed(productionService.selectOrder(pageNo, pageSize, factoryId, status, zhidan, alias, softModel, productNo, customerNumber, customerName, orderDateFrom, orderDateTo, deliveryDateFrom, deliveryDateTo, isRework, customerMaterialNo, reworkZhidan, userVO)));
	}


	/**@author HCJ
	 * 查询订单详情
	 * @param id 订单ID
	 * @date 2019年8月8日 下午3:58:57
	 */
	public void selectOrderDetail(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		renderJson(ResultUtil.succeed(productionService.selectOrderDetail(id)));
	}


	/**@author HCJ
	 * 编辑订单信息
	 * @param order 订单信息
	 * @date 2019年8月8日 下午3:59:13
	 */
	@ProductionLog("修改订单")
	public void editOrder(@Para("") Orders order) {
		if (order == null) {
			throw new ParameterException("参数不能为空");
		}
		boolean isOrderInformationExist = StringUtils.isAnyBlank(order.getZhidan(), order.getSoftModel());
		if (isOrderInformationExist || order.getQuantity() == null) {
			throw new ParameterException("订单号、机型和数量不能为空");
		}
		if (order.getQuantity() < Constant.INTEGER_ZERO) {
			throw new ParameterException("订单数量格式错误");
		} /*
			if (order.getOrderDate().compareTo(order.getDeliveryDate()) >= Constant.INTEGER_ZERO) {
			throw new ParameterException("订单日期不能晚于或者等于交货日期");
			}*/
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		if (productionService.editOrder(order, userVO)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 导入订单文件
	 * @param uploadFile 存储订单信息的Excel文件
	 * @param isRework 是否为返工订单
	 * @param factory 工厂ID
	 * @date 2020年5月15日 下午2:57:48
	 */
	@ProductionLog("导入订单")
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


	/**@author HCJ
	 * 上传订单的相关文件
	 * @date 2019年8月8日 下午4:00:12
	 */
	@ProductionLog("导入订单相关表格")
	public void importOrderTable() {
		List<UploadFile> uploadFiles = getFiles();
		if (uploadFiles == null || uploadFiles.isEmpty()) {
			throw new ParameterException("请添加文件");
		}
		/*for (UploadFile uploadFile : uploadFiles) {
			if (!uploadFile.getOriginalFileName().endsWith(".xls") && !uploadFile.getOriginalFileName().endsWith(".xlsx")) {
				throw new ParameterException("只能上传Excel文件");
			}
		}*/
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
		/*if (Constant.ENGINEER_USERTYPE.equals(userVO.getWebUserType()) && !type.equals(Constant.SOP_FILETYPE - 1)) {
			throw new OperationException("工程及生产只能上传SOP表");
		}*/
		if (productionService.importOrderTable(uploadFiles, type, id, userVO)) {
			renderJson(ResultUtil.succeed());
		}
	}


	/**@author HCJ
	 * 下载订单的相关文件
	 * @param id 订单ID
	 * @date 2019年8月8日 下午4:00:40
	 */
	@ProductionLog("下载订单相关文件")
	public void downloadOrderTable(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		File file = productionService.downloadOrderTable(id);
		HttpServletResponse response = getResponse();
		try {
			response.setHeader("Content-Disposition", "attachment; filename=" + URLEncoder.encode(file.getName(), "utf-8"));
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
			renderJson(ResultUtil.failed("文件下载出错"));
		}
		response.addHeader("Access-Control-Expose-Headers", "Content-Disposition");
		if (file.getName().contains(".xlsx")) {
			response.setContentType("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
		} else if (file.getName().contains(".xls")) {
			response.setContentType("application/vnd.ms-excel");
		} else {
			response.setContentType("application/octet-stream");
		}
		try (ServletOutputStream os = response.getOutputStream(); FileInputStream input = new FileInputStream(file);) {
			byte[] buffer = new byte[1024];
			int i = 0;
			while ((i = input.read(buffer, 0, 1024)) != -1) {
				os.write(buffer, 0, i);
			}
			os.flush();
		} catch (IOException e) {
			e.printStackTrace();
			renderJson(ResultUtil.failed("文件下载出错"));
		}
		renderNull();
	}


	/**@author HCJ
	 * 更新订单相关文件的状态
	 * @param id 文件ID
	 * @param isNormal 是否正确
	 * @date 2020年5月15日 下午2:58:37
	 */
	@ProductionLog("修改订单文件的相关信息")
	public void editOrderTable(Integer id, Boolean isNormal) {
		if (id == null || isNormal == null) {
			throw new ParameterException("参数不能为空");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		if (productionService.editOrderTable(id, userVO, isNormal)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 删除订单相关文件
	 * @param id 文件ID
	 * @param deleteReason 原因
	 * @date 2020年5月15日 下午2:59:00
	 */
	@ProductionLog("删除订单相关文件")
	public void deleteOrderTable(Integer id, String deleteReason) {
		if (id == null || StrKit.isBlank(deleteReason)) {
			throw new ParameterException("参数不能为空");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		if (productionService.deleteOrderTable(id, deleteReason, userVO)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 查询未排产普通订单
	 * @param processGroupId 工序组ID
	 * @param factory 工厂ID
	 * @date 2020年6月9日 下午2:19:14
	 */
	public void selectUnscheduledPlan(Integer processGroupId, Integer factory) {
		if (processGroupId == null || factory == null) {
			throw new ParameterException("参数不能为空");
		}
		renderJson(ResultUtil.succeed(productionService.selectUnscheduledPlan(processGroupId, factory)));
	}


	/**@author HCJ
	 * 查询未排产返工订单
	 * @param processGroupId 工序组ID
	 * @param factory 工厂ID
	 * @date 2020年6月9日 下午2:19:39
	 */
	public void selectReworkPlan(Integer processGroupId, Integer factory) {
		if (processGroupId == null || factory == null) {
			throw new ParameterException("参数不能为空");
		}
		renderJson(ResultUtil.succeed(productionService.selectReworkPlan(processGroupId, factory)));
	}


	/**@author HCJ
	 * 添加排产计划
	 * @param settings 存储排产计划信息的json串
	 * @date 2020年5月15日 下午2:59:39
	 */
	@ProductionLog("添加排产计划")
	public void addPlan(String settings) {
		List<AddPlanInfo> addPlanInfos = JSONObject.parseArray(settings, AddPlanInfo.class);
		if (addPlanInfos == null || addPlanInfos.isEmpty()) {
			throw new ParameterException("参数不能为空");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		if (productionService.addPlan(addPlanInfos, userVO)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 删除排产计划
	 * @param id 计划ID
	 * @date 2019年8月16日 上午11:47:46
	 */
	@ProductionLog("删除排产计划")
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


	/**@author HCJ
	 * 查询排产计划
	 * @param planQueryCriteria 查询条件类
	 * @date 2019年8月16日 上午11:47:58
	 */
	public void selectPlan(@Para("") PlanQueryCriteria planQueryCriteria) {
		renderJson(ResultUtil.succeed(productionService.selectPlan(planQueryCriteria)));
	}


	/**@author HCJ
	 * 查询排产计划详情
	 * @param id 计划ID
	 * @date 2019年8月16日 上午11:48:07
	 */
	public void selectPlanDetail(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		ResultUtil result = ResultUtil.succeed(productionService.selectPlanDetail(id));
		renderJson(result);
	}


	/**@author HCJ
	 * 编辑排产计划
	 * @param id 计划ID
	 * @param isUrgent 是否紧急
	 * @param remark 备注
	 * @param schedulingQuantity 排产数量
	 * @param line 产线ID
	 * @param planStartTime 计划开始时间
	 * @param planCompleteTime 计划完成时间
	 * @param lineChangeTime 换线时间
	 * @param capacity 产能
	 * @param isCompleted 是否完成
	 * @param producedQuantity 已生产数量
	 * @param remainingReason 未完成原因
	 * @param productionPlanningNumber 生产计划编号
	 * @param personNumber 人数
	 * @param rhythm 节拍
	 * @date 2020年5月15日 下午3:00:19
	 */
	@ProductionLog("修改排产计划")
	public void editPlan(Integer id, Boolean isUrgent, String remark, Integer producedQuantity,Integer schedulingQuantity, Integer line, String planStartTime, String planCompleteTime, String lineChangeTime, Integer capacity, Boolean isCompleted, String remainingReason, String productionPlanningNumber, Integer personNumber, Double rhythm) {
		if (lineChangeTime != null) {
			if (StrKit.isBlank(lineChangeTime) || lineChangeTime.length() > Constant.MAX_LINECHANGETIME_LENGTH) {
				throw new ParameterException("转线时间内容为空或长度过长");
			}
		}
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		Date start = null;
		Date end = null;
		if (planStartTime != null && planCompleteTime != null) {
			try {
				start = dateFormat.parse(planStartTime);
				end = dateFormat.parse(planCompleteTime);
			} catch (ParseException e) {
				throw new ParameterException("时间格式出错");
			}
			if (start.compareTo(end) >= Constant.INTEGER_ZERO) {
				throw new ParameterException("开始时间不能晚于或者等于结束时间");
			}
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		if (productionService.editPlan(id, isUrgent, remark, producedQuantity, schedulingQuantity, line, start, end, lineChangeTime, capacity, isCompleted, remainingReason, productionPlanningNumber, userVO, personNumber, rhythm)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 重新排产计划
	 * @param settings 存储排产计划信息的json串
	 * @date 2020年5月15日 下午3:00:52
	 */
	@ProductionLog("重新排产计划")
	public void reSchedulingPlan(String settings) {
		List<AddPlanInfo> addPlanInfos = JSONObject.parseArray(settings, AddPlanInfo.class);
		if (addPlanInfos == null || addPlanInfos.isEmpty()) {
			throw new ParameterException("参数不能为空");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		if (productionService.reSchedulingPlan(addPlanInfos, userVO)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 修改计划状态
	 * @param id 计划ID
	 * @param type 类型
	 * @date 2019年8月16日 上午11:50:05
	 */
	@ProductionLog("修改计划状态")
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
		/*String typeName = userVO.getTypeName();
		if ("engineer".equals(typeName) && type.equals(Constant.WAIT_NOTIFICATION_PLANSTATUS - 1)) {
			throw new ParameterException("工程只能修改计划状态是否为开始或完成");
		}
		if ("schedulingJMPMC".equals(typeName) && !type.equals(Constant.WAIT_NOTIFICATION_PLANSTATUS - 1)) {
			throw new ParameterException("PMC只能修改计划状态是否为待通知");
		}*/
		if (productionService.editPlanStatus(id, type, userVO)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 查询计划已生产数量
	 * @param id 计划ID
	 * @date 2019年8月16日 上午11:50:51
	 */
	public void selectPlanProducedQuantity(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		ResultUtil result = ResultUtil.succeed(productionService.selectPlanProducedQuantityAndUpdate(id, null, dateFormat));
		renderJson(result);
	}


	/**@author HCJ
	 * 获取计划甘特图
	 * @param id 计划ID
	 * @date 2019年8月16日 上午11:51:11
	 */
	public void getPlanGantt(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		renderJson(ResultUtil.succeed(productionService.getPlanGantt(id)));
	}


	/**@author HCJ
	 * 导出排产计划单
	 * @param planQueryCriteria 查询条件类
	 * @date 2019年8月16日 上午11:51:25
	 */
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


	/**@author HCJ
	 * 校验完成时间
	 * @param schedulingQuantity 排产数量
	 * @param planStartTime 计划开始时间
	 * @param planCompleteTime 计划完成时间
	 * @param lineChangeTime 换线时间
	 * @param capacity 产能
	 * @date 2019年8月16日 上午11:51:43
	 */
	public void checkCompleteTime(Integer schedulingQuantity, String planStartTime, String planCompleteTime, String lineChangeTime, Integer capacity) {
		if (schedulingQuantity == null || planStartTime == null || planCompleteTime == null || capacity == null || lineChangeTime == null) {
			throw new ParameterException("参数不能为空");
		}
		if (schedulingQuantity <= Constant.INTEGER_ZERO || capacity <= Constant.INTEGER_ZERO) {
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
		renderJson(ResultUtil.succeed(productionService.checkCompleteTime(schedulingQuantity, start, end, lineChangeTime, capacity)));
	}


	/**@author HCJ
	 * 查询日志
	 * @param pageNo 页码
	 * @param pageSize 页大小
	 * @param startTime 开始时间
	 * @param endTime 结束时间
	 * @param userName 用户名称
	 * @param address 访问者IP
	 * @param zhidan 订单号
	 * @param alias 内部替换号
	 * @date 2020年5月15日 下午3:02:21
	 */
	public void selectProductionLog(Integer pageNo, Integer pageSize, String startTime, String endTime, String userName, String address, String zhidan, String alias) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		renderJson(ResultUtil.succeed(productionService.selectProductionLog(pageNo, pageSize, startTime, endTime, userName, address, zhidan, alias)));
	}


	/**@author HCJ
	 * 设置产线工作时间表
	 * @param executorId 产线ID
	 * @param workTimes 工作时间集合的字符串
	 * @param isDefault 是否为设置默认工作时间表
	 * @date 2020年6月1日 上午10:54:42
	 */
	public void setWorkTimeByExecutorId(Integer executorId, String workTimes, Boolean isDefault) {
		if (StrKit.isBlank(workTimes) || executorId == null || isDefault == null) {
			throw new ParameterException("参数不能为空");
		}
		List<WorkTime> times;
		try {
			times = JSONObject.parseArray(workTimes, WorkTime.class);
		} catch (Exception e) {
			throw new ParameterException("参数格式出错");
		}
		try {
			if (productionService.setWorkTimeByExecutorId(executorId, times, isDefault)) {
				renderJson(ResultUtil.succeed());
			} else {
				renderJson(ResultUtil.failed());
			}
		} catch (Exception e) {
			throw new ParameterException("时间格式错误，请重新输入");
		}
	}


	/**@author HCJ
	 * 获取产线工作时间表
	 * @param executorId 产线ID
	 * @param isDefault 是否获取默认工作时间表
	 * @date 2020年6月1日 上午10:56:14
	 */
	public void getWorkTimeByExecutorId(Integer executorId, Boolean isDefault) {
		if (executorId == null || isDefault == null) {
			throw new ParameterException("参数不能为空");
		}
		renderJson(ResultUtil.succeed(productionService.getWorkTimeByExecutorId(executorId, isDefault)));
	}


	/**@author HCJ
	 * 计算排产计划的预计时间
	 * @param details json串信息
	 * @date 2020年5月28日 下午6:17:55
	 */
	public void calculatePlanResult(String details) {
		if (StrKit.isBlank(details)) {
			throw new ParameterException("参数不能为空");
		}
		CalculatePlanResultParam calculatePlanResultParam;
		try {
			calculatePlanResultParam = JSONObject.parseObject(details, CalculatePlanResultParam.class);
		} catch (Exception e) {
			throw new ParameterException("参数格式出错");
		}
		try {
			renderJson(ResultUtil.succeed(productionService.calculatePlanResult(calculatePlanResultParam)));
		} catch (Exception e) {
			e.printStackTrace();
			renderJson(ResultUtil.failed("计算出错"));
		}
	}


	/**@author HCJ
	 * 根据订单ID获取排产计划信息
	 * @param orderId 订单ID
	 * @date 2020年6月1日 上午10:56:37
	 */
	public void getPlanByOrder(Integer orderId) {
		if (orderId == null) {
			throw new ParameterException("参数不能为空");
		}
		renderJson(ResultUtil.succeed(productionService.getPlanByOrder(orderId)));
	}

}
