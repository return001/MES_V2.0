package com.jimi.mes_server.controller;

import java.io.IOException;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.Date;

import javax.servlet.http.HttpServletResponse;

import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jfinal.core.paragetter.Para;
import com.jimi.mes_server.annotation.Access;
import com.jimi.mes_server.entity.DeleteTable;
import com.jimi.mes_server.exception.OperationException;
import com.jimi.mes_server.model.DataRelativeSheet;
import com.jimi.mes_server.model.GpsUser;
import com.jimi.mes_server.service.ReportService;
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
	/*
	 * public static void main(String[] args) { PropKit.use("properties.ini");
	 * DruidPlugin dp = new DruidPlugin(PropKit.get("d_url"), PropKit.get("d_user"),
	 * PropKit.get("d_password")); ActiveRecordPlugin arp = new
	 * ActiveRecordPlugin(dp); arp.setDialect(new SqlServerDialect()); //
	 * 用什么数据库，就设置什么Dialect arp.setShowSql(true); MappingKit.mapping(arp);
	 * dp.start(); arp.start(); selectGpsManuPrintParam( 1, 1024, null,
	 * null,"12","45", null, new Date(), new Date(), 1); }
	 */


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
		ResultUtil result = ResultUtil.succeed(reportService.selectGpsManuSimDataParam(pageNo, pageSize, ascBy, descBy, filter));
		renderJson(result);
	}


	/**
	 * 根据条件删除数据库记录
	 * @param table
	 * @param filter
	 * @param type
	 */
	@Access({"SuperAdmin", "admin"})
	public void delete(String table, String filter, Integer type) {
		if (table.equals("Gps_ManuCpParam")) {
			throw new OperationException("Gps_ManuCpParam仅能查询不能删除");
		}
		if (table.equals("Gps_CartonBoxTwenty_Result")) {
			throw new OperationException("当前用户无权限删除");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		GpsUser user = TokenBox.get(tokenId, SESSION_KEY_LOGIN_USER);
		if (user.getUserType().equals("SuperAdmin")) {
			reportService.delete(table, filter, type);
			renderJson(ResultUtil.succeed());
			return ;
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
	@Access({"SuperAdmin", "admin"})
	public void deleteByIds(String table, String filter, Integer type) {
		if (table.equals("Gps_ManuCpParam")) {
			throw new OperationException("Gps_ManuCpParam仅能查询不能删除");
		}
		if (table.equals("Gps_CartonBoxTwenty_Result")) {
			throw new OperationException("当前用户无权限删除");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		GpsUser user = TokenBox.get(tokenId, SESSION_KEY_LOGIN_USER);
		if (user.getUserType().equals("SuperAdmin")) {
			reportService.deleteByIds(table, filter, type);
			renderJson(ResultUtil.succeed());
			return ;
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
	@Access({"SuperAdmin", "admin"})
	public void deleteGpsManuPrintParam(String startIMEI, String endIMEI, String zhiDan, Date startTime, Date endTime, Integer printType) {
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		String table = "Gps_ManuPrintParam";
		GpsUser user = TokenBox.get(tokenId, SESSION_KEY_LOGIN_USER);
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
		if (user.getUserType().equals("SuperAdmin")) {
			reportService.deleteGpsManuPrintParam(filter);
			renderJson(ResultUtil.succeed());
			return ;
		}
		if (user.getDeletePermission() == null) {
			throw new OperationException("当前用户无权限删除");
		}
		String[] deletePermissions = user.getDeletePermission().split(",");
		for (int i = 0; i < deletePermissions.length; i++) {
			if (deletePermissions[i].equals("1") && DeleteTable.getNameById(i).equals(table)) {
				reportService.deleteGpsManuPrintParam(filter);
				renderJson(ResultUtil.succeed());
				return ;
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
	@Access({"SuperAdmin", "admin"})
	public void deleteGpsManuSimDataParam(String startIMEI, String endIMEI, String zhiDan, Date startTime, Date endTime, String rID) {
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		GpsUser user = TokenBox.get(tokenId, SESSION_KEY_LOGIN_USER);
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
		if (user.getUserType().equals("SuperAdmin")) {
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
	@Access({"SuperAdmin"})
	public void download(String table, String ascBy, String descBy, String filter, Integer type) {
		OutputStream output = null;
		try {
			// 设置响应
			HttpServletResponse response = getResponse();
			reportService.download(table, ascBy, descBy, filter, type, response);
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
	@Access({"SuperAdmin"})
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
			reportService.download("Gps_ManuPrintParam", ascBy, descBy, filter, null, response);
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
	@Access({"SuperAdmin"})
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
			reportService.download("Gps_ManuSimDataParam", ascBy, descBy, filter, null, response);
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
