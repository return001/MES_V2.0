package com.jimi.mes_server.model.base;

import com.jfinal.plugin.activerecord.Model;
import com.jfinal.plugin.activerecord.IBean;

/**
 * Generated by JFinal, do not modify this file.
 */
@SuppressWarnings({"serial", "unchecked"})
public abstract class BaseGpsManuprintparam<M extends BaseGpsManuprintparam<M>> extends Model<M> implements IBean {

	public M setID(java.lang.Integer ID) {
		set("ID", ID);
		return (M)this;
	}
	
	public java.lang.Integer getID() {
		return getInt("ID");
	}

	public M setZhiDan(java.lang.String ZhiDan) {
		set("ZhiDan", ZhiDan);
		return (M)this;
	}
	
	public java.lang.String getZhiDan() {
		return getStr("ZhiDan");
	}

	public M setIMEI(java.lang.String IMEI) {
		set("IMEI", IMEI);
		return (M)this;
	}
	
	public java.lang.String getIMEI() {
		return getStr("IMEI");
	}

	public M setIMEIStart(java.lang.String IMEIStart) {
		set("IMEIStart", IMEIStart);
		return (M)this;
	}
	
	public java.lang.String getIMEIStart() {
		return getStr("IMEIStart");
	}

	public M setIMEIEnd(java.lang.String IMEIEnd) {
		set("IMEIEnd", IMEIEnd);
		return (M)this;
	}
	
	public java.lang.String getIMEIEnd() {
		return getStr("IMEIEnd");
	}

	public M setSN(java.lang.String SN) {
		set("SN", SN);
		return (M)this;
	}
	
	public java.lang.String getSN() {
		return getStr("SN");
	}

	public M setIMEIRel(java.lang.String IMEIRel) {
		set("IMEIRel", IMEIRel);
		return (M)this;
	}
	
	public java.lang.String getIMEIRel() {
		return getStr("IMEIRel");
	}

	public M setSIM(java.lang.String SIM) {
		set("SIM", SIM);
		return (M)this;
	}
	
	public java.lang.String getSIM() {
		return getStr("SIM");
	}

	public M setVIP(java.lang.String VIP) {
		set("VIP", VIP);
		return (M)this;
	}
	
	public java.lang.String getVIP() {
		return getStr("VIP");
	}

	public M setBAT(java.lang.String BAT) {
		set("BAT", BAT);
		return (M)this;
	}
	
	public java.lang.String getBAT() {
		return getStr("BAT");
	}

	public M setSoftModel(java.lang.String SoftModel) {
		set("SoftModel", SoftModel);
		return (M)this;
	}
	
	public java.lang.String getSoftModel() {
		return getStr("SoftModel");
	}

	public M setVersion(java.lang.String Version) {
		set("Version", Version);
		return (M)this;
	}
	
	public java.lang.String getVersion() {
		return getStr("Version");
	}

	public M setRemark(java.lang.String Remark) {
		set("Remark", Remark);
		return (M)this;
	}
	
	public java.lang.String getRemark() {
		return getStr("Remark");
	}

	public M setJsPrinttime(java.lang.String jsPrinttime) {
		set("JS_PrintTime", jsPrinttime);
		return (M)this;
	}
	
	public java.lang.String getJsPrinttime() {
		return getStr("JS_PrintTime");
	}

	public M setJsTemplatepath(java.lang.String jsTemplatepath) {
		set("JS_TemplatePath", jsTemplatepath);
		return (M)this;
	}
	
	public java.lang.String getJsTemplatepath() {
		return getStr("JS_TemplatePath");
	}

	public M setJsReprintnum(java.lang.Integer jsReprintnum) {
		set("JS_RePrintNum", jsReprintnum);
		return (M)this;
	}
	
	public java.lang.Integer getJsReprintnum() {
		return getInt("JS_RePrintNum");
	}

	public M setJsRefirstprinttime(java.util.Date jsRefirstprinttime) {
		set("JS_ReFirstPrintTime", jsRefirstprinttime);
		return (M)this;
	}
	
	public java.util.Date getJsRefirstprinttime() {
		return get("JS_ReFirstPrintTime");
	}

	public M setJsReendprinttime(java.util.Date jsReendprinttime) {
		set("JS_ReEndPrintTime", jsReendprinttime);
		return (M)this;
	}
	
	public java.util.Date getJsReendprinttime() {
		return get("JS_ReEndPrintTime");
	}

	public M setUserName(java.lang.String UserName) {
		set("UserName", UserName);
		return (M)this;
	}
	
	public java.lang.String getUserName() {
		return getStr("UserName");
	}

	public M setChPrinttime(java.lang.String chPrinttime) {
		set("CH_PrintTime", chPrinttime);
		return (M)this;
	}
	
	public java.lang.String getChPrinttime() {
		return getStr("CH_PrintTime");
	}

	public M setChTemplatepath1(java.lang.String chTemplatepath1) {
		set("CH_TemplatePath1", chTemplatepath1);
		return (M)this;
	}
	
	public java.lang.String getChTemplatepath1() {
		return getStr("CH_TemplatePath1");
	}

	public M setChTemplatepath2(java.lang.String chTemplatepath2) {
		set("CH_TemplatePath2", chTemplatepath2);
		return (M)this;
	}
	
	public java.lang.String getChTemplatepath2() {
		return getStr("CH_TemplatePath2");
	}

	public M setChReprintnum(java.lang.Integer chReprintnum) {
		set("CH_RePrintNum", chReprintnum);
		return (M)this;
	}
	
	public java.lang.Integer getChReprintnum() {
		return getInt("CH_RePrintNum");
	}

	public M setChRefirstprinttime(java.util.Date chRefirstprinttime) {
		set("CH_ReFirstPrintTime", chRefirstprinttime);
		return (M)this;
	}
	
	public java.util.Date getChRefirstprinttime() {
		return get("CH_ReFirstPrintTime");
	}

	public M setChReendprinttime(java.util.Date chReendprinttime) {
		set("CH_ReEndPrintTime", chReendprinttime);
		return (M)this;
	}
	
	public java.util.Date getChReendprinttime() {
		return get("CH_ReEndPrintTime");
	}

	public M setICCID(java.lang.String ICCID) {
		set("ICCID", ICCID);
		return (M)this;
	}
	
	public java.lang.String getICCID() {
		return getStr("ICCID");
	}

	public M setMAC(java.lang.String MAC) {
		set("MAC", MAC);
		return (M)this;
	}
	
	public java.lang.String getMAC() {
		return getStr("MAC");
	}

	public M setEquipment(java.lang.String Equipment) {
		set("Equipment", Equipment);
		return (M)this;
	}
	
	public java.lang.String getEquipment() {
		return getStr("Equipment");
	}

	public M setRFID(java.lang.String RFID) {
		set("RFID", RFID);
		return (M)this;
	}
	
	public java.lang.String getRFID() {
		return getStr("RFID");
	}

}