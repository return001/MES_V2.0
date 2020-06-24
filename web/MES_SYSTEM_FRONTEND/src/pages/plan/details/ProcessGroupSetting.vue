<template>
  <div id="process-group-setting">
    <div class="process-group-setting-main">
      <div class="query-comp">
        <div class="query-comp-container" v-for="(item, index) in queryOptions" :key="index">
          <!--纯文本框-->
          <div class="query-comp-text" v-if="item.type === 'text'">
            <label :for="item.key + index">{{item.label}}:</label>
            <el-input v-model="thisQueryOptions[item.key].value" :id="item.key + index"
                      :placeholder="'请填写' + item.label" size="small"></el-input>
          </div>
        </div>
        <div class="query-comp-container">
          <el-button type="info" @click="initQueryOptions" size="small">重置条件</el-button>
        </div>
        <div class="query-comp-container">
          <el-button type="primary" size="small" @click="queryData">查询</el-button>
        </div>
<!--        <div class="query-comp-container">-->
<!--          <el-button type="primary" size="small" @click="editData('add')">新增</el-button>-->
<!--        </div>-->
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

<!--        <el-table-column-->
<!--          label="操作"-->
<!--          width="160"-->
<!--          fixed="right"-->
<!--        >-->
<!--          <template slot-scope="scope">-->
<!--&lt;!&ndash;            <el-tooltip content="上移" placement="top">&ndash;&gt;-->
<!--&lt;!&ndash;              <el-button type="text" @click="changePosition(scope, 'up')" icon="el-icon-sort-up"></el-button>&ndash;&gt;-->
<!--&lt;!&ndash;            </el-tooltip>&ndash;&gt;-->
<!--&lt;!&ndash;            <el-tooltip content="下移" placement="top">&ndash;&gt;-->
<!--&lt;!&ndash;              <el-button type="text" @click="changePosition(scope, 'down')" icon="el-icon-sort-down"></el-button>&ndash;&gt;-->
<!--&lt;!&ndash;            </el-tooltip>&ndash;&gt;-->
<!--            <el-tooltip content="编辑" placement="top">-->
<!--              <el-button type="text" @click="editData('edit', scope.row)" icon="el-icon-edit-outline"></el-button>-->
<!--            </el-tooltip>-->
<!--            <el-tooltip content="删除" placement="top">-->
<!--              <el-button type="text" @click="deleteData(scope.row)" icon="el-icon-delete"></el-button>-->
<!--            </el-tooltip>-->
<!--          </template>-->
<!--        </el-table-column>-->
      </el-table>
    </div>
    <!--dialog component-->
    <el-dialog
      :title="editPanelTitle"
      :visible.sync="isProcessGroupEditing"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @closed="resetEditProcessGroupForm"
      width="650px">
      <el-form
        ref="processGroupEditForm"
        :model="processGroupEditOptionsData"
        class="process-group-edit-form"
        label-position="top"
        @submit.native.prevent
        :rules="processGroupEditOptionsRules">
        <el-form-item
          size="small"
          class="process-group-edit-form-comp"
          v-for="(item, index) in processGroupEditOptions"
          :key="index"
          :label="item.label + '：'"
          :prop="item.key">
          <div class="process-group-edit-form-comp-text"  v-if="item.type === 'text'">
            <el-input
              type="text"
              :id="'edit' + item.key + index" :placeholder="'请填写' + item.label"
              clearable
              autocomplete="off"
              v-model="processGroupEditOptionsData[item.key]"></el-input>
          </div>
          <div class="process-group-edit-form-comp-text" v-if="item.type === 'select'">
            <el-select
            size="small"
            v-model="processGroupEditOptionsData[item.key]"
            :id="item.key + index"
            :placeholder="'请选择'+item.label"
            autocomplete="off"
            clearable
            >
              <el-option v-for="(item,index) in options"
              :key="item.id"
              :value="item.id"
              :label="item.groupName"
              >

              </el-option>
            </el-select>
          </div>
        </el-form-item>
      </el-form>
      <span slot="footer" class="dialog-footer">
        <el-button @click="closeEditProcessGroupPanel" type="info">取消</el-button>
        <el-button @click="submitEditProcessGroup" type="primary">保存</el-button>
      </span>
    </el-dialog>
  </div>

</template>

