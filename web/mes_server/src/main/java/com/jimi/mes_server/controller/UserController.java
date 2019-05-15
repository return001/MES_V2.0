package com.jimi.mes_server.controller;

import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jfinal.core.paragetter.Para;
import com.jimi.mes_server.annotation.Access;
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


	@Access({ "SuperAdmin","admin","operator" })
	public void select(String table, Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter, Integer type) {
		renderJson(ResultUtil.succeed(userService.select(table, pageNo, pageSize, ascBy, descBy, filter, type)));
	}


	public void login(String name, String password) {
		System.out.println("a" + name + password);
		LUserAccount user = userService.login(name, password);
		// 判断重复登录
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		if (tokenId != null) {
			LUserAccount user2 = TokenBox.get(tokenId, SESSION_KEY_LOGIN_USER);
			if (user2 != null && user.getId() == user2.getId()) {
				throw new ParameterException("请不要重复登录");
			}
		}
		tokenId = TokenBox.createTokenId();
		user.put(TokenBox.TOKEN_ID_KEY_NAME, tokenId);
		TokenBox.put(tokenId, SESSION_KEY_LOGIN_USER, user);
		renderJson(ResultUtil.succeed(user));
	}


	public void logout() {
		// 判断是否未登录
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccount user = TokenBox.get(tokenId, SESSION_KEY_LOGIN_USER);
		if (user == null) {
			throw new ParameterException("未登录时无需退出");
		}
		TokenBox.remove(tokenId);
		renderJson(ResultUtil.succeed());
	}


	public void checkLogined() {
		LUserAccount user = TokenBox.get(getPara(TokenBox.TOKEN_ID_KEY_NAME), SESSION_KEY_LOGIN_USER);
		if (user != null) {
			renderJson(ResultUtil.succeed(user));
		} else {
			renderJson(ResultUtil.succeed("不存在登录的用户"));
		}
	}


	@Access({ "SuperAdmin" })
	public void add(@Para("") LUserAccount user) {
		if (userService.add(user)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "SuperAdmin" })
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
	@Access({ "SuperAdmin" })
	public void getUserType() {
		renderJson(userService.getUserType());
	}


	/**@author HCJ
	 * 校验传入的参数与内存中记录的TOKEN是否一致
	 * @date 2019年5月14日 下午5:19:31
	 */
	@Access({ "SuperAdmin","admin"})
	public void validate(String name, String password, String TOKEN) {
		userService.validate(name, password);
		LUserAccount user = TokenBox.get(TOKEN, SESSION_KEY_LOGIN_USER);
		if (user == null || !user.getName().equals(name) || !user.getPassword().equals(password)) {
			throw new OperationException("用户校验失败");
		}
		renderJson(ResultUtil.succeed());
	}
}
