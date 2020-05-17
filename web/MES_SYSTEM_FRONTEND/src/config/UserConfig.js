/*部门设置*/
export const DeptQueryConfig = [
  {
    type: 'select',
    label: '所属公司',
    size: 'small',
    selectList: [],
    prop: 'company',
  },
  {
    type: 'text',
    label: '部门编号',
    size: 'small',
    prop: 'no',
  },
  {
    type: 'text',
    label: '部门名称',
    size: 'small',
    prop: 'name',
  },
];

export const DeptTableColumns = [
  {
    'label': '所属公司',
    'key': 'companyName',
    'min-width': '100px'
  },
  {
    'label': '部门编号',
    'key': 'no',
    'min-width': '100px'
  },
  {
    'label': '部门名称',
    'key': 'name',
    'min-width': '100px'
  },
];

export const DeptFormItems = [
  {
    'label': '所属公司',
    'prop': 'company',
    'type': 'select',
    'selectList': []
  },
  {
    'label': '部门编号',
    'prop': 'no',
    'type': 'text',
    'limit': [0, 16],
  },
  {
    'label': '部门名称',
    'prop': 'name',
    'type': 'text',
    'limit': [0, 16],
  },
];

export const DeptFormRules = {
  name: [
    {required: true, message: '请输入部门名称', trigger: 'blur'},
  ],
  no: [
    {required: true, message: '请输入部门编号', trigger: 'blur'},
  ],
  company: [
    {required: true, message: '请选择所属公司', trigger: 'change'},
  ],
};

/*角色设置*/
export const CharQueryConfig = [
  {
    type: 'select',
    label: '所属公司',
    size: 'small',
    selectList: [],
    prop: 'company',
  },
  {
    type: 'select',
    label: '所属部门',
    size: 'small',
    selectList: [],
    prop: 'department',
  },
  {
    type: 'text',
    label: '角色编号',
    size: 'small',
    prop: 'no',
  },
  {
    type: 'text',
    label: '角色名称',
    size: 'small',
    prop: 'name',
  },
];

export const CharTableColumns = [
  {
    'label': '所属公司',
    'key': 'companyName',
    'min-width': '100px'
  },
  {
    'label': '所属部门',
    'key': 'departmentName',
    'min-width': '100px'
  },
  {
    'label': '角色编号',
    'key': 'no',
    'min-width': '100px'
  },
  {
    'label': '角色名称',
    'key': 'name',
    'min-width': '100px'
  },
];
export const CharFormItems = [
  {
    'label': '所属公司',
    'prop': 'company',
    'type': 'select',
    'selectList': []
  },
  {
    'label': '所属部门',
    'prop': 'department',
    'type': 'select',
    'selectList': []
  },
  {
    'label': '角色编号',
    'prop': 'no',
    'type': 'text',
    'limit': [0, 16],
  },
  {
    'label': '角色名称',
    'prop': 'name',
    'type': 'text',
    'limit': [0, 16],
  },
];
export const CharFormRules = {
  name: [
    {required: true, message: '请输入部门名称', trigger: 'blur'},
  ],
  no: [
    {required: true, message: '请输入部门编号', trigger: 'blur'},
  ],
  company: [
    {required: true, message: '请选择所属公司', trigger: 'change'},
  ],
  department: [
    {required: true, message: '请选择所属部门', trigger: 'change'},
  ],
};

export const CharAthoritiesColumns = [
  {
    'label': '功能',
    'key': 'func',
    'min-width': '100px'
  },
  {
    'label': '模块',
    'key': 'module',
    'min-width': '100px'
  },
  {
    'label': '页面',
    'key': 'page',
    'min-width': '100px'
  },
  {
    'label': '查询',
    'key': '0',
    'min-width': '60px',
    'slot': true
  },
  {
    'label': '新增',
    'key': '1',
    'min-width': '60px',
    'slot': true
  },
  {
    'label': '修改',
    'key': '2',
    'min-width': '60px',
    'slot': true
  },
  {
    'label': '删除',
    'key': '3',
    'min-width': '60px',
    'slot': true
  },
  {
    'label': '上传',
    'key': '4',
    'min-width': '60px',
    'slot': true
  },
  {
    'label': '下载',
    'key': '5',
    'min-width': '60px',
    'slot': true
  },
  {
    'label': '审核',
    'key': '6',
    'min-width': '60px',
    'slot': true
  },
  {
    'label': '其他',
    'key': '7',
    'min-width': '60px',
    'slot': true
  },
  {
    'label': '其他说明',
    'key': 'remark',
    'min-width': '160px'
  }
];