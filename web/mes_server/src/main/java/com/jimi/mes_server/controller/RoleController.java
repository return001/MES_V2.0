/**  
*  
*/  
package com.jimi.mes_server.controller;

import java.util.List;

import com.alibaba.fastjson.JSONObject;
import com.jfinal.aop.Enhancer;
import com.jfinal.core.Controller;
import com.jimi.mes_server.annotation.UserLog;
import com.jimi.mes_server.entity.AuthorityBO;
import com.jimi.mes_server.entity.vo.AuthorityVO;
import com.jimi.mes_server.entity.vo.RoleInfoVO;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.model.LUserAccount;
import com.jimi.mes_server.service.RoleService;
import com.jimi.mes_server.util.ResultUtil;
import com.jimi.mes_server.util.TokenBox;

/**  
 * <p>Title: RoleController</p>  
 * <p>Description: </p>  
 * <p>Copyright: Copyright (c) 2019</p>  
 * <p>Company: 惠州市几米物联技术有限公司</p>  
 * @author trjie  
 * @date 2020年4月24日
 *
 */
public class RoleController extends Controller{
	
	RoleService roleService = Enhancer.enhance(RoleService.class);
	
	@UserLog("添加角色类型，ID：{id}，部门：{department}，名称：{name}， 编号：{no}，权限：{authority}")
	public void update(Integer department, String name, String no, String authority, Integer id) {
		
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		List<AuthorityBO> authorityBOs = null;
		if (authority != null && !authority.trim().equals("")) {
			try {
				authorityBOs = JSONObject.parseArray(authority, AuthorityBO.class);
			} catch (Exception e) {
				e.printStackTrace();
				throw new ParameterException("权限格式错误");
			}
		}
		roleService.update(department, name, no, authorityBOs, id);
		renderJson(ResultUtil.succeed());
	}
	
	
	/**
	 * 
	 * <p>Description: 删除角色类型<p>
	 * @return
	 * @exception
	 * @author trjie
	 * @Time 2020年4月23日
	 */
	@UserLog("删除角色类型，ID：{id}")
	public void delete(Integer id) {
		if (id == null) {
			throw new ParameterException("参数不能为空");
		}
		roleService.delete(id);
		renderJson(ResultUtil.succeed());
	}
	
	
	/**
	 * 
	 * <p>Description: 添加角色类型<p>
	 * @return
	 * @exception
	 * @author trjie
	 * @Time 2020年4月24日
	 */
	@UserLog("添加角色类型，部门：{department}，名称：{name}， 编号：{no}，权限：{authority}")
	public void add(Integer department, String name, String no, String authority) {
		if (department == null || name == null || no == null) {
			throw new ParameterException("参数不能为空");
		}
		List<AuthorityBO> authorityBOs = null;
		if (authority != null && !authority.trim().equals("")) {
			try {
				authorityBOs = JSONObject.parseArray(authority, AuthorityBO.class);
			} catch (Exception e) {
				e.printStackTrace();
				throw new ParameterException("权限格式错误");
			}
		}
		roleService.add(department, name, no, authorityBOs);
		renderJson(ResultUtil.succeed());
	}
	
	
	/**
	 * 
	 * <p>Description: 获取角色类型信息<p>
	 * @return
	 * @exception
	 * @author trjie
	 * @Time 2020年4月24日
	 */
	public void get(Integer company, Integer department, String name, String no) {
		
		List<RoleInfoVO> roleInfoVOs =roleService.get(company, department, name, no);
		renderJson(ResultUtil.succeed(roleInfoVOs));
	}
	
	
	/**
	 * 
	 * <p>Description:根据角色类型获取权限 <p>
	 * @return
	 * @exception
	 * @author trjie
	 * @Time 2020年4月24日
	 */
	public void getAuthority(Integer role) {
		if (role == null) {
			throw new ParameterException("参数不能为空");
		}
		List<AuthorityVO> authorities = roleService.getAuthority(role);
		renderJson(ResultUtil.succeed(authorities));
	}
	
	
	/**
	 * 
	 * <p>Description: 设置用户权限<p>
	 * @return
	 * @exception
	 * @author trjie
	 * @Time 2020年4月24日
	 */
	@UserLog("设置用户权限，用户名：{user}， 权限：{authority}")
	public void setAtypicalRoleUser(Integer user, String authority) {
		if (user == null) {
			throw new ParameterException("参数不能为空");
		}
		List<AuthorityBO> authorityBOs = null;
		if (authority != null && !authority.trim().equals("")) {
			try {
				authorityBOs = JSONObject.parseArray(authority, AuthorityBO.class);
			} catch (Exception e) {
				e.printStackTrace();
				throw new ParameterException("权限格式错误");
			}
		}
		LUserAccount lUserAccount = roleService.setAtypicalRoleUser(user, authorityBOs);
		TokenBox.removeUserTokenByUsername(lUserAccount.getName());
		renderJson(ResultUtil.succeed());
	}
	
}
