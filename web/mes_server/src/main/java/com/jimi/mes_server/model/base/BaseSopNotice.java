package com.jimi.mes_server.model.base;

import com.jfinal.plugin.activerecord.Model;
import com.jfinal.plugin.activerecord.IBean;

/**
 * Generated by JFinal, do not modify this file.
 */
@SuppressWarnings({"serial", "unchecked"})
public abstract class BaseSopNotice<M extends BaseSopNotice<M>> extends Model<M> implements IBean {

	public M setId(java.lang.Integer id) {
		set("id", id);
		return (M)this;
	}
	
	public java.lang.Integer getId() {
		return getInt("id");
	}

	public M setTitle(java.lang.String title) {
		set("title", title);
		return (M)this;
	}
	
	public java.lang.String getTitle() {
		return getStr("title");
	}

	public M setContent(java.lang.String content) {
		set("content", content);
		return (M)this;
	}
	
	public java.lang.String getContent() {
		return getStr("content");
	}

	public M setStartTime(java.util.Date startTime) {
		set("start_time", startTime);
		return (M)this;
	}
	
	public java.util.Date getStartTime() {
		return get("start_time");
	}

	public M setEndTime(java.util.Date endTime) {
		set("end_time", endTime);
		return (M)this;
	}
	
	public java.util.Date getEndTime() {
		return get("end_time");
	}

	public M setIsAllSite(java.lang.Boolean isAllSite) {
		set("is_all_site", isAllSite);
		return (M)this;
	}
	
	public java.lang.Boolean getIsAllSite() {
		return get("is_all_site");
	}

}
