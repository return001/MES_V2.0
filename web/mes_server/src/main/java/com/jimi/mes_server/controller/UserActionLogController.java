/**  
*  
*/  
package com.jimi.mes_server.controller;

import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.service.UserActionLogService;
import com.jimi.mes_server.util.ResultUtil;

/**  
 * <p>Title: UserActionLogController</p>  
 * <p>Description: </p>  
 * <p>Copyright: Copyright (c) 2019</p>  
 * <p>Company: 惠州市几米物联技术有限公司</p>  
 * @author trjie  
 * @date 2020年4月26日
 *
 */
public class UserActionLogController extends Controller {

	private UserActionLogService logService = Enhancer.enhance(UserActionLogService.class);
	/**
	 * 
	 * <p>Description: 查询日志 <p>
	 * @return
	 * @exception
	 * @author trjie
	 * @Time 2020年4月26日
	 */
	public void selectUserActionLog(Integer pageNo, Integer pageSize, String timeFrom, String timeTo, String uid, String resultCode) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页容量不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页容量均需要大于0");
		}
		renderJson(ResultUtil.succeed(logService.selectUserActionLog(pageNo, pageSize, timeFrom, timeTo, uid, resultCode)));
	}
}
