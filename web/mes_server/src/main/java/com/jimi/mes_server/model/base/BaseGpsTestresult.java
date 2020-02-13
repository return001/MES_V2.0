package com.jimi.mes_server.model.base;

import com.jfinal.plugin.activerecord.Model;
import com.jfinal.plugin.activerecord.IBean;

/**
 * Generated by JFinal, do not modify this file.
 */
@SuppressWarnings({"serial", "unchecked"})
public abstract class BaseGpsTestresult<M extends BaseGpsTestresult<M>> extends Model<M> implements IBean {

	public M setId(java.lang.Integer Id) {
		set("Id", Id);
		return (M)this;
	}
	
	public java.lang.Integer getId() {
		return getInt("Id");
	}

	public M setSN(java.lang.String SN) {
		set("SN", SN);
		return (M)this;
	}
	
	public java.lang.String getSN() {
		return getStr("SN");
	}

	public M setIMEI(java.lang.String IMEI) {
		set("IMEI", IMEI);
		return (M)this;
	}
	
	public java.lang.String getIMEI() {
		return getStr("IMEI");
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

	public M setFunctionResult(java.lang.Integer FunctionResult) {
		set("FunctionResult", FunctionResult);
		return (M)this;
	}
	
	public java.lang.Integer getFunctionResult() {
		return getInt("FunctionResult");
	}

	public M setGPSResult(java.lang.Integer GPSResult) {
		set("GPSResult", GPSResult);
		return (M)this;
	}
	
	public java.lang.Integer getGPSResult() {
		return getInt("GPSResult");
	}

	public M setCoupleResult(java.lang.Integer CoupleResult) {
		set("CoupleResult", CoupleResult);
		return (M)this;
	}
	
	public java.lang.Integer getCoupleResult() {
		return getInt("CoupleResult");
	}

	public M setWriteImeiResult(java.lang.Integer WriteImeiResult) {
		set("WriteImeiResult", WriteImeiResult);
		return (M)this;
	}
	
	public java.lang.Integer getWriteImeiResult() {
		return getInt("WriteImeiResult");
	}

	public M setParamDownloadResult(java.lang.Integer ParamDownloadResult) {
		set("ParamDownloadResult", ParamDownloadResult);
		return (M)this;
	}
	
	public java.lang.Integer getParamDownloadResult() {
		return getInt("ParamDownloadResult");
	}

	public M setAutoTestResult(java.lang.Integer AutoTestResult) {
		set("AutoTestResult", AutoTestResult);
		return (M)this;
	}
	
	public java.lang.Integer getAutoTestResult() {
		return getInt("AutoTestResult");
	}

	public M setResult(java.lang.Integer Result) {
		set("Result", Result);
		return (M)this;
	}
	
	public java.lang.Integer getResult() {
		return getInt("Result");
	}

	public M setAutoTestSMTResult(java.lang.Integer AutoTestSMTResult) {
		set("AutoTestSMTResult", AutoTestSMTResult);
		return (M)this;
	}
	
	public java.lang.Integer getAutoTestSMTResult() {
		return getInt("AutoTestSMTResult");
	}

	public M setSMTIQCResult(java.lang.Integer SMTIQCResult) {
		set("SMTIQCResult", SMTIQCResult);
		return (M)this;
	}
	
	public java.lang.Integer getSMTIQCResult() {
		return getInt("SMTIQCResult");
	}

	public M setOtherTestSign(java.lang.String OtherTestSign) {
		set("OtherTestSign", OtherTestSign);
		return (M)this;
	}
	
	public java.lang.String getOtherTestSign() {
		return getStr("OtherTestSign");
	}

	public M setZhiDan(java.lang.String ZhiDan) {
		set("ZhiDan", ZhiDan);
		return (M)this;
	}
	
	public java.lang.String getZhiDan() {
		return getStr("ZhiDan");
	}

	public M setRecordTime(java.util.Date RecordTime) {
		set("RecordTime", RecordTime);
		return (M)this;
	}
	
	public java.util.Date getRecordTime() {
		return get("RecordTime");
	}

	public M setMaskFromV2(byte[] MaskFromV2) {
		set("_MASK_FROM_V2", MaskFromV2);
		return (M)this;
	}
	
	public byte[] getMaskFromV2() {
		return get("_MASK_FROM_V2");
	}

	public M setCPResult(java.lang.Integer CPResult) {
		set("CPResult", CPResult);
		return (M)this;
	}
	
	public java.lang.Integer getCPResult() {
		return getInt("CPResult");
	}

	public M setWeightResult(java.lang.Integer WeightResult) {
		set("WeightResult", WeightResult);
		return (M)this;
	}
	
	public java.lang.Integer getWeightResult() {
		return getInt("WeightResult");
	}

	public M setOQCResult01(java.lang.Integer OQCResult01) {
		set("OQCResult01", OQCResult01);
		return (M)this;
	}
	
	public java.lang.Integer getOQCResult01() {
		return getInt("OQCResult01");
	}

	public M setOQCResult02(java.lang.Integer OQCResult02) {
		set("OQCResult02", OQCResult02);
		return (M)this;
	}
	
	public java.lang.Integer getOQCResult02() {
		return getInt("OQCResult02");
	}

	public M setNBCalResult(java.lang.Integer NBCalResult) {
		set("NBCalResult", NBCalResult);
		return (M)this;
	}
	
	public java.lang.Integer getNBCalResult() {
		return getInt("NBCalResult");
	}

	public M setNBCouResult(java.lang.Integer NBCouResult) {
		set("NBCouResult", NBCouResult);
		return (M)this;
	}
	
	public java.lang.Integer getNBCouResult() {
		return getInt("NBCouResult");
	}

	public M setRfidResult(java.lang.Integer RfidResult) {
		set("RfidResult", RfidResult);
		return (M)this;
	}
	
	public java.lang.Integer getRfidResult() {
		return getInt("RfidResult");
	}

}
