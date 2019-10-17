package com.jimi.mes_server.websocket.container;

import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import javax.websocket.Session;


/**
 * 存放会话的容器
 * <br>
 * <b>2019年4月9日</b>
 * @author 几米物联自动化部-洪达浩
 */
public class SessionBox {
	
	// 将site的ID与session绑定起来
	private static final Map<Integer, Session> sessionMap = new HashMap<>();

	
	public static synchronized Session getSessionById(Integer id) {
		return sessionMap.get(id);
	}
	
	
	public static synchronized Integer getIdBySession(Session session) {
		for (Entry<Integer, Session> sessionEntry : sessionMap.entrySet()) {
			if(sessionEntry.getValue().equals(session)) {
				return sessionEntry.getKey();
			}
		}
		return null;
	}
	
	
	public static synchronized void addSession(Integer id, Session session) {
		sessionMap.put(id, session);
	}
	
	
	public static synchronized void removeSession(Session session) {
		sessionMap.remove(getIdBySession(session));
	}
}
