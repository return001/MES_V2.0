package com.jimi.mes_server.controller;

import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jfinal.core.paragetter.Para;
import com.jimi.mes_server.annotation.Access;
import com.jimi.mes_server.model.GpsManuorderparam;
import com.jimi.mes_server.service.OrderService;
import com.jimi.mes_server.util.ResultUtil;

/**
 * 订单控制器
 * <br>
 * <b>2018年5月29日</b>
 * @author 沫熊工作室 <a href="http://www.darhao.cc">www.darhao.cc</a>
 */
public class OrderController extends Controller {

	private static OrderService orderService = Enhancer.enhance(OrderService.class);

	public static final String ORDER_TABLE_NAME = "Gps_ManuOrderParam";


	@Access({ "SuperAdmin", "operator", "engineer" })
	public void select(Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter, Integer type){
		renderJson(ResultUtil.succeed(orderService.select(ORDER_TABLE_NAME, pageNo, pageSize, ascBy, descBy, filter, type)));
	}


	@Access({"SuperAdmin", "engineer"})
	public void update(@Para("") GpsManuorderparam order) {
		if(orderService.update(order)) {
			renderJson(ResultUtil.succeed());
		}else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({"SuperAdmin", "engineer"})
	public void create(@Para("") GpsManuorderparam order) {
		if(orderService.create(order)) {
			renderJson(ResultUtil.succeed());
		}else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({"SuperAdmin", "engineer"})
	public void copy(String key) {
		if(orderService.copy(key)) {
			renderJson(ResultUtil.succeed());
		}else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({"SuperAdmin", "engineer"})
	public void start(String key) {
		if(orderService.start(key)) {
			renderJson(ResultUtil.succeed());
		}else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({"SuperAdmin", "engineer"})
	public void finish(String key) {
		if(orderService.finish(key)) {
			renderJson(ResultUtil.succeed());
		}else {
			renderJson(ResultUtil.failed());
		}
	}


	@Access({"SuperAdmin", "engineer"})
	public void cancel(String key) {
		if(orderService.cancel(key)) {
			renderJson(ResultUtil.succeed());
		}else {
			renderJson(ResultUtil.failed());
		}
	}
}
