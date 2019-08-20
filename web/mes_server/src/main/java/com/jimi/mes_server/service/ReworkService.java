package com.jimi.mes_server.service;

import java.util.List;

import com.jfinal.kit.StrKit;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Record;
import com.jimi.mes_server.entity.MacReworkInfo;
import com.jimi.mes_server.entity.SQL;
import com.jimi.mes_server.exception.OperationException;
import com.jimi.mes_server.model.GpsAutotestAntidup;
import com.jimi.mes_server.model.GpsAutotestResult2;
import com.jimi.mes_server.model.GpsTestresult;
import com.jimi.mes_server.util.ResultUtil;

/**返工功能业务层
 * @author   HCJ
 * @date     2019年6月28日 上午10:38:38
 */
public class ReworkService {

	/**@author HCJ 
	 * 获取mac返工信息
	 * @param sn        SN号
	 * @param version   软件版本
	 * @param softModel 机型
	 * @param mac       MAC
	 * @date 2019年6月28日 上午10:33:38
	 */
	public ResultUtil getMacReworkInfo(String sn, String version, String softModel, String mac) {
		StringBuilder sql = new StringBuilder();
		List<GpsAutotestAntidup> gpsAutotestAntidups = null;
		List<GpsAutotestResult2> gpsAutotestResult2s = null;
		List<GpsTestresult> gpsTestresults = null;
		String suffix = " )";
		if (!StrKit.isBlank(sn)) {
			sql = concatSqlParameter(" SN in (", sn.split(","), suffix);
			if (!StrKit.isBlank(version)) {
				sql.append(concatSqlParameter(" and Version in (", version.split(","), suffix));
			}
			if (!StrKit.isBlank(softModel)) {
				sql.append(concatSqlParameter(" and SoftModel in (", softModel.split(","), suffix));
			}
			gpsAutotestAntidups = GpsAutotestAntidup.dao.find(SQL.SELECT_AUTOTEST_ANTIDUP_SQL_FRAGMENT + sql);
			if (gpsAutotestAntidups == null || gpsAutotestAntidups.isEmpty()) {
				throw new OperationException("Gps_AutoTest_AntiDup表格不存在符合条件的记录");
			}
			gpsAutotestResult2s = GpsAutotestResult2.dao.find(SQL.SELECT_AUTOTEST_RESULT2_SQL_FRAGMENT + sql);
			if (gpsAutotestResult2s == null || gpsAutotestResult2s.isEmpty()) {
				throw new OperationException("Gps_AutoTest_Result2表格不存在符合条件的记录");
			}
			gpsTestresults = GpsTestresult.dao.find(SQL.SELECT_TESTRESULT_SQL_FRAGMENT + sql);
			if (gpsTestresults == null || gpsTestresults.isEmpty()) {
				throw new OperationException("Gps_TestResult表格不存在符合条件的记录");
			}
		} else if (!StrKit.isBlank(mac)) {
			sql = concatSqlParameter(" Data1 in (", mac.split(","), suffix);
			gpsAutotestAntidups = GpsAutotestAntidup.dao.find(SQL.SELECT_AUTOTEST_ANTIDUP_SQL_FRAGMENT + sql);
			if (gpsAutotestAntidups == null || gpsAutotestAntidups.isEmpty()) {
				throw new OperationException("Gps_AutoTest_AntiDup表格不存在符合条件的记录");
			}
			gpsAutotestResult2s = GpsAutotestResult2.dao.find(SQL.SELECT_TABLE_DETAIL_SQL_FRAGMENT1 + sql + SQL.SELECT_TABLE_DETAIL_SQL_FRAGMENT2);
			if (gpsAutotestResult2s == null || gpsAutotestResult2s.isEmpty()) {
				throw new OperationException("Gps_AutoTest_Result2表格不存在符合条件的记录");
			}
			gpsTestresults = GpsTestresult.dao.find(SQL.SELECT_TABLE_DETAIL_SQL_FRAGMENT1 + sql + SQL.SELECT_TABLE_DETAIL_SQL_FRAGMENT3);
			if (gpsTestresults == null || gpsTestresults.isEmpty()) {
				throw new OperationException("Gps_TestResult表格不存在符合条件的记录");
			}
		}
		MacReworkInfo macReworkInfo = new MacReworkInfo(gpsAutotestAntidups, gpsAutotestResult2s, gpsTestresults);
		return ResultUtil.succeed(macReworkInfo);
	}


