/*测试数据配置*/
import store from '../store'


let url;
if (process.env.NODE_ENV === 'production') {
  url = window.g.API_URL
} else {
  url = window.g.LOCAL_URL
}
export const testOperUrl = url + '/test';
export const testSelectUrl = url + "/test/select";
// export const testLogGetUrl = url + "/test/select";
export const testLogGetUrl = url + "/test/getLog";
export const uploadGuideFile = url + "/test/uploadGuideFile";
export const downloadGuideFile = url + "/test/downloadGuideFile";

//export const routerUrl = "http://10.10.11.109:8080/mes_server/order/select";
export const getTestConfig = () => {
    return {
      data: ROUTER_CONFIG.TestManage
    };

};

const ROUTER_CONFIG = {
  TestManage: {
    queryOptions: [
      {
        id: 'SoftWare',
        name: '版本号',
        model: '',
        type: 'text'
      },
      {
        id: 'MachineName',
        name: '机型名',
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
      {field: 'SoftWare', title: '版本号', colStyle: {'width': '120px'}, notNull: true},
      {field: 'MachineName', title: '机型名', colStyle: {'width': '100px'}, notNull: true, formatter(row, column, cellValue, index) {
            return cellValue.split("@@")[0]
        }},
      {field: 'RecordTime', title: '记录时间', colStyle: {'width': '120px'}},
      // {field: 'Station', title: '配置', colStyle: {'width': '300px'}, notNull: true},
    ]
  },
};
export const testLogQueryOptions = [
  {
    type:'select',
    label:'配置类型',
    key:'settingType',
    list:[
      {value:'0',label:'SMT功能测试'},
      {value:'1',label:'组装功能测试'},
      {value:'2',label:'组装耦合测试'},
      {value:'3',label:'研发功能测试'},
      {value:'4',label:'研发耦合测试'},
      {value:'5',label:'OQC'},
    ]
  },

  {
    type:'select',
    label:'操作类型',
    key:'operationType',
    list:[
      {value:'0',label:'新增'},
      {value:'1',label:'修改'},
      {value:'2',label:'删除'},
    ]
  },

  {
    type:'text',
    label:'软件版本',
    key:'softVersion'
  },
  {
    type:'text',
    label:'机型',
    key:'model'
  },
  {
    type:'text',
    label:'操作人',
    key:'operator'
  },
  {
    type:'select',
    label:'操作结果',
    key:'result',
    list:[
      {value:'true', label:'成功'},
      {value:'false', label:'失败'}
    ]
  },
  {
    type:'timeRange',
    label:'操作时间',
    key:'time'
  },
];
export const testLogTableOptions =[
  {
    'label':'版本号',
    'key':'softVersion',
    'min-width':'200'
  },
  {
    'label':'订单号',
    'key':'orderName',
    'min-width':'200'
  },
  {
    'label':'配置类型',
    'key':'settingType',
    'min-width':'200'
  },
  {
    'label':'机型名',
    'key':'model',
    'min-width':'150'
  },
  {
    'label':'操作类型',
    'key':'operationType',
    'min-width':'80'
  },
  {
    'label':'操作人',
    'key':'operator',
    'min-width':'80'
  },
  {
    'label':'操作时间',
    'key':'time',
    'min-width':'200'
  },
  {
    'label':'操作结果',
    'key':'result',
    'min-width':'80'
  },
]
export const logDetailHeader = [
  {
    type:'select',
    label:'配置类型',
    key:'settingType',
  },
  {
    type:'text',
    label:'软件版本',
    key:'softVersion'
  },
  {
    type:'text',
    label:'机型',
    key:'model'
  },
  {
    type:'text',
    label:'操作人',
    key:'operator'
  },
]

export const logDetailTable =[
    {
      'label':'工位',
      'key':'place',
      'min-width':'70'
    },
    {
      'label':'项目',
      'key':'pro',
      'min-width':'230'
    },
    {
      'label':'AT指令',
      'key':'orderAT',
      'min-width':'200'
    },
    {
      'label':'可变参数',
      'key':'variable',
      'min-width':'200',
    },
    {
      'label':'返回值',
      'key':'response',
      'min-width':'200'
    },
  ]
