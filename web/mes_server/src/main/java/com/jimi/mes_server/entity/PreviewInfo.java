package com.jimi.mes_server.entity;

import java.util.List;

import com.jimi.mes_server.websocket.entity.Notice;
import com.jimi.mes_server.websocket.entity.Picture;

/**SOP站点预览信息
 * @author   HCJ
 * @date     2019年10月24日 下午1:55:38
 */
public class PreviewInfo {

	private List<Picture> pictures;

	private List<Notice> notices;


	public List<Picture> getPictures() {
		return pictures;
	}


	public void setPictures(List<Picture> pictures) {
		this.pictures = pictures;
	}


	public List<Notice> getNotices() {
		return notices;
	}


	public void setNotices(List<Notice> notices) {
		this.notices = notices;
	}
}
