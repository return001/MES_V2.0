package com.jimi.mes_server.entity;

public class SopSQL {

	public static final String SELECT_FACTORY_ID_BY_COLUMN = "SELECT id FROM sop_factory WHERE factory_alias = ? UNION ALL SELECT id FROM sop_factory WHERE abbreviation = ? UNION ALL SELECT id FROM sop_factory WHERE full_name = ?";

	public static final String SELECT_FACTORY = "SELECT id,factory_alias as factoryAlias,abbreviation,full_name as fullName from sop_factory ";

	public static final String SELECT_WORKSHOP_BY_FACTORY = "SELECT * FROM sop_workshop WHERE factory_id = ?";

	public static final String SELECT_CUSTOMER_BY_FACTORY = "SELECT * FROM sop_customer WHERE factory_id = ?";

	public static final String SELECT_WORKSHOP_BY_COLUMN = "SELECT id FROM sop_workshop WHERE workshop_name = ? UNION ALL SELECT id FROM sop_workshop WHERE workshop_number = ?";

	public static final String SELECT_LINE_BY_WORKSHOP = "SELECT * FROM line WHERE workshop_id = ?";

	public static final String SELECT_WORKSHOP_AND_FACTORY_ABBREVIATION = "SELECT w.id,w.workshop_name as workshopName,w.workshop_number as workshopNumber,f.abbreviation,f.id as factoryId FROM sop_workshop w, sop_factory f WHERE w.factory_id = f.id ";

	public static final String SELECT_SITE_BY_LINE = "SELECT * FROM sop_site WHERE line_id = ?";

	public static final String SELECT_SITE_BY_SITENUMBER = "SELECT * FROM sop_site WHERE site_number = ? and line_id = ?";

	public static final String SELECT_SITE_JOIN_LINE = "SELECT s.id,s.site_name as siteName,s.site_number as siteNumber,s.mac,s.play_times as playTimes,s.process_order as processOrder,s.switch_interval as switchInterval,s.second_mac as secondMac,s.state,l.line_name as lineName,l.line_no as 'lineNo',l.id as lineId FROM sop_site s, line l WHERE s.line_id = l.id ";

	public static final String SELECT_CUSTOMER_ID_BY_COLUMN = "SELECT id FROM sop_customer WHERE customer_name = ? and factory_id = ? UNION ALL SELECT id FROM sop_customer WHERE customer_number = ? and factory_id = ?";

	public static final String SELECT_CUSTOMER_JOIN_FACTORY = "SELECT c.id,c.customer_name as customerName,c.customer_number as customerNumber,f.abbreviation,f.id as factoryId FROM sop_customer c, sop_factory f WHERE c.factory_id = f.id ";

	public static final String SELECT_SERIESMODEL_BY_SERIESMODELNAME = "SELECT * FROM sop_series_model WHERE series_model_name = ?";

	public static final String SELECT_PRODUCTMODEL_BY_SERIESMODEL = "SELECT * FROM sop_product_model WHERE series_model_id = ?";

	public static final String SELECT_SERIESMODEL = "SELECT id,series_model_name as seriesModelName FROM sop_series_model ";

	public static final String SELECT_PRODUCTMODEL_BY_NAME_ID = "SELECT * FROM sop_product_model WHERE product_model_name = ? AND series_model_id = ?";

	public static final String SELECT_PRODUCTMODEL_JOIN_SERIESMODEL = "SELECT p.id,p.product_model_name as productModelName,s.series_model_name as seriesModelName,s.id AS seriesModelId FROM sop_product_model p, sop_series_model s WHERE p.series_model_id = s.id ";

	public static final String SELECT_SOPFILE = "SELECT id,file_name as fileName,file_number as fileNumber,version,customer,series_model as seriesModel,product_model as productModel,reviewer,review_time as reviewTime,state FROM sop_file ";

	public static final String SELECT_SOPNOTICE = "SELECT id,title,content,start_time as startTime,end_time as endTime FROM sop_notice ";

	public static final String SELECT_ACTIONLOG = "SELECT id,[time],address,url,result_code as resultCode,uid,consume_time as consumeTime,[action],parameters FROM action_log ";

	public static final String SELECT_SITE_BY_MAC = "SELECT * FROM sop_site WHERE mac = ?";

