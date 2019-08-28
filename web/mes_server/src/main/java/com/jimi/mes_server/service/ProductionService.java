package com.jimi.mes_server.service;

import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.math.BigDecimal;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import javax.servlet.http.HttpServletResponse;

import org.apache.commons.io.FileUtils;
import org.apache.commons.lang3.StringUtils;

import com.jfinal.aop.Enhancer;
import com.jfinal.kit.StrKit;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Page;
import com.jfinal.plugin.activerecord.Record;
import com.jfinal.plugin.activerecord.SqlPara;
import com.jfinal.upload.UploadFile;
import com.jimi.mes_server.entity.AddPlanInfo;
import com.jimi.mes_server.entity.Constant;
import com.jimi.mes_server.entity.OrderDetail;
import com.jimi.mes_server.entity.OrderFileInfo;
import com.jimi.mes_server.entity.OrderItem;
import com.jimi.mes_server.entity.PlanDetail;
import com.jimi.mes_server.entity.PlanGantt;
import com.jimi.mes_server.entity.PlanQueryCriteria;
import com.jimi.mes_server.entity.SQL;
import com.jimi.mes_server.entity.vo.LUserAccountVO;
import com.jimi.mes_server.entity.vo.OrderVO;
import com.jimi.mes_server.exception.OperationException;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.model.LUserAccount;
import com.jimi.mes_server.model.Line;
import com.jimi.mes_server.model.LineComputer;
import com.jimi.mes_server.model.ModelCapacity;
import com.jimi.mes_server.model.OrderFile;
import com.jimi.mes_server.model.Orders;
import com.jimi.mes_server.model.Process;
import com.jimi.mes_server.model.ProcessGroup;
import com.jimi.mes_server.model.SchedulingPlan;
import com.jimi.mes_server.service.base.SelectService;
import com.jimi.mes_server.util.CommonUtil;
import com.jimi.mes_server.util.ExcelHelper;

/**排产模块业务层
 * @author   HCJ
 * @date     2019年8月16日 上午11:53:55
 */
public class ProductionService {

	private static SelectService daoService = Enhancer.enhance(SelectService.class);


	public Page<Record> getProcessGroup() {
		SqlPara sqlPara = new SqlPara();
		sqlPara.setSql(SQL.SELECT_PROCESSGROUP_NAME_ID);
		return Db.paginate(Constant.DEFAULT_PAGE_NUM, Constant.DEFAULT_PAGE_SIZE, sqlPara);
	}


	public Page<Record> getProcess() {
		SqlPara sqlPara = new SqlPara();
		sqlPara.setSql(SQL.SELECT_PROCESS_NAME_ID);
		return Db.paginate(Constant.DEFAULT_PAGE_NUM, Constant.DEFAULT_PAGE_SIZE, sqlPara);
	}


	public Page<Record> getLine() {
		SqlPara sqlPara = new SqlPara();
		sqlPara.setSql(SQL.SELECT_LINE_NAME_ID);
		return Db.paginate(Constant.DEFAULT_PAGE_NUM, Constant.DEFAULT_PAGE_SIZE, sqlPara);
	}


	public Page<Record> getOrderStatus() {
		SqlPara sqlPara = new SqlPara();
		sqlPara.setSql(SQL.SELECT_ORDERSTATUS_NAME_ID);
		return Db.paginate(Constant.DEFAULT_PAGE_NUM, Constant.DEFAULT_PAGE_SIZE, sqlPara);
	}


	public Page<Record> getSchedulingPlanStatus() {
		SqlPara sqlPara = new SqlPara();
		sqlPara.setSql(SQL.SELECT_SCHEDULINGPLANSTATUS_NAME_ID);
		return Db.paginate(Constant.DEFAULT_PAGE_NUM, Constant.DEFAULT_PAGE_SIZE, sqlPara);
	}


	public boolean addProcessGroup(String groupNo, String groupName, String groupRemark) {
		if (ProcessGroup.dao.findFirst(SQL.SELECT_PROCESSGROUP_BY_GROUPNO, groupNo) != null) {
			throw new OperationException("工序组编号已存在");
		}
		if (ProcessGroup.dao.findFirst(SQL.SELECT_PROCESSGROUP_BY_GROUPNAME, groupName) != null) {
			throw new OperationException("工序组名称已存在");
		}
		ProcessGroup processGroup = new ProcessGroup();
		processGroup.setGroupNo(groupNo).setGroupName(groupName);
		if (!StrKit.isBlank(groupRemark)) {
			processGroup.setGroupRemark(groupRemark);
		}
		processGroup.save();
		return processGroup.setPosition(processGroup.getId()).update();
	}


	public boolean deleteProcessGroup(Integer id) {
		ProcessGroup processGroup = ProcessGroup.dao.findById(id);
		if (processGroup == null) {
			throw new OperationException("删除失败，项目组不存在");
		}
		return processGroup.delete();
	}


	public Page<Record> selectProcessGroup(String filter) {
		return daoService.select(SQL.SELECT_PROCESSGROUP, Constant.DEFAULT_PAGE_NUM, Constant.DEFAULT_PAGE_SIZE, "position", null, filter);
	}


	public boolean editProcessGroup(Integer id, String groupNo, String groupName, String groupRemark, Integer position) {
		ProcessGroup processGroup = ProcessGroup.dao.findById(id);
		if (position != null) {
			ProcessGroup secondProcessGroup = ProcessGroup.dao.findById(position);
			if (secondProcessGroup == null) {
				throw new OperationException("需要进行交换位置的工序组不存在");
			}
			Db.update(SQL.UPDATE_PROCESSGROUP_POSITION, processGroup.getPosition(), secondProcessGroup.getId());
			Db.update(SQL.UPDATE_PROCESSGROUP_POSITION, secondProcessGroup.getPosition(), processGroup.getId());
			return true;
		}
		if (processGroup == null) {
			throw new OperationException("修改失败，项目组不存在");
		}
		if (!StrKit.isBlank(groupNo)) {
			ProcessGroup group = ProcessGroup.dao.findFirst(SQL.SELECT_PROCESSGROUP_BY_GROUPNO, groupNo);
			if (group != null && !group.getId().equals(id)) {
				throw new OperationException("工序组编号已存在");
			}
			processGroup.setGroupNo(groupNo);
		}
		if (!StrKit.isBlank(groupName)) {
			ProcessGroup group = ProcessGroup.dao.findFirst(SQL.SELECT_PROCESSGROUP_BY_GROUPNAME, groupName);
			if (group != null && !group.getId().equals(id)) {
				throw new OperationException("工序组名称已存在");
			}
			processGroup.setGroupName(groupName);
		}
		if (groupRemark != null) {
			processGroup.setGroupRemark(groupRemark);
		}
		return processGroup.update();
	}


	public boolean addLine(String lineNo, String lineName, String lineRemark, Integer lineDirector, Integer lineEngineer, Integer lineQc, Integer processGroup) {
		if (Line.dao.findFirst(SQL.SELECT_LINE_BY_LINENO, lineNo) != null) {
			throw new OperationException("产线线别已存在");
		}
		if (Line.dao.findFirst(SQL.SELECT_LINE_BY_LINENAME, lineName) != null) {
			throw new OperationException("产线名称已存在");
		}
		Line line = new Line();
		line.setLineNo(lineNo).setLineName(lineName);
		if (!StrKit.isBlank(lineRemark)) {
			line.setLineRemark(lineRemark);
		}
		if (lineDirector != null) {
			checkUserById(lineDirector);
			line.setLineDirector(lineDirector);
		}
		if (lineEngineer != null) {
			checkUserById(lineEngineer);
			line.setLineEngineer(lineEngineer);
		}
		if (lineQc != null) {
			checkUserById(lineQc);
			line.setLineQc(lineQc);
		}
		if (processGroup != null) {
			if (ProcessGroup.dao.findById(processGroup) == null) {
				throw new OperationException("工序组不存在");
			}
			line.setProcessGroup(processGroup);
		}
		return line.save();
	}


	public boolean deleteLine(Integer id) {
		Line line = Line.dao.findById(id);
		if (line == null) {
			throw new OperationException("删除失败，产线不存在");
		}
		return line.delete();
	}


	public Page<Record> selectLine(String lineNo, String lineName, Integer processGroup) {
		StringBuilder filter = new StringBuilder();
		if (!StrKit.isBlank(lineNo)) {
			filter.append(" and line_no like '%" + lineNo + "%'");
		}
		if (!StrKit.isBlank(lineName)) {
			filter.append(" and line_name like '%" + lineName + "%'");
		}
		if (processGroup != null) {
			if (ProcessGroup.dao.findById(processGroup) == null) {
				throw new OperationException("工序组不存在");
			}
			filter.append(" and process_group = " + processGroup);
		}
		SqlPara sqlPara = new SqlPara();
		String orderBy = " order by processGroup ";
		sqlPara.setSql(SQL.SELECT_LINE_PROCESSGROUP + filter + orderBy);
		return Db.paginate(Constant.DEFAULT_PAGE_NUM, Constant.DEFAULT_PAGE_SIZE, sqlPara);
	}


