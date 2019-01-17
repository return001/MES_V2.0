/*测试数据配置*/
let url;
if (process.env.NODE_ENV === 'production') {
  url = window.g.API_URL
} else {
  url = window.g.LOCAL_URL
}
export const testOperUrl = url + '/mes_server/test';
export const testSelectUrl = url + "/mes_server/test/select";

//export const routerUrl = "http://10.10.11.109:8080/mes_server/order/select";
export const getTestConfig = (name) => {
  let config = JSON.parse(JSON.stringify(ROUTER_CONFIG));
  if (name === 'test_manage') {
    for (let i = 0; i < 68; i ++){
      config.TestManage.dataColumns.push({field: 'Setting' + i, title: '配置项' + i, colStyle: {'width': '100px'}})
    }
    return {
      data: config.TestManage
    };
  }
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
      }
    ],
    dataColumns: [
      {field: 'showId', title: '序号', colStyle: {'width': '70px'}},
      {title: '操作', tdComp: 'EditOptions', colStyle: {'width': '100px'}},
      {field: 'SoftWare', title: '版本号', colStyle: {'width': '120px'}, notNull: true},
      {field: 'MachineName', title: '机型名', colStyle: {'width': '100px'}, notNull: true},
      {field: 'RecordTime', title: '记录时间', colStyle: {'width': '120px'}},
      {field: 'Station', title: '配置', colStyle: {'width': '300px'}, notNull: true},
    ]
  },
};
