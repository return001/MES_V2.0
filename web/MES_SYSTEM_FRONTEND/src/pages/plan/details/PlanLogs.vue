<template>
  <div id="plan-logs">
    <div class="plan-logs-main">
      <div class="query-comp">
        <div class="query-comp-container" v-for="(item, index) in queryOptions" :key="index">
          <!--纯文本框-->
          <div class="query-comp-text" v-if="item.type === 'text'">
            <label :for="item.key + index">{{item.label}}:</label>
            <el-input v-model="thisQueryOptions[item.key].value" :id="item.key + index"
                      :placeholder="'请填写' + item.label" size="small"></el-input>
          </div>
          <!--时间范围-->
          <div class="query-comp-timerange" v-else-if="item.type === 'timeRange'">
            <label :for="item.key + index">{{item.label}}:</label>
            <el-date-picker
              :id="item.key + index"
              v-model="thisQueryOptions[item.key].value"
              type="daterange"
              :picker-options="timePickerOptions"
              prefix-icon="el-icon-date"
              range-separator="至"
              start-placeholder="开始日期"
              end-placeholder="结束日期"
              value-format="yyyy-MM-dd"
              autocomplete="off"
              size="small">
            </el-date-picker>
          </div>
        </div>
        <div class="query-comp-container">
          <el-button type="info" @click="initQueryOptions" size="small">重置条件</el-button>
        </div>
        <div class="query-comp-container">
          <el-button type="primary" size="small" @click="queryData">查询</el-button>
        </div>
      </div>
    </div>

    <div class="content-comp">
      <el-table
        :data="tableData"
        max-height="560"
        ref="tablecomponent"
        stripe>
        <el-table-column v-for="(item, index) in tableColumns"
                         :key="index"
                         :prop="item.key"
                         :label="item.label"
                         :min-width="item['min-width']"
                         :formatter="item.formatter">
        </el-table-column>

        <el-table-column
          type="index"
          :index="indexMethod"
          fixed="left"
          width="60">
        </el-table-column>
      </el-table>
      <!--分页控制-->
      <el-pagination
        background
        :current-page.sync="paginationOptions.currentPage"
        :page-sizes="[20, 40, 80]"
        :page-size.sync="paginationOptions.pageSize"
        layout="total, sizes, prev, pager, next, jumper"
        :total="paginationOptions.total"
        @current-change="pageChange"
        @size-change="queryData"
        class="page-pagination">
      </el-pagination>
    </div>
  </div>

</template>

