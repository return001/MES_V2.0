export const getTime = function () {
  let date = new Date();
  let yyyy =  date.getFullYear();
  let MM = (date.getMonth() + 1).toString().length > 1 ? date.getMonth() + 1 : '0' + (date.getMonth() + 1);
  let dd = date.getDate().toString().length > 1 ? date.getDate() : '0' + date.getDate();
  let hh = date.getHours().toString().length > 1 ? date.getHours() : '0' + date.getHours();
  let mm = date.getMinutes().toString().length > 1 ? date.getMinutes() : '0' + date.getMinutes();
  let ss = date.getSeconds().toString().length > 1 ? date.getSeconds() : '0' + date.getSeconds();
  return yyyy  + '-' + MM + '-' +dd + ' ' + hh + ':' + mm  + ':' + ss;
};

import store from '../store'

export const checkDelPermission = function (queryType) {
  let tableList = ['DataRelativeSheet','DataRelativeUnique','Gps_AutoTest_Result','Gps_AutoTest_Result3','Gps_CartonBoxTwenty_Result','Gps_CoupleTest_Result','Gps_ManuPrintParam','Gps_TestResult','NetMarkIMEI','Gps_ManuSimDataParam'];

  let index = tableList.indexOf(queryType);
  return store.state.delPermission[index] !== "0"
};

export const checkAccessPermission = function (path) {
  /*table, order, test, func, users, redtea*/
  let permissionList;

  /*行政管理仅有人员管理权限
  * 超管拥有所有权限
  * 工程管理员无人员管理权限*/
  if (store.state.userType === 'administration') {
    permissionList =  ['users'];
  } else if (store.state.userType === 'SuperAdmin') {
    permissionList = ['table', 'order', 'test', 'func', 'users', 'redtea']
  } else {
    permissionList = ['table', 'order', 'test', 'func', 'redtea']
  }
  return permissionList.indexOf(path) >= 0
};
