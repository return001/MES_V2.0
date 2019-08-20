package com.jimi.mes_server.controller;

import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jfinal.core.paragetter.Para;
import com.jimi.mes_server.annotation.Access;
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


	@Access({ "operator", "administration" })
	public void select(String table, Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter, Integer type) {
		renderJson(ResultUtil.succeed(userService.select(table, pageNo, pageSize, ascBy, descBy, filter, type)));
	}


	public void login(String name, String password) {
		System.out.println("a" + name + password);
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
		userVO.put(TokenBox.TOKEN_ID_KEY_NAME, tokenId);
		TokenBox.put(tokenId, SESSION_KEY_LOGIN_USER, userVO);
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


	@Access({ "administration" })
	public void add(@Para("") LUserAccount user) {
		String tokenId = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO tokenUser = TokenBox.get(tokenId, SESSION_KEY_LOGIN_USER);
		if (userService.add(user, tokenUser)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({ "administration" })
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
	@Access({ "administration", "engineer" })
	public void getUserType() {
		renderJson(userService.getUserType());
	}


	/**@author HCJ
	 * 校验传入的参数与内存中记录的TOKEN是否一致
	 * @date 2019年5月14日 下午5:19:31
	 */
	@Access({ "administration", "engineer" })
	public void validate(String name, String password) {
		userService.validate(name, password);
		String token = getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO userVO = TokenBox.get(token, SESSION_KEY_LOGIN_USER);
		if (userVO == null || !userVO.getName().equalsIgnoreCase(name) || !userVO.getPassword().equalsIgnoreCase(password)) {
			throw new OperationException("用户校验失败");
		}
		renderJson(ResultUtil.succeed());
	}


	public void getUserIdAndName(String userName) {
		renderJson(ResultUtil.succeed(userService.getUserIdAndName(userName)));
	}
}
