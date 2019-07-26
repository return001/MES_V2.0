package com.jimi.mes_server.controller;

import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jfinal.kit.StrKit;
import com.jimi.mes_server.annotation.Access;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.service.ReworkService;
import com.jimi.mes_server.util.ResultUtil;

/**返工功能控制器
 * @author   HCJ
 * @date     2019年5月24日 上午9:58:10
 */
public class ReworkController extends Controller {

	private static ReworkService reworkService = Enhancer.enhance(ReworkService.class);


	/**@author HCJ
	 * 获取mac返工信息
	 * @param sn SN号
	 * @param version 软件版本
	 * @param softModel 机型
	 * @param mac MAC
	 * @date 2019年6月28日 上午10:21:18
	 */
	@Access({ "engineer" })
	public void getMacReworkInfo(String sn, String version, String softModel, String mac) {
		if (!(StrKit.isBlank(sn) && !StrKit.isBlank(mac) || !StrKit.isBlank(sn) && StrKit.isBlank(mac))) {
			throw new ParameterException("SN号和MAC地址不能同时为空或者同时存在");
		}
		renderJson(reworkService.getMacReworkInfo(sn, version, softModel, mac));
	}


	/**@author HCJ
	 * 进行mac返工操作
	 * @param sn SN号
	 * @param version 软件版本
	 * @param softModel 机型
	 * @param mac MAC
	 * @date 2019年6月28日 上午10:21:57
	 */
	@Access({ "engineer" })
	public void macRework(String sn, String version, String softModel, String mac) {
		if (!(StrKit.isBlank(sn) && !StrKit.isBlank(mac) || !StrKit.isBlank(sn) && StrKit.isBlank(mac))) {
			throw new ParameterException("SN号和MAC地址不能同时为空或者同时存在");
		}
		if (reworkService.macRework(sn, version, softModel, mac)) {
			renderJson(ResultUtil.succeed());
		}
	}
}
