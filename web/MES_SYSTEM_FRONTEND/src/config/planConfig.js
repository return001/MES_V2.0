/*排产计划订单相关*/
export const orderQueryOptions = [
  {
    type: 'select',
    label: '状态',
    key: 'orderStatus',
    list: [
      {
        value: 1,
        label: '未排产'
      },
      {
        value: 2,
        label: '已排产'
      },
      {
        value: 3,
        label: '已完成'
      },
      {
        value: 4,
        label: '已作废'
      },
    ]
  },
  {
    type: 'text',
    label: '订单号',
    key: 'zhidan'
  },
  {
    type: 'text',
    label: '替换号',
    key: 'alias'
  },
  {
    type: 'text',
    label: '机型',
    key: 'softModel'
  },
  {
    type: 'text',
    label: '成品编码',
    key: 'productNo'
  },
  {
    type: 'text',
    label: '客户编号',
    key: 'customerNumber'
  },
  {
    type: 'text',
    label: '客户名称',
    key: 'customerName'
  },
  {
    type: 'timeRange',
    label: '订单日期',
    key: 'orderDate'
  },
  {
    type: 'timeRange',
    label: '交货日期',
    key: 'deliveryDate'
  }
];


export const orderEditOptions = [
  {
    type: 'text',
    label: '订单号',
    key: 'zhidan'
  },
  {
    type: 'text',
    label: '内部替换号',
    key: 'alias'
  },
  {
    type: 'text',
    label: '机型',
    key: 'softModel'
  },
  {
    type: 'text',
    label: '版本',
    key: 'version'
  },
  {
    type: 'text',
    label: '成品编码',
    key: 'productNo'
  },
  {
    type: 'text',
    label: '客户编号',
    key: 'customerNumber'
  },
  {
    type: 'text',
    label: '客户名称',
    key: 'customerName'
  },
  {
    type: 'text',
    label: '订单数量',
    key: 'quantity'
  },
  {
    type: 'date',
    label: '订单日期',
    key: 'orderDate'
  },
  {
    type: 'date',
    label: '交货日期',
    key: 'deliveryDate',
  },
  {
    type: 'textArea',
    label: '备注',
    key: 'remark'
  }
];

export const orderEditOptionsRules = {
  'zhidan': [
    {required: true, message: '请输入制单号', trigger: 'blur'}
  ],
  'softModel': [
    {required: true, message: '请输入机型号', trigger: 'blur'}
  ],
  'version': [
    {required: true, message: '请输入版本号', trigger: 'blur'}
  ],
  'productNo': [
    {required: true, message: '请输入成品编码', trigger: 'blur'}
  ],
  'customerNumber': [
    {required: true, message: '请输入客户编号', trigger: 'blur'}
  ],
  'customerName': [
    {required: true, message: '请输入客户名称', trigger: 'blur'}
  ],
  'quantity': [
    {required: true, message: '请输入订单数量', trigger: 'blur'},
    {pattern: /^[1-9]\d*$/, message: '请输入正整数', trigger: 'blur'}
  ],
  'orderDate': [
    {required: true, message: '请输入订单日期', trigger: 'blur'}
  ],
  'deliveryDate': [
    {required: true, message: '请输入交货日期', trigger: 'blur'}
  ],
};

