package com.jimi.mes_server.interceptor;

import com.jfinal.aop.Interceptor;
import com.jfinal.aop.Invocation;
import com.jimi.mes_server.annotation.PermissionPass;
import com.jimi.mes_server.controller.UserController;
import com.jimi.mes_server.entity.vo.AuthorityVO;
import com.jimi.mes_server.entity.vo.LUserAccountVO;
import com.jimi.mes_server.exception.AccessException;
import com.jimi.mes_server.util.TokenBox;

/**
 * 用户权限拦截器
 * <br>
 * <b>2018年5月29日</b>
 * @author 沫熊工作室 <a href="http://www.darhao.cc">www.darhao.cc</a>
 */
public class AccessInterceptor implements Interceptor {

	@Override
	public void intercept(Invocation invocation) {
		PermissionPass permissionPass = invocation.getMethod().getAnnotation(PermissionPass.class);
		if (permissionPass != null) {
			invocation.invoke();
			return;
		}
		/**
		 * 先获取所有文件，才能获取到其他的参数
		 */
		if (invocation.getController().getRequest().getContentType().contains("multipart/form-data")) {
			invocation.getController().getFiles();
		}
		String token = invocation.getController().getPara(TokenBox.TOKEN_ID_KEY_NAME);
		if (token == null || token.trim().equals("")) {
			throw new AccessException("未登录");
		}
		LUserAccountVO user = TokenBox.get(token, UserController.SESSION_KEY_LOGIN_USER);
		if (user == null) {
			throw new AccessException("未登录");
		}
		if (user.getAuthorities() != null) {
			for (AuthorityVO authorityVO : user.getAuthorities()) {
				if (authorityVO.getUrls().contains(invocation.getActionKey())) {
					invocation.invoke();
					return;
				}
			}
		}
		throw new AccessException("权限不足");
	}

}
