package com.jimi.mes_server.service;

import java.util.Date;
import java.util.List;

import com.jimi.mes_server.entity.Constant;
import com.jimi.mes_server.entity.SQL;
import com.jimi.mes_server.entity.UserType;
import com.jimi.mes_server.entity.vo.LUserAccountVO;
import com.jimi.mes_server.exception.OperationException;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.model.LUserAccount;
import com.jimi.mes_server.model.WebUserType;
import com.jimi.mes_server.service.base.SelectService;
import com.jimi.mes_server.util.ResultUtil;

/**
 * 用户业务层
 * <br>
 * <b>2018年5月29日</b>
 * @author 沫熊工作室 <a href="http://www.darhao.cc">www.darhao.cc</a>
 */
public class UserService extends SelectService{

	private static final String loginSql = "SELECT * FROM LUserAccount WHERE Name = ? AND Password = ?";
	private static final String userTypeSql = "SELECT * FROM WebUserType WHERE  TypeId = ?";
	private static final String uniqueCheckSql = "SELECT * FROM LUserAccount WHERE Name = ?";


	/**@author HCJ
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
		if (isNeedUpdate) {
			user.setLoginTime(new Date());
			user.update();
		}
		LUserAccountVO userVO = new LUserAccountVO(user, getTypeName(user.getWebUserType()));
		return userVO;
	}


	public boolean add(LUserAccount user, LUserAccountVO tokenUser) {
		checkUser(user);
		if (user.getPassword() == null || user.getPassword().trim().isEmpty()) {
			throw new OperationException("密码不能为空");
		}
		if (LUserAccount.dao.find(uniqueCheckSql, user.getName()).size() != 0) {
			throw new OperationException("用户名已存在");
		}
		user.setLoginTime(new Date());
		if (Constant.SUPER_ADMIN_USERTYPE.equals(user.getWebUserType()) && !Constant.SUPER_ADMIN_USERTYPE.equals(tokenUser.getWebUserType())) {
			throw new OperationException("权限不足以修改为超级管理员");
		}
		return setDeletePermission(user).save();
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
		LUserAccount originalUser = LUserAccount.dao.findById(user.getId());
		if (Constant.SUPER_ADMIN_USERTYPE.equals(user.getWebUserType()) && !Constant.SUPER_ADMIN_USERTYPE.equals(originalUser.getWebUserType())) {
			throw new OperationException("权限不足以修改为超级管理员");
		}
		return setDeletePermission(user).update();
	}


	public ResultUtil getUserType() {
		UserType type = new UserType();
		List<WebUserType> webTypeList = WebUserType.dao.find(SQL.SELECT_WEBUSERTYPE);
		if (webTypeList == null || webTypeList.isEmpty()) {
			throw new OperationException("用户角色不存在");
		}
		List<WebUserType> pcAccessList = webTypeList;
		type.setPcAccessList(pcAccessList);
		type.setWebTypeList(webTypeList);
		return ResultUtil.succeed(type);
	}


	public void validate(String name, String password) {
		LUserAccount user = LUserAccount.dao.findFirst(SQL.SELECT_USER_BY_NAME_PASSWORD, name, password);
		if (user == null) {
			throw new OperationException("用户名或者密码错误");
		}
		if (!user.getInService()) {
			throw new OperationException("此用户未启用");
		}
	}


	public String getTypeName(Integer typeId) {
		WebUserType webUserType = WebUserType.dao.findFirst(SQL.SELECT_TYPENAME_BY_TYPEID, typeId);
		if(webUserType == null) {
			throw new OperationException("不存在此用户类型");
		}
		return webUserType.getTypeName();
	}


	private void checkUser(LUserAccount user) {
		Integer webUserTypeId = user.getWebUserType();
		if (webUserTypeId == null || WebUserType.dao.find(userTypeSql, webUserTypeId).size() == 0) {
			throw new ParameterException("不存在此用户类型");
		}
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
	private LUserAccount setDeletePermission(LUserAccount user) {
		if (user.getWebUserType().equals(Constant.SUPER_ADMIN_USERTYPE)) {
			user.setDeletePermission(Constant.SUPER_ADMIN_DELETEPERMISSION);
		} else if (user.getWebUserType().equals(Constant.ENGINEER_USERTYPE)) {
			return user;
		} else {
			user.setDeletePermission(Constant.ORDINARY_DELETEPERMISSION);
		}
		return user;
	}
}
