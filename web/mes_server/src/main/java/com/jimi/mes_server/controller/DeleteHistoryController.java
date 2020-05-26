package com.jimi.mes_server.controller;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import javax.servlet.ServletOutputStream;
import javax.servlet.http.HttpServletResponse;
import com.jfinal.core.Controller;
import com.jimi.mes_server.service.DeleteHistoryService;
import com.jimi.mes_server.util.ResultUtil;

/**
 * 删除备份记录控制层
 * @type DeleteHistoryController
 * @Company 几米物联技术有限公司-自动化部
 * @author 汤如杰
 * @date 2019年1月8日
 */
public class DeleteHistoryController extends Controller {

	private static DeleteHistoryService deleteHistoryService = new DeleteHistoryService();


	/**
	 * 查询删除记录备份
	 * @param pageNo
	 * @param pageSize
	 * @param ascBy
	 * @param descBy
	 * @param filter
	 */
	public void select(Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter) {
		renderJson(ResultUtil.succeed(deleteHistoryService.select(pageNo, pageSize, ascBy, descBy, filter)));
	}


	/**
	 * 下载删除记录备份
	 * @param id
	 */
	public void download(String id) {
		File file = deleteHistoryService.download(id);
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
}
