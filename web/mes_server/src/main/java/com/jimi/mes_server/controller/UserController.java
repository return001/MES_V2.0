package com.jimi.mes_server.controller;

import java.util.List;

import org.apache.commons.lang3.StringUtils;

import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jfinal.core.paragetter.Para;
import com.jimi.mes_server.annotation.PermissionPass;
import com.jimi.mes_server.annotation.UserLog;
import com.jimi.mes_server.entity.vo.AuthorityVO;
import com.jimi.mes_server.entity.vo.LUserAccountVO;
import com.jimi.mes_server.exception.OperationException;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.model.LUserAccount;
import com.jimi.mes_server.service.UserService;
import com.jimi.mes_server.util.ResultUtil;
import com.jimi.mes_server.util.TokenBox;

/**
 * 用户控制器 <br>
 * <b>2018年6月1日</b>
 * @author 沫熊工作室 <a href="http://www.darhao.cc">www.darhao.cc</a>
 */
public class UserController extends Controller {

	private UserService userService = Enhancer.enhance(UserService.class);

	public static final String USER_TABLE_NAME = "LUserAccount";

	public static final String SESSION_KEY_LOGIN_USER = "loginUser";

	
	public void select(Integer pageNo, Integer pageSize, String name, String userDes, String otherProcess, Boolean inService, String mainProcess, String proficiency, String lineName, Integer company, Integer department, String roleName) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码与页容量不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0 ) {
			throw new ParameterException("页码与页容量需大于0");
		}
		renderJson(ResultUtil.succeed(userService.select(pageNo, pageSize, name, userDes, otherProcess, inService, mainProcess, proficiency, lineName, company, department, roleName)));
	}
	

	@PermissionPass
	@UserLog("用户工号：{name}，进行登录")
	public void login(String name, String password) {
		LUserAccountVO userVO = userService.login(name, password, true);
		// 判断重复登录
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		if (tokenId != null) {
			LUserAccountVO temp = TokenBox.get(tokenId, SESSION_KEY_LOGIN_USER);
			if (temp != null && userVO.getId() == temp.getId()) {
				throw new ParameterException("请不要重复登录");
			}
		}
		tokenId = TokenBox.createTokenId();
		userVO.setToken(tokenId);
		TokenBox.put(tokenId, SESSION_KEY_LOGIN_USER, userVO);
		TokenBox.putUserToken(userVO.getName(), tokenId);
		renderJson(ResultUtil.succeed(userVO));
	}

	
	public void logout() {
		// 判断是否未登录
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(tokenId, SESSION_KEY_LOGIN_USER);
		if (userVO == null) {
			throw new ParameterException("未登录时无需退出");
		}
		TokenBox.remove(tokenId);
		TokenBox.removeUserTokenByUserNameAndToken(userVO.getName(), tokenId);
		renderJson(ResultUtil.succeed());
	}


	public void checkLogined() {
		LUserAccountVO userVO = TokenBox.get(getPara(TokenBox.TOKEN_ID_KEY_NAME), SESSION_KEY_LOGIN_USER);
		if (userVO != null) {
			renderJson(ResultUtil.succeed(userVO));
		} else {
			renderJson(ResultUtil.succeed("不存在登录的用户"));
		}
	}


	/**
	 * 
	 * <p>Description: 用户添加<p>
	 * @return
	 * @exception
	 * @author trjie
	 * @Time 2020年4月22日
	 */
	@UserLog("添加用户：{user}")
	public void add(@Para("") LUserAccount user) {
		if (userService.add(user)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}

	@UserLog("修改用户：{user}")
	public void update(@Para("") LUserAccount user) {
		if (userService.update(user)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	/**@author HCJ
	 * 获取用户角色
	 * @date 2019年5月15日 下午2:48:40
	 */
	/*@Access({ "administration", "engineer" })
	public void getUserType() {
		renderJson(userService.getUserType());
	}
	*/

	/**@author HCJ
	 * 校验传入的参数与内存中记录的TOKEN是否一致
	 * @date 2019年5月14日 下午5:19:31
	 */
	public void validate(String name, String password) {
		LUserAccount user = userService.validate(name, password);
		String token = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(token, SESSION_KEY_LOGIN_USER);
		if (userVO == null || !user.getId().equals(userVO.getId())) {
			throw new OperationException("用户校验失败");
		}
		renderJson(ResultUtil.succeed());
	}


	/**@author HCJ
	 * 根据用户名称模糊搜索获取用户ID和名称
	 * @param userName
	 * @date 2019年8月27日 下午3:20:02
	 */
	public void getUserIdAndName(String userName) {
		renderJson(ResultUtil.succeed(userService.getUserIdAndName(userName)));
	}


	/**@author HCJ
	 * 更新密码
	 * @param orgPwd 原始密码
	 * @param curPwd 新密码
	 * @date 2020年4月13日 下午5:21:13
	 */
	@UserLog("修改用户密码}")
	public void updatePassword(String orgPwd, String curPwd) {
		if (StringUtils.isAnyBlank(orgPwd, curPwd)) {
			throw new ParameterException("参数不能为空");
		}
		String token = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(token, SESSION_KEY_LOGIN_USER);
		if (userVO == null) {
			throw new OperationException("用户未登录");
		}
		if (userService.updatePassword(orgPwd, curPwd, userVO.getId())) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}
	
	
	public void getAuthority(Integer user) {
		if (user == null) {
			throw new ParameterException("参数不能为空");
		}
		List<AuthorityVO> authorityVOs = userService.getAuthority(user);
		renderJson(ResultUtil.succeed(authorityVOs));
	}
}