export const orderTableColumns = [
  {
    'label': '订单状态',
    'key': 'statusName',
    'min-width': '80px'
  },
  {
    'label': '订单号',
    'key': 'zhidan',
    'min-width': '120px'
  },
  {
    'label': '内部替换号',
    'key': 'alias',
    'min-width': '80px'
  },
  {
    'label': '机型',
    'key': 'softModel',
    'min-width': '100px'
  },
  {
    'label': '版本号',
    'key': 'version',
    'min-width': '80px'
  },
  {
    'label': '成品编码',
    'key': 'productNo',
    'min-width': '120px'
  },
  {
    'label': '客户编号',
    'key': 'customerNumber',
    'min-width': '80px',
    'notShowTo': 'operator'
  },
  {
    'label': '客户名称',
    'key': 'customerName',
    'min-width': '120px',
    'notShowTo': 'operator'
  },
  {
    'label': '订单日期',
    'key': 'orderDate',
    'min-width': '120px'
  },
  {
    'label': '订单数量',
    'key': 'quantity',
    'min-width': '80px'
  },
  {
    'label': '交货日期',
    'key': 'deliveryDate',
    'min-width': '120px'
  },
  {
    'label': '备注',
    'key': 'remark',
    'min-width': '80px'
  },
  {
    'label': '删除原因',
    'key': 'deleteReason',
    'min-width': '180px'
  },
];
export const orderUnscheduledTableColumns = [
  {
    'label': '订单号',
    'key': 'zhidan',
    'min-width': '120px'
  },
  {
    'label': '订单数量',
    'key': 'quantity',
    'min-width': '80px'
  },
  {
    'label': '未排产数量',
    'key': 'unscheduledQuantity',
    'min-width': '80px'
  },
  {
    'label': '产能',
    'key': 'capacity',
    'min-width': '80px'
  },
  {
    'label': '内部替换号',
    'key': 'alias',
    'min-width': '80px'
  },
  {
    'label': '机型',
    'key': 'softModel',
    'min-width': '100px'
  },
  {
    'label': '版本号',
    'key': 'version',
    'min-width': '80px'
  },
  {
    'label': '客户编号',
    'key': 'customerNumber',
    'min-width': '80px'
  },
  {
    'label': '客户名称',
    'key': 'customerName',
    'min-width': '120px'
  },
  {
    'label': '订单日期',
    'key': 'orderDate',
    'min-width': '120px'
  },
  {
    'label': '交货日期',
    'key': 'deliveryDate',
    'min-width': '120px'
  },
];
export const orderDetailsTableColumns = [
  {
    'label': '类型',
    'key': 'typeName',
    'min-width': '60px'
  },
  {
    'label': '文件名',
    'key': 'fileName',
    'min-width': '200px'
  },
  {
    'label': '上传人',
    'key': 'uploaderName',
    'min-width': '80px'
  },
  {
    'label': '上传时间',
    'key': 'uploadTime',
    'min-width': '110px'
  }
];


/*产线设置*/
export const lineQueryOptions = [
  {
    type: 'select',
    async: true,
    label: '工序组',
    key: 'processGroup'
  },
  {
    type: 'text',
    label: '线别',
    key: 'lineNo'
  },
  {
    type: 'text',
    label: '产线名称',
    key: 'lineName'
  },
];

export const lineEditOptions = [
  {
    type: 'text',
    label: '线别',
    key: 'lineNo'
  },
  {
    type: 'text',
    label: '产线名称',
    key: 'lineName'
  },
  {
    type: 'asyncSelect',
    label: '负责人',
    key: 'lineDirector',
    condition: 'directorName'
  },
  {
    type: 'asyncSelect',
    label: '产线工程',
    key: 'lineEngineer',
    condition: 'engineerName'
  },
  {
    type: 'asyncSelect',
    label: '产线品质',
    key: 'lineQc',
    condition: 'qcName'
  },
  {
    type: 'text',
    label: '产线备注',
    key: 'lineRemark'
  }
];

export const lineEditOptionsRules = {
  'processGroup': [{required: true, message: '请选择工序组', trigger: 'change'}],
  'lineNo': [{required: true, message: '请输入线别', trigger: 'blur'}],
  'lineName': [{required: true, message: '请输入产线名称', trigger: 'blur'}],
  'lineDirector': [{required: true, message: '请输入并选择负责人', trigger: 'blur'}],
  'lineEngineer': [{required: true, message: '请输入并选择产线工程', trigger: 'blur'}],
  'lineQc': [{required: true, message: '请输入并选择产线品质', trigger: 'blur'}],
};


export const lineTableColumns = [
  {
    'label': '工序组',
    'key': 'groupName',
    'min-width': '80px'
  },
  {
    'label': '线别',
    'key': 'lineNo',
    'min-width': '60px'
  },
  {
    'label': '产线名称',
    'key': 'lineName',
    'min-width': '100px'
  },
  {
    'label': '负责人',
    'key': 'directorName',
    'min-width': '100px'
  },
  {
    'label': '产线工程',
    'key': 'engineerName',
    'min-width': '100px'
  },
  {
    'label': '产线品质',
    'key': 'qcName',
    'min-width': '100px'
  },
  {
    'label': '备注',
    'key': 'lineRemark',
    'min-width': '120px'
  },
];

/*产能设置*/
export const capacityQueryOptions = [
  {
    type: 'text',
    label: '机型',
    key: 'softModel'
  },
  {
    type: 'text',
    label: '客户型号',
    key: 'customerModel'
  },
  /*{
    type: 'select',
    label: '工序',
    async: true,
    key: 'process'
  },*/
];

export const capacityEditOptions = [
  {
    type: 'text',
    label: '机型',
    key: 'softModel'
  },
  {
    type: 'text',
    label: '人数',
    key: 'processPeopleQuantity'
  },
  {
    type: 'text',
    label: '产能',
    key: 'capacity'
  },
  {
    type: 'text',
    label: '客户型号',
    key: 'customerModel'
  },
  {
    type: 'text',
    label: '节拍',
    key: 'rhythm'
  },
  {
    type: 'text',
    label: '备注',
    key: 'remark'
  },
];

