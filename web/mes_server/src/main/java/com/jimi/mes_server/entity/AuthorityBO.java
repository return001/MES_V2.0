/**  
*  
*/  
package com.jimi.mes_server.entity;

/**  
 * <p>Title: AuthorityBO</p>  
 * <p>Description: </p>  
 * <p>Copyright: Copyright (c) 2019</p>  
 * <p>Company: 惠州市几米物联技术有限公司</p>  
 * @author trjie  
 * @date 2020年4月23日
 *
 */
public class AuthorityBO {

	private String system;
	
	private String model;
	
	private String page;
	
	private String operation;

	public String getSystem() {
		return system;
	}

	public void setSystem(String system) {
		this.system = system;
	}

	public String getModel() {
		return model;
	}

	public void setModel(String model) {
		this.model = model;
	}

	public String getPage() {
		return page;
	}

	public void setPage(String page) {
		this.page = page;
	}

	public String getOperation() {
		return operation;
	}

	public void setOperation(String operation) {
		this.operation = operation;
	}
	
	
}
