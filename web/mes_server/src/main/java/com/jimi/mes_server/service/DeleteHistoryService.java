package com.jimi.mes_server.service;

import java.io.File;
import java.util.Date;

import com.jfinal.aop.Enhancer;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Page;
import com.jfinal.plugin.activerecord.Record;
import com.jimi.mes_server.exception.OperationException;
import com.jimi.mes_server.service.base.SelectService;

/**
 * 删除备份记录业务层
 * @type DeleteHistoryService
 * @Company 几米物联技术有限公司-自动化部
 * @author 汤如杰
 * @date 2019年1月8日
 */
public class DeleteHistoryService {

	private static SelectService selectService = Enhancer.enhance(SelectService.class);

	private static final String SELECT_DELETE_HISTORY_BY_ID = "select * from DeleteHistory where ID = ?";


	/**
	 * 根据条件查询删除备份记录
	 * @param pageNo
	 * @param pageSize
	 * @param ascBy
	 * @param descBy
	 * @param filter
	 * @return
	 */
	public Page<Record> select(Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter) {
		Page<Record> records = selectService.select("DeleteHistory", pageNo, pageSize, ascBy, descBy, filter, null);
		return records;
	}


	/**
	 * 下载删除备份记录
	 * @param id
	 * @return
	 */
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


	/**
	 * 添加删除备份记录，仅供删除时使用
	 * @param fileName
	 * @param filePath
	 * @param time
	 */
	public void add(String fileName, String filePath, Date time) {
		Record record = new Record();
		record.set("FileName", fileName).set("FilePath", filePath).set("Time", time);
		Db.save("DeleteHistory", record);
	}
}
