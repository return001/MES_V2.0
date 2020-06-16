/**  
*  
*/
package com.jimi.mes_server.entity.vo;

import java.util.ArrayList;
import java.util.List;

import com.jfinal.plugin.activerecord.Record;

/**  
 * <p>Title: DepartmentInfoVO</p>  
 * <p>Description: </p>  
 * <p>Copyright: Copyright (c) 2019</p>  
 * <p>Company: 惠州市几米物联技术有限公司</p>  
 * @author trjie  
 * @date 2020年4月23日
 *
 */
public class DepartmentInfoVO {

	private Integer id;

	private Integer company;

	private String companyName;

	private String no;

	private String name;


	public Integer getCompany() {
		return company;
	}


	public void setCompany(Integer company) {
		this.company = company;
	}


	public String getNo() {
		return no;
	}


	public void setNo(String no) {
		this.no = no;
	}


	public String getName() {
		return name;
	}


	public void setName(String name) {
		this.name = name;
	}


	public Integer getId() {
		return id;
	}


	public void setId(Integer id) {
		this.id = id;
	}


	public String getCompanyName() {
		return companyName;
	}


	public void setCompanyName(String companyName) {
		this.companyName = companyName;
	}


	public static List<DepartmentInfoVO> fillList(List<Record> records) {
		List<DepartmentInfoVO> departmentInfoVOs = new ArrayList<DepartmentInfoVO>(records.size());
		for (Record record : records) {
			DepartmentInfoVO departmentInfoVO = new DepartmentInfoVO();
			departmentInfoVO.setNo(record.getStr("departmentNo"));
			departmentInfoVO.setName(record.getStr("departmentName"));
			departmentInfoVO.setId(record.getInt("departmentId"));
			departmentInfoVO.setCompany(record.getInt("companyId"));
			departmentInfoVO.setCompanyName(record.getStr("companyName"));
			departmentInfoVOs.add(departmentInfoVO);
		}
		return departmentInfoVOs;
	}
}