export const capacityEditOptionsRules = {
  'processGroup': [{required: true, message: '请选择工序组', trigger: 'change'}],
  'process': [{required: true, message: '请选择工序', trigger: 'change'}],
  'softModel': [{required: true, message: '请填写机型', trigger: 'blur'}],
  // 'customerModel': [{required: true, message: '请填写客户型号', trigger: 'blur'}],
  'rhythm': [{required: true, message: '请填写节拍', trigger: 'blur'}],
  'processPeopleQuantity': [{required: true, message: '请填写人数', trigger: 'blur'},
    {pattern: /^[1-9]\d*$/, message: '请输入正整数', trigger: 'blur'}],
  'capacity': [{required: true, message: '请填写产能', trigger: 'blur'}],
};

export const capacityTableColumns = [
  {
    'label': '机型',
    'key': 'softModel',
    'min-width': '100px'
  },
  {
    'label': '客户型号',
    'key': 'customerModel',
    'min-width': '100px'
  },
  {
    'label': '节拍',
    'key': 'rhythm',
    'min-width': '80px'
  },
  {
    'label': '工序组',
    'key': 'groupName',
    'min-width': '80px'
  },
  {
    'label': '工序',
    'key': 'processName',
    'min-width': '100px'
  },
  {
    'label': '人数',
    'key': 'processPeopleQuantity',
    'min-width': '60px'
  },
  {
    'label': '产能(PCS/H)',
    'key': 'capacity',
    'min-width': '60px'
  },
  {
    'label': '备注',
    'key': 'remark',
    'min-width': '100px'
  },
];

/*工序组信息*/
export const processGroupQueryOptions = [
  {
    type: 'text',
    label: '工序组编号',
    key: 'groupNo'
  },
  {
    type: 'text',
    label: '工序组名称',
    key: 'groupName'
  },
];

export const processGroupEditOptions = [
  {
    type: 'text',
    label: '工序组编号',
    key: 'groupNo'
  },
  {
    type: 'text',
    label: '工序组名称',
    key: 'groupName'
  },
  {
    type: 'text',
    label: '备注',
    key: 'groupRemark'
  },
];

export const processGroupEditOptionsRules = {
  'groupNo': [{required: true, message: '请填写工序组编号', trigger: 'blur'}],
  'groupName': [{required: true, message: '请填写工序组名称', trigger: 'blur'}],
};

export const processGroupTableColumns = [
  {
    'label': '工序组编号',
    'key': 'groupNo',
    'min-width': '100px'
  },
  {
    'label': '工序组名称',
    'key': 'groupName',
    'min-width': '100px'
  },
  {
    'label': '备注',
    'key': 'groupRemark',
    'min-width': '100px'
  },
];

/*工序信息*/
export const processQueryOptions = [
  {
    type: 'text',
    label: '工序编号',
    key: 'processNo'
  },
  {
    type: 'text',
    label: '工序名称',
    key: 'processName'
  },
];

export const processEditOptions = [
  {
    type: 'text',
    label: '工序编号',
    key: 'processNo'
  },
  {
    type: 'text',
    label: '工序名称',
    key: 'processName'
  },
  {
    type: 'text',
    label: '备注',
    key: 'processRemark'
  },
];

export const processEditOptionsRules = {
  'processGroup': [{required: true, message: '请选择工序组', trigger: 'change'}],
  'processNo': [{required: true, message: '请填写工序编号', trigger: 'blur'}],
  'processName': [{required: true, message: '请填写工序名称', trigger: 'blur'}],
};

export const processTableColumns = [
  {
    'label': '工序组',
    'key': 'groupName',
    'min-width': '100px'
  },
  {
    'label': '工序编号',
    'key': 'processNo',
    'min-width': '100px'
  },
  {
    'label': '工序名称',
    'key': 'processName',
    'min-width': '100px'
  },
  {
    'label': '备注',
    'key': 'processRemark',
    'min-width': '100px'
  },
];

