package com.jimi.mes_server.websocket.logger;

import java.util.Date;

import com.jimi.mes_server.model.PackageLog;
import com.jimi.mes_server.websocket.container.SessionBox;
import com.jimi.mes_server.websocket.entity.Constant;

import cc.darhao.pasta.PastaLogCatcher;
import cc.darhao.pasta.RequestResponsePair;


public class PackageLogger extends PastaLogCatcher{

	@Override
	public void onRequestFinish(RequestResponsePair info) {
		PackageLog log = createLog(info);
		log.setFromId(Constant.SERVER_WEBSOCKET_ID);
		log.setToId(SessionBox.getIdBySession(info.getSession()));
		log.setConsumeTime((int) (new Date().getTime() - info.getRequestTime().getTime()));
		log.save();
	}


	@Override
	public void onRequestTimeout(RequestResponsePair info) {
		PackageLog log = createLog(info);
		log.setFromId(Constant.SERVER_WEBSOCKET_ID);
		log.setToId(SessionBox.getIdBySession(info.getSession()));
		log.setConsumeTime(0);
		log.save();
	}


	@Override
	public void onResponseFinish(RequestResponsePair info) {
		PackageLog log = createLog(info);
		log.setFromId(SessionBox.getIdBySession(info.getSession()));
		log.setToId(Constant.SERVER_WEBSOCKET_ID);
		log.setConsumeTime((int) (new Date().getTime() - info.getRequestTime().getTime()));
		log.save();
	}


	private static PackageLog createLog(RequestResponsePair pair) {
		PackageLog log = new PackageLog();
		log.setPackageId(pair.getRequestPackage().getId());
		log.setTime(pair.getRequestTime());
		log.setPackageType(pair.getRequestPackage().getType());
		try {
			log.setPackageRequest(pair.getRequestPackage().getBody().toString());
		} catch (NullPointerException e) {
		}
		try {
			log.setPackageResponse(pair.getResponsePackage().getBody().toString());
		} catch (NullPointerException e) {
		}
		return log;
	}
	
}
