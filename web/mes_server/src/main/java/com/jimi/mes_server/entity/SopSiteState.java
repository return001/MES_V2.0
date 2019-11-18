package com.jimi.mes_server.entity;


/**SOP站点状态枚举类
 * @author   HCJ
 * @date     2019年11月14日 下午4:49:14
 */
public enum SopSiteState {

	UNCONFIRMED(0, "未确认"), 
	PASS(1, "审核通过"), 
	FAIL(2, "审核不通过");

	private Integer id;

	private String name;


	private SopSiteState(Integer id, String name) {
		this.id = id;
		this.name = name;
	}


	public Integer getId() {
		return id;
	}


	public String getName() {
		return name;
	}


	public static String getNameById(Integer id) {
		for (SopSiteState sopSiteState : SopSiteState.values()) {
			if (sopSiteState.getId().equals(id)) {
				return sopSiteState.getName();
			}
		}
		return null;
	}


	public static Integer getIdByName(String name) {
		for (SopSiteState sopSiteState : SopSiteState.values()) {
			if (sopSiteState.getName().equals(name)) {
				return sopSiteState.getId();
			}
		}
		return null;
	}
}
