<template>
  <div id="plan-setting">
    <div class="plan-setting-main">
      <div class="query-comp" ref="query-comp">
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
        <div class="query-comp-container">
          <el-button type="primary" size="small" @click="showOrderImport">导入订单</el-button>
        </div>
        <div class="query-comp-container">
          <el-button type="primary" size="small" @click="exportPlan">导出计划</el-button>
        </div>
        <div class="query-comp-container">
          <el-button type="primary" size="small" @click="showOrderImport('rework')">导入待返工单</el-button>
        </div>
        <div class="query-comp-container">
          <el-button type="primary" size="small" @click="showOrderPlannedImport">重排已排产</el-button>
        </div>

        <div class="query-comp-fold" ref="query-comp-fold">
          <span class="el-icon-d-caret query-comp-fold-btn" @click="toggleQueryCompHide"></span>
        </div>
      </div>
      <div class="content-comp" v-if="processGroupSelectGroup.length > 0">
        <div class="content-tag">
          <div class="content-tag-item" v-for="item in processGroupSelectGroup"
               v-if="item.parentGroup === null"
               :class="activeProcessGroup === item.id ? 'active' : ''" @click="switchTag(item)">{{item.groupName}}
          </div>
        </div>
        <div class="vice-tag" v-if="viceGroup.length > 0">
          <el-button size="small" :type="activeProcessGroup === item.id ? 'info' : 'primary'" v-for="item in viceGroup"
                @click="viceSwitchTag(item)">{{item.groupName}}
          </el-button>
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
          >
            <template slot-scope="scope">
              <el-button type="text" icon="el-icon-picture-outline-round" @click="showGantt(scope.row)"></el-button>
            </template>
          </el-table-column>
          <el-table-column
              label="操作"
              width="140"
              fixed="right"
          >
            <template slot-scope="scope">
              <el-tooltip content="详细" placement="top">
                <el-button type="text" @click="showDetailsData(scope.row)" icon="el-icon-document"></el-button>
              </el-tooltip>
              <el-tooltip content="编辑" placement="top">
                <el-button
                    type="text"
                    :disabled="scope.row.schedulingPlanStatus === 4 || scope.row.schedulingPlanStatus === 5"
                    @click="editData(scope.row)"
                    icon="el-icon-edit-outline"></el-button>
              </el-tooltip>
              <el-tooltip content="状态" placement="top">
                <el-button
                    type="text"
                    :disabled="scope.row.schedulingPlanStatus === 4 || scope.row.schedulingPlanStatus === 5"
                    @click="openStatusEditDialog(scope.row)"
                    icon="el-icon-more"></el-button>
              </el-tooltip>
              <el-tooltip content="删除" placement="top">
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
      <div class="order-details-picture">
        <img :src="item" :alt="index" v-for="(item, index) in tableDetailsPicturesList">
      </div>
    </el-dialog>


    <el-dialog
        :title="reImportingOrderData.length === 0 ? '导入未排产订单' : '重排已排产订单'"
        :visible.sync="isOrderImporting"
        :close-on-click-modal="false"
        :close-on-press-escape="false"
        @closed="resetOrderImportDialog"
        width="90%">

      <el-table
          class="order-import-table"
          borderi
          size="small"
          :data="reImportingOrderData.length === 0 ? importingOrderData : reImportingOrderData"
          max-height="560"
          ref="importtablecomponent"
          @selection-change="orderImportSelectionChange">
        <el-table-column
            type="selection"
            width="40">
        </el-table-column>
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
      <span slot="footer" class="dialog-footer">
        <el-button size="small" type="info" @click="isOrderImporting = false">取消</el-button>
        <el-button size="small" type="primary" @click="setOrderImportOptions">配置</el-button>
      </span>

      <edit-importing-order-comp
          v-if="isOrderImportingSetting"
          :is-order-importing-setting.sync="isOrderImportingSetting"
          :importing-orders="importingOrderSelection"
          :active-process-group="activeProcessGroup"
          :line-group="lineSelectGroup"
          :src-orders="importingOrderData"
          :order-columns="importingOrderColumns"
          :is-re-import="isReImport"
          @parentDailogClose="resetOrderImportDialog"
          v-on:reload="partlyReload"/>
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
                     v-if="planEditRow.schedulingPlanStatus === 3 || planEditRow.schedulingPlanStatus === 1"></el-option>
          <!--<el-option :value="2" label="完成"></el-option>-->
          <el-option :value="3" label="暂停"
                     v-if="planEditRow.schedulingPlanStatus === 2"></el-option>
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
    <gantt-comp :prop-table-data="ganttData" v-if="isShowingGantt"/>
  </div>
</template>

