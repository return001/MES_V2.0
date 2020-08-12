package com.jimi.mes_server.entity;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.List;

import com.jimi.mes_server.model.ModelCapacity;

public class ModelCapacityInfo {

	private Integer id;

	private String softModel;

	private String customerModel;

	private Integer processGroup;

	private Integer processPeopleQuantity;

	private Integer capacity;

	private String remark;

	private Double rhythm;

	private Integer transferLineTime;

	private String customerNumber;

	private String customerName;


	public Integer getId() {
		return id;
	}


	public void setId(Integer id) {
		this.id = id;
	}


	public String getSoftModel() {
		return softModel;
	}


	public void setSoftModel(String softModel) {
		this.softModel = softModel;
	}


	public String getCustomerModel() {
		return customerModel;
	}


	public void setCustomerModel(String customerModel) {
		this.customerModel = customerModel;
	}


	public Integer getProcessGroup() {
		return processGroup;
	}


	public void setProcessGroup(Integer processGroup) {
		this.processGroup = processGroup;
	}


	public Integer getProcessPeopleQuantity() {
		return processPeopleQuantity;
	}


	public void setProcessPeopleQuantity(Integer processPeopleQuantity) {
		this.processPeopleQuantity = processPeopleQuantity;
	}


	public Integer getCapacity() {
		return capacity;
	}


	public void setCapacity(Integer capacity) {
		this.capacity = capacity;
	}


	public String getRemark() {
		return remark;
	}


	public void setRemark(String remark) {
		this.remark = remark;
	}


	public Double getRhythm() {
		return rhythm;
	}


	public void setRhythm(Double rhythm) {
		this.rhythm = rhythm;
	}


	public Integer getTransferLineTime() {
		return transferLineTime;
	}


	public void setTransferLineTime(Integer transferLineTime) {
		this.transferLineTime = transferLineTime;
	}


	public String getCustomerNumber() {
		return customerNumber;
	}


	public void setCustomerNumber(String customerNumber) {
		this.customerNumber = customerNumber;
	}


	public String getCustomerName() {
		return customerName;
	}


	public void setCustomerName(String customerName) {
		this.customerName = customerName;
	}


	public static List<ModelCapacity> fill(List<ModelCapacityInfo> infos) {
		List<ModelCapacity> list = new ArrayList<ModelCapacity>();
		for (ModelCapacityInfo info : infos) {
			ModelCapacity modelCapacity = new ModelCapacity();
			if (info.getCapacity() != null) {
				modelCapacity.setCapacity(info.getCapacity());
			}
			if (info.getCustomerModel() != null) {
				modelCapacity.setCustomerModel(info.getCustomerModel());
			}
			if (info.getCustomerName() != null) {
				modelCapacity.setCustomerName(info.getCustomerName());
			}
			if (info.getCustomerNumber() != null) {
				modelCapacity.setCustomerNumber(info.getCustomerNumber());
			}
			if (info.getProcessGroup() != null) {
				modelCapacity.setProcessGroup(info.getProcessGroup());
			}
			if (info.getProcessPeopleQuantity() != null) {
				modelCapacity.setProcessPeopleQuantity(info.getProcessPeopleQuantity());
			}
			if (info.getRemark() != null) {
				modelCapacity.setRemark(info.getRemark());
			}
			if (info.getRhythm() != null) {
				modelCapacity.setRhythm(BigDecimal.valueOf(info.getRhythm()).setScale(2));
			}
			if (info.getSoftModel() != null) {
				modelCapacity.setSoftModel(info.getSoftModel());
			}
			if (info.getTransferLineTime() != null) {
				modelCapacity.setTransferLineTime(info.getTransferLineTime());
			}
			if (info.getId() != null) {
				modelCapacity.setId(info.getId());
			}
			list.add(modelCapacity);
		}
		return list;
	}
	
}
