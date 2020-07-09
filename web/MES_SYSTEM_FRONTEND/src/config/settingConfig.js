/*查询设置*/

export const FactoryQueryConfig = [
  {
    type: 'text',
    label: '厂别',
    size: 'small',
    prop: 'factoryAlias',
  },
  {
    type: 'text',
    label: '简称',
    size: 'small',
    prop: 'abbreviation',
  },
  {
    type: 'text',
    label: '全称',
    size: 'small',
    prop: 'fullName',
  },
];

export const WorkshopQueryConfig = [
  {
    type: 'text',
    label: '车间编号',
    size: 'small',
    prop: 'workshopNumber',
  },
  {
    type: 'text',
    label: '车间名称',
    size: 'small',
    prop: 'workshopName',
  },
  {
    type: 'select',
    label: '所属工厂',
    size: 'small',
    selectList: [],
    prop: 'factoryId',
  }
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
/*  {
    type: 'text',
    label: '工序顺序',
    size: 'small',
    prop: 'processOrder',
  },*/
  {
    type: 'select',
    label: '所属产线',
    size: 'small',
    selectList: [],
    prop: 'lineId',
  }
];

export const CustomerQueryConfig = [
  {
    type: 'text',
    label: '客户编号',
    size: 'small',
    prop: 'customerNumber',
  },
  {
    type: 'text',
    label: '客户名称',
    size: 'small',
    prop: 'customerName',
  },
  {
    type: 'select',
    label: '所属工厂',
    size: 'small',
    selectList: [],
    prop: 'factoryId',
  }
];

export const ModelQueryConfig = [
  {
    type: 'text',
    label: '系列机型',
    size: 'small',
    prop: 'seriesModelName',
  }
];

export const ProductQueryConfig = [
  {
    type: 'text',
    label: '产品型号',
    size: 'small',
    prop: 'productModelName',
  },
  {
    type: 'select',
    label: '系列机型',
    size: 'small',
    selectList: [],
    prop: 'seriesModelId',
  }
];


/*表头配置*/
export const FactoryTableColumns = [
  {
    'label': '厂别',
    'key': 'factoryAlias',
    'min-width': '80px'
  },
  {
    'label': '简称',
    'key': 'abbreviation',
    'min-width': '100px'
  },
  {
    'label': '全称',
    'key': 'fullName',
    'min-width': '120px'
  },
];

export const WorkshopTableColumns = [
  {
    'label': '车间编号',
    'key': 'workshopNumber',
    'min-width': '100px'
  },
  {
    'label': '车间名称',
    'key': 'workshopName',
    'min-width': '100px'
  },
  {
    'label': '所属工厂',
    'key': 'abbreviation',
    'min-width': '100px'
  },
];

export const SiteTableColumns = [
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
    'label': '线别',
    'key': 'lineNo',
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
    'min-width': '100px'
  },
  {
    'label': '切换时间(s)',
    'key': 'switchInterval',
    'min-width': '100px'
  },
  {
    'label': 'MAC地址',
    'key': 'mac',
    'min-width': '100px'
  },
  {
    'label': 'MAC地址2',
    'key': 'secondMac',
    'min-width': '100px'
  },
];

export const ModelTableColumns = [
  {
    'label': '系列机型',
    'key': 'seriesModelName',
    'min-width': '100px'
  },
];

export const ProductTableColumns = [
  {
    'label': '产品型号',
    'key': 'productModelName',
    'min-width': '100px'
  },
  {
    'label': '系列机型',
    'key': 'seriesModelName',
    'min-width': '100px'
  },
];

export const CustomerTableColumns = [
  {
    'label': '客户编号',
    'key': 'customerNumber',
    'min-width': '100px'
  },
  {
    'label': '客户名称',
    'key': 'customerName',
    'min-width': '100px'
  },
  {
    'label': '所属工厂',
    'key': 'abbreviation',
    'min-width': '100px'
  },
];

/*新增、编辑项目表单字段*/
export const FactoryFormItems = [
  {
    'type': 'text',
    'label': '别名',
    'prop': 'factoryAlias',
    'limit': [0, 8],
  },
  {
    'type': 'text',
    'label': '简称',
    'prop': 'abbreviation',
    'limit': [0, 16],
  },
  {
    'type': 'text',
    'label': '全名',
    'prop': 'fullName',
    'limit': [0, 32],
  },
];

export const WorkshopFormItems = [
  {
    'label': '车间编号',
    'prop': 'workshopNumber',
    'type': 'text',
    'limit': [0, 16],
  },
  {
    'label': '车间名称',
    'prop': 'workshopName',
    'type': 'text',
    'limit': [0, 16],
  },
  {
    'label': '所属工厂',
    'prop': 'factoryId',
    'type': 'select',
    'selectList': []
  },
];

