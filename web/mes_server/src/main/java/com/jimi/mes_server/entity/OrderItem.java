package com.jimi.mes_server.entity;

import java.util.Date;

import com.jimi.mes_server.util.ExcelHelper.Excel;

/**导入的订单文件条目类
 * @author   HCJ
 * @date     2019年8月20日 上午10:01:15
 */
public class OrderItem {

	@Excel(col = 0, head = "订单号")
	private String zhidan;

	@Excel(col = 1, head = "内部替换号")
	private String alias;

	@Excel(col = 2, head = "机型")
	private String softModel;

	@Excel(col = 3, head = "成品编码")
	private String productNo;

	@Excel(col = 4, head = "版本")
	private String version;

	@Excel(col = 5, head = "客户编号")
	private String customerNumber;

	@Excel(col = 6, head = "客户名称")
	private String customerName;

	@Excel(col = 7, head = "订单日期")
	private Date orderDate;

	@Excel(col = 8, head = "订单数量")
	private Integer quantity;

	@Excel(col = 9, head = "交货日期")
	private Date deliveryDate;

	@Excel(col = 10, head = "备注")
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


	public String getVersion() {
		return version;
	}


	public void setVersion(String version) {
		this.version = version;
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


	public Date getOrderDate() {
		return orderDate;
	}


	public void setOrderDate(Date orderDate) {
		this.orderDate = orderDate;
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


	@Override
	public String toString() {
		return "OrderItem [zhidan=" + zhidan + ", alias=" + alias + ", softModel=" + softModel + ", productNo=" + productNo + ", version=" + version + ", customerNumber=" + customerNumber + ", customerName=" + customerName + ", orderDate=" + orderDate + ", quantity=" + quantity + ", deliveryDate=" + deliveryDate + ", remark=" + remark + "]";
	}

}
