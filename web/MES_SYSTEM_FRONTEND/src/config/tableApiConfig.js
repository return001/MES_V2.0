/*报表统一配置页面*/
let url;
if (process.env.NODE_ENV === 'production') {
  url = window.g.API_URL
} else {
  url = window.g.LOCAL_URL
}
export const tableSelectUrl = url + "/mes_server/report/select";
export const getRequestUrl = (name) => {
  switch (name) {
    case 'DataRelativeSheet':
      return url + '/report/selectDataRelativeSheet';
    case 'Gps_ManuSimDataParam':
      return url + '/report/selectGpsManuSimDataParam';
    case 'Gps_ManuPrintParam':
      return url + '/report/selectGpsManuPrintParam';
    case 'Gps_CartonBoxTwenty_Result':
      return url + '/report/selectGpsCartonBox';
    default:
      return url + '/report/select'
  }
};


//export const routerUrl = "http://10.10.11.109:8080/mes_server/report/select";
export const setRouterConfig = (name, params) => {
  switch (name) {
    case 'DataRelativeSheet':
      if (params.isReferred) {
        return {
          data: ROUTER_CONFIG.DataRelativeSheet_True
        };
      } else {
        return {
          data: ROUTER_CONFIG.DataRelativeSheet_False
        };
      }

    case 'DataRelativeUnique':
      return {
        data: ROUTER_CONFIG.DataRelativeUnique
      };
    case 'DataRelativeUpdate':
      return {
        data: ROUTER_CONFIG.DataRelativeUpdate
      };
    case 'Gps_CoupleTest_Result':
    case 'Gps_AutoTest_Result':
    case 'Gps_AutoTest_Result3':
    case 'Gps_AutoTest_Result2':
      return {
        data: ROUTER_CONFIG.Gps_AutoTest_Result
      };
    case 'Gps_CartonBoxTwenty_Result':
      // return {
      //   data: ROUTER_CONFIG.Gps_CartonBoxTwenty_Result
      // };

      if (params.isReferred) {
        return {
          data: ROUTER_CONFIG.Gps_CartonBoxTwenty_Result_True
        };
      } else {
        return {
          data: ROUTER_CONFIG.Gps_CartonBoxTwenty_Result_False
        };
      }

    case 'Gps_ManuCpParam':
      return {
        data: ROUTER_CONFIG.Gps_ManuCpParam
      };

    case 'Gps_ManuPrintParam':
      return {
        data: ROUTER_CONFIG.Gps_ManuPrintParam
      };
    case 'NetMarkIMEI':
      return {
        data: ROUTER_CONFIG.NetMarkIMEI
      };
    case 'Gps_ManuSimDataParam':
      return {
        data: ROUTER_CONFIG.Gps_ManuSimDataParam
      };
    case 'Gps_TestResult':
      return {
        data: ROUTER_CONFIG.Gps_TestResult
      };
    case 'LTestLogMessage':
      return {
        data: ROUTER_CONFIG.LTestLogMessage
      };
    case 'GPSOQC_Result':
      return {
        data: ROUTER_CONFIG.GPSOQC_Result
      };
  }
};

