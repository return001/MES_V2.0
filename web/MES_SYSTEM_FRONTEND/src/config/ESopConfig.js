/*查询配置*/
export const FileQueryConfig = [
  {
    type: 'select',
    label: '状态',
    size: 'small',
    selectList: [
      {key: '待审核', label: '待审核'},
      {key: '已审核', label: '已审核'},
      {key: '播放中', label: '播放中'},
      {key: '已作废', label: '已作废'},
    ],
    prop: 'state',
  },
  {
    type: 'text',
    label: '文件编号',
    size: 'small',
    prop: 'fileNumber',
  },
  {
    type: 'text',
    label: '文件名',
    size: 'small',
    prop: 'fileName',
  },
  {
    type: 'text',
    label: '版本号',
    size: 'small',
    prop: 'version',
  },
  {
    type: 'text',
    label: '客户名',
    size: 'small',
    prop: 'customer',
  },
  {
    type: 'text',
    label: '系列型号',
    size: 'small',
    prop: 'seriesModel',
  },
  {
    type: 'text',
    label: '产品型号',
    size: 'small',
    prop: 'productModel',
  },
  {
    type: 'text',
    label: '审核人',
    size: 'small',
    prop: 'reviewer',
  },
  {
    type: 'date-range',
    label: '审核时间',
    size: 'small',
    pickerList: [
      {
        label: '最近一个月',
        duringTime: 3600 * 24 * 1000 * 30,
      }
    ],
    prop: 'reviewTime',
  }
];
export const NoticeQueryConfig = [
  {
    type: 'text',
    label: '标题',
    size: 'small',
    prop: 'title',
  },
  {
    type: 'text',
    label: '内容',
    size: 'small',
    prop: 'content',
  },
  {
    type: 'date-range',
    label: '开始播放时间',
    size: 'small',
    pickerList: [
      {
        label: '最近一个月',
        duringTime: 3600 * 24 * 1000 * 30,
      }
    ],
    prop: 'startTime',
  },
  {
    type: 'date-range',
    label: '结束播放时间',
    size: 'small',
    pickerList: [
      {
        label: '最近一个月',
        duringTime: 3600 * 24 * 1000 * 30,
      }
    ],
    prop: 'endTime',
  },
];

export const ActionLogQueryConfig = [
  {
    type: 'text',
    label: '用户',
    size: 'small',
    prop: 'uid',
  },
  {
    type: 'text',
    label: '结果码',
    size: 'small',
    prop: 'resultCode',
  },
  {
    type: 'date-range',
    label: '操作时间',
    size: 'small',
    pickerList: [
      {
        label: '最近一个月',
        duringTime: 3600 * 24 * 1000 * 30,
      }
    ],
    prop: 'time',
  },
];

export const ClientLogQueryConfig = [
  {
    type: 'text',
    label: '用户',
    size: 'small',
    prop: 'userName',
  },
  {
    type: 'text',
    label: '站点编号',
    size: 'small',
    prop: 'siteNumber',
  },
  {
    type: 'date-range',
    label: '操作时间',
    size: 'small',
    pickerList: [
      {
        label: '最近一个月',
        duringTime: 3600 * 24 * 1000 * 30,
      }
    ],
    prop: 'time',
  },
];

export const SiteQueryConfig = [
  {
    type: 'text',
    label: '站点编号',
    size: 'small',
    prop: 'siteNumber',
  },
  {
    type: 'text',
    label: '站点名称',
    size: 'small',
    prop: 'siteName',
  },
  {
    type: 'text',
    label: '工序顺序',
    size: 'small',
    prop: 'processOrder',
  },
];
export const FileHistoryQueryConfig = [
  {
    type: 'text',
    label: '发放人',
    size: 'mini',
    prop: 'pushPerson',
  },
  {
    type: 'date-range',
    label: '发放时间',
    size: 'mini',
    pickerList: [
      {
        label: '最近一个月',
        duringTime: 3600 * 24 * 1000 * 30,
      }
    ],
    prop: 'time',
  }
];