<script>
  import {
    planQueryOptions,
    planTableColumns,
    planTableExtraOrderColumns,
    planTableExtraUserColumns,
    orderTableColumns,
    orderUnscheduledTableColumns,
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
    planDetailsReworkSelectUrl,
    planProcessGroupSelectUrl,
  } from "../../../config/globalUrl";
  import PlanEditPanel from './comp/PlanEditPanel';
  import GanttComp from './comp/GanttComp'
  import eventBus from "../../../utils/eventBus";
  import {MessageBox} from "element-ui";
  import {saveAs} from 'file-saver';
  import EditImportingOrderComp from "./comp/EditImportingOrderComp";

  export default {
    name: "PlanSetting",
    inject: ['reload'],
    components: {
      EditImportingOrderComp,
      PlanEditPanel,
      GanttComp
    },
    data() {
      return {
        fetchDateAble:true,     //加载时第一个工序组标签是否有二级标签有二级标签就不能获取 产能数据，
        sessionFactory:sessionStorage.getItem('factory'),
        queryOptions: planQueryOptions,
        thisQueryOptions: {},
        //预加载信息
        processSelectGroupSrc: [],
        processSelectGroup: [],
        processGroupSelectGroup: [],
        viceGroup:[],                //存放副标签
        lineSelectGroupSrc: [],
        lineSelectGroup: [],
        tableData: [],
        tableColumns: planTableColumns,
        tableDetailsOrderData: [],
        tableDetailsUserData: [],
        tableDetailsPicturesList: [],
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
        activeTag:{},
        activeProcessGroup: -1,
        activeProcessGroupType: 0, //selectReworkPlan param
        //导入未排产订单
        isOrderImporting: false, //导入订单显示控制
        isOrderImportingSetting: false, //编辑选中订单显示控制
        isOrderImportingSettingShowing: false, //已暂存的订单详情显示控制
        showingStashImportingSetting: {}, //显示暂存的订单详情
        importingOrderData: [],
        reImportingOrderData: [], //重排订单
        importingOrderDataTemp: [], //用于临时存放已配置的数据
        importingOrderColumns: orderUnscheduledTableColumns,
        importingOrderSelection: [],
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
        isShowingGantt: false,
        ganttData: [],


        //切换查询栏伸缩
        isQueryCompHidden: false,

        //是否重排订单
        isReImport:false
      }
    },
    watch: {
      activeProcessGroup: function (val) {
        if (val !== -1) {
          this.lineSelectGroup = [];
          let childrenGroup = this.processGroupSelectGroup.filter(item=>item.id === val)   //子工序组标签没有对应的 产线的话  就找父标签对应的产线
          this.lineSelectGroupSrc.forEach(item => {
            if (item.processGroup === val || item.processGroup === childrenGroup[0].parentGroup) {
              this.lineSelectGroup.push(item)
            }
          })
        }
      }
    },
    async created() {
      this.$openLoading();
      this.initQueryOptions();
      await this.dataPreload();
      this.$closeLoading();
      //加载表格

      if(this.processGroupSelectGroup.length>0){
        this.activeProcessGroup = this.processGroupSelectGroup[0].id
        this.fetchProcessGroup(this.activeProcessGroup)                              //加载时显示的第一个工序组标签 有子标签的话显示，没有的话获取 排产数据
      }else {
        this.$alertDanger('获取工序组失败')
      }
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
        _partlyReload(['thisQueryOptions', 'lineSelectGroupSrc', 'lineSelectGroup', 'processSelectGroupSrc', 'processGroupSelectGroup', 'activeProcessGroup'])
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
      fetchProcessGroup: function (parentGroup = undefined) {
        return new Promise(resolve => {
          axiosFetch({
            url: planProcessGroupSelectUrl,
            data:{
              parentGroup:parentGroup,
              factory:this.sessionFactory === '1'?'0':this.sessionFactory,
            }
          }).then(response => {
            if (response.data.result === 200) {
              //加载时查询工序组
              if(typeof(parentGroup) === "undefined"){             //获取一级工序组标签
              // if(this.activeProcessGroup === ""){             //获取一级工序组标签
                if(response.data.data.list.length >0){
                  if(this.sessionFactory ==='1'){                  //集团角色  全部都显示
                    this.processGroupSelectGroup = response.data.data.list;
                  }else{
                    response.data.data.list.forEach((item,i)=>{
                      if(item.factoryId === Number(this.sessionFactory)){
                        this.processGroupSelectGroup.push(item)
                      }
                    })
                  }
                }else{
                  this.$alertWarning('未设置工序组')
                }
              }else{                                              //点击一级工序组标签（传自己的id：就是paretGroup字段）
                if(response.data.data.list.length >0){            //有二级工序组标签的情况
                  this.fetchDateAble = false;                     //加载时 如果第一个页签就有二级工序组标签的话  不允许直接获取产能数据
                  this.viceGroup = response.data.data.list
                }else{                                            //没有二级工序组标签的情况：直接获取工序组产能数据
                  this.queryData();
                }
              }
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            console.log(err)
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
        this.viceGroup = [];
        this.tableData = [];
        this.activeProcessGroup = item.id;
        this.activeProcessGroupType = item.id - 1;
        this.initQueryOptions();
        this.fetchProcessGroup(item.id)
      },
      viceSwitchTag: function (item) {
        this.activeProcessGroup = item.id;
        this.activeProcessGroupType = item.parentGroup - 1;
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
              factory:this.sessionFactory === '1' ? '0':this.sessionFactory,
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
          options.data.factory = this.sessionFactory === '1' ? '0':this.sessionFactory;
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
          if(this.activeProcessGroupType === 0){
            this.activeProcessGroupType = this.processGroupSelectGroup[0].id-1
          };
          this.isPending = true;
          this.$openLoading();

          axiosFetch({
            url: url === 'rework' ? planDetailsReworkSelectUrl : planDetailsUnscheduledSelectUrl,
            data: {
              processGroupId:this.activeProcessGroup,
              // type: this.activeProcessGroupType, //track
              factory:this.sessionFactory === '1' ? '0':this.sessionFactory
            }
          }).then(response => {
            if (response.data.result === 200) {
              this.importingOrderData = response.data.data;
              if (this.importingOrderData && this.importingOrderData.length !== 0) {
                this.isOrderImporting = true;
              } else {
                this.$alertInfo('当前工序组下无可排产订单')
              }
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
        if (this.importingOrderSelection.length > 0) {
          let noEmptyArray = this.importingOrderSelection.filter(item => {
            return item.unscheduledQuantity === 0
          });
          if (noEmptyArray.length > 0) {
            this.$alertInfo('存在未排产数量为0的订单');
            return;
          }
          let noCapacityArray = this.importingOrderSelection.filter(item => {
            return item.capacity === 0
          });
          if (noCapacityArray.length > 0) {
            this.$alertInfo('存在产能为0的订单，请前往产能管理模块添加信息');
            return;
          }
          this.isOrderImportingSetting = true
        } else {
          this.$alertInfo("请选择订单")
        }
      },



      resetOrderImportDialog: function () {
        this.isOrderImporting = false;
        this.importingOrderData = [];
        this.importingOrderDataTemp = [];
        this.reImportingOrderData = [];
        this.isReImport = false;
        this.importingOrderSelection = null;
      },

      editData: function (val) {
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
              this.tableDetailsPicturesList = Object.keys(response.data.data.picture).map(key => response.data.data.picture[key]);
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
                this.isShowingGantt = true;
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
      },

      toggleQueryCompHide() {
        let queryComp = this.$refs['query-comp'];
        let foldComp = this.$refs['query-comp-fold'];
        if (this.isQueryCompHidden) {
          queryComp.style.height = 'unset';
          foldComp.style.cssText = 'box-shadow: unset';
        } else {
          queryComp.style.height = queryComp.clientHeight * 0.38 + 'px';
          foldComp.style.cssText = 'box-shadow: inset 0px -10px 15px -8px #CFCFCF;';
        }
        this.isQueryCompHidden = !this.isQueryCompHidden;
      },

      /*重排已排产*/
      showOrderPlannedImport() {
        this.reImportingOrderData = this.tableData.filter(item => item.statusName === '已排产');
        if (this.reImportingOrderData.length !== 0) {
          this.reImportingOrderData.map(item => {

            item.unscheduledQuantity = item.schedulingQuantity;
            item.lineChangeTime = Number(item.lineChangeTime)
            return item
          });
          this.isReImport = true;
          /*获取子页面插单功能的列表源*/
          this.showOrderImport();
        } else {
          this.$alertInfo('当前工序组不存在已排产订单');
        }
      }
    }
  }
</script>

<style scoped>
  #plan-setting /deep/ .el-button i {
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
    position: relative;
    background-color: #ffffff;
    border: 1px solid #eeeeee;
    border-radius: 5px;
    padding: 10px 20px;
    display: flex;
    flex-wrap: wrap;
    overflow: hidden;
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

  .query-comp-fold {
    width: 100%;
    position: absolute;
    bottom: 0;
    left: 0;
    display: flex;
    align-items: center;
    justify-content: center;
  }

  .query-comp-fold-btn {
    color: #c1c1c1;
    cursor: pointer;
    z-index: 10;
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
    min-width: 110px!important;
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
  .vice-tag{
    width: 100%;
    margin-top: 5px;
    min-height: 50px;
  }
  .vice-tag /deep/ .el-button{
    margin-top: 5px;
  }
  .order-details-title {
    margin-top: 5px;
    font-weight: bold;
    color: #909399;
  }

  .order-details-picture {
    width: 100%;
    height: auto;
  }
  .order-details-picture img {
    width: 100%;
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