<script>
  import {
    processGroupQueryOptions,
    processGroupTableColumns,
    processGroupEditOptions,
    processGroupEditOptionsRules,
  } from "../../../config/planConfig";
  import {axiosFetch} from "../../../utils/fetchData";
  import {
    planProcessGroupAddUrl,
    planProcessGroupDeleteUrl,
    planProcessGroupEditUrl,
    planProcessGroupSelectUrl
  } from "../../../config/globalUrl";
  import {MessageBox} from 'element-ui'

  export default {
    name: "ProcessGroupSetting",
    inject: ['reload'],
    data() {
      return {
        sessionFactory:sessionStorage.getItem('factory'),
        queryOptions: [],
        thisQueryOptions: {},
        tableData: [],
        options:[],                //新增和编辑时 要用到的 所属工序组的数据
        tableColumns: processGroupTableColumns,
        paginationOptions: {
          currentPage: 1,
          pageSize: 65535,
          total: 0
        },
        /*编辑新增工序组*/
        isProcessGroupEditing: false,
        processGroupEditType: '',
        processGroupEditOptionsData: {},
        processGroupEditOptions: processGroupEditOptions,
        processGroupEditOptionsRules:processGroupEditOptionsRules,
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
        this.queryOptions = JSON.parse(JSON.stringify(processGroupQueryOptions));
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
            url: planProcessGroupSelectUrl,
            data: {
              pageNo: this.paginationOptions.currentPage,
              pageSize: this.paginationOptions.pageSize,
              factory:this.sessionFactory === '1'? 0 :this.sessionFactory,
            }
          };
          // if(sessionFactory !== "null"){
          //   options.data.factory=sessionFactory
          // }
          Object.keys(this.thisQueryOptions).forEach(item => {
            options.data[item] = JSON.parse(JSON.stringify(this.thisQueryOptions[item])).value
          });
          // if (this.queryString !== '') {
          //   options.data.filter = this.queryString;
          // }
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
      editData: function (type, val) {
        //初始化要提交的值
        this.processGroupEditOptions.forEach(item => {
          this.$set(this.processGroupEditOptionsData, item.key, '')
        });
        let options = {
          url: planProcessGroupSelectUrl,
          data: {
            pageNo: this.paginationOptions.currentPage,
            pageSize: this.paginationOptions.pageSize,
            factory:this.sessionFactory === '1'? 0 :this.sessionFactory,

          }
        };
        axiosFetch(options).then(response => {
          if (response.data.result === 200) {
            response.data.data.list.forEach(item=>{
              if(item.parentGroup === null){
                this.options.push(item)
              }});
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
        if (type === 'edit') {
          this.processGroupEditType = 'edit';
          Object.keys(val).forEach(item => {
            this.processGroupEditOptions.forEach(option => {
              if (item === option.key && val[item] !== null) {
                this.$set(this.processGroupEditOptionsData, item, val[item])
              }
            })
          });
          this.$set(this.processGroupEditOptionsData, 'id', val.id);
          this.isProcessGroupEditing = true;
        } else if (type === 'add') {
          this.processGroupEditType = 'add';
          this.isProcessGroupEditing = true;
        }
      },
      closeEditProcessGroupPanel: function () {
        this.isProcessGroupEditing = false;
      },

      submitEditProcessGroup: function () {
        this.$refs['processGroupEditForm'].validate((isValid) => {
          if (isValid && !this.isPending) {
            this.isPending = true;
            this.$openLoading();
            this.processGroupEditOptionsData.factory = this.sessionFactory ==='1'?'0':this.sessionFactory;
            let options = {
              url: '',
              data: this.processGroupEditOptionsData,
            };
            if (this.processGroupEditType === 'edit') {
              options.url = planProcessGroupEditUrl
            } else if (this.processGroupEditType === 'add') {
              options.url = planProcessGroupAddUrl
            }
            axiosFetch(options).then(response => {
              if (response.data.result === 200) {
                this.$alertSuccess('操作成功');
                this.resetEditProcessGroupForm();
                this.closeEditProcessGroupPanel();
              } else {
                this.$alertWarning(response.data.data)
              }
            }).catch(err => {
              this.$alertDanger("未知错误")
            }).finally(() => {
              this.isPending = false;
              this.$closeLoading();
              this.reload();
            })
          } else {
            this.$alertInfo('请完善表单信息')
          }
        })
      },
      resetEditProcessGroupForm: function () {
        this.processGroupEditOptionsData = {};
        this.$refs['processGroupEditForm'].clearValidate();
      },
      deleteData: function (val) {
        MessageBox.confirm('将删除该配置，是否继续?', '提示', {
          confirmButtonText: '确认',
          cancelButtonText: '取消',
          type: 'warning'
        }).then(() => {
          this.$openLoading();
          this.isPending = true;
          axiosFetch({
            url: planProcessGroupDeleteUrl,
            data: {
              id: val.groupId,
            }
          }).then(response => {
            if (response.data.result === 200) {
              this.$alertSuccess('删除成功');
              this.reload();
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.$alertDanger("未知错误")
          }).finally(() => {
            this.$closeLoading();
            this.isPending = false;
          })
        }).catch(() => {

        })
      },

      changePosition: function (val, direction) {
        let position;
        if (direction === 'up') {
          if (val.$index > 0) {
            position = this.tableData[val.$index - 1].groupId
          } else {
            return
          }
        } else if (direction === 'down') {
          if (val.$index < (this.tableData.length - 1)) {
            position = this.tableData[val.$index + 1].groupId
          } else {
            return
          }
        }
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          //初始化要提交的值，行内容填充
          this.processGroupEditOptions.forEach(item => {
            this.$set(this.processGroupEditOptionsData, item.key, '')
          });
          Object.keys(val.row).forEach(item => {
            this.processGroupEditOptions.forEach(option => {
              if (item === option.key && val.row[item] !== null) {
                this.$set(this.processGroupEditOptionsData, item, val.row[item])
              }
            })
          });
          this.$set(this.processGroupEditOptionsData, 'id', val.row.groupId);
          this.$set(this.processGroupEditOptionsData, 'position', position);
          this.$set(this.processGroupEditOptionsData, 'factory', Number(this.sessionFactory === '1'?'0':this.sessionFactory));
        }
        axiosFetch({
          url: planProcessGroupEditUrl,
          data: this.processGroupEditOptionsData
        }).then(response => {
          if (response.data.result === 200) {
            this.$alertSuccess('操作成功');
            this.processGroupEditOptionsData = {};
            this.reload();
          } else {
            this.$alertWarning(response.data.data)
          }
        }).catch(err => {
          this.$alertDanger("未知错误")
        }).finally(() => {
          this.isPending = false;
          this.$closeLoading();
        })

      }
    }
  }
</script>

<style scoped>
  #process-group-setting /deep/ .el-button i{
    font-size: 17px;
    font-weight: bold;
  }

  #process-group-setting {
    position: relative;
    width: 100%;
    min-height: 600px;
    height: 100%;
  }

  .process-group-setting-main {
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
