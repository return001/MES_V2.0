let url;
if (process.env.NODE_ENV === 'production') {
  url = window.g.API_URL
} else {
  url = window.g.LOCAL_URL
}
const mockUrl = '';

/*用户*/
export const validateUrl = url + '/user/validate';
export const loginUrl = url + '/user/login';
export const logoutUrl = url + '/user/logout';
export const userUpdateUrl = url + '/user/update';
export const userAddUrl = url + '/user/add';
export const userQueryUrl = url + '/user/select';
export const getUserTypeUrl = url + '/user/getUserType';
export const getUserUrl = url + '/user/getUserIdAndName';
export const editUserPasswordUrl = url + '/user/updatePassword';
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


/*排产计划*/

/*文件类型*/
export const planFileTypeSelectUrl = url + '/production/selectFileType'; //查询文件类型
export const planFileTypeDeleteUrl = url + '/production/deleteFileType'; //删除文件类型
export const planFileTypeEditUrl = url + '/production/editFileType'; //编辑文件类型
export const planFileTypeAddUrl = url + '/production/addFileType'; //添加文件类型
/*订单管理*/
export const planOrderSelectUrl = url + '/production/selectOrder'; //查询
export const planOrderDeleteUrl = url + '/production/abnormalEndOrder'; //异常结单
export const planOrderEditUrl = url + '/production/editOrder'; //修改
export const planOrderAddUrl = url + '/production/addOrder'; //新增
export const planOrderImportUrl = url + '/production/importOrder'; //导入订单
export const planOrderDetailsSelectUrl = url + '/production/selectOrderDetail'; //查询xx表
export const planOrderTableImportUrl = url + '/production/importOrderTable'; //导入xx表
export const planOrderTableDownloadUrl = url + '/production/downloadOrderTable'; //下载xx表
export const planOrderTableDeleteUrl = url + '/production/deleteOrderTable'; //删除xx表
export const planOrderTableEditUrl = url + '/production/editOrderTable'; //修改订单文件状态
/*工序组管理*/
export const planProcessGroupSelectUrl = url + '/production/selectProcessGroup';//查询
export const planProcessGroupDeleteUrl = url + '/production/deleteProcessGroup';//删除
export const planProcessGroupEditUrl = url + '/production/editProcessGroup';//修改
export const planProcessGroupAddUrl = url + '/production/addProcessGroup';//新增
export const planProcessGroupGetUrl = url + '/production/getProcessGroup'; //获取工序组信息 用于页面加载时异步读取

/*工序管理*/
export const planProcessSelectUrl = url + '/production/selectProcess'; //查询
export const planProcessDeleteUrl = url + '/production/deleteProcess'; //删除
export const planProcessEditUrl = url + '/production/editProcess'; //修改
export const planProcessAddUrl = url + '/production/addProcess'; //新增
export const planProcessGetUrl = url + '/production/getProcess'; //获取 (select)

/*产线管理*/
export const planLineSelectUrl = url + '/production/selectLine'; //查询
export const planLineDeleteUrl = url + '/production/deleteLine'; //删除
export const planLineEditUrl = url + '/production/editLine'; //修改
export const planLineAddUrl = url + '/production/addLine'; //新增
export const planLineGetUrl = url + '/production/getLine'; //获取线号信息，加载页面时异步加载
/*产线PC管理*/
export const planLinePCSelectUrl = url + '/production/selectComputer'; //查询
export const planLinePCDeleteUrl = url + '/production/deleteComputer'; //删除
export const planLinePCEditUrl = url + '/production/editComputer'; //修改
export const planLinePCAddUrl = url + '/production/addComputer'; //新增
/*产能管理*/
export const planCapacitySelectUrl = url + '/production/selectCapacity';//查询
export const planCapacityDeleteUrl = url + '/production/deleteCapacity';//删除
export const planCapacityEditUrl = url + '/production/editCapacity';//修改
export const planCapacityAddUrl = url + '/production/addCapacity';//新增

/*计划管理*/
export const planDetailsSelectUrl = url + '/production/selectPlan'; //查询
export const planDetailsDeleteUrl = url + '/production/deletePlan'; //删除
export const planDetailsEditUrl = url + '/production/editPlan'; //修改
export const planDetailsAddUrl = url + '/production/addPlan'; //新增
export const planDetailsReAddUrl = url + '/production/reSchedulingPlan'; //新增重排
export const planDetailsStatusEditUrl = url + '/production/editPlanStatus'; //修改状态
export const planDetailsExportUrl = url + '/production/exportPlan'; //导出计划
export const planDetailsGanttGetUrl = url + '/production/getPlanGantt'; //获取甘特图数据
export const planDetailsUnscheduledSelectUrl = url + '/production/selectUnscheduledPlan'; //获取未排产订单
export const planExtraDetailsSelectUrl = url + '/production/selectPlanDetail'; //查询计划详情
export const planDetailsReworkSelectUrl = url + '/production/selectReworkPlan'; //获取未排产订单(返工
export const planProducedQuantitySelectUrl = url + '/production/selectPlanProducedQuantity'; //获取订单已生产数量
export const planCheckCompleteTimeSelectUrl = url + '/production/checkCompleteTime'; //检测是否可以完成

