export const MultiTableConfig = [
  {
    title: 'Gps_AutoTest_Result',
    columns: [
      {field: 'Id', title: 'ID', colStyle: {'width': '70px'},},
      {field: 'IMEI', title: 'IMEI号', colStyle: {'width': '135px'}},
      {field: 'SN', title: 'SN号', colStyle: {'width': '260px'}},
      {field: 'ZhiDan', title: '制单号', colStyle: {'width': '135px'}},
      {field: 'TestTime', title: '测试时间', colStyle: {'width': '175px'}},
      {field: 'SoftModel', title: '机型', colStyle: {'width': '120px'}},
      {field: 'Version', title: '软件版本', colStyle: {'width': '200px'}},
      {field: 'Result', title: '结果', colStyle: {'width': '50px'}, formatter(row, column, cellValue, index) {
          switch (cellValue) {
            case 0:
              return '失败';
            case 1:
              return '成功';
          }
        }},
      {field: 'TesterId', title: '测试员', colStyle: {'width': '70px'}},
      {field: 'Computer', title: '地址', colStyle: {'width': '160px'}},
      // {field: 'TestSetting', title: '测试配置', colStyle: {'width': '400px'}},
      {field: 'Remark', title: '备注', colStyle: {'width': '50px'}}
    ]
  },
  {
    title: 'Gps_AutoTest_Result3',
    columns: [
      {field: 'Id', title: 'ID', colStyle: {'width': '70px'},},
      {field: 'IMEI', title: 'IMEI号', colStyle: {'width': '135px'}},
      {field: 'SN', title: 'SN号', colStyle: {'width': '260px'}},
      {field: 'ZhiDan', title: '制单号', colStyle: {'width': '135px'}},
      {field: 'TestTime', title: '测试时间', colStyle: {'width': '175px'}},
      {field: 'SoftModel', title: '机型', colStyle: {'width': '120px'}},
      {field: 'Version', title: '软件版本', colStyle: {'width': '200px'}},
      {field: 'Result', title: '结果', colStyle: {'width': '50px'}, formatter(row, column, cellValue, index) {
          switch (cellValue) {
            case 0:
              return '失败';
            case 1:
              return '成功';
          }
        }},
      {field: 'TesterId', title: '测试员', colStyle: {'width': '70px'}},
      {field: 'Computer', title: '地址', colStyle: {'width': '160px'}},
      // {field: 'TestSetting', title: '测试配置', colStyle: {'width': '400px'}},
      {field: 'Remark', title: '备注', colStyle: {'width': '50px'}}
    ]
  },
  {
    title: 'DataRelativeSheet',
    columns: [
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
      // {field: 'IMEI11', title: 'IMEI11', colStyle: {'width': '120px'}, visible: false},
      // {field: 'IMEI12', title: 'IMEI12', colStyle: {'width': '120px'}, visible: false},
      {field: 'TestTime', title: '测试时间', colStyle: {'width': '180px'}},
    ]
  },
  {
    title: 'Gps_TestResult',
    columns: [
      {field: 'Id', title: 'ID', colStyle: {'width': '100px'}},
      {field: 'SN', title: 'SN', colStyle: {'width': '100px'}},
      {field: 'IMEI', title: 'IMEI', colStyle: {'width': '100px'}},
      {field: 'ZhiDan', title: '制单号', colStyle: {'width': '100px'}},
      {field: 'RecordTime', title: '记录时间', colStyle: {'width': '100px'}},
      {field: 'SoftModel', title: '软件型号', colStyle: {'width': '80px'}},
      {field: 'Version', title: '版本号', colStyle: {'width': '80px'}},
      {field: 'FunctionResult', title: '功能测试', colStyle: {'width': '80px'}},
      {field: 'GPSResult', title: 'GPS测试', colStyle: {'width': '80px'}, formatter(row, column, cellValue, index) {
          switch (cellValue) {
            case 0:
              return '失败';
            case 1:
              return '成功';
          }
        }},
      {field: 'CoupleResult', title: '耦合测试', colStyle: {'width': '80px'}},
      {field: 'WriteIMEIResult', title: 'IMEI写入', colStyle: {'width': '80px'}},
      {field: 'ParamDownloadResult', title: '参数下载', colStyle: {'width': '80px'}},
      {field: 'AutoTestResult', title: '自动测试', colStyle: {'width': '80px'}},
      {field: 'AutoTestSMTResult', title: 'SMT自动测试', colStyle: {'width': '80px'}},
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
    ]
  },
  {
    title: 'Gps_CoupleTest_Result',
    columns: [
      {field: 'Id', title: 'ID', colStyle: {'width': '70px'},},
      {field: 'IMEI', title: 'IMEI号', colStyle: {'width': '135px'}},
      {field: 'SN', title: 'SN号', colStyle: {'width': '260px'}},
      {field: 'ZhiDan', title: '制单号', colStyle: {'width': '135px'}},
      {field: 'TestTime', title: '测试时间', colStyle: {'width': '175px'}},
      {field: 'SoftModel', title: '机型', colStyle: {'width': '120px'}},
      {field: 'Version', title: '软件版本', colStyle: {'width': '200px'}},
      {field: 'Result', title: '结果', colStyle: {'width': '50px'}, formatter(row, column, cellValue, index) {
          switch (cellValue) {
            case 0:
              return '失败';
            case 1:
              return '成功';
          }
        }},
      {field: 'TesterId', title: '测试员', colStyle: {'width': '70px'}},
      {field: 'Computer', title: '地址', colStyle: {'width': '160px'}},
      // {field: 'TestSetting', title: '测试配置', colStyle: {'width': '400px'}},
      {field: 'Remark', title: '备注', colStyle: {'width': '50px'}}
    ]
  },
  {
    title: 'DataRelativeUnique',
    columns: [
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
  {
    title: 'Gps_ManuSimDataParam',
    columns: [
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
  {
    title: 'Gps_ManuPrintParam',
    columns: [
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
      {field: 'SoftModel', title: '软件型号', colStyle: {'width': '100px'}},
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
  {
    title: 'Gps_ManuCpParam',
    columns: [
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
  {
    title: 'Gps_CartonBoxTwenty_Result',
    columns: [
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
  {
    title: 'NetMarkIMEI',
    columns: [
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
  }


];
