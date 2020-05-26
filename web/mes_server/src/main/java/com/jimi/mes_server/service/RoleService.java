/**  
*  
*/
package com.jimi.mes_server.service;

import java.util.ArrayList;
import java.util.List;

import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Record;
import com.jfinal.plugin.activerecord.SqlPara;
import com.jimi.mes_server.entity.AuthorityBO;
import com.jimi.mes_server.entity.vo.AuthorityVO;
import com.jimi.mes_server.entity.vo.RoleAuthorityVO;
import com.jimi.mes_server.entity.vo.RoleInfoVO;
import com.jimi.mes_server.exception.OperationException;
import com.jimi.mes_server.model.Authority;
import com.jimi.mes_server.model.Department;
import com.jimi.mes_server.model.LUserAccount;
import com.jimi.mes_server.model.Role;
import com.jimi.mes_server.model.RoleAuthority;

/**  
 * <p>Title: RoleService</p>  
 * <p>Description: </p>  
 * <p>Copyright: Copyright (c) 2019</p>  
 * <p>Company: 惠州市几米物联技术有限公司</p>  
 * @author trjie  
 * @date 2020年4月20日
 *
 */
public class RoleService {

	private static final String DELETE_ALL_AUTHORITY_BY_ROLE = "DELETE FROM role_authority WHERE role_authority.role = ?";

	private static final String BASE_SELECT_AUTHORITY = "SELECT * FROM authority WHERE 1 = 1 ";

	private static final String SELECT_AUTHORITY_BY_ROLE = "SELECT authority.* FROM authority INNER JOIN role_authority ON role_authority.authority = authority.id WHERE role_authority.role = ?";

	private static final String SELECT_USER_BY_ROLE = "SELECT LUserAccount.* FROM LUserAccount INNER JOIN role ON role.id = LUserAccount.Role WHERE role.id = ?";

	private static final String SELECT_ROLE_BY_NO_AND_DEPARTMENT = "SELECT * FROM role WHERE no = ? AND department = ?";

	private static final String SELECT_ROLE_BY_NAME_DEPARTMENT = "SELECT * FROM role WHERE name = ? AND department = ?";

	private static final String BASE_SELECT_ROLE_INFO = "SELECT sop_factory.id AS companyId, sop_factory.abbreviation AS companyName, department.id AS departmentId, department.name AS departmentName, role.id AS roleId, role.name AS roleName, role.no AS roleNo FROM sop_factory INNER JOIN department ON department.company = sop_factory.id INNER JOIN role ON department.id = role.department WHERE role.is_typical = 1 ";

	private int batchSize = 1500;


	public void update(Integer department, String name, String no, List<AuthorityBO> authorityBOs, Integer id) {
		Role role = Role.dao.findById(id);
		if (role == null) {
			throw new OperationException("角色类型不存在");
		}
		if (name != null) {
			role.setName(name);
		}
		if (no != null) {
			role.setNo(no);
		}
		if (authorityBOs != null) {
			List<Authority> authorities = checkAndReturnAuthority(authorityBOs);
			if (authorities.isEmpty()) {
				throw new OperationException("权限配置存在错误");
			}
			List<RoleAuthority> roleAuthorities = new ArrayList<>(authorities.size());
			for (Authority authority : authorities) {
				RoleAuthority roleAuthority = new RoleAuthority();
				roleAuthority.setRole(role.getId()).setAuthority(authority.getId());
				roleAuthorities.add(roleAuthority);
			}
			Db.delete(DELETE_ALL_AUTHORITY_BY_ROLE, role.getId());
			Db.batchSave(roleAuthorities, batchSize);
		}
		role.update();
	}


	/**
	 * 
	 * <p>Description: 删除角色类型<p>
	 * @return
	 * @exception
	 * @author trjie
	 * @Time 2020年4月23日
	 */
	public void delete(Integer id) {
		Role role = Role.dao.findById(id);
		if (role == null) {
			throw new OperationException("角色类型不存在");
		}
		LUserAccount lUserAccount = LUserAccount.dao.findFirst(SELECT_USER_BY_ROLE, role.getId());
		if (lUserAccount != null) {
			throw new OperationException("角色类型已被用户绑定，无法删除");
		}
		Db.delete(DELETE_ALL_AUTHORITY_BY_ROLE, role.getId());
		role.delete();
	}


