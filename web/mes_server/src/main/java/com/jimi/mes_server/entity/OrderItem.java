package com.jimi.mes_server.entity;

import java.util.Date;

import com.jimi.mes_server.util.ExcelHelper.Excel;

public class OrderItem {

	@Excel(col = 0, head = "订单号")
	private String zhidan;

	@Excel(col = 1, head = "内部替换号")
	private String alias;

	@Excel(col = 2, head = "机型")
	private String softModel;

	@Excel(col = 3, head = "成品编码")
	private String productNo;

	@Excel(col = 4, head = "客户编号")
	private String customerNumber;

	@Excel(col = 5, head = "客户名称")
	private String customerName;

	@Excel(col = 6, head = "订单日期")
	private Date createTime;

	@Excel(col = 7, head = "数量")
	private Integer quantity;

	@Excel(col = 8, head = "交货日期")
	private Date deliveryDate;

	@Excel(col = 9, head = "备注")
	private String remark;

	public String getZhidan() {
		return zhidan;
	}

	public void setZhidan(String zhidan) {
		this.zhidan = zhidan;
	}

	public String getAlias() {
		return alias;
	}

	public void setAlias(String alias) {
		this.alias = alias;
	}

	public String getSoftModel() {
		return softModel;
	}

	public void setSoftModel(String softModel) {
		this.softModel = softModel;
	}

	public String getProductNo() {
		return productNo;
	}

	public void setProductNo(String productNo) {
		this.productNo = productNo;
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

	public Date getCreateTime() {
		return createTime;
	}

	public void setCreateTime(Date createTime) {
		this.createTime = createTime;
	}

	public Integer getQuantity() {
		return quantity;
	}

	public void setQuantity(Integer quantity) {
		this.quantity = quantity;
	}

	public Date getDeliveryDate() {
		return deliveryDate;
	}

	public void setDeliveryDate(Date deliveryDate) {
		this.deliveryDate = deliveryDate;
	}

	public String getRemark() {
		return remark;
	}

	public void setRemark(String remark) {
		this.remark = remark;
	}

}
