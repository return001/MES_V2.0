import {axiosFetch} from "../../../../utils/fetchData";
import {MessageBox} from "element-ui";
import store from "../../../../store";

function setStashData(data) {
  store.commit('setStashData', data)
}

export default {
  data() {
    return {
      isPending: false,
      paginationOptions: {
        currentPage: 1,
        pageSize: 20,
        total: 0
      },
    }
  },

  methods: {
    /*重载*/
    _partlyReload (stashData) {
      let obj = {};
      stashData.forEach(item => {
        obj[item] = this.$data[item]
      });
      setStashData(obj);
      Object.assign(this.$data, this.$options.data());
      Object.assign(this.$data, this.$store.state.stashData);
      this._queryData();
      setStashData({});
    },

    /*重置查询*/
    _initQueryOptions() {
      this.queryCompData = {};
    },

    /*查询*/
    _queryData() {
      this.paginationOptions.currentPage = 1;
      this.paginationOptions.total = 0;
      this.fetchData();
    },

    /*表格方法*/
    _indexMethod (index) {
      return index + (this.paginationOptions.currentPage - 1) * this.paginationOptions.pageSize + 1
    },

    /*数据获取*/
    _fetchData(options) {
      return new Promise(resolve => {
        axiosFetch(options).then(response => {
          if (response.data.result === 200) {
            this.paginationOptions.currentPage = response.data.data.pageNumber;
            this.paginationOptions.total = response.data.data.totalRow;
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
    _deleteData(row) {
      MessageBox.confirm('将删除该项目，是否继续?', '提示', {
        confirmButtonText: '确认',
        cancelButtonText: '取消',
        type: 'warning'
      }).then(() => {
        this.$openLoading();
        axiosFetch({
          url: this.formDeleteUrl,
          data: {
            id: row.id
          }
        }).then(response => {
          if (response.data.result === 200) {
            this.$alertSuccess('删除成功');
            this.partlyReload();
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
    },

    /*权限控制*/
    _permissionControl: function (userArray) {
      let thisUser = this.$store.state.userType;
      return userArray.indexOf(thisUser) !== -1 || thisUser === 'SuperAdmin';
    },
  }
}
