package com.jimi.mes_server.entity;

import java.io.File;

public class Constant {

	public static final Integer SUPER_ADMIN_USERTYPE = 2;

	public static final Integer ADMIN_USERTYPE = 1;

	public static final String SUPER_ADMIN_DELETEPERMISSION = "1,1,1,1,1,1,1,1,1,1";

	public static final String ORDINARY_DELETEPERMISSION = "0,0,0,0,0,0,0,0,0,0";

	public static final String EXIST_DELETEPERMISSION = "1";

	public final static String FILE_BACKUP_PATH = File.separator + "home" + File.separator + "mes_document" + File.separator + "backups" + File.separator;

	public final static String FILE_TABLE_PATH = File.separator + "home" + File.separator + "mes_document" + File.separator + "tables" + File.separator;

	public static final Integer UNSCHEDULED_ORDERSTATUS = 0;

	public static final Integer SCHEDULED_ORDERSTATUS = 1;

	public static final Integer COMPLETED_ORDERSTATUS = 2;

	public static final Integer DELETED_ORDERSTATUS = 3;

	public static final Integer INFORMATION_FILETYPE = 0;

	public static final Integer BOM_FILETYPE = 1;

	public static final Integer SOP_FILETYPE = 2;

	public static final Integer SCHEDULED_PLANSTATUS = 0;

	public static final Integer WORKING_PLANSTATUS = 1;

	public static final Integer COMPLETED_PLANSTATUS = 2;

	public static final Integer WAIT_NOTIFICATION_PLANSTATUS = 3;
}