	public static final String SELECT_SITE_BY_SENCONDMAC = "SELECT * FROM sop_site WHERE second_mac = ?";

	public static final String SELECT_SOPPICTURE_BY_FILEID = "SELECT id,sop_file_id as sopFileId,picture_name as pictureName,picture_number as pictureNumber,picture_path as picturePath FROM sop_file_picture WHERE sop_file_id = ?";

	public static final String SELECT_FACEINFORMATION_BY_USERNAME = "SELECT * FROM sop_face_information WHERE user_name = ?";

	public static final String SELECT_FACEINFORMATION = "SELECT user_name as userName,feature FROM sop_face_information";

	public static final String SELECT_LOGIN_LOG = "SELECT id,user_name as userName,site_number as siteNumber,[time],type FROM sop_login_log";

	public static final String SELECT_SOPFILEPICTURE_BY_FILEID = "SELECT * FROM sop_file_picture WHERE sop_file_id = ?";

	public static final String DELETE_SOPFILEPICTURE_BY_FILEID = "DELETE FROM sop_file_picture WHERE sop_file_id = ?";

	public static final String SELECT_SOPSITEDISPLAY = "SELECT * FROM sop_site_display";

	public static final String SELECT_SITE_JOIN_LINE_WORKSHOP_FACTORY = "SELECT s.site_name,s.site_number,l.line_name,w.workshop_name,f.abbreviation FROM sop_site s,line l,sop_workshop w,sop_factory f WHERE s.line_id = l.id AND w.factory_id = f.id AND l.workshop_id = w.id AND s.id = ?";

	public static final String SELECT_FILEHISTORY_JOIN_FILE = "SELECT h.id,h.file_id as fileId,h.push_person as pushPerson,h.push_time as pushTime,f.file_number as fileNumber,f.file_name as fileName,f.version,f.product_model as productModel,f.series_model as seriesModel FROM sop_file_history h,sop_file f WHERE h.file_id = f.id AND h.file_id = ? ";

	public static final String SELECT_FILEHISTORY_NUMBER = "SELECT count(*) FROM sop_file_history WHERE file_id = ?";

	public static final String SELECT_FILEPICTUREHISTORY_BY_FILEHISTORYID = "SELECT * FROM sop_picture_history WHERE file_history_id = ?";

	public static final String SELECT_SITEDISPLAY_BY_SITE = "SELECT * FROM sop_site_display WHERE site_id = ?";

	public static final String SELECT_NOTICEHISTORY = "SELECT id,line,workshop,factory,push_time as pushTime,push_person as pushPerson,title,content FROM sop_notice_history";

	public static final String SELECT_CONFIRM_LOG = "SELECT id,user_name as userName,[time],site_number as siteNumber,line_name as lineName,content,type FROM sop_confirm_log";

	public static final String SELECT_COUNT_LOG = "SELECT id,number,line_name as lineName,site_number as siteNumber,user_name as userName,start_time as startTime,end_time as endTime FROM sop_count_log";

	public static final String SELECT_SITE_PROCESS_LINE = "SELECT s.site_name AS siteName, s.id AS siteId, p.id AS processId, p.process_name AS processName FROM sop_site s, process p WHERE s.process_order = p.process_no AND s.line_id = ?";

	public static final String SELECT_POSTINFO = "SELECT s.site_name AS siteName, s.id AS siteId, p.id AS processId, p.process_name AS processName, u.UserDes AS userName, u.Id AS userId,a.id as postId FROM sop_position_assignment a, sop_site s, process p, LUserAccount u WHERE a.process_id = p.id AND a.site_id = s.id AND a.line_id = s.line_id AND a.user_id = u.Id AND s.line_id = ?";

	public static final String SELECT_ASSIGNABLE_USER_PREFIX = "SELECT * FROM (SELECT * FROM LUserAccount WHERE MainProcess = ? UNION SELECT * FROM LUserAccount WHERE CHARINDEX(?, OtherProcess)>0 ";

	public static final String SELECT_ASSIGNABLE_USER_SUFFIX = " UNION SELECT * FROM LUserAccount ) a ";

	public static final String SELECT_POSTINFO_BY_SITE = "SELECT * FROM sop_position_assignment WHERE site_id = ?";
}
