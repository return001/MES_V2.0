let url;
if (process.env.NODE_ENV === 'production') {
  url = window.g.API_URL
} else {
  url = window.g.LOCAL_URL
}

/*用户*/
export const validateUrl = url + '/user/validate';
export const loginUrl = url + '/user/login';
export const logoutUrl = url + '/user/logout';
export const userUpdateUrl = url + '/user/update';
export const userAddUrl = url + '/user/add';
export const userQueryUrl = url + '/user/select';
export const getUserTypeUrl = url + '/user/getUserType';

export const redTeaUrl = url + '/redTea/findCId';

export const tableDownloadUrl = url + '/report/download';
export const tablePrintDownloadUrl = url + '/report/downloadGpsManuPrintParam';
export const tableSimDownloadUrl = url + '/report/downloadGpsManuSimDataParam';
export const tableCartonDownloadUrl = url + '/report/downloadGpsCartonBox';


export const tableDeleteUrl = url + '/report/delete';
export const tablePrintDeleteUrl = url + '/report/deleteGpsManuPrintParam';
export const tableSimDeleteUrl = url + '/report/deleteGpsManuSimDataParam';

export const tableDeleteByIdsUrl = url + '/report/deleteByIds';

export const tableHistoryQueryUrl = url + '/history/select';
export const tableHistoryDownloadUrl = url + '/history/download';


export const relationUpdateUrl = url + '/report/updateRelativeSheet';


/*Mac返工*/
export const macReworkQueryUrl = url + '/rework/getMacReworkInfo';
export const setMacRework = url + '/rework/macRework';

/*查询已使用IMEI*/
export const imeiQueryUrl = url + '/report/selectUnusedIMEI';
export const imeiDownloadUrl = url + '/report/downloadUnusedIMEI';
export const imeiZhidanQueryUrl = url + '/report/selectZhiDanInfo';


/*根据IMEI清空指定项目*/
export const cleanupRelUrl = url + '/report/cleanupInRel';


/*多表*/
export const multiTableQueryUrl = url + '/report/multiTableQuery';
export const multiTableDeleteUrl = url + '/report/multiTableDelete';
export const multiTableDownloadUrl = url + '/report/downloadMultiTable';
