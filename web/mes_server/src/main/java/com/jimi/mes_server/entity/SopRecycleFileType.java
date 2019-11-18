package com.jimi.mes_server.entity;

/**SOP回收文件类型枚举类
 * @author   HCJ
 * @date     2019年10月24日 下午1:54:16
 */
public enum SopRecycleFileType {

	FILE(0, "文件"), 
	NOTICE(1, "通知"), 
	FILE_AND_NOTICE(2, "文件和通知");

	private Integer id;

	private String name;


	private SopRecycleFileType(Integer id, String name) {
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
		for (SopRecycleFileType type : SopRecycleFileType.values()) {
			if (type.getId().equals(id)) {
				return type.getName();
			}
		}
		return null;
	}


	public static Integer getIdByName(String name) {
		for (SopRecycleFileType type : SopRecycleFileType.values()) {
			if (type.getName().equals(name)) {
				return type.getId();
			}
		}
		return null;
	}

}
