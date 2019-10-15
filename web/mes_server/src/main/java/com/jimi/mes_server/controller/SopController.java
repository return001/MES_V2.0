package com.jimi.mes_server.controller;

import org.apache.commons.lang3.StringUtils;

import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.service.SopService;
import com.jimi.mes_server.util.ResultUtil;

public class SopController extends Controller {

	private static SopService sopService = Enhancer.enhance(SopService.class);


	public void addFactory(String alias, String abbreviation, String fullName) {
		if (StringUtils.isAnyBlank(alias, abbreviation, fullName)) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.addFactory(alias, abbreviation, fullName)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	public void deleteFactory(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.deleteFactory(id)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	public void selectFactory(Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter) {
		renderJson(ResultUtil.succeed(sopService.selectFactory(pageNo, pageSize, ascBy, descBy, filter)));
	}


	public void editFactory(Integer id, String alias, String abbreviation, String fullName) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.editFactory(id, alias, abbreviation, fullName)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	public void addWorkshop(Integer factoryId, String number, String name) {
		if (factoryId == null || StringUtils.isAnyBlank(number, name)) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.addWorkshop(factoryId, number, name)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	public void deleteWorkshop(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.deleteWorkshop(id)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}


	public void selectWorkshop(Integer pageNo, Integer pageSize, Integer factoryId, String number, String name) {
		if (pageNo == null || pageSize == null) {
			throw new ParameterException("页码和页大小不能为空");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("页码与页大小均需要大于0");
		}
		renderJson(ResultUtil.succeed(sopService.selectWorkshop(pageNo, pageSize, factoryId, number, name)));
	}


	public void editWorkshop(Integer id, Integer factoryId, String number, String name) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		if (sopService.editWorkshop(id, factoryId, number, name)) {
			renderJson(ResultUtil.succeed());
		} else {
			renderJson(ResultUtil.failed());
		}
	}
}
