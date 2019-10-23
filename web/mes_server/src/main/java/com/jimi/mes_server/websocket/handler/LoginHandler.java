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
public class LoginHandler {

	private SopService sopService = Enhancer.enhance(SopService.class);
	
	
	public ResultUtil login(Session session, String mac,String userName,String time) {
		//如果SessionBox中存在该Session，则提示重复登录
		if(SessionBox.getIdBySession(session) != null) {
			throw new OperationException("一个连接只能登录一次，请不要重复登录");
		}
		//如果数据库不存在该实例，则保存到数据库并得到ADCT_ID
		SopSite sopSite = sopService.getSopSiteByMac(mac);
		if (sopSite==null) {
			throw new OperationException("当前MAC地址非法，无法登录");
		}
		 //如果数据库存在该实例，则直接获取记录中实例的ID做为ADCT_ID
			Integer loginId = sopSite.getId();
			//判断SessionBox中是否存在该ID的Session，如果存在，则提示：该名称已经被其他实例登录，如果一定要登录，请先关闭登录了该名称的实例。
			if(SessionBox.getSessionById(loginId) != null) {
				throw new OperationException("MAC为： "+sopSite.getMac()+" 的地址已经被其他客户端登录，如果您一定要继续登录，请先关闭该客户端");
		}
		//把ID与Session加入到SessionBox中
		SessionBox.addSession(loginId, session);
		sopService.addLoginLog( userName,time,sopSite.getSiteNumber(),RequestType.LOGIN);
		return ResultUtil.succeed("登录成功");
	}
	
}
