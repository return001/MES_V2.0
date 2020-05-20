/**  
*  
*/  
package com.jimi.mes_server.entity.vo;

import java.util.ArrayList;
import java.util.List;

import com.jimi.mes_server.model.Authority;

/**  
 * <p>Title: AuthorityVO</p>  
 * <p>Description: 用户权限表-显示层</p>  
 * <p>Copyright: Copyright (c) 2019</p>  
 * <p>Company: 惠州市几米物联技术有限公司</p>  
 * @author trjie  
 * @date 2020年4月22日
 *
 */
public class AuthorityVO {

	private Integer id;
	
	private String operation;
	
	private String page;
	
	private String model;
	
	private String system;

	private String urls;
	
	public Integer getId() {
		return id;
	}

	public void setId(Integer id) {
		this.id = id;
	}

	public String getOperation() {
		return operation;
	}

	public void setOperation(String operation) {
		this.operation = operation;
	}

	public String getPage() {
		return page;
	}

	public void setPage(String page) {
		this.page = page;
	}

	public String getModel() {
		return model;
	}

	public void setModel(String model) {
		this.model = model;
	}

	public String getSystem() {
		return system;
	}

	public void setSystem(String system) {
		this.system = system;
	}
	
	public String getUrls() {
		return urls;
	}

	public void setUrls(String urls) {
		this.urls = urls;
	}
	

	public static AuthorityVO fillAuthorityVO(Authority authority) {
		 AuthorityVO authorityVO = new AuthorityVO();
		 authorityVO.setId(authority.getId());
		 authorityVO.setOperation(authority.getOperation());
		 authorityVO.setPage(authority.getPage());
		 authorityVO.setModel(authority.getSystem());
		 authorityVO.setSystem(authority.getSystem());
		 authorityVO.setUrls(authority.getUrls());
		 return authorityVO;
	}
	
	
	public static List<AuthorityVO> fillList(List<Authority> authorities) {
		List<AuthorityVO> authorityVOs = new ArrayList<AuthorityVO>();
		
		for (Authority authority : authorities) {
			 AuthorityVO authorityVO = new AuthorityVO();
			 authorityVO.setId(authority.getId());
			 authorityVO.setOperation(authority.getOperation());
			 authorityVO.setPage(authority.getPage());
			 authorityVO.setModel(authority.getSystem());
			 authorityVO.setSystem(authority.getSystem());
			 authorityVO.setUrls(authority.getUrls());
			 authorityVOs.add(authorityVO);
		}
		return authorityVOs;
	}
	
}
