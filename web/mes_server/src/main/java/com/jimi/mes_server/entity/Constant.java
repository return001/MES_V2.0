package com.jimi.mes_server.entity;

import java.io.File;

public class Constant {

	public final static String FILE_BACKUP_PATH = "mes_document" + File.separator + "backups" + File.separator;

	public final static String FILE_TABLE_PATH = "mes_document" + File.separator + "tables" + File.separator;

	public final static String FILE_DELETE_TABLE_PATH = "mes_document" + File.separator + "delete_tables" + File.separator;

	public final static String SOP_FILE_PATH = "mes_document" + File.separator + "sop_files" + File.separator;

	public final static String SOP_PICTURE_PATH = "mes_document" + File.separator + "sop_pictures" + File.separator;

	public static final Integer WAIT_CONFIRM_ORDERSTATUS = 1;

	public static final Integer UNSCHEDULED_ORDERSTATUS = 2;

	public static final Integer SCHEDULED_ORDERSTATUS = 3;

	public static final Integer ONGOING_ORDERSTATUS = 4;

	public static final Integer WAIT_NOTICE__ORDERSTATUS = 5;

	public static final Integer COMPLETED_ORDERSTATUS = 6;

	public static final Integer ABNORMAL_END_ORDERSTATUS = 7;

	public static final Integer SCHEDULED_PLANSTATUS = 1;

	public static final Integer WORKING_PLANSTATUS = 2;

	public static final Integer WAIT_NOTIFICATION_PLANSTATUS = 3;

	public static final Integer COMPLETED_PLANSTATUS = 4;

	public static final Integer ABNORMAL_END_PLANSTATUS = 5;

	public static final Integer DEFAULT_PAGE_NUM = 1;

	public static final Integer DEFAULT_PAGE_SIZE = 2147483647;

	public static final Integer ASSEMBLING_PROCESS_GROUP = 1;

	public static final Integer TESTING_PROCESS_GROUP = 2;

	public static final Integer PACKING_PROCESS_GROUP = 3;

	public static final Integer PATCH_PROCESS_GROUP = 4;

	public static final Integer SMT_PROCESS_GROUP = 5;

	public static final Integer AUTOTEST_LINEID = 0;

	public static final Integer COUPLETEST_LINEID = 1;

	public static final Integer CARTONTEST_LINEID = 2;

	public static final Integer PATCH_LINEID = 3;

	public static final Integer SMT_LINEID = 4;

	public static final String SMT_TEST = "SMT功能测试位";

	public static final String FUNCTION_TEST = "组装测试位";

	public static final String AGED_TEST = "老化后测试";

	public static final String COUPLING_TEST = "耦合测试";

	public static final String CARTON_TEST = "卡通箱测试";

	public static final Integer INTEGER_ZERO = 0;

	public static final Integer MAX_LINECHANGETIME_LENGTH = 8;

	public static final Integer HOUR_TO_SECOND = 3600;

	public static String SOP_PICTURE_URL;

	public static final String OPERATOR_CONFIRMATION_SUCCEED = "操作员确认成功";

	public static final String OPERATOR_CONFIRMATION_TIME_OUT = "操作员确认超时";

	public static final String QC_CONFIRMATION = "品质确认";

	public static final Integer MINUTE_OF_TWENTY_ONE_DAYS = 30240;

	public static final String CLIENT_LOGIN = "登录";

	public static final String CLIENT_LOGOUT = "退出登录";

	public static final Integer SCHEDULING_ABOUT = 1;

	public static final String DEFAULT_CUSTOMERNUMBER = "JMWL";

	public static final String DEFAULT_CUSTOMERNAME = "几米物联";

	public static final Integer TO_BE_CONFIRMED_MODELCAPACITY_STATUS = 1;

	public static final Integer FAIL_MODELCAPACITY_STATUS = 2;

	public static final Integer PASS_MODELCAPACITY_STATUS = 3;


	public static void setSopPictureUrl(String value) {
		SOP_PICTURE_URL = value;
	}
}
