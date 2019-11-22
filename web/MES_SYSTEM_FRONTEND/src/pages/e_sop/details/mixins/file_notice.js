import {axiosFetch} from "../../../../utils/fetchData";
import {eSopWorkshopSelectUrl, planLineSelectUrl} from "../../../../config/globalUrl";

export default {
  data() {
    return {
      /*文件发放*/
      isDispatching: false,
      pageList: [], //excel文件页（图片）列表
      remainingPageList: [], //待分配页面
      lineQueryData: {
        factoryId: null,
        workshopId: null,
        lineId: null
      }, //查询栏
      //查询栏的工厂车间产线列表
      factoryList: [],
      workshopList: [],
      lineList: [],
      siteList: [],
    }
  },

  methods: {

    getDataList(url, errItemTitle, extraOption) {
      return new Promise(resolve => {
        let options = {
          url: url,
          data: {
            pageNo: 1,
            pageSize: 2147483647
          }
        };
        if (!!extraOption) {
          Object.assign(options.data, extraOption)
        }
        axiosFetch(options).then(response => {
          if (response.data.result === 200) {
            resolve(response.data.data.list);
          } else {
            this.$alertWarning(response.data.data);
          }
        }).catch(() => {
          this.$alertDanger(`获取${errItemTitle}列表失败，请刷新重试`)
        })
      })
    },

    /*当选择厂区后触发车间查询*/
    setWorkshopList(val) {
      this.isPending = true;
      this.$openLoading();
      this.getDataList(eSopWorkshopSelectUrl, '车间', {
        factoryId: val
      }).then(data => {
        this.workshopList = data.map(item => {
          return {
            key: item.id,
            label: item.workshopName,
          }
        });

        /*重置下一层数据*/
        this.lineQueryData.workshopId = null;
        this.lineQueryData.lineId = null;
        this.lineList = [];
        this.siteList = [];

        this.isPending = false;
        this.$closeLoading();
      })
    },

    /*当选择车间后触发产线查询*/
    setLineList(val) {
      this.isPending = true;
      this.$openLoading();
      this.getDataList(planLineSelectUrl, '产线', {
        workshopId: val
      }).then(data => {
        this.lineList = data.map(item => {
          return {
            key: item.id,
            label: item.lineName,
          }
        });

        /*重置下一层数据*/
        this.lineQueryData.lineId = null;
        this.siteList = [];

        this.isPending = false;
        this.$closeLoading();
      })
    },


  }
}
