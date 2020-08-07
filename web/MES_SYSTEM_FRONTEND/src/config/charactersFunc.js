/**
 **@description: 生成指定长度与填充值的数组
 **@date: 2020/4/27 14:04
 **@author: DarkNin
 **@method: generateArray
 **@params: 长度, 填充值
 */

const generateArray = function(length, fillValue, fillPos) {
  let tempArray = new Array(length).fill(undefined);
  for (let i in fillPos) {
    tempArray[fillPos[i]] = fillValue
  }
  return tempArray
};


/**
 **@description: 功能名字匹配表
 **@date: 2020/4/27 14:03
 **@author: DarkNin
 */
const funcNameMap = {
  '基础': 'basic',
  '排产': 'plan',
  '工单': 'work_order',
  '产量': 'capacity',
  '配置': 'test',
  'ESOP': 'esop',
  '功能': 'func',
  '查询': 'table',
  '用户': 'user'
};

/**
 **@description: 子模块匹配表
 **@date: 2020/4/27 18:05
 **@author: DarkNin
 **@method:
 **@params:
 */

const moduleNameMap = {
  '数据表': 'table',
  '多表查询': 'multi',
  '备份': 'backup',
  '工单': 'work_order',
  'SMT测试': 'smt_test',
  '组装测试': 'zz_test',
  '研发测试': 'yf_test',
  'OQC': 'oqc_test',
  '测试日志':'test_log',
  '排产计划': 'plan',
  '订单': 'order',
  'MAC返工': 'mac',
  'IMEI查询': 'imei',
  '文件发放': 'file',
  '客户端信息': 'client',
  '红茶': 'redtea',
  '基础配置': 'basic',
  '用户管理': 'user'
};


/**
 **@description: 页面名字匹配表
 **@date: 2020/4/27 15:37
 **@author: DarkNin
 **@method:
 **@params:
 */
const pageNameMap = {
  '厂区设置': 'factory',
  '车间设置': 'workshop',
  '工序组设置': 'process_group',
  '工序设置': 'process',
  '产线设置': 'line',
  '站点设置': 'site_set',
  '客户信息': 'customer',
  '系列机型': 'model',
  '产品型号': 'product',
  '标准产能设置': 'capacity',
  '文件类型管理': 'file_type',
  '订单管理': 'order',
  '排产计划': 'detail',
  '功能测试': 'func',
  '耦合测试': 'couple',
  'OQC': 'oqc',
  '配置测试项日志':'test_item_log',
  '文件管理与发放': 'file',
  '通知管理': 'notice',
  '站点信息': 'site',
  '指派员工': 'assign',
  '操作日志': 'action_log',
  '确认日志': 'confirm_log',
  '通知日志': 'notice_log',
  '日志': 'login_log',
  '产能记录': 'count_log',
  'MAC返工': 'mac',
  'IMEI查询': 'imei',
  'AMS数据关联表': 'DataRelativeSheet',
  // '数据关联表(工厂)': 'DataRelativeUnique',
  '终端信息附表(工厂)': 'DataRelativeUnique',
  '终端信息附表(关联更新)': 'DataRelativeUpdate',//终端信息附表(关联更新)
  '组装功能表': 'Gps_AutoTest_Result',
  'SMT功能表': 'Gps_AutoTest_Result2',
  '老化后功能表': 'Gps_AutoTest_Result3',
  '卡通箱表': 'Gps_CartonBoxTwenty_Result',
  '耦合表': 'Gps_CoupleTest_Result',
  '比对表': 'Gps_ManuCpParam',
  '机身彩盒打印记录表': 'Gps_ManuPrintParam',
  '网标与IMEI绑定表': 'NetMarkIMEI',
  'OQC测试结果': 'GPSOQC_Result',
  '种子物联网卡表': 'Gps_ManuSimDataParam',
  '错误日志': 'LTestLogMessage',
  '总表': 'Gps_TestResult',
  '多表查询': 'multi',
  '备份': 'backups',
  '部门信息设置': 'dept',
  '用户类型设置': 'char',
  '用户信息': 'users',
  '工单': 'work_order',
  '产量录入': 'capacity',
  '红茶': 'redtea',
};


