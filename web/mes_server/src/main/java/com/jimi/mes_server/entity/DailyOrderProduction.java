package com.jimi.mes_server.entity;

public class DailyOrderProduction {

	private String zhidan;
	private String version;
	private String softModel;
	private Integer functionProduct;
	private Integer smtProduct;
	private Integer agedProduct;
	private Integer couplingProduct;
	private Integer cartonProduct;
	private Integer chPrintProduct;
	private Integer jsPrintProduct;

	public String getZhidan() {
		return zhidan;
	}

	public void setZhidan(String zhidan) {
		this.zhidan = zhidan;
	}

	public String getVersion() {
		return version;
	}

	public void setVersion(String version) {
		this.version = version;
	}

	public String getSoftModel() {
		return softModel;
	}

	public void setSoftModel(String softModel) {
		this.softModel = softModel;
	}

	public Integer getFunctionProduct() {
		return functionProduct;
	}

	public void setFunctionProduct(Integer functionProduct) {
		this.functionProduct = functionProduct;
	}

	public Integer getSmtProduct() {
		return smtProduct;
	}

	public void setSmtProduct(Integer smtProduct) {
		this.smtProduct = smtProduct;
	}

	public Integer getAgedProduct() {
		return agedProduct;
	}

	public void setAgedProduct(Integer agedProduct) {
		this.agedProduct = agedProduct;
	}

	public Integer getCouplingProduct() {
		return couplingProduct;
	}

	public void setCouplingProduct(Integer couplingProduct) {
		this.couplingProduct = couplingProduct;
	}

	public Integer getCartonProduct() {
		return cartonProduct;
	}

	public void setCartonProduct(Integer cartonProduct) {
		this.cartonProduct = cartonProduct;
	}

	public Integer getChPrintProduct() {
		return chPrintProduct;
	}

	public void setChPrintProduct(Integer chPrintProduct) {
		this.chPrintProduct = chPrintProduct;
	}

	public Integer getJsPrintProduct() {
		return jsPrintProduct;
	}

	public void setJsPrintProduct(Integer jsPrintProduct) {
		this.jsPrintProduct = jsPrintProduct;
	}

	public DailyOrderProduction(String zhidan, String version, String softModel, Integer functionProduct,
			Integer smtProduct, Integer agedProduct, Integer couplingProduct, Integer cartonProduct,
			Integer chPrintProduct, Integer jsPrintProduct) {
		super();
		this.zhidan = zhidan;
		this.version = version;
		this.softModel = softModel;
		this.functionProduct = functionProduct;
		this.smtProduct = smtProduct;
		this.agedProduct = agedProduct;
		this.couplingProduct = couplingProduct;
		this.cartonProduct = cartonProduct;
		this.chPrintProduct = chPrintProduct;
		this.jsPrintProduct = jsPrintProduct;
	}

}