const ROUTER_CONFIG = {
  Gps_AutoTest_Result: {
    primaryKey: 'Id',
    type: 'int',
    queryOptions: [
      {
        id: 'SN',
        name: 'SN号',
        model: '',
        type: 'text'
      },
      {
        id: 'IMEI',
        name: 'IMEI',
        model: '',
        type: 'text'
      },
      {
        id: 'ZhiDan',
        name: '制单号',
        model: '',
        type: 'text'
      },
      {
        id: 'SoftModel',
        name: '机型',
        model: '',
        type: 'text'
      },
      {
        id: 'Version',
        name: '软件版本',
        model: '',
        type: 'text'
      },
      {
        id: 'TestTime',
        name: '测试时间',
        timeRange: '',
        type: 'date'
      }
    ],
    dataColumns: [
      {field: 'Id', title: 'ID', colStyle: {'width': '70px'},},
      {field: 'SN', title: 'SN号', colStyle: {'width': '260px'}},
      {field: 'IMEI', title: 'IMEI号', colStyle: {'width': '135px'}},
      {field: 'ZhiDan', title: '制单号', colStyle: {'width': '135px'}},
      {field: 'SoftModel', title: '机型', colStyle: {'width': '120px'}},
      {field: 'Version', title: '软件版本', colStyle: {'width': '200px'}},
      {
        field: 'Result', title: '结果', colStyle: {'width': '50px'}, formatter(row, column, cellValue, index) {
          switch (cellValue) {
            case 0:
              return '失败';
            case 1:
              return '成功';
          }
        }
      },
      {field: 'TesterId', title: '测试员', colStyle: {'width': '70px'}},
      {field: 'Computer', title: '地址', colStyle: {'width': '160px'}},
      {field: 'TestSetting', title: '测试配置', colStyle: {'width': '400px'}},
      {field: 'TestTime', title: '测试时间', colStyle: {'width': '175px'}},
      {field: 'Remark', title: '备注', colStyle: {'width': '50px'}}
    ]
  },

  Gps_CartonBoxTwenty_Result: {
    primaryKey: 'Id',
    type: 'int',
    queryOptions: [
      {
        id: 'BoxNo',
        name: '箱号',
        model: '',
        type: 'text'
      },
      // {
      //   id: 'IMEI',
      //   name: 'IMEI号',
      //   model: '',
      //   type: 'text'
      // },
      {
        id: 'IMEI',
        name: 'IMEI号',
        modelFrom: '',
        modelTo: '',
        type: 'range'
      },
      {
        id: 'ZhiDan',
        name: '制单号',
        model: '',
        type: 'text'
      },
      {
        id: 'SoftModel',
        name: '机型',
        model: '',
        type: 'text'
      },
      {
        id: 'Version',
        name: '软件版本',
        model: '',
        type: 'text'
      },
      {
        id: 'TestTime',
        name: '测试时间',
        timeRange: '',
        type: 'date'
      }
    ],
    dataColumns: [
      {field: 'Id', title: 'ID', colStyle: {'width': '70px'}},
      {field: 'BoxNo', title: '箱号', colStyle: {'width': '90px'}},
      {field: 'IMEI', title: 'IMEI号', colStyle: {'width': '120px'}},
      {field: 'ZhiDan', title: '制单号', colStyle: {'width': '120px'}},
      {field: 'SoftModel', title: '机型', colStyle: {'width': '60px'}},
      {field: 'Version', title: '软件版本', colStyle: {'width': '180px'}},
      {field: 'ProductCode', title: '产品编号', colStyle: {'width': '180px'}},
      {field: 'Color', title: '颜色', colStyle: {'width': '60px'}},
      {field: 'Qty', title: '数量', colStyle: {'width': '60px'}},
      {field: 'Weight', title: '重量', colStyle: {'width': '60px'}},
      {field: 'Date', title: '日期', colStyle: {'width': '90px'}},
      {field: 'TACInfo', title: '前缀', colStyle: {'width': '60px'}},
      {field: 'CompanyName', title: '公司名称', colStyle: {'width': '60px'}},
      {field: 'TesterId', title: '测试员', colStyle: {'width': '60px'}},
      {field: 'TestTime', title: '测试时间', colStyle: {'width': '120px'}},
      {field: 'Remark1', title: '备注1', colStyle: {'width': '90px'}},
      {field: 'Remark2', title: '备注2', colStyle: {'width': '120px'}},
      {field: 'Remark3', title: '备注3', colStyle: {'width': '90px'}},
      {field: 'Remark4', title: '备注4', colStyle: {'width': '400px'}},
      {field: 'Remark5', title: '备注5', colStyle: {'width': '60px'}},
      {field: 'Computer', title: '计算机地址', colStyle: {'width': '120px'}},
    ]
  },
  Gps_CartonBoxTwenty_Result_True: {
    primaryKey: 'Id',
    type: 'int',
    queryOptions: [
      {
        id: 'Gps_CartonBoxTwenty_Result.BoxNo',
        name: '箱号',
        model: '',
        type: 'text'
      },
      // {
      //   id: 'IMEI',
      //   name: 'IMEI号',
      //   model: '',
      //   type: 'text'
      // },
      {
        id: 'Gps_CartonBoxTwenty_Result.IMEI',
        name: 'IMEI号',
        modelFrom: '',
        modelTo: '',
        type: 'range'
      },
      {
        id: 'Gps_CartonBoxTwenty_Result.ZhiDan',
        name: '制单号',
        model: '',
        type: 'text'
      },
      {
        id: 'Gps_CartonBoxTwenty_Result.SoftModel',
        name: '机型',
        model: '',
        type: 'text'
      },
      {
        id: 'Gps_CartonBoxTwenty_Result.Version',
        name: '软件版本',
        model: '',
        type: 'text'
      },
      {
        id: 'Gps_CartonBoxTwenty_Result.TestTime',
        name: '测试时间',
        timeRange: '',
        type: 'date'
      }
    ],
    dataColumns: [
      {field: 'GpsCartonBoxTwentyResult_Id', title: 'ID', colStyle: {'width': '70px'}},
      {field: 'GpsCartonBoxTwentyResult_BoxNo', title: '箱号', colStyle: {'width': '90px'}},
      {field: 'GpsCartonBoxTwentyResult_IMEI', title: 'IMEI号', colStyle: {'width': '120px'}},
      {field: 'GpsCartonBoxTwentyResult_ZhiDan', title: '制单号', colStyle: {'width': '120px'}},
      {field: 'GpsCartonBoxTwentyResult_SoftModel', title: '机型', colStyle: {'width': '60px'}},
      {field: 'GpsCartonBoxTwentyResult_Version', title: '软件版本', colStyle: {'width': '180px'}},
      {field: 'GpsCartonBoxTwentyResult_ProductCode', title: '产品编号', colStyle: {'width': '180px'}},
      {field: 'GpsCartonBoxTwentyResult_Color', title: '颜色', colStyle: {'width': '60px'}},
      {field: 'GpsCartonBoxTwentyResult_Qty', title: '数量', colStyle: {'width': '60px'}},
      {field: 'GpsCartonBoxTwentyResult_Weight', title: '重量', colStyle: {'width': '60px'}},
      {field: 'GpsCartonBoxTwentyResult_Date', title: '日期', colStyle: {'width': '90px'}},
      {field: 'GpsCartonBoxTwentyResult_TACInfo', title: '前缀', colStyle: {'width': '60px'}},
      {field: 'GpsCartonBoxTwentyResult_CompanyName', title: '公司名称', colStyle: {'width': '60px'}},
      {field: 'GpsCartonBoxTwentyResult_TesterId', title: '测试员', colStyle: {'width': '60px'}},
      {field: 'GpsCartonBoxTwentyResult_TestTime', title: '测试时间', colStyle: {'width': '120px'}},
      {field: 'GpsCartonBoxTwentyResult_Remark1', title: '备注1', colStyle: {'width': '90px'}},
      {field: 'GpsCartonBoxTwentyResult_Remark2', title: '备注2', colStyle: {'width': '120px'}},
      {field: 'GpsCartonBoxTwentyResult_Remark3', title: '备注3', colStyle: {'width': '90px'}},
      {field: 'GpsCartonBoxTwentyResult_Remark4', title: '备注4', colStyle: {'width': '400px'}},
      {field: 'GpsCartonBoxTwentyResult_Remark5', title: '备注5', colStyle: {'width': '60px'}},
      {field: 'GpsCartonBoxTwentyResult_Computer', title: '计算机地址', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_ZhiDan', title: '制单号', colStyle: {'width': '120px'}},
      // {field: 'DataRelativeSheet_SN', title: 'SN', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI1', title: 'IMEI卡号', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI2', title: 'SN号', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI3', title: 'SIM卡号', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI4', title: 'ICCID', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI5', title: '密码:智能锁ID', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI6', title: '蓝牙MAC', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI7', title: '设备号', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI8', title: '服务卡号', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI9', title: '电池序列号', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI10', title: '第二个锁ID', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI11', title: '机器代码', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI12', title: 'IMSI', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI13', title: 'RFID', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_TestTime', title: '测试时间', colStyle: {'width': '180px'}},
    ]
  },

  Gps_CartonBoxTwenty_Result_False: {
    primaryKey: 'Id',
    type: 'int',
    queryOptions: [
      {
        id: 'BoxNo',
        name: '箱号',
        model: '',
        type: 'text'
      },
      // {
      //   id: 'IMEI',
      //   name: 'IMEI号',
      //   model: '',
      //   type: 'text'
      // },
      {
        id: 'IMEI',
        name: 'IMEI号',
        modelFrom: '',
        modelTo: '',
        type: 'range'
      },
      {
        id: 'ZhiDan',
        name: '制单号',
        model: '',
        type: 'text'
      },
      {
        id: 'SoftModel',
        name: '机型',
        model: '',
        type: 'text'
      },
      {
        id: 'Version',
        name: '软件版本',
        model: '',
        type: 'text'
      },
      {
        id: 'TestTime',
        name: '测试时间',
        timeRange: '',
        type: 'date'
      }
    ],
    dataColumns: [
      {field: 'Id', title: 'ID', colStyle: {'width': '70px'}},
      {field: 'BoxNo', title: '箱号', colStyle: {'width': '90px'}},
      {field: 'IMEI', title: 'IMEI号', colStyle: {'width': '120px'}},
      {field: 'ZhiDan', title: '制单号', colStyle: {'width': '120px'}},
      {field: 'SoftModel', title: '机型', colStyle: {'width': '60px'}},
      {field: 'Version', title: '软件版本', colStyle: {'width': '180px'}},
      {field: 'ProductCode', title: '产品编号', colStyle: {'width': '180px'}},
      {field: 'Color', title: '颜色', colStyle: {'width': '60px'}},
      {field: 'Qty', title: '数量', colStyle: {'width': '60px'}},
      {field: 'Weight', title: '重量', colStyle: {'width': '60px'}},
      {field: 'Date', title: '日期', colStyle: {'width': '90px'}},
      {field: 'TACInfo', title: '前缀', colStyle: {'width': '60px'}},
      {field: 'CompanyName', title: '公司名称', colStyle: {'width': '60px'}},
      {field: 'TesterId', title: '测试员', colStyle: {'width': '60px'}},
      {field: 'TestTime', title: '测试时间', colStyle: {'width': '120px'}},
      {field: 'Remark1', title: '备注1', colStyle: {'width': '90px'}},
      {field: 'Remark2', title: '备注2', colStyle: {'width': '120px'}},
      {field: 'Remark3', title: '备注3', colStyle: {'width': '90px'}},
      {field: 'Remark4', title: '备注4', colStyle: {'width': '400px'}},
      {field: 'Remark5', title: '备注5', colStyle: {'width': '60px'}},
      {field: 'Computer', title: '计算机地址', colStyle: {'width': '120px'}},
    ]
  },

  DataRelativeSheet_True: {
    primaryKey: 'IMEI1',
    type: 'string',
    queryOptions: [
      {
        id: 'select-map',
        selection: [
          {id: 'DataRelativeSheet.IMEI1', name: 'IMEI卡号'},
          {id: 'DataRelativeSheet.IMEI2', name: 'SN号'},
          {id: 'DataRelativeSheet.IMEI3', name: 'SIM卡号'},
          {id: 'DataRelativeSheet.IMEI4', name: 'ICCID'},
          {id: 'DataRelativeSheet.IMEI5', name: '密码:智能锁ID'},
          {id: 'DataRelativeSheet.IMEI6', name: '蓝牙MAC'},
          {id: 'DataRelativeSheet.IMEI7', name: '设备号'},
          {id: 'DataRelativeSheet.IMEI8', name: '服务卡号'},
          {id: 'DataRelativeSheet.IMEI9', name: '电池序列号'},
          {id: 'DataRelativeSheet.IMEI10', name: '第二个锁ID'},
          {id: 'DataRelativeSheet.IMEI11', name: '机器代码'},
          {id: 'DataRelativeSheet.IMEI12', name: 'IMSI'},
          {id: 'DataRelativeSheet.IMEI13', name: 'RFID'},
          {id: 'DataRelativeSheet.ZhiDan', name: '制单号'}],
        name: '根据以下标识查看绑定关系',
        selectModel: '',
        textModel: '',
        type: 'map'
      }
    ],
    dataColumns: [
      // {field: 'DataRelativeSheet_Id', title: 'ID', colStyle: {'width': '70px'}},
      {field: 'DataRelativeSheet_ZhiDan', title: '制单号', colStyle: {'width': '120px'}},
      // {field: 'DataRelativeSheet_SN', title: 'SN', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI1', title: 'IMEI卡号', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI2', title: 'SN号', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI3', title: 'SIM卡号', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI4', title: 'ICCID', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI5', title: '密码:智能锁ID', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI6', title: '蓝牙MAC', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI7', title: '设备号', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI8', title: '服务卡号', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI9', title: '电池序列号', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI10', title: '第二个锁ID', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI11', title: '机器代码', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI12', title: 'IMSI', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_IMEI13', title: 'RFID', colStyle: {'width': '120px'}},
      {field: 'DataRelativeSheet_TestTime', title: '测试时间', colStyle: {'width': '180px'}},
      {field: 'GpsManuprintparam_ID', title: '(打印)ID', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_ZhiDan', title: '(打印)制单号', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_IMEI', title: '(打印)打印IMEI', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_IMEIStart', title: '(打印)IMEI起始位', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_IMEIEnd', title: '(打印)IMEI终止位', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_SN', title: '(打印)SN号', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_IMEIRel', title: '(打印)IMEI绑定关系', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_SIM', title: '(打印)SIM卡号', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_VIP', title: '(打印)服务卡号', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_BAT', title: '(打印)电池号', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_SoftModel', title: '(打印)机型', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_Version', title: '(打印)软件版本', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_Remark', title: '(打印)备注', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_JS_PrintTime', title: '(打印)机身贴打印时间', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_JS_TemplatePath', title: '(打印)机身贴模版路径', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_JS_RePrintNum', title: '(打印)机身贴重打次数', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_JS_ReFirstPrintTime', title: '(打印)机身贴首次重打时间', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_JS_ReEndPrintTime', title: '(打印)机身贴末次重打时间', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_UserName', title: '(打印)用户名', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_CH_PrintTime', title: '(打印)彩盒帖打印时间', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_CH_TemplatePath1', title: '(打印)彩盒帖模版路径1', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_CH_TemplatePath2', title: '(打印)彩盒帖模版路径2', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_CH_RePrintNum', title: '(打印)彩盒帖重打次数', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_CH_ReFirstPrintTime', title: '(打印)彩盒帖首次重打时间', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_CH_ReEndPrintTime', title: '(打印)彩盒帖末次重打时间', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_ICCID', title: '(打印)ICCID号', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_MAC', title: '(打印)蓝牙MAC号', colStyle: {'width': '100px'}},
      {field: 'GpsManuprintparam_Equipment', title: '(打印)设备号', colStyle: {'width': '100px'}}
    ]
  },
  DataRelativeSheet_False: {
    primaryKey: 'IMEI1',
    type: 'string',
    queryOptions: [
      {
        id: 'select-map',
        selection: [
          {id: 'IMEI1', name: 'IMEI卡号'},
          {id: 'IMEI2', name: 'SN号'},
          {id: 'IMEI3', name: 'SIM卡号'},
          {id: 'IMEI4', name: 'ICCID'},
          {id: 'IMEI5', name: '密码:智能锁ID'},
          {id: 'IMEI6', name: '蓝牙MAC'},
          {id: 'IMEI7', name: '设备号'},
          {id: 'IMEI8', name: '服务卡号'},
          {id: 'IMEI9', name: '电池序列号'},
          {id: 'IMEI10', name: '第二个锁ID'},
          {id: 'IMEI11', name: '机器代码'},
          {id: 'IMEI12', name: 'IMSI'},
          {id: 'IMEI13', name: 'RFID'},
          {id: 'ZhiDan', name: '制单号'}],
        name: '根据以下标识查看绑定关系',
        selectModel: '',
        textModel: '',
        type: 'map'
      }
    ],
    dataColumns: [
      {field: 'ZhiDan', title: '制单号', colStyle: {'width': '120px'}},
      // {field: 'SN', title: 'SN', colStyle: {'width': '120px'}},
      {field: 'IMEI1', title: 'IMEI卡号', colStyle: {'width': '120px'}},
      {field: 'IMEI2', title: 'SN号', colStyle: {'width': '120px'}},
      {field: 'IMEI3', title: 'SIM卡号', colStyle: {'width': '120px'}},
      {field: 'IMEI4', title: 'ICCID', colStyle: {'width': '120px'}},
      {field: 'IMEI5', title: '密码:智能锁ID', colStyle: {'width': '120px'}},
      {field: 'IMEI6', title: '蓝牙MAC', colStyle: {'width': '120px'}},
      {field: 'IMEI7', title: '设备号', colStyle: {'width': '120px'}},
      {field: 'IMEI8', title: '服务卡号', colStyle: {'width': '120px'}},
      {field: 'IMEI9', title: '电池序列号', colStyle: {'width': '120px'}},
      {field: 'IMEI10', title: '第二个锁ID', colStyle: {'width': '120px'}},
      {field: 'IMEI11', title: '机器代码', colStyle: {'width': '120px'}},
      {field: 'IMEI12', title: 'IMSI', colStyle: {'width': '120px'}},
      {field: 'IMEI13', title: 'RFID', colStyle: {'width': '120px'}},
      {field: 'TestTime', title: '测试时间', colStyle: {'width': '180px'}},
    ]
  },

  Gps_TestResult: {
    primaryKey: 'Id',
    type: 'int',
    queryOptions: [
      {
        id: 'SN',
        name: 'SN号',
        model: '',
        type: 'text'
      },
      {
        id: 'ZhiDan',
        name: '制单号',
        model: '',
        type: 'text'
      },
      {
        id: 'RecordTime',
        name: '记录时间',
        timeRange: '',
        type: 'date'
      },
    ],
    dataColumns: [
      {field: 'Id', title: 'ID', colStyle: {'width': '100px'}},
      {field: 'SN', title: 'SN', colStyle: {'width': '100px'}},
      {field: 'IMEI', title: 'IMEI', colStyle: {'width': '100px'}},
      {field: 'ZhiDan', title: '制单号', colStyle: {'width': '100px'}},
      {field: 'RecordTime', title: '记录时间', colStyle: {'width': '100px'}},
      {field: 'SoftModel', title: '机型', colStyle: {'width': '80px'}},
      {field: 'Version', title: '版本号', colStyle: {'width': '80px'}},
      {field: 'FunctionResult', title: 'SMT测试', colStyle: {'width': '80px'}},
      {field: 'AutoTestResult', title: '组装测试', colStyle: {'width': '80px'}}, //SMT测试组装测试
      {
        field: 'GPSResult', title: '老化后测试', colStyle: {'width': '80px'}, formatter(row, column, cellValue, index) {
          switch (cellValue) {
            case 0:
              return '失败';
            case 1:
              return '成功';
          }
        }
      },
      {field: 'CoupleResult', title: '耦合测试', colStyle: {'width': '80px'}},
      {field: 'WriteIMEIResult', title: 'IMEI写入', colStyle: {'width': '80px'}},
      {field: 'ParamDownloadResult', title: '参数下载', colStyle: {'width': '80px'}},

      {field: 'AutoTestSMTResult', title: '预备字段1', colStyle: {'width': '80px'}},
      {field: 'SMTIQCResult', title: 'SMT IQC', colStyle: {'width': '90px'}},
      // {field: 'OtherTestSign', title: '???', colStyle: {'width': '100px'}, visible: false},
      // {
      //   field: 'Result', title: '总结果', colStyle: {'width': '80px'}, formatter(row, column, cellValue, index) {
      //     switch (cellValue) {
      //       case 0:
      //         return '失败';
      //       case 1:
      //         return '成功';
      //     }
      //   }
      // },
      {
        field: 'CPResult', title: '比对结果', colStyle: {'width': '80px'}, formatter(row, column, cellValue, index) {
          switch (cellValue) {
            case 0:
              return '失败';
            case 1:
              return '成功';
          }
        }
      },
      {
        field: 'WeightResult', title: '称重结果', colStyle: {'width': '80px'},
      },
      {
        field: 'OQCResult01', title: '工厂OQC结果', colStyle: {'width': '80px'}, formatter(row, column, cellValue, index) {
          switch (cellValue) {
            case 0:
              return '失败';
            case 1:
              return '成功';
          }
        }
      },
      {
        field: 'OQCResult02', title: '客户OQC结果', colStyle: {'width': '80px'}, formatter(row, column, cellValue, index) {
          switch (cellValue) {
            case 0:
              return '失败';
            case 1:
              return '成功';
          }
        }
      },
      {
        field: 'Result', title: '总结果', colStyle: {'width': '80px'}, formatter(row, column, cellValue, index) {
          switch (cellValue) {
            case 0:
              return '失败';
            case 1:
              return '成功';
          }
        }
      },
    ]
  },
  DataRelativeUnique: {
    primaryKey: 'DATA1',
    type: 'string',
    queryOptions: [
      {
        id: 'ZhiDan',
        name: '制单号',
        model: '',
        type: 'text'
      },
      {
        id: 'TestTime',
        name: '测试时间',
        timeRange: '',
        type: 'date'
      }
    ],
    dataColumns: [
      {field: 'ZhiDan', title: '制单号', colStyle: {'width': '100px'}},
      {field: 'TestTime', title: '测试时间', colStyle: {'width': '100px'}},
      {field: 'DATA1', title: '绑定数据1', colStyle: {'width': '100px'}},
      {field: 'DATA2', title: '绑定数据2', colStyle: {'width': '100px'}},
      {field: 'DATA3', title: '绑定数据3', colStyle: {'width': '100px'}},
      {field: 'DATA4', title: '绑定数据4', colStyle: {'width': '100px'}},
      {field: 'DATA5', title: '绑定数据5', colStyle: {'width': '100px'}},
      {field: 'DATA6', title: '绑定数据6', colStyle: {'width': '100px'}},
      {field: 'DATA7', title: '绑定数据7', colStyle: {'width': '100px'}},
      {field: 'DATA8', title: '绑定数据8', colStyle: {'width': '100px'}},
      {field: 'DATA9', title: '绑定数据9', colStyle: {'width': '100px'}},
      {field: 'DATA10', title: '绑定数据10', colStyle: {'width': '100px'}},
      {field: 'DATA11', title: '绑定数据11', colStyle: {'width': '100px'}},
      {field: 'DATA12', title: '绑定数据12', colStyle: {'width': '100px'}}
    ]
  },

  Gps_ManuCpParam: {
    primaryKey: 'ID',
    type: 'int',
    queryOptions: [
      {
        id: 'ZhiDan',
        name: '制单号',
        model: '',
        type: 'text'
      },
      {
        id: 'IMEI1',
        name: '机身贴IMEI',
        model: '',
        type: 'text'
      },
      {
        id: 'IMEI2',
        name: '彩盒贴IMEI',
        model: '',
        type: 'text'
      }
    ],
    dataColumns: [
      {field: 'ID', title: 'ID', colStyle: {'width': '70px'},},
      {field: 'ZhiDan', title: '制单号', colStyle: {'width': '100px'}},
      {field: 'CPIP', title: '主机IP', colStyle: {'width': '100px'}},
      {field: 'IMEI1', title: '机身贴IMEI', colStyle: {'width': '100px'}},
      {field: 'IMEI2', title: '彩盒贴IMEI', colStyle: {'width': '100px'}},
      {field: 'SECTIONNO1', title: '号段1', colStyle: {'width': '100px'}},
      {field: 'SECTIONNO2', title: '号段2', colStyle: {'width': '100px'}},
      {
        field: 'CPRESULT', title: '对比结果', colStyle: {'width': '100px'}, formatter(row, column, cellValue, index) {
          switch (cellValue) {
            case 0:
              return '失败';
            case 1:
              return '成功';
          }
        }
      },
      {field: 'CPTIME', title: '对比成功时间', colStyle: {'width': '100px'}},
      {
        field: 'CPTYPE', title: '号段类型', colStyle: {'width': '100px'}, formatter(row, column, cellValue, index) {
          switch (cellValue) {
            case 0:
              return 'IMEI号';
            case 1:
              return 'SN号';
            case 2:
              return 'SIM卡号';
            case 3:
              return 'VIP号';
            case 4:
              return 'ICCID号';
            case 5:
              return 'BAT号';
            case 6:
              return '蓝牙号';
            case 7:
              return '设备号';
          }
        }
      },
      {field: 'CPFalseCount', title: '对比失败次数', colStyle: {'width': '60px'}},
      {field: 'RECPTIME', title: '重对比时间', colStyle: {'width': '100px'}},
      {field: 'CPERROR', title: '失败原因', colStyle: {'width': '120px'}},
    ]
  },
  NetMarkIMEI: {
    primaryKey: 'Id',
    type: 'int',
    queryOptions: [
      {
        id: 'NetMark',
        name: '网标',
        model: '',
        type: 'text'
      },
      {
        id: 'IMEI',
        name: 'IMEI',
        model: '',
        type: 'text'
      },
      {
        id: 'SN',
        name: 'SN号',
        model: '',
        type: 'text'
      }
    ],
    dataColumns: [
      {field: 'Id', title: 'ID', colStyle: {'width': '70px'},},
      {field: 'NetMark', title: '网标', colStyle: {'width': '100px'}},
      {field: 'IMEI', title: 'IMEI', colStyle: {'width': '100px'}},
      {field: 'PrintCount', title: '打印计数', colStyle: {'width': '100px'}},
      {field: 'SN', title: 'SN号', colStyle: {'width': '100px'}},
      {field: 'Zhidan', title: '制单号', colStyle: {'width': '100px'}},
      {field: 'NMTime', title: '网标绑定时间', colStyle: {'width': '100px'}},
      {field: 'RFID', title: 'RFID', colStyle: {'width': '100px'}},
      {field: 'JS_PrintTime', title: '机身贴打印时间', colStyle: {'width': '100px'}},
      {field: 'JS_TemPlate', title: '机身贴模版', colStyle: {'width': '100px'}},
      {field: 'RePrintNum', title: '重打次数', colStyle: {'width': '100px'}},
      {field: 'RePrintFirstTime', title: '首次重打时间', colStyle: {'width': '100px'}},
      {field: 'RePrintEndTime', title: '末次重打时间', colStyle: {'width': '100px'}}

    ]
  },

  LTestLogMessage: {
    queryOptions: [
      {
        id: 'SN',
        name: 'SN号',
        model: '',
        type: 'text'
      },
      {
        id: 'ZhiDan',
        name: '制单号',
        model: '',
        type: 'text'
      },
      {
        id: 'SoftModel',
        name: '机型',
        model: '',
        type: 'text'
      },
      {
        id: 'TestTime',
        name: '时间',
        timeRange: '',
        type: 'date'
      }
    ],
    dataColumns: [
      {field: 'SN', title: 'SN号', colStyle: {'width': '100px'},},
      {field: 'IMEI', title: 'IMEI', colStyle: {'width': '100px'},},
      {field: 'SoftModel', title: '机型', colStyle: {'width': '100px'},},
      {field: 'Version', title: '版本号', colStyle: {'width': '100px'},},
      {field: 'ZhiDan', title: '订单号', colStyle: {'width': '100px'},},
      {field: 'ErrorMessage1', title: '组装错误信息', colStyle: {'width': '100px'},},
      {field: 'CategoryNum1', title: '组装错误类型', colStyle: {'width': '100px'},},
      {field: 'Computer', title: '错误主机(组装)', colStyle: {'width': '100px'},},
      {field: 'ErrorMessage2', title: '耦合错误信息', colStyle: {'width': '100px'},},
      {field: 'CategoryNum2', title: '耦合错误类型', colStyle: {'width': '100px'},},
      {field: 'Computer2', title: '错误主机(耦合)', colStyle: {'width': '100px'},},
      {field: 'ErrorMessage3', title: 'SMT错误信息', colStyle: {'width': '100px'},},
      {field: 'CategoryNum3', title: 'SMT错误类型', colStyle: {'width': '100px'},},
      {field: 'Computer3', title: '错误主机(SMT)', colStyle: {'width': '100px'},},
      {field: 'ErrorMessage4', title: '站位错误信息', colStyle: {'width': '100px'},},
      {field: 'CategoryNum4', title: '站位错误类型', colStyle: {'width': '100px'},},
      {field: 'Computer4', title: '错误主机(站位)', colStyle: {'width': '100px'},},
      {field: 'ErrorMessage5', title: '老化后错误信息', colStyle: {'width': '100px'},},
      {field: 'CategoryNum5', title: '老化后错误类型', colStyle: {'width': '100px'},},
      {field: 'Computer5', title: '错误主机(老化后)', colStyle: {'width': '100px'},},
      {field: 'TestTime', title: '时间', colStyle: {'width': '100px'},},


    ]
  },
  GPSOQC_Result: {
    primaryKey: 'Id',
    type: 'int',
    queryOptions: [
      {
        id: 'SN',
        name: '芯片ID',
        model: '',
        type: 'text'
      },
      {
        id: 'IMEI',
        name: 'IMEI号',
        model: '',
        type: 'text'
      },
      {
        id: 'ZhiDan',
        name: '制单号',
        model: '',
        type: 'text'
      },
      {
        id: 'SoftModel',
        name: '机型',
        model: '',
        type: 'text'
      },
      {
        id: 'Version',
        name: '软件版本',
        model: '',
        type: 'text'
      },
      {
        id: 'TestTime',
        name: '时间',
        timeRange: '',
        type: 'date'
      }
    ],
    dataColumns: [
      {field: 'Id', title: 'ID', colStyle: {'width': '80px'},},
      {field: 'SN', title: '芯片ID', colStyle: {'width': '200px'},},
      {field: 'IMEI', title: 'IMEI', colStyle: {'width': '100px'},},
      {field: 'ZhiDan', title: '订单号', colStyle: {'width': '200px'},},
      {field: 'SoftModel', title: '机型', colStyle: {'width': '100px'},},
      {field: 'Version', title: '版本号', colStyle: {'width': '100px'},},
      {field: 'Result', title: '测试结果', colStyle: {'width': '100px'}, formatter(row, column, cellValue, index) {
        switch (cellValue) {
        case 0:
          return '失败';
        case 1:
          return '成功';
        }
      }},
      {field: 'TestId', title: '测试人员', colStyle: {'width': '100px'},},
      {field: 'ConfigId', title: '配置人员', colStyle: {'width': '100px'},},
      {field: 'TestGrade', title: '配置权限', colStyle: {'width': '100px'}, formatter(row, column, cellValue, index) {
          switch (cellValue) {
            case 0:
              return '测试人员';
            case 1:
              return '管理员';
          }
        }},
      {field: 'Computer', title: '地址', colStyle: {'width': '100px'},},
      {field: 'TestSetting', title: '配置指令', colStyle: {'width': '400px'},},
      {field: 'TestTime', title: '测试时间', colStyle: {'width': '100px'},},
      {field: 'Remark', title: '备注', colStyle: {'width': '100px'},},


    ]
  },


  DataRelativeUpdate: {
    primaryKey: 'IMEI1',
    type: 'string',
    queryOptions: [
      {
        id: 'select-map',
        selection: [
          {id: 'IMEI1', name: 'IMEI卡号'},
          {id: 'IMEI2', name: 'SN号'},
          {id: 'IMEI3', name: 'SIM卡号'},
          {id: 'IMEI4', name: 'ICCID'},
          {id: 'IMEI5', name: '密码:智能锁ID'},
          {id: 'IMEI6', name: '蓝牙MAC'},
          {id: 'IMEI7', name: '设备号'},
          {id: 'IMEI8', name: '服务卡号'},
          {id: 'IMEI9', name: '电池序列号'},
          {id: 'IMEI10', name: '第二个锁ID'},
          {id: 'IMEI11', name: '机器代码'},
          {id: 'IMEI12', name: 'IMSI'},
          {id: 'IMEI13', name: 'RFID'},
          {id: 'ZhiDan', name: '制单号'}],
        name: '根据以下标识查看绑定关系',
        selectModel: '',
        textModel: '',
        type: 'map'
      }
    ],
    dataColumns: [
      {field: 'RelativeNum', title: '关联码', colStyle: {'width': '120px'}},
      {field: 'ZhiDan', title: '制单号', colStyle: {'width': '120px'}},
      // {field: 'SN', title: 'SN', colStyle: {'width': '120px'}},
      {field: 'IMEI1', title: 'IMEI卡号', colStyle: {'width': '120px'}},
      {field: 'IMEI2', title: 'SN号', colStyle: {'width': '120px'}},
      {field: 'IMEI3', title: 'SIM卡号', colStyle: {'width': '120px'}},
      {field: 'IMEI4', title: 'ICCID', colStyle: {'width': '120px'}},
      {field: 'IMEI5', title: '密码:智能锁ID', colStyle: {'width': '120px'}},
      {field: 'IMEI6', title: '蓝牙MAC', colStyle: {'width': '120px'}},
      {field: 'IMEI7', title: '设备号', colStyle: {'width': '120px'}},
      {field: 'IMEI8', title: '服务卡号', colStyle: {'width': '120px'}},
      {field: 'IMEI9', title: '电池序列号', colStyle: {'width': '120px'}},
      {field: 'IMEI10', title: '第二个锁ID', colStyle: {'width': '120px'}},
      {field: 'IMEI11', title: '机器代码', colStyle: {'width': '120px'}},
      {field: 'IMEI12', title: 'IMSI', colStyle: {'width': '120px'}},
      {field: 'IMEI13', title: 'RFID', colStyle: {'width': '120px'}},
      {field: 'SimEffectiveDate', title: 'SIM卡激活时间', colStyle: {'width': '120px'}},
      {field: 'NetMark', title: '网标', colStyle: {'width': '120px'}},
      {field: 'TestTime', title: '测试时间', colStyle: {'width': '180px'}},
    ]
  },

};
const ROUTER_CONFIG_SP = {
  Gps_ManuSimDataParam: {
    primaryKey: 'ID',
    type: 'int',
    // queryOptions: [
    //   {
    //     id: 'IMEI',
    //     name: 'IMEI',
    //     model: '',
    //     type: 'text'
    //   }
    // ],
    dataColumns: [
      {field: 'ID', title: 'ID', colStyle: {'width': '70px'},},
      {field: 'SDIP', title: '主机IP', colStyle: {'width': '100px'}},
      {field: 'RID', title: '基带ID', colStyle: {'width': '160px'}},
      {field: 'IMEI', title: 'IMEI', colStyle: {'width': '100px'}},
      {field: 'CID', title: 'SIM种子号', colStyle: {'width': '100px'}},
      {field: 'ICCID', title: 'SIM卡串号', colStyle: {'width': '100px'}},
      {field: 'SDOperator', title: '操作员', colStyle: {'width': '80px'}},
      {field: 'SDTime', title: '下载成功时间', colStyle: {'width': '120px'}},
      {
        field: 'SDRESULT', title: '下载结果', colStyle: {'width': '120px'}, formatter(row, column, cellValue, index) {
          switch (cellValue) {
            case 0:
              return '失败';
            case 1:
              return '成功';
            case 2:
              return '已返工,未下载'
          }
        }
      },
      {field: 'ReSDTime', title: '返工成功时间', colStyle: {'width': '120px'}},
      {field: 'ReSDCount', title: '返工次数', colStyle: {'width': '80px'}},

    ]
  },
  Gps_ManuPrintParam: {
    primaryKey: 'ID',
    type: 'int',
    // queryOptions: [
    //   {
    //     id: 'IMEI',
    //     name: '打印IMEI',
    //     model: '',
    //     type: 'text'
    //   },
    //   {
    //     id: 'ZhiDan',
    //     name: '制单号',
    //     model: '',
    //     type: 'text'
    //   },
    //   {
    //     id: 'JS_PrintTime',
    //     name: '机身贴打印时间',
    //     modelFrom: '',
    //     modelTo: '',
    //     type: 'date'
    //   }
    // ],
    dataColumns: [
      {field: 'ID', title: 'ID', colStyle: {'width': '70px'},},
      {field: 'ZhiDan', title: '制单号', colStyle: {'width': '100px'}},
      {field: 'IMEI', title: '打印IMEI', colStyle: {'width': '100px'}},
      {field: 'IMEIStart', title: 'IMEI起始位', colStyle: {'width': '100px'}},
      {field: 'IMEIEnd', title: 'IMEI终止位', colStyle: {'width': '100px'}},
      {field: 'SN', title: 'SN号', colStyle: {'width': '100px'}},
      {field: 'IMEIRel', title: 'IMEI绑定关系', colStyle: {'width': '100px'}},
      {field: 'SIM', title: 'SIM卡号', colStyle: {'width': '100px'}},
      {field: 'VIP', title: '服务卡号', colStyle: {'width': '100px'}},
      {field: 'BAT', title: '电池号', colStyle: {'width': '100px'}},
      {field: 'SoftModel', title: '机型', colStyle: {'width': '100px'}},
      {field: 'Version', title: '软件版本', colStyle: {'width': '100px'}},
      {field: 'Remark', title: '备注', colStyle: {'width': '100px'}},
      {field: 'JS_PrintTime', title: '机身贴打印时间', colStyle: {'width': '100px'}},
      {field: 'JS_TemplatePath', title: '机身贴模版路径', colStyle: {'width': '160px'}},
      {field: 'JS_RePrintNum', title: '机身贴重打次数', colStyle: {'width': '100px'}},
      {field: 'JS_ReFirstPrintTime', title: '机身贴首次重打时间', colStyle: {'width': '100px'}},
      {field: 'JS_ReEndPrintTime', title: '机身贴末次重打时间', colStyle: {'width': '100px'}},
      {field: 'UserName', title: '用户名', colStyle: {'width': '100px'}},
      {field: 'CH_PrintTime', title: '彩盒帖打印时间', colStyle: {'width': '100px'}},
      {field: 'CH_TemplatePath1', title: '彩盒帖模版路径1', colStyle: {'width': '100px'}},
      {field: 'CH_TemplatePath2', title: '彩盒帖模版路径2', colStyle: {'width': '100px'}},
      {field: 'CH_RePrintNum', title: '彩盒帖重打次数', colStyle: {'width': '100px'}},
      {field: 'CH_ReFirstPrintTime', title: '彩盒帖首次重打时间', colStyle: {'width': '100px'}},
      {field: 'CH_ReEndPrintTime', title: '彩盒帖末次重打时间', colStyle: {'width': '100px'}},
      {field: 'ICCID', title: 'ICCID号', colStyle: {'width': '100px'}},
      {field: 'MAC', title: '蓝牙MAC号', colStyle: {'width': '100px'}},
      {field: 'Equipment', title: '设备号', colStyle: {'width': '100px'}}

    ]
  },
};
export const setRouterConfigSP = (name) => {
  if (name === 'Gps_ManuSimDataParam') {
    return {
      data: ROUTER_CONFIG_SP.Gps_ManuSimDataParam
    }
  } else if (name === 'Gps_ManuPrintParam') {
    return {
      data: ROUTER_CONFIG_SP.Gps_ManuPrintParam
    }
  }
};
