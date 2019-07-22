package com.jimi.mes_server.entity.vo;

import com.jimi.mes_server.model.Orders;

public class OrderVO extends Orders {

	private static final long serialVersionUID = 5840191253006973812L;

	private Integer unscheduledQuantity;

	private Integer capacity;

	public Integer getCapacity() {
		return capacity;
	}

	public void setCapacity(Integer capacity) {
		this.capacity = capacity;
	}

	public Integer getUnscheduledQuantity() {
		return unscheduledQuantity;
	}

	public void setUnscheduledQuantity(Integer unscheduledQuantity) {
		this.unscheduledQuantity = unscheduledQuantity;
	}

	public OrderVO(Orders order, Integer unscheduledQuantity, Integer capacity) {
		this.setUnscheduledQuantity(unscheduledQuantity);
		this.set("unscheduledQuantity", getUnscheduledQuantity());
		this.setCapacity(capacity);
		this.set("capacity", getCapacity());
		this.setAlias(order.getAlias());
		this.setOrderDate(order.getOrderDate());
		this.setCustomerName(order.getCustomerName());
		this.setCustomerNumber(order.getCustomerNumber());
		this.setDeleteReason(order.getDeleteReason());
		this.setDeliveryDate(order.getDeliveryDate());
		this.setId(order.getId());
		this.setOrderStatus(order.getOrderStatus());
		this.setProductNo(order.getProductNo());
		this.setQuantity(order.getQuantity());
		this.setRemark(order.getRemark());
		this.setSoftModel(order.getSoftModel());
		this.setVersion(order.getVersion());
		this.setZhidan(order.getZhidan());
	}
}
