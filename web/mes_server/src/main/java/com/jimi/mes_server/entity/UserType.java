package com.jimi.mes_server.entity;

import java.util.List;

import com.jimi.mes_server.model.WebUserType;

/**用户角色存储类
 * @author   HCJ
 * @date     2019年5月15日 下午2:52:46
 */
public class UserType {

	/**
	 * pcAccessList : 存储pc端所涉及的角色
	 */
	private List<WebUserType> pcAccessList;
	
	/**
	 * webTypeList : 存储web端所涉及的角色
	 */
	private List<WebUserType> webTypeList;

	public List<WebUserType> getPcAccessList() {
		return pcAccessList;
	}

	public void setPcAccessList(List<WebUserType> pcAccessList) {
		this.pcAccessList = pcAccessList;
	}

	public List<WebUserType> getWebTypeList() {
		return webTypeList;
	}

	public void setWebTypeList(List<WebUserType> webTypeList) {
		this.webTypeList = webTypeList;
	}
	
	
}
