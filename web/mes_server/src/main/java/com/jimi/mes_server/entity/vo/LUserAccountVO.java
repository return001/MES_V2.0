package com.jimi.mes_server.entity.vo;

import com.jimi.mes_server.model.LUserAccount;

public class LUserAccountVO extends LUserAccount {

	/**
	 * serialVersionUID : TODO
	 */
	private static final long serialVersionUID = 5451131369780221420L;

	private String typeName;

	public String getTypeName() {
		return typeName;
	}

	public void setTypeName(String typeName) {
		this.typeName = typeName;
	}

	public LUserAccountVO(LUserAccount user, String typeName) {
		this.setDeletePermission(user.getDeletePermission());
		this.setInService(user.getInService());
		this.setLoginStatus(user.getLoginStatus());
		this.setLoginTime(user.getLoginTime());
		this.setName(user.getName());
		this.setPassword(user.getPassword());
		this.setUserDes(user.getUserDes());
		this.setUserType(user.getUserType());
		this.setWebUserType(user.getWebUserType());
		this.setTypeName(typeName);
		this.set("typeName", getTypeName());
	}

}
