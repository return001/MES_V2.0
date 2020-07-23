<template>
  <div id="process-setting">
    <div class="process-setting-main">
      <div class="query-comp">
        <!--选择器-->
        <div class="query-comp-container" v-if="!!thisQueryOptions.processGroup">
          <div class="query-comp-select">
            <label for="process-group-query-item">工序组:</label>
            <el-select
              v-model="thisQueryOptions['processGroup'].value"
              id="process-group-query-item"
              placeholder="请选择工序组"
              size="small"
              clearable
              >
              <el-option v-for="listItem in processGroupSelectGroup"
                         :key="listItem.id"
                         :value="listItem.id"
                         :label="listItem.groupName"></el-option>
            </el-select>
          </div>
        </div>
        <div class="query-comp-container" v-for="(item, index) in queryOptions" :key="index">
          <!--纯文本框-->
          <div class="query-comp-text" v-if="item.type === 'text'">
            <label :for="item.key + index">{{item.label}}:</label>
            <el-input v-model.trim="thisQueryOptions[item.key].value" :id="item.key + index"
                      :placeholder="'请填写' + item.label" size="small" clearable></el-input>
          </div>
        </div>
        <div class="query-comp-container">
          <el-button type="info" @click="initQueryOptions" size="small">重置条件</el-button>
        </div>
        <div class="query-comp-container">
          <el-button type="primary" size="small" @click="queryData">查询</el-button>
        </div>
        <div class="query-comp-container">
          <el-button type="primary" size="small" @click="editData('add')">新增</el-button>
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

        <el-table-column
          label="操作"
          width="160"
          fixed="right"
          v-if="_getFunctionPermission(2) || _getFunctionPermission(3)"
        >
          <template slot-scope="scope">
            <el-tooltip content="上移" placement="top">
              <el-button type="text" @click="changePosition(scope, 'up')" icon="el-icon-sort-up" v-if="_getFunctionPermission(2)"></el-button>
            </el-tooltip>
            <el-tooltip content="下移" placement="top">
              <el-button type="text" @click="changePosition(scope, 'down')" icon="el-icon-sort-down" v-if="_getFunctionPermission(2)"></el-button>
            </el-tooltip>
            <el-tooltip content="编辑" placement="top">
              <el-button type="text" @click="editData('edit', scope.row)" icon="el-icon-edit-outline" v-if="_getFunctionPermission(2)"></el-button>
            </el-tooltip>
            <el-tooltip content="删除" placement="top">
              <el-button type="text" @click="deleteData(scope.row)" icon="el-icon-delete" v-if="_getFunctionPermission(3)"></el-button>
            </el-tooltip>
          </template>
        </el-table-column>
      </el-table>
    </div>
    <!--dialog component-->
    <el-dialog
      :title="editPanelTitle"
      :visible.sync="isProcessEditing"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @close="resetEditProcessForm"
      width="730px">
      <el-form
        ref="processEditForm"
        :model="processEditOptionsData"
        class="process-edit-form"
        label-position="top"
        @submit.native.prevent
        :rules="processEditOptionsRules">
        <el-form-item size="small" class="process-edit-form-comp" label="工序组" prop="processGroup">
          <el-select v-model="processEditOptionsData.processGroup" class="process-edit-form-comp-text">
            <el-option v-for="listItem in processGroupSelectGroup"
                       :key="listItem.id"
                       :value="listItem.id"
                       :label="listItem.groupName"></el-option>
          </el-select>
        </el-form-item>
        <el-form-item
          size="small"
          class="process-edit-form-comp"
          v-for="(item, index) in processEditOptions"
          :key="index"
          :label="item.label + '：'"
          :prop="item.key">
          <div class="process-edit-form-comp-text">
            <el-input
              type="text"
              :id="'edit' + item.key + index" :placeholder="'请填写' + item.label"
              clearable
              autocomplete="off"
              v-model="processEditOptionsData[item.key]"></el-input>
          </div>
        </el-form-item>
      </el-form>
      <span slot="footer" class="dialog-footer">
        <el-button size="small" @click="closeEditProcessPanel" type="info">取消</el-button>
        <el-button size="small" @click="submitEditProcess" type="primary">保存</el-button>
      </span>
    </el-dialog>
  </div>

</template>

