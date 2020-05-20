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
import com.jimi.mes_server.entity.AddPlanInfo;
import com.jimi.mes_server.entity.Constant;
import com.jimi.mes_server.entity.PlanQueryCriteria;
import com.jimi.mes_server.entity.vo.AuthorityVO;
import com.jimi.mes_server.entity.vo.LUserAccountVO;
import com.jimi.mes_server.exception.OperationException;
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
	 * 添加工序组
	 * @param groupNo 工序组编号
	 * @param groupName 工序组名称
	 * @param groupRemark 备注
	 * @date 2019年8月8日 下午3:41:13
	 */
	
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


	/**@author HCJ
	 * 删除工序组
	 * @param id 工序组ID
	 * @date 2019年8月8日 下午3:41:54
	 */
	
	public void deleteProcessGroup(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (id <= Constant.DEFAULT_MAX_PROCESSGROUP_ID) {
			throw new OperationException("无法操作");
		}

		if (productionService.deleteProcessGroup(id)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 查询工序组
	 * @param filter 查询条件
	 * @date 2019年8月8日 下午3:43:20
	 */
	
	public void selectProcessGroup(String filter) {
		ResultUtil result = ResultUtil.succeed(productionService.selectProcessGroup(filter));
		renderJson(result);
	}


	/**@author HCJ
	 * 编辑工序组信息
	 * @param id 工序组ID
	 * @param groupNo 工序组编号
	 * @param groupName 工序组名称
	 * @param groupRemark 备注
	 * @date 2019年8月8日 下午3:43:47
	 */
	
	public void editProcessGroup(Integer id, String groupNo, String groupName, String groupRemark, Integer position) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (id <= Constant.DEFAULT_MAX_PROCESSGROUP_ID || position <= Constant.DEFAULT_MAX_PROCESSGROUP_ID) {
			throw new OperationException("无法操作");
		}

		if (productionService.editProcessGroup(id, groupNo, groupName, groupRemark, position)) {
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
	 * @date 2019年8月8日 下午3:47:17
	 */
	
	public void addLine(String lineNo, String lineName, String lineRemark, Integer lineDirector, Integer lineEngineer, Integer lineQc, Integer processGroup, Integer workshopId, Integer factoryId, Integer timeLength) {
		if (StringUtils.isAnyBlank(lineNo, lineName) || timeLength == null) {
			throw new ParameterException("参数不能为空");
		}
		if (timeLength > Constant.MINUTE_OF_TWENTY_ONE_DAYS) {
			throw new ParameterException("确认时间的长度超过限制");
		}
		if (productionService.addLine(lineNo, lineName, lineRemark, lineDirector, lineEngineer, lineQc, processGroup, workshopId, factoryId, timeLength)) {
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
	 * @date 2019年8月8日 下午3:49:17
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
	 * @date 2019年8月8日 下午3:49:46
	 */
	
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
	 * @param processGroup 工序组
	 * @date 2019年8月8日 下午3:50:40
	 */
	
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
	 * @param softModel 机型
	 * @param customerModel 客户型号
	 * @param process 工序ID
	 * @param processGroup 工序组ID
	 * @param processPeopleQuantity 工序人数
	 * @param capacity 产能
	 * @param remark 备注
	 * @date 2019年8月8日 下午3:54:49
	 */
	
	public void addCapacity(String softModel, String customerModel, Integer process, Integer processGroup, Integer processPeopleQuantity, Integer capacity, String remark, Double rhythm) {
		if (StrKit.isBlank(softModel) || process == null || processGroup == null || processPeopleQuantity == null || capacity == null || rhythm == null) {
			throw new ParameterException("参数不能为空");
		}
		if (processPeopleQuantity < Constant.INTEGER_ZERO || capacity < Constant.INTEGER_ZERO) {
			throw new ParameterException("产能或人数不合理");
		}
		if (rhythm < Constant.INTEGER_ZERO || rhythm > Constant.HOUR_TO_SECOND || rhythm.toString().length() > 7) {
			throw new ParameterException("节拍不合理");
		}
		if (productionService.addCapacity(softModel, customerModel, process, processGroup, processPeopleQuantity, capacity, remark, rhythm)) {
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
	 * @param process 工序ID
	 * @date 2019年8月8日 下午3:55:50
	 */
	
	public void selectCapacity(Integer pageNo, Integer pageSize, String softModel, String customerModel, Integer process) {
		renderJson(ResultUtil.succeed(productionService.selectCapacity(pageNo, pageSize, softModel, customerModel, process)));
	}


	/**@author HCJ
	 * 编辑机型产能
	 * @param id 机型产能ID
	 * @param softModel 机型
	 * @param customerModel 客户型号
	 * @param process 工序ID
	 * @param processGroup 工序组ID
	 * @param processPeopleQuantity 工序人数
	 * @param capacity 产能
	 * @param remark 备注
	 * @param position 需要与其交换位置的机型产能ID
	 * @date 2019年8月8日 下午3:56:19
	 */
	
	public void editCapacity(Integer id, String softModel, String customerModel, Integer process, Integer processGroup, Integer processPeopleQuantity, Integer capacity, String remark, Integer position, Double rhythm) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (rhythm != null) {
			if (rhythm < Constant.INTEGER_ZERO || rhythm > Constant.HOUR_TO_SECOND || rhythm.toString().length() > 7) {
				throw new ParameterException("节拍不合理");
			}
		}
		if (productionService.editCapacity(id, softModel, customerModel, process, processGroup, processPeopleQuantity, capacity, remark, position, rhythm)) {
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
		if (order.getOrderDate().after(order.getDeliveryDate())) {
			throw new ParameterException("订单日期与交货日期冲突");
		}
		if (productionService.addOrder(order, userVO)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 删除订单
	 * @param id 订单ID
	 * @param deleteReason 删除原因
	 * @date 2019年8月8日 下午3:57:52
	 */
	
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


	/**@author HCJ
	 * 查询订单
	 * @param pageNo 页码
	 * @param pageSize 页大小
	 * @param ascBy 以哪个字段进行增序
	 * @param descBy 以哪个字段进行降序
	 * @param filter 查询条件
	 * @param isRework 是否为返工订单
	 * @date 2019年8月8日 下午3:58:08
	 */
	
	public void selectOrder(Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter, Boolean isRework) {
		if (isRework == null) {
			throw new ParameterException("是否为返工订单的参数不能为空");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		renderJson(ResultUtil.succeed(productionService.selectOrder(pageNo, pageSize, ascBy, descBy, filter, isRework, userVO)));
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
		}
		if (order.getOrderDate().compareTo(order.getDeliveryDate()) >= Constant.INTEGER_ZERO) {
			throw new ParameterException("订单日期不能晚于或者等于交货日期");
		}
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
	 * @param uploadFile Excel文件
	 * @param isRework 是否为返工订单
	 * @date 2019年8月8日 下午3:59:27
	 */
	
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
		for (AuthorityVO authorityVO : userVO.getAuthorities()) {
			if (authorityVO.getUrls().contains("/production/importOrderTable:uploadAll")) {
				productionService.importOrderTable(uploadFiles, type, id, userVO);
				renderJson(ResultUtil.succeed());
				return;
			}
		}
		if (!type.equals(Constant.SOP_FILETYPE - 1)) {
			throw new OperationException("用户仅有权限上传SOP表");
		}
		productionService.importOrderTable(uploadFiles, type, id, userVO);
		renderJson(ResultUtil.succeed());
	}


	/**@author HCJ
	 * 下载订单的相关文件
	 * @param id 订单ID
	 * @date 2019年8月8日 下午4:00:40
	 */
	
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
	 * 删除订单的相关文件
	 * @param id 文件ID
	 * @date 2019年8月28日 上午9:45:06
	 */
	
	public void deleteOrderTable(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, UserController.SESSION_KEY_LOGIN_USER);
		if (productionService.deleteOrderTable(id, userVO)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 查询未排产普通订单
	 * @param type
	 * @date 2019年8月16日 上午11:42:33
	 */
	
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


	/**@author HCJ
	 * 查询未排产返工订单
	 * @param type
	 * @date 2019年8月16日 上午11:42:55
	 */
	
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


	/**@author HCJ
	 * 添加排产计划
	 * @param order 订单ID
	 * @param remark 备注
	 * @param schedulingQuantity 排产数量
	 * @param line 产线ID
	 * @param processGroup 工序组ID
	 * @param capacity 产能
	 * @date 2019年8月16日 上午11:43:23
	 */
	
	public void addPlan(String setting) {
		List<AddPlanInfo> addPlanInfos = JSONObject.parseArray(setting, AddPlanInfo.class);
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
	 * @date 2019年8月16日 上午11:48:39
	 */
	
	public void editPlan(Integer id, Boolean isUrgent, String remark, Integer schedulingQuantity, Integer line, String planStartTime, String planCompleteTime, String lineChangeTime, Integer capacity, Boolean isCompleted, String remainingReason, String productionPlanningNumber) {
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
		if (productionService.editPlan(id, isUrgent, remark, schedulingQuantity, line, start, end, lineChangeTime, capacity, isCompleted, remainingReason, productionPlanningNumber, userVO)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 编辑计划状态
	 * @param id 计划ID
	 * @param type 类型
	 * @date 2019年8月16日 上午11:50:05
	 */
	
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
		for (AuthorityVO authorityVO : userVO.getAuthorities()) {
			if (authorityVO.getUrls().contains("/production/editPlanStatus:startOrFinish") && !type.equals(Constant.WAIT_NOTIFICATION_PLANSTATUS - 1)) {
				productionService.editPlanStatus(id, type, userVO);
				renderJson(ResultUtil.succeed());
				return;
			}else if (authorityVO.getUrls().contains("/production/editPlanStatus:waitNotify") && type.equals(Constant.WAIT_NOTIFICATION_PLANSTATUS - 1)) {
				productionService.editPlanStatus(id, type, userVO);
				renderJson(ResultUtil.succeed());
				return;
			}
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
		ResultUtil result = ResultUtil.succeed(productionService.selectPlanProducedQuantity(id));
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

}