	public boolean editLine(Integer id, String lineNo, String lineName, String lineRemark, Integer lineDirector, Integer lineEngineer, Integer lineQc, Integer processGroup) {
		Line line = Line.dao.findById(id);
		if (line == null) {
			throw new OperationException("修改失败，产线不存在");
		}
		if (!StrKit.isBlank(lineNo)) {
			Line temp = Line.dao.findFirst(SQL.SELECT_LINE_BY_LINENO, lineNo);
			if (temp != null && !temp.getId().equals(id)) {
				throw new OperationException("产线线别已存在");
			}
			line.setLineNo(lineNo);
		}
		if (!StrKit.isBlank(lineName)) {
			Line temp = Line.dao.findFirst(SQL.SELECT_LINE_BY_LINENAME, lineName);
			if (temp != null && !temp.getId().equals(id)) {
				throw new OperationException("产线名称已存在");
			}
			line.setLineName(lineName);
		}
		if (lineRemark != null) {
			line.setLineRemark(lineRemark);
		}
		if (lineDirector != null) {
			checkUserById(lineDirector);
			line.setLineDirector(lineDirector);
		}
		if (lineEngineer != null) {
			checkUserById(lineEngineer);
			line.setLineEngineer(lineEngineer);
		}
		if (lineQc != null) {
			checkUserById(lineQc);
			line.setLineQc(lineQc);
		}
		if (processGroup != null) {
			if (ProcessGroup.dao.findById(processGroup) == null) {
				throw new OperationException("工序组不存在");
			}
			line.setProcessGroup(processGroup);
		}
		return line.update();
	}


	public boolean addProcess(String processNo, String processName, String processRemark, Integer processGroup) {
		if (Process.dao.findFirst(SQL.SELECT_PROCESS_BY_PROCESSNO, processNo) != null) {
			throw new OperationException("工序编号已存在");
		}
		if (Process.dao.findFirst(SQL.SELECT_PROCESS_BY_PROCESSNAME, processName) != null) {
			throw new OperationException("工序名称已存在");
		}
		Process process = new Process();
		process.setProcessNo(processNo).setProcessName(processName);
		if (!StrKit.isBlank(processRemark)) {
			process.setProcessRemark(processRemark);
		}
		if (processGroup != null) {
			if (ProcessGroup.dao.findById(processGroup) == null) {
				throw new OperationException("工序组不存在");
			}
			process.setProcessGroup(processGroup);
		}
		process.save();
		return process.setPosition(process.getId()).update();
	}


	public boolean deleteProcess(Integer id) {
		Process process = Process.dao.findById(id);
		if (process == null) {
			throw new OperationException("删除失败，工序不存在");
		}
		return process.delete();
	}


	public Page<Record> selectProcess(String processNo, String processName, Integer processGroup) {
		StringBuilder filter = new StringBuilder();
		if (!StrKit.isBlank(processNo)) {
			filter.append(" and process_no like '%" + processNo + "%' ");
		}
		if (!StrKit.isBlank(processName)) {
			filter.append(" and process_name like '%" + processName + "%' ");
		}
		if (processGroup != null) {
			if (ProcessGroup.dao.findById(processGroup) == null) {
				throw new OperationException("工序组不存在");
			}
			filter.append(" and process_group = " + processGroup);
		}
		SqlPara sqlPara = new SqlPara();
		String orderBy = " order by processGroup,process.position ";
		sqlPara.setSql(SQL.SELECT_PROCESS_PROCESSGROUP + filter + orderBy);
		return Db.paginate(Constant.DEFAULT_PAGE_NUM, Constant.DEFAULT_PAGE_SIZE, sqlPara);
	}


	public boolean editProcess(Integer id, String processNo, String processName, String processRemark, Integer processGroup, Integer position) {
		Process process = Process.dao.findById(id);
		if (process == null) {
			throw new OperationException("修改失败，工序不存在");
		}
		if (position != null) {
			Process secondProcess = Process.dao.findById(position);
			if (secondProcess == null) {
				throw new OperationException("需要进行交换位置的工序组不存在");
			}
			Db.update(SQL.UPDATE_PROCESS_POSITION, process.getPosition(), secondProcess.getId());
			Db.update(SQL.UPDATE_PROCESS_POSITION, secondProcess.getPosition(), process.getId());
			return true;
		}
		if (!StrKit.isBlank(processNo)) {
			Process temp = Process.dao.findFirst(SQL.SELECT_PROCESS_BY_PROCESSNO, processNo);
			if (temp != null && !temp.getId().equals(id)) {
				throw new OperationException("工序编号已存在");
			}
			process.setProcessNo(processNo);
		}
		if (!StrKit.isBlank(processName)) {
			Process temp = Process.dao.findFirst(SQL.SELECT_PROCESS_BY_PROCESSNAME, processName);
			if (temp != null && !temp.getId().equals(id)) {
				throw new OperationException("工序名称已存在");
			}
			process.setProcessName(processName);
		}
		if (processRemark != null) {
			process.setProcessRemark(processRemark);
		}
		if (processGroup != null) {
			if (ProcessGroup.dao.findById(processGroup) == null) {
				throw new OperationException("工序组不存在");
			}
			process.setProcessGroup(processGroup);
		}
		return process.update();
	}


	public boolean addComputer(String ip, String computerName, String remark, Integer line) {
		if (Line.dao.findById(line) == null) {
			throw new OperationException("产线不存在");
		}
		LineComputer computer = LineComputer.dao.findFirst(SQL.SELECT_LINECOMPUTER_BY_IP, ip);
		if (computer != null) {
			Line temp = Line.dao.findById(computer.getLine());
			if (temp != null) {
				throw new OperationException("此IP地址已被产线： " + temp.getLineName() + " 所使用");
			} else {
				throw new OperationException("此IP地址已被使用");
			}
		}
		LineComputer lineComputer = new LineComputer();
		lineComputer.setIp(ip).setLine(line);
		if (!StrKit.isBlank(computerName)) {
			lineComputer.setComputerName(computerName);
		}
		if (!StrKit.isBlank(remark)) {
			lineComputer.setRemark(remark);
		}
		return lineComputer.save();
	}


	public boolean deleteComputer(Integer id) {
		LineComputer lineComputer = LineComputer.dao.findById(id);
		if (lineComputer == null) {
			throw new OperationException("删除失败，产线电脑不存在");
		}
		return lineComputer.delete();
	}


	public Page<Record> selectComputer(Integer lineId) {
		if (Line.dao.findById(lineId) == null) {
			throw new OperationException("产线不存在");
		}
		SqlPara sqlPara = new SqlPara();
		sqlPara.setSql(SQL.SELECT_LINECOMPUTER_BY_LINE);
		sqlPara.addPara(lineId);
		return Db.paginate(Constant.DEFAULT_PAGE_NUM, Constant.DEFAULT_PAGE_SIZE, sqlPara);
	}


	public boolean editComputer(Integer id, String ip, String computerName, String remark) {
		LineComputer lineComputer = LineComputer.dao.findById(id);
		if (lineComputer == null) {
			throw new OperationException("产线电脑不存在");
		}
		LineComputer computer = LineComputer.dao.findFirst(SQL.SELECT_LINECOMPUTER_BY_IP, ip);
		if (computer != null && !computer.getId().equals(id)) {
			Line line = Line.dao.findById(computer.getLine());
			if (line != null) {
				throw new OperationException("此IP地址已被产线： " + line.getLineName() + " 所使用");
			} else {
				throw new OperationException("此IP地址已被使用");
			}
		}
		lineComputer.setIp(ip);
		if (!StrKit.isBlank(computerName)) {
			lineComputer.setComputerName(computerName);
		}
		if (remark != null) {
			lineComputer.setRemark(remark);
		}
		return lineComputer.update();
	}


	public boolean addCapacity(String softModel, String customerModel, Integer process, Integer processGroup, Integer processPeopleQuantity, Integer capacity, String remark, Integer rhythm) {
		ModelCapacity modelCapacity = ModelCapacity.dao.findFirst(SQL.SELECT_MODELCAPACITY_BY_MODEL_PROCESS, softModel, process, processGroup);
		if (modelCapacity != null) {
			throw new OperationException("机型产能已存在");
		}
		modelCapacity = new ModelCapacity();
		if (!StrKit.isBlank(remark)) {
			modelCapacity.setRemark(remark);
		}
		modelCapacity.setSoftModel(softModel).setCustomerModel(customerModel).setProcess(process).setProcessGroup(processGroup);
		modelCapacity.setProcessPeopleQuantity(processPeopleQuantity).setCapacity(capacity).save();
		modelCapacity.setPosition(modelCapacity.getId()).update();
		Db.update(SQL.UPDATE_MODELCAPACITY_BY_MODEL_PROCESS, rhythm, processGroup, softModel);
		return true;
	}


