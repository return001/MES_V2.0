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
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import javax.servlet.http.HttpServletResponse;

import org.apache.commons.io.FileUtils;
import org.apache.commons.lang3.StringUtils;

import com.jfinal.aop.Enhancer;
import com.jfinal.kit.PropKit;
import com.jfinal.kit.StrKit;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Page;
import com.jfinal.plugin.activerecord.Record;
import com.jfinal.plugin.activerecord.SqlPara;
import com.jfinal.upload.UploadFile;
import com.jimi.mes_server.entity.AddPlanInfo;
import com.jimi.mes_server.entity.Constant;
import com.jimi.mes_server.entity.ModelCapacityInfo;
import com.jimi.mes_server.entity.OrderItem;
import com.jimi.mes_server.entity.PlanDetail;
import com.jimi.mes_server.entity.PlanGantt;
import com.jimi.mes_server.entity.PlanQueryCriteria;
import com.jimi.mes_server.entity.PlanResult;
import com.jimi.mes_server.entity.CalculatePlanResultParam;
import com.jimi.mes_server.entity.SQL;
import com.jimi.mes_server.entity.SchedulingProgress;
import com.jimi.mes_server.entity.SopSQL;
import com.jimi.mes_server.entity.Task;
import com.jimi.mes_server.entity.WorkTime;
import com.jimi.mes_server.entity.WorkTimes;
import com.jimi.mes_server.entity.vo.AuthorityVO;
import com.jimi.mes_server.entity.vo.LUserAccountVO;
import com.jimi.mes_server.entity.vo.OrderVO;
import com.jimi.mes_server.exception.OperationException;
import com.jimi.mes_server.model.FileType;
import com.jimi.mes_server.model.LUserAccount;
import com.jimi.mes_server.model.Line;
import com.jimi.mes_server.model.LineComputer;
import com.jimi.mes_server.model.ModelCapacity;
import com.jimi.mes_server.model.ModelCapacityStatus;
import com.jimi.mes_server.model.OrderFile;
import com.jimi.mes_server.model.Orders;
import com.jimi.mes_server.model.Process;
import com.jimi.mes_server.model.ProcessGroup;
import com.jimi.mes_server.model.Role;
import com.jimi.mes_server.model.SchedulingPlan;
import com.jimi.mes_server.model.SopFactory;
import com.jimi.mes_server.model.SopSite;
import com.jimi.mes_server.model.SopWorkshop;
import com.jimi.mes_server.model.WorkingSchedule;
import com.jimi.mes_server.service.base.SelectService;
import com.jimi.mes_server.util.CommonUtil;
import com.jimi.mes_server.util.ExcelHelper;

/**排产模块业务层
 * @author   HCJ
 * @date     2019年8月16日 上午11:53:55
 */
public class ProductionService {

	private static SelectService daoService = Enhancer.enhance(SelectService.class);

	private static ExecutorService executorService = Executors.newFixedThreadPool(6);


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


	public Page<Record> getFileType() {
		SqlPara sqlPara = new SqlPara();
		sqlPara.setSql(SQL.SELECT_FILETYPE_NAME_ID);
		return Db.paginate(Constant.DEFAULT_PAGE_NUM, Constant.DEFAULT_PAGE_SIZE, sqlPara);
	}


	public Map<Integer, String> getWorkingSchedule() {
		List<WorkingSchedule> workingSchedules = WorkingSchedule.dao.find(SQL.SELECT_WORKINGSCHEDULE);
		Map<Integer, String> map = new HashMap<>();
		for (WorkingSchedule workingSchedule : workingSchedules) {
			map.put(workingSchedule.getId(), workingSchedule.getTime());
		}
		return map;
	}


	public boolean addProcessGroup(String groupNo, String groupName, String groupRemark, Integer factory) {
		if (SopFactory.dao.findById(factory) == null) {
			throw new OperationException("当前工厂不存在");
		}
		if (ProcessGroup.dao.findFirst(SQL.SELECT_PROCESSGROUP_BY_GROUPNO, groupNo, factory) != null) {
			throw new OperationException("工序组编号已存在");
		}
		if (ProcessGroup.dao.findFirst(SQL.SELECT_PROCESSGROUP_BY_GROUPNAME, groupName, factory) != null) {
			throw new OperationException("工序组名称已存在");
		}
		ProcessGroup processGroup = new ProcessGroup();
		processGroup.setGroupNo(groupNo).setGroupName(groupName).setGroupRemark(groupRemark).setFactory(factory);
		processGroup.save();
		return processGroup.setPosition(processGroup.getId()).update();
	}


	public boolean deleteProcessGroup(Integer id) {
		if (id <= 5) {
			throw new OperationException("删除失败，无法删除当前工序组");
		}
		ProcessGroup processGroup = ProcessGroup.dao.findById(id);
		if (processGroup == null) {
			throw new OperationException("删除失败，工序组不存在");
		}
		return processGroup.delete();
	}


	public Page<Record> selectProcessGroup(String groupNo, String groupName, Integer factory) {
		SqlPara sqlPara = new SqlPara();
		StringBuilder sql = new StringBuilder(SQL.SELECT_PROCESSGROUP);
		if (!StrKit.isBlank(groupNo)) {
			sql.append(" and group_no like '%").append(groupNo).append("%'");
		}
		if (!StrKit.isBlank(groupName)) {
			sql.append(" and group_name like '%").append(groupName).append("%'");
		}
		if (factory != null && factory > 0) {
			sql.append(" and process_group.factory = ").append(factory);
		}
		sql.append(" order by position");
		sqlPara.setSql(sql.toString());
		return Db.paginate(Constant.DEFAULT_PAGE_NUM, Constant.DEFAULT_PAGE_SIZE, sqlPara);
	}


	public boolean editProcessGroup(Integer id, String groupNo, String groupName, String groupRemark, Integer factory) {
		if (SopFactory.dao.findById(factory) == null) {
			throw new OperationException("当前工厂不存在");
		}
		ProcessGroup processGroup = ProcessGroup.dao.findById(id);
		if (processGroup == null) {
			throw new OperationException("修改失败，项目组不存在");
		}
		if (!StrKit.isBlank(groupNo)) {
			ProcessGroup group = ProcessGroup.dao.findFirst(SQL.SELECT_PROCESSGROUP_BY_GROUPNO, groupNo, factory);
			if (group != null && !group.getId().equals(id)) {
				throw new OperationException("工序组编号已存在");
			}
			processGroup.setGroupNo(groupNo);
		}
		if (!StrKit.isBlank(groupName)) {
			ProcessGroup group = ProcessGroup.dao.findFirst(SQL.SELECT_PROCESSGROUP_BY_GROUPNAME, groupName, factory);
			if (group != null && !group.getId().equals(id)) {
				throw new OperationException("工序组名称已存在");
			}
			processGroup.setGroupName(groupName);
		}
		processGroup.setGroupRemark(groupRemark).setFactory(factory);
		return processGroup.update();
	}


