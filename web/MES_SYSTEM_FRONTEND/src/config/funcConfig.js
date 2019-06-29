export const AutoTestResultColumn = () => [
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
];

export const TestResultColumn = () => [
  {field: 'Id', title: 'ID', colStyle: {'width': '100px'}},
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
  {field: 'AutoTestSMTResult', title: 'SMT自动测试', colStyle: {'width': '120px'}},
  {field: 'SMTIQCResult', title: 'SMT IQC', colStyle: {'width': '90px'}},
  // {field: 'OtherTestSign', title: '???', colStyle: {'width': '100px'}, visible: false},
  {
    field: 'Result', title: '总结果', colStyle: {'width': '80px'}, formatter(row, column, cellValue, index) {
      switch (cellValue) {
        case 0:
          return '失败';
        case 1:
          return '成功';
      }
    }
  }
];


export const AntiDupColumn = () => [
  {field: 'SN', title: 'SN', colStyle: {'width': '100px'}},
  {field: 'Data1', title: 'MAC地址', colStyle: {'width': '100px'}},
  // {field: 'SN', title: 'SN', colStyle: {'width': '100px'}},
  // {field: 'SN', title: 'SN', colStyle: {'width': '100px'}},
  // {field: 'SN', title: 'SN', colStyle: {'width': '100px'}},
  // {field: 'SN', title: 'SN', colStyle: {'width': '100px'}},
  // {field: 'SN', title: 'SN', colStyle: {'width': '100px'}},
  {field: 'ZhiDan', title: '制单号', colStyle: {'width': '100px'}},
  {field: 'Computer', title: '计算机', colStyle: {'width': '100px'}},
  {field: 'TestTime', title: '测试时间', colStyle: {'width': '100px'}},
  {field: 'Version', title: '版本号', colStyle: {'width': '100px'}},
  {field: 'SoftModel', title: '软件型号', colStyle: {'width': '100px'}},
];