	public boolean deleteCapacity(Integer id) {
		ModelCapacity modelCapacity = ModelCapacity.dao.findById(id);
		if (modelCapacity == null) {
			throw new OperationException("删除失败，机型产能不存在");
		}
		return modelCapacity.delete();
	}


	public Page<Record> selectCapacity(Integer pageNo, Integer pageSize, String softModel, String customerModel, Integer process) {
		StringBuilder filter = new StringBuilder();
		if (!StrKit.isBlank(softModel)) {
			filter.append(" and soft_model like '%" + softModel + "%' ");
		}
		if (!StrKit.isBlank(customerModel)) {
			filter.append(" and customer_model like '%" + customerModel + "%' ");
		}
		if (process != null) {
			if (Process.dao.findById(process) == null) {
				throw new OperationException("工序不存在");
			}
			filter.append(" and process = " + process);
		}
		String orderBy = "ORDER BY soft_model,process_group.[position],model_capacity.[position]";
		SqlPara sqlPara = new SqlPara();
		sqlPara.setSql(SQL.SELECT_MODELCAPACITY + filter + orderBy);
		return Db.paginate(pageNo, pageSize, sqlPara);
	}


	public boolean editCapacity(Integer id, String softModel, String customerModel, Integer process, Integer processGroup, Integer processPeopleQuantity, Integer capacity, String remark, Integer position, Integer rhythm) {
		ModelCapacity firstModelCapacity = ModelCapacity.dao.findById(id);
		if (firstModelCapacity == null) {
			throw new OperationException("机型产能不存在");
		}
		if (position != null) {
			ModelCapacity secondModelCapacity = ModelCapacity.dao.findById(position);
			if (secondModelCapacity == null) {
				throw new OperationException("机型产能不存在");
			}
			if (!secondModelCapacity.getSoftModel().equals(firstModelCapacity.getSoftModel())) {
				throw new OperationException("同一个机型才可移动位置");
			}
			if (!secondModelCapacity.getProcessGroup().equals(firstModelCapacity.getProcessGroup())) {
				throw new OperationException("同一个工序组才可移动位置");
			}
			Db.update(SQL.UPDATE_MODELCAPACITY_POSITION, firstModelCapacity.getPosition(), secondModelCapacity.getId());
			Db.update(SQL.UPDATE_MODELCAPACITY_POSITION, secondModelCapacity.getPosition(), firstModelCapacity.getId());
			return true;
		}
		if (!StrKit.isBlank(softModel) && process != null && processGroup != null) {
			ModelCapacity modelCapacity = ModelCapacity.dao.findFirst(SQL.SELECT_MODELCAPACITY_BY_MODEL_PROCESS, softModel, process, processGroup);
			if (modelCapacity != null && !modelCapacity.getId().equals(id)) {
				throw new OperationException("机型产能已存在");
			}
		}
		if (!StrKit.isBlank(softModel)) {
			firstModelCapacity.setSoftModel(softModel);
		}
		if (process != null) {
			if (Process.dao.findById(process) == null) {
				throw new OperationException("工序不存在");
			}
			firstModelCapacity.setProcess(process);
		}
		if (processGroup != null) {
			if (ProcessGroup.dao.findById(processGroup) == null) {
				throw new OperationException("工序组不存在");
			}
			firstModelCapacity.setProcessGroup(processGroup);
		}
		if (customerModel != null) {
			firstModelCapacity.setCustomerModel(customerModel);
		}
		if (processPeopleQuantity != null) {
			if (processPeopleQuantity < Constant.INTEGER_ZERO) {
				throw new ParameterException("人数不合理");
			}
			firstModelCapacity.setProcessPeopleQuantity(processPeopleQuantity);
		}
		if (capacity != null) {
			if (capacity < Constant.INTEGER_ZERO) {
				throw new ParameterException("产能不合理");
			}
			firstModelCapacity.setCapacity(capacity);
		}
		if (remark != null) {
			firstModelCapacity.setRemark(remark);
		}
		firstModelCapacity.update();
		if (rhythm != null && processGroup != null && softModel != null) {
			Db.update(SQL.UPDATE_MODELCAPACITY_BY_MODEL_PROCESS, rhythm, processGroup, softModel);
		}
		return true;
	}


	public boolean addOrder(Orders order, LUserAccountVO userVO) {
		Orders temp = Orders.dao.findFirst(SQL.SELECT_ORDER_BY_ZHIDAN, order.getZhidan());
		if (temp != null) {
			throw new OperationException("订单号已存在");
		}
		order.setOrderStatus(Constant.UNSCHEDULED_ORDERSTATUS);
		order.setOrderCreator(userVO.getId()).setOrderCreateTime(new Date());
		return order.save();
	}


	public boolean deleteOrder(Integer id, String deleteReason, LUserAccountVO userVO) {
		Orders order = Orders.dao.findById(id);
		if (order == null) {
			throw new OperationException("删除失败，订单不存在");
		}
		if (!Constant.UNSCHEDULED_ORDERSTATUS.equals(order.getOrderStatus())) {
			throw new OperationException("删除失败，只能删除未排产订单");
		}
		checkOrderUserType(userVO, order);
		order.setDeleteReason(deleteReason).setOrderStatus(Constant.DELETED_ORDERSTATUS);
		order.setDeletePerson(userVO.getId()).setDeleteTime(new Date());
		return order.update();
	}


	public Page<Record> selectOrder(Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter, Boolean isRework, LUserAccountVO userVO) {
		Page<Record> page = new Page<>();
		Boolean isOperator = false;
		if (Constant.SUPER_OPERATOR_USERTYPE.equals(userVO.getWebUserType())) {
			isOperator = true;
		}
		if (!isRework) {
			String reworkSql = "AND is_rework = 0 ";
			if (StrKit.isBlank(filter)) {
				page = daoService.select(SQL.SELECT_ORDER + reworkSql, pageNo, pageSize, ascBy, descBy, filter);
			} else {
				page = daoService.select(SQL.SELECT_ORDER + reworkSql + " AND ", pageNo, pageSize, ascBy, descBy, filter);
			}
		} else {
			String reworkSql = "AND is_rework = 1 ";
			if (StrKit.isBlank(filter)) {
				page = daoService.select(SQL.SELECT_ORDER + reworkSql, pageNo, pageSize, ascBy, descBy, filter);
			} else {
				page = daoService.select(SQL.SELECT_ORDER + reworkSql + " AND ", pageNo, pageSize, ascBy, descBy, filter);
			}
		}
		return formatOrderDateAndCustomer(page, isOperator);
	}


	public OrderDetail selectOrderDetail(Integer id) {
		OrderDetail detail = new OrderDetail();
		List<Record> information = new ArrayList<>();
		List<Record> bom = new ArrayList<>();
		List<Record> sop = new ArrayList<>();
		List<Record> orderFiles = selectOrderFile(id);
		for (Record orderFile : orderFiles) {
			Integer fileType = orderFile.getInt("fileType");
			if (Constant.INFORMATION_FILETYPE.equals(fileType)) {
				information.add(orderFile);
			} else if (Constant.BOM_FILETYPE.equals(fileType)) {
				bom.add(orderFile);
			} else if (Constant.SOP_FILETYPE.equals(fileType)) {
				sop.add(orderFile);
			}
		}
		OrderFileInfo orderFileInfo = new OrderFileInfo(information, bom, sop);
		detail.setOrderFileInfo(orderFileInfo);
		detail.setOrderUser(selectOrderUser(id));
		return detail;
	}


	public Record selectOrderUser(Integer id) {
		SqlPara sqlPara = new SqlPara();
		Record record = new Record();
		sqlPara.setSql(SQL.SELECT_ORDERDETAIL_BY_ID);
		sqlPara.addPara(id);
		Page<Record> page = Db.paginate(Constant.DEFAULT_PAGE_NUM, Constant.DEFAULT_PAGE_SIZE, sqlPara);
		if (!page.getList().isEmpty()) {
			record = page.getList().get(0);
			String orderModifier = record.getStr("orderModifier");
			String deletePerson = record.getStr("deletePerson");
			if (orderModifier != null) {
				LUserAccount modifier = LUserAccount.dao.findById(orderModifier);
				if (modifier != null) {
					record.set("modifierName", modifier.getName());
				}
			}
			if (deletePerson != null) {
				LUserAccount deletePersonUser = LUserAccount.dao.findById(deletePerson);
				if (deletePersonUser != null) {
					record.set("deletePersonName", deletePersonUser.getName());
				}
			}
		}
		return record;
	}


	public List<Record> selectOrderFile(Integer id) {
		Orders order = Orders.dao.findById(id);
		if (order == null) {
			throw new OperationException("订单不存在");
		}
		return Db.find(SQL.SELECT_ORDERFILE_BY_ORDER, id);
	}


