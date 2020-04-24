/*订单数据配置*/
import fa from "element-ui/src/locale/lang/fa";

let url;
if (process.env.NODE_ENV === 'production') {
  url = window.g.API_URL
} else {
  url = window.g.LOCAL_URL
}
export const orderOperUrl = url + '/order';
export const orderSelectUrl = url + "/order/select";

//export const routerUrl = "http://10.10.11.109:8080/mes_server/order/select";
export const getOrderConfig = (name) => {
  if (name === 'order_manage') {
    return {
      data: ROUTER_CONFIG.OrderManage
    };
  }
};

const ROUTER_CONFIG = {
  OrderManage: {
    queryOptions: [
      {
        id: 'ZhiDan',
        name: '制单号',
        model: '',
        type: 'text'
      }
    ],
    dataColumns: [
      // {field: 'Id', title: '序号', visible: false},
      // {field: 'showId', title: '序号', colStyle: {'width': '70px'}},
      // {title: '操作', tdComp: 'EditOptions', colStyle: {'width': '120px'}},
      // {field: 'ShowStatus', title: '状态', colStyle: {'width': '80px'}},
      {
        field: 'Status', title: '状态', colStyle: {'width': '80px'}, formatter(row, column, cellValue, index) {
          switch (cellValue) {
            case 0:
              return '未开始';
            case 1:
              return '进行中';
            case 2:
              return '已完成';
            case 3:
              return '已作废';
          }
        }
      },
      {field: 'ZhiDan', title: '制单号', colStyle: {'width': '130px'}, notNull: true},
      {field: 'SoftModel', title: '型号', colStyle: {'width': '100px'}, notNull: true},
      {field: 'SN1', title: 'SN1', colStyle: {'width': '100px'}, notNull: true},
      {field: 'SN2', title: 'SN2', colStyle: {'width': '70px'}, notNull: true},
      {field: 'SN3', title: 'SN3', colStyle: {'width': '70px'}, notNull: true},
      {field: 'BoxNo1', title: '箱号1', colStyle: {'width': '70px'}, notNull: true},
      {field: 'BoxNo2', title: '箱号2', colStyle: {'width': '70px'}, notNull: true},
      {field: 'ProductDate', title: '生产日期', colStyle: {'width': '100px'}, notNull: true},
      {field: 'Color', title: '颜色', colStyle: {'width': '60px'}, notNull: true},
      {field: 'Weight', title: '重量', colStyle: {'width': '80px'}, notNull: true},
      {field: 'Qty', title: '数量', colStyle: {'width': '80px'}, notNull: true},
      {field: 'PlanProduction', title: '计划数量', colStyle: {'width': '80px'}, notNull: false},
      {field: 'RFID_Area', title: '地区', colStyle: {'width': '80px'}, notNull: true},
      {field: 'ProductNo', title: '产品编号', colStyle: {'width': '140px'}, notNull: true},
      {field: 'Version', title: '版本', colStyle: {'width': '140px'}, notNull: true},
      {field: 'IMEIStart', title: '起始IMEI号', colStyle: {'width': '150px'}, notNull: true},
      {field: 'IMEIEnd', title: '终止IMEI号', colStyle: {'width': '150px'}, notNull: true},
      {field: 'SIMStart', title: '起始SIM卡号', colStyle: {'width': '150px'}, notNull: false},
      {field: 'SIMEnd', title: '终止SIM卡号', colStyle: {'width': '150px'}, notNull: false},
      {field: 'BATStart', title: '起始BAT号', colStyle: {'width': '120px'}, notNull: false},
      {field: 'BATEnd', title: '终止BAT号', colStyle: {'width': '120px'}, notNull: false},
      {field: 'VIPStart', title: '起始VIP号', colStyle: {'width': '120px'}, notNull: false},
      {field: 'VIPEnd', title: '终止VIP号', colStyle: {'width': '120px'}, notNull: false},
      {
        field: 'IMEIRel',
        title: 'IMEI关联',
        colStyle: {'width': '150px'},
        notNull: true,
        formatter(row, column, cellValue, index) {
          switch (cellValue) {
            case 0:
              return '无绑定';
            case 1:
              return '与SMI卡绑定';
            case 2:
              return '与SIM&BAT绑定';
            case 3:
              return '与SIM&VIP绑定';
            case 4:
              return '与BAT绑定';
          }
        }
      },
      {field: 'TACInfo', title: 'TAC信息', colStyle: {'width': '100px'}, notNull: true},
      {field: 'CompanyName', title: '公司名', colStyle: {'width': '100px'}, notNull: false},
      {field: 'Remark1', title: '备注1', colStyle: {'width': '200px'}, notNull: false},
      {field: 'Remark2', title: '备注2', colStyle: {'width': '100px'}, notNull: false},
      {field: 'Remark3', title: '备注3', colStyle: {'width': '100px'}, notNull: false},
      {field: 'Remark4', title: '备注4', colStyle: {'width': '100px'}, notNull: false},
      {field: 'Remark5', title: '备注5', colStyle: {'width': '100px'}, notNull: false},
      {field: 'JST_template', title: 'JST模板', colStyle: {'width': '100px'}, notNull: false},
      {field: 'CHT_template1', title: 'CHT模板1', colStyle: {'width': '100px'}, notNull: false},
      {field: 'CHT_template2', title: 'CHT模板2', colStyle: {'width': '100px'}, notNull: false},
      {field: 'BAT_prefix', title: 'BAT前缀', colStyle: {'width': '100px'}, notNull: false},
      {field: 'BAT_digits', title: 'BAT位数', colStyle: {'width': '100px'}, notNull: false},
      {field: 'SIM_prefix', title: 'SIM前缀', colStyle: {'width': '100px'}, notNull: false},
      {field: 'SIM_digits', title: 'SIM位数', colStyle: {'width': '100px'}, notNull: false},
      {field: 'VIP_prefix', title: 'VIP前缀', colStyle: {'width': '100px'}, notNull: false},
      {field: 'VIP_digits', title: 'VIP位数', colStyle: {'width': '100px'}, notNull: false},
      {field: 'ICCID_prefix', title: 'ICCID前缀', colStyle: {'width': '100px'}, notNull: false},
      {field: 'ICCID_digits', title: 'ICCID位数', colStyle: {'width': '100px'}, notNull: false},
      {field: 'IMEIPrints', title: 'IMEI打印', colStyle: {'width': '100px'}, notNull: false},
      {field: 'MAC_prefix', title: 'MAC前缀', colStyle: {'width': '100px'}, value: '', "notNull": false},
      {field: 'MAC_digits', title: 'MAC位数', colStyle: {'width': '100px'}, value: '', "notNull": false},
      {field: 'Equipment_prefix', title: 'Equipment前缀', colStyle: {'width': '100px'}, value: '', "notNull": false},
      {field: 'Equipment_digits', title: 'Equipment位数', colStyle: {'width': '100px'}, value: '', "notNull": false},
      {field: 'RFID_Start', title: '起始RFID', colStyle: {'width': '100px'}, value: '', "notNull": false},
      {field: 'RFID_End', title: '终止RFID', colStyle: {'width': '100px'}, value: '', "notNull": false},
      {field: 'RFID_prefix', title: 'RFID前缀', colStyle: {'width': '100px'}, value: '', "notNull": false},
      {field: 'RFID_digits', title: 'RFID位数', colStyle: {'width': '100px'}, value: '', "notNull": false},
      {field: 'IMEI2Start', title: 'IMEI2起始位', colStyle: {'width': '100px'}, value: '', "notNull": false},
      {field: 'IMEI2End', title: 'IMEI2终止位', colStyle: {'width': '100px'}, value: '', "notNull": false},
      {field: 'IMEI2Prints', title: 'IMEI2打印', colStyle: {'width': '100px'}, value: '', "notNull": false},
      {field: 'IMEIMutiRange', title: 'IMEI多号段', colStyle: {'width': '100px'}, value: '', "notNull": false},
      {field: 'Week', title: '周期', colStyle: {'width': '100px'}, value: '', "notNull": false},
    ]
  },


};

