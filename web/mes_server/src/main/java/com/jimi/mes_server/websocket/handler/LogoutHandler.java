package com.jimi.mes_server.websocket.handler;

import javax.websocket.Session;

import com.jfinal.aop.Enhancer;
import com.jimi.mes_server.exception.OperationException;
import com.jimi.mes_server.model.SopSite;
import com.jimi.mes_server.service.SopService;
import com.jimi.mes_server.util.ResultUtil;
import com.jimi.mes_server.websocket.container.SessionBox;
import com.jimi.mes_server.websocket.entity.RequestType;

/**
 * 实例端登录控制器
 * <br>
 * <b>2019年4月11日</b>
 * @author 几米物联自动化部-洪达浩
 */
public class LogoutHandler {

	private SopService sopService = Enhancer.enhance(SopService.class);
	
	
	public ResultUtil logout(Session session, String userName,String time) {
		//如果SessionBox中存在该Session，则提示重复登录
		Integer siteId = SessionBox.getIdBySession(session);
		if(siteId == null) {
			throw new OperationException("请先登录");
		}
		//如果数据库不存在该实例，则保存到数据库并得到ADCT_ID
		SopSite sopSite = SopSite.dao.findById(siteId);
		if (sopSite==null) {
			throw new OperationException("当前站点信息不存在");
		}
		sopService.addLoginLog( userName,time,sopSite.getSiteNumber(),RequestType.LOGOUT);
		return ResultUtil.succeed("退出登录成功");
	}
	
}
