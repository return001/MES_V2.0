package com.jimi.mes_server.entity;

import java.util.List;

import com.jimi.mes_server.entity.vo.PictureVO;

public class FileHistoryDetail {

	private String siteNumber;
	private String siteName ;
	private String line ;
	private String workshop ;
	private String factory ;
	 private List<PictureVO> pictures;
	
	public String getSiteNumber() {
		return siteNumber;
	}
	
	public void setSiteNumber(String siteNumber) {
		this.siteNumber = siteNumber;
	}
	
	public String getSiteName() {
		return siteName;
	}
	
	public void setSiteName(String siteName) {
		this.siteName = siteName;
	}
	
	public String getLine() {
		return line;
	}
	
	public void setLine(String line) {
		this.line = line;
	}
	
	public String getWorkshop() {
		return workshop;
	}
	
	public void setWorkshop(String workshop) {
		this.workshop = workshop;
	}
	
	public String getFactory() {
		return factory;
	}
	
	public void setFactory(String factory) {
		this.factory = factory;
	}
	
	public List<PictureVO> getPictures() {
		return pictures;
	}
	
	public void setPictures(List<PictureVO> pictures) {
		this.pictures = pictures;
	}
}