	public boolean editOrder(Orders order, LUserAccountVO userVO) {
		Orders temp = Orders.dao.findById(order.getId());
		if (temp == null) {
			throw new OperationException("订单不存在");
		}
		checkOrderUserType(userVO, temp);
		if (Constant.SCHEDULED_ORDERSTATUS.equals(temp.getOrderStatus())) {
			temp.setQuantity(order.getQuantity());
			return temp.update();
		}
		if (!Constant.UNSCHEDULED_ORDERSTATUS.equals(temp.getOrderStatus())) {
			throw new OperationException("已完成、已删除的订单无法修改");
		}
		order.setOrderModifier(userVO.getId()).setOrderModifyTime(new Date());
		return order.update();
	}


	public String importOrder(File file, LUserAccountVO userVO, Boolean isRework) {
		String resultString = "导入成功";
		ExcelHelper helper;
		int indexOfOrderItem = 2;
		List<Orders> orders = new ArrayList<>();
		Map<String, Integer> zhidanMap = new HashMap<String, Integer>();
		try {
			helper = ExcelHelper.from(file);
			List<OrderItem> orderItems = helper.unfill(OrderItem.class, 0);
			if (orderItems == null) {
				throw new OperationException("表格无有效数据或者表格格式不正确！");
			}
			for (OrderItem orderItem : orderItems) {
				System.out.println(orderItem);
			}
			for (OrderItem orderItem : orderItems) {
				String zhidan = orderItem.getZhidan();
				String softModel = orderItem.getSoftModel();
				boolean isDateExist = orderItem.getOrderDate() != null && orderItem.getDeliveryDate() != null;
				boolean isQuantityExist = orderItem.getQuantity() != null;
				if (StringUtils.isAnyBlank(zhidan, softModel) || !isDateExist || !isQuantityExist) {
					resultString = "导入失败，表格第" + indexOfOrderItem + "行的订单号和机型为空或者订单日期、交货日期和订单数量格式错误！";
					return resultString;
				}
				Orders temp = Orders.dao.findFirst(SQL.SELECT_ORDER_BY_ZHIDAN, orderItem.getZhidan());
				if (temp != null) {
					resultString = "导入失败，表格第" + indexOfOrderItem + "行的订单号已存在！";
					return resultString;
				}
				if (!zhidanMap.isEmpty()) {
					for (Entry<String, Integer> entry : zhidanMap.entrySet()) {
						if (entry.getKey().equals(zhidan)) {
							resultString = "导入失败，表格第 " + entry.getValue() + " 行和第 " + indexOfOrderItem + " 行的订单号重复！";
							return resultString;
						}
					}
				}
				zhidanMap.put(zhidan, indexOfOrderItem);
				Orders order = new Orders();
				order.setZhidan(zhidan).setSoftModel(softModel).setVersion(orderItem.getVersion());
				order.setCustomerName(orderItem.getCustomerName()).setCustomerNumber(orderItem.getCustomerNumber());
				order.setAlias(orderItem.getAlias()).setProductNo(orderItem.getProductNo()).setOrderDate(orderItem.getOrderDate());
				order.setQuantity(orderItem.getQuantity()).setDeliveryDate(orderItem.getDeliveryDate()).setRemark(orderItem.getRemark());
				orders.add(order);

				indexOfOrderItem++;
			}
			for (Orders order : orders) {
				order.setOrderStatus(Constant.UNSCHEDULED_ORDERSTATUS).setIsRework(isRework).setOrderCreator(userVO.getId()).setOrderCreateTime(new Date()).save();
			}
		} catch (Exception e) {
			e.printStackTrace();
			throw new OperationException("文件解析出错，请检查文件内容和格式");
		} finally {
			file.delete();
		}
		return resultString;
	}


	public boolean importOrderTable(List<UploadFile> uploadFiles, Integer type, Integer id, LUserAccountVO userVO) {
		OrderFile orderFile = new OrderFile();
		switch (type) {
		case 0:
			orderFile.setFileType(Constant.INFORMATION_FILETYPE);
			break;
		case 1:
			orderFile.setFileType(Constant.BOM_FILETYPE);
			break;
		case 2:
			orderFile.setFileType(Constant.SOP_FILETYPE);
			break;
		default:
			throw new OperationException("无法识别的类型");
		}
		Orders order = Orders.dao.findById(id);
		if (order == null) {
			throw new OperationException("订单不存在");
		}
		File dir = new File(Constant.FILE_TABLE_PATH);
		if (!dir.exists()) {
			dir.mkdirs();
		}
		for (UploadFile uploadFile : uploadFiles) {
			File tempFile = uploadFile.getFile();
			String fileName = uploadFile.getOriginalFileName();
			// 防止文件名中携带&nbsp的空格，导致前端输入文件名查不到数据
			try {
				fileName = URLEncoder.encode(fileName, "utf-8");
				fileName = fileName.replaceAll("%C2%A0", "%20");
				fileName = URLDecoder.decode(fileName, "utf-8");
			} catch (UnsupportedEncodingException e) {
				e.printStackTrace();
			}
			File file = new File(Constant.FILE_TABLE_PATH + fileName);
			if (file.exists()) {
				file.delete();
			}
			try {
				FileUtils.moveFile(tempFile, file);
			} catch (IOException e) {
				throw new OperationException("文件保存失败");
			}
			orderFile.setFileName(fileName).setPath(file.getAbsolutePath()).setOrders(id).setUploader(userVO.getId()).setUploadTime(new Date()).save();
			orderFile.remove("id");
		}
		return true;
	}


	public File downloadOrderTable(Integer id) {
		OrderFile orderFile = OrderFile.dao.findById(id);
		if (orderFile == null) {
			throw new OperationException("文件记录不存在");
		}
		File file = new File(orderFile.getPath());
		if (!file.exists()) {
			throw new OperationException("文件不存在,请重新上传");
		}
		return file;
	}


	public boolean deleteOrderTable(Integer id, LUserAccountVO userVO) {
		OrderFile orderFile = OrderFile.dao.findById(id);
		if (orderFile == null) {
			throw new OperationException("文件记录不存在");
		}
		if (Constant.ENGINEER_USERTYPE.equals(userVO.getWebUserType()) && !Constant.SOP_FILETYPE.equals(orderFile.getFileType())) {
			throw new OperationException("工程及生产只能删除SOP表");
		}
		File file = new File(orderFile.getPath());
		if (file.exists()) {
			file.delete();
		}
		return orderFile.delete();
	}


	public List<OrderVO> selectUnscheduledPlan(Integer type) {
		List<OrderVO> orderVOs = new ArrayList<>();
		List<Record> orderRecords = new ArrayList<>();
		Integer rework = 0;
		switch (type) {
		case 0:
			List<Orders> orders = Orders.dao.find(SQL.SELECT_ORDER_BY_STATUS_ISREWORK, rework, Constant.UNSCHEDULED_ORDERSTATUS, Constant.SCHEDULED_ORDERSTATUS);
			if (orders == null || orders.isEmpty()) {
				throw new OperationException("不存在可排产的订单");
			}
			for (Orders order : orders) {
				orderVOs.add(genOrderVO(order, type));
			}
			return removeScheduledOrder(orderVOs);
		case 1:
			orderRecords = Db.find(SQL.SELECT_DISTINCT_ORDER_BY_PROCESSGROUP_ORDERSTATUS, Constant.ASSEMBLING_PROCESS_GROUP, Constant.UNSCHEDULED_ORDERSTATUS, Constant.SCHEDULED_ORDERSTATUS);
			if (orderRecords == null || orderRecords.isEmpty()) {
				throw new OperationException("不存在前置工序组已排产的订单");
			}
			break;
		case 2:
			orderRecords = Db.find(SQL.SELECT_DISTINCT_ORDER_BY_PROCESSGROUP_ORDERSTATUS, Constant.TESTING_PROCESS_GROUP, Constant.UNSCHEDULED_ORDERSTATUS, Constant.SCHEDULED_ORDERSTATUS);
			if (orderRecords == null || orderRecords.isEmpty()) {
				throw new OperationException("不存在前置工序组已排产的订单");
			}
			break;
		default:
			break;
		}
		for (Record orderRecord : orderRecords) {
			Orders order = Orders.dao.findById(orderRecord.getInt("orders"));
			orderVOs.add(genOrderVO(order, type));
		}
		return removeScheduledOrder(orderVOs);
	}


	public List<OrderVO> selectReworkPlan(Integer type) {
		List<OrderVO> orderVOs = new ArrayList<>();
		Integer rework = 1;
		List<Orders> orders = Orders.dao.find(SQL.SELECT_ORDER_BY_STATUS_ISREWORK, rework, Constant.UNSCHEDULED_ORDERSTATUS, Constant.SCHEDULED_ORDERSTATUS);
		if (orders == null || orders.isEmpty()) {
			throw new OperationException("不存在返工订单");
		}
		for (Orders order : orders) {
			orderVOs.add(genOrderVO(order, type));
		}
		return removeScheduledOrder(orderVOs);
	}


