package com.jimi.mes_server.model.base;

import com.jfinal.plugin.activerecord.Model;
import com.jfinal.plugin.activerecord.IBean;

/**
 * Generated by JFinal, do not modify this file.
 */
@SuppressWarnings({"serial", "unchecked"})
public abstract class BaseRoleAuthority<M extends BaseRoleAuthority<M>> extends Model<M> implements IBean {

	public M setId(java.lang.Integer id) {
		set("id", id);
		return (M)this;
	}
	
	public java.lang.Integer getId() {
		return getInt("id");
	}

	public M setAuthority(java.lang.Integer authority) {
		set("authority", authority);
		return (M)this;
	}
	
	public java.lang.Integer getAuthority() {
		return getInt("authority");
	}

	public M setRole(java.lang.Integer role) {
		set("role", role);
		return (M)this;
	}
	
	public java.lang.Integer getRole() {
		return getInt("role");
	}

}