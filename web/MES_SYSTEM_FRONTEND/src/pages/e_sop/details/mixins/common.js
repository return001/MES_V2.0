import {axiosFetch} from "../../../../utils/fetchData";
import {eSopFactorySelectUrl, eSopModelSelectUrl} from "../../../../config/globalUrl";
import {MessageBox} from "element-ui";

function getDataList(url, errItemTitle) {
  return new Promise(resolve => {
    axiosFetch({
      url: url,
      data: {
        pageNo: 1,
        pageSize: 2147483647
      }
    }).then(response => {
      if (response.data.result === 200) {
        resolve(response.data.data.list);
      } else {
        this.$alertWarning(response.data.data);
      }
    }).catch(() => {
      this.$alertDanger(`获取${errItemTitle}列表失败，请刷新重试`)
    })
  })
}

export default {
  methods: {
    /*表格方法*/
    _indexMethod: function (index) {
      return index + (this.paginationOptions.currentPage - 1) * this.paginationOptions.pageSize + 1
    },

    /*获取select列表*/
    _getFactoryList() {
      return getDataList(eSopFactorySelectUrl, '工厂');
    },
    _getLineList() {
      return getDataList('', '产线');
    },
    _getModelList() {
      return getDataList(eSopModelSelectUrl, '系列机型');
    },

    /*数据获取*/
    _fetchData(options) {
      return new Promise(resolve => {
        axiosFetch(options).then(response => {
          if (response.data.result === 200) {
            resolve(response.data.data);
          } else {
            this.$alertWarning(response.data.data);
          }
        }).catch(() => {
          this.$alertDanger(`未知错误`)
        }).finally(() => {
          this.$closeLoading();
        })
      })
    },

    /*数据删除*/
    _deleteData(url, id) {
      return new Promise(resolve => {
        MessageBox.confirm('将删除该配置，是否继续?', '提示', {
          confirmButtonText: '确认',
          cancelButtonText: '取消',
          type: 'warning'
        }).then(() => {
          this.$openLoading();
          axiosFetch({
            url: url,
            data: {
              id: id
            }
          }).then(response => {
            if (response.data.result === 200) {
              this.$alertSuccess('删除成功');
              resolve();
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.$alertDanger("未知错误")
          }).finally(() => {
            this.$closeLoading();
          })
        }).catch(() => {

        })
      })
    }
  }
}
