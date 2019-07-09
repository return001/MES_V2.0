package com.jimi.mes_server.controller;

import java.io.File;

import com.jfinal.core.Controller;
import com.jimi.mes_server.annotation.Access;
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
	@Access({ "SuperAdmin", "admin", "operator" })
	public void select(Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter) {
		renderJson(ResultUtil.succeed(deleteHistoryService.select(pageNo, pageSize, ascBy, descBy, filter)));
	}


	/**
	 * 下载删除记录备份
	 * @param id
	 */
	@Access({ "SuperAdmin", "admin", "operator" })
	public void download(String id) {
		File file = deleteHistoryService.download(id);
		if (file != null) {
			renderFile(file);
			return;
		}
		renderNull();
	}
}
