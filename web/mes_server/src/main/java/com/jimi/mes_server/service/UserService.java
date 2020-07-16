package com.jimi.mes_server.service;

import java.util.Date;
import java.util.List;

import com.jfinal.aop.Enhancer;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Page;
import com.jfinal.plugin.activerecord.Record;
import com.jfinal.plugin.activerecord.SqlPara;
import com.jimi.mes_server.entity.Constant;
import com.jimi.mes_server.entity.SQL;
import com.jimi.mes_server.entity.UserInfoVO;
import com.jimi.mes_server.entity.vo.LUserAccountVO;
import com.jimi.mes_server.entity.vo.RoleAuthorityVO;
import com.jimi.mes_server.exception.OperationException;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.model.Authority;
import com.jimi.mes_server.model.Department;
import com.jimi.mes_server.model.LUserAccount;
import com.jimi.mes_server.model.Role;
import com.jimi.mes_server.service.base.SelectService;

/**
 * 用户业务层
 * <br>
 * <b>2018年5月29日</b>
 * @author 沫熊工作室 <a href="http://www.darhao.cc">www.darhao.cc</a>
 */
public class UserService extends SelectService {

	private static final String loginSql = "SELECT * FROM LUserAccount WHERE Name = ? AND Password = ?";

	private static final String uniqueCheckSql = "SELECT * FROM LUserAccount WHERE Name = ? AND InService = 1";

	private static final String SELECT_USER_AUTHORITY_BY_USER = "SELECT authority.* FROM authority INNER JOIN role_authority ON authority.id = role_authority.authority where role_authority.role = ?";

	private static final String BASE_SELECT_USER_INFO = "SELECT LUserAccount.Name AS name, LUserAccount.UserDes AS userDes, LUserAccount.Id AS id, LUserAccount.LoginTime AS loginTime, LUserAccount.InService AS inService, LUserAccount.LineName AS lineName, LUserAccount.MainProcess AS mainProcess, LUserAccount.Proficiency AS proficiency, LUserAccount.OtherProcess AS otherProcess, role.id AS roleId, role.name AS roleName, department.id AS departmentId, department.name AS departmentName, sop_factory.id AS companyId, sop_factory.abbreviation AS companyName FROM LUserAccount INNER JOIN role ON role.id = LUserAccount.Role INNER JOIN department ON role.department = department.id INNER JOIN sop_factory ON department.company = sop_factory.id WHERE LUserAccount.[Delete] = 0";

	private RoleService roleService = Enhancer.enhance(RoleService.class);


	/**
	 * @author HCJ
	  * 登录
	 * @param name 用户名
	 * @param password 用户密码
	 * @param isNeedUpdate 是否需要更新登录时间
	 * @date 2019年6月27日 上午9:56:19
	 */
	public LUserAccountVO login(String name, String password, Boolean isNeedUpdate) {
		LUserAccount user = LUserAccount.dao.findFirst(loginSql, name, password);
		if (user == null) {
			throw new OperationException("用户名或者密码错误");
		}
		if (!user.getInService()) {
			throw new OperationException("此用户未启用");
		}
		LUserAccountVO userVO = null;
		if (user.getRole() != null) {
			Role role = Role.dao.findById(user.getRole());
			if (role == null) {
				throw new OperationException("用户的角色不存在");
			}
			Department department = Department.dao.findById(role.getDepartment());
			List<Authority> authorities = Authority.dao.find(SELECT_USER_AUTHORITY_BY_USER, role.getId());
			userVO = LUserAccountVO.filLUserAccountVO(user, role, authorities, department);
		} else {
			throw new OperationException("用户未绑定角色");

		}
		if (isNeedUpdate) {
			user.setLoginTime(new Date());
			user.update();
		}
		return userVO;
	}


	public boolean add(LUserAccount user) {
		checkUser(user);
		if (user.getPassword() == null || user.getPassword().trim().isEmpty()) {
			throw new OperationException("密码不能为空");
		}
		if (LUserAccount.dao.find(uniqueCheckSql, user.getName()).size() != 0) {
			throw new OperationException("用户名已存在");
		}
		user.setUserType("&0301");
		user.setLoginTime(new Date());
		Role role = Role.dao.findById(user.getRole());
		if (role == null) {
			throw new OperationException("用户角色不存在");
		}
		return user.save();
	}


	public boolean update(LUserAccount user) {
		checkUser(user);
		if (user.getPassword() == null) {
			user.remove("Password");
		}
		if (user.getInService() == null) {
			user.remove("InService");
		}
		if (user.getLoginStatus() == null) {
			user.remove("LoginStatus");
		}
		if (user.getLoginTime() == null) {
			user.remove("LoginTime");
		}
		LUserAccount temp = LUserAccount.dao.findFirst(uniqueCheckSql, user.getName());
		if (temp != null && !temp.getId().equals(user.getId())) {
			throw new OperationException("用户名已存在");
		}

		return user.update();
	}