export const ConfirmLogQueryConfig = [
  {
    type: 'text',
    label: '确认人',
    size: 'small',
    prop: 'userName',
  },
  {
    type: 'text',
    label: '产线',
    size: 'small',
    prop: 'lineName',
  },
  {
    type: 'text',
    label: '站点编号',
    size: 'small',
    prop: 'siteNumber',
  },
  {
    type: 'select',
    label: '类型',
    size: 'small',
    prop: 'type',
    selectList: [
      {label: '操作员确认成功', key: '操作员确认成功'},
      {label: '操作员确认超时', key: '操作员确认超时'},
      {label: '品质审核通过', key: '品质审核通过'},
      {label: '品质审核不通过', key: '品质审核不通过'},
    ]
  },
  {
    type: 'date-range',
    label: '发放时间',
    size: 'small',
    pickerList: [
      {
        label: '最近一个月',
        duringTime: 3600 * 24 * 1000 * 30,
      }
    ],
    prop: 'time',
  }
];
export const NoticeLogQueryConfig = [
  {
    type: 'text',
    label: '发放人',
    size: 'small',
    prop: 'pushPerson',
  },
  {
    type: 'text',
    label: '产线',
    size: 'small',
    prop: 'line',
  },
  {
    type: 'text',
    label: '通知标题',
    size: 'small',
    prop: 'title',
  },
  {
    type: 'date-range',
    label: '发放时间',
    size: 'small',
    pickerList: [
      {
        label: '最近一个月',
        duringTime: 3600 * 24 * 1000 * 30,
      }
    ],
    prop: 'time',
  }
];


/*表头*/
export const FileTableColumns = [
  {
    'label': '状态',
    'key': 'state',
    'min-width': '80px'
  },
  {
    'label': '文件编号',
    'key': 'fileNumber',
    'min-width': '100px'
  },
  {
    'label': '文件名称',
    'key': 'fileName',
    'min-width': '120px'
  },
  {
    'label': '版本',
    'key': 'version',
    'min-width': '80px'
  },
  // {
  //   'label': '文件路径',
  //   'key': 'path',
  //   'min-width': '120px'
  // },
  {
    'label': '客户',
    'key': 'customer',
    'min-width': '80px'
  },
  {
    'label': '系列机型',
    'key': 'seriesModel',
    'min-width': '80px'
  },
  {
    'label': '产品型号',
    'key': 'productModel',
    'min-width': '80px'
  },
  {
    'label': '历史发放次数',
    'key': 'fileHistoryNumber',
    'min-width': '80px'
  },
  {
    'label': '审核人',
    'key': 'reviewer',
    'min-width': '80px'
  },
  {
    'label': '审核日期',
    'key': 'reviewTime',
    'min-width': '120px'
  },
];

export const NoticeTableColumns = [
  {
    'label': '标题',
    'key': 'title',
    'min-width': '100px'
  },
  {
    'label': '内容',
    'key': 'content',
    'min-width': '200px'
  },
  {
    'label': '开始播放时间',
    'key': 'startTime',
    'min-width': '120px'
  },
  {
    'label': '结束播放时间',
    'key': 'endTime',
    'min-width': '120px'
  },
];

export const ActionLogTableColumns = [
  {
    'label': '用户',
    'key': 'uid',
    'min-width': '80px'
  },
  {
    'label': '操作',
    'key': 'action',
    'min-width': '200px'
  },
  {
    'label': '操作时间',
    'key': 'time',
    'min-width': '120px'
  },
  // {
  //   'label': '结果码',
  //   'key': 'resultCode',
  //   'min-width': '80px',
  // },
];

export const ClientLogTableColumns = [
  {
    'label': '用户',
    'key': 'userName',
    'min-width': '100px'
  },
  {
    'label': '刷脸时间',
    'key': 'time',
    'min-width': '200px'
  },
  {
    'label': '站点编号',
    'key': 'siteNumber',
    'min-width': '120px'
  },
  {
    'label': '类型',
    'key': 'type',
    'min-width': '80px'
  },
];


