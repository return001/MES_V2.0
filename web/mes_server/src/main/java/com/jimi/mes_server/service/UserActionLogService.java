/**  
*  
*/
package com.jimi.mes_server.service;

import org.apache.commons.lang3.StringUtils;

import com.jfinal.kit.StrKit;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Page;
import com.jfinal.plugin.activerecord.Record;
import com.jfinal.plugin.activerecord.SqlPara;

/**  
 * <p>Title: UserActionLogService</p>  
 * <p>Description: </p>  
 * <p>Copyright: Copyright (c) 2019</p>  
 * <p>Company: 惠州市几米物联技术有限公司</p>  
 * @author trjie  
 * @date 2020年4月26日
 *
 */
public class UserActionLogService {

	public static final String SELECT_ACTIONLOG = "SELECT id,[time],address,url,result_code as resultCode,uid,consume_time as consumeTime,[action],parameters FROM action_log ";


	public Page<Record> selectUserActionLog(Integer pageNo, Integer pageSize, String timeFrom, String timeTo, String uid, String resultCode) {
		SqlPara sqlPara = new SqlPara();
		StringBuilder sql = new StringBuilder(SELECT_ACTIONLOG);
		sql.append(" WHERE 1 = 1 ");
		if (!StrKit.isBlank(uid)) {
			sql.append(" AND uid like '%").append(uid).append("%'");
		}
		if (!StrKit.isBlank(resultCode)) {
			sql.append(" AND result_code like '%").append(resultCode).append("%'");
		}
		if (!StrKit.isBlank(timeFrom)) {
			sql.append(" AND time >= '").append(timeFrom).append("'");
		}
		if (!StrKit.isBlank(timeTo)) {
			sql.append(" AND time <= '").append(timeTo).append("'");
		}
		if (StringUtils.endsWith(sql, "1 ")) {
			sql.delete(sql.lastIndexOf("WHERE"), sql.length());
		}
		sql.append(" ORDER BY id DESC");
		sqlPara.setSql(sql.toString());
		return Db.paginate(pageNo, pageSize, sqlPara);
	}
}
