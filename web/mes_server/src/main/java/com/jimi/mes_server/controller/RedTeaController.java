package com.jimi.mes_server.controller;

import java.io.IOException;
import org.apache.http.client.ClientProtocolException;
import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jimi.mes_server.service.RedTeaService;
import com.jimi.mes_server.util.ResultUtil;

public class RedTeaController extends Controller {

	private static RedTeaService redTeaService = Enhancer.enhance(RedTeaService.class);


	public void findCId(String sn, String imei, String deviceModel) {
		System.out.println(sn + imei + deviceModel);
		String resonp = "";
		try {
			resonp = redTeaService.getCId(sn, deviceModel, imei);
		} catch (ClientProtocolException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		if (!"".equals(resonp)) {
			renderJson(ResultUtil.succeed(resonp));
		} else {
			renderJson(ResultUtil.failed());
		}
	}
}
