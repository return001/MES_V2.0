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
