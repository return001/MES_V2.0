/**  
*  
*/  
package com.jimi.mes_server.entity.vo;

import com.jimi.mes_server.model.Department;
import com.jimi.mes_server.model.Role;

/**  
 * <p>Title: RoleVO</p>  
 * <p>Description: </p>  
 * <p>Copyright: Copyright (c) 2019</p>  
 * <p>Company: 惠州市几米物联技术有限公司</p>  
 * @author trjie  
 * @date 2020年4月22日
 *
 */
public class RoleVO {
	
	private Integer id;

	private Integer company;
	
	private Integer department;
	
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

	public static RoleVO fillRoleVO(Role role, Department department) {
		RoleVO roleVO = new RoleVO();
		roleVO.setNo(role.getNo());
		roleVO.setName(role.getName());
		roleVO.setDepartment(role.getDepartment());
		roleVO.setId(role.getId());
		if (department != null) {
			roleVO.setCompany(department.getCompany());
		}
		
		return roleVO;
	}
}
