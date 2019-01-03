package com.jimi.mes_server.service;

import java.io.File;
import java.util.Date;

import com.jfinal.aop.Enhancer;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Page;
import com.jfinal.plugin.activerecord.Record;
import com.jimi.mes_server.exception.OperationException;
import com.jimi.mes_server.service.base.SelectService;

public class DeleteHistoryService {

	private static SelectService selectService = Enhancer.enhance(SelectService.class);
	private static final String SELECT_DELETE_HISTORY_BY_ID = "select * from DeleteHistory where ID = ?";
	
	
	public Page<Record> select(Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter){
		Page<Record> records = selectService.select("DeleteHistory", pageNo, pageSize, ascBy, descBy, filter, null);
		return records;
	}
	
	
	public File download(String id) {
		Record record = Db.findFirst(SELECT_DELETE_HISTORY_BY_ID, id);
		if (record == null) {
			return null;
		}
		String filePath = record.getStr("FilePath");
		File file = new File(filePath);
		if (!file.exists()) {
			throw new OperationException("文件不存在，下载失败");
		}
		return file;
	}
	
	
	public void add(String fileName, String filePath, Date time) {
		Record record = new Record();
		record.set("FileName", fileName).set("FilePath", filePath).set("Time", time);
		Db.save("DeleteHistory", record);
	}
}
