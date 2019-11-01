package com.jimi.mes_server.websocket.handler;

import javax.websocket.Session;

import com.jfinal.aop.Enhancer;
import com.jfinal.kit.StrKit;
import com.jimi.mes_server.entity.Constant;
import com.jimi.mes_server.entity.SopSQL;
import com.jimi.mes_server.model.Line;
import com.jimi.mes_server.model.SopFilePicture;
import com.jimi.mes_server.model.SopSite;
import com.jimi.mes_server.model.SopSiteDisplay;
import com.jimi.mes_server.service.SopService;
import com.jimi.mes_server.util.ResultUtil;
import com.jimi.mes_server.websocket.container.SessionBox;

public class ConfirmHandler {

	private SopService sopService = Enhancer.enhance(SopService.class);


	public ResultUtil confirm(Session session, String userName, String time) {
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
		Line line = Line.dao.findById(sopSite.getLineId());
		String lineName = null;
		StringBuilder content = new StringBuilder();
		if (line != null && !StrKit.isBlank(line.getLineName())) {
			lineName = line.getLineName();
		}
		SopSiteDisplay sopSiteDisplay = SopSiteDisplay.dao.findFirst(SopSQL.SELECT_SITEDISPLAY_BY_SITE, sopSite.getId());
		if (sopSiteDisplay != null && !StrKit.isBlank(sopSiteDisplay.getPictures())) {
			content.append("该站点播放的图片：");
			for (String pictureId : sopSiteDisplay.getPictures().split(",")) {
				if (StrKit.isBlank(pictureId)) {
					continue;
				}
				SopFilePicture sopFilePicture = SopFilePicture.dao.findById(Integer.parseInt(pictureId));
				if (sopFilePicture != null) {
					content.append(sopFilePicture.getPictureName() + " , ");
				}
			}
			content.append("操作员确认成功");
		}
		sopService.addConfirmLog(userName, time, sopSite.getSiteNumber(), lineName, content.toString(), Constant.OPERATOR_CONFIRMATION);
		return ResultUtil.succeed("接收确认成功");
	}
}
