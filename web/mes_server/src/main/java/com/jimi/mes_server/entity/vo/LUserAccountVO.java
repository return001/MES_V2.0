package com.jimi.mes_server.entity.vo;

import java.util.Date;
import java.util.List;

import com.jimi.mes_server.model.Authority;
import com.jimi.mes_server.model.Department;
import com.jimi.mes_server.model.LUserAccount;
import com.jimi.mes_server.model.Role;

/**
 * 
 * <p>Title: LUserAccountVO</p>  
 * <p>Description: 用户信息表-显示层</p>  
 * <p>Copyright: Copyright (c) 2019</p>  
 * <p>Company: 惠州市几米物联技术有限公司</p>  
 * @author trjie  
 * @date 2020年4月22日
 *
 */
public class LUserAccountVO {

	private Integer id;

	private Boolean inService;

	private Integer loginStatus;

	private Date loginTime;

	private String name;

	private String userDes;

	private RoleVO roleVO;

	private String token;

	private List<AuthorityVO> webAuthorities;

	private List<AuthorityVO> pcAuthorities;

	public Integer getId() {
		return id;
	}


	public void setId(Integer id) {
		this.id = id;
	}


	public Boolean getInService() {
		return inService;
	}


	public void setInService(Boolean inService) {
		this.inService = inService;
	}


	public Integer getLoginStatus() {
		return loginStatus;
	}


	public void setLoginStatus(Integer loginStatus) {
		this.loginStatus = loginStatus;
	}


	public Date getLoginTime() {
		return loginTime;
	}


	public void setLoginTime(Date loginTime) {
		this.loginTime = loginTime;
	}


	public String getName() {
		return name;
	}


	public void setName(String name) {
		this.name = name;
	}


	public String getUserDes() {
		return userDes;
	}


	public void setUserDes(String userDes) {
		this.userDes = userDes;
	}


	public RoleVO getRoleVO() {
		return roleVO;
	}


	public void setRoleVO(RoleVO roleVO) {
		this.roleVO = roleVO;
	}
	
	
	public List<AuthorityVO> getWebAuthorities() {
		return webAuthorities;
	}

	
	public void setWebAuthorities(List<AuthorityVO> webAuthorities) {
		this.webAuthorities = webAuthorities;
	}
	
	
	public List<AuthorityVO> getPcAuthorities() {
		return pcAuthorities;
	}
	
	
	public void setPcAuthorities(List<AuthorityVO> pcAuthorities) {
		this.pcAuthorities = pcAuthorities;
	}


	public String getToken() {
		return token;
	}


	public void setToken(String token) {
		this.token = token;
	}


	public static LUserAccountVO filLUserAccountVO(LUserAccount user, Role role, List<Authority> authorities, Department department) {
		LUserAccountVO lUserAccountVO = new LUserAccountVO();
		lUserAccountVO.setInService(user.getInService());
		lUserAccountVO.setLoginStatus(user.getLoginStatus());
		lUserAccountVO.setLoginTime(user.getLoginTime());
		lUserAccountVO.setName(user.getName());
		lUserAccountVO.setUserDes(user.getUserDes());
		lUserAccountVO.setId(user.getId());
		if (authorities != null) {
			lUserAccountVO.setWebAuthorities(AuthorityVO.fillWebList(authorities));
			lUserAccountVO.setPcAuthorities(AuthorityVO.fillPcList(authorities));
		}
		if (role != null) {
			lUserAccountVO.setRoleVO(RoleVO.fillRoleVO(role, department));
		}

		return lUserAccountVO;

	}

}
