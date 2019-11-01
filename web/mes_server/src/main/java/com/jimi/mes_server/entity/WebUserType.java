package com.jimi.mes_server.entity;

/**网页端用户类型枚举类
 * @author   HCJ
 * @date     2019年10月25日 上午10:29:07
 */
public enum WebUserType {

	ENGINEER(1, "engineer"), 
	SUPERADMIN(2, "SuperAdmin"), 
	OPARATOR(3, "operator"), 
	ORDINARYUSER(4, "ordinaryUser"), 
	ADMINISTRATION(5, "administration"), 
	SCHEDULINGSZPC(7, "schedulingSZPC"), 
	SCHEDULINGJMPMC(8, "schedulingJMPMC"), 
	SOPMANAGER(9, "SopManager"), 
	SOPREVIEWER(11, "SopReviewer"),
	SOPQCCONFIRMER(12, "SopQcConfirmer"), 
	CASUALWORKER(13, "casualWorker");

	private Integer id;

	private String name;


	private WebUserType(Integer id, String name) {
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
		for (WebUserType webUserType : WebUserType.values()) {
			if (webUserType.getId().equals(id)) {
				return webUserType.getName();
			}
		}
		return null;
	}


	public static Integer getIdByName(String name) {
		for (WebUserType webUserType : WebUserType.values()) {
			if (webUserType.getName().equals(name)) {
				return webUserType.getId();
			}
		}
		return null;
	}
}