	public boolean addPlan(List<AddPlanInfo> addPlanInfos, LUserAccountVO userVO) {
		for (AddPlanInfo addPlanInfo : addPlanInfos) {
			String[] quantitys = addPlanInfo.getSchedulingQuantity().split(",");
			String[] lines = addPlanInfo.getLine().split(",");
			String[] capacitys = addPlanInfo.getCapacity().split(",");
			Integer processGroup = addPlanInfo.getProcessGroup();
			Integer order = addPlanInfo.getOrder();
			Date planStartTime = addPlanInfo.getPlanStartTime();
			Date planCompleteTime = addPlanInfo.getPlanCompleteTime();
			if (planStartTime.compareTo(planCompleteTime) >= Constant.INTEGER_ZERO) {
				throw new ParameterException("开始时间不能晚于或者等于结束时间");
			}
			if (quantitys.length != lines.length || quantitys.length != capacitys.length || lines.length != capacitys.length) {
				throw new ParameterException("多产线排产时参数个数必须一致");
			}
			Orders orderRecord = Orders.dao.findById(addPlanInfo.getOrder());
			if (orderRecord == null) {
				throw new OperationException("添加排产计划失败，订单不存在");
			}
			if (ProcessGroup.dao.findById(addPlanInfo.getProcessGroup()) == null) {
				throw new OperationException("添加排产计划失败，工序组不存在");
			}
			for (int i = 0; i < capacitys.length; i++) {
				if (!CommonUtil.isPositiveInteger(lines[i]) || !CommonUtil.isPositiveInteger(quantitys[i]) || !CommonUtil.isPositiveInteger(capacitys[i])) {
					throw new OperationException("格式必须为正整数");
				}
				if (orderRecord.getQuantity() < Integer.parseInt(quantitys[i])) {
					throw new OperationException("排产数量不能大于订单数量");
				}
				SchedulingPlan schedulingPlan = new SchedulingPlan();
				if (!StrKit.isBlank(addPlanInfo.getRemark())) {
					schedulingPlan.setRemark(addPlanInfo.getRemark());
				}
				schedulingPlan.setProcessGroup(processGroup).setLine(Integer.parseInt(lines[i])).setSchedulingQuantity(Integer.parseInt(quantitys[i])).setOrders(order).setIsTimeout(false);
				schedulingPlan.setLineChangeTime(Constant.DEFAULT_LINE_CHANGE_TIME).setCapacity(Integer.parseInt(capacitys[i])).setSchedulingPlanStatus(Constant.SCHEDULED_PLANSTATUS);
				schedulingPlan.setScheduler(userVO.getId()).setSchedulingTime(new Date());
				schedulingPlan.setPlanStartTime(planStartTime).setPlanCompleteTime(planCompleteTime);
				schedulingPlan.save();
			}
			if (Constant.UNSCHEDULED_ORDERSTATUS.equals(orderRecord.getOrderStatus())) {
				orderRecord.setOrderStatus(Constant.SCHEDULED_ORDERSTATUS);
				orderRecord.update();
			}
		}
		/*String[] quantitys = schedulingQuantity.split(",");
		String[] lines = line.split(",");
		String[] capacitys = capacity.split(",");
		if (quantitys.length != lines.length || quantitys.length != capacitys.length || lines.length != capacitys.length) {
			throw new ParameterException("多产线排产时参数个数必须一致");
		}
		Orders orderRecord = Orders.dao.findById(order);
		if (orderRecord == null) {
			throw new OperationException("添加排产计划失败，订单不存在");
		}
		if (ProcessGroup.dao.findById(processGroup) == null) {
			throw new OperationException("添加排产计划失败，工序组不存在");
		}
		for (int i = 0; i < capacitys.length; i++) {
			if (!CommonUtil.isPositiveInteger(lines[i]) || !CommonUtil.isPositiveInteger(quantitys[i]) || !CommonUtil.isPositiveInteger(capacitys[i])) {
				throw new OperationException("格式必须为正整数");
			}
			if (orderRecord.getQuantity() < Integer.parseInt(quantitys[i])) {
				throw new OperationException("排产数量不能大于订单数量");
			}
			SchedulingPlan schedulingPlan = new SchedulingPlan();
			if (!StrKit.isBlank(remark)) {
				schedulingPlan.setRemark(remark);
			}
			schedulingPlan.setProcessGroup(processGroup).setLine(Integer.parseInt(lines[i])).setSchedulingQuantity(Integer.parseInt(quantitys[i])).setOrders(order).setIsTimeout(false);
			schedulingPlan.setLineChangeTime(Constant.DEFAULT_LINE_CHANGE_TIME).setCapacity(Integer.parseInt(capacitys[i])).setSchedulingPlanStatus(Constant.SCHEDULED_PLANSTATUS);
			schedulingPlan.setScheduler(userVO.getId()).setSchedulingTime(new Date());
			schedulingPlan.save();
		}
		if (Constant.UNSCHEDULED_ORDERSTATUS.equals(orderRecord.getOrderStatus())) {
			orderRecord.setOrderStatus(Constant.SCHEDULED_ORDERSTATUS);
			orderRecord.update();
		}*/
		return true;
	}


	public boolean deletePlan(Integer id) {
		SchedulingPlan schedulingPlan = SchedulingPlan.dao.findById(id);
		if (schedulingPlan == null) {
			throw new OperationException("排产计划不存在");
		}
		if (!Constant.SCHEDULED_PLANSTATUS.equals(schedulingPlan.getSchedulingPlanStatus())) {
			throw new OperationException("只能删除状态为已排产的计划");
		}
		Integer orderPlanNumber = Db.queryInt(SQL.SELECT_SCHEDULINGPLAN_BY_ORDERID, schedulingPlan.getOrders());
		Orders order = Orders.dao.findById(schedulingPlan.getOrders());
		if (orderPlanNumber == 1 && Constant.SCHEDULED_ORDERSTATUS.equals(order.getOrderStatus())) {
			order.setOrderStatus(Constant.UNSCHEDULED_ORDERSTATUS);
			return schedulingPlan.delete() && order.update();
		}
		return schedulingPlan.delete();
	}


	public Page<Record> selectPlan(PlanQueryCriteria planQueryCriteria) {
		StringBuilder filter = new StringBuilder();
		filter.append(concatEqualSqlFilter("scheduling_plan_status", planQueryCriteria.getSchedulingPlanStatus()));
		filter.append(concatEqualSqlFilter("scheduling_plan.process_group", planQueryCriteria.getProcessGroup()));
		filter.append(concatEqualSqlFilter("line", planQueryCriteria.getLine()));
		filter.append(concatSqlFilter("zhidan", planQueryCriteria.getZhidan(), true, false));
		filter.append(concatSqlFilter("customer_name", planQueryCriteria.getCustomerName(), true, false));
		filter.append(concatSqlFilter("production_planning_number", planQueryCriteria.getProductionPlanningNumber(), true, false));
		filter.append(concatSqlFilter("soft_model", planQueryCriteria.getSoftModel(), true, false));
		filter.append(concatSqlFilter("product_no", planQueryCriteria.getProductNo(), true, false));
		filter.append(concatSqlFilter("order_date", planQueryCriteria.getOrderDateFrom(), false, true));
		filter.append(concatSqlFilter("order_date", planQueryCriteria.getOrderDateTo(), false, false));
		filter.append(concatSqlFilter("plan_start_time", planQueryCriteria.getPlanStartTimeFrom(), false, true));
		filter.append(concatSqlFilter("plan_start_time", planQueryCriteria.getPlanStartTimeTo(), false, false));
		filter.append(concatSqlFilter("plan_complete_time", planQueryCriteria.getPlanCompleteTimeFrom(), false, true));
		filter.append(concatSqlFilter("plan_complete_time", planQueryCriteria.getPlanCompleteTimeTo(), false, false));
		filter.append(concatSqlFilter("start_time", planQueryCriteria.getStartTimeFrom(), false, true));
		filter.append(concatSqlFilter("start_time", planQueryCriteria.getStartTimeTo(), false, false));
		filter.append(concatSqlFilter("complete_time", planQueryCriteria.getCompleteTimeFrom(), false, true));
		filter.append(concatSqlFilter("complete_time", planQueryCriteria.getCompleteTimeTo(), false, false));
		SqlPara sqlPara = new SqlPara();
		String orderBy = " ORDER BY id DESC ";
		sqlPara.setSql(SQL.SELECT_SCHEDULINGPLAN + filter + orderBy);
		return formatPlanTimeOut(formatOrderDateAndCustomer(Db.paginate(planQueryCriteria.getPageNo(), 10, sqlPara), null));
	}


