package com.jimi.mes_server.websocket.socket;

import java.io.IOException;

import javax.websocket.CloseReason;
import javax.websocket.OnClose;
import javax.websocket.OnError;
import javax.websocket.OnMessage;
import javax.websocket.OnOpen;
import javax.websocket.Session;
import javax.websocket.server.ServerEndpoint;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import com.alibaba.fastjson.JSONException;
import com.jimi.mes_server.websocket.container.SessionBox;

import cc.darhao.pasta.Pasta;

/**SOP的websocket服务端
 * @author   HCJ
 * @date     2019年10月24日 下午1:52:18
 */
@ServerEndpoint("/sopserver")
public class SopServerWebSocket {

	private static final Logger logger = LogManager.getRootLogger();


	@OnOpen
	public void onOpen(Session session) {
		logger.debug("session接入，ID为:" + session.getId());
	}


	@OnMessage
	public void onMessage(Session session, String message) {
		logger.debug("session收到信息，ID为:" + session.getId() + "，信息为:" + message);
		try {
			Pasta.receiveMessage(session, message);
		} catch (IOException e) {
			logger.error("session发生错误，ID为:" + session.getId() + "，信息为:" + e.getMessage());
		} catch (JSONException e) {
			logger.warn(e.getMessage());
		}
	}


	@OnClose
	public void onClose(Session session, CloseReason closeReason) {
		logger.warn("session关闭，ID为:" + session.getId() + "，原因为:" + closeReason.getReasonPhrase());
		SessionBox.removeSession(session);
	}


	@OnError
	public void onError(Session session, Throwable error) {
		logger.error("session发生错误，ID为:" + session.getId() + "，信息为:" + error.getMessage());
	}

}