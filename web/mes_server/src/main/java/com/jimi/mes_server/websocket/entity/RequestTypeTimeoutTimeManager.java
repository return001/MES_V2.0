package com.jimi.mes_server.websocket.entity;

import java.util.HashMap;
import java.util.Map;

/**各种请求类型的超时时间
 * @date     2019年10月24日 下午1:45:54
 */
public class RequestTypeTimeoutTimeManager {

	private static final long SHOW = 30 * 1000;

	private static final long CANCELSHOW = 10 * 1000;

	private static final long LOGIN = 10 * 1000;

	private static final long LOGOUT = 10 * 1000;

	private static final long CANCELPICTURE = 10 * 1000;

	private static final long CANCELNOTICE = 10 * 1000;

	private static final Map<String, Long> map = new HashMap<>();

	static {
		map.put(RequestType.SHOW, SHOW);
		map.put(RequestType.CANCELSHOW, CANCELSHOW);
		map.put(RequestType.LOGIN, LOGIN);
		map.put(RequestType.LOGOUT, LOGOUT);
		map.put(RequestType.CANCELPICTURE, CANCELPICTURE);
		map.put(RequestType.CANCELNOTICE, CANCELNOTICE);
	}


	public static Map<String, Long> getMap() {
		return map;
	}

}