	public PlanDetail selectPlanDetail(Integer id) {
		PlanDetail planDetail = new PlanDetail();
		Record planUser = new Record();
		Record order = new Record();
		SqlPara sqlPara = new SqlPara();
		sqlPara.setSql(SQL.SELECT_SCHEDULINGPLANDETAIL_BY_ID);
		sqlPara.addPara(id);
		Page<Record> page = Db.paginate(Constant.DEFAULT_PAGE_NUM, Constant.DEFAULT_PAGE_SIZE, sqlPara);
		if (page.getList() != null && !page.getList().isEmpty()) {
			planUser = page.getList().get(0);
			String planModifier = planUser.getStr("planModifier");
			String productionConfirmer = planUser.getStr("productionConfirmer");
			if (planModifier != null) {
				LUserAccount modifier = LUserAccount.dao.findById(planModifier);
				if (modifier != null) {
					planUser.set("modifierName", modifier.getName());
				}
			}
			if (productionConfirmer != null) {
				LUserAccount confirmer = LUserAccount.dao.findById(productionConfirmer);
				if (confirmer != null) {
					planUser.set("confirmerName", confirmer.getName());
				}
			}
		}
		order = Db.findFirst(SQL.SELECT_ORDER_BY_PLAN_ID, id);
		planDetail.setOrder(order);
		planDetail.setPlanUser(planUser);
		return planDetail;
	}


	public Record selectPlanProducedQuantity(Integer id) {
		SchedulingPlan schedulingPlan = SchedulingPlan.dao.findById(id);
		if (schedulingPlan == null) {
			throw new OperationException("计划不存在");
		}
		Orders order = Orders.dao.findById(schedulingPlan.getOrders());
		if (order == null) {
			throw new OperationException("订单不存在");
		}
		List<LineComputer> lineComputers = LineComputer.dao.find(SQL.SELECT_LINECOMPUTER_BY_LINE, schedulingPlan.getLine());
		if (lineComputers == null || lineComputers.isEmpty()) {
			throw new OperationException("产线电脑不存在");
		}
		String sql = null;
		Integer planProducedQuantity = 0;
		if (Constant.ASSEMBLING_PROCESS_GROUP.equals(schedulingPlan.getProcessGroup())) {
			sql = SQL.SELECT_ASSEMBLING_PROCESS_GROUP_PRUDUCEDQUANTITY;
		} else if (Constant.TESTING_PROCESS_GROUP.equals(schedulingPlan.getProcessGroup())) {
			sql = SQL.SELECT_TESTING_PROCESS_GROUP_PRUDUCEDQUANTITY;
		} else if (Constant.PACKING_PROCESS_GROUP.equals(schedulingPlan.getProcessGroup())) {
			sql = SQL.SELECT_PACKING_PROCESS_GROUP_PRUDUCEDQUANTITY;
		}
		for (LineComputer lineComputer : lineComputers) {
			planProducedQuantity += Db.queryInt(sql, order.getZhidan(), order.getSoftModel(), "%" + lineComputer.getIp() + "%");
		}
		if (schedulingPlan.getSchedulingQuantity() <= planProducedQuantity) {
			schedulingPlan.setRemainingQuantity(0);
		} else {
			schedulingPlan.setRemainingQuantity(schedulingPlan.getSchedulingQuantity() - planProducedQuantity);
		}
		schedulingPlan.setProducedQuantity(planProducedQuantity).update();
		Record record = new Record();
		record.set("planProducedQuantity", planProducedQuantity);
		return record;
	}


	public boolean editPlan(Integer id, Boolean isUrgent, String remark, Integer schedulingQuantity, Integer line, Date planStartTime, Date planCompleteTime, String lineChangeTime, Integer capacity, Boolean isCompleted, String remainingReason, String productionPlanningNumber, LUserAccountVO userVO) {
		SchedulingPlan schedulingPlan = SchedulingPlan.dao.findById(id);
		if (schedulingPlan == null) {
			throw new OperationException("排产计划不存在");
		}
		if (Constant.COMPLETED_PLANSTATUS.equals(schedulingPlan.getSchedulingPlanStatus())) {
			throw new OperationException("已完成排产计划无法修改");
		}
		if (lineChangeTime != null) {
			double changeLineTime = 0.0;
			try {
				changeLineTime = Double.parseDouble(lineChangeTime);
			} catch (Exception e) {
				throw new OperationException("转线时间格式出错");
			}
			if (changeLineTime <= Constant.INTEGER_ZERO) {
				throw new OperationException("转线时间格式出错");
			}
			schedulingPlan.setLineChangeTime(lineChangeTime);
		}
		if (isUrgent != null) {
			schedulingPlan.setIsUrgent(isUrgent);
		}
		if (line != null) {
			schedulingPlan.setLine(line);
		}
		if (planStartTime != null) {
			schedulingPlan.setPlanStartTime(planStartTime);
		}
		if (planCompleteTime != null) {
			schedulingPlan.setPlanCompleteTime(planCompleteTime);
		}
		if (capacity != null) {
			schedulingPlan.setCapacity(capacity);
		}
		if (remark != null) {
			schedulingPlan.setRemark(remark);
		}
		if (schedulingQuantity != null) {
			schedulingPlan.setSchedulingQuantity(schedulingQuantity);
		}
		if (remainingReason != null) {
			schedulingPlan.setRemainingReason(remainingReason);
		}
		if (productionPlanningNumber != null) {
			schedulingPlan.setProductionPlanningNumber(productionPlanningNumber);
		}
		schedulingPlan.setPlanModifier(userVO.getId()).setPlanModifyTime(new Date());
		Orders order = Orders.dao.findById(schedulingPlan.getOrders());
		if (order == null) {
			throw new OperationException("订单不存在");
		}
		if (isCompleted != null && isCompleted) {
			if (schedulingPlan.getProducedQuantity() < schedulingQuantity) {
				throw new OperationException("无法完成此排产计划，已生产数量未达到排产数量");
			}
			schedulingPlan.setSchedulingPlanStatus(Constant.COMPLETED_PLANSTATUS);
			schedulingPlan.setCompleteTime(new Date());
			Integer quantity = Db.queryInt(SQL.SELECT_CARTONTEST_NUMBER_BY_ZHIDAN_SOFTMODEL, order.getZhidan(), order.getSoftModel());
			if (quantity >= order.getQuantity()) {
				order.setOrderStatus(Constant.COMPLETED_ORDERSTATUS);
				return schedulingPlan.update() && order.update();
			}
		}
		return schedulingPlan.update();
	}


	public boolean editPlanStatus(Integer id, Integer type, LUserAccountVO userVO) {
		SchedulingPlan schedulingPlan = SchedulingPlan.dao.findById(id);
		if (schedulingPlan == null) {
			throw new OperationException("排产计划不存在");
		}
		Integer planStatus = schedulingPlan.getSchedulingPlanStatus();
		if (planStatus.equals(Constant.COMPLETED_PLANSTATUS)) {
			throw new OperationException("已完成的排产计划无法修改");
		}
		if (Constant.SCHEDULED_PLANSTATUS.equals(planStatus) && !type.equals(Constant.WORKING_PLANSTATUS - 1) && !type.equals(Constant.COMPLETED_PLANSTATUS - 1)) {
			throw new OperationException("已排产的排产计划只能修改为进行中或已完成");
		}
		if (Constant.WORKING_PLANSTATUS.equals(planStatus) && !type.equals(Constant.COMPLETED_PLANSTATUS - 1) && !type.equals(Constant.WAIT_NOTIFICATION_PLANSTATUS - 1)) {
			throw new OperationException("进行中的排产计划只能修改为待通知或已完成");
		}
		if (Constant.WAIT_NOTIFICATION_PLANSTATUS.equals(planStatus) && !type.equals(Constant.WORKING_PLANSTATUS - 1)) {
			throw new OperationException("待通知的排产计划只能修改为进行中");
		}
		switch (type) {
		case 0:
			schedulingPlan.setSchedulingPlanStatus(Constant.SCHEDULED_PLANSTATUS);
			break;
		case 1:
			if (schedulingPlan.getPlanStartTime() == null || schedulingPlan.getPlanCompleteTime() == null) {
				throw new OperationException("排产计划的预计开始和预计结束时间不能为空");
			}
			schedulingPlan.setSchedulingPlanStatus(Constant.WORKING_PLANSTATUS);
			schedulingPlan.setProductionConfirmer(userVO.getId());
			schedulingPlan.setStartTime(new Date());
			break;
		case 2:
			schedulingPlan.setSchedulingPlanStatus(Constant.COMPLETED_PLANSTATUS);
			break;
		case 3:
			schedulingPlan.setSchedulingPlanStatus(Constant.WAIT_NOTIFICATION_PLANSTATUS);
			break;
		default:
			break;
		}
		schedulingPlan.setPlanModifier(userVO.getId()).setPlanModifyTime(new Date());
		return schedulingPlan.update();
	}


