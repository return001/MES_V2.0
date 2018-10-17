package com.jimi.mes_server.controller;

import java.io.IOException;
import java.io.OutputStream;

import javax.servlet.http.HttpServletResponse;

import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jfinal.core.paragetter.Para;
import com.jimi.mes_server.annotation.Access;
import com.jimi.mes_server.model.DataRelativeSheet;
import com.jimi.mes_server.service.ReportService;
import com.jimi.mes_server.service.base.SelectService;
import com.jimi.mes_server.util.ResultUtil;

/**
 * 报表控制器 <br>
 * <b>2018年5月23日</b>
 * 
 * @author 沫熊工作室 <a href="http://www.darhao.cc">www.darhao.cc</a>
 */
public class ReportController extends Controller {

	private static SelectService daoService = Enhancer.enhance(SelectService.class);

	private static ReportService reportService = Enhancer.enhance(ReportService.class);

	public void select(String table, Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter, Integer type) {
		renderJson(ResultUtil.succeed(daoService.select(table, pageNo, pageSize, ascBy, descBy, filter, type)));
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
	public void download(String table, String ascBy, String descBy, String filter) {
		OutputStream output = null;
		try {
			// 设置响应
			String fileName = getFileName(table);
			HttpServletResponse response = getResponse();
			response.reset();
			response.setHeader("Content-Disposition", "attachment; filename=" + new String((fileName).getBytes("utf-8"), "iso-8859-1"));
			response.setContentType("application/vnd.ms-excel");
			output = response.getOutputStream();
			reportService.download(table, ascBy, descBy, filter, output, fileName);
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
	 * @author HCJ 创建绑定关系
	 * @param dataRelativeSheet
	 * @date 2018年10月13日 下午8:57:03
	 */
	@Access({ "SuperAdmin" })
	public void createRelativeSheet(@Para("") DataRelativeSheet dataRelativeSheet) {
		if (reportService.createRelativeSheet(dataRelativeSheet)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}

	
	/**
	 * @author HCJ 更新绑定关系
	 * @param dataRelativeSheet
	 * @date 2018年10月13日 下午8:57:33
	 */
	@Access({ "SuperAdmin" })
	public void updateRelativeSheet(@Para("") DataRelativeSheet dataRelativeSheet) {
		if (reportService.updateRelativeSheet(dataRelativeSheet)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}

	
	/**
	 * @author HCJ 根据表格名称返回文件名
	 * @param table
	 * @date 2018年10月11日 下午5:59:11
	 */
	private String getFileName(String table) {
		String fileName = "未知报表.xlsx";
		switch (table) {
		case "Gps_AutoTest_Result":
			fileName = "SMT测试报表.xlsx";
			break;
		case "Gps_AutoTest_Result2":
			fileName = "组装功能测试1报表.xlsx";
			break;
		case "Gps_AutoTest_Result3":
			fileName = "组装功能测试2报表.xlsx";
			break;
		case "Gps_CoupleTest_Result":
			fileName = "耦合测试报表.xlsx";
			break;
		case "Gps_CartonBoxTwenty_Result":
			fileName = "卡通位测试报表.xlsx";
			break;
		case "Gps_ManuLdParam":
			fileName = "镭雕数据记录报表.xlsx";
			break;
		case "Gps_ManuPrintParam":
			fileName = "打印记录报表.xlsx";
			break;
		case "Gps_TestResult":
			fileName = "总结果记录报表.xlsx";
			break;
		case "GpsSMT_TcData":
			fileName = "SMT状态报表.xlsx";
			break;
		case "GpsTcData":
			fileName = "组装情况报表.xlsx";
			break;
		case "Gps_OperRecord":
			fileName = "操作记录报表.xlsx";
			break;
		case "DataRelativeSheet":
			fileName = "绑定结果报表.xlsx";
			break;
		default:
			break;
		}
		return fileName;
	}

}