/**
 **@description: 权限对应哈希表数组中位置
 **@date: 2020/4/27 19:35
 **@author: DarkNin
 **@method:
 **@params:
 */
const permissionIndexMap = {
  '查询': 0,
  '新增': 1,
  '修改': 2,
  '删除': 3,
  '上传': 4,
  '下载': 5,
  '审核': 6,
  '其他': 7
};
const reversePermissionIndexMap = {
  '0': '查询',
  '1': '新增',
  '2': '修改',
  '3': '删除',
  '4': '上传',
  '5': '下载',
  '6': '审核',
  '7': '其他'
};


/**
 **@description: 获取一个未配置的各模块权限哈希表
 **@date: 2020/4/27 19:33
 **@author: DarkNin
 **@method: getCharactersFuncList
 **@params:
 */
const getCharactersFuncList = function() {
  return {
    'basic': {
      'basic': {
        'factory': generateArray(4, false, [0, 1, 2, 3]),
        'workshop': generateArray(4, false, [0, 1, 2, 3]),
        'process_group': generateArray(4, false, [0, 1, 2, 3]),
        'process': generateArray(4, false, [0, 1, 2, 3]),
        'line': generateArray(4, false, [0, 1, 2, 3]),
        'site_set': generateArray(4, false, [0, 1, 2, 3]),
        'customer': generateArray(4, false, [0, 1, 2, 3]),
        'model': generateArray(4, false, [0, 1, 2, 3]),
        'product': generateArray(4, false, [0, 1, 2, 3]),
        'capacity': generateArray(4, false, [0, 1, 2, 3, 6]),
        'file_type': generateArray(4, false, [0, 1, 2, 3]),
      },
    },
    'plan': {
      'plan': {
        'detail': generateArray(8, false, [0, 1, 2, 3, 5, 7]),
        'action_log': generateArray(1, false, [0]),
      },
      'order': {
        'order': generateArray(7, false, [0, 1, 2, 3, 4, 5, 6, 7]),
      }
    },
    'esop': {
      'file': {
        'file': generateArray(8, false, [0, 1, 3, 4, 5, 6, 7]),
        'notice': generateArray(8, false, [0, 1, 2, 3, 7]),
        'site': generateArray(8, false, [0, 6, 7]),
        'assign': generateArray(3, false, [0, 2]),
        'action_log': generateArray(1, false, [0]),
        'confirm_log': generateArray(1, false, [0]),
        'notice_log': generateArray(1, false, [0]),
      },
      'client': {
        'login_log': generateArray(1, false, [0]),
        'count_log': generateArray(1, false, [0])
      }
    },
    'work_order': {
      'work_order': {
        'work_order': generateArray(7, false, [0, 1, 2, 6])
      }
    },
    'test': {
      'smt_test': {
        'func': generateArray(4, false, [0, 1, 2, 3])
      },
      'zz_test': {
        'func': generateArray(4, false, [0, 1, 2, 3]),
        'couple': generateArray(4, false, [0, 1, 2, 3])
      },
      'yf_test': {
        'func': generateArray(4, false, [0, 1, 2, 3, 4, 5]),
        'couple': generateArray(4, false, [0, 1, 2, 3, 4, 5])
      },
      'oqc_test': {
        'oqc': generateArray(4, false, [0, 1, 2, 3])
      },
      'test_log':{
        'test_item_log': generateArray(4, false, [0])
      }
    },
    'table': {
      'table': {
        'DataRelativeSheet': generateArray(6, false, [0, 3, 5]),
        'DataRelativeUnique': generateArray(6, false, [0, 3, 5]),
        'DataRelativeUpdate': generateArray(6, false, [0, 3, 5]),
        'Gps_AutoTest_Result': generateArray(6, false, [0, 3, 5]),
        'Gps_AutoTest_Result2': generateArray(6, false, [0, 3, 5]),
        'Gps_AutoTest_Result3': generateArray(6, false, [0, 3, 5]),
        'Gps_CartonBoxTwenty_Result': generateArray(6, false, [0, 3, 5]),
        'Gps_CoupleTest_Result': generateArray(6, false, [0, 3, 5]),
        'Gps_ManuCpParam': generateArray(6, false, [0, 3, 5]),
        'Gps_ManuPrintParam': generateArray(6, false, [0, 3, 5]),
        'NetMarkIMEI': generateArray(6, false, [0, 3, 5]),
        'GPSOQC_Result': generateArray(6, false, [0, 3, 5]),
        'Gps_ManuSimDataParam': generateArray(6, false, [0, 3, 5]),
        'LTestLogMessage': generateArray(6, false, [0, 3, 5]),
        'Gps_TestResult': generateArray(6, false, [0, 3, 5]),
      },
      'multi': {
        'multi': generateArray(6, false, [0, 3, 5]),
      },
      'backup': {
        'backups': generateArray(6, false, [0, 3, 5])
      }
    },
    'func': {
      'mac': {
        'mac': generateArray(3, false, [0, 2])
      },
      'imei': {
        'imei': generateArray(6, false, [0, 5])
      },
      'redtea': {
        'redtea': generateArray(1, false, [0])
      }
    },
    'user': {
      'user': {
        'dept': generateArray(4, false, [0, 1, 2, 3]),
        'char': generateArray(8, false, [0, 1, 2, 3, 7]),
        'users': generateArray(8, false, [0, 1, 2, 3, 7]),
        // 'action_log': generateArray(8, false),
      }
    }
  }
};


