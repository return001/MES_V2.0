import {alertDanger, alertWarning, alertSuccess, alertInfo} from "./modal";

export const errHandler = function (code) {
  switch (code) {
    case 400:
      alertInfo("请求逻辑错误，请联系管理员");
      break;
    case 401:
      alertWarning("权限不足");
      break;
    case 412:
      alertWarning("操作错误");
      break;
    case 500:
      alertDanger("服务器异常，请联系管理员");
      break;
    case 501:
      alertDanger("未知错误，请联系管理员");
      break;
    default:
      break;
  }
};
