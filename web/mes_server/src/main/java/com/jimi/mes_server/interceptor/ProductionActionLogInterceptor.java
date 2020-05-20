package com.jimi.mes_server.interceptor;

import java.util.Date;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.servlet.http.HttpServletRequest;
import com.jfinal.aop.Interceptor;
import com.jfinal.aop.Invocation;
import com.jfinal.core.Controller;
import com.jfinal.json.Json;
import com.jimi.mes_server.annotation.ProductionLog;
import com.jimi.mes_server.controller.UserController;
import com.jimi.mes_server.entity.vo.LUserAccountVO;
import com.jimi.mes_server.model.Orders;
import com.jimi.mes_server.model.ProductionActionLog;
import com.jimi.mes_server.util.TokenBox;

public class ProductionActionLogInterceptor implements Interceptor {

	private static final String REGEX = "\\{[a-zA-Z0-9]+\\}";


	@Override
	public void intercept(Invocation invocation) {
		ProductionLog log = invocation.getMethod().getAnnotation(ProductionLog.class);
		Controller controller = invocation.getController();
		ProductionActionLog productionActionLog = new ProductionActionLog();

		// 如果存在@Log注解，则进行日志记录
		if (log != null) {
			// 匹配参数并替换值
			Matcher matcher = Pattern.compile(REGEX).matcher(log.value());
			StringBuffer sb = new StringBuffer();
			while (matcher.find()) {
				String a = matcher.group();
				a = a.substring(1, a.length() - 1);
				String value = controller.getPara(a);
				if (value == null) {
					value = "null";
				}
				// 美元符转义
				if (value.contains("$")) {
					value = value.replaceAll("\\$", "\\\\\\$");
				}
				matcher.appendReplacement(sb, value);
			}
			matcher.appendTail(sb);
			// 日志生成
			HttpServletRequest request = controller.getRequest();
			String url = request.getRequestURL().toString();
			String address = request.getRemoteAddr() + ":" + request.getRemotePort();
			String parameters = Json.getJson().toJson(request.getParameterMap());
			productionActionLog.setAddress(address);
			productionActionLog.setParameters(parameters);
			productionActionLog.setTime(new Date());
			productionActionLog.setUrl(url);
			if (sb.toString().contains("null")) {
				productionActionLog.setAction(sb.toString().replace("null", parameters));
			} else {
				productionActionLog.setAction(sb.toString());
			}
			LUserAccountVO user = TokenBox.get(controller.getPara(TokenBox.TOKEN_ID_KEY_NAME), UserController.SESSION_KEY_LOGIN_USER);
			if (user != null) {
				productionActionLog.setUid(user.getUserDes());
			}
			if ("addOrder".equals(invocation.getMethodName()) || "editOrderDetail".equals(invocation.getMethodName()) || "editOrder".equals(invocation.getMethodName()) || "editOrderRemarks".equals(invocation.getMethodName())) {
				productionActionLog.setZhidan(controller.getPara("zhidan")).setAlias(controller.getPara("alias"));
			} else if ("deleteOrder".equals(invocation.getMethodName()) || "updateOrderTableStatus".equals(invocation.getMethodName())) {
				Orders orders = Orders.dao.findById(controller.getPara("id"));
				if (orders != null) {
					productionActionLog.setZhidan(orders.getZhidan()).setAlias(orders.getAlias());
				}
			}
			// 执行
			long t1 = System.currentTimeMillis();
			try {
				invocation.invoke();
				long t2 = System.currentTimeMillis() - t1;
				productionActionLog.setConsumeTime((int) t2);
				productionActionLog.setResultCode(200);
				// 插入
				productionActionLog.save();
			} catch (Exception e) {
				long t3 = System.currentTimeMillis() - t1;
				productionActionLog.setConsumeTime((int) t3);
				productionActionLog.setResultCode(ErrorLogInterceptor.getResultCode(e));
				// 插入
				productionActionLog.save();
				throw e;
			}
		} else {
			invocation.invoke();
		}
	}
}