/**
 **@description: 根据后端登录接口返回的数据初始化权限哈希表
 **@date: 2020/4/30 16:54
 **@author: DarkNin
 **@method: initCharactersFuncMap
 **@return: {map:Object, pageList:Array, moduleList: Array}
 **@params: srcMap, authorities
 */

const initCharactersFuncMap = function(srcMap, authorities) {
  let Obj = {
    map: srcMap,
    pageList: undefined,
    moduleList: undefined
  };
  /*新建Set结构数据*/
  try {
    let pageList = new Set();
    let moduleList = new Set();
    authorities.forEach(item => {
      /*根据哈希表匹配到具体功能项*/
      // if (!funcNameMap[item.system] || !moduleNameMap[item.model] || !pageNameMap[item.page]) return;
      Obj.map[funcNameMap[item.system]][moduleNameMap[item.model]][pageNameMap[item.page]][permissionIndexMap[item.operation]] = true;
      /*建立以"模块-类别-页面"组成的列表*/
      pageList.add(`${funcNameMap[item.system]}-${moduleNameMap[item.model]}-${pageNameMap[item.page]}`);
      /*同时将一级路由页面插入列表中以便匹配*/
      pageList.add(`${funcNameMap[item.system]}`);
      /*主模块列表*/
      moduleList.add(`${funcNameMap[item.system]}`);
    });
    Obj.pageList = [...pageList];
    Obj.moduleList = [...moduleList];

  } catch (e) {
    throw new Error(e)
  }
  return Obj;
};

/**
 **@description: 根据路径返回规定的页面索引
 **@date: 2020/5/5 2:48
 **@author: DarkNin
 **@method: getModuleIndex
 **@params: path: String
 **@return: _page: String
 */
