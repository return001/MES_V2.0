package com.jimi.mes_server.entity.vo;

import com.jimi.mes_server.model.Orders;

/**订单VO
 * @author   HCJ
 * @date     2019年8月16日 下午2:32:52
 */
public class OrderVO extends Orders {

	private static final long serialVersionUID = 5840191253006973812L;


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


	public OrderVO(Orders order, Integer unscheduledQuantity, Integer capacity) {
		this.setUnscheduledQuantity(unscheduledQuantity);
		this.setCapacity(capacity);
		this.set("alias", order.getAlias());
		this.set("orderDate", order.getOrderDate());
		this.set("customerName", order.getCustomerName());
		this.set("customerNumber", order.getCustomerNumber());
		this.set("deliveryDate", order.getDeliveryDate());
		this.set("id", order.getId());
		this.set("productNo", order.getProductNo());
		this.set("quantity", order.getQuantity());
		this.set("remark", order.getRemark());
		this.set("softModel", order.getSoftModel());
		this.set("zhidan", order.getZhidan());
	}
}
