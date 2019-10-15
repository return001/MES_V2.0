package com.jimi.mes_server.service;

import org.apache.commons.lang3.StringUtils;

import com.jfinal.kit.StrKit;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Page;
import com.jfinal.plugin.activerecord.Record;
import com.jfinal.plugin.activerecord.SqlPara;
import com.jimi.mes_server.entity.SopSQL;
import com.jimi.mes_server.exception.OperationException;
import com.jimi.mes_server.model.Line;
import com.jimi.mes_server.model.SopCustomer;
import com.jimi.mes_server.model.SopFactory;
import com.jimi.mes_server.model.SopWorkshop;
import com.jimi.mes_server.service.base.SelectService;

public class SopService extends SelectService {

	public boolean addFactory(String alias, String abbreviation, String fullName) {
		SopFactory sopFactory = SopFactory.dao.findFirst(SopSQL.SELECT_FACTORY_ID_BY_COLUMN, alias, abbreviation, fullName);
		if (sopFactory != null) {
			throw new OperationException("数据项已存在,请重新输入");
		}
		sopFactory = new SopFactory();
		sopFactory.setAbbreviation(abbreviation).setFactoryAlias(alias).setFullName(fullName);
		return sopFactory.save();
	}


	public boolean deleteFactory(Integer id) {
		SopFactory sopFactory = SopFactory.dao.findById(id);
		if (sopFactory == null) {
			throw new OperationException("不存在当前工厂");
		}
		SopWorkshop sopWorkshop = SopWorkshop.dao.findFirst(SopSQL.SELECT_WORKSHOP_BY_FACTORY, id);
		if (sopWorkshop != null) {
			throw new OperationException("当前工厂被车间所引用，请先删除车间，再进行操作");
		}
		SopCustomer sopCustomer = SopCustomer.dao.findFirst(SopSQL.SELECT_CUSTOMER_BY_FACTORY, id);
		if (sopCustomer != null) {
			throw new OperationException("当前工厂被客户所引用，请先删除客户，再进行操作");
		}
		Line line = Line.dao.findFirst(SopSQL.SELECT_LINE_BY_FACTORY, id);
		if (line != null) {
			throw new OperationException("当前工厂被产线所引用，请先删除产线，再进行操作");
		}
		return sopFactory.delete();
	}


	public Page<Record> selectFactory(Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter) {
		return select(SopSQL.SELECT_FACTORY, pageNo, pageSize, ascBy, descBy, filter);
	}


	public boolean editFactory(Integer id, String alias, String abbreviation, String fullName) {
		SopFactory sopFactory = SopFactory.dao.findById(id);
		if (sopFactory == null) {
			throw new OperationException("不存在当前工厂");
		}
		if (!StrKit.isBlank(alias)) {
			SopFactory temp = SopFactory.dao.findFirst(SopSQL.SELECT_FACTORY_ID_BY_COLUMN, alias, null, null);
			if (temp != null&&!temp.getId().equals(id)) {
				throw new OperationException("工厂别名已存在,请重新输入");
			}
			sopFactory.setFactoryAlias(alias);
		}
		if (!StrKit.isBlank(abbreviation)) {
			SopFactory temp = SopFactory.dao.findFirst(SopSQL.SELECT_FACTORY_ID_BY_COLUMN, null, abbreviation, null);
			if (temp != null&&!temp.getId().equals(id)) {
				throw new OperationException("工厂简称已存在,请重新输入");
			}
			sopFactory.setAbbreviation(abbreviation);
		}
		if (!StrKit.isBlank(fullName)) {
			SopFactory temp = SopFactory.dao.findFirst(SopSQL.SELECT_FACTORY_ID_BY_COLUMN, null, null, fullName);
			if (temp != null&&!temp.getId().equals(id)) {
				throw new OperationException("工厂全称已存在,请重新输入");
			}
			sopFactory.setFullName(fullName);
		}
		return sopFactory.update();
	}


	public boolean addWorkshop(Integer factoryId, String number, String name) {
		SopWorkshop sopWorkshop = SopWorkshop.dao.findFirst(SopSQL.SELECT_WORKSHOP_BY_COLUMN, name, number);
		if (sopWorkshop != null) {
			throw new OperationException("数据项已存在,请重新输入");
		}
		if (SopFactory.dao.findById(factoryId) == null) {
			throw new OperationException("不存在当前工厂");
		}
		sopWorkshop = new SopWorkshop();
		sopWorkshop.setFactoryId(factoryId).setWorkshopName(name).setWorkshopNumber(number);
		return sopWorkshop.save();
	}


	public boolean deleteWorkshop(Integer id) {
		SopWorkshop sopWorkshop = SopWorkshop.dao.findById(id);
		if (sopWorkshop == null) {
			throw new OperationException("不存在当前车间");
		}
		Line line = Line.dao.findFirst(SopSQL.SELECT_LINE_BY_WORKSHOP, id);
		if (line != null) {
			throw new OperationException("当前车间被产线所引用，请先删除产线，再进行操作");
		}
		return sopWorkshop.delete();
	}


	public Page<Record> selectWorkshop(Integer pageNo, Integer pageSize, Integer factoryId, String number, String name) {
		SqlPara sqlPara = new SqlPara();
		StringBuilder sql = new StringBuilder(SopSQL.SELECT_WORKSHOP_AND_FACTORY_ABBREVIATION);
		if (factoryId != null) {
			sql.append(" AND ").append("w.factory_id = ").append(factoryId);
		}
		if (!StringUtils.isBlank(number)) {
			sql.append(" AND ").append("w.workshop_number like '%").append(number).append("%'");
		}
		if (!StringUtils.isBlank(name)) {
			sql.append(" AND ").append("w.workshop_name like '%").append(name).append("%'");
		}
		sqlPara.setSql(sql.toString());
		return Db.paginate(pageNo, pageSize, sqlPara);
	}


	public boolean editWorkshop(Integer id, Integer factoryId, String number, String name) {
		SopWorkshop sopWorkshop = SopWorkshop.dao.findById(id);
		if (sopWorkshop == null) {
			throw new OperationException("不存在当前车间");
		}
		if (factoryId != null && !factoryId.equals(sopWorkshop.getFactoryId())) {
			if (SopFactory.dao.findById(factoryId) == null) {
				throw new OperationException("不存在当前工厂");
			}
			sopWorkshop.setFactoryId(factoryId);
		}
		if (!StringUtils.isBlank(number)) {
			SopWorkshop temp = SopWorkshop.dao.findFirst(SopSQL.SELECT_WORKSHOP_BY_COLUMN, null, number);
			if (temp != null&&!temp.getId().equals(id)) {
				throw new OperationException("车间编号已存在,请重新输入");
			}
			sopWorkshop.setWorkshopNumber(number);
		}
		if (!StringUtils.isBlank(name)) {
			SopWorkshop temp = SopWorkshop.dao.findFirst(SopSQL.SELECT_WORKSHOP_BY_COLUMN, name, null);
			if (temp != null&&!temp.getId().equals(id)) {
				throw new OperationException("车间名称已存在,请重新输入");
			}
			sopWorkshop.setWorkshopName(name);
		}
		return sopWorkshop.update();
	}

}
