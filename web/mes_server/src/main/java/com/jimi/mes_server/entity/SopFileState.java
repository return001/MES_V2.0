package com.jimi.mes_server.entity;

/**SOP文件状态枚举类
 * @author   HCJ
 * @date     2019年10月24日 下午1:54:16
 */
public enum SopFileState {

	WAITREVIEW_STATE(0, "待审核"), REVIEWED_STATE(1, "已审核"), PLAYING_STATE(2, "播放中"), INVALID_STATE(3, "已作废");

	private Integer id;

	private String name;


	private SopFileState(Integer id, String name) {
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
		for (SopFileState sopFileState : SopFileState.values()) {
			if (sopFileState.getId().equals(id)) {
				return sopFileState.getName();
			}
		}
		return null;
	}


	public static Integer getIdByName(String name) {
		for (SopFileState sopFileState : SopFileState.values()) {
			if (sopFileState.getName().equals(name)) {
				return sopFileState.getId();
			}
		}
		return null;
	}

}
