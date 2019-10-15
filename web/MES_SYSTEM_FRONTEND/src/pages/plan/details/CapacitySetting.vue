<template>
  <div id="capacity-setting">
    <div class="capacity-setting-main">
      <div class="query-comp">
        <div class="query-comp-container" v-for="(item, index) in queryOptions" :key="index">
          <!--纯文本框-->
          <div class="query-comp-text" v-if="item.type === 'text'">
            <label :for="item.key + index">{{item.label}}:</label>
            <el-input v-model="thisQueryOptions[item.key].value" :id="item.key + index"
                      :placeholder="'请填写' + item.label" size="small"></el-input>
          </div>
        </div>
        <!--选择器-->
        <div class="query-comp-container" v-if="!!thisQueryOptions.process">
          <div class="query-comp-select">
            <label for="process-query-item">工序:</label>
            <el-select
              v-model="thisQueryOptions['process'].value"
              id="process-query-item"
              placeholder="请选择工序"
              size="small">
              <el-option value="" label="未选择"></el-option>
              <el-option v-for="listItem in processSelectGroupSrc"
                         :key="listItem.id"
                         :value="listItem.id"
                         :label="listItem.processName"></el-option>
            </el-select>
          </div>
        </div>
        <div class="query-comp-container">
          <el-button type="info" @click="initQueryOptions" size="small">重置条件</el-button>
        </div>
        <div class="query-comp-container">
          <el-button type="primary" size="small" @click="queryData">查询</el-button>
        </div>
        <div class="query-comp-container"
             v-if="permissionControl(['engineer'])">
          <el-button type="primary" size="small" @click="editData('add')">新增</el-button>
        </div>
      </div>
      <div class="content-comp">
        <el-table
          :data="tableData"
          max-height="560"
          :span-method="detailsTableSpanMethod"
          border
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
            v-if="permissionControl(['engineer'])"
          >
            <template slot-scope="scope">
              <el-tooltip content="上移" placement="top">
                <el-button type="text" @click="changePosition(scope, 'up')" icon="el-icon-sort-up"></el-button>
              </el-tooltip>
              <el-tooltip content="下移" placement="top">
                <el-button type="text" @click="changePosition(scope, 'down')" icon="el-icon-sort-down"></el-button>
              </el-tooltip>
              <el-tooltip content="编辑" placement="top">
                <el-button type="text" @click="editData('edit', scope.row)" icon="el-icon-edit-outline"></el-button>
              </el-tooltip>
              <el-tooltip content="删除" placement="top">
                <el-button type="text" @click="deleteData(scope.row)" icon="el-icon-delete"></el-button>
              </el-tooltip>
            </template>
          </el-table-column>
        </el-table>
      </div>
    </div>

    <!--dialog component-->
    <el-dialog
      :title="editPanelTitle"
      :visible.sync="isCapacityEditing"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @closed="resetEditCapacityForm"
      width="730px">
      <el-form
        ref="capacityEditForm"
        :model="capacityEditOptionsData"
        class="capacity-edit-form"
        label-position="top"
        @submit.native.prevent
        :rules="capacityEditOptionsRules">
        <el-form-item class="capacity-edit-form-comp" label="工序组" prop="processGroup">
          <el-select v-model="capacityEditOptionsData.processGroup" class="capacity-edit-form-comp-text"
                     @change="processGroupEditChange" placeholder="请选择工序组">
            <el-option value="" label="未选择"></el-option>
            <el-option v-for="listItem in processGroupSelectGroup"
                       :key="listItem.id"
                       :value="listItem.id"
                       :label="listItem.groupName"></el-option>
          </el-select>
        </el-form-item>
        <el-form-item class="capacity-edit-form-comp" label="工序" prop="process">
          <el-select v-model="capacityEditOptionsData.process" placeholder="请选择工序" class="capacity-edit-form-comp-text"
                     :disabled="processGroupSelectGroup.length === 0">
            <el-option value="" label="未选择"></el-option>
            <el-option v-for="listItem in processSelectGroup"
                       :key="listItem.id"
                       :value="listItem.id"
                       :label="listItem.processName"></el-option>
          </el-select>
        </el-form-item>
        <el-form-item
          class="capacity-edit-form-comp"
          v-for="(item, index) in capacityEditOptions"
          :key="index"
          :label="item.label + '：'"
          :prop="item.key">
          <div class="capacity-edit-form-comp-text" v-if="item.type === 'text'">
            <el-input
              type="text"
              :id="'edit' + item.key + index" :placeholder="'请填写' + item.label"
              clearable
              autocomplete="off"
              v-model="capacityEditOptionsData[item.key]"></el-input>
          </div>
          <div class="capacity-edit-form-comp-text" v-if="item.type === 'date'">
            <el-date-picker
              :id="'edit' + item.key + index"
              v-model="capacityEditOptionsData[item.key]"
              type="date"
              placeholder="选择日期"
              value-format="yyyy-MM-dd"></el-date-picker>
          </div>
          <div class="capacity-edit-form-comp-textarea" v-if="item.type === 'textArea'">
            <el-input type="textarea"
                      :rows="4"
                      :id="'edit' + item.key + index" :placeholder="'请填写' + item.label"
                      clearable
                      autocomplete="off"
                      v-model="capacityEditOptionsData[item.key]"></el-input>
          </div>
        </el-form-item>
      </el-form>
      <span slot="footer" class="dialog-footer">
        <el-button @click="closeEditCapacityPanel" type="info">取消</el-button>
        <el-button @click="submitEditCapacity" type="primary">保存</el-button>
      </span>
    </el-dialog>
  </div>