export const SiteFormItems = [
  {
    'label': '站点编号',
    'prop': 'siteNumber',
    'type': 'text',
    'limit': [0, 16],
  },
  {
    'label': '站点名称',
    'prop': 'siteName',
    'type': 'text',
    'limit': [0, 16],
  },
  {
    'label': '工序顺序',
    'prop': 'processOrder',
    'type': 'text',
  },
  {
    'label': '线别',
    'prop': 'lineId',
    'type': 'select',
    'selectList': []
  },
  {
    'label': '播放次数',
    'prop': 'playTimes',
    'type': 'text',
  },
  {
    'label': '切换时间(s)',
    'prop': 'switchInterval',
    'type': 'text',
  },
  {
    'label': 'MAC地址',
    'prop': 'mac',
    'type': 'text',
  },
  {
    'label': 'MAC地址2',
    'prop': 'secondMac',
    'type': 'text',
  },
];

export const ModelFormItems = [
  {
    'label': '系列机型',
    'prop': 'seriesModelName',
    'type': 'text',
    'limit': [0, 32],
  },
];

export const ProductFormItems = [
  {
    'label': '产品型号',
    'prop': 'productModelName',
    'type': 'text',
    'limit': [0, 32],
  },
  {
    'label': '系列机型',
    'prop': 'seriesModelId',
    'type': 'select',
    'selectList': []
  },
];

export const CustomerFormItems = [
  {
    'label': '客户编号',
    'prop': 'customerNumber',
    'type': 'text',
    'limit': [0, 16],
  },
  {
    'label': '客户名称',
    'prop': 'customerName',
    'type': 'text',
    'limit': [0, 16],
  },
  {
    'label': '所属工厂',
    'prop': 'factoryId',
    'type': 'select',
    'selectList': []
  },
];

/*规则*/
export const FactoryFormRules = {
  factoryAlias: [
    {required: true, message: '请输入别名', trigger: 'blur'},
  ],
  abbreviation: [
    {required: true, message: '请输入简称', trigger: 'blur'},
  ],
  fullName: [
    {required: true, message: '请输入全名', trigger: 'blur'},
  ],
};

export const WorkshopFormRules = {
  workshopNumber: [
    {required: true, message: '请输入车间编号', trigger: 'blur'},
  ],
  workshopName: [
    {required: true, message: '请输入车间名称', trigger: 'blur'},
  ],
  factoryId: [
    {required: true, message: '请选择所属工厂', trigger: 'change'},
  ],
};


export const SiteFormRules = {
  siteNumber: [
    {required: true, message: '请输入站点编号', trigger: 'blur'},
  ],
  siteName: [
    {required: true, message: '请输入站点名称', trigger: 'blur'},
  ],
  processOrder: [
    {required: true, message: '请输入工序顺序', trigger: 'blur'},
    {pattern: /^[1-9]\d*$/, message: '请输入正整数', trigger: 'blur'},
  ],
  lineId: [
    {required: true, message: '请选择线别', trigger: 'change'},
  ],
  playTimes: [
    {pattern: /^[1-9]\d*$/, message: '请输入正整数', trigger: 'blur'},
  ],
  switchInterval: [
    {required: true, message: '请输入切换时间', trigger: 'blur'},
    {pattern: /^[1-9]\d*$/, message: '请输入正整数', trigger: 'blur'},
  ],
  mac: [
    {required: true, message: '请输入MAC地址', trigger: 'blur'},
    {pattern: /([A-Fa-f0-9]{2}:){5}[A-Fa-f0-9]{2}/, message: '请输入正确的MAC地址(以:分隔)', trigger: 'blur'},
  ],
  secondMac: [
    {pattern: /([A-Fa-f0-9]{2}:){5}[A-Fa-f0-9]{2}/, message: '请输入正确的MAC地址(以:分隔)', trigger: 'blur'},
  ],
};

export const ModelFormRules = {
  seriesModelName: [
    {required: true, message: '请输入系列机型', trigger: 'blur'},
  ],
};

export const ProductFormRules = {
  productModelName: [
    {required: true, message: '请输入产品型号', trigger: 'blur'},
  ],
  seriesModelId: [
    {required: true, message: '请选择系列机型', trigger: 'change'},
  ],
};

export const CustomerFormRules = {
  customerNumber: [
    {required: true, message: '请输入客户编号', trigger: 'blur'},
  ],
  customerName: [
    {required: true, message: '请输入客户名称', trigger: 'blur'},
  ],
  factoryId: [
    {required: true, message: '请选择所属工厂', trigger: 'change'},
  ],
};