	public boolean addLine(String lineNo, String lineName, String lineRemark, Integer lineDirector, Integer lineEngineer, Integer lineQc, Integer processGroup, Integer workshopId, Integer timeLength) {
		if (Line.dao.findFirst(SQL.SELECT_LINE_BY_LINENO, lineNo, workshopId) != null) {
			throw new OperationException("产线线别已存在");
		}
		if (Line.dao.findFirst(SQL.SELECT_LINE_BY_LINENAME, lineName, workshopId) != null) {
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
		if (workshopId != null) {
			if (SopWorkshop.dao.findById(workshopId) == null) {
				throw new OperationException("车间不存在");
			}
			line.setWorkshopId(workshopId);
		}
		line.setTimeLength(timeLength);
		return line.save();
	}


	public boolean deleteLine(Integer id) {
		Line line = Line.dao.findById(id);
		if (line == null) {
			throw new OperationException("删除失败，产线不存在");
		}
		SopSite sopSite = SopSite.dao.findFirst(SopSQL.SELECT_SITE_BY_LINE, id);
		if (sopSite != null) {
			throw new OperationException("当前产线被站点所引用，请先删除站点，再进行操作");
		}
		List<LineComputer> lineComputers = LineComputer.dao.find(SQL.SELECT_LINECOMPUTER_BY_LINE, id);
		if (lineComputers != null && !lineComputers.isEmpty()) {
			for (LineComputer lineComputer : lineComputers) {
				lineComputer.delete();
			}
		}
		return line.delete();
	}


	public Page<Record> selectLine(String lineNo, String lineName, Integer processGroup, Integer workshopId, Integer factoryId, String lineDirector, String lineEngineer, String lineQc) {
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
		if (workshopId != null) {
			filter.append(" and line.workshop_id = " + workshopId);
		}
		if (factoryId != null && factoryId > 0) {
			filter.append(" and sop_factory.id = " + factoryId);

		}
		if (!StrKit.isBlank(lineDirector)) {
			filter.append(" and a.Name like '%" + lineDirector + "%'");
		}
		if (!StrKit.isBlank(lineEngineer)) {
			filter.append(" and b.Name like '%" + lineEngineer + "%'");
		}
		if (!StrKit.isBlank(lineQc)) {
			filter.append(" and c.Name like '%" + lineQc + "%'");
		}
		SqlPara sqlPara = new SqlPara();
		String orderBy = " order by processGroup ";
		sqlPara.setSql(SQL.SELECT_LINE_PROCESSGROUP + filter + orderBy);
		return Db.paginate(Constant.DEFAULT_PAGE_NUM, Constant.DEFAULT_PAGE_SIZE, sqlPara);
	}


	public boolean editLine(Integer id, String lineNo, String lineName, String lineRemark, Integer lineDirector, Integer lineEngineer, Integer lineQc, Integer processGroup, Integer workshopId, Integer factoryId, Integer timeLength) {
		Line line = Line.dao.findById(id);
		if (line == null) {
			throw new OperationException("修改失败，产线不存在");
		}
		if (!StrKit.isBlank(lineNo)) {
			Line temp = Line.dao.findFirst(SQL.SELECT_LINE_BY_LINENO, lineNo, workshopId);
			if (temp != null && !temp.getId().equals(id)) {
				throw new OperationException("产线线别已存在");
			}
			line.setLineNo(lineNo);
		}
		if (!StrKit.isBlank(lineName)) {
			Line temp = Line.dao.findFirst(SQL.SELECT_LINE_BY_LINENAME, lineName, workshopId);
			if (temp != null && !temp.getId().equals(id)) {
				throw new OperationException("产线名称已存在");
			}
			line.setLineName(lineName);
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
		if (workshopId != null) {
			if (SopWorkshop.dao.findById(workshopId) == null) {
				throw new OperationException("车间不存在");
			}
			line.setWorkshopId(workshopId);
		}
		line.setLineRemark(lineRemark);
		line.setTimeLength(timeLength);
		return line.update();
	}


	public boolean addProcess(Integer processNo, String processName, String processRemark, Integer processGroup) {
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


	public Page<Record> selectProcess(Integer processNo, String processName, Integer processGroup) {
		StringBuilder filter = new StringBuilder();
		if (processNo != null) {
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


	public boolean editProcess(Integer id, Integer processNo, String processName, String processRemark, Integer processGroup, Integer position) {
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
		if (processNo != null) {
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
		if (processGroup != null) {
			if (ProcessGroup.dao.findById(processGroup) == null) {
				throw new OperationException("工序组不存在");
			}
			process.setProcessGroup(processGroup);
		}
		process.setProcessRemark(processRemark);
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
		lineComputer.setRemark(remark);
		return lineComputer.update();
	}


	public boolean addCapacity(List<ModelCapacityInfo> modelCapacityInfos) {
		List<ModelCapacity> modelCapacities = ModelCapacityInfo.fill(modelCapacityInfos);
		if (modelCapacities != null && !modelCapacities.isEmpty()) {
			for (ModelCapacity modelCapacity : modelCapacities) {
				Record record = Db.findFirst(SQL.SELECT_MODELCAPACITY_BY_MODEL_PROCESS, modelCapacity.getSoftModel(), modelCapacity.getProcessGroup(), modelCapacity.getCustomerName(), modelCapacity.getCustomerNumber());
				if (record != null) {
					throw new OperationException("当前客户机型产能已存在");
				}
				modelCapacity.remove("id");
				modelCapacity.setModelCapacityStatus(Constant.TO_BE_CONFIRMED_MODELCAPACITY_STATUS);
			}
			Db.batchSave(modelCapacities, modelCapacities.size());
		}
		return true;
	}


	public boolean deleteCapacity(Integer id) {
		ModelCapacity modelCapacity = ModelCapacity.dao.findById(id);
		if (modelCapacity == null) {
			throw new OperationException("删除失败，机型产能不存在");
		}
		return modelCapacity.delete();
	}


	public Page<Record> selectCapacity(Integer pageNo, Integer pageSize, String softModel, String customerModel, Integer processGroup, Integer factory, Integer statusId) {
		StringBuilder filter = new StringBuilder();
		if (processGroup != null) {
			if (ProcessGroup.dao.findById(processGroup) == null) {
				throw new OperationException("工序组不存在");
			}
			filter.append(" and process_group = " + processGroup);
		}
		if (!StrKit.isBlank(softModel)) {
			filter.append(" and soft_model like '" + softModel + "%' ");
		}
		if (!StrKit.isBlank(customerModel)) {
			filter.append(" and customer_model like '" + customerModel + "%' ");
		}
		if (factory != null && factory > 0) {
			filter.append(" AND sop_factory.id = " + factory);
		}
		if (statusId != null) {
			filter.append(" AND model_capacity_status.id =  " + statusId);
		}
		String orderBy = "ORDER BY soft_model";
		SqlPara sqlPara = new SqlPara();
		sqlPara.setSql(SQL.SELECT_MODELCAPACITY + filter + orderBy);
		return Db.paginate(pageNo, pageSize, sqlPara);
	}


	public boolean editCapacity(List<ModelCapacityInfo> modelCapacityInfos) {
		List<ModelCapacity> modelCapacities = ModelCapacityInfo.fill(modelCapacityInfos);
		if (modelCapacities != null && !modelCapacities.isEmpty()) {
			for (ModelCapacity modelCapacity : modelCapacities) {
				ModelCapacity capacity = ModelCapacity.dao.findFirst(SQL.SELECT_MODELCAPACITY_BY_MODEL_PROCESS, modelCapacity.getSoftModel(), modelCapacity.getProcessGroup(), modelCapacity.getCustomerName(), modelCapacity.getCustomerNumber());
				if (capacity != null && !capacity.getId().equals(modelCapacity.getId())) {
					throw new OperationException("当前客户机型产能已存在");
				}
				modelCapacity.setModelCapacityStatus(Constant.TO_BE_CONFIRMED_MODELCAPACITY_STATUS);
			}
			Db.batchUpdate(modelCapacities, modelCapacities.size());
		}
		return true;
	}


	public boolean reviewCapacity(String softModel, String customerNumber, String customerModel, String reviewRemark, Integer statusId, Integer factoryId, LUserAccountVO userVO) {
		if (SopFactory.dao.findById(factoryId) == null) {
			throw new OperationException("当前工厂不存在");
		}
		if (ModelCapacityStatus.dao.findById(statusId) == null) {
			throw new OperationException("当前机型产能状态不存在");
		}
		StringBuilder sql = new StringBuilder(SQL.SELECT_MODELCAPACITY_BY_COLUMNS);
		if (!StrKit.isBlank(customerModel)) {
			sql.append(" AND customer_model = '").append(customerModel).append("'");
		}
		if (!StrKit.isBlank(customerNumber)) {
			sql.append(" AND customer_number = '").append(customerNumber).append("'");
		}
		List<ModelCapacity> capacities = ModelCapacity.dao.find(sql.toString(), factoryId, softModel);
		for (ModelCapacity modelCapacity : capacities) {
			modelCapacity.setModelCapacityStatus(statusId).setReviewRemark(reviewRemark);
			modelCapacity.setReviewer(userVO.getId()).setReviewTime(new Date());
		}
		Db.batchUpdate(capacities, capacities.size());
		return true;
	}


	public boolean addFileType(String typeName, String typeRemarks) {
		if (FileType.dao.findFirst(SQL.SELECT_FILETYPE_BY_TYPENAME, typeName) != null) {
			throw new OperationException("类型名称已存在");
		}
		FileType fileType = new FileType();
		fileType.setTypeName(typeName).setTypeRemarks(typeRemarks);
		return fileType.save();
	}


	public boolean deleteFileType(Integer id) {
		if (id == 1) {
			throw new OperationException("无法删除当前类型");
		}
		FileType fileType = FileType.dao.findById(id);
		if (fileType == null) {
			throw new OperationException("删除失败，文件类型不存在");
		}
		return fileType.delete();
	}


	public Page<Record> selectFileType(String filter) {
		return daoService.select(SQL.SELECT_FILETYPE, Constant.DEFAULT_PAGE_NUM, Constant.DEFAULT_PAGE_SIZE, null, null, filter);
	}


	public boolean editFileType(Integer id, String typeName, String typeRemarks) {
		if (id == 1) {
			throw new OperationException("无法修改当前类型");
		}
		FileType fileType = FileType.dao.findById(id);
		if (fileType == null) {
			throw new OperationException("修改失败，文件类型不存在");
		}
		if (!StrKit.isBlank(typeName)) {
			FileType type = FileType.dao.findFirst(SQL.SELECT_FILETYPE_BY_TYPENAME, typeName);
			if (type != null && !type.getId().equals(id)) {
				throw new OperationException("类型名称已存在");
			}
			fileType.setTypeName(typeName);
		}
		fileType.setTypeRemarks(typeRemarks);
		return fileType.update();
	}


	public boolean addOrder(Orders order, LUserAccountVO userVO) {
		Orders zhidan = null;
		Orders alias = null;
		if (order.getIsRework()) {
			zhidan = Orders.dao.findFirst(SQL.SELECT_ORDER_BY_ZHIDAN_REWORKZHIDAN, order.getZhidan(), order.getReworkZhidan());
			alias = Orders.dao.findFirst(SQL.SELECT_ORDER_BY_ZHIDAN_ALIAS, order.getReworkZhidan(), order.getAlias());
		} else {
			zhidan = Orders.dao.findFirst(SQL.SELECT_ORDER_BY_ZHIDAN_ISREWORK, order.getZhidan());
			alias = Orders.dao.findFirst(SQL.SELECT_ORDER_BY_ALIAS_ISREWORK, order.getAlias());
		}
		if (zhidan != null) {
			throw new OperationException("订单号已存在");
		}
		if (alias != null) {
			throw new OperationException("内部替换号已存在");
		}
		order.setOrderStatus(Constant.WAIT_CONFIRM_ORDERSTATUS);
		order.setOrderCreator(userVO.getId()).setOrderCreateTime(new Date());
		if (!StrKit.isBlank(order.getPcRemarks())) {
			order.setPc(userVO.getUserDes());
			order.setPcRemarksTime(new Date());
		}
		return order.save();
	}


	public boolean abnormalEndOrder(Integer id, String abnormalEndReason, LUserAccountVO userVO) {
		Orders order = Orders.dao.findById(id);
		if (order == null) {
			throw new OperationException("异常结单失败，订单不存在");
		}
		checkOrderUserType(userVO, order);
		order.setAbnormalEndReason(abnormalEndReason).setOrderStatus(Constant.ABNORMAL_END_ORDERSTATUS);
		order.setAbnormalEndPerson(userVO.getId()).setAbnormalEndTime(new Date());
		return order.update();
	}


	/**
	 * 
	 * <p>Description: 查询订单，需要权限“/production/selectOrder:withCustomer”才可以查询到客户名<p>
	 * @return
	 * @exception 
	 * @author trjie
	 * @Time 2020年4月22日
	 */
	public Page<Record> selectOrder(Integer pageNo, Integer pageSize, Integer factoryId, Integer status, String zhidan, String alias, String softModel, String productNo, String customerNumber, String customerName, String orderDateFrom, String orderDateTo, String deliveryDateFrom, String deliveryDateTo, Boolean isRework, String customerMaterialNo, String reworkZhidan, LUserAccountVO userVO) {

		SqlPara sqlPara = new SqlPara();
		StringBuilder sql = new StringBuilder(SQL.SELECT_ORDER);
		if (factoryId != null) {
			sql.append(" and factory = " + factoryId);
		}
		if (status != null) {
			sql.append(" and order_status = " + status);
		}
		if (isRework) {
			sql.append(" and is_rework = 1 ");
		}
		if (!StrKit.isBlank(zhidan)) {
			sql.append(" and zhidan like '" + zhidan + "%'");
		}
		if (!StrKit.isBlank(alias)) {
			sql.append(" and alias like '" + alias + "%'");
		}
		if (!StrKit.isBlank(softModel)) {
			sql.append(" and soft_model like '" + softModel + "%'");
		}
		if (!StrKit.isBlank(productNo)) {
			sql.append(" and product_no like '" + productNo + "%'");
		}
		if (!StrKit.isBlank(customerNumber)) {
			sql.append(" and customer_number like '" + customerNumber + "%'");
		}
		if (!StrKit.isBlank(customerName)) {
			sql.append(" and customer_name like '" + customerName + "%'");
		}
		if (!StrKit.isBlank(customerMaterialNo)) {
			sql.append(" and customer_material_no like '" + customerMaterialNo + "%'");
		}
		if (!StrKit.isBlank(reworkZhidan)) {
			sql.append(" and rework_zhidan like '" + reworkZhidan + "%'");
		}
		if (!StrKit.isBlank(orderDateFrom)) {
			sql.append(" and order_date >= '" + orderDateFrom + "'");
		}
		if (!StrKit.isBlank(orderDateTo)) {
			sql.append(" and order_date <= '" + orderDateTo + "'");
		}
		if (!StrKit.isBlank(deliveryDateFrom)) {
			sql.append(" and delivery_date >= '" + deliveryDateFrom + "'");
		}
		if (!StrKit.isBlank(deliveryDateTo)) {
			sql.append(" and delivery_date <= '" + deliveryDateTo + "'");
		}
		sql.append(" ORDER BY order_status ASC,delivery_date ASC");
		sqlPara.setSql(sql.toString());
		for (AuthorityVO authorityVO : userVO.getWebAuthorities()) {
			if (authorityVO.getUrls().contains("/production/selectOrder:withCustomer")) {
				return formatOrderDateAndCustomer(Db.paginate(pageNo, pageSize, sqlPara), false);
			}
		}
		return formatOrderDateAndCustomer(Db.paginate(pageNo, pageSize, sqlPara), true);
	}


	public Record selectOrderDetail(Integer id) {
		Orders order = Orders.dao.findById(id);
		if (order == null) {
			throw new OperationException("订单不存在");
		}
		List<Record> orderFiles = Db.find(SQL.SELECT_ORDERFILE_BY_ORDER, id);
		Record record = new Record();
		record.set("orderUser", selectOrderUser(id));
		record.set("orderFileInfo", orderFiles);
		return record;
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
			String abnormalEndPerson = record.getStr("abnormalEndPerson");
			if (orderModifier != null) {
				LUserAccount modifier = LUserAccount.dao.findById(orderModifier);
				if (modifier != null) {
					record.set("modifierName", modifier.getUserDes());
				}
			}
			if (abnormalEndPerson != null) {
				LUserAccount abnormalEndPersonUser = LUserAccount.dao.findById(abnormalEndPerson);
				if (abnormalEndPersonUser != null) {
					record.set("abnormalEndPersonName", abnormalEndPersonUser.getUserDes());
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
		Orders zhidan = null;
		Orders alias = null;
		if (order.getIsRework()) {
			zhidan = Orders.dao.findFirst(SQL.SELECT_ORDER_BY_ZHIDAN_REWORKZHIDAN, order.getZhidan(), order.getReworkZhidan());
			alias = Orders.dao.findFirst(SQL.SELECT_ORDER_BY_ZHIDAN_ALIAS, order.getZhidan(), order.getAlias());
		} else {
			zhidan = Orders.dao.findFirst(SQL.SELECT_ORDER_BY_ZHIDAN_ISREWORK, order.getZhidan());
			alias = Orders.dao.findFirst(SQL.SELECT_ORDER_BY_ALIAS_ISREWORK, order.getAlias());
		}
		if (zhidan != null && !zhidan.getId().equals(order.getId())) {
			throw new OperationException("订单号已存在");
		}
		if (alias != null && !alias.getId().equals(order.getId())) {
			throw new OperationException("内部替换号已存在");
		}
		if (!StrKit.isBlank(order.getPcRemarks())) {
			if ((!StrKit.isBlank(temp.getPcRemarks()) && !temp.getPcRemarks().equals(order.getPcRemarks())) || StrKit.isBlank(temp.getPcRemarks())) {
				order.setPc(userVO.getUserDes());
				order.setPcRemarksTime(new Date());
			}
		}
		if (!StrKit.isBlank(order.getPmcRemarks())) {
			if ((!StrKit.isBlank(temp.getPmcRemarks()) && !temp.getPmcRemarks().equals(order.getPmcRemarks())) || StrKit.isBlank(temp.getPmcRemarks())) {
				order.setPmc(userVO.getUserDes());
				order.setPmcRemarksTime(new Date());
			}
		}
		if (!StrKit.isBlank(order.getEngineerRemarks())) {
			if ((!StrKit.isBlank(temp.getEngineerRemarks()) && !temp.getEngineerRemarks().equals(order.getEngineerRemarks())) || StrKit.isBlank(temp.getEngineerRemarks())) {
				order.setEngineer(userVO.getUserDes());
				order.setEngineerRemarksTime(new Date());
			}
		}
		if (!StrKit.isBlank(order.getProducerRemarks())) {
			if ((!StrKit.isBlank(temp.getProducerRemarks()) && !temp.getProducerRemarks().equals(order.getProducerRemarks())) || StrKit.isBlank(temp.getProducerRemarks())) {
				order.setProducer(userVO.getUserDes());
				order.setProducerRemarksTime(new Date());
			}
		}
		if (!StrKit.isBlank(order.getQcRemarks())) {
			if ((!StrKit.isBlank(temp.getQcRemarks()) && !temp.getQcRemarks().equals(order.getQcRemarks())) || StrKit.isBlank(temp.getQcRemarks())) {
				order.setQc(userVO.getUserDes());
				order.setQcRemarksTime(new Date());
			}
		}
		order.setOrderModifier(userVO.getId()).setOrderModifyTime(new Date());
		if (Constant.ABNORMAL_END_ORDERSTATUS.equals(order.getOrderStatus())) {
			Db.update(SQL.UPDATE_SCHEDULING_STATUS_BY_ZHIDAN, Constant.ABNORMAL_END_PLANSTATUS, order.getId());
			return order.update();
		}
		if (Constant.SCHEDULED_ORDERSTATUS.equals(temp.getOrderStatus())) {
			temp.setQuantity(order.getQuantity());
			temp.update();
			/*throw new OperationException("只有待确定和未排产订单能够修改");*/
		}
		/*if (!Constant.UNSCHEDULED_ORDERSTATUS.equals(temp.getOrderStatus()) && !Constant.WAIT_CONFIRM_ORDERSTATUS.equals(temp.getOrderStatus())) {
			throw new OperationException("只有待确定和未排产订单能够修改");
		}*/
		return order.update();
	}


	public String importOrder(File file, LUserAccountVO userVO, Boolean isRework, Integer factory) {
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
				String zhidan = orderItem.getZhidan();
				String softModel = orderItem.getSoftModel();
				boolean isDateExist = orderItem.getOrderDate() != null && orderItem.getDeliveryDate() != null;
				boolean isQuantityExist = orderItem.getQuantity() != null;
				if (StringUtils.isAnyBlank(zhidan, softModel) || !isDateExist || !isQuantityExist) {
					resultString = "导入失败，表格第" + indexOfOrderItem + "行的订单号和机型为空或者订单日期、交货日期和订单数量格式错误！";
					return resultString;
				}
				if (!isRework) {
					Orders temp = Orders.dao.findFirst(SQL.SELECT_ORDER_BY_ZHIDAN_ISREWORK, orderItem.getZhidan());
					if (temp != null) {
						resultString = "导入失败，表格第" + indexOfOrderItem + "行的订单号已存在！";
						return resultString;
					}
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
				order.setCustomerName(Constant.DEFAULT_CUSTOMERNAME).setCustomerNumber(Constant.DEFAULT_CUSTOMERNUMBER);
				order.setProductNo(orderItem.getProductNo()).setOrderDate(orderItem.getOrderDate());
				order.setQuantity(orderItem.getQuantity()).setDeliveryDate(orderItem.getDeliveryDate());
				order.setFactory(factory);
				if (!StrKit.isBlank(orderItem.getRemark())) {
					order.setPcRemarks(orderItem.getRemark()).setPc(userVO.getUserDes()).setPcRemarksTime(new Date());
				}
				orders.add(order);
				indexOfOrderItem++;
			}
			for (Orders order : orders) {
				order.setOrderStatus(Constant.WAIT_CONFIRM_ORDERSTATUS).setIsRework(isRework).setOrderCreator(userVO.getId()).setOrderCreateTime(new Date());
			}
			Db.batchSave(orders, orders.size());
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
		/*switch (type) {
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
		}*/
		Orders order = Orders.dao.findById(id);
		if (order == null) {
			throw new OperationException("订单不存在");
		}
		PropKit.use("properties.ini");
		String filePath = null;
		if (CommonUtil.isWindows()) {
			filePath = PropKit.get("windowsFlagPath");
		} else {
			filePath = PropKit.get("linuxFlagPath");
		}
		File dir = new File(filePath + Constant.FILE_TABLE_PATH);
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
			File file = new File(filePath + Constant.FILE_TABLE_PATH + fileName);
			if (file.exists()) {
				file.delete();
			}
			try {
				FileUtils.moveFile(tempFile, file);
			} catch (IOException e) {
				throw new OperationException("文件保存失败");
			}
			orderFile.setFileType(type);
			orderFile.setFileName(fileName).setPath(file.getAbsolutePath()).setOrders(id).setUploader(userVO.getId()).setUploadTime(new Date());
			orderFile.remove("id");
			orderFile.save();
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


	public boolean editOrderTable(Integer id, LUserAccountVO userVO, Boolean isNormal) {
		OrderFile orderFile = OrderFile.dao.findById(id);
		if (orderFile == null) {
			throw new OperationException("文件记录不存在");
		}
		orderFile.setConfirmPerson(userVO.getId()).setConfirmTime(new Date()).setIsNormal(isNormal);
		return orderFile.update();
	}


	public boolean deleteOrderTable(Integer id, String deleteReason, LUserAccountVO userVO) {
		OrderFile orderFile = OrderFile.dao.findById(id);
		if (orderFile == null) {
			throw new OperationException("文件记录不存在");
		}
		File file = new File(orderFile.getPath());
		PropKit.use("properties.ini");
		String filePath = null;
		if (CommonUtil.isWindows()) {
			filePath = PropKit.get("windowsFlagPath");
		} else {
			filePath = PropKit.get("linuxFlagPath");
		}
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy年MM月dd日HH时mm分ss秒删除");
		String newFileName = orderFile.getFileName().replace(".", "_" + dateFormat.format(new Date()) + ".");
		String newFilePath = filePath + Constant.FILE_DELETE_TABLE_PATH + newFileName;
		if (file.exists()) {
			File dir = new File(filePath + Constant.FILE_DELETE_TABLE_PATH);
			if (!dir.exists()) {
				dir.mkdirs();
			}
			File newFile = new File(newFilePath);
			try {
				FileUtils.moveFile(file, newFile);
			} catch (Exception e) {
				throw new OperationException("当前文件已存在，移动失败");
			}
		}
		orderFile.setDeletePerson(userVO.getId()).setDeleteTime(new Date()).setDeleteReason(deleteReason);
		orderFile.setFileName(newFileName).setPath(newFilePath);
		return orderFile.update();
	}


	public List<OrderVO> selectUnscheduledPlan(Integer type, Integer factory) {
		List<OrderVO> orderVOs = new ArrayList<>();
		List<Record> orderRecords = new ArrayList<>();
		Integer rework = 0;
		switch (type) {
		case 0:
			List<Orders> orders;
			if (factory > 0) {
				orders = Orders.dao.find(SQL.SELECT_ORDER_BY_STATUS_FACTORY_ISREWORK, rework, factory, Constant.UNSCHEDULED_ORDERSTATUS, Constant.SCHEDULED_ORDERSTATUS, Constant.ONGOING_ORDERSTATUS);
			} else {
				orders = Orders.dao.find(SQL.SELECT_ORDER_BY_STATUS_ISREWORK, rework, Constant.UNSCHEDULED_ORDERSTATUS, Constant.SCHEDULED_ORDERSTATUS, Constant.ONGOING_ORDERSTATUS);
			}
			if (orders == null || orders.isEmpty()) {
				throw new OperationException("不存在可排产的订单");
			}
			for (Orders order : orders) {
				orderVOs.add(genOrderVO(order, type));
			}
			return removeScheduledOrder(orderVOs);
		case 1:
			if (factory > 0) {
				orderRecords = Db.find(SQL.SELECT_DISTINCT_ORDER_BY_PROCESSGROUP__FACTORY_ORDERSTATUS, Constant.ASSEMBLING_PROCESS_GROUP, factory, Constant.UNSCHEDULED_ORDERSTATUS, Constant.SCHEDULED_ORDERSTATUS, Constant.ONGOING_ORDERSTATUS);
			} else {
				orderRecords = Db.find(SQL.SELECT_DISTINCT_ORDER_BY_PROCESSGROUP_ORDERSTATUS, Constant.ASSEMBLING_PROCESS_GROUP, Constant.UNSCHEDULED_ORDERSTATUS, Constant.SCHEDULED_ORDERSTATUS, Constant.ONGOING_ORDERSTATUS);
			}
			if (orderRecords == null || orderRecords.isEmpty()) {
				throw new OperationException("不存在前置工序组已排产的订单");
			}
			break;
		case 2:
			if (factory > 0) {
				orderRecords = Db.find(SQL.SELECT_DISTINCT_ORDER_BY_PROCESSGROUP__FACTORY_ORDERSTATUS, Constant.TESTING_PROCESS_GROUP, factory, Constant.UNSCHEDULED_ORDERSTATUS, Constant.SCHEDULED_ORDERSTATUS, Constant.ONGOING_ORDERSTATUS);
			} else {
				orderRecords = Db.find(SQL.SELECT_DISTINCT_ORDER_BY_PROCESSGROUP_ORDERSTATUS, Constant.TESTING_PROCESS_GROUP, Constant.UNSCHEDULED_ORDERSTATUS, Constant.SCHEDULED_ORDERSTATUS, Constant.ONGOING_ORDERSTATUS);
			}
			if (orderRecords == null || orderRecords.isEmpty()) {
				throw new OperationException("不存在前置工序组已排产的订单");
			}
			break;
		case 3:
		case 4:
			List<Orders> list = null;
			if (factory > 0) {
				list = Orders.dao.find(SQL.SELECT_ORDER_BY_STATUS_FACTORY_ISREWORK, rework, factory, Constant.UNSCHEDULED_ORDERSTATUS, Constant.SCHEDULED_ORDERSTATUS, Constant.ONGOING_ORDERSTATUS);
			} else {
				list = Orders.dao.find(SQL.SELECT_ORDER_BY_STATUS_ISREWORK, rework, Constant.UNSCHEDULED_ORDERSTATUS, Constant.SCHEDULED_ORDERSTATUS, Constant.ONGOING_ORDERSTATUS);
			}
			if (list == null || list.isEmpty()) {
				throw new OperationException("不存在可排产的订单");
			}
			for (Orders order : list) {
				orderVOs.add(genOrderVO(order, type));
			}
			return removeScheduledOrder(orderVOs);
		default:
			break;
		}
		for (Record orderRecord : orderRecords) {
			Orders order = Orders.dao.findById(orderRecord.getInt("orders"));
			orderVOs.add(genOrderVO(order, type));
		}
		return removeScheduledOrder(orderVOs);
	}


	public List<OrderVO> selectReworkPlan(Integer type, Integer factory) {
		List<OrderVO> orderVOs = new ArrayList<>();
		Integer rework = 1;
		List<Orders> orders = null;
		if (factory > 0) {
			orders = Orders.dao.find(SQL.SELECT_ORDER_BY_STATUS_FACTORY_ISREWORK, rework, factory, Constant.UNSCHEDULED_ORDERSTATUS, Constant.SCHEDULED_ORDERSTATUS, Constant.ONGOING_ORDERSTATUS);
		} else {
			orders = Orders.dao.find(SQL.SELECT_ORDER_BY_STATUS_ISREWORK, rework, Constant.UNSCHEDULED_ORDERSTATUS, Constant.SCHEDULED_ORDERSTATUS, Constant.ONGOING_ORDERSTATUS);
		}
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
			Integer quantity = addPlanInfo.getSchedulingQuantity();
			Integer line = addPlanInfo.getLine();
			Integer capacity = addPlanInfo.getCapacity();
			Integer processGroup = addPlanInfo.getProcessGroup();
			Integer order = addPlanInfo.getOrder();
			Date planStartTime = addPlanInfo.getPlanStartTime();
			Date planCompleteTime = addPlanInfo.getPlanCompleteTime();
			Double rhythm = addPlanInfo.getRhythm();
			Integer personNumber = addPlanInfo.getPersonNumber();
			String lineChangeTime = addPlanInfo.getLineChangeTime();
			Boolean isUrgent = addPlanInfo.getIsUrgent();
			Orders orderRecord = Orders.dao.findById(addPlanInfo.getOrder());
			if (orderRecord == null) {
				throw new OperationException("添加排产计划失败，订单不存在");
			}
			if (ProcessGroup.dao.findById(addPlanInfo.getProcessGroup()) == null) {
				throw new OperationException("添加排产计划失败，工序组不存在");
			}
			if (orderRecord.getQuantity() < quantity) {
				throw new OperationException("排产数量不能大于订单数量");
			}
			SchedulingPlan schedulingPlan = new SchedulingPlan();
			if (!StrKit.isBlank(addPlanInfo.getRemark())) {
				schedulingPlan.setRemark(addPlanInfo.getRemark());
			}
			schedulingPlan.setProcessGroup(processGroup).setLine(line).setSchedulingQuantity(quantity).setOrders(order).setIsTimeout(false);
			schedulingPlan.setLineChangeTime(lineChangeTime).setCapacity(capacity).setSchedulingPlanStatus(Constant.SCHEDULED_PLANSTATUS);
			schedulingPlan.setScheduler(userVO.getId()).setSchedulingTime(new Date());
			schedulingPlan.setPlanStartTime(planStartTime).setPlanCompleteTime(planCompleteTime);
			schedulingPlan.setPersonNumber(personNumber).setRhythm(BigDecimal.valueOf(rhythm)).setIsUrgent(isUrgent);
			schedulingPlan.save();

			if (Constant.UNSCHEDULED_ORDERSTATUS.equals(orderRecord.getOrderStatus())) {
				orderRecord.setOrderStatus(Constant.SCHEDULED_ORDERSTATUS);
				orderRecord.update();
			}
		}
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
		filter.append(concatSqlFilter("customer_material_no", planQueryCriteria.getCustomerMaterialNo(), true, false));
		filter.append(concatSqlFilter("alias", planQueryCriteria.getAlias(), true, false));
		SqlPara sqlPara = new SqlPara();
		String orderBy = " ORDER BY scheduling_plan_status asc,plan_start_time ASC  ";
		sqlPara.setSql(SQL.SELECT_SCHEDULINGPLAN + filter + orderBy);
		Page<Record> page = Db.paginate(planQueryCriteria.getPageNo(), planQueryCriteria.getPageSize(), sqlPara);
		return formatPlanTimeOut(formatOrderDateAndCustomer(page, null));
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
					planUser.set("modifierName", modifier.getUserDes());
				}
			}
			if (productionConfirmer != null) {
				LUserAccount confirmer = LUserAccount.dao.findById(productionConfirmer);
				if (confirmer != null) {
					planUser.set("confirmerName", confirmer.getUserDes());
				}
			}
		}
		order = Db.findFirst(SQL.SELECT_ORDER_BY_PLAN_ID, id);
		Map<String, String> picture = new HashMap<>();
		List<OrderFile> orderFiles = OrderFile.dao.find(SQL.SELECT_ORDER_FILE_BY_ORDER, order.getInt("id"));
		if (orderFiles != null && !orderFiles.isEmpty()) {
			for (OrderFile orderFile : orderFiles) {
				if (Constant.SCHEDULING_ABOUT.equals(orderFile.getFileType())) {
					if (orderFile.getFileName().contains(".jpg") || orderFile.getFileName().contains(".jpeg") || orderFile.getFileName().contains(".png") || orderFile.getFileName().contains(".bmp")) {
						picture.put(orderFile.getFileName(), orderFile.getPath());
					}
				}
			}
		}
		planDetail.setOrder(order);
		planDetail.setPlanUser(planUser);
		planDetail.setPicture(picture);
		return planDetail;
	}


	/**@author HCJ
	 * 查询计划已生产数量并且更新信息
	 * @param id 排产计划ID
	 * @param record 排产计划显示记录
	 * @date 2020年5月6日 下午4:15:52
	 */
	public Record selectPlanProducedQuantityAndUpdate(Integer id, Record record, SimpleDateFormat dateFormat) {
		SchedulingPlan schedulingPlan = SchedulingPlan.dao.findById(id);
		if (schedulingPlan == null) {
			throw new OperationException("计划不存在");
		}
		Orders order = Orders.dao.findById(schedulingPlan.getOrders());
		if (order == null) {
			throw new OperationException("订单不存在");
		}
		Line line = Line.dao.findById(schedulingPlan.getLine());
		if (line == null) {
			throw new OperationException("产线不存在");
		}
		String sql = null;
		Integer planProducedQuantity = 0;
		Integer scheduledQuantity = 0;
		List<Record> scheduledQuantities = null;
		String lineNameParam = "'" + line.getLineName() + "%'";
		// 查询产线和订单各工序组对应产量
		if (Constant.ASSEMBLING_PROCESS_GROUP.equals(schedulingPlan.getProcessGroup())) {
			sql = SQL.SELECT_ASSEMBLING_PROCESS_GROUP_PRUDUCEDQUANTITY;
		} else if (Constant.TESTING_PROCESS_GROUP.equals(schedulingPlan.getProcessGroup())) {
			sql = SQL.SELECT_TESTING_PROCESS_GROUP_PRUDUCEDQUANTITY;
		} else if (Constant.PACKING_PROCESS_GROUP.equals(schedulingPlan.getProcessGroup())) {
			sql = SQL.SELECT_PACKING_PROCESS_GROUP_PRUDUCEDQUANTITY;
		} else if (Constant.SMT_PROCESS_GROUP.equals(schedulingPlan.getProcessGroup())) {
			sql = SQL.SELECT_ASSEMBLING_PROCESS_GROUP_PRUDUCEDQUANTITY;
		}
		String planStartTime = dateFormat.format(schedulingPlan.getPlanStartTime());
		String zhidan;
		if (order.getIsRework()) {
			zhidan = order.getReworkZhidan();
		} else {
			zhidan = order.getZhidan();
		}
		if (sql != null) {
			planProducedQuantity = Db.queryInt(sql, zhidan, lineNameParam, planStartTime);
		}
		// 最后一个工序组订单已排产数量
		scheduledQuantities = Db.find(SQL.SELECT_SCHEDULED_ORDER_WORKSTATION_QUANTITY, order.getId(), order.getId());
		if (scheduledQuantities != null && !scheduledQuantities.isEmpty()) {
			for (Record scheduledQuantityRecord : scheduledQuantities) {
				if (scheduledQuantityRecord.getInt("scheduled_quantity") != null) {
					scheduledQuantity = scheduledQuantityRecord.getInt("scheduled_quantity");
					break;
				}
			}
		}
		order.setScheduledQuantity(scheduledQuantity);
		// 进行中状态
		if (Constant.SCHEDULED_ORDERSTATUS.equals(order.getOrderStatus()) && planProducedQuantity > 0) {
			order.setOrderStatus(Constant.ONGOING_ORDERSTATUS);
		}
		// 最后一个段的生产数量和未完成原因
		if (Constant.PACKING_PROCESS_GROUP.equals(schedulingPlan.getProcessGroup()) || Constant.SMT_PROCESS_GROUP.equals(schedulingPlan.getProcessGroup())) {
			Integer zhidanPlanProducedQuantity = 0;
			if (Constant.PACKING_PROCESS_GROUP.equals(schedulingPlan.getProcessGroup())) {
				zhidanPlanProducedQuantity = Db.queryInt(SQL.SELECT_PACKING_PROCESS_GROUP_TOTAL_PRUDUCEDQUANTITY, zhidan);
			} else {
				zhidanPlanProducedQuantity = Db.queryInt(SQL.SELECT_ASSEMBLING_PROCESS_GROUP_TOTAL_PRUDUCEDQUANTITY, zhidan);
			}
			order.setProducedQuantity(zhidanPlanProducedQuantity);
			order.setUnfinishedReason(schedulingPlan.getRemainingReason());
			// 已完成
			if (order.getIsRework()) {
				if (zhidanPlanProducedQuantity >= order.getReworkQuantity()) {
					order.setOrderStatus(Constant.COMPLETED_ORDERSTATUS);
				}
			} else {
				if (zhidanPlanProducedQuantity >= order.getQuantity()) {
					order.setOrderStatus(Constant.COMPLETED_ORDERSTATUS);
				}
			}
			// 待通知
			if (Constant.WAIT_NOTIFICATION_PLANSTATUS.equals(schedulingPlan.getSchedulingPlanStatus())) {
				order.setOrderStatus(Constant.WAIT_NOTICE__ORDERSTATUS);
			}
		}
		if (record != null) {
			Integer processGroup = record.getInt("processGroup");
			// 开始时间
			if (StrKit.isBlank(record.getStr("startTime"))) {
				Record startTime = null;
				if (Constant.ASSEMBLING_PROCESS_GROUP.equals(processGroup) || Constant.SMT_PROCESS_GROUP.equals(schedulingPlan.getProcessGroup())) {
					startTime = Db.findFirst(SQL.SELECT_ASSEMBLING_START_TIME, zhidan, lineNameParam);
				} else if (Constant.TESTING_PROCESS_GROUP.equals(processGroup)) {
					startTime = Db.findFirst(SQL.SELECT_TESTING_START_TIME, zhidan, lineNameParam);
				} else if (Constant.PACKING_PROCESS_GROUP.equals(processGroup)) {
					startTime = Db.findFirst(SQL.SELECT_PACKING_START_TIME, zhidan, lineNameParam);
				}
				if (startTime != null) {
					record.set("startTime", startTime.getStr("TestTime"));
					schedulingPlan.setStartTime(startTime.getDate("TestTime"));
				}
			}
			// 完成时间和未完成量
			if (StrKit.isBlank(record.getStr("completeTime"))) {
				Record lastTime = null;
				if (Constant.ASSEMBLING_PROCESS_GROUP.equals(processGroup) || Constant.SMT_PROCESS_GROUP.equals(schedulingPlan.getProcessGroup())) {
					lastTime = Db.findFirst(SQL.SELECT_ASSEMBLING_LAST_TIME, zhidan, lineNameParam);
				} else if (Constant.TESTING_PROCESS_GROUP.equals(processGroup)) {
					lastTime = Db.findFirst(SQL.SELECT_TESTING_LAST_TIME, zhidan, lineNameParam);
				} else if (Constant.PACKING_PROCESS_GROUP.equals(processGroup)) {
					lastTime = Db.findFirst(SQL.SELECT_PACKING_LAST_TIME, zhidan, lineNameParam);
				}
				if (planProducedQuantity >= schedulingPlan.getSchedulingQuantity() && lastTime != null) {
					record.set("completeTime", lastTime.getStr("TestTime"));
					schedulingPlan.setSchedulingPlanStatus(Constant.COMPLETED_PLANSTATUS);
					schedulingPlan.setCompleteTime(lastTime.getDate("TestTime"));
					schedulingPlan.setRemainingQuantity(0);
				} else {
					record.set("remainingQuantity", schedulingPlan.getSchedulingQuantity() - planProducedQuantity);
					schedulingPlan.setRemainingQuantity(schedulingPlan.getSchedulingQuantity() - planProducedQuantity);
				}
				schedulingPlan.setProducedQuantity(planProducedQuantity);
			}
		}

		Runnable runnable = new Runnable() {

			@Override
			public void run() {
				order.update();
				schedulingPlan.update();
			}
		};
		executorService.execute(runnable);
		Record quantity = new Record();
		quantity.set("planProducedQuantity", planProducedQuantity);
		return quantity;
	}


	public boolean editPlan(Integer id, Boolean isUrgent, String remark, Integer schedulingQuantity, Integer line, Date planStartTime, Date planCompleteTime, String lineChangeTime, Integer capacity, Boolean isCompleted, String remainingReason, String productionPlanningNumber, LUserAccountVO userVO, Integer personNumber, Double rhythm) {
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
		schedulingPlan.setRemark(remark);
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
		if (rhythm != null) {
			schedulingPlan.setRhythm(BigDecimal.valueOf(rhythm));
		}
		schedulingPlan.setPersonNumber(personNumber);
		if (isCompleted != null && isCompleted) {
			if (schedulingPlan.getProducedQuantity() < schedulingQuantity) {
				throw new OperationException("无法完成此排产计划，已生产数量未达到排产数量");
			}
			schedulingPlan.setSchedulingPlanStatus(Constant.COMPLETED_PLANSTATUS);
			schedulingPlan.setCompleteTime(new Date());
			Integer quantity = 0;
			if (order.getIsRework()) {
				quantity = Db.queryInt(SQL.SELECT_CARTONTEST_NUMBER_BY_ZHIDAN_SOFTMODEL, order.getReworkZhidan());
			} else {
				quantity = Db.queryInt(SQL.SELECT_CARTONTEST_NUMBER_BY_ZHIDAN_SOFTMODEL, order.getZhidan());
			}
			if (quantity >= order.getQuantity()) {
				order.setOrderStatus(Constant.COMPLETED_ORDERSTATUS);
				return schedulingPlan.update() && order.update();
			}
		}
		return schedulingPlan.update();
	}


	public boolean reSchedulingPlan(List<AddPlanInfo> addPlanInfos, LUserAccountVO userVO) {
		List<SchedulingPlan> plans = AddPlanInfo.fill(addPlanInfos);
		for (SchedulingPlan schedulingPlan : plans) {
			if (schedulingPlan.getId() != null) {
				schedulingPlan.delete();
			}
		}
		List<Orders> orders = new ArrayList<>();
		for (SchedulingPlan schedulingPlan : plans) {
			schedulingPlan.setScheduler(userVO.getId()).setSchedulingTime(new Date());
			schedulingPlan.setPlanModifier(userVO.getId()).setPlanModifyTime(new Date());
			schedulingPlan.setSchedulingPlanStatus(Constant.SCHEDULED_PLANSTATUS).setIsTimeout(false);
			schedulingPlan.remove("id");
			Orders order = Orders.dao.findById(schedulingPlan.getOrders());
			if (Constant.UNSCHEDULED_ORDERSTATUS.equals(order.getOrderStatus())) {
				order.setOrderStatus(Constant.SCHEDULED_ORDERSTATUS);
				orders.add(order);
			}
		}
		Db.batchSave(plans, plans.size());
		Db.batchUpdate(orders, orders.size());
		return true;
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
		String[] field = new String[] { "id", "lineNo", "lineName", "statusName", "zhidan", "alias", "softModel", "productNo", "customerMaterialNo", "customerNumber", "customerName", "isUrgent", "orderDate", "deliveryDate", "schedulingQuantity", "productionPlanningNumber", "planStartTime", "planCompleteTime", "pcRemarks" };
		String[] head = new String[] { "序号", "线别", "线名", "状态", "订单号/生产PO", "内部替换号/系统单号", "机型", "成品编码", "客户料号", "客户编号", "客户名称", "是否紧急", "订单日期", "交货日期", "排产数量", "生产计划单号", "预计开始时间", "预计完成时间", "PC备注" };
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
		String zhidan;
		if (records.get(0).getBoolean("is_rework")) {
			zhidan = records.get(0).getStr("rework_zhidan");
		} else {
			zhidan = records.get(0).getStr("ZhiDan");
		}
		Integer planProduction = records.get(0).getInt("scheduling_quantity");
		PlanGantt planGantt = new PlanGantt();
		planGantt.setId(index);
		planGantt.setName(zhidan);
		planGantt.setPlanProduction(planProduction.toString());
		/*StringBuilder computerSql = new StringBuilder(" and (");
		for (Record record : records) {
			computerSql.append(" Computer LIKE '%" + record.getStr("ip") + "%' or ");
		}
		if (StringUtils.endsWith(computerSql, "or ")) {
			computerSql.delete(computerSql.lastIndexOf("or"), computerSql.length());
		}
		String sql = computerSql.append(")").toString();*/
		PlanGantt smtGantt = genPlanGantt(index + 1, zhidan, planProduction);
		PlanGantt functionGantt = genPlanGantt(index + 2, zhidan, planProduction);
		PlanGantt agedGantt = genPlanGantt(index + 3, zhidan, planProduction);
		PlanGantt couplingGantt = genPlanGantt(index + 4, zhidan, planProduction);
		PlanGantt cartonGantt = genPlanGantt(index + 5, zhidan, planProduction);
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


	public Page<Record> selectProductionLog(Integer pageNo, Integer pageSize, String startTime, String endTime, String userName, String address, String zhidan, String alias) {
		SqlPara sqlPara = new SqlPara();
		StringBuilder sql = new StringBuilder(SQL.SELECT_PRODUCTION_ACTION_LOG);
		sql.append(" WHERE 1 = 1 ");
		if (!StrKit.isBlank(userName)) {
			sql.append(" AND uid like '").append(userName).append("%'");
		}
		if (!StrKit.isBlank(zhidan)) {
			sql.append(" AND zhidan like '").append(zhidan).append("%'");
		}
		if (!StrKit.isBlank(alias)) {
			sql.append(" AND alias like '").append(alias).append("%'");
		}
		if (!StrKit.isBlank(address)) {
			sql.append(" AND address like '").append(address).append("%'");
		}
		if (!StrKit.isBlank(startTime)) {
			sql.append(" AND time >= '").append(startTime).append(" 00:00:00'");
		}
		if (!StrKit.isBlank(endTime)) {
			sql.append(" AND time <= '").append(endTime).append(" 23:59:59'");
		}
		if (StringUtils.endsWith(sql, "WHERE 1 = 1 ")) {
			sql.substring(0, sql.lastIndexOf("WHERE"));
		}
		sql.append(" order by id desc");
		sqlPara.setSql(sql.toString());
		return Db.paginate(pageNo, pageSize, sqlPara);
	}


	public boolean setDefaultWorkTimeByExecutorId(Integer executorId, List<WorkTime> times) {
		Line line = Line.dao.findById(executorId);
		if (line == null) {
			throw new OperationException("当前产线不存在");
		}
		StringBuilder defaultWorkTime = new StringBuilder();
		for (WorkTime workTime : times) {
			defaultWorkTime.append(workTime.getStartTime()).append("-").append(workTime.getEndTime()).append(",");
		}
		if (StringUtils.contains(defaultWorkTime, ",")) {
			defaultWorkTime.deleteCharAt(defaultWorkTime.lastIndexOf(","));
		}
		return line.setDefaultWorkTime(defaultWorkTime.toString()).update();
	}


	public Object getDefaultWorkTimeByExecutorId(Integer executorId) {
		Line line = Line.dao.findById(executorId);
		if (line == null) {
			throw new OperationException("当前产线不存在");
		}
		if (!StrKit.isBlank(line.getDefaultWorkTime())) {
			List<WorkTime> workTimes = new ArrayList<>();
			String[] WorkTimeStrings = line.getDefaultWorkTime().split(",");
			for (String WorkTimeString : WorkTimeStrings) {
				WorkTime workTime = new WorkTime();
				workTime.setStartTime(WorkTimeString.split("-")[0]);
				workTime.setEndTime(WorkTimeString.split("-")[1]);
				workTimes.add(workTime);
			}
			return workTimes;
		}
		return Collections.EMPTY_LIST;
	}


	public List<PlanResult> calculatePlanResult(CalculatePlanResultParam calculatePlanResultParam) throws Exception {
		// 存储产线ID和工作进度的MAP
		Map<Integer, SchedulingProgress> map = new LinkedHashMap<>();
		for (WorkTimes workTimes : calculatePlanResultParam.getWorkTimes()) {
			SchedulingProgress schedulingProgress = new SchedulingProgress();
			schedulingProgress.setWorkSchedule(workTimes.getWorkTime());
			map.put(workTimes.getExecutorId(), schedulingProgress);
		}
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
		SimpleDateFormat dateAndHourFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm");
		List<PlanResult> planResults = new ArrayList<>();
		for (Entry<Integer, SchedulingProgress> entry : map.entrySet()) {
			for (Task task : calculatePlanResultParam.getTasks()) {
				if (task.getExecutorId().equals(entry.getKey())) {
					// 预计生产时长
					double planProductionHour = BigDecimal.valueOf((double) task.getPlanQuantity() / (double) task.getStandardCapacity() + (double) task.getSwitchConsumingTime() / 60).setScale(2, BigDecimal.ROUND_HALF_UP).doubleValue();
					// 预计开始时间
					String planStartTime = null;
					// 预计结束时间
					String planEndTime = null;
					// 预计生产时长的毫秒数
					long planProductionMillisecond = (long) (planProductionHour * 60 * 60 * 1000);
					// 上一个订单预计结束时间对应工作时间表的下标
					Integer index = entry.getValue().getPreviousOrderPlanEndTimeIndex();
					// 上一个订单预计结束时间
					Date time = entry.getValue().getPreviousOrderPlanEndTime();
					// 工作时间表
					List<WorkTime> workTimes = entry.getValue().getWorkSchedule();
					if (index >= workTimes.size()) {
						planStartTime = "超出预期";
						planEndTime = "超出预期";
					} else {
						WorkTime workTimeFirst = workTimes.get(index);
						if (time == null) {
							// 不存在上一个订单时
							planStartTime = dateFormat.format(workTimeFirst.getDate()) + " " + workTimeFirst.getStartTime();
						} else {
							// 存在上一个订单时
							planStartTime = dateAndHourFormat.format(time);
						}
						if (index < workTimes.size()) {
							// 遍历时间表然后将预计需要花费的时间分散到时间表的各个时间段
							while (index < workTimes.size()) {
								WorkTime workTimeSecond = workTimes.get(index);
								String tempStartTime;
								String tempEndTime;
								// 获取当前订单对应时间表上可用的开始时间
								if (time == null) {
									tempStartTime = dateFormat.format(workTimeSecond.getDate()) + " " + workTimeSecond.getStartTime();
								} else {
									tempStartTime = dateAndHourFormat.format(time);
									time = null;
								}
								// 获取当前订单对应时间表上可用的结束时间
								tempEndTime = dateFormat.format(workTimeSecond.getDate()) + " " + workTimeSecond.getEndTime();
								// 判断间隔时间是否满足所需要花费的时间
								if (dateAndHourFormat.parse(tempEndTime).getTime() - dateAndHourFormat.parse(tempStartTime).getTime() < planProductionMillisecond) {
									// 计算还需要多少时间才能完成
									planProductionMillisecond = planProductionMillisecond - (dateAndHourFormat.parse(tempEndTime).getTime() - dateAndHourFormat.parse(tempStartTime).getTime());
									// 时间表的下一个可用时间段
									index++;
								} else {
									// 间隔时间满足所需要花费的时间所以直接使用开始时间和花费的时间
									planEndTime = dateAndHourFormat.format(new Date((dateAndHourFormat.parse(tempStartTime).getTime() + planProductionMillisecond)));
									break;
								}
							}
							if (index >= workTimes.size()) {
								planEndTime = "超出预期";
							}
						} else {
							planEndTime = "超出预期";
						}
					}
					// 存储上一个订单的预计结束时间
					if (!"超出预期".equals(planEndTime)) {
						entry.getValue().setPreviousOrderPlanEndTime(dateAndHourFormat.parse(planEndTime));
					}
					// 存储上一个订单使用到的时间段对应时间表上的序号
					entry.getValue().setPreviousOrderPlanEndTimeIndex(index);
					// 存储订单对应的计算结果
					PlanResult planResult = new PlanResult();
					planResult.setExecutorId(task.getExecutorId());
					planResult.setOrderId(task.getOrderId());
					planResult.setEstimatedProductionTime(planProductionHour);
					planResult.setEstimatedStartTime(planStartTime);
					planResult.setEstimatedEndTime(planEndTime);
					planResults.add(planResult);
				}
			}
		}
		return planResults;
	}


	/**@author HCJ
	 * 生成甘特图
	 * @param index 位置
	 * @param zhidan 订单号
	 * @param planProduction 计划产量
	 * @param computerSql 产线电脑IP的查询条件
	 * @date 2019年8月16日 上午11:56:26
	 */
	private PlanGantt genPlanGantt(Integer index, String zhidan, Integer planProduction) {
		Map<String, Integer> dailyCapacity = new HashMap<>();
		PlanGantt gantt = new PlanGantt();
		gantt.setId(index);
		List<Record> records = null;
		switch (index) {
		case 2:
			gantt.setName(Constant.SMT_TEST);
			records = Db.find(SQL.SELECT_SMTTEST_NUMBER_BY_ZHIDAN, zhidan);
			break;
		case 3:
			gantt.setName(Constant.FUNCTION_TEST);
			records = Db.find(SQL.SELECT_FUNCTIONTEST_NUMBER_BY_ZHIDAN, zhidan);
			break;
		case 4:
			gantt.setName(Constant.AGED_TEST);
			records = Db.find(SQL.SELECT_AGEDTEST_NUMBER_BY_ZHIDAN, zhidan);
			break;
		case 5:
			gantt.setName(Constant.COUPLING_TEST);
			records = Db.find(SQL.SELECT_COUPLETEST_NUMBER_BY_ZHIDAN, zhidan);
			break;
		case 6:
			gantt.setName(Constant.CARTON_TEST);
			records = Db.find(SQL.SELECT_CARTONTEST_NUMBER_BY_ZHIDAN, zhidan);
			break;
		default:
			throw new OperationException("无法识别的类型");
		}
		Integer number = 0;
		if (records != null && records.size() > 0) {
			for (Record record : records) {
				number = number + record.getInt("number");
				dailyCapacity.put(record.getStr("time"), record.getInt("number"));
			}
			gantt.setPlanProduction(planProduction.toString());
			gantt.setStartTime(records.get(0).getStr("time"));
			if (number >= planProduction) {
				/*Record lastRecord = new Record();
				switch (index) {
				case 2:
					lastRecord = Db.findFirst(SQL.SELECT_LAST_SMTTEST_FRAGMENT_ONE + planProduction + SQL.SELECT_LAST_TEST_FRAGMENT_ONE + SQL.SELECT_LAST_TEST_FRAGMENT_TWO, zhidan);
					break;
				case 3:
					lastRecord = Db.findFirst(SQL.SELECT_LAST_FUNCTIONTEST_FRAGMENT_ONE + planProduction + SQL.SELECT_LAST_TEST_FRAGMENT_ONE + SQL.SELECT_LAST_TEST_FRAGMENT_TWO, zhidan);
					break;
				case 4:
					lastRecord = Db.findFirst(SQL.SELECT_LAST_AGEDTEST_FRAGMENT_ONE + planProduction + SQL.SELECT_LAST_TEST_FRAGMENT_ONE + SQL.SELECT_LAST_TEST_FRAGMENT_TWO, zhidan);
					break;
				case 5:
					lastRecord = Db.findFirst(SQL.SELECT_LAST_COUPLETEST_FRAGMENT_ONE + planProduction + SQL.SELECT_LAST_TEST_FRAGMENT_ONE + SQL.SELECT_LAST_TEST_FRAGMENT_TWO, zhidan);
					break;
				case 6:
					lastRecord = Db.findFirst(SQL.SELECT_LAST_CARTONTEST_FRAGMENT_ONE + planProduction + SQL.SELECT_LAST_TEST_FRAGMENT_ONE + SQL.SELECT_LAST_TEST_FRAGMENT_TWO, zhidan);
					break;
				default:
					break;
				}*/
				gantt.setEndTime(records.get(records.size() - 1).getStr("time"));
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
		gantt.setDailyCapacity(dailyCapacity);
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
		SimpleDateFormat format = new SimpleDateFormat("yyyy.MM.dd");
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
		for (Record record : records) {
			if (isOperator != null && isOperator) {
				record.set("customerNumber", "***");
				record.set("customerName", "***");
			}
			if (!StringUtils.isAnyBlank(record.getStr("orderDate"), record.getStr("deliveryDate"))) {
				record.set("orderDate", record.getStr("orderDate").replace(" 00:00:00", ""));
				record.set("deliveryDate", record.getStr("deliveryDate").replace(" 00:00:00", ""));
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
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		for (Record record : records) {
			if (record.getDate("planCompleteTime") == null) {
				continue;
			}
			Record planProducedQuantity = selectPlanProducedQuantityAndUpdate(record.getInt("id"), record, dateFormat);
			if (planProducedQuantity != null) {
				record.set("producedQuantity", planProducedQuantity.getInt("planProducedQuantity"));
			}
			if (new Date().after(record.getDate("planCompleteTime"))) {
				if (planProducedQuantity.getInt("planProducedQuantity") < record.getInt("schedulingQuantity")) {
					record.set("isTimeout", true);
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
		OrderVO orderVO = new OrderVO(order);
		Record capacityRecord = null;
		Integer scheduledQuantity = null;
		if (type == Constant.AUTOTEST_LINEID) {
			scheduledQuantity = Db.queryInt(SQL.SELECT_SCHEDULED_ORDER_QUANTITY, Constant.ASSEMBLING_PROCESS_GROUP, order.getId());
			if (StringUtils.isAnyBlank(order.getCustomerNumber(), order.getCustomerName())) {
				capacityRecord = Db.findFirst(SQL.SELECT_PEOPLE_CAPACITY_BY_SOFTMODEL, Constant.ASSEMBLING_PROCESS_GROUP, order.getSoftModel());
			} else {
				capacityRecord = Db.findFirst(SQL.SELECT_PEOPLE_CAPACITY_BY_SOFTMODEL_PROCESSGROUP, Constant.ASSEMBLING_PROCESS_GROUP, order.getSoftModel(), order.getCustomerNumber(), order.getCustomerName());
			}
		} else if (type == Constant.COUPLETEST_LINEID) {
			scheduledQuantity = Db.queryInt(SQL.SELECT_SCHEDULED_ORDER_QUANTITY, Constant.TESTING_PROCESS_GROUP, order.getId());
			if (StringUtils.isAnyBlank(order.getCustomerNumber(), order.getCustomerName())) {
				capacityRecord = Db.findFirst(SQL.SELECT_PEOPLE_CAPACITY_BY_SOFTMODEL, Constant.TESTING_PROCESS_GROUP, order.getSoftModel());
			} else {
				capacityRecord = Db.findFirst(SQL.SELECT_PEOPLE_CAPACITY_BY_SOFTMODEL_PROCESSGROUP, Constant.TESTING_PROCESS_GROUP, order.getSoftModel(), order.getCustomerNumber(), order.getCustomerName());
			}
		} else if (type == Constant.CARTONTEST_LINEID) {
			scheduledQuantity = Db.queryInt(SQL.SELECT_SCHEDULED_ORDER_QUANTITY, Constant.PACKING_PROCESS_GROUP, order.getId());
			if (StringUtils.isAnyBlank(order.getCustomerNumber(), order.getCustomerName())) {
				capacityRecord = Db.findFirst(SQL.SELECT_PEOPLE_CAPACITY_BY_SOFTMODEL, Constant.PACKING_PROCESS_GROUP, order.getSoftModel());
			} else {
				capacityRecord = Db.findFirst(SQL.SELECT_PEOPLE_CAPACITY_BY_SOFTMODEL_PROCESSGROUP, Constant.PACKING_PROCESS_GROUP, order.getSoftModel(), order.getCustomerNumber(), order.getCustomerName());
			}
		} else if (type == Constant.PATCH_LINEID) {
			scheduledQuantity = Db.queryInt(SQL.SELECT_SCHEDULED_ORDER_QUANTITY, Constant.PATCH_PROCESS_GROUP, order.getId());
			if (StringUtils.isAnyBlank(order.getCustomerNumber(), order.getCustomerName())) {
				capacityRecord = Db.findFirst(SQL.SELECT_PEOPLE_CAPACITY_BY_SOFTMODEL, Constant.PATCH_PROCESS_GROUP, order.getSoftModel());
			} else {
				capacityRecord = Db.findFirst(SQL.SELECT_PEOPLE_CAPACITY_BY_SOFTMODEL_PROCESSGROUP, Constant.PATCH_PROCESS_GROUP, order.getSoftModel(), order.getCustomerNumber(), order.getCustomerName());
			}
		} else if (type == Constant.SMT_LINEID) {
			scheduledQuantity = Db.queryInt(SQL.SELECT_SCHEDULED_ORDER_QUANTITY, Constant.SMT_PROCESS_GROUP, order.getId());
			if (StringUtils.isAnyBlank(order.getCustomerNumber(), order.getCustomerName())) {
				capacityRecord = Db.findFirst(SQL.SELECT_PEOPLE_CAPACITY_BY_SOFTMODEL, Constant.SMT_PROCESS_GROUP, order.getSoftModel());
			} else {
				capacityRecord = Db.findFirst(SQL.SELECT_PEOPLE_CAPACITY_BY_SOFTMODEL_PROCESSGROUP, Constant.SMT_PROCESS_GROUP, order.getSoftModel(), order.getCustomerNumber(), order.getCustomerName());
			}
		}
		if (scheduledQuantity == null) {
			if (order.getIsRework()) {
				orderVO.setUnscheduledQuantity(order.getReworkQuantity());
			} else {
				orderVO.setUnscheduledQuantity(order.getQuantity());
			}
		} else {
			if (order.getIsRework()) {
				orderVO.setUnscheduledQuantity(order.getReworkQuantity() - scheduledQuantity);
			} else {
				orderVO.setUnscheduledQuantity(order.getQuantity() - scheduledQuantity);
			}
		}
		if (capacityRecord == null) {
			orderVO.setCapacity(Constant.INTEGER_ZERO);
			orderVO.setProcessPeopleQuantity(Constant.INTEGER_ZERO);
			orderVO.setTransferLineTime(Constant.INTEGER_ZERO);
			orderVO.setRhythm(0.0);
		} else {
			Integer capacity = capacityRecord.getInt("capacity");
			Integer processPeopleQuantity = capacityRecord.getInt("processPeopleQuantity");
			Integer transferLineTime = capacityRecord.getInt("transferLineTime");
			Double rhythm = capacityRecord.getDouble("rhythm");
			if (capacity == null) {
				orderVO.setCapacity(Constant.INTEGER_ZERO);
			} else {
				orderVO.setCapacity(capacity);
			}
			if (processPeopleQuantity == null) {
				orderVO.setProcessPeopleQuantity(Constant.INTEGER_ZERO);
			} else {
				orderVO.setProcessPeopleQuantity(processPeopleQuantity);
			}
			if (transferLineTime == null) {
				orderVO.setTransferLineTime(Constant.INTEGER_ZERO);
			} else {
				orderVO.setTransferLineTime(transferLineTime);
			}
			if (rhythm == null) {
				orderVO.setRhythm(0.0);
			} else {
				orderVO.setRhythm(rhythm);
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
		if (user.getRole() == null || userVO == null || userVO.getRoleVO() == null) {
			throw new OperationException("当前角色无法操作此订单");
		}
		Role role = Role.dao.findById(user.getRole());
		if (user != null && role != null) {
			if (!role.getName().equals(userVO.getRoleVO().getName())) {
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