	public void exportPlan(Page<Record> page, HttpServletResponse response, OutputStream output) throws Exception {
		SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd_HH:mm:ss");
		int size = page.getTotalRow();
		List<Record> records = page.getList();
		for (Record record : records) {
			Boolean isUrgent = record.getBoolean("isUrgent");
			if (isUrgent == null || !isUrgent) {
				record.set("isUrgent", "否");
			} else {
				record.set("isUrgent", "是");
			}
		}
		String fileName = "排产计划单导出_" + simpleDateFormat.format(new Date()) + "_" + size + ".xlsx";
		String title = "排产计划单";
		response.setHeader("Content-Disposition", "attachment; filename=" + URLEncoder.encode(fileName, "utf-8"));
		response.setContentType("application/vnd.ms-excel");
		response.addHeader("Access-Control-Expose-Headers", "Content-Disposition");
		ExcelHelper helper = ExcelHelper.create(true);
		String[] field = new String[] { "id", "lineNo", "lineName", "statusName", "zhidan", "alias", "softModel", "productNo", "customerNumber", "customerName", "isUrgent", "orderDate", "deliveryDate", "schedulingQuantity", "productionPlanningNumber", "planStartTime", "planCompleteTime", "remark" };
		String[] head = new String[] { "序号", "线别", "线名", "状态", "订单号", "内部替换号", "机型", "成品编码", "客户编号", "客户名称", "是否紧急", "订单日期", "交货日期", "排产数量", "生产计划单号", "预计开始时间", "预计完成时间", "备注" };
		helper.fill(records, title, field, head);
		helper.write(output, true);
	}


	public static void main(String[] args) {
		Integer a = 76;
		Integer b = 6;
		String c = String.valueOf((double) a / (double) b * 100.0);
		Double double1 = (double) a / (double) b;
		System.out.println(c.substring(0, c.indexOf(".")) + "%");
		System.out.println(BigDecimal.valueOf(double1).setScale(0, BigDecimal.ROUND_HALF_UP).intValue());
	}


	public List<PlanGantt> getPlanGantt(Integer id) {
		List<Record> records = Db.find(SQL.SELECT_PLAN_INFORMATION, id);
		if (records == null || records.isEmpty()) {
			throw new OperationException("此排产计划无法获取甘特图,请检查所有配置信息");
		}
		int index = 1;
		String zhidan = records.get(0).getStr("ZhiDan");
		Integer planProduction = records.get(0).getInt("scheduling_quantity");
		PlanGantt planGantt = new PlanGantt();
		planGantt.setId(index);
		planGantt.setName(zhidan);
		planGantt.setPlanProduction(planProduction.toString());
		StringBuilder computerSql = new StringBuilder(" and (");
		for (Record record : records) {
			computerSql.append(" Computer LIKE '%" + record.getStr("ip") + "%' or ");
		}
		if (StringUtils.endsWith(computerSql, "or ")) {
			computerSql.delete(computerSql.lastIndexOf("or"), computerSql.length());
		}
		String sql = computerSql.append(")").toString();
		PlanGantt smtGantt = genPlanGantt(index + 1, zhidan, planProduction, sql);
		PlanGantt functionGantt = genPlanGantt(index + 2, zhidan, planProduction, sql);
		PlanGantt agedGantt = genPlanGantt(index + 3, zhidan, planProduction, sql);
		PlanGantt couplingGantt = genPlanGantt(index + 4, zhidan, planProduction, sql);
		PlanGantt cartonGantt = genPlanGantt(index + 5, zhidan, planProduction, sql);
		List<PlanGantt> gantts = new ArrayList<>();
		gantts.add(planGantt);
		gantts.add(smtGantt);
		gantts.add(functionGantt);
		gantts.add(agedGantt);
		gantts.add(couplingGantt);
		gantts.add(cartonGantt);
		planGantt.setStartTime("-");
		for (int i = 1; i < gantts.size(); i++) {
			if (!gantts.get(i).getStartTime().equals("-")) {
				planGantt.setStartTime(gantts.get(i).getStartTime());
				break;
			}
		}
		if (!cartonGantt.getEndTime().equals("-")) {
			planGantt.setEndTime(cartonGantt.getEndTime());
		} else {
			planGantt.setEndTime("-");
		}
		if (!planGantt.getStartTime().equals("-") && !planGantt.getEndTime().equals("-")) {
			planGantt.setIntervalDay(getDateIntervalDays(planGantt.getStartTime(), planGantt.getEndTime()) + "");
		} else {
			planGantt.setIntervalDay("-");
		}
		planGantt.setCompletionRate(cartonGantt.getCompletionRate());
		planGantt.setCompletionQuantity(cartonGantt.getCompletionQuantity());
		return gantts;

	}


	public boolean checkCompleteTime(Integer schedulingQuantity, Date planStartTime, Date planCompleteTime, String lineChangeTime, Integer capacity) {
		double changeLineTime = 0.0;
		try {
			changeLineTime = Double.parseDouble(lineChangeTime);
		} catch (Exception e) {
			throw new OperationException("转线时间格式出错");
		}
		if (changeLineTime <= Constant.INTEGER_ZERO) {
			throw new OperationException("转线时间格式出错");
		}
		double actualCostHours = BigDecimal.valueOf((double) schedulingQuantity / (double) capacity).setScale(2, BigDecimal.ROUND_DOWN).doubleValue();
		long lineChangeCostMilliseconds = (long) (changeLineTime * 60 * 60 * 1000);
		long actualCostMilliseconds = (long) actualCostHours * 60 * 60 * 1000 + lineChangeCostMilliseconds;
		long planCostMilliseconds = planCompleteTime.getTime() - planStartTime.getTime() + 1;
		if (actualCostMilliseconds > planCostMilliseconds) {
			return false;
		}
		return true;
	}


	/**@author HCJ
	 * 生成甘特图
	 * @param index 位置
	 * @param zhidan 订单号
	 * @param planProduction 计划产量
	 * @param computerSql 产线电脑IP的查询条件
	 * @date 2019年8月16日 上午11:56:26
	 */
	private PlanGantt genPlanGantt(Integer index, String zhidan, Integer planProduction, String computerSql) {
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
		Integer number = 0;
		PlanGantt gantt = new PlanGantt();
		gantt.setId(index);
		switch (index) {
		case 2:
			gantt.setName(Constant.SMT_TEST);
			number = Db.queryInt(SQL.SELECT_SMTTEST_NUMBER_BY_ZHIDAN_COMPUTER + computerSql, zhidan);
			break;
		case 3:
			gantt.setName(Constant.FUNCTION_TEST);
			number = Db.queryInt(SQL.SELECT_FUNCTIONTEST_NUMBER_BY_ZHIDAN_COMPUTER + computerSql, zhidan);
			break;
		case 4:
			gantt.setName(Constant.AGED_TEST);
			number = Db.queryInt(SQL.SELECT_AGEDTEST_NUMBER_BY_ZHIDAN_COMPUTER + computerSql, zhidan);
			break;
		case 5:
			gantt.setName(Constant.COUPLING_TEST);
			number = Db.queryInt(SQL.SELECT_COUPLETEST_NUMBER_BY_ZHIDAN_COMPUTER + computerSql, zhidan);
			break;
		case 6:
			gantt.setName(Constant.CARTON_TEST);
			number = Db.queryInt(SQL.SELECT_CARTONTEST_NUMBER_BY_ZHIDAN_COMPUTER + computerSql, zhidan);
			break;
		default:
			throw new OperationException("无法识别的类型");
		}
		if (number > 0) {
			gantt.setPlanProduction(planProduction.toString());
			Record firstRecord = new Record();
			switch (index) {
			case 2:
				firstRecord = Db.findFirst(SQL.SELECT_FIRST_SMTTEST + computerSql, zhidan);
				break;
			case 3:
				firstRecord = Db.findFirst(SQL.SELECT_FIRST_FUNCTIONTEST + computerSql, zhidan);
				break;
			case 4:
				firstRecord = Db.findFirst(SQL.SELECT_FIRST_AGEDTEST + computerSql, zhidan);
				break;
			case 5:
				firstRecord = Db.findFirst(SQL.SELECT_FIRST_COUPLETEST + computerSql, zhidan);
				break;
			case 6:
				firstRecord = Db.findFirst(SQL.SELECT_FIRST_CARTONTEST + computerSql, zhidan);
				break;
			default:
				break;
			}
			gantt.setStartTime(dateFormat.format(firstRecord.getDate("TestTime")));
			if (number >= planProduction) {
				Record lastRecord = new Record();
				switch (index) {
				case 2:
					lastRecord = Db.findFirst(SQL.SELECT_LAST_SMTTEST_FRAGMENT_ONE + computerSql + SQL.SELECT_LAST_TEST_FRAGMENT_TWO, planProduction, zhidan);
					break;
				case 3:
					lastRecord = Db.findFirst(SQL.SELECT_LAST_FUNCTIONTEST_FRAGMENT_ONE + computerSql + SQL.SELECT_LAST_TEST_FRAGMENT_TWO, planProduction, zhidan);
					break;
				case 4:
					lastRecord = Db.findFirst(SQL.SELECT_LAST_AGEDTEST_FRAGMENT_ONE + computerSql + SQL.SELECT_LAST_TEST_FRAGMENT_TWO, planProduction, zhidan);
					break;
				case 5:
					lastRecord = Db.findFirst(SQL.SELECT_LAST_COUPLETEST_FRAGMENT_ONE + computerSql + SQL.SELECT_LAST_TEST_FRAGMENT_TWO, planProduction, zhidan);
					break;
				case 6:
					lastRecord = Db.findFirst(SQL.SELECT_LAST_CARTONTEST_FRAGMENT_ONE + computerSql + SQL.SELECT_LAST_TEST_FRAGMENT_TWO, planProduction, zhidan);
					break;
				default:
					break;
				}
				gantt.setEndTime(dateFormat.format(lastRecord.getDate("TestTime")));
				gantt.setIntervalDay(getDateIntervalDays(gantt.getStartTime(), gantt.getEndTime()) + "");
				gantt.setCompletionQuantity(number.toString());
				gantt.setCompletionRate("100%");
			} else {
				gantt.setEndTime("-");
				gantt.setIntervalDay("-");
				gantt.setCompletionQuantity(number.toString());
				String completionRate = String.valueOf((double) number / (double) planProduction * 100.0);
				gantt.setCompletionRate(completionRate.substring(0, completionRate.indexOf(".")) + "%");
			}
		} else {
			gantt.setCompletionQuantity("-");
			gantt.setCompletionRate("-");
			gantt.setEndTime("-");
			gantt.setIntervalDay("-");
			gantt.setPlanProduction("-");
			gantt.setStartTime("-");
		}
		return gantt;
	}


