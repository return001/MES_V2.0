package com.jimi.mes_server.entity;

import java.io.File;

public class Constant {

	public static final Integer ENGINEER_USERTYPE = 1;

	public static final Integer SUPER_ADMIN_USERTYPE = 2;

	public static final Integer SUPER_OPERATOR_USERTYPE = 3;

	public static final Integer SCHEDULING_SZPC_USERTYPE = 7;

	public static final Integer SCHEDULING_JMPMC_USERTYPE = 8;

	public static final String SUPER_ADMIN_DELETEPERMISSION = "1,1,1,1,1,1,1,1,1,1";

	public static final String ORDINARY_DELETEPERMISSION = "0,0,0,0,0,0,0,0,0,0";

	public static final String EXIST_DELETEPERMISSION = "1";

	public final static String FILE_BACKUP_PATH = File.separator + "home" + File.separator + "mes_document" + File.separator + "backups" + File.separator;

	public final static String FILE_TABLE_PATH = File.separator + "home" + File.separator + "mes_document" + File.separator + "tables" + File.separator;

	public static final Integer UNSCHEDULED_ORDERSTATUS = 1;

	public static final Integer SCHEDULED_ORDERSTATUS = 2;

	public static final Integer COMPLETED_ORDERSTATUS = 3;

	public static final Integer DELETED_ORDERSTATUS = 4;

	public static final Integer INFORMATION_FILETYPE = 1;

	public static final Integer BOM_FILETYPE = 2;

	public static final Integer SOP_FILETYPE = 3;

	public static final Integer SCHEDULED_PLANSTATUS = 1;

	public static final Integer WORKING_PLANSTATUS = 2;

	public static final Integer COMPLETED_PLANSTATUS = 3;

	public static final Integer WAIT_NOTIFICATION_PLANSTATUS = 4;

	public static final String TABLE_PROCESS = "process";

	public static final String TABLE_PROCESS_GROUP = "process_group";

	public static final String TABLE_LINE = "line";

	public static final String TABLE_ORDERS = "orders";

	public static final String TABLE_SCHEDULING_PLAN = "scheduling_plan";

	public static final String TABLE_MODEL_CAPACITY = "model_capacity";

	public static final Integer DEFAULT_PAGE_NUM = 1;

	public static final Integer DEFAULT_PAGE_SIZE = 2147483647;

	public static final Integer ASSEMBLING_PROCESS_GROUP = 1;

	public static final Integer TESTING_PROCESS_GROUP = 2;

	public static final Integer PACKING_PROCESS_GROUP = 3;

	public static final String DEFAULT_LINE_CHANGE_TIME = "0.5";

	public static final Integer AUTOTEST_LINEID = 0;

	public static final Integer COUPLETEST_LINEID = 1;

	public static final Integer CARTONTEST_LINEID = 2;

	public static final String SMT_TEST = "SMT功能测试位";

	public static final String FUNCTION_TEST = "组装测试位";

	public static final String AGED_TEST = "老化后测试";

	public static final String COUPLING_TEST = "耦合测试";

	public static final String IMEI_TEST = "IMEI对比";

	public static final String CARTON_TEST = "卡通箱测试";
	
}
