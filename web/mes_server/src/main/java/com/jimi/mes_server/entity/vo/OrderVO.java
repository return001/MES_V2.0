package com.jimi.mes_server.entity.vo;

import java.text.SimpleDateFormat;

import com.jimi.mes_server.model.Orders;

/**订单VO
 * @author   HCJ
 * @date     2019年8月16日 下午2:32:52
 */
public class OrderVO extends Orders {

	private static final long serialVersionUID = 5840191253006973812L;

	private SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");


	public Integer getCapacity() {
		return this.getInt("capacity");
	}


	public void setCapacity(Integer capacity) {
		this.set("capacity", capacity);
	}


	public Integer getUnscheduledQuantity() {
		return this.getInt("unscheduledQuantity");
	}


	public void setUnscheduledQuantity(Integer unscheduledQuantity) {
		this.set("unscheduledQuantity", unscheduledQuantity);
	}


	public Integer getProcessPeopleQuantity() {
		return this.getInt("processPeopleQuantity");
	}


	public void setProcessPeopleQuantity(Integer processPeopleQuantity) {
		this.set("processPeopleQuantity", processPeopleQuantity);
	}


	public Integer getTransferLineTime() {
		return this.getInt("transferLineTime");
	}


	public void setTransferLineTime(Integer transferLineTime) {
		this.set("transferLineTime", transferLineTime);
	}


	public Double getRhythm() {
		return this.getDouble("rhythm");
	}


	public void setRhythm(Double rhythm) {
		this.set("rhythm", rhythm);
	}


	public OrderVO(Orders order) {
		this.set("alias", order.getAlias());
		this.set("orderDate", order.getOrderDate());
		this.set("customerName", order.getCustomerName());
		this.set("customerNumber", order.getCustomerNumber());
		this.set("deliveryDate", order.getDeliveryDate());
		this.set("id", order.getId());
		this.set("productNo", order.getProductNo());
		this.set("quantity", order.getQuantity());
		this.set("remark", order.getPcRemarks());
		this.set("softModel", order.getSoftModel());
		this.set("zhidan", order.getZhidan());
		this.set("customerMaterialNo", order.getCustomerMaterialNo());
		this.set("reworkZhidan", order.getReworkZhidan());
		this.set("reworkQuantity", order.getReworkQuantity());
		this.set("materialState", order.getMaterialState());
		if (order.getEnoughMaterialTime() != null) {
			this.set("enoughMaterialTime", dateFormat.format(order.getEnoughMaterialTime()));
		} else {
			this.set("enoughMaterialTime", order.getEnoughMaterialTime());
		}
		this.set("pcba", order.getPcba());
	}
}
