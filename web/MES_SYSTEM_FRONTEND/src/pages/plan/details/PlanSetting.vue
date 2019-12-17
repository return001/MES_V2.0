<template>
  <div id="plan-setting">
    <div class="plan-setting-main">
      <div class="query-comp">
        <div class="query-comp-container" v-if="!!thisQueryOptions.line">
          <div class="query-comp-select">
            <label for="process-query-item">产线:</label>
            <el-select
              v-model="thisQueryOptions['line'].value"
              id="process-query-item"
              placeholder="请选择产线"
              size="small">
              <el-option v-for="listItem in lineSelectGroup"
                         :key="listItem.id"
                         :value="listItem.id"
                         :label="listItem.lineName"></el-option>
            </el-select>
          </div>
        </div>
        <div class="query-comp-container" v-for="(item, index) in queryOptions" :key="index">
          <!--纯文本框-->
          <div class="query-comp-text" v-if="item.type === 'text'">
            <label :for="item.key + index">{{item.label}}:</label>
            <el-input v-model="thisQueryOptions[item.key].value" :id="item.key + index"
                      :placeholder="'请填写' + item.label" size="small"
                      autocomplete="off"></el-input>
          </div>
          <!--选择器-->
          <div class="query-comp-select" v-else-if="item.type === 'select'">
            <label :for="item.key + index">{{item.label}}:</label>
            <el-select v-model="thisQueryOptions[item.key].value" :id="item.key + index"
                       :placeholder="'请选择' + item.label" size="small"
                       autocomplete="off">
              <el-option v-for="listItem in item.list"
                         :key="listItem.value"
                         :value="listItem.value"
                         :label="listItem.label"></el-option>
            </el-select>
          </div>
          <!--时间范围-->
          <div class="query-comp-timerange" v-else-if="item.type === 'timeRange'">
            <label :for="item.key + index">{{item.label}}:</label>
            <el-date-picker
              :id="item.key + index"
              v-model="thisQueryOptions[item.key].value"
              type="datetimerange"
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
        <div class="query-comp-container"
             v-if="permissionControl(['schedulingJMPMC'])">
          <el-button type="primary" size="small" @click="showOrderImport">导入订单</el-button>
        </div>
        <div class="query-comp-container"
             v-if="permissionControl([])">
          <el-button type="primary" size="small" @click="exportPlan">导出计划</el-button>
        </div>
        <div class="query-comp-container"
             v-if="permissionControl(['schedulingJMPMC'])">
          <el-button type="primary" size="small" @click="showOrderImport('rework')">导入待返工单</el-button>
        </div>
      </div>
      <div class="content-comp" v-if="processGroupSelectGroup.length > 0">
        <div class="content-tag">
          <div class="content-tag-item" v-for="item in processGroupSelectGroup"
               :class="activeProcessGroup === item.id ? 'active' : ''" @click="switchTag(item)">{{item.groupName}}
          </div>
        </div>
        <el-table
          :data="tableData"
          max-height="560"
          ref="tablecomponent"
          :row-class-name="setTimeoutHighlight"
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
            label="甘特图"
            width="80"
            fixed="right"
            v-if="permissionControl(['engineer', 'schedulingJMPMC'])"
          >
            <template slot-scope="scope">
              <el-button type="text" icon="el-icon-picture-outline-round" @click="showGantt(scope.row)"></el-button>
            </template>
          </el-table-column>
          <el-table-column
            label="操作"
            width="140"
            fixed="right"
            v-if="permissionControl(['engineer','schedulingJMPMC'])"
          >
            <template slot-scope="scope">
              <el-tooltip content="详细" placement="top">
                <el-button type="text" @click="showDetailsData(scope.row)" icon="el-icon-document"></el-button>
              </el-tooltip>
              <el-tooltip content="编辑" placement="top">
                <el-button
                  type="text"
                  :disabled="scope.row.schedulingPlanStatus === 3"
                  @click="editData(scope.row)"
                  icon="el-icon-edit-outline"></el-button>
              </el-tooltip>
              <el-tooltip content="状态" placement="top">
                <el-button
                  type="text"
                  :disabled="scope.row.schedulingPlanStatus === 3"
                  @click="openStatusEditDialog(scope.row)"
                  icon="el-icon-more"></el-button>
              </el-tooltip>
              <el-tooltip content="删除" placement="top"
                          v-if="permissionControl(['schedulingJMPMC'])">
                <el-button
                  type="text"
                  @click="deleteData(scope.row)"
                  icon="el-icon-delete"></el-button>
              </el-tooltip>
            </template>
          </el-table-column>
        </el-table>
        <!--分页控制-->
        <el-pagination
          background
          :current-page.sync="paginationOptions.currentPage"
          :page-sizes="[10]"
          :page-size.sync="paginationOptions.pageSize"
          layout="total, sizes, prev, pager, next, jumper"
          :total="paginationOptions.total"
          @current-change="fetchData"
          @size-change="queryData"
          class="page-pagination">
        </el-pagination>
      </div>
    </div>

    <!--dialog component-->
    <el-dialog
      title="额外信息"
      :visible.sync="isOrderDetailsShowing"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @closed="tableDetailsData = []"
      width="650px">

      <div class="order-details-title">订单信息:</div>
      <el-table
        :data="tableDetailsOrderData"
        max-height="560"
        size="small"
        ref="tabledetailscomponent">
        <el-table-column v-for="(item, index) in tableDetailsOrderColumns"
                         :key="index"
                         :prop="item.key"
                         :label="item.label"
                         :min-width="item['min-width']"
                         :formatter="item.formatter">
        </el-table-column>

      </el-table>
      <div class="order-details-title">责任人信息:</div>
      <el-table
        :data="tableDetailsUserData"
        max-height="560"
        size="small"
        ref="tabledetailscomponent">
        <el-table-column v-for="(item, index) in tableDetailsUserColumns"
                         :key="index"
                         :prop="item.key"
                         :label="item.label"
                         :min-width="item['min-width']"
                         :formatter="item.formatter">
        </el-table-column>

      </el-table>
    </el-dialog>


    <el-dialog
      title="导入未排产订单"
      :visible.sync="isOrderImporting"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @closed="resetOrderImportDialog"
      width="90%">

      <el-table
        class="order-import-table"
        border
        size="small"
        :data="importingOrderData"
        max-height="560"
        ref="importtablecomponent"
        highlight-current-row
        @current-change="orderImportSelectionChange">
        <el-table-column
          type="index"
          width="50">
        </el-table-column>
        <el-table-column v-for="(item, index) in importingOrderColumns"
                         :key="index"
                         :prop="item.key"
                         :label="item.label"
                         :min-width="item['min-width']"
                         :formatter="item.formatter">
        </el-table-column>

      </el-table>
      <div class="order-import-stash-group"
           v-if="importingOrderSettingAll.length > 0">
        <div class="order-import-stash-title">待导入订单:</div>
        <el-tag
          v-for="(item, index) in importingOrderSettingAll"
          :key="item.order"
          closable
          @close="spliceOrderSetting(index)">
          {{item.orderName}}
        </el-tag>
      </div>
      <span slot="footer" class="dialog-footer">
        <el-button size="small" type="info" @click="isOrderImporting = false">取消</el-button>
        <el-button size="small" type="primary" @click="setOrderImportOptions">配置</el-button>
        <el-button size="small" type="primary" v-if="importingOrderSettingAll.length > 0" @click="submitOrderImport">导入</el-button>
      </span>

      <el-dialog
        title="详情"
        :visible.sync="isOrderImportingSettingShowing"
        :close-on-click-modal="false"
        :close-on-press-escape="false"
        width="400px"
        append-to-body>
        {{showingStashImportingSetting}}
      </el-dialog>

      <el-dialog
        title="设置"
        :visible.sync="isOrderImportingSetting"
        :close-on-click-modal="false"
        :close-on-press-escape="false"
        @closed="resetOrderImportSettingDialog"
        width="500px"
        append-to-body>
        <div class="order-setting-group">
          <div class="order-setting-title">
            <span>排产数量*</span>
            <span>产能*</span>
            <span>产线*</span>
          </div>
          <div class="order-setting-item" v-for="(item, index) in importingOrderSettingGroup">
            <span>#{{index + 1}}</span>
            <el-input v-model.number="item.schedulingQuantity" placeholder="请填写排产数量" size="small"></el-input>
            <el-input v-model.number="item.capacity" placeholder="请填写产能" size="small"></el-input>
            <el-select v-model="item.line" placeholder="请选择产线" size="small">
              <el-option v-for="listItem in lineSelectGroup"
                         :key="listItem.id"
                         :value="listItem.id"
                         :label="listItem.lineName"></el-option>
            </el-select>

          </div>
          <div class="order-setting-btn-group">
            <el-tooltip content="删除最后一条" placement="top">
              <el-button type="info" circle icon="el-icon-minus" size="mini"
                         @click="orderImportCountControl('minus')"></el-button>
            </el-tooltip>
            <el-tooltip content="新增计划" placement="top">
              <el-button type="primary" circle icon="el-icon-plus" size="mini"
                         @click="orderImportCountControl('plus')"></el-button>
            </el-tooltip>
          </div>
          <div class="divider"></div>
          <div class="order-setting-time">
            <div class="order-setting-time-item">
              <label for="order-setting-plan-start-time">预计开始时间:* </label>
              <el-date-picker
                id="order-setting-plan-start-time"
                v-model="importingOrderSettingStartTime"
                type="datetime"
                size="small"
                prefix-icon="el-icon-date"
                default-time="08:00:00"
                value-format="yyyy-MM-dd HH:mm:ss"
                autocomplete="off">
              </el-date-picker>
            </div>
            <div class="order-setting-time-item">
              <label for="order-setting-plan-complete-time">预计结束时间:* </label>
              <el-date-picker
                id="order-setting-plan-complete-time"
                v-model="importingOrderSettingCompleteTime"
                type="datetime"
                size="small"
                prefix-icon="el-icon-date"
                default-time="08:00:00"
                value-format="yyyy-MM-dd HH:mm:ss"
                autocomplete="off">
              </el-date-picker>
            </div>

          </div>
          <div class="order-setting-plan-remark">
            <label for="order-setting-plan-remark">备注</label>
            <el-input type="textarea"
                      id="order-setting-plan-remark"
                      :rows="4"
                      clearable
                      autocomplete="off"
                      v-model="importingOrderSettingRemark">
            </el-input>
          </div>
        </div>
        <span slot="footer" class="dialog-footer">
          <el-button size="small" type="info" @click="isOrderImportingSetting = false">取消</el-button>
          <el-button size="small" type="primary" @click="stashOrderSetting">保存</el-button>
      </span>

      </el-dialog>
    </el-dialog>

    <!--状态编辑-->
    <el-dialog
      title="修改状态"
      :visible.sync="isOrderStatusEditing"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @closed="resetStatusEdit"
      width="300px">
      <div class="edit-status">
        <el-select size="small" v-model="orderEditStatus" placeholder="请选择状态">
          <el-option :value="1" label="进行"
                     v-if="planEditRow.schedulingPlanStatus === 4 || planEditRow.schedulingPlanStatus === 1"></el-option>
          <!--<el-option :value="2" label="完成"></el-option>-->
          <el-option :value="3" label="暂停"
                     v-if="planEditRow.schedulingPlanStatus === 2 && permissionControl(['schedulingJMPMC'])"></el-option>
        </el-select>
      </div>
      <span slot="footer" class="dialog-footer">
          <el-button type="info" @click="isOrderStatusEditing = false" size="small">取消</el-button>
          <el-button type="primary" @click="submitStatusEdit" size="small">保存</el-button>
      </span>
    </el-dialog>

    <!--edit panel-->
    <plan-edit-panel
      :row="planEditRow"
      :pmc-editing="pmcEditing"
      :engineer-editing="engineerEditing"
      :line-select-group="lineSelectGroup"
      :totally-editing="totallyEditing"/>

    <!--甘特图-->
    <gantt-comp :prop-table-data="ganttData"/>
  </div>