</template>

<script>
  import {
    capacityQueryOptions,
    capacityTableColumns,
    capacityEditOptions,
    capacityEditOptionsRules
  } from "../../../config/planConfig";
  import {
    planCapacityAddUrl,
    planCapacityDeleteUrl,
    planCapacityEditUrl,
    planCapacitySelectUrl,
    planProcessGroupGetUrl,
    planProcessGetUrl
  } from "../../../config/globalUrl";
  import {axiosFetch} from "../../../utils/fetchData";
  import {MessageBox} from "element-ui"

  export default {
    name: "CapacitySetting",
    inject: ['reload'],
    data() {
      return {
        queryOptions: [],
        thisQueryOptions: {},
        processSelectGroupSrc: [], //工序信息 源
        processSelectGroup: [], //工序信息
        processGroupSelectGroup: [], //工序组信息
        tableData: [],
        tableColumns: [],
        paginationOptions: {
          currentPage: 1,
          pageSize: 65535,
          total: 0
        },
        /*编辑新增产能信息*/
        isCapacityEditing: false,
        capacityEditType: '',
        capacityEditOptionsData: {
          'process': ''
        },


        mergeData: {},//合并行的记录
        mergePos: {},//mergeData中每项的索引
        mergeProp: ['softModel'],
        mergeKeys: ['softModel'],

      }
    },
    computed: {
      capacityEditOptions: function () {
        return capacityEditOptions
      },
      capacityEditOptionsRules: function () {
        return capacityEditOptionsRules
      },
      editPanelTitle: function () {
        if (this.capacityEditType === 'edit') {
          return '编辑'
        } else if (this.capacityEditType === 'add') {
          return '新增'
        }
      },
    },
    async created() {
      await this.dataPreload();
      this.initQueryOptions();
      this.initTableColumns();
    },
    mounted() {
      this.fetchData();
    },
    methods: {
      /**
       **@description: 权限控制-显示隐藏
       **@date: 2019/8/13 11:39
       **@author: DarkNin
       **@method: permissionControl
       **@params: Array[] 可显示的用户
       */
      permissionControl: function (userArray) {
        let thisUser = this.$store.state.userType;
        if (userArray.indexOf(thisUser) !== -1 || thisUser === 'SuperAdmin') {
          return true
        }
      },

      initQueryOptions: function () {
        this.queryOptions = JSON.parse(JSON.stringify(capacityQueryOptions));
        this.queryOptions.forEach(item => {
          this.$set(this.thisQueryOptions, item.key, {
            type: item.type,
            value: ''
          })
        });
        this.$set(this.thisQueryOptions, 'process', {
          type: 'select',
          value: ''
        })
      },
      initTableColumns: function () {
        this.tableColumns = JSON.parse(JSON.stringify(capacityTableColumns))
      },

      initEditOptions: function () {
        this.capacityEditOptions.forEach(item => {
          this.$set(this.capacityEditOptionsData, item.key, '')
        });
        this.$set(this.capacityEditOptionsData, 'processGroup', '');
        this.$set(this.capacityEditOptionsData, 'process', '');
      },

      dataPreload: async function () {
        return new Promise(resolve => {
          Promise.all([this.fetchProcessGroup(), this.fetchProcess()]).then(() => {
            resolve();
          })
        })
      },
      /*获取工序信息*/
      fetchProcess: function () {
        return new Promise(resolve => {
          axiosFetch({
            url: planProcessGetUrl
          }).then(response => {
            if (response.data.result === 200) {
              this.processSelectGroupSrc = response.data.data.list;
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.$alertDanger('获取工序信息失败，请刷新重试');
          }).finally(() => {
            resolve();
          })
        });
      },
      /*获取工序组信息*/
      fetchProcessGroup: function () {
        return new Promise(resolve => {
          axiosFetch({
            url: planProcessGroupGetUrl
          }).then(response => {
            if (response.data.result === 200) {
              this.processGroupSelectGroup = response.data.data.list;
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
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          this.mergeData = {};
          this.mergePos = {};
          let options = {
            url: planCapacitySelectUrl,
            data: {
              pageNo: this.paginationOptions.currentPage,
              pageSize: this.paginationOptions.pageSize,
            }
          };
          Object.keys(this.thisQueryOptions).forEach(item => {
            if (this.thisQueryOptions[item].value !== "") {
              options.data[item] = this.thisQueryOptions[item].value
            }
          });
          axiosFetch(options).then(response => {
            if (response.data.result === 200) {
              this.getSpanArr(response.data.data.list, this.mergeKeys);
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

      /**
       **@description: 编辑新增产线配置
       **@date: 2019/7/24 17:20
       **@author: DarkNin
       **@method:
       **@params: mod
       */
      editData: function (type, val) {
        if (!!this.$refs['capacityEditForm']) {
          this.$refs['capacityEditForm'].clearValidate();
        }
        if (type === 'edit') {
          this.capacityEditType = 'edit';
          Object.keys(val).forEach(item => {
            this.capacityEditOptions.forEach(option => {
              if (item === option.key && val[item] !== null) {
                this.$set(this.capacityEditOptionsData, item, val[item])
              }
            })
          });
          //工序组、工序、id
          this.processSelectGroup = [];
          this.processSelectGroupSrc.forEach(item => {
            if (item.processGroup === val.processGroup) {
              this.processSelectGroup.push(item)
            }
          });
          this.$set(this.capacityEditOptionsData, 'processGroup', val.processGroup);
          this.$set(this.capacityEditOptionsData, 'process', val.process);
          this.$set(this.capacityEditOptionsData, 'id', val.id);

          this.isCapacityEditing = true;
        } else if (type === 'add') {
          this.capacityEditType = 'add';
          this.isCapacityEditing = true;
        }
      },

      closeEditCapacityPanel: function () {
        this.isCapacityEditing = false;
      },
      submitEditCapacity: function () {
        this.$refs['capacityEditForm'].validate((isValid) => {
          if (isValid) {
            this.isPending = true;
            this.$openLoading();
            let options = {
              url: '',
              data: this.capacityEditOptionsData
            };
            if (this.capacityEditType === 'edit') {
              options.url = planCapacityEditUrl
            } else if (this.capacityEditType === 'add') {
              options.url = planCapacityAddUrl
            }
            axiosFetch(options).then(response => {
              if (response.data.result === 200) {
                this.$alertSuccess('操作成功');
                this.resetEditCapacityForm();
                this.closeEditCapacityPanel();
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
          } else {
            this.$alertInfo('请完善表单信息')
          }
        })
      },
      resetEditCapacityForm: function () {
        this.processSelectGroup = [];
        this.capacityEditOptionsData = {};
        this.$refs['capacityEditForm'].clearValidate();
        this.initEditOptions();
      },

      processGroupEditChange: function (val) {
        this.capacityEditOptionsData.process = '';
        this.processSelectGroup = [];
        if (val !== '') {
          this.processSelectGroupSrc.forEach(item => {
            if (item.processGroup === val) {
              this.processSelectGroup.push(item)
            }
          })
        }
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
            url: planCapacityDeleteUrl,
            data: {
              id: val.id
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

      /*详情表格行合并，获取合并记录*/
      getSpanArr: function (tableData, keyName) {
        keyName.forEach((kitem, k) => {
          tableData.forEach((data, i) => {
            if (i === 0) {
              this.mergeData[kitem] = this.mergeData[kitem] || [];
              this.mergeData[kitem].push(1);
              this.mergePos[kitem] = 0
            } else {
              // 判断当前元素与上一个元素是否相同
              if (data[kitem] === tableData[i - 1][kitem]) {
                this.mergeData[kitem][this.mergePos[kitem]] += 1;
                this.mergeData[kitem].push(0)
              } else {
                this.mergeData[kitem].push(1);
                this.mergePos[kitem] = i
              }
            }
          })
        });
      },
      detailsTableSpanMethod: function ({row, column, rowIndex, columnIndex}) {
        if (this.mergeProp.includes(column.property)) {
          const _row = this.mergeData[column.property][rowIndex];
          const _col = _row > 0 ? 1 : 0;
          return {
            rowspan: _row,
            colspan: _col
          }
        }
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
          this.capacityEditOptions.forEach(item => {
            this.$set(this.capacityEditOptionsData, item.key, '')
          });
          Object.keys(val.row).forEach(item => {
            this.capacityEditOptions.forEach(option => {
              if (item === option.key && val.row[item] !== null) {
                this.$set(this.capacityEditOptionsData, item, val.row[item])
              }
            })
          });
          this.$set(this.capacityEditOptionsData, 'processGroup', val.row.processGroup);
          this.$set(this.capacityEditOptionsData, 'process', val.row.process);
          this.$set(this.capacityEditOptionsData, 'id', val.row.id);
          this.$set(this.capacityEditOptionsData, 'position', position);
        }
        axiosFetch({
          url: planCapacityEditUrl,
          data: this.capacityEditOptionsData
        }).then(response => {
          if (response.data.result === 200) {
            this.$alertSuccess('操作成功');
            this.capacityEditOptionsData = {};
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

  .el-button /deep/ i{
    font-size: 17px;
    font-weight: bold;
  }
  #capacity-setting {
    position: relative;
    width: 100%;
    min-height: 600px;
    height: 100%;
  }

  .capacity-setting-main {
  }

  .query-comp {
    background-color: #ffffff;
    border: 1px solid #eeeeee;
    border-radius: 5px;
    padding: 10px;
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

  .capacity-edit-form {
    display: flex;
    flex-wrap: wrap;
  }

  .capacity-edit-form-comp {
    padding: 0 10px;
  }

  .capacity-edit-form-comp label {
    font-size: 15px;
    line-height: 24px;
    padding: 0;
  }

  .capacity-edit-form-comp-text {
    width: 210px;
  }

  .capacity-edit-form-comp-text /deep/ .el-input {
    width: 210px;
  }

</style>