	/**@author HCJ
	 * 检查用户情况
	 * @param id 用户ID
	 * @date 2019年8月16日 上午11:57:48
	 */
	private void checkUserById(Integer id) {
		LUserAccount user = LUserAccount.dao.findById(id);
		if (user == null) {
			throw new OperationException("此用户不存在");
		}
		if (!user.getInService()) {
			throw new OperationException("此用户未启用");
		}
	}


	/**@author HCJ
	 * 获取间隔天数
	 * @param startDate 开始日期
	 * @param endDate 结束日期
	 * @date 2019年8月16日 上午11:58:05
	 */
	private long getDateIntervalDays(String startDate, String endDate) {
		long daysBetween = 0;
		SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd");
		try {
			Date start = format.parse(startDate);
			Date end = format.parse(endDate);
			daysBetween = (end.getTime() - start.getTime()) / (3600 * 24 * 1000);
		} catch (Exception e) {
			throw new OperationException("格式化时间出错，请检查时间格式");
		}
		return daysBetween + 1;
	}


	/**@author HCJ
	 * 拼接sql语句
	 * @param key 键
	 * @param value 值
	 * @param isLike 是否为like语句
	 * @param isGreater 是否为大于
	 * @date 2019年8月16日 上午11:58:32
	 */
	private StringBuilder concatSqlFilter(String key, String value, Boolean isLike, Boolean isGreater) {
		StringBuilder filter = new StringBuilder();
		if (!StrKit.isBlank(value)) {
			if (isLike) {
				filter.append(" AND " + key + " like '%" + value + "%'");
			} else {
				if (isGreater) {
					filter.append(" AND " + key + " > '" + value + "'");
				} else {
					filter.append(" AND " + key + " < '" + value + "'");
				}
			}
		}
		return filter;
	}


	/**@author HCJ
	 * 拼接含=号的sql语句
	 * @param key 键
	 * @param value 值
	 * @date 2019年8月16日 上午11:59:19
	 */
	private StringBuilder concatEqualSqlFilter(String key, Integer value) {
		StringBuilder filter = new StringBuilder();
		if (value != null) {
			filter.append(" AND " + key + " = " + value);
		}
		return filter;
	}


	/**@author HCJ
	 * 格式化订单日期和客户信息
	 * @param page 需要进行格式化的内容
	 * @param isOperator 用户类型是否为操作员
	 * @date 2019年8月16日 上午11:59:47
	 */
	private Page<Record> formatOrderDateAndCustomer(Page<Record> page, Boolean isOperator) {
		List<Record> records = page.getList();
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
		for (Record record : records) {
			if (isOperator != null && isOperator) {
				record.set("customerNumber", "***");
				record.set("customerName", "***");
			}
			try {
				record.set("orderDate", dateFormat.format(record.getDate("orderDate")));
				record.set("deliveryDate", dateFormat.format(record.getDate("deliveryDate")));
			} catch (Exception e) {
				record.set("orderDate", record.getStr("orderDate"));
				record.set("deliveryDate", record.getStr("deliveryDate"));
			}
		}
		return page;
	}


	/**@author HCJ
	 * 设置计划是否超时的字段
	 * @param page 需要进行设置的内容
	 * @date 2019年8月16日 下午12:00:40
	 */
	private Page<Record> formatPlanTimeOut(Page<Record> page) {
		List<Record> records = page.getList();
		for (Record record : records) {
			if (record.getDate("planCompleteTime") == null) {
				continue;
			} else {
				if (new Date().after(record.getDate("planCompleteTime"))) {
					if (selectPlanProducedQuantity(record.getInt("id")).getInt("planProducedQuantity") < record.getInt("schedulingQuantity")) {
						record.set("isTimeout", true);
					}
				}
			}
		}
		return page;
	}


	/**@author HCJ
	 * 生成OrderVO
	 * @param order 订单
	 * @param type 类型
	 * @date 2019年8月16日 下午12:01:26
	 */
	private OrderVO genOrderVO(Orders order, Integer type) {
		OrderVO orderVO = new OrderVO(order, null, null);
		Record rhythmRecord = null;
		Integer scheduledQuantity = null;
		if (type == Constant.AUTOTEST_LINEID) {
			scheduledQuantity = Db.queryInt(SQL.SELECT_SCHEDULED_ORDER_QUANTITY, Constant.ASSEMBLING_PROCESS_GROUP, order.getId());
			rhythmRecord = Db.findFirst(SQL.SELECT_PEOPLE_CAPACITY_BY_SOFTMODEL_PROCESSGROUP, Constant.ASSEMBLING_PROCESS_GROUP, order.getSoftModel());
		} else if (type == Constant.COUPLETEST_LINEID) {
			scheduledQuantity = Db.queryInt(SQL.SELECT_SCHEDULED_ORDER_QUANTITY, Constant.TESTING_PROCESS_GROUP, order.getId());
			rhythmRecord = Db.findFirst(SQL.SELECT_PEOPLE_CAPACITY_BY_SOFTMODEL_PROCESSGROUP, Constant.TESTING_PROCESS_GROUP, order.getSoftModel());
		} else if (type == Constant.CARTONTEST_LINEID) {
			scheduledQuantity = Db.queryInt(SQL.SELECT_SCHEDULED_ORDER_QUANTITY, Constant.PACKING_PROCESS_GROUP, order.getId());
			rhythmRecord = Db.findFirst(SQL.SELECT_PEOPLE_CAPACITY_BY_SOFTMODEL_PROCESSGROUP, Constant.PACKING_PROCESS_GROUP, order.getSoftModel());
		}
		if (scheduledQuantity == null) {
			orderVO.setUnscheduledQuantity(order.getQuantity());
		} else {
			orderVO.setUnscheduledQuantity(order.getQuantity() - scheduledQuantity);
		}
		if (rhythmRecord == null) {
			orderVO.setCapacity(Constant.INTEGER_ZERO);
		} else {
			Integer rhythm = rhythmRecord.getInt("rhythm");
			if (rhythm == null) {
				orderVO.setCapacity(Constant.INTEGER_ZERO);
			} else {
				orderVO.setCapacity(BigDecimal.valueOf((double) Constant.HOUR_TO_SECOND / (double) rhythm).setScale(0, BigDecimal.ROUND_HALF_UP).intValue());
			}
		}
		return orderVO;
	}


	/**@author HCJ
	 * 检查操作订单的用户类型
	 * @param userVO TOKEN内的用户信息
	 * @param order 订单
	 * @date 2019年8月16日 下午12:02:06
	 */
	private void checkOrderUserType(LUserAccountVO userVO, Orders order) {
		LUserAccount user = LUserAccount.dao.findById(order.getOrderCreator());
		if (user != null && user.getWebUserType() != null) {
			if (!userVO.getWebUserType().equals(Constant.SUPER_ADMIN_USERTYPE) && !user.getWebUserType().equals(userVO.getWebUserType())) {
				throw new OperationException("当前角色无法操作此订单");
			}
		}
	}


	/**@author HCJ
	 * 移除已经排产完的订单
	 * @param orderVOs 需要进行排产的订单
	 * @date 2019年8月24日 上午10:59:13
	 */
	private List<OrderVO> removeScheduledOrder(List<OrderVO> orderVOs) {
		if (orderVOs != null && !orderVOs.isEmpty()) {
			Iterator<OrderVO> iterator = orderVOs.iterator();
			while (iterator.hasNext()) {
				OrderVO orderVO = iterator.next();
				if (orderVO.getUnscheduledQuantity() <= Constant.INTEGER_ZERO) {
					iterator.remove();
				}
			}
		}
		return orderVOs;
	}

}
