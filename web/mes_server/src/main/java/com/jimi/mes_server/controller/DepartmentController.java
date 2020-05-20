/**  
*  
*/  
package com.jimi.mes_server.controller;

import java.util.List;

import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;

import com.jimi.mes_server.annotation.UserLog;
import com.jimi.mes_server.entity.vo.DepartmentInfoVO;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.service.DepartmentService;
import com.jimi.mes_server.util.ResultUtil;

/**  
 * <p>Title: DepartmentController</p>  
 * <p>Description: </p>  
 * <p>Copyright: Copyright (c) 2019</p>  
 * <p>Company: 惠州市几米物联技术有限公司</p>  
 * @author trjie  
 * @date 2020年4月24日
 *
 */
public class DepartmentController extends Controller{

	private DepartmentService departmentService = Enhancer.enhance(DepartmentService.class);
	
	
	/**
	 * 
	 * <p>Description: 修改部门信息<p>
	 * @return
	 * @exception
	 * @author trjie
	 * @Time 2020年4月24日
	 */
	@UserLog("修改部门，部门ID: {id}, 公司ID：{company}, 编号：{no}， 名称：{name}")
	public void update(Integer company, String no, String name, Integer id) {
		
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		departmentService.update(company, no, name, id);
		renderJson(ResultUtil.succeed());
	}
	
	
	/**
	 * 
	 * <p>Description: 添加部门<p>
	 * @return
	 * @exception
	 * @author trjie
	 * @Time 2020年4月24日
	 */
	@UserLog("添加部门，公司ID：{company}, 编号：{no}， 名称：{name}")
	public void add(Integer company, String no, String name) {
		
		if (company == null || no == null || name == null || no.trim().equals("") || name.trim().equals("")) {
			throw new ParameterException("参数不能为空");
		}
		departmentService.add(company, no, name);
		renderJson(ResultUtil.succeed());
	}
	
	
	/**
	 * 
	 * <p>Description: 删除部门<p>
	 * @return
	 * @exception
	 * @author trjie
	 * @Time 2020年4月24日
	 */
	@UserLog("删除部门，部门ID：{id}")
	public void delete(Integer id) {
		
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		departmentService.delete(id);
		renderJson(ResultUtil.succeed());
		
	}
	
	
	/**
	 * 
	 * <p>Description: 获取部门信息<p>
	 * @return
	 * @exception
	 * @author trjie
	 * @Time 2020年4月24日
	 */
	public void get(Integer company, String no, String name) {
		
		List<DepartmentInfoVO> departmentInfoVOs = departmentService.get(company, no, name);
		renderJson(ResultUtil.succeed(departmentInfoVOs));

	}
}
