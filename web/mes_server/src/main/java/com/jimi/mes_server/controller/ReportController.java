package com.jimi.mes_server.controller;

import java.io.IOException;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.Date;

import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang3.StringUtils;

import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jfinal.core.paragetter.Para;
import com.jfinal.kit.StrKit;
import com.jimi.mes_server.annotation.Access;
import com.jimi.mes_server.entity.DeleteTable;
import com.jimi.mes_server.exception.OperationException;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.model.DataRelativeSheet;
import com.jimi.mes_server.model.LUserAccount;
import com.jimi.mes_server.service.ReportService;
import com.jimi.mes_server.service.UserService;
import com.jimi.mes_server.service.base.SelectService;
import com.jimi.mes_server.util.ResultUtil;
import com.jimi.mes_server.util.TokenBox;


/**
 * 报表控制器 <br>
 * <b>2018年5月23日</b>
 * 
 * @author 沫熊工作室 <a href="http://www.darhao.cc">www.darhao.cc</a>
 */
public class ReportController extends Controller {

	public static final String SESSION_KEY_LOGIN_USER = "loginUser";

	private static SelectService daoService = Enhancer.enhance(SelectService.class);

	private static ReportService reportService = Enhancer.enhance(ReportService.class);

	private static UserService userService = Enhancer.enhance(UserService.class);


	/**
	 * 根据表名和其他条件查询数据库
	 * @param table
	 * @param pageNo
	 * @param pageSize
	 * @param ascBy
	 * @param descBy
	 * @param filter
	 * @param type
	 */
	@Access({ "SuperAdmin", "admin", "operator" })
	public void select(String table, Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter, Integer type) {
		ResultUtil result = ResultUtil.succeed(daoService.select(table, pageNo, pageSize, ascBy, descBy, filter, type));
		renderJson(result);
	}


	/**
	 * 根据条件查询DataRelativeSheet表
	 * @param pageNo
	 * @param pageSize
	 * @param ascBy
	 * @param descBy
	 * @param filter
	 */
	@Access({ "SuperAdmin", "admin", "operator" })
	public void selectDataRelativeSheet(Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter, Boolean isReferred) {
		ResultUtil result = ResultUtil.succeed(reportService.selectDataRelativeSheet(pageNo, pageSize, ascBy, descBy, filter, isReferred));
		renderJson(result);
	}


	/**
	 * 根据条件查询Gps_ManuPrintParam表
	 * @param pageNo
	 * @param pageSize
	 * @param ascBy
	 * @param descBy
	 * @param startIMEI
	 * @param endIMEI
	 * @param zhiDan
	 * @param startTime
	 * @param endTime
	 * @param printType
	 */
	@Access({ "SuperAdmin", "admin", "operator" })
	public void selectGpsManuPrintParam(Integer pageNo, Integer pageSize, String ascBy, String descBy, String startIMEI, String endIMEI, String zhiDan, Date startTime, Date endTime, Integer printType) {
		String filter = "";
		if (zhiDan != null && !zhiDan.equals("")) {
			filter = filter + "(ZhiDan = '" + zhiDan + "')";
		}
		if (startIMEI != null && !startIMEI.equals("") && endIMEI != null && !endIMEI.equals("")) {
			if (!filter.equals("")) {
				filter = filter + " and ";
			}
			filter = filter + "(IMEI >= '" + startIMEI + "' and IMEI <= '" + endIMEI + "')";
		}
		if (startTime != null && endTime != null && printType != null) {
			if (!filter.equals("")) {
				filter = filter + " and ";
			}
			String startTimeString = formatDateToString(startTime, "yyyy.MM.dd HH:mm:ss");
			String endTimeString = formatDateToString(endTime, "yyyy.MM.dd HH:mm:ss");
			if (printType == 0) {
				filter = filter + "(((CH_ReEndPrintTime >= '" + startTimeString + "' and CH_ReEndPrintTime <= '" + endTimeString + "'))" + " or ((CH_ReEndPrintTime is null) and (CH_ReFirstPrintTime >= '" + startTimeString + "' and CH_ReFirstPrintTime <= '" + endTimeString + "'))" + " or ((CH_ReFirstPrintTime is null) and (CH_PrintTime >= '" + startTimeString + "' and CH_PrintTime <= '" + endTimeString + "')))";
			} else if (printType == 1) {
				filter = filter + "(((JS_ReEndPrintTime >= '" + startTimeString + "' and JS_ReEndPrintTime <= '" + endTimeString + "'))" + " or ((JS_ReEndPrintTime is null) and (JS_ReFirstPrintTime >= '" + startTimeString + "' and JS_ReFirstPrintTime <= '" + endTimeString + "'))" + " or ((JS_ReFirstPrintTime is null) and (JS_PrintTime >= '" + startTimeString + "' and JS_PrintTime <= '" + endTimeString + "')))";
			}
		}
		System.err.println(filter);
		ResultUtil result = ResultUtil.succeed(reportService.selectGpsManuPrintParam(pageNo, pageSize, ascBy, descBy, filter));
		renderJson(result);
	}


