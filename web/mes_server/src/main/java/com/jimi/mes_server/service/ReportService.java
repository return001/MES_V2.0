package com.jimi.mes_server.service;

import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.Date;

import javax.servlet.http.HttpServletResponse;

import com.jfinal.aop.Enhancer;
import com.jfinal.kit.PathKit;
import com.jfinal.kit.PropKit;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Page;
import com.jfinal.plugin.activerecord.Record;
import com.jfinal.plugin.activerecord.SqlPara;
import com.jimi.mes_server.exception.OperationException;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.model.DataRelativeSheet;
import com.jimi.mes_server.service.base.DeleteService;
import com.jimi.mes_server.service.base.SelectService;
import com.jimi.mes_server.util.ExcelHelper;

/**报表业务层
 * @author   HCJ
 * @date     2018年10月11日 下午6:00:21
 */
public class ReportService extends SelectService{

	private static SelectService daoService = Enhancer.enhance(SelectService.class);
	
	private static DeleteService deleteService = Enhancer.enhance(DeleteService.class);
	
	private static DeleteHistoryService deleteHistoryService = new DeleteHistoryService();
	
	private static final String SELECT_DATARELATIVESHEET_AND_PRINTPARAM = "SELECT DataRelativeSheet.sN AS DataRelativeSheet_SN,DataRelativeSheet.iMEI1 AS DataRelativeSheet_IMEI1,DataRelativeSheet.iMEI2 AS DataRelativeSheet_IMEI2,DataRelativeSheet.iMEI3 AS DataRelativeSheet_IMEI3,DataRelativeSheet.iMEI4 AS DataRelativeSheet_IMEI4,DataRelativeSheet.iMEI5 AS DataRelativeSheet_IMEI5,DataRelativeSheet.iMEI6 AS DataRelativeSheet_IMEI6,DataRelativeSheet.iMEI7 AS DataRelativeSheet_IMEI7,DataRelativeSheet.iMEI8 AS DataRelativeSheet_IMEI8,DataRelativeSheet.iMEI9 AS DataRelativeSheet_IMEI9,DataRelativeSheet.iMEI10 AS DataRelativeSheet_IMEI10,DataRelativeSheet.iMEI11 AS DataRelativeSheet_IMEI11,DataRelativeSheet.iMEI12 AS DataRelativeSheet_IMEI12,DataRelativeSheet.iMEI13 AS DataRelativeSheet_IMEI13,DataRelativeSheet.zhiDan AS DataRelativeSheet_ZhiDan,DataRelativeSheet.testTime AS DataRelativeSheet_TestTime,DataRelativeSheet.simEffectiveDate AS DataRelativeSheet_SimEffectiveDate,Gps_ManuPrintParam.sN AS GpsManuprintparam_SN,Gps_ManuPrintParam.zhiDan AS GpsManuprintparam_ZhiDan,Gps_ManuPrintParam.softModel AS GpsManuprintparam_SoftModel,Gps_ManuPrintParam.js_Templatepath AS GpsManuprintparam_JS_TemplatePath,Gps_ManuPrintParam.js_Refirstprinttime AS GpsManuprintparam_JS_ReFirstPrintTime,Gps_ManuPrintParam.js_Reendprinttime AS GpsManuprintparam_JS_ReEndPrintTime,Gps_ManuPrintParam.ch_Templatepath1 AS GpsManuprintparam_CH_TemplatePath1,Gps_ManuPrintParam.ch_Templatepath2 AS GpsManuprintparam_CH_TemplatePath2,Gps_ManuPrintParam.ch_Refirstprinttime AS GpsManuprintparam_CH_ReFirstPrintTime,Gps_ManuPrintParam.ch_Reendprinttime AS GpsManuprintparam_CH_ReEndPrintTime,Gps_ManuPrintParam.ch_Reprintnum AS GpsManuprintparam_CH_RePrintNum,Gps_ManuPrintParam.iCCID AS GpsManuprintparam_ICCID,Gps_ManuPrintParam.mAC AS GpsManuprintparam_MAC,Gps_ManuPrintParam.equipment AS GpsManuprintparam_Equipment,Gps_ManuPrintParam.rFID AS GpsManuprintparam_RFID,Gps_ManuPrintParam.iD AS GpsManuprintparam_ID,Gps_ManuPrintParam.iMEI AS GpsManuprintparam_IMEI,Gps_ManuPrintParam.sIM AS GpsManuprintparam_SIM,Gps_ManuPrintParam.vIP AS GpsManuprintparam_VIP,Gps_ManuPrintParam.bAT AS GpsManuprintparam_BAT,Gps_ManuPrintParam.remark AS GpsManuprintparam_Remark,Gps_ManuPrintParam.js_Printtime AS GpsManuprintparam_JS_PrintTime,Gps_ManuPrintParam.js_Reprintnum AS GpsManuprintparam_JS_RePrintNum,Gps_ManuPrintParam.ch_Printtime AS GpsManuprintparam_CH_PrintTime,Gps_ManuPrintParam.userName AS GpsManuprintparam_UserName,Gps_ManuPrintParam.version AS GpsManuprintparam_Version,Gps_ManuPrintParam.iMEIStart AS GpsManuprintparam_IMEIStart,Gps_ManuPrintParam.iMEIEnd AS GpsManuprintparam_IMEIEnd,Gps_ManuPrintParam.iMEIRel AS GpsManuprintparam_IMEIRel FROM DataRelativeSheet JOIN Gps_ManuPrintParam  ON DataRelativeSheet.IMEI1 = Gps_ManuPrintParam.IMEI";
	
	
	/**
	 * 导出列表
	 * @param table
	 * @param ascBy
	 * @param descBy
	 * @param filter
	 * @param type
	 * @param response
	 * @throws Exception
	 */
	public void download(String table, String ascBy, String descBy, String filter, Integer type, HttpServletResponse response) throws Exception {
		SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd_HH:mm:ss");
		Page<Record> records = null;
		if (table.equals("Gps_ManuSimDataParam")) {
			records = selectGpsManuSimDataParam(1, PropKit.use("properties.ini").getInt("defaultPageSize"), ascBy, descBy, filter);
		}else if (table.equals("Gps_ManuPrintParam")) {
			records = selectGpsManuPrintParam(1, PropKit.use("properties.ini").getInt("defaultPageSize"), ascBy, descBy, filter);
		}else {
			records = daoService.select(table, null, null, ascBy, descBy, filter, type);
		}
		int size = records.getTotalRow();
		String fileName = table + "_" + simpleDateFormat.format(new Date()) + "_" + size + ".xls";
		response.reset();
		response.setHeader("Content-Disposition", "attachment; filename=" + new String((fileName).getBytes("utf-8"), "iso-8859-1"));
		response.setContentType("application/vnd.ms-excel");
		OutputStream output = response.getOutputStream();
		ExcelHelper helper = ExcelHelper.create(false);
		String[] field = null;
		String[] head = null;
		if (table.equals("Gps_AutoTest_Result") || table.equals("Gps_AutoTest_Result2") || table.equals("Gps_AutoTest_Result3") || table.equals("Gps_CoupleTest_Result")) {
			field = new String[] { "Id", "SN", "IMEI", "ZhiDan", "SoftModel", "Version", "Result", "TesterId", "Computer", "TestSetting", "TestTime", "Remark"};
			head = new String[] { "Id", "SN", "IMEI", "ZhiDan", "SoftModel", "Version", "Result", "TesterId", "Computer", "TestSetting", "TestTime", "Remark"};
		}else if(table.equals("Gps_CartonBoxTwenty_Result")) {
			field = new String[] { "Id", "BoxNo", "IMEI", "ZhiDan", "SoftModel", "Version", "ProductCode", "Color", "Qty", "Weight", "Date", "TACInfo", "CompanyName", "TesterId", "TestTime", "Remark1", "Remark2", "Remark3", "Remark4", "Remark5", "Computer"};
			head = new String[] { "Id", "BoxNo", "IMEI", "ZhiDan", "SoftModel", "Version", "ProductCode", "Color", "Qty", "Weight", "Date", "TACInfo", "CompanyName", "TesterId", "TestTime", "Remark1", "Remark2", "Remark3", "Remark4", "Remark5", "Computer"};
		}else if(table.equals("Gps_ManuLdParam")) {
			field = new String[] { "ID", "LDTime", "LDIP", "ZhiDan", "IMEI", "SN", "ICCID", "SIM", "VIP", "BAT", "EquipmentNumber", "MAC", "SoftModel", "Version", "Remark", "LDRESULT", "ReLdImeiNum", "ReLdImeiTime"};
			head = new String[] { "ID", "LDTime", "LDIP", "ZhiDan", "IMEI", "SN", "ICCID", "SIM", "VIP", "BAT", "EquipmentNumber", "MAC", "SoftModel", "Version", "Remark", "LDRESULT", "ReLdImeiNum", "ReLdImeiTime"};
		}else if(table.equals("Gps_ManuPrintParam")) {			
			field = new String[] { "ID", "ZhiDan", "IMEI", "IMEIStart", "IMEIEnd", "SN", "IMEIRel", "SIM", "VIP", "BAT", "SoftModel", "Version", "Remark", "JS_PrintTime", "JS_TemplatePath", "JS_RePrintNum", "JS_ReFirstPrintTime", "JS_ReEndPrintTime", "UserName", "CH_PrintTime", "CH_TemplatePath1", "CH_TemplatePath2", "CH_RePrintNum", "CH_ReFirstPrintTime", "CH_ReEndPrintTime", "ICCID", "MAC", "Equipment"};
			head = new String[] { "ID", "ZhiDan", "IMEI", "IMEIStart", "IMEIEnd", "SN", "IMEIRel", "SIM", "VIP", "BAT", "SoftModel", "Version", "Remark", "JS_PrintTime", "JS_TemplatePath", "JS_RePrintNum", "JS_ReFirstPrintTime", "JS_ReEndPrintTime", "UserName", "CH_PrintTime", "CH_TemplatePath1", "CH_TemplatePath2", "CH_RePrintNum", "CH_ReFirstPrintTime", "CH_ReEndPrintTime", "ICCID", "MAC", "Equipment"};
		}else if(table.equals("Gps_TestResult")) {
			field = new String[] { "Id", "SN", "IMEI", "SoftModel", "Version", "GPSResult", "CoupleResult", "WriteImeiResult", "ParamDownloadResult", "AutoTestResult", "Result", "AutoTestSMTResult", "ZhiDan", "RecordTime", "CPResult"};
			head = new String[] { "Id", "SN", "IMEI", "SoftModel", "Version", "GPSResult", "CoupleResult", "WriteImeiResult", "ParamDownloadResult", "AutoTestResult", "Result", "AutoTestSMTResult", "ZhiDan", "RecordTime", "CPResult"};
		}else if(table.equals("DataRelativeSheet")) {
			field = new String[] { "SN", "IMEI1", "IMEI2", "IMEI3", "IMEI4", "IMEI5", "IMEI6", "IMEI7", "IMEI8", "IMEI9", "IMEI10", "IMEI11", "IMEI12", "ZhiDan", "TestTime", "SimEffectiveDate"};
			head = new String[] { "SN", "IMEI1", "IMEI2", "IMEI3", "IMEI4", "IMEI5", "IMEI6", "IMEI7", "IMEI8", "IMEI9", "IMEI10", "IMEI11", "IMEI12", "ZhiDan", "TestTime", "SimEffectiveDate"};
		}else if(table.equals("GpsSMT_TcData") || table.equals("GpsTcData")) {
			field = new String[] { "Id", "SN", "FixMode", "gpsdb0", "gpsdb1", "gpsdb2", "gpsdb3", "gpsdb4", "gpsdb5", "gpsdb6", "gpsdb7", "gpsdb8", "gpsdb9", "gpsdb10", "gpsdb11"};
			head = new String[] { "Id", "SN", "FixMode", "gpsdb0", "gpsdb1", "gpsdb2", "gpsdb3", "gpsdb4", "gpsdb5", "gpsdb6", "gpsdb7", "gpsdb8", "gpsdb9", "gpsdb10", "gpsdb11"};
		}else if(table.equals("Gps_OperRecord")) {
			field = new String[] { "id", "OperName", "OperContent", "OperTime", "OperDemo"};
			head = new String[] { "id", "OperName", "OperContent", "OperTime", "OperDemo"};
		}else if (table.equals("DataRelativeUnique")) {
			field = new String[] {"DATA1", "DATA2", "DATA3", "DATA4", "DATA5", "DATA6", "DATA7", "DATA8", "DATA9", "DATA10", "DATA11", "DATA12", "ZhiDan", "TestTime"};
			head = new String[] {"DATA1", "DATA2", "DATA3", "DATA4", "DATA5", "DATA6", "DATA7", "DATA8", "DATA9", "DATA10", "DATA11", "DATA12", "ZhiDan", "TestTime"};
		}else if (table.equals("Gps_ManuCpParam")) {
			field = new String[] {"ID", "CPIP", "ZhiDan", "IMEI1", "IMEI2", "IMEI3", "SECTIONNO1", "SECTIONNO2", "CPRESULT", "CPTIME", "CPTYPE", "CPFalseCount", "RECPTIME", "CPERROR"};
			head = new String[] {"ID", "CPIP", "ZhiDan", "IMEI1", "IMEI2", "IMEI3", "SECTIONNO1", "SECTIONNO2", "CPRESULT", "CPTIME", "CPTYPE", "CPFalseCount", "RECPTIME", "CPERROR"};
		}else if (table.equals("NetMarkIMEI")) {
			field = new String[] {"Id", "NetMark", "IMEI", "PrintCount", "SN"};
			head = new String[] {"Id", "NetMark", "IMEI", "PrintCount", "SN"};
		}else if (table.equals("Gps_ManuSimDataParam")) {
			field = new String[] {"ID", "SDIP", "RID", "IMEI", "CID", "ICCID", "SDOperator", "SDTime", "SDRESULT", "ReSDTime", "ReSDCount"};
			head = new String[] {"ID", "SDIP", "RID", "IMEI", "CID", "ICCID", "SDOperator", "SDTime", "SDRESULT", "ReSDTime", "ReSDCount"};
		}
		helper.fill(records.getList(), fileName, field, head);
		helper.write(output, true);
	}
	
	
	/**
	 * 备份删除记录
	 * @param table
	 * @param ascBy
	 * @param descBy
	 * @param filter
	 * @param type
	 * @throws Exception
	 */
	public void backup(String table, String ascBy, String descBy, String filter, Integer type) throws Exception {
		SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyyMMddHHmmss");
		Page<Record> records = null;
		//判断表格名，选取恰当的查询方法
		if (table.equals("Gps_ManuSimDataParam")) {
			records = selectGpsManuSimDataParam(1, PropKit.use("properties.ini").getInt("defaultPageSize"), ascBy, descBy, filter);
		}else if (table.equals("Gps_ManuPrintParam")) {
			records = selectGpsManuPrintParam(1, PropKit.use("properties.ini").getInt("defaultPageSize"), ascBy, descBy, filter);
		}else {
			records = daoService.select(table, null, null, ascBy, descBy, filter, type);
		}
		int size = records.getTotalRow();
		Date time = new Date();
		String fileName = table + "_" + simpleDateFormat.format(time) + "_" + size + ".xls";
		System.err.println(fileName);
		ExcelHelper helper = ExcelHelper.create(false);
		File dir = new File(getFilePath());
		if (!dir.exists() || !dir.isDirectory()) {
			dir.mkdirs();
		}
		File file = new File(getFilePath() + fileName);
		if (file.exists()) {
			file.delete();
		}
		file.createNewFile();
		OutputStream output = new FileOutputStream(file);
		String[] field = null;
		String[] head = null;
		if (table.equals("Gps_AutoTest_Result") || table.equals("Gps_AutoTest_Result2") || table.equals("Gps_AutoTest_Result3") || table.equals("Gps_CoupleTest_Result")) {
			field = new String[] { "Id", "SN", "IMEI", "ZhiDan", "SoftModel", "Version", "Result", "TesterId", "Computer", "TestSetting", "TestTime", "Remark"};
			head = new String[] { "Id", "SN", "IMEI", "ZhiDan", "SoftModel", "Version", "Result", "TesterId", "Computer", "TestSetting", "TestTime", "Remark"};
		}else if(table.equals("Gps_CartonBoxTwenty_Result")) {
			field = new String[] { "Id", "BoxNo", "IMEI", "ZhiDan", "SoftModel", "Version", "ProductCode", "Color", "Qty", "Weight", "Date", "TACInfo", "CompanyName", "TesterId", "TestTime", "Remark1", "Remark2", "Remark3", "Remark4", "Remark5", "Computer"};
			head = new String[] { "Id", "BoxNo", "IMEI", "ZhiDan", "SoftModel", "Version", "ProductCode", "Color", "Qty", "Weight", "Date", "TACInfo", "CompanyName", "TesterId", "TestTime", "Remark1", "Remark2", "Remark3", "Remark4", "Remark5", "Computer"};
		}else if(table.equals("Gps_ManuLdParam")) {
			field = new String[] { "ID", "LDTime", "LDIP", "ZhiDan", "IMEI", "SN", "ICCID", "SIM", "VIP", "BAT", "EquipmentNumber", "MAC", "SoftModel", "Version", "Remark", "LDRESULT", "ReLdImeiNum", "ReLdImeiTime"};
			head = new String[] { "ID", "LDTime", "LDIP", "ZhiDan", "IMEI", "SN", "ICCID", "SIM", "VIP", "BAT", "EquipmentNumber", "MAC", "SoftModel", "Version", "Remark", "LDRESULT", "ReLdImeiNum", "ReLdImeiTime"};
		}else if(table.equals("Gps_ManuPrintParam")) {			
			field = new String[] { "ID", "ZhiDan", "IMEI", "IMEIStart", "IMEIEnd", "SN", "IMEIRel", "SIM", "VIP", "BAT", "SoftModel", "Version", "Remark", "JS_PrintTime", "JS_TemplatePath", "JS_RePrintNum", "JS_ReFirstPrintTime", "JS_ReEndPrintTime", "UserName", "CH_PrintTime", "CH_TemplatePath1", "CH_TemplatePath2", "CH_RePrintNum", "CH_ReFirstPrintTime", "CH_ReEndPrintTime", "ICCID", "MAC", "Equipment"};
			head = new String[] { "ID", "ZhiDan", "IMEI", "IMEIStart", "IMEIEnd", "SN", "IMEIRel", "SIM", "VIP", "BAT", "SoftModel", "Version", "Remark", "JS_PrintTime", "JS_TemplatePath", "JS_RePrintNum", "JS_ReFirstPrintTime", "JS_ReEndPrintTime", "UserName", "CH_PrintTime", "CH_TemplatePath1", "CH_TemplatePath2", "CH_RePrintNum", "CH_ReFirstPrintTime", "CH_ReEndPrintTime", "ICCID", "MAC", "Equipment"};
		}else if(table.equals("Gps_TestResult")) {
			field = new String[] { "Id", "SN", "IMEI", "SoftModel", "Version", "GPSResult", "CoupleResult", "WriteImeiResult", "ParamDownloadResult", "AutoTestResult", "Result", "AutoTestSMTResult", "ZhiDan", "RecordTime", "CPResult"};
			head = new String[] { "Id", "SN", "IMEI", "SoftModel", "Version", "GPSResult", "CoupleResult", "WriteImeiResult", "ParamDownloadResult", "AutoTestResult", "Result", "AutoTestSMTResult", "ZhiDan", "RecordTime", "CPResult"};
		}else if(table.equals("DataRelativeSheet")) {
			field = new String[] { "SN", "IMEI1", "IMEI2", "IMEI3", "IMEI4", "IMEI5", "IMEI6", "IMEI7", "IMEI8", "IMEI9", "IMEI10", "IMEI11", "IMEI12", "ZhiDan", "TestTime", "SimEffectiveDate"};
			head = new String[] { "SN", "IMEI1", "IMEI2", "IMEI3", "IMEI4", "IMEI5", "IMEI6", "IMEI7", "IMEI8", "IMEI9", "IMEI10", "IMEI11", "IMEI12", "ZhiDan", "TestTime", "SimEffectiveDate"};
		}else if(table.equals("GpsSMT_TcData") || table.equals("GpsTcData")) {
			field = new String[] { "Id", "SN", "FixMode", "gpsdb0", "gpsdb1", "gpsdb2", "gpsdb3", "gpsdb4", "gpsdb5", "gpsdb6", "gpsdb7", "gpsdb8", "gpsdb9", "gpsdb10", "gpsdb11"};
			head = new String[] { "Id", "SN", "FixMode", "gpsdb0", "gpsdb1", "gpsdb2", "gpsdb3", "gpsdb4", "gpsdb5", "gpsdb6", "gpsdb7", "gpsdb8", "gpsdb9", "gpsdb10", "gpsdb11"};
		}else if(table.equals("Gps_OperRecord")) {
			field = new String[] { "id", "OperName", "OperContent", "OperTime", "OperDemo"};
			head = new String[] { "id", "OperName", "OperContent", "OperTime", "OperDemo"};
		}else if (table.equals("DataRelativeUnique")) {
			field = new String[] {"DATA1", "DATA2", "DATA3", "DATA4", "DATA5", "DATA6", "DATA7", "DATA8", "DATA9", "DATA10", "DATA11", "DATA12", "ZhiDan", "TestTime"};
			head = new String[] {"DATA1", "DATA2", "DATA3", "DATA4", "DATA5", "DATA6", "DATA7", "DATA8", "DATA9", "DATA10", "DATA11", "DATA12", "ZhiDan", "TestTime"};
		}else if (table.equals("Gps_ManuCpParam")) {
			field = new String[] {"ID", "CPIP", "ZhiDan", "IMEI1", "IMEI2", "IMEI3", "SECTIONNO1", "SECTIONNO2", "CPRESULT", "CPTIME", "CPTYPE", "CPFalseCount", "RECPTIME", "CPERROR"};
			head = new String[] {"ID", "CPIP", "ZhiDan", "IMEI1", "IMEI2", "IMEI3", "SECTIONNO1", "SECTIONNO2", "CPRESULT", "CPTIME", "CPTYPE", "CPFalseCount", "RECPTIME", "CPERROR"};
		}else if (table.equals("NetMarkIMEI")) {
			field = new String[] {"Id", "NetMark", "IMEI", "PrintCount", "SN"};
			head = new String[] {"Id", "NetMark", "IMEI", "PrintCount", "SN"};
		}else if (table.equals("Gps_ManuSimDataParam")) {
			field = new String[] {"ID", "SDIP", "RID", "IMEI", "CID", "ICCID", "SDOperator", "SDTime", "SDRESULT", "ReSDTime", "ReSDCount"};
			head = new String[] {"ID", "SDIP", "RID", "IMEI", "CID", "ICCID", "SDOperator", "SDTime", "SDRESULT", "ReSDTime", "ReSDCount"};
		}
		helper.fill(records.getList(), fileName, field, head);
		helper.write(output, true);
		deleteHistoryService.add(fileName, file.getAbsolutePath(), time);
	}
	
	
	/**
	 * 更新DataRelativeSheet
	 * @param dataRelativeSheet
	 * @return
	 */
	public boolean updateRelativeSheet(DataRelativeSheet dataRelativeSheet) {
		if(dataRelativeSheet.getIMEI1() == null) {
			throw new ParameterException("only update the dataRelativeSheet have the IMEI1");
		}
		DataRelativeSheet sheet = DataRelativeSheet.dao.findById(dataRelativeSheet.getIMEI1());
		if (sheet == null) {
			dataRelativeSheet.setTestTime(new Date());
			return dataRelativeSheet.save();
		}
		return dataRelativeSheet.update();
	}
	
	
	/**
	 * 根据条件查询DataRelativeSheet
	 * @param pageNo
	 * @param pageSize
	 * @param ascBy
	 * @param descBy
	 * @param filter
	 * @return
	 */
	public Page<Record> selectDataRelativeSheet(Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter, Boolean isReferred){
		if (isReferred) {
			return daoService.select(SELECT_DATARELATIVESHEET_AND_PRINTPARAM, pageNo, pageSize, ascBy, descBy, filter);
		}
		return daoService.select("DataRelativeSheet", pageNo, pageSize, ascBy, descBy, filter, null);
	}
	
	
	/**
	 * 根据条件查询Gps_ManuPrintParam
	 * @param pageNo
	 * @param pageSize
	 * @param ascBy
	 * @param descBy
	 * @param filter
	 * @return
	 */
	public Page<Record> selectGpsManuPrintParam(Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter){
		SqlPara sqlPara = new SqlPara();
		StringBuffer sql = new StringBuffer();
		sql.append("SELECT * FROM Gps_ManuPrintParam");
		if (!filter.equals("")) {
			sql.append(" where " + filter);
		}
		createOrderBy(ascBy, descBy, sql);
		sqlPara.setSql(sql.toString());
		System.err.println(sql.toString());
		return Db.paginate(pageNo, pageSize, sqlPara);
	}
	
	
	/**
	 * 根据条件查询Gps_ManuSimDataParam
	 * @param pageNo
	 * @param pageSize
	 * @param ascBy
	 * @param descBy
	 * @param filter
	 * @return
	 */
	public Page<Record> selectGpsManuSimDataParam(Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter){
		SqlPara sqlPara = new SqlPara();
		StringBuffer sql = new StringBuffer();
		sql.append("SELECT * FROM Gps_ManuSimDataParam");
		if (!filter.equals("")) {
			sql.append(" where " + filter);
		}
		createOrderBy(ascBy, descBy, sql);
		sqlPara.setSql(sql.toString());
		System.err.println(sql.toString());
		return Db.paginate(pageNo, pageSize, sqlPara);
	}
	
	
	/**
	 * 根据条件删除并备份记录
	 * @param table
	 * @param filter
	 * @param type
	 */
	public void delete(String table, String filter, Integer type) {
		try {
			backup(table, null, null, filter, type);
		} catch (Exception e) {
			e.printStackTrace();
			throw new OperationException("备份数据失败，删除失败");
		}
		deleteService.delete(table, filter, type);
	}
	
	
	/**
	 * 根据条件删除并备份表Gps_ManuPrintParam记录
	 * @param filter
	 */
	public void deleteGpsManuPrintParam(String filter){
		String table = "Gps_ManuPrintParam";
		try {
			backup(table, null, null, filter, null);
		} catch (Exception e) {
			throw new OperationException("备份数据失败，删除失败");
		}
		String sql = null;
		if (!filter.equals("")) {
			sql = "delete from " + table + " where " + filter;
		}else {
			throw new OperationException("请填写删除条件");
		}
		deleteService.delete(sql);
	}
	
	
	/**
	 * 根据条件删除并备份表Gps_ManuSimDataParam记录
	 * @param filter
	 */
	public void deleteGpsManuSimDataParam(String filter){
		String table = "Gps_ManuSimDataParam";
		try {
			backup(table, null, null, filter, null);
		} catch (Exception e) {
			throw new OperationException("备份数据失败，删除失败");
		}
		String sql = null;
		if (!filter.equals("")) {
			sql = "delete from " + table + " where " + filter;
		}else {
			throw new OperationException("请填写删除条件");
		}
		deleteService.delete(sql);
	}
	
	
	public void deleteByIds(String table, String filter, Integer type) {
		if (filter == null || filter.trim().equals("")) {
			throw new OperationException("请选择删除的记录");
		}
		String tempFilter = filter;
		if (table.equals("Gps_ManuSimDataParam") || table.equals("Gps_ManuPrintParam")) {
			tempFilter = filter.replace("#in#", " in ");
		}
		try {
			backup(table, null, null, tempFilter, type);
		} catch (Exception e) {
			throw new OperationException("备份数据失败，删除失败");
		}
		deleteService.delete(table, filter, type);
	}
	
	
	private void createOrderBy(String ascBy, String descBy, StringBuffer sql) {
		if(ascBy != null && descBy != null ) {
			throw new ParameterException("ascBy and descBy can not be provided at the same time");
		}else if(ascBy != null) {
			sql.append(" ORDER BY " + ascBy + " ASC ");
		}else if(descBy != null){
			sql.append(" ORDER BY " + descBy + " DESC ");
		}
	}
	
	
	/**
	 * 获取备份文件存储路径
	 * @return
	 */
	private String getFilePath() {
		return PathKit.getWebRootPath() + File.separator + "backup" + File.separator;
	}
}