export const getOrderEditOptions = () => {
  return [
    {field: 'ZhiDan', title: '制单号', value: '', type: 'text', notNull: true},
    {
      field: 'Status', title: '状态', value: '', type: 'select', valueList: [
        {
          label: '未开始',
          key: 0
        },
        {
          label: '进行中',
          key: 1
        },
        {
          label: '已完成',
          key: 2
        },
        {
          label: '已作废',
          key: 3
        }
      ], notNull: false
    },
    {field: 'SoftModel', title: '型号', value: '', type: 'text', notNull: true},
    {field: 'SN1', title: 'SN1', value: '', type: 'text', notNull: true},
    {field: 'SN2', title: 'SN2', value: '', type: 'text', notNull: true},
    {field: 'SN3', title: 'SN3', value: '', type: 'text', notNull: true},
    {field: 'BoxNo1', title: '箱号1', value: '', type: 'text', notNull: true},
    {field: 'BoxNo2', title: '箱号2', value: '', type: 'text', notNull: true},
    {field: 'ProductDate', title: '生产日期', value: '', type: 'text', notNull: true},
    {field: 'Color', title: '颜色', value: '', type: 'text', notNull: true},
    {field: 'Weight', title: '重量', value: '', type: 'text', notNull: true},
    {field: 'Qty', title: '数量', value: '', type: 'text', notNull: true},
    {field: 'PlanProduction', title: '计划数量', value: '', type: 'text', notNull: false},
    {
      field: 'RFID_Area', title: '地区', value: '', type: 'select', notNull: true, valueList: [
        {
          label: '天波',
          key: '天波'
        },
        {
          label: '辽宁',
          key: '辽宁'
        },
        {
          label: '贵州',
          key: '贵州'
        },
        {
          label: '江西',
          key: '江西'
        },
        {
          label: '四川',
          key: '四川'
        },
        {
          label: '长治',
          key: '长治'
        },
        {
          label: '阿艾富',
          key: '阿艾富'
        },
        {
          label: '海振邦',
          key: '海振邦'
        }
      ]
    },
    {field: 'ProductNo', title: '产品编号', value: '', type: 'text', notNull: true},
    {field: 'Version', title: '版本', value: '', type: 'text', notNull: true},
    {field: 'IMEIStart', title: '起始IMEI号', value: '', type: 'text', notNull: true},
    {field: 'IMEIEnd', title: '终止IMEI号', value: '', type: 'text', notNull: true},
    {field: 'SIMStart', title: '起始SIM卡号', value: '', type: 'text', notNull: false},
    {field: 'SIMEnd', title: '终止SIM卡号', value: '', type: 'text', notNull: false},
    {field: 'BATStart', title: '起始BAT号', value: '', type: 'text', notNull: false},
    {field: 'BATEnd', title: '终止BAT号', value: '', type: 'text', notNull: false},
    {field: 'VIPStart', title: '起始VIP号', value: '', type: 'text', notNull: false},
    {field: 'VIPEnd', title: '终止VIP号', value: '', type: 'text', notNull: false},
    {
      field: 'IMEIRel', title: 'IMEI关联', value: '', type: 'select', notNull: true, valueList: [
        {
          label: '无绑定',
          key: 0
        },
        {
          label: '与SMI卡绑定',
          key: 1
        },
        {
          label: '与SIM&BAT绑定',
          key: 2
        },
        {
          label: '与SIM&VIP绑定',
          key: 3
        },
        {
          label: '与BAT绑定',
          key: 4
        }
      ]
    },
    {field: 'TACInfo', title: 'TAC信息', value: '', type: 'text', notNull: true},
    {field: 'CompanyName', title: '公司名', type: 'text', notNull: false},
    {field: 'Remark1', title: '备注1', value: '', type: 'text', notNull: false},
    {field: 'Remark2', title: '备注2', value: '', type: 'text', notNull: false},
    {field: 'Remark3', title: '备注3', value: '', type: 'text', notNull: false},
    {field: 'Remark4', title: '备注4', value: '', type: 'text', notNull: false},
    {field: 'Remark5', title: '备注5', value: '', type: 'text', notNull: false},
    {field: 'JST_template', title: 'JST模板', value: '', type: 'text', notNull: false},
    {field: 'CHT_template1', title: 'CHT模板1', value: '', type: 'text', notNull: false},
    {field: 'CHT_template2', title: 'CHT模板2', value: '', type: 'text', notNull: false},
    {field: 'BAT_prefix', title: 'BAT前缀', value: '', type: 'text', notNull: false},
    {field: 'BAT_digits', title: 'BAT位数', value: '', type: 'text', notNull: false},
    {field: 'SIM_prefix', title: 'SIM前缀', value: '', type: 'text', notNull: false},
    {field: 'SIM_digits', title: 'SIM位数', value: '', type: 'text', notNull: false},
    {field: 'VIP_prefix', title: 'VIP前缀', value: '', type: 'text', notNull: false},
    {field: 'VIP_digits', title: 'VIP位数', value: '', type: 'text', notNull: false},
    {field: 'ICCID_prefix', title: 'ICCID前缀', value: '', type: 'text', notNull: false},
    {field: 'ICCID_digits', title: 'ICCID位数', value: '', type: 'text', notNull: false},
    {field: 'IMEIPrints', title: 'IMEI打印', value: '', type: 'text', notNull: false},
    {field: 'MAC_prefix', title: 'MAC前缀', value: '', type: 'text', notNull: false},
    {field: 'MAC_digits', title: 'MAC位数', value: '', type: 'text', notNull: false},
    {field: 'Equipment_prefix', title: 'Equipment前缀', value: '', type: 'text', notNull: false},
    {field: 'Equipment_digits', title: 'Equipment位数', value: '', type: 'text', notNull: false},
    {field: 'RFID_Start', title: '起始RFID', value: '', type: 'text', notNull: false},
    {field: 'RFID_End', title: '终止RFID', value: '', type: 'text', notNull: false},
    {field: 'RFID_prefix', title: 'RFID前缀', value: '', type: 'text', notNull: false},
    {field: 'RFID_digits', title: 'RFID位数', value: '', type: 'text', notNull: false},
    {field: 'IMEI2Start', title: 'IMEI2起始位', value: '', type: 'text', notNull: false},
    {field: 'IMEI2End', title: 'IMEI2终止位', value: '', type: 'text', notNull: false},
    {field: 'IMEI2Prints', title: 'IMEI2打印', value: '', type: 'text', notNull: false},
    {field: 'IMEIMutiRange', title: 'IMEI多号段', value: '', type: 'text', notNull: false},
    {field: 'Week', title: '周期', value: '', type: 'text', notNull: false},
  ]
};
