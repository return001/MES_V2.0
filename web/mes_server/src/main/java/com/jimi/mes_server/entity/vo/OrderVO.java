package com.jimi.mes_server.entity.vo;

import com.jimi.mes_server.model.Orders;

public class OrderVO extends Orders {

	private static final long serialVersionUID = 5840191253006973812L;

	private Integer unscheduledQuantity;

	public Integer getUnscheduledQuantity() {
		return unscheduledQuantity;
	}

	public void setUnscheduledQuantity(Integer unscheduledQuantity) {
		this.unscheduledQuantity = unscheduledQuantity;
	}

	public OrderVO(Orders order, Integer unscheduledQuantity) {
		this.setUnscheduledQuantity(unscheduledQuantity);
		this.set("unscheduledQuantity", getUnscheduledQuantity());
		this.setAlias(order.getAlias());
		this.setCreateTime(order.getCreateTime());
		this.setCustomerName(order.getCustomerName());
		this.setCustomerNumber(order.getCustomerNumber());
		this.setDeleteReason(order.getDeleteReason());
		this.setDeliveryDate(order.getDeliveryDate());
		this.setId(order.getId());
		this.setOrdersStatus(order.getOrdersStatus());
		this.setProductNo(order.getProductNo());
		this.setQuantity(order.getQuantity());
		this.setRemark(order.getRemark());
		this.setSoftModel(order.getSoftModel());
		this.setVersion(order.getVersion());
		this.setZhidan(order.getZhidan());
	}
}
