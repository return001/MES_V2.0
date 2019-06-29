/*报表统一配置页面*/
let url;
if (process.env.NODE_ENV === 'production') {
  url = window.g.API_URL
} else {
  url = window.g.LOCAL_URL
}
export const tableSelectUrl = url + "/mes_server/report/select";
export const getRequestUrl = (name) => {
  switch (name){
    case 'DataRelativeSheet':
      return url + '/report/selectDataRelativeSheet';
    case 'Gps_ManuSimDataParam':
      return url + '/report/selectGpsManuSimDataParam';
    default:
      return url + '/report/select'
  }
};
//export const routerUrl = "http://10.10.11.109:8080/mes_server/report/select";
export const setRouterConfig = (name) => {
  switch (name) {
    case 'GpsTcData':
    case 'GpsSMT_TcData':
      return {
        data: ROUTER_CONFIG.GpsTcData
      };
    case 'Gps_AutoTest_Result':
    case 'Gps_AutoTest_Result2':
    case 'Gps_AutoTest_Result3':
    case 'Gps_CoupleTest_Result':
      return {
        data: ROUTER_CONFIG.Gps_AutoTest_Result
      };
    case 'Gps_ParamDownload_Result':
      return {
        data: ROUTER_CONFIG.Gps_ParamDownload_Result
      };
    case 'Gps_CartonBoxTwenty_Result':
      return {
        data: ROUTER_CONFIG.Gps_CartonBox_Result
      };
    case 'Gps_OperRecord':
      return {
        data: ROUTER_CONFIG.Gps_OperRecord
      };
    case 'DataRelativeSheet':
      return {
        data: ROUTER_CONFIG.DataRelative_Sheet
      };
    case 'JS_PrintTime':
      return {
        data: ROUTER_CONFIG.JS_PrintTime
      };
    case 'CH_PrintTime':
      return {
        data: ROUTER_CONFIG.CH_PrintTime
      };
    case 'Gps_TestResult':
      return {
        data: ROUTER_CONFIG.Gps_TestResult
      };
    case 'Gps_ManuLdParam':
      return {
        data: ROUTER_CONFIG.Gps_ManuLdParam
      }
  }
};

