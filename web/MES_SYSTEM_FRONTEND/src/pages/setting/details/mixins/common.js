import {axiosFetch} from "../../../../utils/fetchData";
import {eSopFactorySelectUrl, eSopModelSelectUrl, planLineGetUrl} from "../../../../config/globalUrl";
import {MessageBox} from "element-ui";
import store from "../../../../store";

function getDataList(url, errItemTitle, vue) {
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
        vue.$alertWarning(response.data.data);
      }
    }).catch(() => {
      vue.$alertDanger(`获取${errItemTitle}列表失败，请刷新重试`)
    })
  })
}

function setStashData(data) {
  store.commit('setStashData', data)
}
export const getFactoryList = function getFactoryList() {
  return getDataList(eSopFactorySelectUrl, '工厂', this);
};

export default {
  inject: ['reload'],
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

    /*获取select列表*/
    _getFactoryList() {
      return getDataList(eSopFactorySelectUrl, '工厂', this);
    },

    _getLineList() {
      return getDataList(planLineGetUrl, '产线', this);
    },
    _getModelList() {
      return getDataList(eSopModelSelectUrl, '系列机型', this);
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
              this._partlyReload(['queryConfig', 'buttonGroup', 'queryCompData', 'formItemList'])
              // this.reload();
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

    /*编辑数据时初始化表单数据*/
    _initFormData(row) {
      this.formItemList.forEach(item => {
        this.formData[item.prop] = row[item.prop] === null ? '' : row[item.prop];
      })
    },

    /*确认编辑、添加项目*/
    _submitFormEdit() {
      let url;
      if (this.editType === 0) {
        url = this.formAddUrl
      } else {
        url = this.formUpdateUrl
      }
      if (!this.isPending) {
        this.$refs[this.formConfig.refName].validate(isValid => {
          if (isValid) {
            this.isPending = true;
            this.$openLoading();
            axiosFetch({
              url: url,
              data: this.formData
            }).then(response => {
              if (response.data.result === 200) {
                this.$alertSuccess('操作成功');
                this._partlyReload(['queryConfig', 'buttonGroup', 'queryCompData', 'formItemList'])
                // this.reload();
              } else {
                this.$alertWarning(response.data.data)
              }
            }).catch((err) => {
              console.log(err)
              this.$alertDanger('未知错误');
            }).finally(() => {
              this.isPending = false;
              this.$closeLoading();
            })
          } else {
            this.$alertInfo('请检查输入内容')
          }
        })
      }
    },


    /*新增项目*/
    _addData() {
      this.editType = 0;
      this.editPanelTitle = '新增项目';
      this.isEditing = true;
    },

    /*编辑指定项目*/
    _editData(row) {
      this.editType = 1;
      this.editPanelTitle = '编辑项目';
      this.formData.id = row.id;
      this._initFormData(row);
      this.isEditing = true;
    },

    _resetEditForm() {
      this.formData = {};
    },

    _closeEditPanel() {
      this.isEditing = false;
    },

    /*权限控制*/
    _permissionControl: function (userArray) {
      let thisUser = this.$store.state.userType;
      return userArray.indexOf(thisUser) !== -1 || thisUser === 'SuperAdmin';
    },
  }
}
