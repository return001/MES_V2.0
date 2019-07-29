package com.jimi.mes_server.interceptor;

import java.io.File;
import java.util.List;

import com.jfinal.aop.Aop;
import com.jfinal.aop.Interceptor;
import com.jfinal.aop.Invocation;
import com.jfinal.upload.UploadFile;
import com.jimi.mes_server.annotation.Access;
import com.jimi.mes_server.controller.UserController;
import com.jimi.mes_server.entity.Constant;
import com.jimi.mes_server.entity.vo.LUserAccountVO;
import com.jimi.mes_server.exception.AccessException;
import com.jimi.mes_server.service.UserService;
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
		Access access = invocation.getMethod().getAnnotation(Access.class);
		if (access == null) {
			invocation.invoke();
			return;
		}
		UserService userService = Aop.get(UserService.class);
		if (invocation.getController().getRequest().getContentType().contains("multipart/form-data")) {
			// 获取文件
			List<UploadFile> uploadFiles = invocation.getController().getFiles();
		}
		String token = invocation.getController().getPara(TokenBox.TOKEN_ID_KEY_NAME);
		LUserAccountVO user = TokenBox.get(token, UserController.SESSION_KEY_LOGIN_USER);
		if (user == null) {
			throw new AccessException("未登录");
		}
		LUserAccountVO userVO = userService.login(user.getName(), user.getPassword(), false);
		TokenBox.put(token, UserController.SESSION_KEY_LOGIN_USER, userVO);
		if (!userVO.getInService()) {
			throw new AccessException("此用户未启用");
		}
		if (userVO.getWebUserType() != null && Constant.SUPER_ADMIN_USERTYPE.equals(userVO.getWebUserType())) {
			invocation.invoke();
			return;
		}
		String[] accessUserTypes = access.value();
		String typeName = userVO.getTypeName();
		for (String userType : accessUserTypes) {
			if (userType.equals(typeName)) {
				invocation.invoke();
				return;
			}
		}
		throw new AccessException("权限不足");
	}

}