	public LUserAccount validate(String name, String password) {
		LUserAccount user = LUserAccount.dao.findFirst(SQL.SELECT_USER_BY_NAME_PASSWORD, name, password);
		if (user == null) {
			throw new OperationException("用户名或者密码错误");
		}
		if (!user.getInService()) {
			throw new OperationException("此用户未启用");
		}
		return user;
	}


	public Page<Record> getUserIdAndName(String userName) {
		SqlPara sqlPara = new SqlPara();
		sqlPara.setSql(SQL.SELECT_USER_NAME_ID_BY_NAME);
		sqlPara.addPara("%" + userName + "%");
		Page<Record> page = Db.paginate(Constant.DEFAULT_PAGE_NUM, Constant.DEFAULT_PAGE_SIZE, sqlPara);
		for (Record record : page.getList()) {
			System.err.println(record);
		}
		return page;
	}


	private void checkUser(LUserAccount user) {

		if (user.getName() == null || user.getName().trim().isEmpty()) {
			throw new ParameterException("用户名不能为空");
		}
		if (user.getUserDes() == null || user.getUserDes().trim().isEmpty()) {
			throw new ParameterException("用户描述不能为空");
		}
	}


	/**@author HCJ
	 * 根据用户类型设置表格删除权限
	 * @param user 用户信息
	 * @date 2019年6月28日 上午10:40:20
	 */
	/*private LUserAccount setDeletePermission(LUserAccount user) {
		if (user.getWebUserType().equals(Constant.SUPER_ADMIN_USERTYPE)) {
			user.setDeletePermission(Constant.SUPER_ADMIN_DELETEPERMISSION);
			return user;
		}
		if (user.getDeletePermission() == null) {
			user.setDeletePermission(Constant.ORDINARY_DELETEPERMISSION);
			return user;
		} else {
			return user;
		}
	}*/

	public boolean updatePassword(String orgPwd, String curPwd, int userId) {
		LUserAccount user = LUserAccount.dao.findById(userId);
		if (user == null || !user.getInService()) {
			throw new OperationException("当前用户不存在或未启用");
		}
		if (!orgPwd.equals(user.getPassword())) {
			throw new OperationException("用户密码错误");
		}
		user.setPassword(curPwd);
		return user.update();
	}


	public Page<UserInfoVO> select(Integer pageNo, Integer pageSize, String name, String userDes, String otherProcess, Boolean inService, String mainProcess, String proficiency, String lineName, Integer company, Integer department, String roleName) {
		SqlPara sqlPara = new SqlPara();
		StringBuilder sb = new StringBuilder(BASE_SELECT_USER_INFO);
		if (name != null) {
			sb.append(" AND LUserAccount.Name = ?");
			sqlPara.addPara(name);
		}
		if (userDes != null) {
			sb.append(" AND LUserAccount.UserDes like ?");
			sqlPara.addPara("%" + userDes + "%");
		}
		if (roleName != null) {
			sb.append(" AND role.name = ?");
			sqlPara.addPara(roleName);
		}
		if (otherProcess != null) {
			sb.append(" AND LUserAccount.OtherProcess like ?");
			sqlPara.addPara("%" + otherProcess + "%");
		}
		if (mainProcess != null) {
			sb.append(" AND LUserAccount.MainProcess = ?");
			sqlPara.addPara(mainProcess);
		}
		if (proficiency != null) {
			sb.append(" AND LUserAccount.Proficiency = ?");
			sqlPara.addPara(proficiency);
		}
		if (inService != null) {
			sb.append(" AND LUserAccount.InService = ?");
			sqlPara.addPara(inService);
		}
		if (lineName != null) {
			sb.append(" AND LUserAccount.LineName = ?");
			sqlPara.addPara(lineName);
		}
		if (department != null) {
			sb.append(" AND department.id = ?");
			sqlPara.addPara(department);
		}
		if (company != null) {
			sb.append(" AND sop_factory.id = ?");
			sqlPara.addPara(company);
		}
		sqlPara.setSql(sb.toString());
		Page<Record> page = Db.paginate(pageNo, pageSize, sqlPara);
		List<UserInfoVO> userInfoVOs = UserInfoVO.fillList(page.getList());
		Page<UserInfoVO> userInfoPage = new Page<>(userInfoVOs, page.getPageNumber(), page.getPageSize(), page.getTotalPage(), page.getTotalRow());
		return userInfoPage;
	}


	public boolean delete(Integer id) {
		LUserAccount lUserAccount = LUserAccount.dao.findById(id);
		if (lUserAccount == null) {
			throw new OperationException("当前用户不存在");
		}
		Db.update(SQL.DELETE_USER_BY_ID, id);
		return true;
	}


	public RoleAuthorityVO getAuthority(Integer user) {
		LUserAccount lUserAccount = LUserAccount.dao.findById(user);
		if (lUserAccount == null) {
			throw new OperationException("当前用户不存在");
		}
		if (lUserAccount.getRole() == null) {
			throw new OperationException("当前用户没有绑定角色");
		}
		return roleService.getAuthority(lUserAccount.getRole());
	}
}
