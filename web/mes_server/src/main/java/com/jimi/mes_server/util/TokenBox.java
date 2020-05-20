package com.jimi.mes_server.util;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import com.jimi.mes_server.controller.UserController;
import com.jimi.mes_server.entity.vo.LUserAccountVO;

import cc.darhao.dautils.api.MD5Util;

/**
 * 用于保存会话信息，通过tokenId进行信息存取，自带超时检测线程
 * <br>
 * <b>2018年6月2日</b>
 * @author 沫熊工作室 <a href="http://www.darhao.cc">www.darhao.cc</a>
 */
public class TokenBox {

	// TokenId字段名
	public static final String TOKEN_ID_KEY_NAME = "#TOKEN#";
	
	public static final String USER_KEY_NAME = "#USER#";

	// 上次访问时间字段名
	private static final String LAST_ACCESS_TIME_KEY_NAME = "#LAST_ACCESS_TIME#";

	// 超时检查线程遍历周期，单位：毫秒
	private static final long CHECK_CYCLE = 60 * 1000;

	// 同步锁
	private static final Object lock = new Object();

	// 会话合集
	private static Map<String, Map<String, Object>> sessions;

	//用户与token映射Map
	private static Map<String, List<String>> userTokenMap;
	// 超时检查线程
	private static Thread timeoutThread;


	/**
	 * 开启tokenBox，设置超时时间，单位：小时，设置为0表示永不超时
	 */
	public static void start(int timeoutTime) {
		sessions = new HashMap<>();
		userTokenMap = new HashMap<>();
		if (timeoutTime != 0) {
			timeoutThread = new Thread(() -> {
				try {
					while (true) {
						Thread.sleep(CHECK_CYCLE);
						synchronized (lock) {
							long now = System.currentTimeMillis();
							// 创建遍历副本
							Map<String, Map<String, Object>> sessionsCopy = new HashMap<>();
							for (Entry<String, Map<String, Object>> session : sessions.entrySet()) {
								sessionsCopy.put(session.getKey(), session.getValue());
							}
							// 去掉超时session
							for (Entry<String, Map<String, Object>> session : sessionsCopy.entrySet()) {
								if (now - ((long) (session.getValue().get(LAST_ACCESS_TIME_KEY_NAME))) > timeoutTime * 60 * 60 * 1000) {
									LUserAccountVO lUserAccount = (LUserAccountVO) session.getValue().get(UserController.SESSION_KEY_LOGIN_USER);
									if (lUserAccount != null) {
										List<String> tokens = userTokenMap.get(lUserAccount.getName());
										if (tokens != null) {
											tokens.remove(session.getKey());
										}
									}
									sessions.remove(session.getKey());
								}
							}
						}
					}
				} catch (InterruptedException e) {
				}
			});
			timeoutThread.start();
		}
		System.out.println("TokenBox is Running Now...");
	}


	/**
	 * 停止tokenBox
	 */
	public static void stop() {
		if (timeoutThread != null) {
			timeoutThread.interrupt();
		}
		System.out.println("TokenBox was Stopped.");
	}


	/**
	 * 根据tokenId和key，设置值，如果id不存在，则会创建一个和id绑定的session再设置值
	 */
	public static void put(String tokenId, String key, Object value) {
		Map<String, Object> session = sessions.get(tokenId);
		// 如果没有该id的session则创建一个
		if (session == null) {
			session = new HashMap<>();
			sessions.put(tokenId, session);
		}
		refreshLastAccessTime(session);
		session.put(key, value);
	}


	/**
	 * 
	 * <p>Description: 添加用户与token的绑定关系<p>
	 * @return
	 * @exception
	 * @author trjie
	 * @Time 2020年4月24日
	 */
	public static void putUserToken(String username, String tokenId) {
		
		List<String> tokens = userTokenMap.get(username);
		if (tokens == null) {
			tokens = new ArrayList<String>();
		}
		tokens.add(tokenId);
		userTokenMap.put(username, tokens);
	}
	
	/**
	 * 
	 * <p>Description: 删除用户与所有token的绑定关系<p>
	 * @return
	 * @exception
	 * @author trjie
	 * @Time 2020年4月24日
	 */
	public static void removeUserTokenByUsername(String username) {
		synchronized (lock) {
			List<String> tokens = userTokenMap.get(username);
			if (tokens == null) {
				userTokenMap.remove(username);
			}
			for (String tokenId : tokens) {
				sessions.remove(tokenId);
			}
		}
	}
	
	
	/**
	 * 
	 * <p>Description: 删除用户与token的绑定关系<p>
	 * @return
	 * @exception
	 * @author trjie
	 * @Time 2020年4月24日
	 */
	public static void removeUserTokenByUserNameAndToken(String username, String token) {
		synchronized (lock) {
			List<String> tokens = userTokenMap.get(username);
			if (tokens == null) {
				userTokenMap.remove(username);
			}
			for (String tokenId : tokens) {
				if (tokenId.equals(token)) {
					
					sessions.remove(tokenId);
				}
			}
			userTokenMap.get(username).remove(token);
		}
	}
	

	/**
	 * 根据tokenId和key获取值
	 */
	@SuppressWarnings("unchecked")
	public static <T> T get(String tokenId, String key) {
		Map<String, Object> session = sessions.get(tokenId);
		// 如果没有该id的session则返回null
		if (session == null) {
			return null;
		}
		refreshLastAccessTime(session);
		return (T) session.get(key);
	}


	/**
	 * 移除一个session
	 */
	public static void remove(String tokenId) {
		synchronized (lock) {
			sessions.remove(tokenId);
		}
	}


	/**
	 * 创建一个随机的32位TokenId
	 */
	public static String createTokenId() {
		return MD5Util.MD5(System.currentTimeMillis() + "Darhao");
	}


	/**
	 * 刷新最后一次访问时间
	 */
	private static void refreshLastAccessTime(Map<String, Object> session) {
		session.put(LAST_ACCESS_TIME_KEY_NAME, System.currentTimeMillis());
	}


	/**
	 * 单元测试
	 */
	public static void main(String[] args) {
		start(30);
		String token1 = createTokenId();
		String token2 = createTokenId();
		put(token1, "age", 100);
		put(token2, "name", "HONG");
		get(token2, "123");
		get("asdasdasdasd", "123");
		get(token1, "age");
		put(token1, "name", "BOBO");
		remove(token1);
		get(token1, "name");
		stop();
	}
}