export const SiteTableColumns = [
 /* {
    'label': 'SOP状态',
    'key': 'state',
    'min-width': '80px'
  },*/
  {
    'label': '站点编号',
    'key': 'siteNumber',
    'min-width': '100px'
  },
  {
    'label': '站点名称',
    'key': 'siteName',
    'min-width': '100px'
  },
  {
    'label': '工序顺序',
    'key': 'processOrder',
    'min-width': '100px'
  },
  {
    'label': '所属产线',
    'key': 'lineName',
    'min-width': '100px'
  },
  {
    'label': '播放次数',
    'key': 'playTimes',
    'min-width': '60px'
  },
  {
    'label': '切换时间(s)',
    'key': 'switchInterval',
    'min-width': '60px'
  }
];

export const FileHistoryTableColumns = [
  {
    'label': '发放人',
    'key': 'pushPerson',
    'min-width': '80px'
  },
  {
    'label': '文件名',
    'key': 'fileName',
    'min-width': '120px'
  },
  {
    'label': '文件编号',
    'key': 'fileNumber',
    'min-width': '100px'
  },
  {
    'label': '发放时间',
    'key': 'pushTime',
    'min-width': '120px'
  },
  {
    'label': '产品型号',
    'key': 'productModel',
    'min-width': '80px'
  },
  {
    'label': '系列型号',
    'key': 'seriesModel',
    'min-width': '80px'
  },
  {
    'label': '版本',
    'key': 'version',
    'min-width': '60px'
  },
];
export const FileHistoryDetailsTableColumns = [
  {
    'label': '厂区',
    'key': 'factory',
    'min-width': '80px'
  },
  {
    'label': '车间',
    'key': 'workshop',
    'min-width': '80px'
  },
  {
    'label': '产线',
    'key': 'line',
    'min-width': '80px'
  },
  {
    'label': '站点名称',
    'key': 'siteName',
    'min-width': '80px'
  },
  {
    'label': '站点编号',
    'key': 'siteNumber',
    'min-width': '80px'
  },
  {
    'label': '页面',
    'key': 'pictures',
    'min-width': '120px'
  },
];

export const ConfirmLogTableColumns = [
  {
    'label': '用户',
    'key': 'userName',
    'min-width': '80px'
  },
  {
    'label': '站点',
    'key': 'siteNumber',
    'min-width': '80px'
  },
  {
    'label': '产线',
    'key': 'lineName',
    'min-width': '80px'
  },
  {
    'label': '确认内容',
    'key': 'content',
    'min-width': '200px'
  },
  {
    'label': '类型',
    'key': 'type',
    'min-width': '80px'
  },
  {
    'label': '确认时间',
    'key': 'time',
    'min-width': '120px'
  },
];

export const NoticeLogTableColumns = [
  {
    'label': '发放人',
    'key': 'pushPerson',
    'min-width': '80px'
  },
  {
    'label': '工厂',
    'key': 'factory',
    'min-width': '80px'
  },
  {
    'label': '车间',
    'key': 'workshop',
    'min-width': '80px'
  },
  {
    'label': '产线',
    'key': 'line',
    'min-width': '80px'
  },
  {
    'label': '通知标题',
    'key': 'title',
    'min-width': '80px'
  },
  {
    'label': '内容',
    'key': 'content',
    'min-width': '200px'
  },
  {
    'label': '发放时间',
    'key': 'pushTime',
    'min-width': '120px'
  },
];


/*编辑表单*/
export const FileFormItems = [];
export const NoticeFormItems = [
  {
    'label': '标题',
    'prop': 'title',
    'type': 'text',
  },
  {
    'label': '内容',
    'prop': 'content',
    'type': 'text',
    'inputType': 'textarea',
    'rows': 5
  },
  {
    'label': '开始播放时间',
    'prop': 'startTime',
    'type': 'date',
  },
  {
    'label': '结束播放时间',
    'prop': 'endTime',
    'type': 'date',
  },
];

/*规则*/
export const FileFormRules = {};
export const NoticeFormRules = {
  title: [
    {required: true, message: '请输入标题', trigger: 'blur'},
  ],
  content: [
    {required: true, message: '请输入内容', trigger: 'blur'},
  ],
  startTime: [
    {required: true, message: '请选择日期', trigger: 'change'},
  ],
  endTime: [
    {required: true, message: '请选择日期', trigger: 'change'},
  ],
};
