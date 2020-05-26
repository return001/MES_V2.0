/**  
*  
*/  
package com.jimi.mes_server.entity.vo;

import java.util.ArrayList;
import java.util.List;

import com.jfinal.plugin.activerecord.Record;

/**  
 * <p>Title: RoleInfoVO</p>  
 * <p>Description: </p>  
 * <p>Copyright: Copyright (c) 2019</p>  
 * <p>Company: 惠州市几米物联技术有限公司</p>  
 * @author trjie  
 * @date 2020年4月23日
 *
 */
public class RoleInfoVO {


	private Integer id;

	private Integer company;
	
	private String companyName;
	
	private Integer department;
	
	private String departmentName;
	
	private String no;
	
	private String name;

	public Integer getCompany() {
		return company;
	}

	public void setCompany(Integer company) {
		this.company = company;
	}

	public Integer getDepartment() {
		return department;
	}

	public void setDepartment(Integer department) {
		this.department = department;
	}

	public String getNo() {
		return no;
	}

	public void setNo(String no) {
		this.no = no;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}
	
	public Integer getId() {
		return id;
	}

	public void setId(Integer id) {
		this.id = id;
	}

	public String getCompanyName() {
		return companyName;
	}

	public void setCompanyName(String companyName) {
		this.companyName = companyName;
	}

	public String getDepartmentName() {
		return departmentName;
	}

	public void setDepartmentName(String departmentName) {
		this.departmentName = departmentName;
	}

	public static List<RoleInfoVO> fillList(List<Record> records) {
		List<RoleInfoVO> roleInfoVOs = new ArrayList<RoleInfoVO>(records.size());
		for (Record record : records) {
			RoleInfoVO roleInfoVO = new RoleInfoVO();
			roleInfoVO.setNo(record.getStr("roleNo"));
			roleInfoVO.setName(record.getStr("roleName"));
			roleInfoVO.setDepartment(record.getInt("departmentId"));
			roleInfoVO.setId(record.getInt("roleId"));
			roleInfoVO.setDepartmentName(record.getStr("departmentName"));
			roleInfoVO.setCompany(record.getInt("companyId"));
			roleInfoVO.setCompanyName(record.getStr("companyName"));
			roleInfoVOs.add(roleInfoVO);
		}
		return roleInfoVOs;
	}
}
