package com.jimi.mes_server.service;

import java.io.OutputStream;

import com.jfinal.aop.Enhancer;
import com.jfinal.plugin.activerecord.Page;
import com.jfinal.plugin.activerecord.Record;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.model.DataRelativeSheet;
import com.jimi.mes_server.service.base.SelectService;
import com.jimi.mes_server.util.ExcelHelper;

/**报表业务层
 * @author   HCJ
 * @date     2018年10月11日 下午6:00:21
 */
public class ReportService extends SelectService{

	private static SelectService daoService = Enhancer.enhance(SelectService.class);
	
	
	public void download(String table, String ascBy, String descBy, String filter, OutputStream output, String fileName) throws Exception {
		Page<Record> records = daoService.select(table, null, null, ascBy, descBy, filter, null);
		ExcelHelper helper = ExcelHelper.create(true);
		String[] field = null;
		String[] head = null;
		if (table.equals("Gps_AutoTest_Result") || table.equals("Gps_AutoTest_Result2") || table.equals("Gps_AutoTest_Result3") || table.equals("Gps_CoupleTest_Result")) {
			field = new String[] { "Id", "SN", "IMEI", "ZhiDan", "SoftModel", "Version", "Result", "TesterId", "Computer", "TestSetting", "TestTime", "Remark"};
			head =  new String[] { "序号", "SN号", "IMEI号", "制单号", "机型", "软件版本", "结果", "测试员", "测试电脑", "测试配置", "测试时间", "备注"};			
		}else if(table.equals("Gps_CartonBoxTwenty_Result")) {
			field = new String[] { "Id", "BoxNo", "IMEI", "ZhiDan", "SoftModel", "Version", "ProductCode", "Color", "Qty", "Weight", "Date", "TACInfo", "CompanyName", "TesterId", "TestTime", "Remark1", "Remark2", "Remark3", "Remark4", "Remark5", "Computer"};
			head =  new String[] { "序号", "箱号", "IMEI号", "制单号", "机型", "软件版本", "生产编码", "颜色", "Qty", "重量", "日期", "TACInfo", "公司名称", "测试人员", "测试时间", "备注1", "备注2", "备注3", "备注4", "备注5", "电脑名称"};
		}else if(table.equals("Gps_ManuLdParam")) {
			field = new String[] { "ID", "LDTime", "LDIP", "ZhiDan", "IMEI", "SN", "ICCID", "SIM", "VIP", "BAT", "EquipmentNumber", "MAC", "SoftModel", "Version", "Remark", "LDRESULT", "ReLdImeiNum", "ReLdImeiTime"};
			head =  new String[] { "序号", "镭雕成功时间", "主机IP", "制单号", "IMEI号", "递增SN号", "ICCID卡串号", "SIM卡号", "VIP号", "电池号", "设备号", "蓝牙号", "软件型号", "版本号", "备注", "镭雕结果", "此IMEI号重镭次数", "最后重镭时间"};
		}else if(table.equals("Gps_ManuPrintParam")) {			
			field = new String[] { "ID", "ZhiDan", "IMEI", "IMEIStart", "IMEIEnd", "SN", "IMEIRel", "SIM", "VIP", "BAT", "SoftModel", "Version", "Remark", "JS_PrintTime", "JS_TemplatePath", "JS_RePrintNum", "JS_ReFirstPrintTime", "JS_ReEndPrintTime", "UserName", "CH_PrintTime", "CH_TemplatePath1", "CH_TemplatePath2", "CH_RePrintNum", "CH_ReFirstPrintTime", "CH_ReEndPrintTime", "ICCID", "MAC", "Equipment"};
			head =  new String[] { "序号", "制单号", "打印IMEI", "IMEI起始位", "IMEI终止位", "递增SN", "IMEI绑定关系", "SIM卡号", "服务卡号", "电池号", "软件型号", "软件版本", "备注", "机身贴打印时间", "机身贴模板路径", "机身贴重打次数", "机身贴首次重打次数", "机身贴末次重打次数", "用户名", "彩盒打印时间", "彩盒贴模板路径1", "彩盒贴模板路径2", "彩盒贴重打次数", "彩盒贴首次重打时间", "彩盒贴末次重打时间", "ICCID号", "蓝牙号", "设备号"};
		}else if(table.equals("Gps_TestResult")) {
			field = new String[] { "Id", "SN", "IMEI", "SoftModel", "Version", "GPSResult", "CoupleResult", "WriteImeiResult", "ParamDownloadResult", "AutoTestResult", "Result", "AutoTestSMTResult", "ZhiDan", "RecordTime", "CPResult"};
			head =  new String[] { "序号", "序列号", "IMEI号", "软件型号", "版本号", "功能2测试位", "耦合测试位", "IMEI号烧写测试位", "参数下载测试位", "功能1测试位", "总测试位", "SMT自动测试结果", "制单号", "记录时间", "CPResult"};
		}else if(table.equals("DataRelativeSheet")) {
			field = new String[] { "SN", "IMEI1", "IMEI2", "IMEI3", "IMEI4", "IMEI5", "IMEI6", "IMEI7", "IMEI8", "IMEI9", "IMEI10", "IMEI11", "IMEI12", "ZhiDan", "TestTime", "SimEffectiveDate"};
			head =  new String[] { "序列号SN", "IMEI号", "递增SN号", "SIM卡号", "ICCID卡串号", "密码", "蓝牙MAC", "设备号", "服务卡号", "电池序列号", "第二个锁ID", "IMEI11", "IMEI12", "制单号", "测试时间", "Sim卡最迟激活日期"};
		}else if(table.equals("GpsSMT_TcData") || table.equals("GpsTcData")) {
			field = new String[] { "Id", "SN", "FixMode", "gpsdb0", "gpsdb1", "gpsdb2", "gpsdb3", "gpsdb4", "gpsdb5", "gpsdb6", "gpsdb7", "gpsdb8", "gpsdb9", "gpsdb10", "gpsdb11"};
			head =  new String[] { "序号", "SN号", "定位方式", "GpsDb信息_1", "GpsDb信息_2", "GpsDb信息_3", "GpsDb信息_4", "GpsDb信息_5", "GpsDb信息_6", "GpsDb信息_7", "GpsDb信息_8", "GpsDb信息_9", "GpsDb信息_10", "GpsDb信息_11", "GpsDb信息_12"};
		}else if(table.equals("Gps_OperRecord")) {
			field = new String[] { "id", "OperName", "OperContent", "OperTime", "OperDemo"};
			head =  new String[] { "序号", "操作用户", "操作事项", "操作时间", "OperDemo"};
		}
		helper.fill(records.getList(), fileName, field, head);
		helper.write(output, true);
	}


	public boolean createRelativeSheet(DataRelativeSheet dataRelativeSheet) {
		if(dataRelativeSheet.getTestTime() == null || dataRelativeSheet.getIMEI1() == null) {
			throw new ParameterException("only create the dataRelativeSheet have the TestTime and IMEI1");
		}
		DataRelativeSheet sheet = DataRelativeSheet.dao.findById(dataRelativeSheet.getIMEI1());
		if (sheet != null) {
			throw new ParameterException("only create the dataRelativeSheet once");
		}		
		return dataRelativeSheet.save();
	}
	
	
	public boolean updateRelativeSheet(DataRelativeSheet dataRelativeSheet) {
		if(dataRelativeSheet.getTestTime() == null || dataRelativeSheet.getIMEI1() == null) {
			throw new ParameterException("only update the dataRelativeSheet have the TestTime and IMEI1");
		}
		DataRelativeSheet sheet = DataRelativeSheet.dao.findById(dataRelativeSheet.getIMEI1());
		if (sheet == null) {
			throw new ParameterException("only update the dataRelativeSheet exist");
		}		
		return dataRelativeSheet.update();
	}
	
	
}
