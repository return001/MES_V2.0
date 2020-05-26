package com.jimi.mes_server.entity.vo;

import java.util.List;

public class RoleAuthorityVO {

	private List<AuthorityVO> webAuthorities;
	
	private List<AuthorityVO> pcAuthorities;

	
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
	
}
