package com.jimi.mes_server.websocket.handler;

import javax.websocket.Session;

import com.jfinal.aop.Enhancer;
import com.jimi.mes_server.entity.Constant;
import com.jimi.mes_server.entity.SQL;
import com.jimi.mes_server.entity.SopSQL;
import com.jimi.mes_server.model.LUserAccount;
import com.jimi.mes_server.model.SopPositionAssignment;
import com.jimi.mes_server.model.SopSite;
import com.jimi.mes_server.service.SopService;
import com.jimi.mes_server.thread.SendDisplayInfoThread;
import com.jimi.mes_server.util.ResultUtil;
import com.jimi.mes_server.websocket.container.SessionBox;

/**客户端登录处理器
 * @author   HCJ
 * @date     2019年10月24日 下午1:46:37
 */
public class LoginHandler {

	private SopService sopService = Enhancer.enhance(SopService.class);


	public ResultUtil login(Session session, String mac, String userName, String time) {
		// 如果数据库不存在该实例，则提示非法登录
		SopSite sopSite = sopService.getSopSiteByMac(mac);
		if (sopSite == null) {
			return ResultUtil.failed(412, "当前MAC地址非法，无法登录");
		}
		LUserAccount user = LUserAccount.dao.findFirst(SQL.SELECT_USER_BY_DES, userName);
		if (user == null || !user.getInService()) {
			return ResultUtil.failed(412, "当前用户不存在或未启用");
		}
		// 如果数据库存在该实例，则直接获取记录中实例的ID作为登录的ID
		Integer loginId = sopSite.getId();
		SopPositionAssignment positionAssignment = SopPositionAssignment.dao.findFirst(SopSQL.SELECT_POSTINFO_BY_SITE, loginId);
		if (positionAssignment != null) {
			if (!positionAssignment.getUserId().equals(user.getId())) {
				return ResultUtil.failed(412, "当前用户与被指派用户不符");
			}
		}
		// 判断SessionBox中是否存在该ID的Session，如果存在，则移除
		Session temp = SessionBox.getSessionById(loginId);
		if (temp != null) {
			SessionBox.removeSession(temp);
		}
		// 把ID与Session加入到SessionBox中
		SessionBox.addSession(loginId, session);
		sopService.addLoginLog(userName, time, sopSite.getSiteNumber(), Constant.CLIENT_LOGIN);
		sopService.updateUserState(user.getId(), true);
		SendDisplayInfoThread sendDisplayInfoThread = new SendDisplayInfoThread(sopSite);
		sendDisplayInfoThread.start();
		user.keep("UserDes", "MainProcess", "Proficiency", "OtherProcess");
		return ResultUtil.succeed(user);
	}

}