/*排产计划*/
export const planQueryOptions = [
  {
    type: 'select',
    label: '状态',
    key: 'schedulingPlanStatus',
    list: [
      {
        value: 1,
        label: '已排产'
      },
      {
        value: 2,
        label: '进行中'
      },
      {
        value: 3,
        label: '已完成'
      },
      {
        value: 4,
        label: '待通知'
      },
    ]
  },
  {
    type: 'text',
    label: '订单号',
    key: 'zhidan'
  },
  // {
  //   type: 'select',
  //   async: true,
  //   label: '工序组',
  //   key: 'processGroupName',
  //   url: ''
  // },
  // {
  //   type: 'select',
  //   async: true,
  //   label: '产线',
  //   key: 'line',
  //   url: ''
  // },
  {
    type: 'text',
    label: '生产计划单号',
    key: 'productionPlanningNumber'
  },
  {
    type: 'text',
    label: '机型',
    key: 'softModel'
  },
  {
    type: 'timeRange',
    label: '预计开始时间',
    key: 'planStartTime'
  },
  {
    type: 'timeRange',
    label: '预计完成时间',
    key: 'planCompleteTime'
  },
  {
    type: 'timeRange',
    label: '开始时间',
    key: 'startTime'
  },
  {
    type: 'timeRange',
    label: '完成时间',
    key: 'completeTime'
  },
];

import eventBus from '../utils/eventBus'

export const planTableColumns = [
  // {
  //   'label': '工序组',
  //   'key': 'groupName',
  //   'min-width': '80px'
  // },
  {
    'label': '产线',
    'key': 'lineName',
    'min-width': '100px'
  },
  {
    'label': '状态',
    'key': 'statusName',
    'min-width': '80px'
  },
  {
    'label': '订单号',
    'key': 'zhidan',
    'min-width': '120px'
  },
  /*{
    'label': '是否超时',
    'key': 'isTimeout',
    'min-width': '60',
    formatter (row, column, cellValue, index) {
      if (cellValue) {
        //eventBus.$emit('setTimeoutHighlight', column.id);
        return '是'
      } else {
        return '否'
      }
    }
  },*/
  {
    'label': '是否紧急',
    'key': 'isUrgent',
    'min-width': '80px',
    formatter (row, column, cellValue, index) {
      if (cellValue) {
        return '是'
      } else {
        return '否'
      }
    }
  },
  {
    'label': '排产数量',
    'key': 'schedulingQuantity',
    'min-width': '80px'
  },
  {
    'label': '产能(PCS/H)',
    'key': 'capacity',
    'min-width': '70px'
  },
  {
    'label': '生产计划单号',
    'key': 'productionPlanningNumber',
    'min-width': '100px'
  },
  {
    'label': '预计开始时间',
    'key': 'planStartTime',
    'min-width': '170px'
  },
  {
    'label': '预计结束时间',
    'key': 'planCompleteTime',
    'min-width': '170px'
  },
  {
    'label': '开始时间',
    'key': 'startTime',
    'min-width': '170px'
  },
  {
    'label': '结束时间',
    'key': 'completeTime',
    'min-width': '170px'
  },
  {
    'label': '实际完成数量',
    'key': 'producedQuantity',
    'min-width': '80px'
  },
  {
    'label': '未完成量',
    'key': 'remainingQuantity',
    'min-width': '80px'
  },
  {
    'label': '未完成原因',
    'key': 'remainingReason',
    'min-width': '120px'
  },
  {
    'label': '备注',
    'key': 'remark',
    'min-width': '120px'
  },
];

export const planTableExtraOrderColumns = [
  {
    'label': '订单号',
    'key': 'zhidan',
    'min-width': '100px'
  },
  {
    'label': '机型',
    'key': 'softModel',
    'min-width': '100px'
  },
  {
    'label': '订单数量',
    'key': 'quantity',
    'min-width': '80px'
  },
  {
    'label': '内部替换号',
    'key': 'alias',
    'min-width': '100px'
  },
  {
    'label': '客户编号',
    'key': 'customerNumber',
    'min-width': '80px'
  },
  {
    'label': '订单日期',
    'key': 'orderDate',
    'min-width': '100px'
  },
  {
    'label': '交货日期',
    'key': 'deliveryDate',
    'min-width': '100px'
  },
  {
    'label': '成品编码',
    'key': 'productNo',
    'min-width': '100px'
  },
  {
    'label': '客户名称',
    'key': 'customerName',
    'min-width': '100px'
  },
];
export const planTableExtraUserColumns = [
  {
    'label': '排产人',
    'key': 'schedulerName',
    'min-width': '100px'
  },
  {
    'label': '排产时间',
    'key': 'schedulingTime',
    'min-width': '100px'
  },
  {
    'label': '修改人',
    'key': 'modifierName',
    'min-width': '100px'
  },
  {
    'label': '修改时间',
    'key': 'planModifyTime',
    'min-width': '100px'
  },
  {
    'label': '转线时间',
    'key': 'lineChangeTime',
    'min-width': '80px'
  },
  {
    'label': '生产确认人',
    'key': 'confirmerName',
    'min-width': '100px'
  },
];
