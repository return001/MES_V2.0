package com.jimi.mes_server.service;

import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang3.StringUtils;

import com.jfinal.aop.Enhancer;
import com.jfinal.kit.PathKit;
import com.jfinal.kit.PropKit;
import com.jfinal.kit.StrKit;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Page;
import com.jfinal.plugin.activerecord.Record;
import com.jfinal.plugin.activerecord.SqlPara;
import com.jimi.mes_server.entity.MultiTableQueryInfo;
import com.jimi.mes_server.entity.SQL;
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

	private static final Integer IMEI_LENGTH = 14;


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
	public void download(String table, String ascBy, String descBy, String filter, Integer type, HttpServletResponse response,OutputStream output) throws Exception {
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
			throw new ParameterException("IMEI1不能为空");
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


	/**@author HCJ
	 * 根据IMEI1清空关联表相关字段
	 * @param imeis IMEI1集合
	 * @param items 需要进行操作的字段
	 * @date 2019年5月29日 下午3:39:32
	 */
	public boolean cleanupInRel(String imeis, String items) {
		StringBuffer sql = new StringBuffer();
		sql.append("UPDATE DataRelativeSheet SET ");
		for (String item : items.split(",")) {
			if (!item.equals("")) {
				sql.append(item.toUpperCase() + " = NULL , ");
			}
		}
		sql.deleteCharAt(sql.lastIndexOf(","));
		sql.append("WHERE IMEI1 IN (");
		for (String imei : imeis.split(",")) {
			sql.append(" '" + imei + "' , ");
		}
		sql.deleteCharAt(sql.lastIndexOf(","));
		sql.append(" )");
		try {
			Db.update(sql.toString());
			return true;
		} catch (Exception e) {
			e.printStackTrace();
			throw new OperationException("更新数据库失败");
		}
	}


	/**@author HCJ
	 * 根据订单名称获取所有相似的订单信息
	 * @param zhiDan 订单名称
	 * @date 2019年6月5日 下午3:53:14
	 */
	public Page<Record> selectZhiDanInfo(String zhiDan) {
		SqlPara sqlPara = new SqlPara();
		sqlPara.setSql(SQL.SELECT_ZHIDAN_IMEI_INFO_BY_ZHIDAN);
		sqlPara.addPara("%" + zhiDan + "%");
		Page<Record> page = Db.paginate(1, PropKit.use("properties.ini").getInt("defaultPageSize"), sqlPara);
		if (page.getList() == null || page.getList().isEmpty()) {
			throw new OperationException("不存在符合条件的记录");
		}
		return page;
	}


	/**@author HCJ
	 * 查询未使用的IMEI
	 * @param startIMEI 开始IMEI
	 * @param endIMEI 结束IMEI
	 * @param zhiDan 订单名称
	 * @date 2019年6月5日 下午3:53:59
	 */
	public List<String> selectUnusedIMEI(String startIMEI, String endIMEI, String zhiDan) {
		List<String> unusedIMEIs = new ArrayList<>();
		long start;
		long end;
		try {
			start = Long.parseLong(startIMEI);
			end = Long.parseLong(endIMEI);
		} catch (Exception e) {
			throw new ParameterException("起止IMEI格式错误");
		}
		if (start <= 0 || end <= 0 || start > end || start == end) {
			throw new ParameterException("IMEI必须大于0并且结束IMEI必须大于开始IMEI");
		}
		Record imeiRecord = Db.findFirst(SQL.SELECT_IMEI_BEGIN_END_BY_ZHIDAN, zhiDan + "%");
		if (imeiRecord == null) {
			throw new OperationException("不存在符合条件的记录");
		}
		String realStartIMEI = imeiRecord.getStr("IMEIStart");
		String realEndIMEI = imeiRecord.getStr("IMEIEnd");
		if (startIMEI.length() != realStartIMEI.length() || endIMEI.length() != realEndIMEI.length() || start < Long.parseLong(realStartIMEI) || end > Long.parseLong(realEndIMEI)) {
			throw new OperationException("起止IMEI的长度和范围必须在数据表设定的范围内");
		}
		List<Record> records = null;
		if (realStartIMEI.length() == IMEI_LENGTH) {
			records = Db.find(SQL.SELECT_SHORT_IMEI_BY_ZHIDAN, zhiDan + "%");
		} else {
			records = Db.find(SQL.SELECT_LONG_IMEI_BY_ZHIDAN, zhiDan + "%");
		}
		List<Long> imeis = new ArrayList<>();
		for (Record record : records) {
			try {
				imeis.add(Long.parseLong(record.getStr("realIMEI")));
			} catch (Exception e) {
				e.printStackTrace();
				throw new OperationException("数据表中存在格式错误的IMEI号");
			}
		}
		for (long i = start; i <= end; i++) {
			if (!imeis.contains(i)) {
				unusedIMEIs.add(i + "");
			}
		}
		if (realStartIMEI.length() == IMEI_LENGTH) {
			List<String> realUnusedIMEIs = new ArrayList<>();
			for (String unusedIMEI : unusedIMEIs) {
				realUnusedIMEIs.add(genIMEICode(unusedIMEI));
			}
			return realUnusedIMEIs;
		} else {
			return unusedIMEIs;
		}
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
	public void downloadUnusedIMEI(String startIMEI, String endIMEI, String zhiDan, HttpServletResponse response, OutputStream output) throws Exception {
		List<String> unusedIMEIs = selectUnusedIMEI(startIMEI, endIMEI, zhiDan);
		if (unusedIMEIs == null || unusedIMEIs.isEmpty()) {
			throw new OperationException("不存在符合条件的记录");
		}
		SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd_HH:mm:ss");
		String fileName = "Gps_ManuPrintParam" + "_" + simpleDateFormat.format(new Date()) + "_" + zhiDan + "_" + "未使用的IMEI" + ".xls";
		response.reset();
		response.setHeader("Content-Disposition", "attachment; filename=" + new String((fileName).getBytes("utf-8"), "iso-8859-1"));
		response.setContentType("application/vnd.ms-excel");
		ExcelHelper helper = ExcelHelper.create(false);
		String head = "未使用的IMEI";
		String title = "";
		helper.fillStringList(unusedIMEIs, title, head);
		helper.write(output, true);
	}


	/**@author HCJ
	 * 根据条件进行多表查询
	 * @param imei IMEI号
	 * @param sn SN号
	 * @param zhiDan ZhiDan号
	 * @date 2019年6月6日 上午11:30:51
	 */
	public MultiTableQueryInfo multiTableQuery(String imei, String sn, String zhiDan) {
		StringBuilder imeiSnZhiDan = concatSqlParameter(imei, sn, zhiDan).get("imeiSnZhiDan");
		StringBuilder zhiDanDATA1 = concatSqlParameter(imei, sn, zhiDan).get("zhiDanDATA1");
		StringBuilder zhiDanIMEI1 = concatSqlParameter(imei, sn, zhiDan).get("zhiDanIMEI1");
		StringBuilder zhiDanIMEI = concatSqlParameter(imei, sn, zhiDan).get("zhiDanIMEI");
		StringBuilder zhidanIMEI = concatSqlParameter(imei, sn, zhiDan).get("zhidanIMEI");
		StringBuilder ridIMEI = concatSqlParameter(imei, sn, zhiDan).get("ridIMEI");

		List<Record> dataRelativeSheets = null;
		List<Record> dataRelativeUniques = null;
		List<Record> gpsAutotestResults = null;
		List<Record> gpsAutotestResult3s = null;
		List<Record> gpsCartonboxtwentyResults = null;
		List<Record> gpsCoupletestResults = null;
		List<Record> gpsManucpparams = null;
		List<Record> gpsManuprintparams = null;
		List<Record> gpsTestresults = null;
		List<Record> netMarkIMEIs = null;
		List<Record> gpsManusimdataparams = null;

		if (!zhiDanDATA1.toString().isEmpty()) {
			dataRelativeUniques = Db.find(SQL.SELECT_DATARELATIVEUNIQUE_SQL_FRAGMENT + zhiDanDATA1);
		}
		if (!zhiDanIMEI1.toString().isEmpty()) {
			dataRelativeSheets = Db.find(SQL.SELECT_DATARELATIVESHEET_SQL_FRAGMENT + zhiDanIMEI1);
			gpsManucpparams = Db.find(SQL.SELECT_MANUCP_RESULT_SQL_FRAGMENT + zhiDanIMEI1);
		}
		if (!imeiSnZhiDan.toString().isEmpty()) {
			gpsAutotestResults = Db.find(SQL.SELECT_AUTOTEST_RESULT_SQL_FRAGMENT + imeiSnZhiDan);
			gpsAutotestResult3s = Db.find(SQL.SELECT_AUTOTEST_RESULT3_SQL_FRAGMENT + imeiSnZhiDan);
			gpsCoupletestResults = Db.find(SQL.SELECT_COUPLE_RESULT_SQL_FRAGMENT + imeiSnZhiDan);
			gpsTestresults = Db.find(SQL.SELECT_TEST_RESULT_SQL_FRAGMENT + imeiSnZhiDan);
		}
		if (!zhiDanIMEI.toString().isEmpty()) {
			gpsCartonboxtwentyResults = Db.find(SQL.SELECT_CARTONBOX_RESULT_SQL_FRAGMENT + zhiDanIMEI);
			gpsManuprintparams = Db.find(SQL.SELECT_MANUPRINT_RESULT_SQL_FRAGMENT + zhiDanIMEI);
		}
		if (!zhidanIMEI.toString().isEmpty()) {
			netMarkIMEIs = Db.use("db1").find(SQL.SELECT_NETMARKIMEI_RESULT_SQL_FRAGMENT + zhidanIMEI);
		}
		if (!ridIMEI.toString().isEmpty()) {
			gpsManusimdataparams = Db.find(SQL.SELECT_MANUSIM_RESULT_SQL_FRAGMENT + ridIMEI);
		}
		return new MultiTableQueryInfo(dataRelativeSheets, dataRelativeUniques, gpsAutotestResults, gpsAutotestResult3s, gpsCartonboxtwentyResults, gpsCoupletestResults, gpsManucpparams, gpsManuprintparams, gpsTestresults, netMarkIMEIs, gpsManusimdataparams);
	}


	public void multiTableDelete(String imei, String sn, String zhiDan) {
		try {
			multiTableBackup(imei, sn, zhiDan);
		} catch (Exception e) {
			e.printStackTrace();
			throw new OperationException("备份数据失败，删除失败");
		}
		batchDelete(imei, sn, zhiDan);
	}


	public static void main(String[] args) {
		
	}


	private void createOrderBy(String ascBy, String descBy, StringBuffer sql) {
		if (ascBy != null && descBy != null) {
			throw new ParameterException("ascBy and descBy can not be provided at the same time");
		} else if (ascBy != null) {
			sql.append(" ORDER BY " + ascBy + " ASC ");
		} else if (descBy != null) {
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


	/**
	 * 根据IMEI校验算法将14位IMEI生成15位IMEI 
	 * (1).将偶数位数字分别乘以2，分别计算个位数和十位数之和
	 * (2).将奇数位数字相加，再加上上一步算得的值 
	 * (3).如果得出的数个位是0则校验位为0，否则为10减去个位数
	 * @param code 14位的IMEI
	 * @date 2019年6月5日 下午3:12:36
	 */
	private String genIMEICode(String code) {
		int total = 0, oddSum = 0, evenSum = 0;
		int temp = 0;
		char[] codeChars = code.toCharArray();
		for (int i = 0; i < codeChars.length; i++) {
			// 字符转数字用于计算
			int num = codeChars[i] - '0';
			// 将奇数位数字相加(从1开始计数)
			if (i % 2 == 0) {
				oddSum = oddSum + num;
			} else {
				// 将偶数位数字分别乘以2，分别计算个位数和十位数之和(从1开始计数)
				temp = num * 2;
				if (temp < 10) {
					evenSum = evenSum + temp;
				} else {
					evenSum = evenSum + temp + 1 - 10;
				}
			}
		}
		String validateBit = "";
		total = oddSum + evenSum;
		// 如果得出的数个位是0，则校验位为0，否则为10减去个位数
		if (total % 10 == 0) {
			validateBit = "0";
		} else {
			validateBit = (10 - (total % 10)) + "";
		}
		return code + validateBit;
	}


	private Map<String, StringBuilder> concatSqlParameter(String imei, String sn, String zhiDan) {
		Map<String, StringBuilder> map = new HashMap<>();
		StringBuilder imeiSnZhiDanWithAnd = new StringBuilder();
		StringBuilder zhiDanDATA1WithAnd = new StringBuilder();
		StringBuilder zhiDanIMEI1WithAnd = new StringBuilder();
		StringBuilder zhiDanIMEIWithAnd = new StringBuilder();
		StringBuilder zhidanIMEIWithAnd = new StringBuilder();
		StringBuilder ridIMEIWithAnd = new StringBuilder();
		if (!StrKit.isBlank(imei)) {
			imeiSnZhiDanWithAnd.append("IMEI = '" + imei + "' and ");
			zhiDanDATA1WithAnd.append("DATA1 = '" + imei + "' and ");
			zhiDanIMEI1WithAnd.append("IMEI1 = '" + imei + "' and ");
			zhiDanIMEIWithAnd.append("IMEI = '" + imei + "' and ");
			zhidanIMEIWithAnd.append("IMEI = '" + imei + "' and ");
			ridIMEIWithAnd.append("IMEI = '" + imei + "' and ");
		}
		if (!StrKit.isBlank(sn)) {
			imeiSnZhiDanWithAnd.append("SN = '" + sn + "' and ");
			ridIMEIWithAnd.append("RID = '" + sn + "' and ");
		}
		if (!StrKit.isBlank(zhiDan)) {
			imeiSnZhiDanWithAnd.append("ZhiDan = '" + zhiDan + "'");
			zhiDanDATA1WithAnd.append("ZhiDan = '" + zhiDan + "'");
			zhiDanIMEI1WithAnd.append("ZhiDan = '" + zhiDan + "'");
			zhiDanIMEIWithAnd.append("ZhiDan = '" + zhiDan + "'");
			zhidanIMEIWithAnd.append("Zhidan = '" + zhiDan + "'");
		}
		map.put("imeiSnZhiDan", deleteAndCharacter(imeiSnZhiDanWithAnd));
		map.put("zhiDanDATA1", deleteAndCharacter(zhiDanDATA1WithAnd));
		map.put("zhiDanIMEI1", deleteAndCharacter(zhiDanIMEI1WithAnd));
		map.put("zhiDanIMEI", deleteAndCharacter(zhiDanIMEIWithAnd));
		map.put("zhidanIMEI", deleteAndCharacter(zhidanIMEIWithAnd));
		map.put("ridIMEI", deleteAndCharacter(ridIMEIWithAnd));
		return map;
	}


	/**@author HCJ
	 * 根据文本内容去除and字符串
	 * @date 2019年6月6日 上午11:30:05
	 */
	private StringBuilder deleteAndCharacter(StringBuilder content) {
		if (StringUtils.endsWith(content, "and ")) {
			content.delete(content.lastIndexOf("and"), content.length());
		}
		return content;
	}


	private void multiTableBackup(String imei, String sn, String zhiDan) throws Exception {
		MultiTableQueryInfo multiTableQueryInfo = multiTableQuery(imei, sn, zhiDan);
		SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyyMMddHHmmss");
		Date time = new Date();
		StringBuilder paramStringBuilder = new StringBuilder();
		if (!StrKit.isBlank(imei)) {
			paramStringBuilder.append("IMEI号-" + imei + "_");
		}
		if (!StrKit.isBlank(sn)) {
			paramStringBuilder.append("SN号-" + sn + "_");
		}
		if (!StrKit.isBlank(zhiDan)) {
			paramStringBuilder.append("订单号-" + zhiDan);
		}
		if (StringUtils.endsWith(paramStringBuilder, "-")) {
			paramStringBuilder.delete(paramStringBuilder.lastIndexOf("-"), paramStringBuilder.length());
		}
		String fileName = "批量删除数据备份" + "_" + simpleDateFormat.format(time) + "_" + paramStringBuilder + ".xls";
		System.err.println(fileName);
		ExcelHelper helper = ExcelHelper.create(false);
		File dir = new File(getFilePath());
		if (!dir.exists() || !dir.isDirectory()) {
			dir.mkdirs();
		}
		/* File file = new File(getFilePath() + fileName); */
		File file = new File("D:\\" + fileName);
		if (file.exists()) {
			file.delete();
		}
		file.createNewFile();
		OutputStream output = new FileOutputStream(file);
		String[] field = null;
		String[] head = null;

		if (multiTableQueryInfo.getDataRelativeSheets() != null && !multiTableQueryInfo.getDataRelativeSheets().isEmpty()) {
			helper.getBook().setSheetName(0, "DataRelativeSheet");
			helper.switchSheet(0);
			field = new String[] { "SN", "IMEI1", "IMEI2", "IMEI3", "IMEI4", "IMEI5", "IMEI6", "IMEI7", "IMEI8", "IMEI9", "IMEI10", "IMEI11", "IMEI12", "ZhiDan", "TestTime", "SimEffectiveDate" };
			head = new String[] { "SN", "IMEI1", "IMEI2", "IMEI3", "IMEI4", "IMEI5", "IMEI6", "IMEI7", "IMEI8", "IMEI9", "IMEI10", "IMEI11", "IMEI12", "ZhiDan", "TestTime", "SimEffectiveDate" };
			helper.fill(multiTableQueryInfo.getDataRelativeSheets(), "DataRelativeSheet", field, head);
		} else {
			helper.getBook().removeSheetAt(0);
		}

		if (multiTableQueryInfo.getDataRelativeUniques() != null && !multiTableQueryInfo.getDataRelativeUniques().isEmpty()) {
			helper.getBook().createSheet("DataRelativeUnique");
			helper.switchSheet("DataRelativeUnique");
			field = new String[] { "DATA1", "DATA2", "DATA3", "DATA4", "DATA5", "DATA6", "DATA7", "DATA8", "DATA9", "DATA10", "DATA11", "DATA12", "ZhiDan", "TestTime" };
			head = new String[] { "DATA1", "DATA2", "DATA3", "DATA4", "DATA5", "DATA6", "DATA7", "DATA8", "DATA9", "DATA10", "DATA11", "DATA12", "ZhiDan", "TestTime" };
			helper.fill(multiTableQueryInfo.getDataRelativeUniques(), "DataRelativeUnique", field, head);
		}

		if (multiTableQueryInfo.getGpsAutotestResults() != null && !multiTableQueryInfo.getGpsAutotestResults().isEmpty()) {
			helper.getBook().createSheet("Gps_AutoTest_Result");
			helper.switchSheet("Gps_AutoTest_Result");
			field = new String[] { "Id", "SN", "IMEI", "ZhiDan", "SoftModel", "Version", "Result", "TesterId", "Computer", "TestSetting", "TestTime", "Remark" };
			head = new String[] { "Id", "SN", "IMEI", "ZhiDan", "SoftModel", "Version", "Result", "TesterId", "Computer", "TestSetting", "TestTime", "Remark" };
			helper.fill(multiTableQueryInfo.getGpsAutotestResults(), "Gps_AutoTest_Result", field, head);
		}

		if (multiTableQueryInfo.getGpsAutotestResult3s() != null && !multiTableQueryInfo.getGpsAutotestResult3s().isEmpty()) {
			helper.getBook().createSheet("Gps_AutoTest_Result3");
			helper.switchSheet("Gps_AutoTest_Result3");
			field = new String[] { "Id", "SN", "IMEI", "ZhiDan", "SoftModel", "Version", "Result", "TesterId", "Computer", "TestSetting", "TestTime", "Remark" };
			head = new String[] { "Id", "SN", "IMEI", "ZhiDan", "SoftModel", "Version", "Result", "TesterId", "Computer", "TestSetting", "TestTime", "Remark" };
			helper.fill(multiTableQueryInfo.getGpsAutotestResult3s(), "Gps_AutoTest_Result3", field, head);
		}

		if (multiTableQueryInfo.getGpsCartonboxtwentyResults() != null && !multiTableQueryInfo.getGpsCartonboxtwentyResults().isEmpty()) {
			helper.getBook().createSheet("Gps_CartonBoxTwenty_Result");
			helper.switchSheet("Gps_CartonBoxTwenty_Result");
			field = new String[] { "Id", "BoxNo", "IMEI", "ZhiDan", "SoftModel", "Version", "ProductCode", "Color", "Qty", "Weight", "Date", "TACInfo", "CompanyName", "TesterId", "TestTime", "Remark1", "Remark2", "Remark3", "Remark4", "Remark5", "Computer" };
			head = new String[] { "Id", "BoxNo", "IMEI", "ZhiDan", "SoftModel", "Version", "ProductCode", "Color", "Qty", "Weight", "Date", "TACInfo", "CompanyName", "TesterId", "TestTime", "Remark1", "Remark2", "Remark3", "Remark4", "Remark5", "Computer" };
			helper.fill(multiTableQueryInfo.getGpsCartonboxtwentyResults(), "Gps_CartonBoxTwenty_Result", field, head);
		}

		if (multiTableQueryInfo.getGpsCoupletestResults() != null && !multiTableQueryInfo.getGpsCoupletestResults().isEmpty()) {
			helper.getBook().createSheet("Gps_CoupleTest_Result");
			helper.switchSheet("Gps_CoupleTest_Result");
			field = new String[] { "Id", "SN", "IMEI", "ZhiDan", "SoftModel", "Version", "Result", "TesterId", "Computer", "TestSetting", "TestTime", "Remark" };
			head = new String[] { "Id", "SN", "IMEI", "ZhiDan", "SoftModel", "Version", "Result", "TesterId", "Computer", "TestSetting", "TestTime", "Remark" };
			helper.fill(multiTableQueryInfo.getGpsCoupletestResults(), "Gps_CoupleTest_Result", field, head);
		}

		if (multiTableQueryInfo.getGpsManuprintparams() != null && !multiTableQueryInfo.getGpsManuprintparams().isEmpty()) {
			helper.getBook().createSheet("Gps_ManuPrintParam");
			helper.switchSheet("Gps_ManuPrintParam");
			field = new String[] { "ID", "ZhiDan", "IMEI", "IMEIStart", "IMEIEnd", "SN", "IMEIRel", "SIM", "VIP", "BAT", "SoftModel", "Version", "Remark", "JS_PrintTime", "JS_TemplatePath", "JS_RePrintNum", "JS_ReFirstPrintTime", "JS_ReEndPrintTime", "UserName", "CH_PrintTime", "CH_TemplatePath1", "CH_TemplatePath2", "CH_RePrintNum", "CH_ReFirstPrintTime", "CH_ReEndPrintTime", "ICCID", "MAC", "Equipment" };
			head = new String[] { "ID", "ZhiDan", "IMEI", "IMEIStart", "IMEIEnd", "SN", "IMEIRel", "SIM", "VIP", "BAT", "SoftModel", "Version", "Remark", "JS_PrintTime", "JS_TemplatePath", "JS_RePrintNum", "JS_ReFirstPrintTime", "JS_ReEndPrintTime", "UserName", "CH_PrintTime", "CH_TemplatePath1", "CH_TemplatePath2", "CH_RePrintNum", "CH_ReFirstPrintTime", "CH_ReEndPrintTime", "ICCID", "MAC", "Equipment" };
			helper.fill(multiTableQueryInfo.getGpsManuprintparams(), "Gps_ManuPrintParam", field, head);
		}

		if (multiTableQueryInfo.getGpsTestresults() != null && !multiTableQueryInfo.getGpsTestresults().isEmpty()) {
			helper.getBook().createSheet("Gps_TestResult");
			helper.switchSheet("Gps_TestResult");
			field = new String[] { "Id", "SN", "IMEI", "SoftModel", "Version", "GPSResult", "CoupleResult", "WriteImeiResult", "ParamDownloadResult", "AutoTestResult", "Result", "AutoTestSMTResult", "ZhiDan", "RecordTime", "CPResult" };
			head = new String[] { "Id", "SN", "IMEI", "SoftModel", "Version", "GPSResult", "CoupleResult", "WriteImeiResult", "ParamDownloadResult", "AutoTestResult", "Result", "AutoTestSMTResult", "ZhiDan", "RecordTime", "CPResult" };
			helper.fill(multiTableQueryInfo.getGpsTestresults(), "Gps_TestResult", field, head);
		}

		if (multiTableQueryInfo.getNetMarkIMEIs() != null && !multiTableQueryInfo.getNetMarkIMEIs().isEmpty()) {
			helper.getBook().createSheet("NetMarkIMEI");
			helper.switchSheet("NetMarkIMEI");
			field = new String[] { "Id", "NetMark", "IMEI", "PrintCount", "SN" };
			head = new String[] { "Id", "NetMark", "IMEI", "PrintCount", "SN" };
			helper.fill(multiTableQueryInfo.getNetMarkIMEIs(), "NetMarkIMEI", field, head);
		}

		if (multiTableQueryInfo.getGpsManusimdataparams() != null && !multiTableQueryInfo.getGpsManusimdataparams().isEmpty()) {
			helper.getBook().createSheet("Gps_ManuSimDataParam");
			helper.switchSheet("Gps_ManuSimDataParam");
			field = new String[] { "ID", "SDIP", "RID", "IMEI", "CID", "ICCID", "SDOperator", "SDTime", "SDRESULT", "ReSDTime", "ReSDCount" };
			head = new String[] { "ID", "SDIP", "RID", "IMEI", "CID", "ICCID", "SDOperator", "SDTime", "SDRESULT", "ReSDTime", "ReSDCount" };
			helper.fill(multiTableQueryInfo.getGpsManusimdataparams(), "Gps_ManuSimDataParam", field, head);
		}
		try {
			helper.write(output, true);
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (output != null) {
				output.close();
			}
		}
	}


	private void batchDelete(String imei, String sn, String zhiDan) {
		StringBuilder imeiSnZhiDan = concatSqlParameter(imei, sn, zhiDan).get("imeiSnZhiDan");
		StringBuilder zhiDanDATA1 = concatSqlParameter(imei, sn, zhiDan).get("zhiDanDATA1");
		StringBuilder zhiDanIMEI1 = concatSqlParameter(imei, sn, zhiDan).get("zhiDanIMEI1");
		StringBuilder zhiDanIMEI = concatSqlParameter(imei, sn, zhiDan).get("zhiDanIMEI");
		StringBuilder zhidanIMEI = concatSqlParameter(imei, sn, zhiDan).get("zhidanIMEI");
		StringBuilder ridIMEI = concatSqlParameter(imei, sn, zhiDan).get("ridIMEI");
		try {
			if (!zhiDanDATA1.toString().isEmpty()) {
				Db.update(SQL.DELETE_DATARELATIVEUNIQUE_SQL_FRAGMENT + zhiDanDATA1);
			}
			if (!zhiDanIMEI1.toString().isEmpty()) {
				Db.update(SQL.DELETE_DATARELATIVESHEET_SQL_FRAGMENT + zhiDanIMEI1);
			}
			if (!imeiSnZhiDan.toString().isEmpty()) {
				Db.update(SQL.DELETE_AUTOTEST_RESULT_SQL_FRAGMENT + imeiSnZhiDan);
				Db.update(SQL.DELETE_AUTOTEST_RESULT3_SQL_FRAGMENT + imeiSnZhiDan);
				Db.update(SQL.DELETE_COUPLE_RESULT_SQL_FRAGMENT + imeiSnZhiDan);
				Db.update(SQL.DELETE_TEST_RESULT_SQL_FRAGMENT + imeiSnZhiDan);
			}
			if (!zhiDanIMEI.toString().isEmpty()) {
				Db.update(SQL.DELETE_CARTONBOX_RESULT_SQL_FRAGMENT + zhiDanIMEI);
				Db.update(SQL.DELETE_MANUPRINT_RESULT_SQL_FRAGMENT + zhiDanIMEI);
			}
			if (!zhidanIMEI.toString().isEmpty()) {
				Db.use("db1").update(SQL.DELETE_NETMARKIMEI_RESULT_SQL_FRAGMENT + zhidanIMEI);
			}
			if (!ridIMEI.toString().isEmpty()) {
				Db.update(SQL.DELETE_MANUSIM_RESULT_SQL_FRAGMENT + ridIMEI);
			}
		} catch (Exception e) {
			e.printStackTrace();
			throw new OperationException("删除数据时出错");
		}
	}
}
