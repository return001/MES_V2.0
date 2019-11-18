package com.jimi.mes_server.thread;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.List;
import javax.websocket.Session;

import com.alibaba.fastjson.JSONObject;
import com.jfinal.kit.StrKit;
import com.jimi.mes_server.entity.Constant;
import com.jimi.mes_server.entity.SopSQL;
import com.jimi.mes_server.model.Line;
import com.jimi.mes_server.model.SopFilePicture;
import com.jimi.mes_server.model.SopNotice;
import com.jimi.mes_server.model.SopSite;
import com.jimi.mes_server.model.SopSiteDisplay;
import com.jimi.mes_server.websocket.container.SessionBox;
import com.jimi.mes_server.websocket.entity.Notice;
import com.jimi.mes_server.websocket.entity.Picture;
import com.jimi.mes_server.websocket.entity.RequestType;

import cc.darhao.pasta.Pasta;

/**发送站点当前需要显示的内容
 * @author   HCJ
 * @date     2019年11月18日 上午11:25:29
 */
public class SendDisplayInfoThread extends Thread {

	private SopSite sopSite;


	public SendDisplayInfoThread(SopSite sopSite) {
		super();
		this.sopSite = sopSite;
	}


	@Override
	public void run() {
		try {
			Thread.sleep(3000);
			sendDisplayInfo();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}


	private void sendDisplayInfo() throws Exception {
		SopSiteDisplay sopSiteDisplay = SopSiteDisplay.dao.findFirst(SopSQL.SELECT_SITEDISPLAY_BY_SITE, sopSite.getId());
		if (sopSiteDisplay != null && (!StrKit.isBlank(sopSiteDisplay.getPictures()) || !StrKit.isBlank(sopSiteDisplay.getNotices()))) {
			JSONObject requestBody = new JSONObject();
			SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
			if (!StrKit.isBlank(sopSiteDisplay.getPictures())) {
				List<Picture> pictures = new ArrayList<Picture>();
				for (String pictureId : sopSiteDisplay.getPictures().split(",")) {
					if (!StrKit.isBlank(pictureId)) {
						SopFilePicture sopFilePicture = SopFilePicture.dao.findById(Integer.parseInt(pictureId));
						if (sopFilePicture == null) {
							continue;
						}
						String picturePath = sopFilePicture.getPicturePath();
						String urlPath = picturePath.substring(picturePath.indexOf("mes_document")).replace("\\", "/");
						Picture picture = new Picture();
						picture.setPath(Constant.SOP_PICTURE_URL + urlPath);
						pictures.add(picture);
					}
				}
				requestBody.put("picture", pictures);
			}
			if (!StrKit.isBlank(sopSiteDisplay.getNotices())) {
				List<Notice> notices = new ArrayList<Notice>();
				for (String noticeId : sopSiteDisplay.getNotices().split(",")) {
					SopNotice sopNotice = SopNotice.dao.findById(Integer.parseInt(noticeId));
					if (sopNotice == null) {
						continue;
					}
					Notice notice = new Notice();
					notice.setTitle(sopNotice.getTitle());
					notice.setContent(sopNotice.getContent());
					notice.setStartTime(dateFormat.format(sopNotice.getStartTime()));
					notice.setEndTime(dateFormat.format(sopNotice.getEndTime()));
					notices.add(notice);
				}
				requestBody.put("notice", notices);
			}
			Line line = Line.dao.findById(sopSite.getLineId());
			requestBody.put("switchInterval", sopSite.getSwitchInterval());
			requestBody.put("timeLength", line.getTimeLength());
			Session session = SessionBox.getSessionById(sopSite.getId());
			Pasta.sendRequest(session, RequestType.SHOW, requestBody);
		}
	}

}
