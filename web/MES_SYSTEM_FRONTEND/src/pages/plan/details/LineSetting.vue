<template>
  <div id="line-setting">
    <div class="line-setting-main">
      <div class="query-comp">
        <div class="query-comp-container" v-for="(item, index) in queryOptions" :key="index">
          <!--纯文本框-->
          <div class="query-comp-text" v-if="item.type === 'text'">
            <label :for="item.key + index">{{item.label}}:</label>
            <el-input v-model="thisQueryOptions[item.key].value" :id="item.key + index"
                      :placeholder="'请填写' + item.label" size="small"></el-input>
          </div>
          <!--选择器-->
          <div class="query-comp-select" v-else-if="item.type === 'select' && item.async === true">
            <label :for="item.key + index">{{item.label}}:</label>
            <el-select v-model="thisQueryOptions[item.key].value" :id="item.key + index"
                       :placeholder="'请选择' + item.label" size="small">
              <el-option value="" label="未选择"></el-option>
              <el-option v-for="listItem in asyncSelectGroup"
                         :key="listItem.id"
                         :value="listItem.id"
                         :label="listItem.groupName"></el-option>
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
            width="120"
            fixed="right"
            v-if="permissionControl(['engineer'])"
          >
            <template slot-scope="scope">
              <el-tooltip content="产线PC配置" placement="top">
                <el-button type="text" @click="showLineSetting(scope.row)" icon="el-icon-t-setting"></el-button>
              </el-tooltip>
              <el-tooltip content="编辑" placement="top">
                <el-button type="text" @click="editData('edit', scope.row)" icon="el-icon-t-edit"></el-button>
              </el-tooltip>
              <el-tooltip content="删除" placement="top">
                <el-button type="text" @click="deleteData(scope.row)" icon="el-icon-t-delete"></el-button>
              </el-tooltip>
            </template>
          </el-table-column>
        </el-table>
      </div>
    </div>

    <!--dialog component-->
    <el-dialog
      :title="editPanelTitle"
      :visible.sync="isLineEditing"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @close="resetEditLineForm"
      width="730px">
      <el-form
        ref="lineEditForm"
        :model="lineEditOptionsData"
        class="line-edit-form"
        label-position="top"
        @submit.native.prevent
        :rules="lineEditOptionsRules">
        <el-form-item class="line-edit-form-comp" label="工序组" prop="processGroup">
          <el-select v-model="lineEditOptionsData.processGroup" class="line-edit-form-comp-text">
            <el-option value="" label="未选择" placeholder="请选择工序组" size="small"></el-option>
            <el-option v-for="listItem in asyncSelectGroup"
                       :key="listItem.id"
                       :value="listItem.id"
                       :label="listItem.groupName"></el-option>
          </el-select>
        </el-form-item>
        <el-form-item
          class="line-edit-form-comp"
          v-for="(lineItem, index) in lineEditOptions"
          :key="index"
          :label="lineItem.label + '：' + (lineItem.type === 'asyncSelect' ? ((!!lineEditOptionsData[lineItem.key] ? ('（ID:' + lineEditOptionsData[lineItem.key] + '）') : '（未选择）')) : '')"
          :prop="lineItem.key">
          <div class="line-edit-form-comp-text" v-if="lineItem.type === 'text'">
            <el-input
              type="text"
              :id="'edit' + lineItem.key + index" :placeholder="'请填写' + lineItem.label"
              clearable
              autocomplete="off"
              v-model="lineEditOptionsData[lineItem.key]"></el-input>
          </div>
          <div class="line-edit-form-comp-text" v-if="lineItem.type === 'date'">
            <el-date-picker
              :id="'edit' + lineItem.key + index"
              v-model="lineEditOptionsData[lineItem.key]"
              type="date"
              placeholder="选择日期"
              value-format="yyyy-MM-dd"></el-date-picker>
          </div>
          <div class="line-edit-form-comp-text" v-if="lineItem.type === 'asyncSelect'">

            <el-autocomplete
              style="display: block;"
              v-model.trim="tempStorage[lineItem.condition]"
              :fetch-suggestions="searchItem"
              :placeholder="'请输入工号并选择'"
              @input="autoClear"
              @focus="setAsyncSearchProp(lineItem.key, lineItem.condition)"
              @select="handleSelect"
              :trigger-on-focus="false"
              :id="'upload-' + lineItem.prop">
              <template slot-scope="{ item }">
                <el-tooltip :content="'用户名: ' + item.name" placement="top-start">
                  <div>
                    <span style="font-size: 10px; color: #777;">id: {{ item.id }}</span>
                    <div class="upload-name">用户名: {{ item.name }}</div>
                  </div>
                </el-tooltip>
              </template>
            </el-autocomplete>
            <!--<el-select
              v-model="lineEditOptionsData[lineItem.key]"
              filterable
              remote
              reserve-keyword
              placeholder="请输入关键词"
              :remote-method="userRemoteMethod"
              :loading="remoteLoading">
              <el-option
                v-for="subItem in asyncUserSelectList"
                :key="subItem.id"
                :label="subItem.name"
                :value="subItem.id">
              </el-option>
            </el-select>-->

          </div>
          <div class="line-edit-form-comp-textarea" v-if="lineItem.type === 'textArea'">
            <el-input type="textarea"
                      :rows="4"
                      :id="'edit' + lineItem.key + index" :placeholder="'请填写' + lineItem.label"
                      clearable
                      autocomplete="off"
                      v-model="lineEditOptionsData[lineItem.key]"></el-input>
          </div>
        </el-form-item>
      </el-form>
      <span slot="footer" class="dialog-footer">
        <el-button @click="closeEditLinePanel" type="info">取消</el-button>
        <el-button @click="submitEditLine" type="primary">保存</el-button>
      </span>
    </el-dialog>

    <!--产线PC设置-->
    <el-dialog
      title="PC配置"
      :visible.sync="isPCSettingShowing"
      width="600px"
      :close-on-click-modal="false"
      :close-on-press-escape="false">
      <el-table
        :data="pcSettingData"
        max-height="500"
        stripe>
        <el-table-column
          type="index"
          width="60">
        </el-table-column>
        <el-table-column
          v-for="(item, index) in pcSettingColumns"
          :key="index"
          :prop="item.key"
          :label="item.label"
          :min-width="item['min-width']"></el-table-column>
        <el-table-column
          label="操作"
          width="80"
        >
          <template slot-scope="scope">
            <el-tooltip content="编辑" placement="top">
              <el-button type="text" @click="editPCData(scope.row)" icon="el-icon-t-edit"></el-button>
            </el-tooltip>
            <el-tooltip content="删除" placement="top">
              <el-button type="text" @click="deletePCData(scope.row)" icon="el-icon-t-delete"></el-button>
            </el-tooltip>
          </template>
        </el-table-column>
      </el-table>
      <span slot="footer" class="dialog-footer">
        <el-button @click="closePCSettingDetailsPanel" type="info">关闭</el-button>
        <el-button @click="addPCSetting" type="primary">新增</el-button>
      </span>
      <el-dialog
        title="编辑"
        :visible.sync="isPCSettingEditing"
        width="500px"
        :close-on-click-modal="false"
        :close-on-press-escape="false"
        append-to-body>
        <el-form
          ref="linePCEditForm"
          :model="editSettingData"
          class="line-edit-form"
          label-position="top"
          @closed="resetEditPCSetting"
          @submit.native.prevent
          :rules="editSettingRules">
          <el-form-item
            class="line-edit-form-comp"
            label="IP："
            prop="ip">
            <div class="line-edit-form-comp-text">
              <el-input
                type="text"
                placeholder="请填写IP地址"
                clearable
                autocomplete="off"
                v-model="editSettingData['ip']"></el-input>
            </div>
          </el-form-item>
          <el-form-item
            class="line-edit-form-comp"
            label="电脑名称："
            prop="computerName">
            <div class="line-edit-form-comp-text">
              <el-input
                type="text"
                placeholder="请填写电脑名称"
                clearable
                autocomplete="off"
                v-model="editSettingData['computerName']"></el-input>
            </div>
          </el-form-item>
          <el-form-item
            class="line-edit-form-comp"
            label="备注："
            prop="remark">
            <div class="line-edit-form-comp-text">
              <el-input
                type="text"
                placeholder="请填写备注"
                clearable
                autocomplete="off"
                v-model="editSettingData['remark']"></el-input>
            </div>
          </el-form-item>
        </el-form>
        <span slot="footer" class="dialog-footer">
        <el-button @click="closeEditPCSettingPanel" type="info">取消</el-button>
        <el-button @click="submitEditPCConfig" type="primary">保存</el-button>
      </span>
      </el-dialog>
    </el-dialog>

  </div>