</template>

<script>
  import {
    planQueryOptions,
    planTableColumns,
    planTableExtraOrderColumns,
    planTableExtraUserColumns,
    orderTableColumns,
    orderUnscheduledTableColumns
  } from "../../../config/planConfig";
  import {axiosFetch, axiosDownload} from "../../../utils/fetchData";
  import {
    planDetailsExportUrl,
    planDetailsDeleteUrl,
    planDetailsEditUrl,
    planDetailsGanttGetUrl,
    planDetailsSelectUrl,
    planDetailsStatusEditUrl,
    planDetailsUnscheduledSelectUrl,
    planDetailsAddUrl,
    planProcessGetUrl,
    planLineGetUrl,
    planProcessGroupGetUrl,
    planExtraDetailsSelectUrl,
    planOrderSelectUrl,
    planDetailsReworkSelectUrl
  } from "../../../config/globalUrl";
  import PlanEditPanel from './comp/PlanEditPanel';
  import GanttComp from './comp/GanttComp'
  import eventBus from "../../../utils/eventBus";
  import {MessageBox} from "element-ui";

  export default {
    name: "PlanSetting",
    inject: ['reload'],
    components: {
      PlanEditPanel,
      GanttComp
    },
    data() {
      return {
        queryOptions: planQueryOptions,
        thisQueryOptions: {},
        //预加载信息
        processSelectGroupSrc: [],
        processSelectGroup: [],
        processGroupSelectGroup: [],
        lineSelectGroupSrc: [],
        lineSelectGroup: [],
        tableData: [],
        tableColumns: planTableColumns,
        tableDetailsOrderData: [],
        tableDetailsUserData: [],
        tableDetailsOrderColumns: planTableExtraOrderColumns,
        tableDetailsUserColumns: planTableExtraUserColumns,
        paginationOptions: {
          currentPage: 1,
          pageSize: 10,
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
        activeProcessGroup: -1,
        activeProcessGroupType: 0, //selectReworkPlan param
        //导入未排产订单
        isOrderImporting: false, //导入订单显示控制
        isOrderImportingSetting: false, //编辑选中订单显示控制
        isOrderImportingSettingShowing: false, //已暂存的订单详情显示控制
        showingStashImportingSetting: {}, //显示暂存的订单详情
        importingOrderData: [],
        importingOrderDataTemp: [], //用于临时存放已配置的数据
        importingOrderColumns: orderUnscheduledTableColumns,
        importingOrderSelection: null,
        importingOrderSettingGroup: [
          {
            schedulingQuantity: '',
            capacity: '',
            line: ''
          }
        ],
        importingOrderSettingRemark: '',
        importingOrderSettingStartTime: '',
        importingOrderSettingCompleteTime: '',
        importingOrderSettingAll: [],
        //编辑计划
        totallyEditing: false,
        planEditRow: {},
        pmcEditing: false, //深圳PMC编辑框
        engineerEditing: false, //惠州几米

        //编辑状态
        isOrderStatusEditing: false,
        orderEditStatus: '',

        //详细信息
        isOrderDetailsShowing: false,

        //甘特图数据
        ganttData: []
      }
    },
    watch: {
      activeProcessGroup: function (val) {
        if (val !== -1) {
          this.lineSelectGroup = [];
          this.lineSelectGroupSrc.forEach(item => {
            if (item.processGroup === val) {
              this.lineSelectGroup.push(item)
            }
          })
        }
      }
    },
    async created() {
      this.initQueryOptions();
      await this.dataPreload();
      //加载表格
      this.activeProcessGroup = this.processGroupSelectGroup[0].id;
      this.fetchData();
    },
    mounted() {
      eventBus.$off('closeEditPanel'); //关闭编辑界面
      eventBus.$off('setTimeoutHighlight'); //设置超时项目高亮
      eventBus.$off('partlyReload');

      eventBus.$on('partlyReload', () => {
        this.partlyReload();
      });

      eventBus.$on('closeEditPanel', () => {
        this.totallyEditing = false;
        this.pmcEditing = false;
        this.engineerEditing = false;
        this.planEditRow = {};
      });

      eventBus.$on('setTimeoutHighlight', (id) => {
        this.$nextTick(() => {
          let elementGroup = document.getElementsByClassName(id);
          let targetTag = Array.prototype.filter.call(elementGroup, (e) => {
            return e.nodeName === 'TD' && e.classList.length === 1
          });
          targetTag[0].parentNode.classList.add('timeout-highlight')
        })
      })
    },
    methods: {
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
        _partlyReload(['thisQueryOptions', 'lineSelectGroupSrc','lineSelectGroup', 'processSelectGroupSrc', 'processGroupSelectGroup', 'activeProcessGroup' ])
      },
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
        this.queryOptions.forEach(item => {
          this.$set(this.thisQueryOptions, item.key, {
            type: item.type,
            value: ''
          })
        });
        this.$set(this.thisQueryOptions, 'line', {
          type: 'select',
          value: ''
        })
      },
      dataPreload: async function () {
        return new Promise(resolve => {
          Promise.all([this.fetchLine(), this.fetchProcessGroup(), this.fetchProcess()]).then(() => {
            resolve();
          })
        })
      },

      /*获取产线信息*/
      fetchLine: function () {
        return new Promise(resolve => {
          axiosFetch({
            url: planLineGetUrl
          }).then(response => {
            if (response.data.result === 200) {
              this.lineSelectGroupSrc = response.data.data.list;
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.$alertDanger('获取产线信息失败，请刷新重试');
          }).finally(() => {
            resolve();
          })
        });
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
        this.paginationOptions.currentPage = 1;
        this.paginationOptions.total = 0;
        this.fetchData();
      },

      switchTag: function (item) {
        this.activeProcessGroup = item.id;
        switch (item.groupName) {
          case '组装':
            this.activeProcessGroupType = 0;
            break;
          case '测试':
            this.activeProcessGroupType = 1;
            break;
          case '包装':
            this.activeProcessGroupType = 2;
            break;
        }
        this.initQueryOptions();
        this.queryData();
      },

      fetchData: function () {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          let options = {
            url: planDetailsSelectUrl,
            data: {
              pageNo: this.paginationOptions.currentPage,
              pageSize: this.paginationOptions.pageSize,
            }
          };
          Object.keys(this.thisQueryOptions).forEach(item => {
            if (!!this.thisQueryOptions[item].value || this.thisQueryOptions[item].value === 0) {
              if (this.thisQueryOptions[item].type === 'timeRange') {
                options.data[item + 'From'] = this.thisQueryOptions[item].value[0];
                options.data[item + 'To'] = this.thisQueryOptions[item].value[1]
              } else {
                options.data[item] = this.thisQueryOptions[item].value
              }
            }
          });
          options.data.processGroup = this.activeProcessGroup;
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
       **@description: 导入未排产订单
       **@date: 2019/7/27 8:26
       **@author: DarkNin
       **@method:
       **@params: mod
       */
      /*显示导入订单界面*/
      showOrderImport: function (url) {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          axiosFetch({
            url: url === 'rework' ? planDetailsReworkSelectUrl : planDetailsUnscheduledSelectUrl,
            data: {
              type: this.activeProcessGroupType //track
            }
          }).then(response => {
            if (response.data.result === 200) {
              this.importingOrderData = response.data.data;
              this.isOrderImporting = true;
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
      /*选中订单*/
      orderImportSelectionChange: function (val) {
        this.importingOrderSelection = val;
      },
      /*显示该订单的排产参数设置界面*/
      setOrderImportOptions: function () {
        if (!!this.importingOrderSelection) {
          if (this.importingOrderSelection.unscheduledQuantity === 0) {
            this.$alertInfo('该订单未排产数量为0');
            return;
          }

          this.$set(this.importingOrderSettingGroup, 0, {
            schedulingQuantity: this.importingOrderSelection.unscheduledQuantity,
            capacity: this.importingOrderSelection.capacity,
            line: this.importingOrderSelection.line
          });
          this.isOrderImportingSetting = true
        } else {
          this.$alertInfo("请选择订单")
        }

      },

      /*控制分产线计划数量的增减*/
      orderImportCountControl: function (type) {
        if (type === 'plus') {
          this.importingOrderSettingGroup.push({
            schedulingQuantity: '',
            capacity: '',
            line: ''
          })
        } else if (type === 'minus') {
          if (this.importingOrderSettingGroup.length > 1) {
            this.importingOrderSettingGroup.pop();
          }
        }
      },

      /*暂存已编辑配置*/
      stashOrderSetting: function () {
        if (!this.isPending) {
          this.isPending = true;
          let mark = false;
          let dataGroup = {
            schedulingQuantity: [],
            capacity: [],
            line: [],
          };
          this.importingOrderSettingGroup.forEach(item => {
            Object.keys(item).forEach(i => {
              if (!item[i] && item[i] !== 0) {
                mark = true
              } else {
                dataGroup[i].push(item[i])
              }
            })
          });

          if (!this.importingOrderSettingStartTime || !this.importingOrderSettingCompleteTime) {
            mark = true
          } else {
            if (new Date(this.importingOrderSettingCompleteTime) - new Date(this.importingOrderSettingStartTime) < 0) {
              mark = true
            }
          }

          dataGroup.schedulingQuantity.forEach(item => {
            let regx = new RegExp("^[1-9]\\d*$");
            if (!regx.test(item)) {
              mark = true;
            }
          });

          if (mark) {
            this.$alertInfo("导入设置有误");
            this.isPending = false;
            return
          }

          let quantitySummary = 0;
          dataGroup.schedulingQuantity.forEach(item => {
            quantitySummary += item
          });

          if (quantitySummary > this.importingOrderSelection.unscheduledQuantity) {
            this.$alertInfo("排产数量大于未排产数量");
            this.isPending = false;
            return
          }

          this.isOrderImportingSetting = false;
          this.importingOrderSettingAll.push({
            order: this.importingOrderSelection.id,
            orderName: this.importingOrderSelection.zhidan,
            processGroup: this.activeProcessGroup,
            schedulingQuantity: dataGroup.schedulingQuantity.toString(),
            capacity: dataGroup.capacity.toString(),
            line: dataGroup.line.toString(),
            remark: this.importingOrderSettingRemark,
            planStartTime: this.importingOrderSettingStartTime,
            planCompleteTime: this.importingOrderSettingCompleteTime
          });

          let index = 0;
          this.importingOrderData.forEach((itm, idx) => {
            if (itm.id === this.importingOrderSelection.id) {
              index = idx;
            }
          });
          this.importingOrderDataTemp.push(this.importingOrderData[index]);
          this.importingOrderData.splice(index, 1);
          this.isPending = false;
        }
      },

      /*删除已暂存配置，回滚列表*/
      spliceOrderSetting: function (index) {
        let tempIndex = 0;
        this.importingOrderDataTemp.forEach((itm, idx) => {
          if (itm.id === this.importingOrderSettingAll[index].order) {
            tempIndex = idx;
          }
        });
        this.importingOrderData.unshift(this.importingOrderDataTemp[tempIndex]);
        this.importingOrderDataTemp.splice(tempIndex, 1);
        this.importingOrderSettingAll.splice(index, 1);
      },

      /*提交*/
      submitOrderImport: function () {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          axiosFetch({
            url: planDetailsAddUrl,
            data: {
              setting: JSON.stringify(this.importingOrderSettingAll)
            }
          }).then(response => {
            if (response.data.result === 200) {
              this.$alertSuccess("操作成功");
              this.partlyReload();
              //this.reload();
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

      resetOrderImportDialog: function () {
        this.importingOrderData = [];
        this.importingOrderDataTemp = [];
        this.importingOrderSelection = null;
        this.importingOrderSettingAll = [];
        this.resetOrderImportSettingDialog()
      },

      resetOrderImportSettingDialog: function () {
        this.importingOrderSettingGroup = [
          {
            schedulingQuantity: '',
            capacity: '',
            line: ''
          }
        ];
        this.importingOrderSettingRemark = '';
        this.importingOrderSettingStartTime = '';
        this.importingOrderSettingCompleteTime = '';
      },

      editData: function (val) {
        this.planEditRow = val;
        if (this.$store.state.userType === 'engineer') {
          this.engineerEditing = true;
        } else if (this.$store.state.userType === 'schedulingJMPMC') {
          this.pmcEditing = true;
        } else if (this.$store.state.userType === 'SuperAdmin') {
          this.totallyEditing = true;
        }

      },


      /*状态编辑*/
      openStatusEditDialog: function (val) {
        this.planEditRow = val;
        this.isOrderStatusEditing = true
      },

      submitStatusEdit: function (val) {
        if (this.orderEditStatus === '') {
          this.$alertInfo();
        }
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          axiosFetch({
            url: planDetailsStatusEditUrl,
            data: {
              id: this.planEditRow.id,
              type: this.orderEditStatus
            }
          }).then(response => {
            if (response.data.result === 200) {
              this.$alertSuccess("修改成功");
              this.partlyReload();
              //this.reload();
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            console.log(err);
            this.$alertDanger("未知错误")
          }).finally(() => {
            this.isPending = false;
            this.$closeLoading();
          })
        }
      },

      resetStatusEdit: function () {
        this.planEditRow = {};
        this.orderEditStatus = '';
      },

      deleteData: function (val) {
        MessageBox.confirm('将删除该计划，是否继续?', '提示', {
          confirmButtonText: '确认',
          cancelButtonText: '取消',
          type: 'warning'
        }).then(() => {
          this.$openLoading();
          this.isPending = true;
          axiosFetch({
            url: planDetailsDeleteUrl,
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

      //详细信息
      showDetailsData: function (val) {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          axiosFetch({
            url: planExtraDetailsSelectUrl,
            data: {
              id: val.id
            }
          }).then(response => {
            if (response.data.result === 200) {
              this.tableDetailsUserData = [response.data.data.planUser];
              this.tableDetailsOrderData = [response.data.data.order];
              this.isOrderDetailsShowing = true;
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            console.log(err);
            this.$alertDanger("未知错误")
          }).finally(() => {
            this.isPending = false;
            this.$closeLoading();
          })
        }
      },

      //导出计划
      exportPlan: function () {

        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          let options = {
            url: planDetailsExportUrl,
            data: {
              pageNo: 1,
              pageSize: 10
            }
          };
          Object.keys(this.thisQueryOptions).forEach(item => {
            if (this.thisQueryOptions[item].value !== "") {
              if (this.thisQueryOptions[item].type === 'timeRange') {
                options.data[item + 'From'] = this.thisQueryOptions[item].value[0];
                options.data[item + 'To'] = this.thisQueryOptions[item].value[1]
              } else {
                options.data[item] = this.thisQueryOptions[item].value
              }
            }
          });
          options.data.processGroup = this.activeProcessGroup;
          axiosDownload(options).then(response => {
            let contentType = response.request.getResponseHeader('content-type');
            if (contentType === 'application/vnd.ms-excel') {
              let name = response.request.getResponseHeader('Content-Disposition').split('=')[1];
              saveAs(response.data, decodeURIComponent(name))
            } else {
              let reader = new FileReader();
              reader.readAsText(response.data);
              reader.addEventListener('loadend', () => {
                this.$alertWarning(JSON.parse(reader.result).data)
              })
            }
          }).catch(err => {
            this.$alertDanger('请求超时，请刷新重试')
          }).finally(() => {
            this.isPending = false;
            this.$closeLoading();
          })
        }
      },

      //甘特图
      showGantt: function (val) {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          axiosFetch({
            url: planDetailsGanttGetUrl,
            data: {
              id: val.id
            }
          }).then(response => {
            if (response.data.result === 200) {
              if (response.data.data.length > 0) {
                this.ganttData = response.data.data;
              } else {
                this.$alertInfo("无数据")
              }
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            console.log(err);
            this.$alertDanger("未知错误")
          }).finally(() => {
            this.isPending = false;
            this.$closeLoading();
          })
        }
      },
      setTimeoutHighlight({row, index}) {
        if (row.isTimeout) {
          return 'timeout-highlight'
        }
      }
    }
  }
</script>

<style scoped>
  .el-button /deep/ i {
    font-size: 17px;
    font-weight: bold;
  }

  #plan-setting {
    position: relative;
    width: 100%;
    min-height: 600px;
    height: 100%;
  }

  .plan-setting-main {
  }

  .query-comp {
    background-color: #ffffff;
    border: 1px solid #eeeeee;
    border-radius: 5px;
    padding: 10px 20px;
    display: flex;
    flex-wrap: wrap;
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
    /*width: 80px;*/
  }

  .content-comp {
    background-color: #ffffff;
    border: 1px solid #eeeeee;
    border-radius: 5px;
    padding: 10px;
    margin-top: 10px;
  }

  .content-tag {
    display: flex;
    width: 100%;
    border-bottom: 1px solid #eeeeee;
  }

  .content-tag-item {
    border: 1px solid #eeeeee;
    min-width: 80px;
    text-align: center;
    line-height: 26px;
    font-size: 14px;
    user-select: none;
    cursor: pointer;
    color: #909399;
    background: #FAFAFA;
    margin-bottom: -1px;
    font-weight: bold;
  }

  .content-tag-item:hover {
    color: #66b1ff;
  }

  .content-tag-item:first-child {
    border-radius: 5px 0 0 0;
  }

  .content-tag-item:last-child {
    border-radius: 0 5px 0 0;
  }

  .content-tag-item + .content-tag-item {
    border-left: none;
  }

  .content-tag-item.active {
    color: #66b1ff;
    border-bottom: none;
    background: #ffffff;
  }

  .order-details-title {
    margin-top: 5px;
    font-weight: bold;
    color: #909399;
  }

  .order-import-table /deep/ td {
    cursor: pointer;
  }

  .order-setting-title {
    width: 100%;
    height: 40px;
    font-size: 16px;
    line-height: 40px;
    color: #909399;
    margin-left: 24px;
  }

  .order-setting-title span {
    display: inline-block;
    width: 136px;
  }

  .order-setting-item {
    display: flex;
  }

  .order-setting-item + .order-setting-item {
    margin-top: 12px;
  }

  .order-setting-item .el-input {
    width: 125px;
    margin-right: 15px;
  }

  .order-setting-item .el-select {
    width: 125px;
  }

  .order-setting-item span {
    display: inline-block;
    height: 32px;
    line-height: 32px;
    color: #909399;
    width: 24px;
  }

  .order-setting-item label {
    display: block;
  }

  .order-setting-btn-group {
    width: 100%;
    display: flex;
    margin-top: 20px;
    justify-content: center;
    margin-bottom: 25px;
  }

  .edit-status {
    width: 100%;
  }

  .edit-status .el-select {
    width: 100%;
  }

  .el-table /deep/ .timeout-highlight {
    color: red;
    font-weight: bold;
  }

  .divider {
    width: 100%;
    height: 1px;
    border-bottom: 1px solid #eeeeee;
    margin: 5px 0;
  }

  .order-setting-time-item {
    color: #909399;
    margin: 6px 5px;
  }

  .order-setting-plan-remark label {
    display: block;
    padding: 5px;
    color: #909399;

  }

  .order-import-stash-group {
    margin-top: 10px;
  }

  .order-import-stash-title {
    color: #909399;
    margin-bottom: 5px;
  }

  .el-tag + .el-tag {
    margin-left: 10px;
  }

  .page-pagination {
    display: flex;
    flex-wrap: wrap;
    margin: 20px 0;
    padding: 0 20px;
  }
</style>
