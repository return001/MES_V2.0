package com.jimi.mes_server.entity;

import java.util.Date;

import com.jimi.mes_server.util.ExcelHelper.Excel;

/**导入的订单文件条目类
 * @author   HCJ
 * @date     2019年8月20日 上午10:01:15
 */
public class OrderItem {

	@Excel(col = 3, head = "产品编码")
	private String productNo;

	@Excel(col = 5, head = "制单号")
	private String zhidan;

	@Excel(col = 6, head = "计划数量")
	private Integer quantity;

	@Excel(col = 7, head = "开始日期")
	private Date orderDate;

	@Excel(col = 8, head = "预计完成日")
	private Date deliveryDate;

	@Excel(col = 13, head = "机型")
	private String softModel;

	@Excel(col = 15, head = "bom版本")
	private String version;

	@Excel(col = 18, head = "生产部门")
	private String abbreviation;

	@Excel(col = 24, head = "备注")
	private String remark;


	public String getProductNo() {
		return productNo;
	}


	public void setProductNo(String productNo) {
		this.productNo = productNo;
	}


	public String getZhidan() {
		return zhidan;
	}


	public void setZhidan(String zhidan) {
		this.zhidan = zhidan;
	}


	public Integer getQuantity() {
		return quantity;
	}


	public void setQuantity(Integer quantity) {
		this.quantity = quantity;
	}


	public Date getOrderDate() {
		return orderDate;
	}


	public void setOrderDate(Date orderDate) {
		this.orderDate = orderDate;
	}


	public Date getDeliveryDate() {
		return deliveryDate;
	}


	public void setDeliveryDate(Date deliveryDate) {
		this.deliveryDate = deliveryDate;
	}


	public String getSoftModel() {
		return softModel;
	}


	public void setSoftModel(String softModel) {
		this.softModel = softModel;
	}


	public String getVersion() {
		return version;
	}


	public void setVersion(String version) {
		this.version = version;
	}


	public String getRemark() {
		return remark;
	}


	public void setRemark(String remark) {
		this.remark = remark;
	}


	public String getAbbreviation() {
		return abbreviation;
	}


	public void setAbbreviation(String abbreviation) {
		this.abbreviation = abbreviation;
	}


	@Override
	public String toString() {
		return "OrderItem [productNo=" + productNo + ", zhidan=" + zhidan + ", quantity=" + quantity + ", orderDate=" + orderDate + ", deliveryDate=" + deliveryDate + ", softModel=" + softModel + ", version=" + version + ", abbreviation=" + abbreviation + ", remark=" + remark + "]";
	}

}
