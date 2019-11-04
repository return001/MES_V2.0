package com.jimi.mes_server.websocket.handler;

import javax.websocket.Session;

import com.jfinal.aop.Enhancer;
import com.jimi.mes_server.entity.Constant;
import com.jimi.mes_server.model.SopSite;
import com.jimi.mes_server.service.SopService;
import com.jimi.mes_server.util.ResultUtil;
import com.jimi.mes_server.websocket.container.SessionBox;

/**客户端退出登录处理器
 * @author   HCJ
 * @date     2019年10月24日 下午1:48:45
 */
public class LogoutHandler {

	private SopService sopService = Enhancer.enhance(SopService.class);


	public ResultUtil logout(Session session, String userName, String time) {
		// 如果SessionBox中不存在该Session,则提示请先登录
		Integer siteId = SessionBox.getIdBySession(session);
		if (siteId == null) {
			return ResultUtil.failed(412, "请先登录");
		}
		// 如果数据库不存在该实例，则提示站点不存在
		SopSite sopSite = SopSite.dao.findById(siteId);
		if (sopSite == null) {
			return ResultUtil.failed(412, "当前站点信息不存在");
		}
		SessionBox.removeSession(session);
		sopService.addLoginLog(userName, time, sopSite.getSiteNumber(), Constant.CLIENT_LOGOUT);
		return ResultUtil.succeed("退出登录成功");
	}

}