<script>
  import {
    processQueryOptions,
    processTableColumns,
    processEditOptions,
    processEditOptionsRules,
  } from "../../../config/planConfig";
  import {axiosFetch} from "../../../utils/fetchData";
  import {
    planProcessAddUrl,
    planProcessDeleteUrl,
    planProcessEditUrl,
    planProcessSelectUrl,
    planProcessGroupGetUrl
  } from "../../../config/globalUrl";
  import {MessageBox} from 'element-ui';

  export default {
    name: "ProcessSetting",
    inject: ['reload', '_getFunctionPermission'],
    data() {
      return {
        sessionFactory:sessionStorage.getItem('factory'),
        queryOptions: processQueryOptions,
        thisQueryOptions: {
          processGroup:undefined,
        },
        processGroupSelectGroup: [],
        tableData: [],
        tableColumns: processTableColumns,
        paginationOptions: {
          currentPage: 1,
          pageSize: 65535,
          total: 0
        },
        /*编辑新增工序组*/
        isProcessEditing: false,
        processEditType: '',
        processEditOptionsData: {},
        processEditOptions: processEditOptions,
        processEditOptionsRules: processEditOptionsRules,
      }
    },
    computed: {
      editPanelTitle: function () {
        if (this.processEditType === 'edit') {
          return '编辑'
        } else if (this.processEditType === 'add') {
          return '新增'
        }
      },
    },
    created() {       //async
      this.initQueryOptions();
      this.fetchProcessGroup();     // await
      this.initEditOptions();
      this.$store.commit('pageActionLimits',this.$store.state.charactersFuncMap.map.basic.basic.process)
    },
    mounted() {
      this.fetchData()
      //传入当前是哪个页面，this.$store.state.limits 就会有相应页面的权限配置情况
      // this.$store.commit('pageActionLimits',this.$store.state.charactersFuncMap.map.basic.basic.process)
    },
    methods: {
      /*局部刷新*/
      partlyReload() {
        this.isPending = false;
        let _partlyReload = stashData => {
          let obj = {};
          stashData.forEach(item => {
            obj[item] = this.$data[item]
          });
          this.$store.commit('setStashData', obj);
          Object.assign(this.$data, this.$options.data());
          Object.assign(this.$data, this.$store.state.stashData);
          this.queryData();
          this.$store.commit('setStashData', {});
        };
        _partlyReload(['thisQueryOptions', 'processEditOptionsData',  'processGroupSelectGroup',  ])
      },


      initQueryOptions: function () {
        this.queryOptions.forEach(item => {
          this.$set(this.thisQueryOptions, item.key, {
            type: item.type,
            value: ''
          })
        });
        this.$set(this.thisQueryOptions, 'processGroup', {
          type: 'select',
          value: ''
        })
      },

      initEditOptions: function () {
        this.processEditOptions.forEach(item => {
          this.$set(this.processEditOptionsData, item.key, '')
        });
        this.$set(this.processEditOptionsData, 'processGroup', '');
      },

      /*获取工序组信息*/
      fetchProcessGroup: async function () {
        return new Promise(resolve => {
          axiosFetch({
            url: planProcessGroupGetUrl
          }).then(response => {
            if (response.data.result === 200) {
              if(this.sessionFactory ==='1'){
                this.processGroupSelectGroup = response.data.data.list;
              }else{
                response.data.data.list.forEach(item=>{
                  if(item.factoryId === Number(this.sessionFactory)){
                    this.processGroupSelectGroup.push(item)
                  }
                })
              }
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.$alertDanger('获取工序组信息失败，请刷新重试');
          }).finally(() => {
            resolve();
          })
        });
      },

      indexMethod: function (index) {
        return index + (this.paginationOptions.currentPage - 1) * this.paginationOptions.pageSize + 1
      },

      queryData: function () {
        this.paginationOptions.pageNo = 1;
        this.paginationOptions.total = 0;
        this.fetchData();
      },

      fetchData: function () {
        if(this.$store.state.limits.select !== true){
          this.$alertWarning('暂无查询权限')
          return
        }
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          let options = {
            url: planProcessSelectUrl,
            data: {
              pageNo: this.paginationOptions.currentPage,
              pageSize: this.paginationOptions.pageSize,
              factory:this.sessionFactory === '1'? '0':this.sessionFactory,
            }
          };
          Object.keys(this.thisQueryOptions).forEach(item => {
            if (this.thisQueryOptions[item].value !== "") {
              options.data[item] = this.thisQueryOptions[item].value
            }
          });
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
        if (!!this.$refs['processEditForm']) {
          this.$refs['processEditForm'].clearValidate();
        }
        if (type === 'edit') {
          if(this.$store.state.limits.update !== true){
            this.$alertWarning('暂无编辑权限')
            return
          }
          this.processEditType = 'edit';
          Object.keys(val).forEach(item => {
            this.processEditOptions.forEach(option => {
              if (item === option.key && val[item] !== null) {
                this.$set(this.processEditOptionsData, item, val[item])
              }
            })
          });
          this.$set(this.processEditOptionsData, 'processGroup', val.processGroup);
          this.$set(this.processEditOptionsData, 'id', val.id);

          this.isProcessEditing = true;
        } else if (type === 'add') {
          if(this.$store.state.limits.add !== true){
            this.$alertWarning('暂无新增权限')
            return
          }
          this.processEditType = 'add';
          this.isProcessEditing = true;
        }
      },
      closeEditProcessPanel: function () {
        this.isProcessEditing = false;
      },

      submitEditProcess: function () {
        this.$refs['processEditForm'].validate((isValid) => {
          if (isValid) {
            this.isPending = true;
            this.$openLoading();
            let options = {
              url: '',
              data: this.processEditOptionsData
            };
            if (this.processEditType === 'edit') {
              options.url = planProcessEditUrl
            } else if (this.processEditType === 'add') {
              options.url = planProcessAddUrl
            }
            options.data.factory = this.sessionFactory === '1'?'0':this.sessionFactory;
            axiosFetch(options).then(response => {
              if (response.data.result === 200) {
                this.$alertSuccess('操作成功');
                this.resetEditProcessForm();
                this.closeEditProcessPanel();
              } else {
                this.$alertWarning(response.data.data)
              }
            }).catch(err => {
              this.$alertDanger("未知错误")
            }).finally(() => {
              this.isPending = false;
              this.$closeLoading();
              this.partlyReload();
              //this.reload();
            })
          } else {
            this.$alertInfo('请完善表单信息')
          }
        })
      },
      resetEditProcessForm: function () {
        this.processEditOptionsData = {};
        this.$refs['processEditForm'].clearValidate();
        this.initEditOptions();
      },
      deleteData: function (val) {
        if(this.$store.state.limits.delete !== true){
          this.$alertWarning('暂无删除权限')
          return
        }
        MessageBox.confirm('将删除该配置，是否继续?', '提示', {
          confirmButtonText: '确认',
          cancelButtonText: '取消',
          type: 'warning'
        }).then(() => {
          this.$openLoading();
          this.isPending = true;
          axiosFetch({
            url: planProcessDeleteUrl,
            data: {
              id: val.id
            }
          }).then(response => {
            if (response.data.result === 200) {
              this.$alertSuccess('删除成功');
              this.partlyReload();
              //this.reload();
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
            position = this.tableData[val.$index - 1].id
          } else {
            return
          }
        } else if (direction === 'down') {
          if (val.$index < (this.tableData.length - 1)) {
            position = this.tableData[val.$index + 1].id
          } else {
            return
          }
        }
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          //初始化要提交的值，行内容填充
          this.processEditOptions.forEach(item => {
            this.$set(this.processEditOptionsData, item.key, '')
          });
          Object.keys(val.row).forEach(item => {
            this.processEditOptions.forEach(option => {
              if (item === option.key && val.row[item] !== null) {
                this.$set(this.processEditOptionsData, item, val.row[item])
              }
            })
          });
          this.$set(this.processEditOptionsData, 'id', val.row.id);
          this.$set(this.processEditOptionsData, 'position', position);
        }
        axiosFetch({
          url: planProcessEditUrl,
          data: this.processEditOptionsData
        }).then(response => {
          if (response.data.result === 200) {
            this.$alertSuccess('操作成功');
            this.processEditOptionsData = {};
            this.isPending = false;
            this.queryData();
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
  #process-setting /deep/ .el-button i{
    font-size: 17px;
    font-weight: bold;
  }

  #process-setting {
    position: relative;
    width: 100%;
    min-height: 600px;
    height: 100%;
  }

  .process-setting-main {
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


  .process-edit-form {
    display: flex;
    flex-wrap: wrap;
  }

  .process-edit-form-comp {
    padding: 0 10px;
  }

  .process-edit-form-comp label {
    font-size: 15px;
    line-height: 24px;
    padding: 0;
  }

  .process-edit-form-comp-text {
    width: 210px;
  }

  .process-edit-form-comp-text /deep/ .el-input {
    width: 210px;
  }
</style>
