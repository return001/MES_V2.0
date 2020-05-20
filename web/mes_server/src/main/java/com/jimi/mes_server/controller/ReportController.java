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
import com.jimi.mes_server.entity.DeleteTable;
import com.jimi.mes_server.entity.vo.AuthorityVO;
import com.jimi.mes_server.entity.vo.LUserAccountVO;
import com.jimi.mes_server.exception.OperationException;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.model.DataRelativeSheet;
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

	private static final int TABLE_NUM = 10;

	private static final int SHORT_HEX_IMEI_LENGTH = 8;

	private static final int LONG_HEX_IMEI_LENGTH = 14;


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
	
	public void selectGpsManuPrintParam(Integer pageNo, Integer pageSize, String ascBy, String descBy, String startIMEI, String endIMEI, String zhiDan, Date startTime, Date endTime, Integer printType, Boolean isIMEIHex) {
		String filter = "";
		if (zhiDan != null && !zhiDan.equals("")) {
			filter = filter + "(ZhiDan = '" + zhiDan + "')";
		}
		if (startIMEI != null && !startIMEI.equals("") && endIMEI != null && !endIMEI.equals("")) {
			if (isIMEIHex) {
				if ((startIMEI.length() != SHORT_HEX_IMEI_LENGTH && startIMEI.length() != LONG_HEX_IMEI_LENGTH) || (endIMEI.length() != SHORT_HEX_IMEI_LENGTH && endIMEI.length() != LONG_HEX_IMEI_LENGTH)) {
					throw new ParameterException("16进制的IMEI号长度必须为8位或者14位");
				}
			}
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
	
	public void selectGpsManuSimDataParam(Integer pageNo, Integer pageSize, String ascBy, String descBy, String startIMEI, String endIMEI, String zhiDan, Date startTime, Date endTime, String rID, Boolean isIMEIHex) {
		String filter = "";
		if (rID != null && !rID.equals("")) {
			filter = filter + "(RID = '" + rID + "')";
		}
		if (startIMEI != null && !startIMEI.equals("") && endIMEI != null && !endIMEI.equals("")) {
			if (isIMEIHex) {
				if ((startIMEI.length() != SHORT_HEX_IMEI_LENGTH && startIMEI.length() != LONG_HEX_IMEI_LENGTH) || (endIMEI.length() != SHORT_HEX_IMEI_LENGTH && endIMEI.length() != LONG_HEX_IMEI_LENGTH)) {
					throw new ParameterException("16进制的IMEI号长度必须为8位或者14位");
				}
			}
			if (!filter.equals("")) {
				filter = filter + " and ";
			}
			filter = filter + "(IMEI >= '" + startIMEI + "' and IMEI <= '" + endIMEI + "')";
		}
		if (startTime != null && endTime != null) {
			if (!filter.equals("")) {
				filter = filter + " and ";
			}
			String startTimeString = formatDateToString(startTime, "yyyy/M/d HH:mm:ss");
			String endTimeString = formatDateToString(endTime, "yyyy/M/d HH:mm:ss");
			filter = filter + " (((ReSDTime >= '" + startTimeString + "' and ReSDTime <= '" + endTimeString + "'))" + " or ((ReSDTime is null) and (SDTime >= '" + startTimeString + "' and SDTime <= '" + endTimeString + "')))";
		}
		ResultUtil result = ResultUtil.succeed(reportService.selectGpsManuSimDataParam(pageNo, pageSize, ascBy, descBy, filter));
		renderJson(result);
	}


	/**
	 * 根据条件删除数据库记录,需要权限：连接:表名
	 * @param table
	 * @param filter
	 * @param type
	 */
	
	public void delete(String table, String filter, Integer type) {
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, SESSION_KEY_LOGIN_USER);
		for (AuthorityVO authorityVO : userVO.getAuthorities()) {
			if (authorityVO.getUrls().contains("/report/delete:" + table)) {
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
	
	public void deleteByIds(String table, String filter, Integer type) {
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, SESSION_KEY_LOGIN_USER);
		for (AuthorityVO authorityVO : userVO.getAuthorities()) {
			if (authorityVO.getUrls().contains("/report/deleteByIds:" + table)) {
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
	
	public void deleteGpsManuPrintParam(String startIMEI, String endIMEI, String zhiDan, Date startTime, Date endTime, Integer printType, Boolean isIMEIHex) {
		String filter = "";
		if (zhiDan != null && !zhiDan.equals("")) {
			filter = filter + "(ZhiDan = '" + zhiDan + "')";
		}
		if (startIMEI != null && !startIMEI.equals("") && endIMEI != null && !endIMEI.equals("")) {
			if (isIMEIHex) {
				if ((startIMEI.length() != SHORT_HEX_IMEI_LENGTH && startIMEI.length() != LONG_HEX_IMEI_LENGTH) || (endIMEI.length() != SHORT_HEX_IMEI_LENGTH && endIMEI.length() != LONG_HEX_IMEI_LENGTH)) {
					throw new ParameterException("16进制的IMEI号长度必须为8位或者14位");
				}
			}
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
		reportService.deleteGpsManuPrintParam(filter);
		renderJson(ResultUtil.succeed());
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
	
	public void deleteGpsManuSimDataParam(String startIMEI, String endIMEI, String zhiDan, Date startTime, Date endTime, String rID, Boolean isIMEIHex) {
		String filter = "";
		if (rID != null && !rID.equals("")) {
			filter = filter + "(RID = '" + rID + "')";
		}
		if (startIMEI != null && !startIMEI.equals("") && endIMEI != null && !endIMEI.equals("")) {
			if (isIMEIHex) {
				if ((startIMEI.length() != SHORT_HEX_IMEI_LENGTH && startIMEI.length() != LONG_HEX_IMEI_LENGTH) || (endIMEI.length() != SHORT_HEX_IMEI_LENGTH && endIMEI.length() != LONG_HEX_IMEI_LENGTH)) {
					throw new ParameterException("16进制的IMEI号长度必须为8位或者14位");
				}
			}
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
		reportService.deleteGpsManuSimDataParam(filter);
		renderJson(ResultUtil.succeed());
	}


	/**
	 * @author HCJ 下载报表
	 * @param table
	 * @param ascBy
	 * @param descBy
	 * @param filter
	 * @date 2018年10月11日 下午5:58:28
	 */
	
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
	
	public void downloadGpsManuPrintParam(String ascBy, String descBy, String startIMEI, String endIMEI, String zhiDan, Date startTime, Date endTime, Integer printType, Boolean isIMEIHex) {
		String filter = "";
		if (zhiDan != null && !zhiDan.equals("")) {
			filter = filter + "(ZhiDan = '" + zhiDan + "')";
		}
		if (startIMEI != null && !startIMEI.equals("") && endIMEI != null && !endIMEI.equals("")) {
			if (isIMEIHex) {
				if ((startIMEI.length() != SHORT_HEX_IMEI_LENGTH && startIMEI.length() != LONG_HEX_IMEI_LENGTH) || (endIMEI.length() != SHORT_HEX_IMEI_LENGTH && endIMEI.length() != LONG_HEX_IMEI_LENGTH)) {
					throw new ParameterException("16进制的IMEI号长度必须为8位或者14位");
				}
			}
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
	
	public void downloadGpsManuSimDataParam(String ascBy, String descBy, String startIMEI, String endIMEI, String zhiDan, Date startTime, Date endTime, String rID, Boolean isIMEIHex) {
		String filter = "";
		if (rID != null && !rID.equals("")) {
			filter = filter + "(RID = '" + rID + "')";
		}
		if (startIMEI != null && !startIMEI.equals("") && endIMEI != null && !endIMEI.equals("")) {
			if (isIMEIHex) {
				if ((startIMEI.length() != SHORT_HEX_IMEI_LENGTH && startIMEI.length() != LONG_HEX_IMEI_LENGTH) || (endIMEI.length() != SHORT_HEX_IMEI_LENGTH && endIMEI.length() != LONG_HEX_IMEI_LENGTH)) {
					throw new ParameterException("16进制的IMEI号长度必须为8位或者14位");
				}
			}
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
	
	public void selectZhiDanInfo(String zhiDan) {
		if (StringUtils.isBlank(zhiDan)) {
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
	
	public void selectUnusedIMEI(String startIMEI, String endIMEI, String zhiDan) {
		if (StringUtils.isAnyBlank(zhiDan, startIMEI, endIMEI)) {
			throw new ParameterException("参数不能为空");
		}
		if (startIMEI.length() != endIMEI.length()) {
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
	
	public void downloadUnusedIMEI(String startIMEI, String endIMEI, String zhiDan) {
		OutputStream output = null;
		try {
			// 设置响应
			HttpServletResponse response = getResponse();
			output = response.getOutputStream();
			reportService.downloadUnusedIMEI(startIMEI, endIMEI, zhiDan, response, output);
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
	
	public void multiTableQuery(String imei, String sn, String zhiDan, Integer type, Boolean isIMEIHex) {
		if (type == null) {
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
			String[] imeis = imei.split(",");
			if (!imei.contains(",") || imeis.length != 2 || StringUtils.isAnyBlank(imeis)) {
				throw new ParameterException("当前类型下IMEI格式错误");
			}
			if (isIMEIHex) {
				for (String eachIMEI : imeis) {
					if (eachIMEI.length() != SHORT_HEX_IMEI_LENGTH && eachIMEI.length() != LONG_HEX_IMEI_LENGTH) {
						throw new ParameterException("16进制的IMEI号长度必须为8位或者14位");
					}
				}
			}
			break;
		default:
			throw new ParameterException("无法识别的类型");
		}
		renderJson(ResultUtil.succeed(reportService.multiTableQuery(imei, sn, zhiDan, type)));
	}


	/**@author HCJ
	 * 根据条件进行多表删除
	 * @param imei IMEI号
	 * @param sn SN号
	 * @param zhiDan ZhiDan号
	 * @param type 参数类型
	 * @param deleteTable 需要进行删除的表格
	 * @date 2019年6月10日 下午3:34:01
	 */
	
	public void multiTableDelete(String imei, String sn, String zhiDan, Integer type, String deleteTable, Boolean isIMEIHex) {
		if (isIMEIHex != null && isIMEIHex) {
			for (String eachIMEI : imei.split(",")) {
				if (eachIMEI.length() != SHORT_HEX_IMEI_LENGTH && eachIMEI.length() != LONG_HEX_IMEI_LENGTH) {
					throw new ParameterException("16进制的IMEI号长度必须为8位或者14位");
				}
			}
		}
		if (StrKit.notBlank(deleteTable) && (!deleteTable.contains(",") || deleteTable.split(",").length != TABLE_NUM)) {
			throw new ParameterException("deleteTable参数格式错误");
		}
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, SESSION_KEY_LOGIN_USER);
		String[] tables = deleteTable.split(",");
		
		
		StringBuilder sb = new StringBuilder();
		for (AuthorityVO authorityVO : userVO.getAuthorities()) {
				sb.append(authorityVO.getUrls());
				sb.append(",");
		}
		for (int i = 0; i < tables.length; i++) {
			if (Integer.parseInt(tables[i]) == 1) {
				if (sb.indexOf("/report/multiTableDelete:" + DeleteTable.getNameById(i)) == -1) {
					throw new OperationException("存在用户无权限删除的表：" + DeleteTable.getNameById(i));
				}
			}
		}
		reportService.multiTableDelete(imei, sn, zhiDan, type, deleteTable);
		renderJson(ResultUtil.succeed());
	}


	/**@author HCJ
	 * 查询卡通箱表数据
	 * @param pageNo 页码
	 * @param pageSize 页大小
	 * @param ascBy 以哪个字段增序
	 * @param descBy 以哪个字段降序
	 * @param filter 查询条件
	 * @param isReferred 是否与关联表相关联
	 * @date 2019年6月14日 上午8:51:06
	 */
	
	public void selectGpsCartonBox(Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter, Boolean isReferred) {
		if (isReferred == null) {
			throw new ParameterException("isReferred不能为空");
		}
		ResultUtil result = ResultUtil.succeed(reportService.selectGpsCartonBox(pageNo, pageSize, ascBy, descBy, filter, isReferred));
		renderJson(result);
	}


	/**@author HCJ
	 * 下载卡通箱表和关联表相互关联的数据
	 * @param ascBy 以哪个字段增序
	 * @param descBy 以哪个字段降序
	 * @param filter 查询条件
	 * @param isReferred 是否与关联表相关联
	 * @date 2019年6月14日 上午8:52:47
	 */
	
	public void downloadGpsCartonBox(String ascBy, String descBy, String filter, Boolean isReferred) {
		if (isReferred == null || !isReferred) {
			throw new ParameterException("isReferred必须为true");
		}
		OutputStream output = null;
		try {
			// 设置响应
			HttpServletResponse response = getResponse();
			output = response.getOutputStream();
			reportService.downloadGpsCartonBox(ascBy, descBy, filter, isReferred, response, output);
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
	 * 将多表查询的结果导出为Excel文件
	 * @param imei IMEI号
	 * @param sn SN号
	 * @param zhiDan ZhiDan号
	 * @param type 参数类型
	 * @date 2019年6月25日 下午3:25:43
	 */
	
	public void downloadMultiTable(String imei, String sn, String zhiDan, Integer type, Boolean isIMEIHex) {
		if (isIMEIHex) {
			for (String eachIMEI : imei.split(",")) {
				if (eachIMEI.length() != SHORT_HEX_IMEI_LENGTH && eachIMEI.length() != LONG_HEX_IMEI_LENGTH) {
					throw new ParameterException("16进制的IMEI号长度必须为8位或者14位");
				}
			}
		}
		OutputStream output = null;
		try {
			// 设置响应
			HttpServletResponse response = getResponse();
			output = response.getOutputStream();
			reportService.downloadMultiTable(imei, sn, zhiDan, type, response, output);
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
	 * 查询看板数据
	 * @param line 产线ID，目前有0：组装,1：测试,2：包装
	 * @date 2019年8月1日 上午10:08:08
	 */
	public void selectDashboardTable(Integer line) {
		if (line == null) {
			throw new ParameterException("参数不能为空");
		}
		renderJson(ResultUtil.succeed(reportService.selectDashboardTable(line)));
	}


	/**@author HCJ
	 * 查询看板表格数据客户版
	 * @param line 产线ID，目前有0：组装,1：测试,2：包装
	 * @date 2019年9月2日 上午10:31:48
	 */
	public void selectDashboardTableForCustomer(Integer line) {
		if (line == null) {
			throw new ParameterException("参数不能为空");
		}
		renderJson(ResultUtil.succeed(reportService.selectDashboardTableForCustomer(line)));
	}


	/**@author HCJ
	 * 查询看板不良数据
	 * @param line 产线ID，目前有0：组装,1：测试,2：包装
	 * @date 2019年8月1日 上午10:08:08
	 */
	public void selectErrorMsg(Integer line) {
		if (line == null) {
			throw new ParameterException("参数不能为空");
		}
		renderJson(ResultUtil.succeed(reportService.selectErrorMsg(line)));
	}


	/**@author HCJ
	 * 查询看板不良数据客户版
	 * @param line 产线ID，目前有0：组装,1：测试,2：包装
	 * @date 2019年9月2日 上午10:32:21
	 */
	public void selectErrorMsgForCustomer(Integer line) {
		if (line == null) {
			throw new ParameterException("参数不能为空");
		}
		renderJson(ResultUtil.succeed(reportService.selectErrorMsgForCustomer(line)));
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