<script>
  import {
    logsQueryOptions,
    logsTableColumns, sessionFactory,

  } from "../../../config/planConfig";
  import {axiosFetch} from "../../../utils/fetchData";
  import {planLogsSelectUrl} from "../../../config/globalUrl";
  import {MessageBox} from 'element-ui'

  export default {
    name: "PlanLogs",
    inject: ['reload'],
    data() {
      return {
        queryOptions: logsQueryOptions,
        thisQueryOptions: {},
        tableData: [],
        tableColumns: logsTableColumns,

        paginationOptions: {
          currentPage: 1,
          pageSize: 20,
          total: 0
        },
        timePickerOptions: {
          shortcuts: [{
            text: '最近一周',
            onClick(picker) {
              const end = new Date();
              const start = new Date();
              start.setTime(start.getTime() - 3600 * 1000 * 24 * 7);
              picker.$emit('pick', [start, end]);
            }
          }, {
            text: '最近一个月',
            onClick(picker) {
              const end = new Date();
              const start = new Date();
              start.setTime(start.getTime() - 3600 * 1000 * 24 * 30);
              picker.$emit('pick', [start, end]);
            }
          }, {
            text: '最近三个月',
            onClick(picker) {
              const end = new Date();
              const start = new Date();
              start.setTime(start.getTime() - 3600 * 1000 * 24 * 90);
              picker.$emit('pick', [start, end]);
            }
          }]
        },
      }
    },
    computed: {
      editPanelTitle: function () {
        if (this.processGroupEditType === 'edit') {
          return '编辑'
        } else if (this.processGroupEditType === 'add') {
          return '新增'
        }
      },
    },
    created() {
      this.initQueryOptions();
    },
    mounted() {
      this.fetchData()
    },
    methods: {
      initQueryOptions: function () {
        this.queryOptions.forEach(item => {
          this.$set(this.thisQueryOptions, item.key, {
            type: item.type,
            value: ''
          })
        })
      },


      indexMethod: function (index) {
        return index + (this.paginationOptions.currentPage - 1) * this.paginationOptions.pageSize + 1
      },

      createQueryString: function () {
        let queryString = "";
        Object.keys(this.thisQueryOptions).forEach(item => {
          let subObj = this.thisQueryOptions[item];
          let itemCopy = item.replace(/([A-Z])/g, "_$1").toLowerCase(); //驼峰转下划线
          switch (subObj.type) {
            case 'text':
              //possible value: null [empty string] [string]
              if (!!subObj.value) {
                queryString += queryString.length === 0 ? (itemCopy + '#like#' + subObj.value) : ('#&#' + itemCopy + '#like#' + subObj.value)
              }
              break;
            case 'select':
              //possible value: [empty string] [string]
              if (subObj.value !== '') {
                queryString += queryString.length === 0 ? (itemCopy + '#=#' + subObj.value) : ('#&#' + itemCopy + '#=#' + subObj.value)
              }
              break;
            case 'timeRange':
              //possible value: null [array] [empty string]
              if (!!subObj.value) {
                queryString += (queryString.length === 0 ? (itemCopy + '#>#' + subObj.value[0]) : ('#&#' + itemCopy + '#>#' + subObj.value[0])) + '#&#' + itemCopy + '#>#' + subObj.value[1]
              }
              break;
          }
        });
        this.queryString = queryString;
      },

      pageChange: function () {
        this.createQueryString();
        this.fetchData();
      },

      //点击查询
      queryData: function () {
        this.paginationOptions.pageNo = 1;
        this.paginationOptions.total = 0;
        this.createQueryString();
        this.fetchData();
      },

      fetchData: function () {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          let options = {
            url: planLogsSelectUrl,
            data: {
              pageNo: this.paginationOptions.currentPage,
              pageSize: this.paginationOptions.pageSize,
            }
          };
          // if (this.queryString !== '') {
          //   options.data.filter = this.queryString;
          // }
          Object.keys(this.thisQueryOptions).forEach(item => {
            options.data[item] = JSON.parse(JSON.stringify(this.thisQueryOptions[item])).value
          });
          options.data.startTime = options.data.orderDate[0]
          options.data.endTime = options.data.orderDate[1]
          options.data.factory = sessionFactory
          axiosFetch(options).then(response => {
            if (response.data.result === 200) {
              this.tableData = response.data.data.list;
              this.paginationOptions.currentPage = response.data.data.pageNumber;
              this.paginationOptions.total = response.data.data.totalRow;
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.$alertDanger('未知错误')
          }).finally(() => {
            this.isPending = false;
            this.$closeLoading();
          })
        }
      },
      // editData: function (type, val) {
      //   //初始化要提交的值
      //   this.processGroupEditOptions.forEach(item => {
      //     this.$set(this.processGroupEditOptionsData, item.key, '')
      //   });
      //   if (type === 'edit') {
      //     this.processGroupEditType = 'edit';
      //     Object.keys(val).forEach(item => {
      //       this.processGroupEditOptions.forEach(option => {
      //         if (item === option.key && val[item] !== null) {
      //           this.$set(this.processGroupEditOptionsData, item, val[item])
      //         }
      //       })
      //     });
      //     this.$set(this.processGroupEditOptionsData, 'id', val.id);
      //     this.isProcessGroupEditing = true;
      //   } else if (type === 'add') {
      //     this.processGroupEditType = 'add';
      //     this.isProcessGroupEditing = true;
      //   }
      // },
      // closeEditProcessGroupPanel: function () {
      //   this.isProcessGroupEditing = false;
      // },
      //
      // submitEditProcessGroup: function () {
      //   this.$refs['processGroupEditForm'].validate((isValid) => {
      //     if (isValid && !this.isPending) {
      //       this.isPending = true;
      //       this.$openLoading();
      //       let options = {
      //         url: '',
      //         data: this.processGroupEditOptionsData
      //       };
      //       if (this.processGroupEditType === 'edit') {
      //         options.url = planProcessGroupEditUrl
      //       } else if (this.processGroupEditType === 'add') {
      //         options.url = planProcessGroupAddUrl
      //       }
      //       axiosFetch(options).then(response => {
      //         if (response.data.result === 200) {
      //           this.$alertSuccess('操作成功');
      //           this.resetEditProcessGroupForm();
      //           this.closeEditProcessGroupPanel();
      //         } else {
      //           this.$alertWarning(response.data.data)
      //         }
      //       }).catch(err => {
      //         this.$alertDanger("未知错误")
      //       }).finally(() => {
      //         this.isPending = false;
      //         this.$closeLoading();
      //         this.reload();
      //       })
      //     } else {
      //       this.$alertInfo('请完善表单信息')
      //     }
      //   })
      // },
      // resetEditProcessGroupForm: function () {
      //   this.processGroupEditOptionsData = {};
      //   this.$refs['processGroupEditForm'].clearValidate();
      // },
      // deleteData: function (val) {
      //   MessageBox.confirm('将删除该配置，是否继续?', '提示', {
      //     confirmButtonText: '确认',
      //     cancelButtonText: '取消',
      //     type: 'warning'
      //   }).then(() => {
      //     this.$openLoading();
      //     this.isPending = true;
      //     axiosFetch({
      //       url: planProcessGroupDeleteUrl,
      //       data: {
      //         id: val.id
      //       }
      //     }).then(response => {
      //       if (response.data.result === 200) {
      //         this.$alertSuccess('删除成功');
      //         this.reload();
      //       } else {
      //         this.$alertWarning(response.data.data)
      //       }
      //     }).catch(err => {
      //       this.$alertDanger("未知错误")
      //     }).finally(() => {
      //       this.$closeLoading();
      //       this.isPending = false;
      //     })
      //   }).catch(() => {})
      // },changePosition: function (val, direction) {
      //   let position;
      //   if (direction === 'up') {
      //     if (val.$index > 0) {
      //       position = this.tableData[val.$index - 1].id
      //     } else {
      //       return
      //     }
      //   } else if (direction === 'down') {
      //     if (val.$index < (this.tableData.length - 1)) {
      //       position = this.tableData[val.$index + 1].id
      //     } else {
      //       return
      //     }
      //   }
      //   if (!this.isPending) {
      //     this.isPending = true;
      //     this.$openLoading();
      //     //初始化要提交的值，行内容填充
      //     this.processGroupEditOptions.forEach(item => {
      //       this.$set(this.processGroupEditOptionsData, item.key, '')
      //     });
      //     Object.keys(val.row).forEach(item => {
      //       this.processGroupEditOptions.forEach(option => {
      //         if (item === option.key && val.row[item] !== null) {
      //           this.$set(this.processGroupEditOptionsData, item, val.row[item])
      //         }
      //       })
      //     });
      //     this.$set(this.processGroupEditOptionsData, 'id', val.row.id);
      //     this.$set(this.processGroupEditOptionsData, 'position', position);
      //   }
      //   axiosFetch({
      //     url: planProcessGroupEditUrl,
      //     data: this.processGroupEditOptionsData
      //   }).then(response => {
      //     if (response.data.result === 200) {
      //       this.$alertSuccess('操作成功');
      //       this.processGroupEditOptionsData = {};
      //       this.reload();
      //     } else {
      //       this.$alertWarning(response.data.data)
      //     }
      //   }).catch(err => {
      //     this.$alertDanger("未知错误")
      //   }).finally(() => {
      //     this.isPending = false;
      //     this.$closeLoading();
      //   })
      //
      // }
    }
  }
