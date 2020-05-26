/**  
*  
*/
package com.jimi.mes_server.service;

import java.util.List;

import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Record;
import com.jfinal.plugin.activerecord.SqlPara;
import com.jimi.mes_server.entity.vo.DepartmentInfoVO;
import com.jimi.mes_server.exception.OperationException;
import com.jimi.mes_server.model.Department;
import com.jimi.mes_server.model.Role;
import com.jimi.mes_server.model.SopFactory;

/**  
 * <p>Title: DepartmentService</p>  
 * <p>Description: </p>  
 * <p>Copyright: Copyright (c) 2019</p>  
 * <p>Company: 惠州市几米物联技术有限公司</p>  
 * @author trjie  
 * @date 2020年4月20日
 *
 */

public class DepartmentService {

	private static final String SELECT_DEPARTMENT_BY_COMPNAY_AND_NAME = "SELECT department.* FROM department INNER JOIN sop_factory ON department.company = sop_factory.id WHERE department.name = ? AND department.company = ? AND department.is_enabled = 1";;

	private static final String SELECT_ROLE_BY_DEPARTMENT = "SELECT * FROM role WEHRE department = ?";

	private static final String BASE_SELECT_DEPARTMENT_INFO = "SELECT sop_factory.id AS companyId, sop_factory.abbreviation AS companyName, department.id AS departmentId, department.name AS departmentName, department.[no] AS departmentNo FROM sop_factory INNER JOIN department ON department.company = sop_factory.id WHERE department.is_enabled = 1 ";


	public void update(Integer company, String no, String name, Integer id) {

		SopFactory sopFactory = SopFactory.dao.findById(company);
		if (sopFactory == null) {
			throw new OperationException("公司不存在，请检查");
		}
		Department department = Department.dao.findById(id);
		if (department == null) {
			throw new OperationException("部门不存在，请检查");
		}
		department.setNo(no).setName(name).setCompany(company).update();
	}


	public void add(Integer company, String no, String name) {

		SopFactory sopFactory = SopFactory.dao.findById(company);
		if (sopFactory == null) {
			throw new OperationException("公司不存在，请检查");
		}
		Department department = Department.dao.findFirst(SELECT_DEPARTMENT_BY_COMPNAY_AND_NAME, name, company);
		if (department != null) {
			throw new OperationException("部门已存在，请检查");
		}
		department = new Department();
		department.setNo(no).setName(name).setCompany(company).setIsEnabled(true).save();
	}


	public void delete(Integer id) {

		Department department = Department.dao.findById(id);
		if (department != null) {
			Role role = Role.dao.findFirst(SELECT_ROLE_BY_DEPARTMENT, department.getId());
			if (role != null) {
				throw new OperationException("部门已被角色类型绑定，无法删除，请检查");
			}
			department.setIsEnabled(false).update();
		}
	}


	public List<DepartmentInfoVO> get(Integer company, String no, String name) {

		SqlPara sqlPara = new SqlPara();
		StringBuilder sb = new StringBuilder(BASE_SELECT_DEPARTMENT_INFO);
		if (company != null) {
			sb.append(" AND sop_factory.id = ?");
			sqlPara.addPara(company);
		}
		if (name != null) {
			sb.append(" AND department.name = ?");
			sqlPara.addPara(name);
		}
		if (no != null) {
			sb.append(" AND department.no = ?");
			sqlPara.addPara(no);
		}
		sqlPara.setSql(sb.toString());
		List<Record> records = Db.find(sqlPara);
		List<DepartmentInfoVO> departmentInfoVOs = DepartmentInfoVO.fillList(records);
		return departmentInfoVOs;
	}
}