	/**
	 * 根据条件查询Gps_ManuSimDataParam
	 * @param pageNo
	 * @param pageSize
	 * @param ascBy
	 * @param descBy
	 * @param startIMEI
	 * @param endIMEI
	 * @param zhiDan
	 * @param startTime
	 * @param endTime
	 * @param rID
	 */
	@Access({ "SuperAdmin", "admin", "operator" })
	public void selectGpsManuSimDataParam(Integer pageNo, Integer pageSize, String ascBy, String descBy, String startIMEI, String endIMEI, String zhiDan, Date startTime, Date endTime, String rID) {
		String filter = "";
		if (rID != null && !rID.equals("")) {
			filter = filter + "(RID = '" + rID + "')";
		}
		if (startIMEI != null && !startIMEI.equals("") && endIMEI != null && !endIMEI.equals("")) {
			if (!filter.equals("")) {
				filter = filter + " and ";
			}
			filter = filter + "(IMEI >= '" + startIMEI + "' and IMEI <= '" + endIMEI + "')";
		}
		if (startTime != null && endTime != null) {
			if (!filter.equals("")) {
				filter = filter + " and ";
			}
			String startTimeString = formatDateToString(startTime, "yyyy/MM/dd HH:mm:ss");
			String endTimeString = formatDateToString(endTime, "yyyy/MM/dd HH:mm:ss");
			filter = filter + " (((ReSDTime >= '" + startTimeString + "' and ReSDTime <= '" + endTimeString + "'))" + " or ((ReSDTime is null) and (SDTime >= '" + startTimeString + "' and SDTime <= '" + endTimeString + "')))";
		}
		ResultUtil result = ResultUtil
				.succeed(reportService.selectGpsManuSimDataParam(pageNo, pageSize, ascBy, descBy, filter));
		renderJson(result);
	}


