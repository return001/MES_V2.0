package com.jimi.mes_server.service;

import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

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
import com.jimi.mes_server.entity.Constant;
import com.jimi.mes_server.entity.OrderDetail;
import com.jimi.mes_server.entity.OrderFileInfo;
import com.jimi.mes_server.entity.OrderItem;
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
import com.jimi.mes_server.util.ExcelHelper;

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
		return processGroup.save();
	}


	public boolean deleteProcessGroup(Integer id) {
		ProcessGroup processGroup = ProcessGroup.dao.findById(id);
		if (processGroup == null) {
			throw new OperationException("删除失败，项目组不存在");
		}
		return processGroup.delete();
	}


	public Page<Record> selectProcessGroup(String filter) {
		return daoService.select(SQL.SELECT_PROCESSGROUP, Constant.DEFAULT_PAGE_NUM, Constant.DEFAULT_PAGE_SIZE, null, null, filter);
	}


	public boolean editProcessGroup(Integer id, String groupNo, String groupName, String groupRemark) {
		ProcessGroup processGroup = ProcessGroup.dao.findById(id);
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
		sqlPara.setSql(SQL.SELECT_LINE_PROCESSGROUP + filter+orderBy);
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
		return process.save();
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
		String orderBy = " order by processGroup ";
		sqlPara.setSql(SQL.SELECT_PROCESS_PROCESSGROUP + filter+orderBy);
		return Db.paginate(Constant.DEFAULT_PAGE_NUM, Constant.DEFAULT_PAGE_SIZE, sqlPara);
	}


	public boolean editProcess(Integer id, String processNo, String processName, String processRemark, Integer processGroup) {
		Process process = Process.dao.findById(id);
		if (process == null) {
			throw new OperationException("修改失败，工序不存在");
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
			throw new OperationException("IP地址已存在，请直接引用");
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
			throw new OperationException("IP地址已存在，请直接引用");
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


	public boolean addCapacity(String softModel, String customerModel, Integer process, Integer processGroup, Integer processPeopleQuantity, Integer capacity, String remark) {
		ModelCapacity modelCapacity = ModelCapacity.dao.findFirst(SQL.SELECT_MODELCAPACITY_BY_MODEL_PROCESS, softModel, process, processGroup);
		if (modelCapacity != null) {
			throw new OperationException("机型产能已存在");
		}
		modelCapacity = new ModelCapacity();
		if (!StrKit.isBlank(remark)) {
			modelCapacity.setRemark(remark);
		}
		modelCapacity.setSoftModel(softModel).setCustomerModel(customerModel).setProcess(process) .setProcessGroup(processGroup);
		modelCapacity.setProcessPeopleQuantity(processPeopleQuantity).setCapacity(capacity).save();
		return modelCapacity.setPosition(modelCapacity.getId()).update();
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
		String orderBy = "ORDER BY soft_model,[position]";
		SqlPara sqlPara = new SqlPara();
		sqlPara.setSql(SQL.SELECT_MODELCAPACITY + filter + orderBy);
		return Db.paginate(pageNo, pageSize, sqlPara);
	}


	public boolean editCapacity(Integer id, String softModel, String customerModel, Integer process, Integer processGroup, Integer processPeopleQuantity, Integer capacity, String remark, Integer position) {
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
			ModelCapacity modelCapacity = ModelCapacity.dao.findFirst(SQL.SELECT_MODELCAPACITY_BY_MODEL_PROCESS,
					softModel, process, processGroup);
			if (modelCapacity != null&&!modelCapacity.getId().equals(id)) {
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
			if (processPeopleQuantity < 0) {
				throw new ParameterException("人数不合理");
			}
			firstModelCapacity.setProcessPeopleQuantity(processPeopleQuantity);
		}
		if (capacity != null) {
			if (capacity < 0) {
				throw new ParameterException("产能不合理");
			}
			firstModelCapacity.setCapacity(capacity);
		}
		if (remark != null) {
			firstModelCapacity.setRemark(remark);
		}
		return firstModelCapacity.update();
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
		order.setDeleteReason(deleteReason).setOrderStatus(Constant.DELETED_ORDERSTATUS);
		order.setDeletePerson(userVO.getId()).setDeleteTime(new Date());
		return order.update();
	}


	public Page<Record> selectOrder(Integer pageNo, Integer pageSize, String ascBy, String descBy, String filter, Boolean isRework,LUserAccountVO userVO) {
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
		return formatOrderDateAndCustomer(page,isOperator);
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
				if (!StringUtils.isAnyBlank(zhidan, softModel) && orderItem.getQuantity() != null) {
					Orders temp = Orders.dao.findFirst(SQL.SELECT_ORDER_BY_ZHIDAN, orderItem.getZhidan());
					if (temp != null) {
						resultString = "导入失败，表格第" + indexOfOrderItem + "行的订单号已存在！";
						return resultString;
					}
					Orders order = new Orders();
					order.setZhidan(zhidan).setSoftModel(softModel);
					order.setCustomerName(orderItem.getCustomerName()).setCustomerNumber(orderItem.getCustomerNumber());
					order.setAlias(orderItem.getAlias()).setProductNo(orderItem.getProductNo()).setOrderDate(orderItem.getOrderDate());
					order.setQuantity(orderItem.getQuantity()).setDeliveryDate(orderItem.getDeliveryDate()).setRemark(orderItem.getRemark());
					orders.add(order);
				}
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


	public List<OrderVO> selectUnscheduledPlan(Integer type) {
		List<OrderVO> orderVOs = new ArrayList<>();
		List<Record> records = new ArrayList<>();
		switch (type) {
		case 0:
			List<Orders> orders = Orders.dao.find(SQL.SELECT_ORDER_BY_STATUS, Constant.UNSCHEDULED_ORDERSTATUS);
			if (orders != null && !orders.isEmpty()) {
				for (Orders order : orders) {
					Record record = Db.findFirst(SQL.SELECT_PEOPLE_CAPACITY_BY_SOFTMODEL_PROCESSGROUP, Constant.ASSEMBLING_PROCESS_GROUP, order.getSoftModel());
					if (record.getInt("people").equals(0)) {
						record.set("people", 1);
					}
					OrderVO orderVO = new OrderVO(order, order.getQuantity(), record.getInt("capacity") / record.getInt("people"));
					orderVOs.add(orderVO);
				}
			}
			return orderVOs;
		case 1:
			records = Db.find(SQL.SELECT_SCHEDULED_ORDER_QUANTITY, Constant.ASSEMBLING_PROCESS_GROUP);
			break;
		case 2:
			records = Db.find(SQL.SELECT_SCHEDULED_ORDER_QUANTITY, Constant.TESTING_PROCESS_GROUP);
			break;
		default:
			break;
		}
		if (records != null && !records.isEmpty()) {
			for (Record record : records) {
				Integer orderId = record.getInt("orders");
				Integer scheduledQuantity = record.getInt("scheduled_quantity");
				Orders order = Orders.dao.findById(orderId);
				Record peopleCapacityRecord = new Record();
				if (type == 1) {
					peopleCapacityRecord = Db.findFirst(SQL.SELECT_PEOPLE_CAPACITY_BY_SOFTMODEL_PROCESSGROUP, Constant.TESTING_PROCESS_GROUP, order.getSoftModel());
				} else if (type == 2) {
					peopleCapacityRecord = Db.findFirst(SQL.SELECT_PEOPLE_CAPACITY_BY_SOFTMODEL_PROCESSGROUP, Constant.PACKING_PROCESS_GROUP, order.getSoftModel());
				}
				if (peopleCapacityRecord.getInt("people").equals(0)) {
					peopleCapacityRecord.set("people", 1);
				}
				OrderVO orderVO = new OrderVO(order, order.getQuantity() - scheduledQuantity, peopleCapacityRecord.getInt("capacity") / peopleCapacityRecord.getInt("people"));
				orderVOs.add(orderVO);
			}
		}
		return orderVOs;
	}


	public List<OrderVO> selectReworkPlan(Integer type) {
		List<OrderVO> orderVOs = new ArrayList<>();
		List<Orders> orders = null;
		Record record = new Record();
		orders = Orders.dao.find(SQL.SELECT_REWORK_ORDER);
		if (orders == null || orders.isEmpty()) {
			throw new OperationException("不存在返工订单");
		}
		if (orders != null && !orders.isEmpty()) {
			for (Orders order : orders) {
				switch (type) {
				case 0:
					record = Db.findFirst(SQL.SELECT_PEOPLE_CAPACITY_BY_SOFTMODEL_PROCESSGROUP, Constant.ASSEMBLING_PROCESS_GROUP, order.getSoftModel());
					break;
				case 1:
					record = Db.findFirst(SQL.SELECT_PEOPLE_CAPACITY_BY_SOFTMODEL_PROCESSGROUP, Constant.TESTING_PROCESS_GROUP, order.getSoftModel());
					break;
				case 2:
					record = Db.findFirst(SQL.SELECT_PEOPLE_CAPACITY_BY_SOFTMODEL_PROCESSGROUP, Constant.PACKING_PROCESS_GROUP, order.getSoftModel());
					break;
				default:
					break;
				}
				if (record.getInt("people").equals(0)) {
					record.set("people", 1);
				}
				OrderVO orderVO = new OrderVO(order, order.getQuantity(), record.getInt("capacity") / record.getInt("people"));
				orderVOs.add(orderVO);
			}
		}
		return orderVOs;
	}


	public boolean addPlan(Integer order, String remark, Integer schedulingQuantity, Integer line, Integer processGroup, Integer capacity) {
		if (Line.dao.findById(line) == null) {
			throw new OperationException("添加排产计划失败，产线不存在");
		}
		Orders orderRecord = Orders.dao.findById(order);
		if (orderRecord == null) {
			throw new OperationException("添加排产计划失败，订单不存在");
		}
		if (ProcessGroup.dao.findById(processGroup) == null) {
			throw new OperationException("添加排产计划失败，工序组不存在");
		}
		SchedulingPlan schedulingPlan = new SchedulingPlan();
		if (remark != null) {
			schedulingPlan.setRemark(remark);
		}
		if (orderRecord.getQuantity() < schedulingQuantity) {
			throw new OperationException("排产数量不能大于订单数量");
		}
		schedulingPlan.setProcessGroup(processGroup).setLine(line).setSchedulingQuantity(schedulingQuantity).setOrders(order).setIsTimeout(false);
		schedulingPlan.setLineChangeTime(Constant.DEFAULT_LINE_CHANGE_TIME).setCapacity(capacity).setSchedulingPlanStatus(Constant.SCHEDULED_PLANSTATUS);
		if (Constant.UNSCHEDULED_ORDERSTATUS.equals(orderRecord.getOrderStatus())) {
			orderRecord.setOrderStatus(Constant.SCHEDULED_ORDERSTATUS);
			return schedulingPlan.save() && orderRecord.update();
		}
		return schedulingPlan.save();
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

	/*public Page<Record> selectPlan(Integer pageNo, Integer pageSize, Integer schedulingPlanStatus, String zhidan,
			String customerName, String orderDateFrom, String orderDateTo, String planStartTimeFrom,
			String planStartTimeTo, String planCompleteTimeFrom, String planCompleteTimeTo, String startTimeFrom,
			String startTimeTo, String completeTimeFrom, String completeTimeTo, Integer processGroup, Integer line,
			String productionPlanningNumber, String softModel, String productNo) {
		StringBuilder filter = new StringBuilder();
		filter.append(concatEqualSqlFilter("scheduling_plan_status", schedulingPlanStatus));
		filter.append(concatEqualSqlFilter("scheduling_plan.process_group", processGroup));
		filter.append(concatEqualSqlFilter("line", line));
		filter.append(concatSqlFilter("zhidan", zhidan, true, false));
		filter.append(concatSqlFilter("customer_name", customerName, true, false));
		filter.append(concatSqlFilter("production_planning_number", productionPlanningNumber, true, false));
		filter.append(concatSqlFilter("soft_model", softModel, true, false));
		filter.append(concatSqlFilter("product_no", productNo, true, false));
		filter.append(concatSqlFilter("create_time", orderDateFrom, false, true));
		filter.append(concatSqlFilter("create_time", orderDateTo, false, false));
		filter.append(concatSqlFilter("plan_start_time", planStartTimeFrom, false, true));
		filter.append(concatSqlFilter("plan_start_time", planStartTimeTo, false, false));
		filter.append(concatSqlFilter("plan_complete_time", planCompleteTimeFrom, false, true));
		filter.append(concatSqlFilter("plan_complete_time", planCompleteTimeTo, false, false));
		filter.append(concatSqlFilter("start_time", startTimeFrom, false, true));
		filter.append(concatSqlFilter("start_time", startTimeTo, false, false));
		filter.append(concatSqlFilter("complete_time", completeTimeFrom, false, true));
		filter.append(concatSqlFilter("complete_time", completeTimeTo, false, false));

		SqlPara sqlPara = new SqlPara();
		String orderBy = " ORDER BY is_timeout DESC ";
		sqlPara.setSql(SQL.SELECT_SCHEDULINGPLAN + filter + orderBy);
		return formatOrderDate(Db.paginate(pageNo, pageSize, sqlPara));
	}*/

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
		String orderBy = " ORDER BY is_timeout DESC ";
		sqlPara.setSql(SQL.SELECT_SCHEDULINGPLAN + filter + orderBy);
		return formatOrderDateAndCustomer(Db.paginate(planQueryCriteria.getPageNo(), planQueryCriteria.getPageSize(), sqlPara),null);
	}


	public Page<Record> selectPlanDetail(Integer id) {
		SqlPara sqlPara = new SqlPara();
		sqlPara.setSql(SQL.SELECT_SCHEDULINGPLANDETAIL_BY_ID);
		sqlPara.addPara(id);
		Page<Record> page = Db.paginate(Constant.DEFAULT_PAGE_NUM, Constant.DEFAULT_PAGE_SIZE, sqlPara);
		if (!page.getList().isEmpty()) {
			for (Record record : page.getList()) {
				String planModifier = record.getStr("planModifier");
				String productionConfirmer = record.getStr("productionConfirmer");
				if (planModifier != null) {
					LUserAccount modifier = LUserAccount.dao.findById(planModifier);
					if (modifier != null) {
						record.set("modifierName", modifier.getName());
					}
				}
				if (productionConfirmer != null) {
					LUserAccount confirmer = LUserAccount.dao.findById(productionConfirmer);
					if (confirmer != null) {
						record.set("confirmerName", confirmer.getName());
					}
				}
			}
		}
		return page;
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
		Record record = new Record();
		record.set("planProducedQuantity", planProducedQuantity);
		return record;
	}


	public boolean editPlan(Integer id, Boolean isUrgent, String remark, Integer schedulingQuantity, Integer line,
			Date planStartTime, Date planCompleteTime, String lineChangeTime, Integer capacity, Boolean isCompleted,
			Integer producedQuantity, String remainingReason, String productionPlanningNumber, Boolean isStarting,
			LUserAccountVO userVO) {
		SchedulingPlan schedulingPlan = SchedulingPlan.dao.findById(id);
		if (schedulingPlan == null) {
			throw new OperationException("排产计划不存在");
		}
		if (Constant.COMPLETED_PLANSTATUS.equals(schedulingPlan.getSchedulingPlanStatus())) {
			throw new OperationException("已完成排产计划无法修改");
		}
		schedulingPlan.setIsUrgent(isUrgent).setLine(line).setPlanStartTime(planStartTime)
				.setPlanCompleteTime(planCompleteTime);
		if (remark != null) {
			schedulingPlan.setRemark(remark);
		}
		if (StrKit.isBlank(lineChangeTime) || lineChangeTime.length() > 8) {
			throw new ParameterException("转线时间内容为空或长度过长");
		} 
		schedulingPlan.setLineChangeTime(lineChangeTime);
		if (capacity != null) {
			schedulingPlan.setCapacity(capacity);
		}
		if (producedQuantity != null) {
			if (producedQuantity > schedulingQuantity) {
				throw new ParameterException("已完成数量不能超过排产数量");
			}
			schedulingPlan.setProducedQuantity(producedQuantity).setSchedulingQuantity(schedulingQuantity).setRemainingQuantity(schedulingQuantity - producedQuantity);
			if (schedulingPlan.getRemainingQuantity() > 0) {
				schedulingPlan.setIsTimeout(true);
			}
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
		if (Constant.SCHEDULED_PLANSTATUS.equals(schedulingPlan.getSchedulingPlanStatus()) && isStarting) {
			schedulingPlan.setSchedulingPlanStatus(Constant.WORKING_PLANSTATUS).setProductionConfirmer(userVO.getId());
		} else if (isCompleted) {
			schedulingPlan.setSchedulingPlanStatus(Constant.COMPLETED_PLANSTATUS);
			if (!order.getIsRework()) {
				Integer quantity = 0;
				List<SchedulingPlan> schedulingPlans = SchedulingPlan.dao.find(SQL.SELECT_PRODUCEDQUANTITY_BY_ORDER,
						schedulingPlan.getOrders());
				if (schedulingPlans != null && !schedulingPlans.isEmpty()) {
					for (SchedulingPlan plan : schedulingPlans) {
						quantity += plan.getProducedQuantity();
					}
				}
				if (quantity + producedQuantity >= order.getQuantity()) {
					order.setOrderStatus(Constant.COMPLETED_ORDERSTATUS);
					return schedulingPlan.update() && order.update();
				}
			} else {
				Integer assemblingQuantity = 0, testingQuantity = 0, packingQuantity = 0;
				Boolean isAssemblingCompleted = false, isTestingCompleted = false, isPackingCompleted = false;
				List<SchedulingPlan> assemblingPlans = SchedulingPlan.dao.find(SQL.SELECT_PRODUCEDQUANTITY_BY_ORDER_PROCESSGROUP, schedulingPlan.getOrders(), Constant.ASSEMBLING_PROCESS_GROUP);
				if (assemblingPlans != null && !assemblingPlans.isEmpty()) {
					for (SchedulingPlan assemblingPlan : assemblingPlans) {
						assemblingQuantity += assemblingPlan.getProducedQuantity();
					}
					if (assemblingQuantity + producedQuantity >= order.getQuantity()) {
						isAssemblingCompleted = true;
					}
				}
				List<SchedulingPlan> testingPlans = SchedulingPlan.dao.find(SQL.SELECT_PRODUCEDQUANTITY_BY_ORDER_PROCESSGROUP, schedulingPlan.getOrders(), Constant.TESTING_PROCESS_GROUP);
				if (testingPlans != null && !testingPlans.isEmpty()) {
					for (SchedulingPlan testingPlan : testingPlans) {
						testingQuantity += testingPlan.getProducedQuantity();
					}
					if (testingQuantity + producedQuantity >= order.getQuantity()) {
						isTestingCompleted = true;
					}
				}
				List<SchedulingPlan> packingPlans = SchedulingPlan.dao.find(SQL.SELECT_PRODUCEDQUANTITY_BY_ORDER_PROCESSGROUP, schedulingPlan.getOrders(), Constant.PACKING_PROCESS_GROUP);
				if (packingPlans != null && !packingPlans.isEmpty()) {
					for (SchedulingPlan packingPlan : packingPlans) {
						packingQuantity += packingPlan.getProducedQuantity();
					}
					if (packingQuantity + producedQuantity >= order.getQuantity()) {
						isPackingCompleted = true;
					}
				}
				Boolean assembling = assemblingQuantity == 0 || (assemblingQuantity > 0 && isAssemblingCompleted);
				Boolean testing = testingQuantity == 0 || (testingQuantity > 0 && isTestingCompleted);
				Boolean packing = packingQuantity == 0 || (packingQuantity > 0 && isPackingCompleted);
				if (assembling && testing && packing) {
					order.setOrderStatus(Constant.COMPLETED_ORDERSTATUS);
					return schedulingPlan.update() && order.update();
				}
			}
		}
		return schedulingPlan.update();
	}


	public boolean editPlanStatus(Integer id, Integer type, LUserAccountVO userVO) {
		SchedulingPlan schedulingPlan = SchedulingPlan.dao.findById(id);
		if (schedulingPlan == null) {
			throw new OperationException("排产计划不存在");
		}
		switch (type) {
		case 0:
			schedulingPlan.setSchedulingPlanStatus(Constant.SCHEDULED_PLANSTATUS);
			break;
		case 1:
			schedulingPlan.setSchedulingPlanStatus(Constant.WORKING_PLANSTATUS);
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
			if (record.getBoolean("isUrgent")) {
				record.set("isUrgent", "是");
			} else {
				record.set("isUrgent", "否");
			}
		}
		String fileName = "排产计划单导出_" + simpleDateFormat.format(new Date()) + "_" + size + ".xlsx";
		String title = "排产计划单";
		response.setHeader("Content-Disposition", "attachment; filename=" + new String((fileName).getBytes("utf-8"), "iso-8859-1"));
		response.setContentType("application/vnd.ms-excel");
		response.addHeader("Access-Control-Expose-Headers", "Content-Disposition");
		ExcelHelper helper = ExcelHelper.create(true);
		String[] field = new String[] { "id", "lineNo", "lineName", "statusName", "zhidan", "alias", "softModel", "productNo", "customerNumber", "customerName", "isUrgent", "orderDate", "deliveryDate", "schedulingQuantity", "productionPlanningNumber", "planStartTime", "planCompleteTime", "remark" };
		String[] head = new String[] { "序号", "线别", "线名", "状态", "订单号", "内部替换号", "机型", "成品编码", "客户编号", "客户名称", "是否紧急", "订单日期", "交货日期", "排产数量", "生产计划单号", "预计开始时间", "预计完成时间", "备注" };
		helper.fill(records, title, field, head);
		helper.write(output, true);
	}


	public Record getPlanGannt(Integer id) {
		List<Record> records = Db.find(SQL.SELECT_PLAN_INFORMATION, id);
		if (records==null||records.isEmpty()) {
			throw new OperationException("此排产计划无法获取甘特图,请检查所有配置信息");
		}
		for (Record record : records) {
			
		}
		if (record == null) {
			throw new OperationException("查询失败，计划不存在");
		}
		record.set("interval", getDateIntervalDays(record.getStr("planStartTime"), record.getStr("planEndTime")) + 1);
		return record;
	}


	public boolean checkCompleteTime(Integer schedulingQuantity, Date planStartTime, Date planCompleteTime, String lineChangeTime, Integer capacity) {
		Integer actualCostHours = schedulingQuantity / capacity;
		long lineChangeCostMilliseconds = (long) (Double.parseDouble(lineChangeTime) * 60 * 60 * 1000);
		long actualCostMilliseconds = actualCostHours * 60 * 60 * 1000 + lineChangeCostMilliseconds;
		long planCostMilliseconds = planCompleteTime.getTime() - planStartTime.getTime();
		if (actualCostMilliseconds > planCostMilliseconds) {
			return false;
		}
		return true;
	}


	private void checkUserById(Integer id) {
		LUserAccount user = LUserAccount.dao.findById(id);
		if (user == null) {
			throw new OperationException("产线负责人不存在");
		}
		if (!user.getInService()) {
			throw new OperationException("产线负责人未启用");
		}
	}


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
		return daysBetween;
	}


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


	private StringBuilder concatEqualSqlFilter(String key, Integer value) {
		StringBuilder filter = new StringBuilder();
		if (value != null) {
			filter.append(" AND " + key + " = " + value);
		}
		return filter;
	}


	private Page<Record> formatOrderDateAndCustomer(Page<Record> page,Boolean isOperator) {
		List<Record> records = page.getList();
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
		for (Record record : records) {
			if (isOperator!=null&&isOperator) {
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
	
}