</template>

<script>
  import {
    planLineAddUrl,
    planLineDeleteUrl,
    planLineEditUrl,
    planLineSelectUrl,
    planProcessGroupGetUrl,
    planLinePCAddUrl,
    planLinePCDeleteUrl,
    planLinePCEditUrl,
    planLinePCSelectUrl,
    getUserUrl
  } from "../../../config/globalUrl";
  import {lineTableColumns, lineQueryOptions, lineEditOptions, lineEditOptionsRules} from "../../../config/planConfig";
  import {axiosFetch} from "../../../utils/fetchData";
  import {MessageBox} from 'element-ui'

  export default {
    name: "LineSetting",
    inject: ['reload'],
    data() {
      return {
        queryOptions: [],
        thisQueryOptions: {},
        asyncSelectGroup: [], //工序组信息
        tableData: [],
        tableColumns: [],
        paginationOptions: {
          currentPage: 1,
          pageSize: 65535,
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
        isPending: false,
        queryString: '',

        /*编辑 新增产线配置*/
        isLineEditing: false,
        lineEditType: '',
        lineEditOptionsData: {
          'processGroup': ''
        },
        asyncUserSelectList: [],
        remoteLoading: false,
        tempStorage: {},
        asyncSearchProp: {},
        /*产线PC配置*/
        isPCSettingShowing: false,
        isPCSettingEditing: false,
        showingSetting: {},
        pcSettingEditType: '',
        pcSettingData: [],
        pcSettingColumns: [
          {
            'key': 'ip',
            'label': 'IP地址',
            'min-width': '120px'
          },
          {
            'key': 'computerName',
            'label': '电脑名称',
            'min-width': '120px'
          },
          {
            'key': 'remark',
            'label': '备注',
            'min-width': '120px'
          }
        ],
        //里层dialog 编辑
        editSettingData: {},
        editSettingRules: {
          'ip': [{required: true, message: '请输入IP地址', trigger: 'blur'},
            {
              pattern: /^(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])(.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])){3}$/,
              message: '请输入正确的IP地址',
              trigger: 'blur'
            }],
          'computerName': [{required: true, message: '请输入电脑名称', trigger: 'blur'}],
        }

      }
    },
    computed: {
      lineEditOptions: function () {
        return lineEditOptions
      },
      lineEditOptionsRules: function () {
        return lineEditOptionsRules
      },
      editPanelTitle: function () {
        if (this.lineEditType === 'edit') {
          return '编辑'
        } else if (this.lineEditType === 'add') {
          return '新增'
        }
      },
    },
    async created() {
      await this.fetchProcessGroup();
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
        this.queryOptions = JSON.parse(JSON.stringify(lineQueryOptions));
        this.queryOptions.forEach(item => {
          this.$set(this.thisQueryOptions, item.key, {
            type: item.type,
            value: ''
          })
        });
      },
      initTableColumns: function () {
        this.tableColumns = JSON.parse(JSON.stringify(lineTableColumns))
      },

      initEditOptions: function () {
        this.lineEditOptions.forEach(item => {
          this.$set(this.lineEditOptionsData, item.key, '')
        });
        this.$set(this.lineEditOptionsData, 'lineGroup', '');
      },

      /*获取工序组信息*/
      fetchProcessGroup: async function () {
        return new Promise(resolve => {
          axiosFetch({
            url: planProcessGroupGetUrl
          }).then(response => {
            if (response.data.result === 200) {
              this.asyncSelectGroup = response.data.data.list;
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
          let options = {
            url: planLineSelectUrl,
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
       **@date: 2019/7/23 17:40
       **@author: DarkNin
       **@method:
       **@params: mod
       */
      editData: function (type, val) {
        if (!!this.$refs['lineEditForm']) {
          this.$refs['lineEditForm'].clearValidate();
        }
        if (type === 'edit') {
          this.lineEditType = 'edit';
          Object.keys(val).forEach(item => {
            this.lineEditOptions.forEach(option => {
              if (item === option.key && val[item] !== null) {
                this.$set(this.lineEditOptionsData, item, val[item]);
                //候选项选择框填充
                if (option.type === 'asyncSelect') {
                  this.$set(this.tempStorage, option.condition, val[option.condition])
                }
              }
            })
          });
          //工序组、id
          this.$set(this.lineEditOptionsData, 'processGroup', val.processGroup);
          this.$set(this.lineEditOptionsData, 'id', val.id);

          this.isLineEditing = true;
        } else if (type === 'add') {
          this.lineEditType = 'add';
          this.isLineEditing = true;
        }
      },

      closeEditLinePanel: function () {
        this.isLineEditing = false;
      },

      submitEditLine: function () {
        this.$refs['lineEditForm'].validate((isValid) => {
          if (isValid) {
            this.isPending = true;
            this.$openLoading();
            let options = {
              url: '',
              data: this.lineEditOptionsData
            };
            if (this.lineEditType === 'edit') {
              options.url = planLineEditUrl
            } else if (this.lineEditType === 'add') {
              options.url = planLineAddUrl
            }
            axiosFetch(options).then(response => {
              if (response.data.result === 200) {
                this.$alertSuccess('操作成功');
                this.resetEditLineForm();
                this.closeEditLinePanel();
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
      resetEditLineForm: function () {
        this.lineEditOptionsData = {};
        this.tempStorage = {};
        this.asyncSearchProp = {};
        this.$refs['lineEditForm'].clearValidate();
        this.initEditOptions();
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
            url: planLineDeleteUrl,
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

      /**
       **@description: 编辑产线详细配置(PC)
       **@date: 2019/7/23 17:45
       **@author: DarkNin
       **@method:
       **@params: mod
       */
      showLineSetting: function (val) {
        this.showingSetting = val;
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          axiosFetch({
            url: planLinePCSelectUrl,
            data: {
              lineId: val.id
            }
          }).then(response => {
            if (response.data.result === 200) {
              this.$set(this.$data, 'pcSettingData', response.data.data.list);
              //this.pcSettingData = response.data.data.list;
              this.isPCSettingShowing = true;
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
      editPCData: function (val) {
        this.editSettingData = {
          id: val.id,
          computerName: val.computerName,
          remark: val.remark,
          ip: val.ip
        };
        this.pcSettingEditType = 'edit';
        this.isPCSettingEditing = true;
      },
      addPCSetting: function () {
        this.editSettingData = {
          computerName: '',
          remark: '',
          ip: ''
        };
        this.pcSettingEditType = 'add';
        this.isPCSettingEditing = true;
      },

      submitEditPCConfig: function () {
        this.$refs['linePCEditForm'].validate((isValid) => {
          if (isValid) {
            this.isPending = true;
            this.$openLoading();
            let options = {
              url: '',
              data: this.editSettingData
            };
            if (this.pcSettingEditType === 'edit') {
              options.url = planLinePCEditUrl
            } else if (this.pcSettingEditType === 'add') {
              options.url = planLinePCAddUrl;
              options.data.line = this.showingSetting.id;
            }
            axiosFetch(options).then(response => {
              if (response.data.result === 200) {
                this.$alertSuccess('操作成功');
                this.editSettingData = {};
                this.$refs['linePCEditForm'].resetFields();
                this.closeEditPCSettingPanel();
                this.isPending = false;
                this.showLineSetting(this.showingSetting)
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

      deletePCData: function (val) {
        MessageBox.confirm('将删除该配置，是否继续?', '提示', {
          confirmButtonText: '确认',
          cancelButtonText: '取消',
          type: 'warning'
        }).then(() => {
          this.$openLoading();
          this.isPending = true;
          axiosFetch({
            url: planLinePCDeleteUrl,
            data: {
              id: val.id
            }
          }).then(response => {
            if (response.data.result === 200) {
              this.$alertSuccess('删除成功');
              this.isPending = false;
              this.showLineSetting(this.showingSetting)
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

      closePCSettingDetailsPanel: function () {
        this.isPCSettingShowing = false;
      },
      closeEditPCSettingPanel: function () {
        this.isPCSettingEditing = false;
      },

      resetEditPCSetting: function () {
        this.editSettingData = {};
        this.showingSetting = {};
        this.$refs['linePCEditForm'].resetFields();
      },

      //人员搜索
      //attr(key: item.key condition: item.condition)
      setAsyncSearchProp: function (key, condition) {
        this.asyncSearchProp = {
          key: key,
          condition: condition
        }
      },
      searchItem: function (qs, cb) {
        let options = {
          url: getUserUrl,
          data: {}
        };
        options.data['userName'] = qs;
        axiosFetch(options).then(response => {
          if (response.data.result === 200) {
            if (response.data.data !== null) {
              cb(response.data.data.list)
            } else {
              cb([])
            }
          } else {
            cb([])
          }
        });
      },

      //item: {id: xx, name: xx}
      handleSelect: function (item) {
        this.$set(this.tempStorage, this.asyncSearchProp.condition, item.name);
        this.$set(this.lineEditOptionsData, this.asyncSearchProp.key, item.id);
        this.$refs['lineEditForm'].validate(() => {
        });
      },

      //清空文本框时清空所选
      autoClear: function (val) {
        if (val === '') {
          this.$set(this.lineEditOptionsData, this.asyncSearchProp.key, '');
          this.$refs['lineEditForm'].validate(() => {
          });
        }
      }
    }
  }
</script>

<style scoped>

  #line-setting {
    position: relative;
    width: 100%;
    min-height: 600px;
    height: 100%;
  }

  .line-setting-main {
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

  .line-edit-form {
    display: flex;
    flex-wrap: wrap;
  }

  .line-edit-form-comp {
    padding: 0 10px;
  }

  .line-edit-form-comp label {
    font-size: 15px;
    line-height: 24px;
    padding: 0;
  }

  .line-edit-form-comp-text {
    width: 210px;
  }

  .line-edit-form-comp-text /deep/ .el-input {
    width: 210px;
  }


</style>