	public void add(Integer department, String name, String no, List<AuthorityBO> authorityBOs) {
		Department departmentEntity = Department.dao.findById(department);
		if (departmentEntity == null) {
			throw new OperationException("部门不存在");
		}
		Role roleByName = Role.dao.findFirst(SELECT_ROLE_BY_NAME_DEPARTMENT, name, department);
		if (roleByName != null) {
			throw new OperationException("角色名称存在");

		}
		Role roleByNo = Role.dao.findFirst(SELECT_ROLE_BY_NO_AND_DEPARTMENT, no, department);
		if (roleByNo != null) {
			throw new OperationException("角色编号存在");

		}
		List<Authority> authorities = checkAndReturnAuthority(authorityBOs);
		if (authorities.isEmpty()) {
			throw new OperationException("权限配置存在错误");
		}
		List<RoleAuthority> roleAuthorities = new ArrayList<>(authorities.size());
		Role role = new Role();
		role.setDepartment(department).setName(name).setNo(no).setIsTypical(true).save();
		for (Authority authority : authorities) {
			RoleAuthority roleAuthority = new RoleAuthority();
			roleAuthority.setRole(role.getId()).setAuthority(authority.getId());
			roleAuthorities.add(roleAuthority);
		}
		Db.batchSave(roleAuthorities, batchSize);
	}


	public List<RoleInfoVO> get(Integer company, Integer department, String name, String no) {
		SqlPara sqlPara = new SqlPara();
		StringBuilder sb = new StringBuilder(BASE_SELECT_ROLE_INFO);
		if (company != null) {
			sb.append(" AND sop_factory.id = ?");
			sqlPara.addPara(company);
		}
		if (department != null) {
			sb.append(" AND department.id = ?");
			sqlPara.addPara(department);
		}
		if (name != null) {
			sb.append(" AND role.name = ?");
			sqlPara.addPara(name);
		}
		if (no != null) {
			sb.append(" AND role.no = ?");
			sqlPara.addPara(no);
		}
		sqlPara.setSql(sb.toString());
		List<Record> records = Db.find(sqlPara);
		List<RoleInfoVO> roleInfoVOs = RoleInfoVO.fillList(records);
		return roleInfoVOs;
	}


	public RoleAuthorityVO getAuthority(Integer role) {

		List<Authority> authorities = Authority.dao.find(SELECT_AUTHORITY_BY_ROLE, role);
		RoleAuthorityVO roleAuthorityVO = new RoleAuthorityVO();
		roleAuthorityVO.setWebAuthorities(AuthorityVO.fillWebList(authorities));
		roleAuthorityVO.setPcAuthorities(AuthorityVO.fillPcList(authorities));
		return roleAuthorityVO;
	}


	public LUserAccount setAtypicalRoleUser(Integer user, List<AuthorityBO> authorityBOs) {
		LUserAccount lUserAccount = LUserAccount.dao.findById(user);

		if (lUserAccount == null) {
			throw new OperationException("用户不存在");
		}
		Role role = Role.dao.findById(lUserAccount.getRole());
		List<Authority> authorities = checkAndReturnAuthority(authorityBOs);
		if (authorities.isEmpty()) {
			throw new OperationException("权限配置存在错误");
		}
		List<RoleAuthority> roleAuthorities = new ArrayList<>(authorities.size());
		if (role.getIsTypical()) {
			role = new Role();
			role.setDepartment(role.getDepartment()).setName(role.getName()).setNo(role.getNo()).setIsTypical(false).save();
		} else {
			Db.delete(DELETE_ALL_AUTHORITY_BY_ROLE, role.getId());
		}
		for (Authority authority : authorities) {
			RoleAuthority roleAuthority = new RoleAuthority();
			roleAuthority.setRole(role.getId()).setAuthority(authority.getId());
			roleAuthorities.add(roleAuthority);
		}
		Db.batchSave(roleAuthorities, batchSize);
		lUserAccount.setRole(role.getId()).update();
		return lUserAccount;
	}


	/**
	 * 
	 * <p>Description: 检测权限json串并返回相应的权限实体<p>
	 * @return List<Authority>
	 * @exception
	 * @author trjie
	 * @Time 2020年4月23日
	 */
	private List<Authority> checkAndReturnAuthority(List<AuthorityBO> authorityBOs) {
		List<Authority> authorities = new ArrayList<Authority>();
		SqlPara sqlPara = new SqlPara();

		for (AuthorityBO authorityBO : authorityBOs) {
			StringBuilder sb = new StringBuilder(BASE_SELECT_AUTHORITY);
			if (!authorityBO.getSystem().equals("*")) {
				sb.append(" AND system = ?");
				sqlPara.addPara(authorityBO.getSystem());
			}
			if (!authorityBO.getModel().equals("*")) {
				sb.append(" AND model = ?");
				sqlPara.addPara(authorityBO.getModel());
			}
			if (!authorityBO.getPage().equals("*")) {
				sb.append(" AND page = ?");
				sqlPara.addPara(authorityBO.getPage());
			}
			if (!authorityBO.getOperation().equals("*")) {
				sb.append(" AND operation = ?");
				sqlPara.addPara(authorityBO.getOperation());
			}
			sqlPara.setSql(sb.toString());
			List<Authority> waitCheckAuthorities = Authority.dao.find(sqlPara);
			if (waitCheckAuthorities.isEmpty()) {
				throw new OperationException("权限配置存在错误");
			}
			authorities.addAll(waitCheckAuthorities);
			sqlPara.clear();
		}
		return authorities;
	}
}