/*排产日志*/
export const planLogsSelectUrl = url + '/production/selectProductionLog'; //查询

/*看板*/
export const dashboardSelectUrl = url + '/report/selectDashboardTable';
export const dashboardErrMsgSelectUrl = url + '/report/selectErrorMsg';
export const dashboardSelectUrlCS = url + '/report/selectDashboardTableForCustomer';
export const dashboardErrMsgSelectUrlCS = url + '/report/selectErrorMsgForCustomer';

/*基础配置*/
/*厂区*/
export const eSopFactorySelectUrl = url + '/sop/selectFactory';
export const eSopFactoryDeleteUrl = url + '/sop/deleteFactory';
export const eSopFactoryEditUrl = url + '/sop/editFactory';
export const eSopFactoryAddUrl = url + '/sop/addFactory';

/*车间*/
export const eSopWorkshopSelectUrl = url + '/sop/selectWorkshop';
export const eSopWorkshopEditUrl = url + '/sop/editWorkshop';
export const eSopWorkshopDeleteUrl = url + '/sop/deleteWorkshop';
export const eSopWorkshopAddUrl = url + '/sop/addWorkshop';

/*站点*/
export const eSopSiteSelectUrl = url + '/sop/selectSite';
export const eSopSiteEditUrl = url + '/sop/editSite';
export const eSopSiteDeleteUrl = url + '/sop/deleteSite';
export const eSopSiteAddUrl = url + '/sop/addSite';
export const eSopSiteStatusEditUrl = url + '/sop/editSiteState';

/*客户*/
export const eSopCustomerSelectUrl = url + '/sop/selectCustomer';
export const eSopCustomerEditUrl = url + '/sop/editCustomer';
export const eSopCustomerDeleteUrl = url + '/sop/deleteCustomer';
export const eSopCustomerAddUrl = url + '/sop/addCustomer';

/*系列机型*/
export const eSopModelSelectUrl = url + '/sop/selectSeriesModel';
export const eSopModelEditUrl = url + '/sop/editSeriesModel';
export const eSopModelDeleteUrl = url + '/sop/deleteSeriesModel';
export const eSopModelAddUrl = url + '/sop/addSeriesModel';

/*产品型号*/
export const eSopProductSelectUrl = url + '/sop/selectProductModel';
export const eSopProductEditUrl = url + '/sop/editProductModel';
export const eSopProductDeleteUrl = url + '/sop/deleteProductModel';
export const eSopProductAddUrl = url + '/sop/addProductModel';

/*E-SOP*/
/*文件管理*/
export const eSopFileSelectUrl = url + '/sop/selectFiles';
export const eSopFileEditUrl = url + '/sop/editFileState';
export const eSopFileDeleteUrl = url + '/sop/deleteFile';
export const eSopFileAddUrl = url + '/sop/addFile';
export const eSopFileUploadUrl = url + '/sop/importFiles';
export const eSopFileDispatchUrl = url + '/sop/dispatchFile';
export const eSopFilePreviewUrl = url + '/sop/previewDispatchingFile';
export const eSopSitePreviewUrl = url + '/sop/previewSite';
export const eSopFileRecycleUrl = url + '/sop/recycleFile';
export const eSopFilePictureSelectUrl = url + '/sop/selectFilePictures';
export const eSopFileHistorySelectUrl = url + '/sop/selectFileHistory';
export const eSopFileHistoryDetailsSelectUrl = url + '/sop/selectFileHistoryDetail';
export const eSopFileDownloadUrl = url + '/sop/downloadFile';

/*通知管理*/
export const eSopNoticeSelectUrl = url + '/sop/selectNotice';
export const eSopNoticeEditUrl = url + '/sop/editNotice';
export const eSopNoticeDeleteUrl = url + '/sop/deleteNotice';
export const eSopNoticeAddUrl = url + '/sop/addNotice';

/*日志*/
export const eSopActionLogSelectUrl = url + '/sop/selectActionLog';
export const eSopConfirmLogSelectUrl = url + '/sop/selectConfirmLog';
export const eSopNoticeLogSelectUrl = url + '/sop/selectNoticeHistory';
export const eSopCountLogSelectUrl = url + '/sop/selectCountLog';

/*客户端*/
export const eSopLoginLogSelectUrl = url + '/sop/selectLoginLog';

/*人员分配*/
export const eSopStationStatusSelectUrl = url + '/sop/selectPost';
export const eSopAssignableStaffSelectUrl = url + '/sop/selectAssignableUser';
export const eSopAssignStaffUrl = url + '/sop/assignUser';

/*获取排班时间*/
export const getWorkingScheduleUrl = url + '/production/getWorkingSchedule';