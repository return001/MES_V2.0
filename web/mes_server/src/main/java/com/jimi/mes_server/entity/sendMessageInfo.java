package com.jimi.mes_server.entity;

public class sendMessageInfo {

	private Integer id;

	private Integer[] pictureList;

	private Integer[] noticeList;


	public Integer getId() {
		return id;
	}


	public void setId(Integer id) {
		this.id = id;
	}


	public Integer[] getPictureList() {
		return pictureList;
	}


	public void setPictureList(Integer[] pictureList) {
		this.pictureList = pictureList;
	}


	public Integer[] getNoticeList() {
		return noticeList;
	}


	public void setNoticeList(Integer[] noticeList) {
		this.noticeList = noticeList;
	}
}
