/**  
*  
*/  
package com.jimi.mes_server.entity;

import java.math.BigDecimal;

import com.jimi.mes_server.model.ModelCapacity;
import com.jimi.mes_server.util.ExcelHelper.Excel;

/**  
 * <p>Title: StandardCapacityItem</p>  
 * <p>Description: 导入标准产能EXCEL表 </p>  
 * <p>Copyright: Copyright (c) 2019</p>  
 * <p>Company: 惠州市几米物联技术有限公司</p>  
 * @author trjie  
 * @date 2020年8月12日
 *
 */
public class StandardCapacityItem {

	@Excel(col = 0, head = "工序组名称")
	private String processGroupName;
	
	@Excel(col = 1, head = "机型")
	private String softModel;
	
	@Excel(col = 2, head = "产能（PCS/H）")
	private String capacity;
	
	@Excel(col = 3, head = "人数")
	private String numberOfPeople;
	
	@Excel(col = 4, head = "客户编号")
	private String customerNumber;

	@Excel(col = 5, head = "客户型号/料号")
	private String customerModel;
	
	@Excel(col = 6, head = "节拍/用时（S）")
	private Double rhythm;
	
	@Excel(col = 7, head = "转线时长（分钟）")
	private String transferLineTime;
	
	@Excel(col = 8, head = "备注")
	private String remarks;

	public String getProcessGroupName() {
		return processGroupName;
	}

	public void setProcessGroupName(String processGroupName) {
		this.processGroupName = processGroupName;
	}

	public String getSoftModel() {
		return softModel;
	}

	public void setSoftModel(String softModel) {
		this.softModel = softModel;
	}

	public String getCapacity() {
		return capacity;
	}

	public void setCapacity(String capacity) {
		this.capacity = capacity;
	}

	public String getNumberOfPeople() {
		return numberOfPeople;
	}

	public void setNumberOfPeople(String numberOfPeople) {
		this.numberOfPeople = numberOfPeople;
	}

	public String getCustomerNumber() {
		return customerNumber;
	}

	public void setCustomerNumber(String customerNumber) {
		this.customerNumber = customerNumber;
	}

	public String getCustomerModel() {
		return customerModel;
	}

	public void setCustomerModel(String customerModel) {
		this.customerModel = customerModel;
	}

	public Double getRhythm() {
		return rhythm;
	}

	public void setRhythm(Double rhythm) {
		this.rhythm = rhythm;
	}

	public String getTransferLineTime() {
		return transferLineTime;
	}

	public void setTransferLineTime(String transferLineTime) {
		this.transferLineTime = transferLineTime;
	}

	public String getRemarks() {
		return remarks;
	}

	public void setRemarks(String remarks) {
		this.remarks = remarks;
	}

	public static ModelCapacity cloneCapacity(StandardCapacityItem item, String customerNumber, String customerName, Integer groupId) {
		ModelCapacity modelCapacity = new ModelCapacity();
		if (item.getCapacity() != null) {
			modelCapacity.setCapacity(Integer.valueOf(item.getCapacity()));
		}
		if (item.getCustomerModel() != null) {
			modelCapacity.setCustomerModel(item.getCustomerModel().trim());
		}
		modelCapacity.setCustomerName(customerName);
		modelCapacity.setCustomerNumber(customerNumber);
		modelCapacity.setProcessGroup(groupId);
		if (item.getNumberOfPeople() != null) {
			modelCapacity.setProcessPeopleQuantity(Integer.valueOf(item.getNumberOfPeople()));
		}
		if (item.getRemarks() != null) {
			modelCapacity.setRemark(item.getRemarks());
		}
		if (item.getRhythm() != null) {
			modelCapacity.setRhythm(BigDecimal.valueOf(item.getRhythm()));
		}
		if (item.getSoftModel() != null) {
			modelCapacity.setSoftModel(item.getSoftModel());
		}
		if (item.getTransferLineTime() != null) {
			modelCapacity.setTransferLineTime(Integer.valueOf(item.getTransferLineTime()));
		}
		return modelCapacity;
	}
	
	@Override
	public String toString() {
		return "StandardCapacityItem [processGroupName=" + processGroupName + ", softModel=" + softModel + ", capacity=" + capacity + ", numberOfPeople=" + numberOfPeople + ", customerNumber="
				+ customerNumber + ", customerModel=" + customerModel + ", rhythm=" + rhythm + ", transferLineTime=" + transferLineTime + ", remarks=" + remarks + "]";
	}
	
	
	
}
