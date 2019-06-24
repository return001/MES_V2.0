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

	private static final String DELETE_TABLE_FLAG = "1";


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
		if (start <= 0 || end <= 0 || start >= end) {
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
		SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyyMMddHHmmss");
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
	 * @param type 参数类型
	 * @date 2019年6月10日 下午3:34:01
	 */
	public MultiTableQueryInfo multiTableQuery(String imei, String sn, String zhiDan, Integer type) {
		Map<String, StringBuilder> sqlParameterMap = concatSqlParameter(imei, sn, zhiDan, type);
		StringBuilder imeiSnZhiDan = sqlParameterMap.get("imeiSnZhiDan");
		StringBuilder zhiDanDATA1 = sqlParameterMap.get("zhiDanDATA1");
		StringBuilder zhiDanIMEI1 = sqlParameterMap.get("zhiDanIMEI1");
		StringBuilder zhiDanIMEI = sqlParameterMap.get("zhiDanIMEI");
		StringBuilder zhidanIMEI = sqlParameterMap.get("zhidanIMEI");
		StringBuilder ridIMEI = sqlParameterMap.get("ridIMEI");

		List<Record> dataRelativeSheets = new ArrayList<>();
		List<Record> dataRelativeUniques = new ArrayList<>();
		List<Record> gpsAutotestResults = new ArrayList<>();
		List<Record> gpsAutotestResult3s = new ArrayList<>();
		List<Record> gpsCartonboxtwentyResults = new ArrayList<>();
		List<Record> gpsCoupletestResults = new ArrayList<>();
		List<Record> gpsManucpparams = new ArrayList<>();
		List<Record> gpsManuprintparams = new ArrayList<>();
		List<Record> gpsTestresults = new ArrayList<>();
		List<Record> netMarkIMEIs = new ArrayList<>();
		List<Record> gpsManusimdataparams = new ArrayList<>();

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


	/**@author HCJ
	 * 根据条件进行多表删除
	 * @param imei IMEI号
	 * @param sn SN号
	 * @param zhiDan ZhiDan号
	 * @param type 参数类型
	 * @param deleteTable 需要进行删除的表格
	 * @date 2019年6月10日 下午3:34:01
	 */
	public void multiTableDelete(String imei, String sn, String zhiDan, Integer type, String deleteTable) {
		try {
			multiTableBackup(imei, sn, zhiDan, type, deleteTable);
		} catch (Exception e) {
			e.printStackTrace();
			throw new OperationException("备份数据失败，删除失败");
		}
		batchDelete(imei, sn, zhiDan, type, deleteTable);
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
	public Page<Record> selectGpsCartonBox(Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter, Boolean isReferred) {
		if (isReferred) {
			return daoService.select(SQL.SELECT_DATARELATIVESHEET_AND_CARTONBOX, pageNo, pageSize, ascBy, descBy, filter);
		}
		return daoService.select("Gps_CartonBoxTwenty_Result", pageNo, pageSize, ascBy, descBy, filter, null);
	}


	/**@author HCJ
	 * 下载卡通箱表和关联表相互关联的数据
	 * @param ascBy 以哪个字段增序
	 * @param descBy 以哪个字段降序
	 * @param filter 查询条件
	 * @param isReferred 是否与关联表相关联
	 * @date 2019年6月14日 上午8:52:47
	 */
	public void downloadGpsCartonBox(String ascBy, String descBy, String filter, Boolean isReferred, HttpServletResponse response, OutputStream output) throws Exception {
		List<Record> records = selectGpsCartonBox(1, PropKit.use("properties.ini").getInt("defaultPageSize"), ascBy, descBy, filter, isReferred).getList();
		if (records == null || records.isEmpty()) {
			throw new OperationException("当前条件下不存在可以导出的数据");
		}
		SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyyMMddHHmmss");
		String fileName = null;
		String[] field = null;
		String[] head = null;
		String title = null;
		fileName = "GpsCartonBoxTwentyResult_DataRelativeSheet_" + simpleDateFormat.format(new Date()) + "_关联表与卡通箱表相互关联的数据导出_" + records.size() + ".xls";
		field = new String[] { "GpsCartonBoxTwentyResult_Id", "GpsCartonBoxTwentyResult_BoxNo", "GpsCartonBoxTwentyResult_IMEI", "GpsCartonBoxTwentyResult_ZhiDan", "GpsCartonBoxTwentyResult_SoftModel", "GpsCartonBoxTwentyResult_Version", "GpsCartonBoxTwentyResult_ProductCode", "GpsCartonBoxTwentyResult_Color", "GpsCartonBoxTwentyResult_Qty", "GpsCartonBoxTwentyResult_Weight", "GpsCartonBoxTwentyResult_Date", "GpsCartonBoxTwentyResult_TACInfo", "GpsCartonBoxTwentyResult_CompanyName", "GpsCartonBoxTwentyResult_TesterId", "GpsCartonBoxTwentyResult_TestTime", "GpsCartonBoxTwentyResult_Remark1", "GpsCartonBoxTwentyResult_Remark2", "GpsCartonBoxTwentyResult_Remark3", "GpsCartonBoxTwentyResult_Remark4", "GpsCartonBoxTwentyResult_Remark5", "GpsCartonBoxTwentyResult_Computer", "DataRelativeSheet_SN", "DataRelativeSheet_IMEI1", "DataRelativeSheet_IMEI2", "DataRelativeSheet_IMEI3", "DataRelativeSheet_IMEI4", "DataRelativeSheet_IMEI5", "DataRelativeSheet_IMEI6", "DataRelativeSheet_IMEI7", "DataRelativeSheet_IMEI8", "DataRelativeSheet_IMEI9", "DataRelativeSheet_IMEI10", "DataRelativeSheet_IMEI11", "DataRelativeSheet_IMEI12", "DataRelativeSheet_IMEI13", "DataRelativeSheet_ZhiDan", "DataRelativeSheet_TestTime", "DataRelativeSheet_SimEffectiveDate" };
		head = field;
		title = "Gps_CartonBoxTwenty_Result————DataRelativeSheet";
		response.reset();
		response.setHeader("Content-Disposition", "attachment; filename=" + new String((fileName).getBytes("utf-8"), "iso-8859-1"));
		response.setContentType("application/vnd.ms-excel");
		ExcelHelper helper = ExcelHelper.create(false);
		helper.fill(records, title, field, head);
		helper.write(output, true);
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


	/**@author HCJ
	 * 根据参数拼接sql语句
	 * @param imei IMEI号
	 * @param sn SN号
	 * @param zhiDan ZhiDan号
	 * @param type 参数类型
	 * @date 2019年6月10日 下午3:34:01
	 */
	private Map<String, StringBuilder> concatSqlParameter(String imei, String sn, String zhiDan, Integer type) {
		Map<String, StringBuilder> map = new HashMap<>(6);
		StringBuilder imeiSnZhiDanWithAnd = new StringBuilder();
		StringBuilder zhiDanDATA1WithAnd = new StringBuilder();
		StringBuilder zhiDanIMEI1WithAnd = new StringBuilder();
		StringBuilder zhiDanIMEIWithAnd = new StringBuilder();
		StringBuilder zhidanIMEIWithAnd = new StringBuilder();
		StringBuilder ridIMEIWithAnd = new StringBuilder();
		switch (type) {
		case 0:
		case 1:
			if (!StrKit.isBlank(imei)) {
				StringBuilder imeiValue = concatSqlParamValue(imei);
				imeiSnZhiDanWithAnd.append("IMEI IN (" + imeiValue + ") and ");
				zhiDanDATA1WithAnd.append("DATA1 IN (" + imeiValue + ") and ");
				zhiDanIMEI1WithAnd.append("IMEI1 IN (" + imeiValue + ") and ");
				zhiDanIMEIWithAnd.append("IMEI IN (" + imeiValue + ") and ");
				zhidanIMEIWithAnd.append("IMEI IN (" + imeiValue + ") and ");
				ridIMEIWithAnd.append("IMEI IN (" + imeiValue + ") and ");
			}
			if (!StrKit.isBlank(sn)) {
				StringBuilder snValue = concatSqlParamValue(sn);
				imeiSnZhiDanWithAnd.append("SN IN (" + snValue + ") and ");
				ridIMEIWithAnd.append("RID IN (" + snValue + ") and ");
			}
			if (!StrKit.isBlank(zhiDan)) {
				StringBuilder zhiDanValue = concatSqlParamValue(zhiDan);
				imeiSnZhiDanWithAnd.append("ZhiDan IN (" + zhiDanValue + ")");
				zhiDanDATA1WithAnd.append("ZhiDan IN (" + zhiDanValue + ")");
				zhiDanIMEI1WithAnd.append("ZhiDan IN (" + zhiDanValue + ")");
				zhiDanIMEIWithAnd.append("ZhiDan IN (" + zhiDanValue + ")");
				zhidanIMEIWithAnd.append("Zhidan IN (" + zhiDanValue + ")");
			}
			break;
		case 2:
			String[] imeiRange = imei.split(",");
			imeiSnZhiDanWithAnd.append("IMEI >= '" + imeiRange[0] + "' and IMEI <= '" + imeiRange[1] + "'");
			zhiDanDATA1WithAnd.append("DATA1 >= '" + imeiRange[0] + "' and DATA1 <= '" + imeiRange[1] + "'");
			zhiDanIMEI1WithAnd.append("IMEI1 >= '" + imeiRange[0] + "' and IMEI1 <= '" + imeiRange[1] + "'");
			zhiDanIMEIWithAnd.append("IMEI >= '" + imeiRange[0] + "' and IMEI <= '" + imeiRange[1] + "'");
			zhidanIMEIWithAnd.append("IMEI >= '" + imeiRange[0] + "' and IMEI <= '" + imeiRange[1] + "'");
			ridIMEIWithAnd.append("IMEI >= '" + imeiRange[0] + "' and IMEI <= '" + imeiRange[1] + "'");
			break;
		default:
			break;
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
	 * 根据参数的值拼接sql语句
	 * @param content 参数值
	 * @date 2019年6月10日 下午3:37:05
	 */
	private StringBuilder concatSqlParamValue(String content) {
		StringBuilder valueStringBuilder = new StringBuilder();
		for (String value : content.split(",")) {
			valueStringBuilder.append("'" + value + "',");
		}
		valueStringBuilder.deleteCharAt(valueStringBuilder.lastIndexOf(","));
		return valueStringBuilder;

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


	/**@author HCJ
	 * 多表删除前将数据备份为Excel文件
	 * @param imei IMEI号
	 * @param sn SN号
	 * @param zhiDan ZhiDan号
	 * @param type 参数类型
	 * @param deleteTable 需要进行删除的表格
	 * @date 2019年6月10日 下午3:34:01
	 */
	private void multiTableBackup(String imei, String sn, String zhiDan, Integer type, String deleteTable) throws Exception {
		MultiTableQueryInfo multiTableQueryInfo = multiTableQuery(imei, sn, zhiDan, type);
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
		if (StringUtils.endsWith(paramStringBuilder, "_")) {
			paramStringBuilder.delete(paramStringBuilder.lastIndexOf("_"), paramStringBuilder.length());
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
		String[] tables = deleteTable.split(",");

		if (multiTableQueryInfo.getDataRelativeSheet() != null && !multiTableQueryInfo.getDataRelativeSheet().isEmpty() && DELETE_TABLE_FLAG.equals(tables[0])) {
			helper.getBook().setSheetName(0, "DataRelativeSheet");
			helper.switchSheet(0);
			field = new String[] { "SN", "IMEI1", "IMEI2", "IMEI3", "IMEI4", "IMEI5", "IMEI6", "IMEI7", "IMEI8", "IMEI9", "IMEI10", "IMEI11", "IMEI12", "ZhiDan", "TestTime", "SimEffectiveDate" };
			head = new String[] { "SN", "IMEI1", "IMEI2", "IMEI3", "IMEI4", "IMEI5", "IMEI6", "IMEI7", "IMEI8", "IMEI9", "IMEI10", "IMEI11", "IMEI12", "ZhiDan", "TestTime", "SimEffectiveDate" };
			helper.fill(multiTableQueryInfo.getDataRelativeSheet(), "DataRelativeSheet", field, head);
		} else {
			helper.getBook().removeSheetAt(0);
		}

		if (multiTableQueryInfo.getDataRelativeUnique() != null && !multiTableQueryInfo.getDataRelativeUnique().isEmpty() && DELETE_TABLE_FLAG.equals(tables[1])) {
			helper.getBook().createSheet("DataRelativeUnique");
			helper.switchSheet("DataRelativeUnique");
			field = new String[] { "DATA1", "DATA2", "DATA3", "DATA4", "DATA5", "DATA6", "DATA7", "DATA8", "DATA9", "DATA10", "DATA11", "DATA12", "ZhiDan", "TestTime" };
			head = new String[] { "DATA1", "DATA2", "DATA3", "DATA4", "DATA5", "DATA6", "DATA7", "DATA8", "DATA9", "DATA10", "DATA11", "DATA12", "ZhiDan", "TestTime" };
			helper.fill(multiTableQueryInfo.getDataRelativeUnique(), "DataRelativeUnique", field, head);
		}

		if (multiTableQueryInfo.getGps_AutoTest_Result() != null && !multiTableQueryInfo.getGps_AutoTest_Result().isEmpty() && DELETE_TABLE_FLAG.equals(tables[2])) {
			helper.getBook().createSheet("Gps_AutoTest_Result");
			helper.switchSheet("Gps_AutoTest_Result");
			field = new String[] { "Id", "SN", "IMEI", "ZhiDan", "SoftModel", "Version", "Result", "TesterId", "Computer", "TestSetting", "TestTime", "Remark" };
			head = new String[] { "Id", "SN", "IMEI", "ZhiDan", "SoftModel", "Version", "Result", "TesterId", "Computer", "TestSetting", "TestTime", "Remark" };
			helper.fill(multiTableQueryInfo.getGps_AutoTest_Result(), "Gps_AutoTest_Result", field, head);
		}

		if (multiTableQueryInfo.getGps_AutoTest_Result3() != null && !multiTableQueryInfo.getGps_AutoTest_Result3().isEmpty() && DELETE_TABLE_FLAG.equals(tables[3])) {
			helper.getBook().createSheet("Gps_AutoTest_Result3");
			helper.switchSheet("Gps_AutoTest_Result3");
			field = new String[] { "Id", "SN", "IMEI", "ZhiDan", "SoftModel", "Version", "Result", "TesterId", "Computer", "TestSetting", "TestTime", "Remark" };
			head = new String[] { "Id", "SN", "IMEI", "ZhiDan", "SoftModel", "Version", "Result", "TesterId", "Computer", "TestSetting", "TestTime", "Remark" };
			helper.fill(multiTableQueryInfo.getGps_AutoTest_Result3(), "Gps_AutoTest_Result3", field, head);
		}

		if (multiTableQueryInfo.getGps_CartonBoxTwenty_Result() != null && !multiTableQueryInfo.getGps_CartonBoxTwenty_Result().isEmpty() && DELETE_TABLE_FLAG.equals(tables[4])) {
			helper.getBook().createSheet("Gps_CartonBoxTwenty_Result");
			helper.switchSheet("Gps_CartonBoxTwenty_Result");
			field = new String[] { "Id", "BoxNo", "IMEI", "ZhiDan", "SoftModel", "Version", "ProductCode", "Color", "Qty", "Weight", "Date", "TACInfo", "CompanyName", "TesterId", "TestTime", "Remark1", "Remark2", "Remark3", "Remark4", "Remark5", "Computer" };
			head = new String[] { "Id", "BoxNo", "IMEI", "ZhiDan", "SoftModel", "Version", "ProductCode", "Color", "Qty", "Weight", "Date", "TACInfo", "CompanyName", "TesterId", "TestTime", "Remark1", "Remark2", "Remark3", "Remark4", "Remark5", "Computer" };
			helper.fill(multiTableQueryInfo.getGps_CartonBoxTwenty_Result(), "Gps_CartonBoxTwenty_Result", field, head);
		}

		if (multiTableQueryInfo.getGps_CoupleTest_Result() != null && !multiTableQueryInfo.getGps_CoupleTest_Result().isEmpty() && DELETE_TABLE_FLAG.equals(tables[5])) {
			helper.getBook().createSheet("Gps_CoupleTest_Result");
			helper.switchSheet("Gps_CoupleTest_Result");
			field = new String[] { "Id", "SN", "IMEI", "ZhiDan", "SoftModel", "Version", "Result", "TesterId", "Computer", "TestSetting", "TestTime", "Remark" };
			head = new String[] { "Id", "SN", "IMEI", "ZhiDan", "SoftModel", "Version", "Result", "TesterId", "Computer", "TestSetting", "TestTime", "Remark" };
			helper.fill(multiTableQueryInfo.getGps_CoupleTest_Result(), "Gps_CoupleTest_Result", field, head);
		}

		if (multiTableQueryInfo.getGps_ManuPrintParam() != null && !multiTableQueryInfo.getGps_ManuPrintParam().isEmpty() && DELETE_TABLE_FLAG.equals(tables[6])) {
			helper.getBook().createSheet("Gps_ManuPrintParam");
			helper.switchSheet("Gps_ManuPrintParam");
			field = new String[] { "ID", "ZhiDan", "IMEI", "IMEIStart", "IMEIEnd", "SN", "IMEIRel", "SIM", "VIP", "BAT", "SoftModel", "Version", "Remark", "JS_PrintTime", "JS_TemplatePath", "JS_RePrintNum", "JS_ReFirstPrintTime", "JS_ReEndPrintTime", "UserName", "CH_PrintTime", "CH_TemplatePath1", "CH_TemplatePath2", "CH_RePrintNum", "CH_ReFirstPrintTime", "CH_ReEndPrintTime", "ICCID", "MAC", "Equipment" };
			head = new String[] { "ID", "ZhiDan", "IMEI", "IMEIStart", "IMEIEnd", "SN", "IMEIRel", "SIM", "VIP", "BAT", "SoftModel", "Version", "Remark", "JS_PrintTime", "JS_TemplatePath", "JS_RePrintNum", "JS_ReFirstPrintTime", "JS_ReEndPrintTime", "UserName", "CH_PrintTime", "CH_TemplatePath1", "CH_TemplatePath2", "CH_RePrintNum", "CH_ReFirstPrintTime", "CH_ReEndPrintTime", "ICCID", "MAC", "Equipment" };
			helper.fill(multiTableQueryInfo.getGps_ManuPrintParam(), "Gps_ManuPrintParam", field, head);
		}

		if (multiTableQueryInfo.getGps_TestResult() != null && !multiTableQueryInfo.getGps_TestResult().isEmpty() && DELETE_TABLE_FLAG.equals(tables[7])) {
			helper.getBook().createSheet("Gps_TestResult");
			helper.switchSheet("Gps_TestResult");
			field = new String[] { "Id", "SN", "IMEI", "SoftModel", "Version", "GPSResult", "CoupleResult", "WriteImeiResult", "ParamDownloadResult", "AutoTestResult", "Result", "AutoTestSMTResult", "ZhiDan", "RecordTime", "CPResult" };
			head = new String[] { "Id", "SN", "IMEI", "SoftModel", "Version", "GPSResult", "CoupleResult", "WriteImeiResult", "ParamDownloadResult", "AutoTestResult", "Result", "AutoTestSMTResult", "ZhiDan", "RecordTime", "CPResult" };
			helper.fill(multiTableQueryInfo.getGps_TestResult(), "Gps_TestResult", field, head);
		}

		if (multiTableQueryInfo.getNetMarkIMEI() != null && !multiTableQueryInfo.getNetMarkIMEI().isEmpty() && DELETE_TABLE_FLAG.equals(tables[8])) {
			helper.getBook().createSheet("NetMarkIMEI");
			helper.switchSheet("NetMarkIMEI");
			field = new String[] { "Id", "NetMark", "IMEI", "PrintCount", "SN", "JS_PrintTime", "JS_TemPlate", "NMTime", "RFID", "RePrintEndTime", "RePrintFirstTime", "RePrintNum", "Zhidan" };
			head = new String[] { "Id", "NetMark", "IMEI", "PrintCount", "SN", "JS_PrintTime", "JS_TemPlate", "NMTime", "RFID", "RePrintEndTime", "RePrintFirstTime", "RePrintNum", "Zhidan" };
			helper.fill(multiTableQueryInfo.getNetMarkIMEI(), "NetMarkIMEI", field, head);
		}

		if (multiTableQueryInfo.getGps_ManuSimDataParam() != null && !multiTableQueryInfo.getGps_ManuSimDataParam().isEmpty() && DELETE_TABLE_FLAG.equals(tables[9])) {
			helper.getBook().createSheet("Gps_ManuSimDataParam");
			helper.switchSheet("Gps_ManuSimDataParam");
			field = new String[] { "ID", "SDIP", "RID", "IMEI", "CID", "ICCID", "SDOperator", "SDTime", "SDRESULT", "ReSDTime", "ReSDCount" };
			head = new String[] { "ID", "SDIP", "RID", "IMEI", "CID", "ICCID", "SDOperator", "SDTime", "SDRESULT", "ReSDTime", "ReSDCount" };
			helper.fill(multiTableQueryInfo.getGps_ManuSimDataParam(), "Gps_ManuSimDataParam", field, head);
		}

		try {
			if (helper.getBook().getNumberOfSheets() == 0) {
				helper.getBook().createSheet("当前选择的条件没有可以备份的数据");
				helper.write(output, false);
			} else {
				helper.write(output, true);
				deleteHistoryService.add(fileName, file.getAbsolutePath(), time);
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (output != null) {
				output.close();
			}
		}
	}


	/**@author HCJ
	 * 根据条件进行批量删除
	 * @param imei IMEI号
	 * @param sn SN号
	 * @param zhiDan ZhiDan号
	 * @param type 参数类型
	 * @param deleteTable 需要进行删除的表格
	 * @date 2019年6月10日 下午3:34:01
	 */
	private void batchDelete(String imei, String sn, String zhiDan, Integer type, String deleteTable) {
		Map<String, StringBuilder> sqlParameterMap = concatSqlParameter(imei, sn, zhiDan, type);
		StringBuilder imeiSnZhiDan = sqlParameterMap.get("imeiSnZhiDan");
		StringBuilder zhiDanDATA1 = sqlParameterMap.get("zhiDanDATA1");
		StringBuilder zhiDanIMEI1 = sqlParameterMap.get("zhiDanIMEI1");
		StringBuilder zhiDanIMEI = sqlParameterMap.get("zhiDanIMEI");
		StringBuilder zhidanIMEI = sqlParameterMap.get("zhidanIMEI");
		StringBuilder ridIMEI = sqlParameterMap.get("ridIMEI");
		String[] tables = deleteTable.split(",");
		try {
			if (!zhiDanDATA1.toString().isEmpty() && DELETE_TABLE_FLAG.equals(tables[1])) {
				Db.update(SQL.DELETE_DATARELATIVEUNIQUE_SQL_FRAGMENT + zhiDanDATA1);
			}
			if (!zhiDanIMEI1.toString().isEmpty() && DELETE_TABLE_FLAG.equals(tables[0])) {
				Db.update(SQL.DELETE_DATARELATIVESHEET_SQL_FRAGMENT + zhiDanIMEI1);
			}
			if (!imeiSnZhiDan.toString().isEmpty()) {
				if (DELETE_TABLE_FLAG.equals(tables[2])) {
					Db.update(SQL.DELETE_AUTOTEST_RESULT_SQL_FRAGMENT + imeiSnZhiDan);
				}
				if (DELETE_TABLE_FLAG.equals(tables[3])) {
					Db.update(SQL.DELETE_AUTOTEST_RESULT3_SQL_FRAGMENT + imeiSnZhiDan);
				}
				if (DELETE_TABLE_FLAG.equals(tables[5])) {
					Db.update(SQL.DELETE_COUPLE_RESULT_SQL_FRAGMENT + imeiSnZhiDan);
				}
				if (DELETE_TABLE_FLAG.equals(tables[7])) {
					Db.update(SQL.DELETE_TEST_RESULT_SQL_FRAGMENT + imeiSnZhiDan);
				}
			}
			if (!zhiDanIMEI.toString().isEmpty()) {
				if (DELETE_TABLE_FLAG.equals(tables[4])) {
					Db.update(SQL.DELETE_CARTONBOX_RESULT_SQL_FRAGMENT + zhiDanIMEI);
				}
				if (DELETE_TABLE_FLAG.equals(tables[6])) {
					Db.update(SQL.DELETE_MANUPRINT_RESULT_SQL_FRAGMENT + zhiDanIMEI);
				}
			}
			if (!zhidanIMEI.toString().isEmpty() && DELETE_TABLE_FLAG.equals(tables[8])) {
				Db.use("db1").update(SQL.DELETE_NETMARKIMEI_RESULT_SQL_FRAGMENT + zhidanIMEI);
			}
			if (!ridIMEI.toString().isEmpty() && DELETE_TABLE_FLAG.equals(tables[9])) {
				Db.update(SQL.DELETE_MANUSIM_RESULT_SQL_FRAGMENT + ridIMEI);
			}
		} catch (Exception e) {
			e.printStackTrace();
			throw new OperationException("删除数据时出错");
		}
	}
}