	/**@author HCJ
	 * 进行mac返工操作
	 * @param sn        SN号
	 * @param version   软件版本
	 * @param softModel 机型
	 * @param mac       MAC
	 * @date 2019年6月28日 上午10:34:57
	 */
	public boolean macRework(String sn, String version, String softModel, String mac) {
		StringBuilder sql = new StringBuilder();
		String suffix = " )";
		if (!StrKit.isBlank(sn)) {
			sql = concatSqlParameter(" SN in (", sn.split(","), suffix);
			if (!StrKit.isBlank(version)) {
				sql.append(concatSqlParameter(" and Version in (", version.split(","), suffix));
			}
			if (!StrKit.isBlank(softModel)) {
				sql.append(concatSqlParameter(" and SoftModel in (", softModel.split(","), suffix));
			}
			try {
				Db.update(SQL.DELETE_AUTOTEST_ANTIDUP_SQL_FRAGMENT + sql);
				Db.update(SQL.DELETE_AUTOTEST_RESULT2_SQL_FRAGMENT1 + sql);
				Db.update(SQL.UPDATE_AUTOTEST_RESULT2_FUNCTIONRESULT_SQL_FRAGMENT1 + sql);
				return true;
			} catch (Exception e) {
				throw new OperationException("更新数据库出错");
			}
		} else if (!StrKit.isBlank(mac)) {
			sql = concatSqlParameter(" Data1 in (", mac.split(","), suffix);
			List<Record> records = Db.find(SQL.SELECT_DISTINCT_AUTOTEST_ANTIDUP_BY_MAC_SQL_FRAGMENT + sql);
			if (records == null || records.isEmpty()) {
				throw new OperationException("Gps_AutoTest_AntiDup表格不存在符合条件的记录");
			}
			StringBuilder whereParam = new StringBuilder();
			whereParam.append(" SN in (");
			for (Record record : records) {
				whereParam.append(" '" + record.getStr("SN") + "',");
			}
			whereParam.deleteCharAt(whereParam.lastIndexOf(","));
			whereParam.append(" )");
			whereParam.append(" and Version in (");
			for (Record record : records) {
				whereParam.append(" '" + record.getStr("Version") + "',");
			}
			whereParam.deleteCharAt(whereParam.lastIndexOf(","));
			whereParam.append(" )");
			whereParam.append(" and SoftModel in (");
			for (Record record : records) {
				whereParam.append(" '" + record.getStr("SoftModel") + "',");
			}
			whereParam.deleteCharAt(whereParam.lastIndexOf(","));
			whereParam.append(" )");
			try {
				Db.update(SQL.DELETE_AUTOTEST_RESULT2_SQL_FRAGMENT2 + sql);
				Db.update(SQL.DELETE_AUTOTEST_ANTIDUP_SQL_FRAGMENT + sql);
				Db.update(SQL.UPDATE_AUTOTEST_RESULT2_FUNCTIONRESULT_SQL_FRAGMENT1 + whereParam);
				return true;
			} catch (Exception e) {
				throw new OperationException("更新数据库出错");
			}
		}
		return false;
	}


	/**@author HCJ
	 * 拼接sql参数
	 * @param prefix 前缀
	 * @param contentArray 参数数组
	 * @param suffix 后缀
	 * @date 2019年6月28日 上午10:37:13
	 */
	private StringBuilder concatSqlParameter(String prefix, String[] contentArray, String suffix) {
		StringBuilder sb = new StringBuilder();
		sb.append(prefix);
		for (String content : contentArray) {
			sb.append(" '" + content + "',");
		}
		sb.deleteCharAt(sb.lastIndexOf(","));
		sb.append(suffix);
		return sb;
	}

}