</script>

<style scoped>
  #plan-logs /deep/ .el-button i{
    font-size: 17px;
    font-weight: bold;
  }

  #plan-logs {
    position: relative;
    width: 100%;
    min-height: 600px;
    height: 100%;
  }

  .plan-logs-main {
  }

  .query-comp {
    background-color: #ffffff;
    border: 1px solid #eeeeee;
    border-radius: 5px;
    padding: 10px 20px;
    display: flex;
    flex-wrap: wrap;
    min-height: 60px;
  }

  .query-comp-container {
    margin: 0 16px 10px 0;
  }

  .query-comp label {
    display: block;
    line-height: 24px;
    font-size: 14px;
  }

  .query-comp-text {
    width: 220px;
  }

  .query-comp-select {
    width: 220px !important;
  }

  .query-comp-select .el-select {
    width: 100%;
  }

  .query-comp-select /deep/ input {
    padding-right: 0;
  }

  .query-comp-container .el-button {
    margin-top: 24px;
    width: 80px;
  }

  .content-comp {
    background-color: #ffffff;
    border: 1px solid #eeeeee;
    border-radius: 5px;
    padding: 10px;
    margin-top: 10px;
  }

  .process-group-edit-form {
    display: flex;
    flex-wrap: wrap;
  }

  .process-group-edit-form-comp {
    padding: 0 10px;
  }

  .process-group-edit-form-comp label {
    font-size: 15px;
    line-height: 24px;
    padding: 0;
  }

  .process-group-edit-form-comp-text {
    width: 210px;
  }

  .process-group-edit-form-comp-text /deep/ .el-input {
    width: 210px;
  }
</style>
