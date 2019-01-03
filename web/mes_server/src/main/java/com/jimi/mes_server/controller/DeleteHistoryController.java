package com.jimi.mes_server.controller;

import java.io.File;

import com.jfinal.core.Controller;
import com.jimi.mes_server.service.DeleteHistoryService;
import com.jimi.mes_server.util.ResultUtil;

public class DeleteHistoryController extends Controller {
	
	private static DeleteHistoryService deleteHistoryService = new DeleteHistoryService();
	
	public void select(Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter) {
		renderJson(ResultUtil.succeed(deleteHistoryService.select(pageNo, pageSize, ascBy, descBy, filter)));
	}
	
	
	public void download(String id) {
		File file = deleteHistoryService.download(id);
		if (file != null) {
			renderFile(file);
			return ;
		}
		renderNull();
	}
}
