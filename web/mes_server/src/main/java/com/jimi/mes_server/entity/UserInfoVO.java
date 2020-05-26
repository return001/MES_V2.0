/**  
*  
*/
package com.jimi.mes_server.entity;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import com.jfinal.plugin.activerecord.Record;

/**  
 * <p>Title: UserInfoVO</p>  
 * <p>Description: </p>  
 * <p>Copyright: Copyright (c) 2019</p>  
 * <p>Company: 惠州市几米物联技术有限公司</p>  
 * @author trjie  
 * @date 2020年4月23日
 *
 */
public class UserInfoVO {

	private String mainProcess;

	private Date loginTime;

	private Integer role;

	private String roleName;

	private Integer company;

	private String companyName;

	private Integer department;

	private String departmentName;

	private String otherProcess;

	private String name;

	private String lineName;

	private String proficiency;

	private boolean inService;

	private String userDes;

	private int id;


	public String getMainProcess() {
		return mainProcess;
	}


	public void setMainProcess(String mainProcess) {
		this.mainProcess = mainProcess;
	}


	public Date getLoginTime() {
		return loginTime;
	}


	public void setLoginTime(Date loginTime) {
		this.loginTime = loginTime;
	}


	public Integer getRole() {
		return role;
	}


	public void setRole(Integer role) {
		this.role = role;
	}


	public String getRoleName() {
		return roleName;
	}


	public void setRoleName(String roleName) {
		this.roleName = roleName;
	}


	public String getOtherProcess() {
		return otherProcess;
	}


	public void setOtherProcess(String otherProcess) {
		this.otherProcess = otherProcess;
	}


	public String getName() {
		return name;
	}


	public void setName(String name) {
		this.name = name;
	}


	public String getLineName() {
		return lineName;
	}


	public void setLineName(String lineName) {
		this.lineName = lineName;
	}


	public String getProficiency() {
		return proficiency;
	}


	public void setProficiency(String proficiency) {
		this.proficiency = proficiency;
	}


	public boolean isInService() {
		return inService;
	}


	public void setInService(boolean inService) {
		this.inService = inService;
	}


	public String getUserDes() {
		return userDes;
	}


	public void setUserDes(String userDes) {
		this.userDes = userDes;
	}


	public int getId() {
		return id;
	}


	public void setId(int id) {
		this.id = id;
	}


	public Integer getCompany() {
		return company;
	}


	public void setCompany(Integer company) {
		this.company = company;
	}


	public String getCompanyName() {
		return companyName;
	}


	public void setCompanyName(String companyName) {
		this.companyName = companyName;
	}


	public Integer getDepartment() {
		return department;
	}


	public void setDepartment(Integer department) {
		this.department = department;
	}


	public String getDepartmentName() {
		return departmentName;
	}


	public void setDepartmentName(String departmentName) {
		this.departmentName = departmentName;
	}


	public static List<UserInfoVO> fillList(List<Record> records) {
		List<UserInfoVO> userInfoVOs = new ArrayList<UserInfoVO>(records.size());
		for (Record record : records) {
			UserInfoVO userInfoVO = new UserInfoVO();
			userInfoVO.setName(record.getStr("name"));
			userInfoVO.setUserDes(record.getStr("userDes"));
			userInfoVO.setId(record.getInt("id"));
			userInfoVO.setLoginTime(record.getDate("loginTime"));
			userInfoVO.setInService(record.getBoolean("inService"));
			userInfoVO.setLineName(record.getStr("lineName"));
			userInfoVO.setMainProcess(record.getStr("mainProcess"));
			userInfoVO.setOtherProcess(record.getStr("otherProcess"));
			userInfoVO.setProficiency(record.getStr("proficiency"));
			userInfoVO.setRole(record.getInt("roleId"));
			userInfoVO.setRoleName(record.getStr("roleName"));
			userInfoVO.setDepartment(record.getInt("departmentId"));
			userInfoVO.setDepartmentName(record.getStr("departmentName"));
			userInfoVO.setCompany(record.getInt("companyId"));
			userInfoVO.setCompanyName(record.getStr("companyName"));
			userInfoVOs.add(userInfoVO);
		}
		return userInfoVOs;
	}

}