	/**
	 * 根据条件删除数据库记录
	 * @param table
	 * @param filter
	 * @param type
	 */
	@Access({ "SuperAdmin", "admin" })
	public void delete(String table, String filter, Integer type) {
		if (table.equals("Gps_ManuCpParam")) {
			throw new OperationException("Gps_ManuCpParam仅能查询不能删除");
		}
		if (table.equals("Gps_CartonBoxTwenty_Result")) {
			throw new OperationException("当前用户无权限删除");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccount user = TokenBox.get(tokenId, SESSION_KEY_LOGIN_USER);
		if (userService.getTypeName(user.getWebUserType()).equals("SuperAdmin")) {
			reportService.delete(table, filter, type);
			renderJson(ResultUtil.succeed());
			return;
		}
		if (user.getDeletePermission() == null) {
			throw new OperationException("当前用户无权限删除");
		}
		String[] deletePermissions = user.getDeletePermission().split(",");
		for (int i = 0; i < deletePermissions.length; i++) {
			if (deletePermissions[i].equals("1") && DeleteTable.getNameById(i).equals(table)) {
				reportService.delete(table, filter, type);
				renderJson(ResultUtil.succeed());
				return;
			}
		}
		throw new OperationException("当前用户无权限删除");
	}


	/**
	 * 根据条件删除数据库记录
	 * @param table
	 * @param filter
	 * @param type
	 */
	@Access({ "SuperAdmin", "admin" })
	public void deleteByIds(String table, String filter, Integer type) {
		if (table.equals("Gps_ManuCpParam")) {
			throw new OperationException("Gps_ManuCpParam仅能查询不能删除");
		}
		if (table.equals("Gps_CartonBoxTwenty_Result")) {
			throw new OperationException("当前用户无权限删除");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccount user = TokenBox.get(tokenId, SESSION_KEY_LOGIN_USER);
		if (userService.getTypeName(user.getWebUserType()).equals("SuperAdmin")) {
			reportService.deleteByIds(table, filter, type);
			renderJson(ResultUtil.succeed());
			return;
		}
		if (user.getDeletePermission() == null) {
			throw new OperationException("当前用户无权限删除");
		}
		String[] deletePermissions = user.getDeletePermission().split(",");
		for (int i = 0; i < deletePermissions.length; i++) {
			if (deletePermissions[i].equals("1") && DeleteTable.getNameById(i).equals(table)) {
				reportService.deleteByIds(table, filter, type);
				renderJson(ResultUtil.succeed());
				return;
			}
		}
		throw new OperationException("当前用户无权限删除");
	}


	/**
	 * 根据条件删除Gps_ManuPrintParam表记录
	 * @param startIMEI
	 * @param endIMEI
	 * @param zhiDan
	 * @param startTime
	 * @param endTime
	 * @param printType
	 */
	@Access({ "SuperAdmin", "admin" })
	public void deleteGpsManuPrintParam(String startIMEI, String endIMEI, String zhiDan, Date startTime, Date endTime, Integer printType) {
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		String table = "Gps_ManuPrintParam";
		LUserAccount user = TokenBox.get(tokenId, SESSION_KEY_LOGIN_USER);
		String filter = "";
		if (zhiDan != null && !zhiDan.equals("")) {
			filter = filter + "(ZhiDan = '" + zhiDan + "')";
		}
		if (startIMEI != null && !startIMEI.equals("") && endIMEI != null && !endIMEI.equals("")) {
			if (!filter.equals("")) {
				filter = filter + " and ";
			}
			filter = filter + "(IMEI >= '" + startIMEI + "' and IMEI <= '" + endIMEI + "')";
		}
		if (startTime != null && endTime != null && printType != null) {
			if (!filter.equals("")) {
				filter = filter + " and ";
			}
			String startTimeString = formatDateToString(startTime, "yyyy.MM.dd HH:mm:ss");
			String endTimeString = formatDateToString(endTime, "yyyy.MM.dd HH:mm:ss");
			if (printType == 0) {
				filter = filter + "(((CH_ReEndPrintTime >= '" + startTimeString + "' and CH_ReEndPrintTime <= '" + endTimeString + "'))" + " or ((CH_ReEndPrintTime is null) and (CH_ReFirstPrintTime >= '" + startTimeString + "' and CH_ReFirstPrintTime <= '" + endTimeString + "'))" + " or ((CH_ReFirstPrintTime is null) and (CH_PrintTime >= '" + startTimeString + "' and CH_PrintTime <= '" + endTimeString + "')))";
			} else if (printType == 1) {
				filter = filter + "(((JS_ReEndPrintTime >= '" + startTimeString + "' and JS_ReEndPrintTime <= '" + endTimeString + "'))" + " or ((JS_ReEndPrintTime is null) and (JS_ReFirstPrintTime >= '" + startTimeString + "' and JS_ReFirstPrintTime <= '" + endTimeString + "'))" + " or ((JS_ReFirstPrintTime is null) and (JS_PrintTime >= '" + startTimeString + "' and JS_PrintTime <= '" + endTimeString + "')))";
			}
		}
		if (userService.getTypeName(user.getWebUserType()).equals("SuperAdmin")) {
			reportService.deleteGpsManuPrintParam(filter);
			renderJson(ResultUtil.succeed());
			return;
		}
		if (user.getDeletePermission() == null) {
			throw new OperationException("当前用户无权限删除");
		}
		String[] deletePermissions = user.getDeletePermission().split(",");
		for (int i = 0; i < deletePermissions.length; i++) {
			if (deletePermissions[i].equals("1") && DeleteTable.getNameById(i).equals(table)) {
				reportService.deleteGpsManuPrintParam(filter);
				renderJson(ResultUtil.succeed());
				return;
			}
		}
		throw new OperationException("当前用户无权限删除");
	}


	/**
	 * 根据条件删除Gps_ManuSimDataParam记录
	 * @param startIMEI
	 * @param endIMEI
	 * @param zhiDan
	 * @param startTime
	 * @param endTime
	 * @param rID
	 */
	@Access({ "SuperAdmin", "admin" })
	public void deleteGpsManuSimDataParam(String startIMEI, String endIMEI, String zhiDan, Date startTime, Date endTime, String rID) {
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccount user = TokenBox.get(tokenId, SESSION_KEY_LOGIN_USER);
		String table = "Gps_ManuSimDataParam";
		String filter = "";
		if (rID != null && !rID.equals("")) {
			filter = filter + "(RID = '" + rID + "')";
		}
		if (startIMEI != null && !startIMEI.equals("") && endIMEI != null && !endIMEI.equals("")) {
			if (!filter.equals("")) {
				filter = filter + " and ";
			}
			filter = filter + "(IMEI >= '" + startIMEI + "' and IMEI <= '" + endIMEI + "')";
		}
		if (startTime != null && endTime != null) {
			if (!filter.equals("")) {
				filter = filter + " and ";
			}
			String startTimeString = formatDateToString(startTime, "yyyy/MM/dd HH:mm:ss");
			String endTimeString = formatDateToString(endTime, "yyyy/MM/dd HH:mm:ss");
			filter = filter + " (((ReSDTime >= '" + startTimeString + "' and ReSDTime <= '" + endTimeString + "'))" + " or ((ReSDTime is null) and (SDTime >= '" + startTimeString + "' and SDTime <= '" + endTimeString + "')))";
		}
		if (userService.getTypeName(user.getWebUserType()).equals("SuperAdmin")) {
			reportService.deleteGpsManuSimDataParam(filter);
			renderJson(ResultUtil.succeed());
			return;
		}
		if (user.getDeletePermission() == null) {
			throw new OperationException("当前用户无权限删除");
		}
		String[] deletePermissions = user.getDeletePermission().split(",");
		for (int i = 0; i < deletePermissions.length; i++) {
			if (deletePermissions[i].equals("1") && DeleteTable.getNameById(i).equals(table)) {
				reportService.deleteGpsManuSimDataParam(filter);
				renderJson(ResultUtil.succeed());
				return;
			}
		}
		throw new OperationException("当前用户无权限删除");
	}


	/**
	 * @author HCJ 下载报表
	 * @param table
	 * @param ascBy
	 * @param descBy
	 * @param filter
	 * @date 2018年10月11日 下午5:58:28
	 */
	@Access({ "SuperAdmin" })
	public void download(String table, String ascBy, String descBy, String filter, Integer type) {
		OutputStream output = null;
		try {
			// 设置响应
			HttpServletResponse response = getResponse();
			output = response.getOutputStream();
			reportService.download(table, ascBy, descBy, filter, type, response, output);
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


	/**
	 * 根据条件下载Gps_ManuPrintParam表记录
	 * @param ascBy
	 * @param descBy
	 * @param startIMEI
	 * @param endIMEI
	 * @param zhiDan
	 * @param startTime
	 * @param endTime
	 * @param printType
	 */
	@Access({ "SuperAdmin" })
	public void downloadGpsManuPrintParam(String ascBy, String descBy, String startIMEI, String endIMEI, String zhiDan, Date startTime, Date endTime, Integer printType) {
		String filter = "";
		if (zhiDan != null && !zhiDan.equals("")) {
			filter = filter + "(ZhiDan = '" + zhiDan + "')";
		}
		if (startIMEI != null && !startIMEI.equals("") && endIMEI != null && !endIMEI.equals("")) {
			if (!filter.equals("")) {
				filter = filter + " and ";
			}
			filter = filter + "(IMEI >= '" + startIMEI + "' and IMEI <= '" + endIMEI + "')";
		}
		if (startTime != null && endTime != null && printType != null) {
			if (!filter.equals("")) {
				filter = filter + " and ";
			}
			String startTimeString = formatDateToString(startTime, "yyyy.MM.dd HH:mm:ss");
			String endTimeString = formatDateToString(endTime, "yyyy.MM.dd HH:mm:ss");
			if (printType == 0) {
				filter = filter + "(((CH_ReEndPrintTime >= '" + startTimeString + "' and CH_ReEndPrintTime <= '" + endTimeString + "'))" + " or ((CH_ReEndPrintTime is null) and (CH_ReFirstPrintTime >= '" + startTimeString + "' and CH_ReFirstPrintTime <= '" + endTimeString + "'))" + " or ((CH_ReFirstPrintTime is null) and (CH_PrintTime >=" + startTimeString + " and CH_PrintTime <= " + endTimeString + ")))";
			} else if (printType == 1) {
				filter = filter + "(((JS_ReEndPrintTime >= '" + startTimeString + "' and JS_ReEndPrintTime <= '" + endTimeString + "'))" + " or ((JS_ReEndPrintTime is null) and (JS_ReFirstPrintTime >= '" + startTimeString + "' and JS_ReFirstPrintTime <= '" + endTimeString + "'))" + " or ((JS_ReFirstPrintTime is null) and (JS_PrintTime >= '" + startTimeString + "' and JS_PrintTime <= '" + endTimeString + "')))";
			}
		}
		OutputStream output = null;
		try {
			// 设置响应
			HttpServletResponse response = getResponse();
			output = response.getOutputStream();
			reportService.download("Gps_ManuPrintParam", ascBy, descBy, filter, null, response, output);
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


	/**
	 * 根据条件下载Gps_ManuSimDataParam表记录
	 * @param ascBy
	 * @param descBy
	 * @param startIMEI
	 * @param endIMEI
	 * @param zhiDan
	 * @param startTime
	 * @param endTime
	 * @param rID
	 */
	@Access({ "SuperAdmin" })
	public void downloadGpsManuSimDataParam(String ascBy, String descBy, String startIMEI, String endIMEI, String zhiDan, Date startTime, Date endTime, String rID) {
		String filter = "";
		if (rID != null && !rID.equals("")) {
			filter = filter + "(RID = '" + rID + "')";
		}
		if (startIMEI != null && !startIMEI.equals("") && endIMEI != null && !endIMEI.equals("")) {
			if (!filter.equals("")) {
				filter = filter + " and ";
			}
			filter = filter + "(IMEI >= '" + startIMEI + "' and IMEI <= '" + endIMEI + "')";
		}
		if (startTime != null && endTime != null) {
			if (!filter.equals("")) {
				filter = filter + " and ";
			}
			String startTimeString = formatDateToString(startTime, "yyyy/MM/dd HH:mm:ss");
			String endTimeString = formatDateToString(endTime, "yyyy/MM/dd HH:mm:ss");
			filter = filter + " (((ReSDTime >= '" + startTimeString + "' and ReSDTime <= '" + endTimeString + "'))" + " or ((ReSDTime is null) and (SDTime >= '" + startTimeString + "' and SDTime <= '" + endTimeString + "')))";
		}
		OutputStream output = null;
		try {
			// 设置响应
			HttpServletResponse response = getResponse();
			output = response.getOutputStream();
			reportService.download("Gps_ManuSimDataParam", ascBy, descBy, filter, null, response, output);
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


	/**
	 * @author HCJ 更新绑定关系
	 * @param dataRelativeSheet
	 * @date 2018年10月13日 下午8:57:33
	 */
	@Access({"SuperAdmin"})
	public void updateRelativeSheet(@Para("") DataRelativeSheet dataRelativeSheet) {
		if (reportService.updateRelativeSheet(dataRelativeSheet)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 根据IMEI1清空关联表相关字段
	 * @param imeis IMEI1集合
	 * @param items 需要进行操作的字段
	 * @date 2019年5月29日 下午3:39:32
	 */
	@Access({ "SuperAdmin" })
	public void cleanupInRel(String imei, String items) {
		if (StrKit.isBlank(imei) || StrKit.isBlank(items)) {
			throw new ParameterException("参数不能存在空值");
		}
		if (reportService.cleanupInRel(imei, items)) {
			renderJson(ResultUtil.succeed());
		}
	}


	/**@author HCJ
	 * 根据订单名称获取所有相似的订单信息
	 * @param zhiDan 订单名称
	 * @date 2019年6月5日 下午3:53:14
	 */
	@Access({ "SuperAdmin","admin","operator" })
	public void selectZhiDanInfo(String zhiDan) {
		if(StringUtils.isBlank(zhiDan)) {
			throw new ParameterException("参数不能为空");
		}
		renderJson(ResultUtil.succeed(reportService.selectZhiDanInfo(zhiDan)));
	}


	/**@author HCJ
	 * 查询未使用的IMEI
	 * @param startIMEI 开始IMEI
	 * @param endIMEI 结束IMEI
	 * @param zhiDan 订单名称
	 * @date 2019年6月5日 下午3:53:59
	 */
	@Access({ "SuperAdmin","admin","operator" })
	public void selectUnusedIMEI(String startIMEI, String endIMEI, String zhiDan) {
		if(StringUtils.isAnyBlank(zhiDan,startIMEI,endIMEI)) {
			throw new ParameterException("参数不能为空");
		}
		if(startIMEI.length()!=endIMEI.length()) {
			throw new ParameterException("起止IMEI长度必须相等");
		}
		renderJson(ResultUtil.succeed(reportService.selectUnusedIMEI(startIMEI, endIMEI, zhiDan)));
	}


	/**@author HCJ
	 * 下载未使用的IMEI文件格式为Excel
	 * @param startIMEI 开始IMEI
	 * @param endIMEI 结束IMEI
	 * @param zhiDan 订单名称
	 * @param response http响应
	 * @param output 输出流
	 * @date 2019年6月5日 下午3:57:01
	 */
	@Access({"SuperAdmin"})
	public void downloadUnusedIMEI(String startIMEI, String endIMEI, String zhiDan) {
		OutputStream output = null;
		try {
			// 设置响应
			HttpServletResponse response = getResponse();
			output = response.getOutputStream();
			reportService.downloadUnusedIMEI(startIMEI,  endIMEI,  zhiDan, response,output);
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
	 * 根据条件进行多表查询
	 * @param imei IMEI号
	 * @param sn SN号
	 * @param zhiDan ZhiDan号
	 * @param type 参数类型
	 * @date 2019年6月10日 下午3:34:01
	 */
	/*@Access({ "SuperAdmin", "admin", "operator" })*/
	public void multiTableQuery(String imei, String sn, String zhiDan,Integer type) {
		if (type==null) {
			throw new ParameterException("类型不能为空");
		}
		switch (type) {
		case 0:
		case 1:	
			if (StringUtils.isAllBlank(imei, sn, zhiDan)) {
				throw new ParameterException("IMEI、SN和ZhiDan不能全部为空");
			}
			break;
		case 2:	
			if (StringUtils.isBlank(imei)) {
				throw new ParameterException("当前类型下IMEI号不能为空");
			}
			if (!imei.contains(",")||imei.split(",").length != 2||StringUtils.isAnyBlank(imei.split(","))) {
				throw new ParameterException("当前类型下IMEI格式错误");
			}
			break;
		default:
			throw new ParameterException("无法识别的类型");
		}
		renderJson(ResultUtil.succeed(reportService.multiTableQuery(imei, sn, zhiDan,type)));
	}


	/**@author HCJ
	 * 根据条件进行多表删除
	 * @param imei IMEI号
	 * @param sn SN号
	 * @param zhiDan ZhiDan号
	 * @param type 参数类型
	 * @date 2019年6月10日 下午3:34:01
	 */
	/*@Access({ "SuperAdmin" })*/
	public void multiTableDelete(String imei, String sn, String zhiDan,Integer type) {
		if (type==null) {
			throw new ParameterException("类型不能为空");
		}
		switch (type) {
		case 0:
		case 1:	
			if (StringUtils.isAllBlank(imei, sn, zhiDan)) {
				throw new ParameterException("IMEI、SN和ZhiDan不能全部为空");
			}
			break;
		case 2:	
			if (StringUtils.isBlank(imei)) {
				throw new ParameterException("当前类型下IMEI号不能为空");
			}
			if (!imei.contains(",")||imei.split(",").length != 2||StringUtils.isAnyBlank(imei.split(","))) {
				throw new ParameterException("当前类型下IMEI格式错误");
			}
			break;
		default:
			throw new ParameterException("无法识别的类型");
		}
		/*String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccount user = TokenBox.get(tokenId, SESSION_KEY_LOGIN_USER);
		if (userService.getTypeName(user.getWebUserType()).equals("SuperAdmin")) {
			reportService.multiTableDelete(imei, sn, zhiDan,type);
			renderJson(ResultUtil.succeed());
			return;
		}
		if (user.getDeletePermission() == null) {
			throw new OperationException("当前用户无权限删除");
		}
		String[] deletePermissions = user.getDeletePermission().split(",");
		for (int i = 0; i < deletePermissions.length; i++) {
			if (!deletePermissions[i].equals("1")) {
				throw new OperationException("当前用户无权限批量删除");
			}
		}*/
		reportService.multiTableDelete(imei, sn, zhiDan,type);
		renderJson(ResultUtil.succeed());
	}


	/**
	 * 根据模板转换时间为字符串
	 * @param time
	 * @param pattern
	 * @return
	 */
	private static String formatDateToString(Date time, String pattern) {
		SimpleDateFormat simpleDateFormat = new SimpleDateFormat(pattern);
		String timeString = simpleDateFormat.format(time);
		return timeString;
	}

}
