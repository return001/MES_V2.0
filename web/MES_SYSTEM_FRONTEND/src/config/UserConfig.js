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

/*用户配置*/
export const UserQueryConfig = [
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
    label: '角色名称',
    size: 'small',
    prop: 'roleName',
  },
  {
    type: 'text',
    label: '工号',
    size: 'small',
    prop: 'name',
  },
  {
    type: 'text',
    label: '用户描述',
    size: 'small',
    prop: 'userDes',
  },
  {
    type: 'select',
    label: '主要工序',
    size: 'small',
    selectList: [],
    prop: 'mainProcess',
  },
  {
    type: 'text',
    label: '其他技能',
    size: 'small',
    prop: 'otherProcess',
  },
  {
    type: 'select',
    label: '所属产线',
    size: 'small',
    selectList: [],
    prop: 'lineName',
  },
  {
    type: 'select',
    label: '是否启用',
    size: 'small',
    selectList: [
      {
        key: true,
        label: '是'
      },
      {
        key: false,
        label: '否'
      }

    ],
    prop: 'inService',
  },
  {
    type: 'select',
    label: '熟练度',
    size: 'small',
    selectList: [
      {
        label: '熟练',
        key: '熟练'
      },
      {
        label: '一般',
        key: '一般'
      },
      {
        label: '不熟',
        key: '不熟'
      },
    ],
    prop: 'proficiency',
  },
];

export const UserTableColumns = [
  {
    'label': '工号',
    'key': 'name',
    'min-width': '100px'
  },
  {
    'label': '描述',
    'key': 'userDes',
    'min-width': '100px'
  },
  {
    'label': '角色',
    'key': 'roleName',
    'min-width': '100px'
  },
  {
    'label': '是否启用',
    'key': 'inService',
    'min-width': '100px',
    formatter(row, column, cellValue, index) {
      switch (cellValue) {
        case true:
          return '是';
        case false:
          return '否';
      }
    }
  },
  {
    'label': '所属产线',
    'key': 'lineName',
    'min-width': '100px'
  },
  {
    'label': '主要工序',
    'key': 'mainProcess',
    'min-width': '100px'
  },
  {
    'label': '熟练程度',
    'key': 'proficiency',
    'min-width': '100px'
  },
  {
    'label': '其他技能',
    'key': 'otherProcess',
    'min-width': '100px',
    formatter(row, column, cellValue, index) {
      return !cellValue ? '' : cellValue.split('@@').join('、')
    }
  },
];


export const UserFormRules = {
  userName: [
    {required: true, message: '请输入用户名(工号)', trigger: 'blur'},
  ],
  userDes: [
    {required: true, message: '请输入用户描述', trigger: 'blur'},
  ],
};