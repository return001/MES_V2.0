package com.jimi.mes_server.entity;

import java.util.List;

public class ShowPackageBody {

	private Integer switchInterval;
    private List<Picture> picture;
    private List<Notice> notice;
	
	public Integer getSwitchInterval() {
		return switchInterval;
	}
	
	public void setSwitchInterval(Integer switchInterval) {
		this.switchInterval = switchInterval;
	}
	
	public List<Picture> getPicture() {
		return picture;
	}
	
	public void setPicture(List<Picture> picture) {
		this.picture = picture;
	}
	
	public List<Notice> getNotice() {
		return notice;
	}
	
	public void setNotice(List<Notice> notice) {
		this.notice = notice;
	}
}
