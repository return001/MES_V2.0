package com.jimi.mes_server.entity;

import java.util.List;

import com.jfinal.plugin.activerecord.Record;

/**岗位信息
 * @author   HCJ
 * @date     2020年4月1日 下午3:59:10
 */
public class PostInfo {

	private String siteName;

	private Integer siteId;

	private String processId;

	private Integer processName;

	private String userName;

	private Integer userId;


	public String getSiteName() {
		return siteName;
	}


	public void setSiteName(String siteName) {
		this.siteName = siteName;
	}


	public Integer getSiteId() {
		return siteId;
	}


	public void setSiteId(Integer siteId) {
		this.siteId = siteId;
	}


	public String getProcessId() {
		return processId;
	}


	public void setProcessId(String processId) {
		this.processId = processId;
	}


	public Integer getProcessName() {
		return processName;
	}


	public void setProcessName(Integer processName) {
		this.processName = processName;
	}


	public String getUserName() {
		return userName;
	}


	public void setUserName(String userName) {
		this.userName = userName;
	}


	public Integer getUserId() {
		return userId;
	}


	public void setUserId(Integer userId) {
		this.userId = userId;
	}


	public static List<Record> getPostInfo(List<Record> tempRecords, List<Record> target) {
		if (target != null && !target.isEmpty()) {
			if (tempRecords == null || tempRecords.isEmpty()) {
				return target;
			}
			for (Record record : target) {
				for (Record tempRecord : tempRecords) {
					if (tempRecord.getInt("siteId").equals(record.getInt("siteId")) && tempRecord.getInt("processId").equals(record.getInt("processId"))) {
						record.setColumns(tempRecord);
					}
				}
			}
			return target;
		}
		return null;
	}
}