const ROUTER_CONFIG = {
  GpsTcData: {
    queryOptions: [
      {
        id: 'SN',
        name: 'SN号',
        model: '',
        type: 'text'
      }
    ],
    dataColumns: [
      {field: 'showId', title: '序号', colStyle: {'width': '70px'}},
      {field: 'SN', title: 'SN号', colStyle: {'min-width': '60px', 'max-width': '200px'}},
      {field: 'FixMode', title: '定位方式', colStyle: {'width': '60px'}},
      {field: 'GpsDb_0', title: 'GpsDb信息_1', colStyle: {'width': '60px'}},
      {field: 'GpsDb_1', title: 'GpsDb信息_2', colStyle: {'width': '60px'}},
      {field: 'GpsDb_2', title: 'GpsDb信息_3', colStyle: {'width': '60px'}},
      {field: 'GpsDb_3', title: 'GpsDb信息_4', colStyle: {'width': '60px'}},
      {field: 'GpsDb_4', title: 'GpsDb信息_5', colStyle: {'width': '60px'}},
      {field: 'GpsDb_5', title: 'GpsDb信息_6', colStyle: {'width': '60px'}},
      {field: 'GpsDb_6', title: 'GpsDb信息_7', colStyle: {'width': '60px'}},
      {field: 'GpsDb_7', title: 'GpsDb信息_8', colStyle: {'width': '60px'}},
      {field: 'GpsDb_8', title: 'GpsDb信息_9', colStyle: {'width': '60px'}},
      {field: 'GpsDb_9', title: 'GpsDb信息_10', colStyle: {'width': '60px'}},
      {field: 'GpsDb_10', title: 'GpsDb信息_11', colStyle: {'width': '60px'}},
      {field: 'GpsDb_11', title: 'GpsDb信息_12', colStyle: {'width': '60px'}},
    ]
  },
  Gps_AutoTest_Result: {
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
        id: 'TestTime',
        name: '测试时间',
        modelFrom: '',
        modelTo: '',
        type: 'date'
      }
    ],
    dataColumns: [
      {field: 'showId', title: '序号', colStyle: {'width': '70px'},},
      {field: 'SN', title: 'SN号', colStyle: {'width': '260px'}},
      {field: 'IMEI', title: 'IMEI号', colStyle: {'width': '135px'}},
      {field: 'ZhiDan', title: '制单号', colStyle: {'width': '135px'}},
      {field: 'SoftModel', title: '机型', colStyle: {'width': '120px'}},
      {field: 'Version', title: '软件版本', colStyle: {'width': '200px'}},
      {field: 'Result', title: '结果', colStyle: {'width': '50px'}},
      {field: 'TesterId', title: '测试员', colStyle: {'width': '70px'}},
      {field: 'Computer', title: '地址', colStyle: {'width': '160px'}},
      {field: 'TestSetting', title: '测试配置', colStyle: {'width': '400px'}},
      {field: 'TestTime', title: '测试时间', colStyle: {'width': '175px'}},
      {field: 'Remark', title: '备注', colStyle: {'width': '50px'}}
    ]
  },
  Gps_ParamDownload_Result: {
    queryOptions: [
      {
        id: 'SN',
        name: 'SN号',
        model: '',
        type: 'text'
      },
      {
        id: 'TestTime',
        name: '测试时间',
        modelFrom: '',
        modelTo: '',
        type: 'date'
      }
    ],
    dataColumns: [
      {field: 'showId', title: '序号', colStyle: {'width': '70px'},},
      {field: 'SN', title: 'SN号', colStyle: {'width': '260px'}},
      {field: 'IMEI', title: 'IMEI号', colStyle: {'width': '135px'}},
      {field: 'SoftModel', title: '机型', colStyle: {'width': '120px'}},
      {field: 'Version', title: '软件版本', colStyle: {'width': '200px'}},
      {field: 'Result', title: '结果', colStyle: {'width': '50px'}},
      {field: 'TesterId', title: '测试员', colStyle: {'width': '70px'}},
      {field: 'Computer', title: '地址', colStyle: {'width': '50px'}},
      {field: 'TestSetting', title: '测试配置', colStyle: {'width': '80px'}},
      {field: 'TestTime', title: '测试时间', colStyle: {'width': '175px'}},
      {field: 'Remark', title: '备注', colStyle: {'width': '50px'}}
    ]
  },

  Gps_CartonBox_Result: {
    queryOptions: [
      {
        id: 'BoxNo',
        name: '箱号',
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
        id: 'TestTime',
        name: '测试时间',
        modelFrom: '',
        modelTo: '',
        type: 'date'
      }
    ],
    dataColumns: [
      {field: 'showId', title: '序号', colStyle: {'width': '70px'}},
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

  Gps_OperRecord: {
    queryOptions: [
      {
        id: 'OperTime',
        name: '操作时间',
        modelFrom: '',
        modelTo: '',
        type: 'date'
      }
    ],
    dataColumns: [
      {field: 'showId', title: '序号', colStyle: {'width': '70px'}},
      {field: 'OperName', title: '操作用户', colStyle: {'width': '100px'}},
      {field: 'OperContent', title: '操作事项', colStyle: {'width': '100px'}},
      {field: 'OperTime', title: '操作时间', colStyle: {'width': '100px'}},
      {field: 'OperDemo', title: 'OperDemo', colStyle: {'width': '100px'}},

    ]
  },

  DataRelative_Sheet: {
    queryOptions: [
      {
        id: 'select-range',
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
          {id: 'ZhiDan', name: '制单号'}],
        name: '根据以下标识查看绑定关系',
        selectModel: '',
        textModel: '',
        type: 'range'
      }
    ],
    dataColumns: [
      {field: 'showId', title: '序号', colStyle: {'width': '70px'}},
      {field: 'SN', title: 'SN', colStyle: {'width': '120px'}},
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
      {field: 'IMEI11', title: '??号', colStyle: {'width': '120px'}, visible: false},
      {field: 'IMEI12', title: '???号', colStyle: {'width': '120px'}, visible: false},
      {field: 'ZhiDan', title: '制单号', colStyle: {'width': '120px'}},
      {field: 'TestTime', title: '测试时间', colStyle: {'width': '180px'}},
    ]
  },

  Gps_ManuLdParam: {
    queryOptions: [
      {
        id: 'LDTime',
        name: '镭雕成功时间',
        modelFrom: '',
        modelTo: '',
        type: 'date'
      }
    ],
    dataColumns: [
      {title: '镭雕成功时间', filed: 'LDTime', colStyle:{'width': '120px'}},
      {title: '主机IP', filed: 'LDIP', colStyle:{'width': '120px'}},
      {title: '制单号', filed: 'ZhiDan', colStyle:{'width': '120px'}},
      {title: 'IMEI号', filed: 'IMEI', colStyle:{'width': '120px'}},
      {title: '递增SN号', filed: 'SN', colStyle:{'width': '120px'}},
      {title: 'ICCID卡串号', filed: 'ICCID', colStyle:{'width': '120px'}},
      {title: 'SIM卡号', filed: 'SIM', colStyle:{'width': '120px'}},
      {title: 'VIP', filed: 'VIP', colStyle:{'width': '120px'}},
      {title: '电池号', filed: 'BAT', colStyle:{'width': '120px'}},
      {title: '设备号', filed: 'EquipmentNumber', colStyle:{'width': '120px'}},
      {title: '蓝牙号', filed: 'MAC', colStyle:{'width': '120px'}},
      {title: '软件型号', filed: 'SoftModel', colStyle:{'width': '120px'}},
      {title: '版本号', filed: 'Version', colStyle:{'width': '120px'}},
      {title: '备注', filed: 'Remark', colStyle:{'width': '120px'}},
      {title: '镭雕结果(0:失败、1:成功)', filed: 'LDRESULT', colStyle:{'width': '120px'}},
      {title: '最后重镭时间', filed: 'ReLdImeiTime', colStyle:{'width': '120px'}},

    ]

  },

  JS_PrintTime: {
    queryOptions: [
      {
        id: 'IMEI',
        name: '打印IMEI',
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
        id: 'JS_PrintTime',
        name: '机身贴打印时间',
        modelFrom: '',
        modelTo: '',
        type: 'date'
      }
    ],
    dataColumns: [

      {field: 'ID', title: '', colStyle: {'width': '100px'}, visible: false},
      {field: 'ZhiDan', title: '制单号', colStyle: {'width': '100px'}},
      {field: 'IMEI', title: '打印IMEI', colStyle: {'width': '100px'}},
      {field: 'IMEIStart', title: 'IMEI起始位', colStyle: {'width': '100px'}},
      {field: 'IMEIEnd', title: 'IMEI终止位', colStyle: {'width': '100px'}},
      {field: 'SN', title: 'SN号', colStyle: {'width': '100px'}},
      {field: 'IMEIRel', title: 'IMEI绑定关系', colStyle: {'width': '100px'}},
      {field: 'SIM', title: 'SIM卡号', colStyle: {'width': '100px'}},
      {field: 'VIP', title: '服务卡号', colStyle: {'width': '100px'}},
      {field: 'BAT', title: '电池号', colStyle: {'width': '100px'}},
      {field: 'SoftModel', title: '软件型号', colStyle: {'width': '100px'}},
      {field: 'Version', title: '软件版本', colStyle: {'width': '100px'}},
      {field: 'Remark', title: '备注', colStyle: {'width': '100px'}},
      {field: 'JS_PrintTime', title: '机身贴打印时间', colStyle: {'width': '100px'}},
      {field: 'JS_TemplatePath', title: '机身贴模版路径', colStyle: {'width': '100px'}},
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
  CH_PrintTime: {
    queryOptions: [
      {
        id: 'IMEI',
        name: '打印IMEI',
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
        id: 'CH_PrintTime',
        name: '彩盒帖打印时间',
        modelFrom: '',
        modelTo: '',
        type: 'date'
      }
    ],
    dataColumns: [

      {field: 'ID', title: '', colStyle: {'width': '100px'}, visible: false},
      {field: 'ZhiDan', title: '制单号', colStyle: {'width': '100px'}},
      {field: 'IMEI', title: '打印IMEI', colStyle: {'width': '100px'}},
      {field: 'IMEIStart', title: 'IMEI起始位', colStyle: {'width': '100px'}},
      {field: 'IMEIEnd', title: 'IMEI终止位', colStyle: {'width': '100px'}},
      {field: 'SN', title: 'SN号', colStyle: {'width': '100px'}},
      {field: 'IMEIRel', title: 'IMEI绑定关系', colStyle: {'width': '100px'}},
      {field: 'SIM', title: 'SIM卡号', colStyle: {'width': '100px'}},
      {field: 'VIP', title: '服务卡号', colStyle: {'width': '100px'}},
      {field: 'BAT', title: '电池号', colStyle: {'width': '100px'}},
      {field: 'SoftModel', title: '软件型号', colStyle: {'width': '100px'}},
      {field: 'Version', title: '软件版本', colStyle: {'width': '100px'}},
      {field: 'Remark', title: '备注', colStyle: {'width': '100px'}},
      {field: 'JS_PrintTime', title: '机身贴打印时间', colStyle: {'width': '100px'}},
      {field: 'JS_TemplatePath', title: '机身贴模版路径', colStyle: {'width': '100px'}},
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
  Gps_TestResult: {
    queryOptions: [
      {
        id: 'IMEI',
        name: '打印IMEI',
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
        modelFrom: '',
        modelTo: '',
        type: 'date'
      },
    ],
    dataColumns:[
      {field: 'Id', title: '', colStyle: {'width': '100px'}, visible: false},
      {field: 'SN', title: 'SN', colStyle: {'width': '100px'}},
      {field: 'IMEI', title: 'IMEI', colStyle: {'width': '100px'}},
      {field: 'ZhiDan', title: '制单号', colStyle: {'width': '100px'}},
      {field: 'RecordTime', title: '记录时间', colStyle: {'width': '100px'}},
      {field: 'SoftModel', title: '软件型号', colStyle: {'width': '80px'}},
      {field: 'Version', title: '版本号', colStyle: {'width': '80px'}},
      {field: 'FunctionResult', title: '功能测试', colStyle: {'width': '80px'}},
      {field: 'GPSResult', title: 'GPS测试', colStyle: {'width': '80px'}},
      {field: 'CoupleResult', title: '耦合测试', colStyle: {'width': '80px'}},
      {field: 'WriteIMEIResult', title: 'IMEI写入', colStyle: {'width': '80px'}},
      {field: 'ParamDownloadResult', title: '参数下载', colStyle: {'width': '80px'}},
      {field: 'AutoTestResult', title: '自动测试', colStyle: {'width': '80px'}},
      {field: 'AutoTestSMTResult', title: 'SMT自动测试', colStyle: {'width': '80px'}},
      {field: 'SMTIQCResult', title: 'SMT IQC', colStyle: {'width': '90px'}},
      {field: 'OtherTestSign', title: '???', colStyle: {'width': '100px'}, visible: false},
      {field: 'Result', title: '总结果', colStyle: {'width': '80px'}}
    ]
  }

}