const getModuleIndex = function(path, query) {
  const pathGroup = path.split('/');
  let _moduleMap, _pageMap, _page;
  const _routerMap = {
    'table': 'table',
    'test': 'test',
    'func': 'func',
    'esop': 'esop',
    'setting': 'basic',
    'plan': 'plan',
    'user': 'user',
    'work_order':'work_order',
  };
  /*当路由未到达叶子路由时将返回值设置为 ‘模块’，排除目前存在的两种单层路由页面的情况 */
  if (pathGroup.length === 2 && pathGroup[1] !== 'order' && pathGroup[1] !== 'redtea') {
    _page = `${_routerMap[pathGroup[1]]}`;
  } else {
    /*访问到具体页面的情况*/
    switch (pathGroup[1]) {
      case 'table':
        _moduleMap = {
          'details': 'table',
          'details_sp': 'table',
          'multi': 'multi',
          'backups': 'backup'
        };
        if (pathGroup[2] === 'multi' || pathGroup[2] === 'backups') {
          _page = `table-${_moduleMap[pathGroup[2]]}-${_moduleMap[pathGroup[2]]}`;
        } else {
          _page = `table-${_moduleMap[pathGroup[2]]}-${query.type}`;
        }
        break;
      case 'order':
        _page = `order-order-order`;
        break;
      case 'test':
        _moduleMap = {
          0: 'smt_test',
          1: 'zz_test',
          2: 'zz_test',
          3: 'yf_test',
          4: 'yf_test',
          5: 'oqc_test',
          6: 'test_log'
        };
        _pageMap = {
          0: 'func',
          1: 'func',
          2: 'couple',
          3: 'func',
          4: 'couple',
          5: 'oqc',
          6: 'test_item_log'
        };
        _page = `test-${_moduleMap[query.type]}-${_pageMap[query.type]}`;
        break;
      case 'func':
        _page = `func-${pathGroup[2]}-${pathGroup[2]}`;
        break;
      case 'esop':
        _moduleMap = {
          'login_log': 'client',
          'count_log': 'client'
        };
        _page = `esop-${_moduleMap[pathGroup[2]] || 'file'}-${pathGroup[2]}`;
        break;
      case 'setting':
        _page = `basic-basic-${pathGroup[2]}`;
        break;
      case 'plan':
        _moduleMap = {
          'detail': 'plan',
          'action_log': 'plan',
          'order': 'order',
        };
        _page = `plan-${_moduleMap[pathGroup[2]]}-${pathGroup[2]}`;
        break;
      case 'user':
        _page = `user-user-${pathGroup[2]}`;
        break;
    }
  }
  return _page;
};


/**
 **@description: 路由拦截方法，根据完整跳转路径匹配拦截规则
 **@date: 2020/5/2 19:16
 **@author: DarkNin
 **@method: getModulePermission
 **@return: Boolean()
 **@params: path: String, query:Object, charactersFuncMap:Object
 */

const getModulePermission = function(path, query, charactersFuncMapPageList) {
  let page = getModuleIndex(path, query);
  if (path === '/editPassword') {
    return true;
  } else {
    return charactersFuncMapPageList.indexOf(page) >= 0;
  }
};

/**
 **@description: 根据当前路由路径返回以 模块、类别、页面组成的数组
 **@date: 2020/5/5 2:59
 **@author: DarkNin
 **@method: getModuleIndexArray
 **@params: path, query
 **@return: moduleIndexArray: Array
 */
const getModuleIndexArray = function(path, query) {
  return getModuleIndex(path, query).split('-')
};

/**
 **@description: 根据输入的操作类型与权限列表返回是否具备权限
 **@date: 2020/5/5 3:38
 **@author: DarkNin
 **@method: getFunctionPermission
 **@params: path: String, query: Object, functionType: Number, funcMap: Object
 **@return: Boolean
 */
const getFunctionPermission = function(path, query, functionType, funcMap) {
  let indexArray = getModuleIndexArray(path, query);
  return Boolean(funcMap[indexArray[0]][indexArray[1]][indexArray[2]][functionType])
};

export {
  getCharactersFuncList,
  moduleNameMap,
  funcNameMap,
  pageNameMap,
  initCharactersFuncMap,
  getModulePermission,
  permissionIndexMap,
  reversePermissionIndexMap,
  getFunctionPermission
};
