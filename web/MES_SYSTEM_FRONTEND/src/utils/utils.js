export const getTime = function () {
  let date = new Date();
  let yyyy = date.getFullYear();
  let MM = (date.getMonth() + 1).toString().length > 1 ? date.getMonth() + 1 : '0' + (date.getMonth() + 1);
  let dd = date.getDate().toString().length > 1 ? date.getDate() : '0' + date.getDate();
  let hh = date.getHours().toString().length > 1 ? date.getHours() : '0' + date.getHours();
  let mm = date.getMinutes().toString().length > 1 ? date.getMinutes() : '0' + date.getMinutes();
  let ss = date.getSeconds().toString().length > 1 ? date.getSeconds() : '0' + date.getSeconds();
  return yyyy + '-' + MM + '-' + dd + ' ' + hh + ':' + mm + ':' + ss;
};

import store from '../store'

export const checkDelPermission = function (queryType) {
  let tableList = ['DataRelativeSheet', 'DataRelativeUnique', 'DataRelativeUpdate', 'Gps_AutoTest_Result', 'Gps_AutoTest_Result2', 'Gps_AutoTest_Result3', 'Gps_CartonBoxTwenty_Result', 'Gps_CoupleTest_Result', 'Gps_ManuPrintParam', 'Gps_TestResult', 'NetMarkIMEI', 'Gps_ManuSimDataParam'];

  let index = tableList.indexOf(queryType);
  return store.state.delPermission[index] !== "0"
};

export const checkAccessPermission = function (path) {
  /*table_old, table, order, test, func, users, redtea*/
  let permissionListArray = permissionList();

  /*行政管理仅有人员管理权限
  * 超管拥有所有权限
  * 工程管理员无人员管理权限*/

  return permissionListArray.indexOf(path) >= 0
};

export const permissionList = function () {
  let list;
  if (store.state.userType === 'administration') {
    list = ['users'];
  } else if (store.state.userType === 'SuperAdmin') {
    list = ['table', 'order', 'test', 'func', 'plan', 'users', 'redtea', 'esop']
    // permissionList = ['order', 'users', 'test']
  } else if (store.state.userType === 'schedulingSZPC') {
    list = ['plan']
  } else if (store.state.userType === 'schedulingJMPMC') {
    list = [ 'plan']
  } else {
    list = ['table', 'order', 'test', 'func', 'plan', 'redtea']
    // permissionList = ['order', 'test']
  }
  return list
};
