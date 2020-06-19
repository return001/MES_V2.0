<!--<script src="../../../../../public/js/config.js"></script>-->
<template>
  <el-dialog
    title="排产"
    :visible="isOrderImportingSetting"
    :close-on-click-modal="false"
    :close-on-press-escape="false"
    width="96%"
    @close="cancelEdit"
    append-to-body>
    <div class="dialog-container">
      <el-form
        ref="order-setting-form"
        :model="planOptions"
        label-position="top"
        @submit.native.prevent
        :rules="planOptionsRules"
        class="order-setting">
        <!--        <el-form-item-->
        <!--          size="mini"-->
        <!--          label="排产预计开始时间"-->
        <!--          prop="planStartTime"-->
        <!--          class="order-setting-comp">-->
        <!--          <el-date-picker-->
        <!--            v-model="planOptions.planStartTime"-->
        <!--            type="datetime"-->
        <!--            value-format="yyyy-MM-dd HH:mm:ss"-->
        <!--            default-time="08:00:00"-->
        <!--            autocomplete="off"-->
        <!--            size="mini"-->
        <!--            prefix-icon="el-icon-date"-->
        <!--            :disabled="!isAuto"-->
        <!--            @change="checkIsInSchedule($event, clearPlanStartTime)"-->
        <!--          >-->
        <!--          </el-date-picker>-->
        <!--        </el-form-item>-->
        <!--        <el-form-item-->
        <!--          size="mini"-->
        <!--          label="预排产天数"-->
        <!--          prop="planDays"-->
        <!--          class="order-setting-comp __text">-->
        <!--          <el-input-->
        <!--            type="text"-->
        <!--            v-model="planOptions.planDays"-->
        <!--            autocomplete="off"-->
        <!--          ></el-input>-->
        <!--        </el-form-item>-->
        <!-- <el-form-item
             size="mini"
             label="一天可排产时长(小时)"
             prop="planTimePerDay"
             class="order-setting-comp __text">
           <el-input
               type="text"
               v-model="planOptions.planTimePerDay"
               autocomplete="off"
           ></el-input>
         </el-form-item>
         <el-form-item
             size="mini"
             label="工作间休息时长(小时)"
             prop="restTime"
             class="order-setting-comp __text">
           <el-input
               type="text"
               v-model="planOptions.restTime"
               autocomplete="off"
           ></el-input>
         </el-form-item>
         <el-form-item
             size="mini"
             label="每天生产开始时间"
             prop="startTimePerDay"
             class="order-setting-comp">
           <el-time-picker
               v-model="planOptions.startTimePerDay"
               autocomplete="off"
               size="mini"
               :picker-options="{
                 selectableRange: '00:00:00 - 23:59:59'
               }"
               :default-value="new Date('1970.01.01 08:00:00')"
               prefix-icon="el-icon-alarm-clock"
           >
           </el-time-picker>
         </el-form-item>-->
        <el-switch
          size="mini"
          v-model="isAuto"
          active-text="自动排产"
          inactive-text="手动排产"
          active-color="#409EFF"
          inactive-color="#6CFF87"
          class="order-setting-comp __swicth"></el-switch>
        <div class="import-order-tips">
          <div class="import-tips-container timeout">字样</div>
          <p>预计时间异常</p>
          <div class="import-tips-container quantity-out"></div>
          <p>排产数量超额(不计算重排订单)</p>
        </div>
        <!--        <div class="order-setting-comp __btn">-->
        <!--          <el-button size="mini" type="info" @click="initPlanOptions">重置</el-button>-->
        <!--          <el-button size="mini" type="primary" @click="calcOptions">计算</el-button>-->
        <!--        </div>-->
      </el-form>
      <div class="order-list-setting">
        <el-table
          :data="tableData"
          max-height="560"
          ref="list-table-component"
          :row-class-name="setHighlight"
          size="mini">
          <el-table-column
            type="index"
            fixed="left"
            width="40">
          </el-table-column>
          <el-table-column
            label="是否紧急"
            width="50"
          >
            <!--            :disabled="!isOptionsEditable"-->
            <template slot-scope="scope">
              <el-checkbox
                v-model="tableEditData[scope.row.id].isUrgent"></el-checkbox>
            </template>
          </el-table-column>
          <el-table-column
            label="产线"
            width="260"
          >
            <template slot-scope="scope">
              <el-select
                size="small"
                v-model="tableEditData[scope.row.id].line"
                placeholder="请选择产线">
                <el-option v-for="listItem in lineGroup"
                           :key="listItem.id"
                           :value="listItem.id"
                           :label="listItem.lineName"></el-option>
              </el-select>
            </template>
          </el-table-column>
          <el-table-column
            label="转线时长(分)"
            width="80"
          >
            <template slot-scope="scope">
              <el-input class="table-inner-input-text"
                        type="text"
                        size="mini"
                        @change="changeLineChangeTime"
                        v-model.number="tableEditData[scope.row.id].lineChangeTime"></el-input>
            </template>
          </el-table-column>
          <el-table-column
            label="排产数量"
            width="90"
          >
            <template slot-scope="scope">
              <el-input class="table-inner-input-text"
                        type="text"
                        size="mini"
                        @change="changeQuantity($event, scope.row)"
                        v-model.number="tableEditData[scope.row.id].schedulingQuantity"></el-input>
            </template>
          </el-table-column>
          <el-table-column v-for="(item, index) in tableColumns"
                           :key="index"
                           :prop="item.key"
                           :label="item.label"
                           :min-width="item['min-width']">
          </el-table-column>

          <el-table-column
            label="预计生产时长"
            width="100"
            fixed="right"
          >
            <template slot-scope="scope">
              <span>{{tableEditData[scope.row.id].planInterval}}</span>
            </template>
          </el-table-column>
          <el-table-column
            label="预计开始时间"
            :width="isAuto ? '150' : '190'"
            fixed="right"
          >
            <template slot-scope="scope">
              <el-date-picker
                v-model="tableEditData[scope.row.id].planStartTime"
                type="datetime"
                value-format="yyyy-MM-dd HH:mm:ss"
                autocomplete="off"
                size="mini"
                prefix-icon="el-icon-date"
                v-if="!isAuto"
                default-time="08:00:00"
                class="table-column-date"
                @change="manualCalcCompleteTime($event, scope.$index)"
              >
              </el-date-picker>
              <span v-else>{{tableEditData[scope.row.id].planStartTime}}</span>

            </template>
          </el-table-column>
          <el-table-column
            label="预计结束时间"
            :width="isAuto ? '160' : '200'"
            fixed="right"
          >
            <template slot-scope="scope">
              <el-date-picker
                v-model="tableEditData[scope.row.id].planCompleteTime"
                type="datetime"
                value-format="yyyy-MM-dd HH:mm:ss"
                autocomplete="off"
                size="mini"
                prefix-icon="el-icon-date"
                v-if="!isAuto"
                class="table-column-date"
                @change="checkIsInSchedule($event, clearDataItemEndTime, scope.$index)"
              >
              </el-date-picker>
              <span v-else>{{tableEditData[scope.row.id].planCompleteTime}}</span>
            </template>
          </el-table-column>
          <el-table-column
            label="操作"
            width="160"
            fixed="right"
          >

            <template slot-scope="scope">
              <el-tooltip content="排产详情" placement="top">
                <el-button type="text" icon="el-icon-t-table" @click="showDetails(scope.row)"></el-button>
              </el-tooltip>
              <el-tooltip content="插入" placement="top">
                <el-button type="text" icon="el-icon-plus" @click="insertOrder(scope.$index)"></el-button>
              </el-tooltip>
              <el-tooltip content="上移" placement="top">
                <el-button type="text" icon="el-icon-sort-up" @click="changeOrderPos(0, scope.$index)"
                           :disabled="scope.$index === 0"></el-button>
              </el-tooltip>
              <el-tooltip content="下移" placement="top">
                <el-button type="text" icon="el-icon-sort-down" @click="changeOrderPos(1, scope.$index)"
                           :disabled="scope.$index === (tableData.length - 1)"></el-button>
              </el-tooltip>
              <el-tooltip content="移除" placement="top">
                <el-button type="text" icon="el-icon-delete" @click="deleteOrder(scope.$index)"></el-button>
              </el-tooltip>
            </template>
          </el-table-column>
        </el-table>
      </div>
    </div>


    <!--    <div class="import-working-schedule">-->
    <!--      <div class="schedule-title">排班表:</div>-->
    <!--      <div v-for="(val, key) in workingSchedule" class="schedule-item">{{scheduleMap[key] + ': ' + (!!val ? val :-->
    <!--        '未设置')}}-->
    <!--      </div>-->
    <!--    </div>-->
    <!-- 设置产线时间 -->
    <div class="setting-time">
      <el-tooltip class="item" effect="dark" content="设置产线工作时间" placement="top-start">
        <button style="outline: none;border: none;background-color: rgba(0,0,0,0)" @click="handleTimeSetting">
          <span><i style="color:#409EFF;font-size: 30px;" class="el-icon-t-setting"></i></span>
        </button>
      </el-tooltip>
    </div>

    <el-dialog
      title="设置时间"
      :visible.sync="isTimeSetting"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @close="closeTimeSetting"
      append-to-body>
      <div class="time-setting-tip">
        <el-select
          v-model="choiceLine"
          @change="handleChoiceLine"
          placeholder="请选择产线">
          <el-option v-for="listItem in lineGroup"
                     :key="listItem.id"
                     :value="listItem.id"
                     :label="listItem.lineName"></el-option>
        </el-select>

        <el-date-picker
          type="dates"
          v-model="lineDate"
          placeholder="选择一个或多个日期"
          @change="handleChoiceDate"
          value-format="yyyy-MM-dd">
        </el-date-picker>
      </div>
      <div class="allreday-choice-date">
        <el-tag v-for="item in choicedDate"
                size="medium"
                effect="dark"
                closable
                @close="handleClose(item)"
                @click="handleShowDateTime(item)"
                :type="clicked === item ? 'info' : ''">{{item}}</el-tag>
      </div>
      <div style="margin-top: 20px">
        <el-table
          class="time-setting-table"
          size="small"
          :data="workTime"
          max-height="560">

          <el-table-column
            label="序号"
            width="50"
            type="index">
          </el-table-column>

          <el-table-column
            label="时段"
            width="500">
            <template slot-scope="scope">
              <div style="display: inline-block;margin-right: 30px">
                <el-time-picker
                  prefix-icon="el-icon-t-schedule"
                  v-model="workTime[scope.$index].startTime"
                  format="HH:mm"
                  value-format="HH:mm"
                  placeholder="开始时间点"
                  editable>
                </el-time-picker>
              </div>

              <el-time-picker
                prefix-icon="el-icon-t-schedule"
                v-model="workTime[scope.$index].endTime"
                format="HH:mm"
                value-format="HH:mm"
                placeholder="结束时间点"
                editable>
              </el-time-picker>
            </template>
          </el-table-column>
          <el-table-column
            label="操作"
            float="right"
            width="150"
          >
            <template slot-scope="scope">
              <el-tooltip content="删除" placement="top">
                <el-button size="small" type="primary" circle class="el-icon-minus" @click="handleDeleteTime(scope.$index)"></el-button>
              </el-tooltip>
            </template>
          </el-table-column>
        </el-table>

        <div style="margin-top: 20px">
          <el-tooltip content="添加" placement="top">
            <el-button size="small" type="primary" circle icon="el-icon-plus" @click="handleAddTime"></el-button>
          </el-tooltip>
        </div>
      </div>


      <span slot="footer" class="dialog-footer">
        <el-button size="small" type="success" @click="submitLineTimeSetting">确定</el-button>
        <el-button size="small" type="primary" @click="closeTimeSetting">完成</el-button>
      </span>
    </el-dialog>

    <!-- 排产详情 -->
    <el-dialog
      title="详情"
      :visible.sync="isDetailsShowing"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @closed="isDetailsShowing = false"
      append-to-body
      width="88%">
      <el-table
        :data="detailData"
        max-height="500"
        ref="tablecomponent"
        border
        size="small"
        stripe>
        <el-table-column v-for="(item, index) in planOrderDetail"
                         :key="index"
                         :prop="item.key"
                         :label="item.label"
                         :min-width="item['min-width']"
                         :formatter="item.formatter">
        </el-table-column>
        <!--        <el-table-column>-->

        <!--        </el-table-column>-->
      </el-table>

      <span slot="footer" class="dialog-footer">
        <el-button size="small" type="primary" @click="isDetailsShowing = false">确定</el-button>
      </span>


    </el-dialog>

    <span slot="footer" class="dialog-footer">
          <el-button size="small" type="primary" @click="submitOptions">计算</el-button>
<!--          <el-button size="small" type="primary" @click="calcOptions">计算</el-button>-->
      <!--          <el-button size="small" type="success" @click="calcOptions">解析</el-button>-->
      <!--          <el-button size="small" type="info" @click="cancelEdit">取消</el-button>-->
          <el-button size="small" type="primary" :disabled="!isImportable" @click="submitEdit">导入</el-button>
      </span>

    <el-dialog
      title="插入订单"
      :visible.sync="isInserting"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      width="80%"
      @close="closeInsert"
      append-to-body>

      <el-table
        class="order-import-table"
        border
        size="small"
        :data="srcOrders"
        max-height="560"
        ref="inserttablecomponent"
        highlight-current-row
        :row-class-name="setExistHighlight"
        @current-change="orderInsertSelectionChange">
        <el-table-column
          type="index"
          width="50">
        </el-table-column>
        <el-table-column v-for="(item, index) in orderColumns"
                         :key="index"
                         :prop="item.key"
                         :label="item.label"
                         :min-width="item['min-width']"
                         :formatter="item.formatter">
        </el-table-column>

      </el-table>
      <div class="insert-tips">
        <div class="insert-tips-container exist-highlight"></div>
        <p>已存在于预排产列表</p>
      </div>
      <span slot="footer" class="dialog-footer">
        <el-button size="small" type="info" @click="isInserting = false">取消</el-button>
        <el-button size="small" type="primary" @click="submitInsert">插入</el-button>
      </span>
    </el-dialog>
  </el-dialog>
</template>

<script>
  import Moment from 'moment'
  import {
    planDetailsAddUrl,
    planDetailsReAddUrl,
    // getWorkingScheduleUrl,
    planDetailsPlanCalculateUrl,
    planLineTimeSelectUrl,
    planLineTimeSettingUrl,
    planOrderDetailSelectUrl,
  } from "../../../../config/globalUrl";
  import{planOrderDetail} from "../../../../config/planConfig";
  import {axiosFetch} from "../../../../utils/fetchData";
  import {MessageBox} from "element-ui";
  export default {
    name: "EditImportingOrderComp",
    props: ['isOrderImportingSetting', 'importingOrders', 'activeProcessGroup', 'lineGroup', 'srcOrders', 'orderColumns', 'isReImport'],
    data() {
      return {
        choiceLine:'',       //选择产线
        eqLine:'',           //选择产线(问题)
        isTimeSetting:false, //设置时间 dialog 显示
        lineDate:'',         //产线日期
        trueTimes:true,      //设置时间时是验证表单（暂时不用）
        clicked:'',          //设置时间页面 点击日期变色
        // isExist:"",          //当前产线是否已经设置过时间
        workTime:[
          {
            startTime:'',       //产线每个时段的开始时间
            endTime:'',         //产线每个时段的结束时间
          },
          {
            startTime:'',
            endTime:'',
          },
        ],
        defaultData:[],
        isCommon:true,         //是否为公共数据，排产时，true:该产线没设置过时间
        unDefaultData:[],       //default 为false （提交到数据库中的时间）
        dateToTime:{},          //保存  2020-06-01:[{s:...,e:...},{s:...,e:...}]  页面显示
        setDate:false,
        choicedDate:[],         //页面中显示已选择过的日期

        workTimesItems:{},
        timesAndTasks:{       //最终要提交计算的数据
          workTimes:[],
          tasks:[],
        },
        //订单排产详情
        planOrderDetail:planOrderDetail,
        isDetailsShowing:false, //该订单排产详情
        detailData:[],          //详情数据

        planOptions: {
          planStartTime: '',
          planDays: null,
          planTimePerDay: null,
          // restTime: null,
          //startTimePerDay: '',
          // line: null
        },
        planOptionsRules: {
          'planStartTime': [
            {required: true, message: '请输入预计开始时间', trigger: 'blur'}
          ],
          'planDays': [
            {required: true, message: '请输入预排产天数', trigger: 'blur'}
          ],
          /*'planTimePerDay': [
            {required: true, message: '请输入一天可排产时长', trigger: 'blur'}
          ],
          'restTime': [
            {required: true, message: '请输入工作间休息时长', trigger: 'blur'}
          ],
          'startTimePerDay': [
            {required: true, message: '请输入生产开始时间', trigger: 'blur'}
          ],*/
          'line': [
            {required: true, message: '请选择产线', trigger: 'blur'}
          ],
        },
        tableColumns: [
          {'label': '订单号/生产PO', 'key': 'zhidan', 'min-width': '100px'},
          {'label': '订单数量', 'key': 'quantity', 'min-width': '60px'},
          {'label': '未排产数量', 'key': 'unscheduledQuantity', 'min-width': '60px'},
          {'label': '人数', 'key': 'processPeopleQuantity', 'min-width': '50px'},
          {'label': '标准产能', 'key': 'capacity', 'min-width': '60px'},
          {'label': '内部替换号', 'key': 'alias', 'min-width': '60px'},
          {'label': '机型', 'key': 'softModel', 'min-width': '60px'},
          // {'label': '版本号', 'key': '', 'min-width': '100px'},
          {'label': '物料状态','key': 'materialState', 'min-width': '90px'},
          {'label': '齐料时间', 'key': 'enoughMaterialTime', 'min-width': '90px'},
          {'label': 'PCBA', 'key': 'pcba', 'min-width': '90px'},
          {'label': '客户料号', 'key': 'customerMaterialNo', 'min-width': '100px'},
          {'label': '客户编号', 'key': 'customerNumber', 'min-width': '100px'},
          {'label': '客户名称', 'key': 'customerName', 'min-width': '100px'},
          {'label': '订单日期', 'key': 'orderDate', 'min-width': '95px'},
          {'label': '交货日期', 'key': 'deliveryDate', 'min-width': '95px'},
        ],
        tableData: [],
        tableTollData:[],    //获取排产时的 未排产数
        tableEditData: {},
        isOptionsEditable: false,
        isAuto: true,
        isImportable: false,
        isPending: false,
        insertingPosition: null,
        isInserting: false,
        insertingOrder: undefined,
        existOrders: [],
        currentOrderQuantity: undefined,
        workingSchedule: {},
        scheduleMap: {
          "0": "白班上班时间",
          "1": "白班下班时间",
          "2": "中午上班时间",
          "3": "下午下班时间",
          "4": "夜班上班时间",
          "5": "夜间下班时间",
          "6": "夜间上班时间",
          "7": "夜班下班时间",
        },
        timeIntervalMinutes: {
          "0": 0,
          "1": 0,
          "2": 0,
          "3": 0,
        }
      }
    },
    watch: {
      'importingOrders': function (val) {
        if (val) {
          this.tableData = val.map(item => {
            this.$set(this.tableEditData, item.id, {
              id: this.isReImport ? item.id : undefined,
              order: this.isReImport ? item.orders : item.id,
              remark: null,
              schedulingQuantity: item.unscheduledQuantity,
              line: item.line || this.lineGroup[0].id,
              capacity: item.capacity,
              processGroup: this.activeProcessGroup,
              planStartTime: '',
              planCompleteTime: '',
              rhythm: item.rhythm,
              personNumber: item.processPeopleQuantity,
              lineChangeTime: item.transferLineTime,
              planInterval: null,
              isUrgent: Boolean(item.isUrgent),
              zhidan:item.zhidan
            });
            return item
          })
        }
      },
      'planOptions': function (val) {
        this.isOptionsEditable = false
      },
      'isAuto': function (val) {
        this.planOptionsRules['planStartTime'][0].required = val
      },
      'tableData': function (val) {
        this.existOrders = val.map(item => item.id)
      },
      'tableEditData': {
        handler: function (val) {
          //存储 订单：数量
          let obj = {};
          Object.keys(val).forEach(key => {
            //仅统计非重排单
            if (val[key].id === undefined) {
              if (obj[val[key].order]) {
                obj[val[key].order] += val[key].schedulingQuantity
              } else {
                obj[val[key].order] = val[key].schedulingQuantity
              }
            }
          });
          this.currentOrderQuantity = obj;
        },
        deep: true
      }
    },
    computed: {},
    async mounted() {
      if(this.lineGroup.length <= 0){
        this.$alertWarning("该工序组未设置产线")
        return;
      }
      this.$openLoading();
      // this.workingSchedule = await this.getWorkingSchedule();
      this.planOptions.planTimePerDay = this.calcWorkingTimePerDay();
      this.tableData = this.importingOrders.map(item => {
        //仅重排单拥有id
        this.$set(this.tableEditData, item.id, {
          id: this.isReImport ? item.id : undefined,
          order: this.isReImport ? item.orders : item.id,
          remark: null,
          schedulingQuantity: item.unscheduledQuantity,
          line: item.line || this.lineGroup[0].id,
          capacity: item.capacity,
          processGroup: this.activeProcessGroup,
          planStartTime: '',
          planCompleteTime: '',
          rhythm: item.rhythm,
          personNumber: item.processPeopleQuantity,
          // lineChangeTime: typeof(item.lineChangeTime) === "undefined" ? item.transferLineTime : item.lineChangeTime,
          lineChangeTime: this.isReImport ? item.lineChangeTime : item.transferLineTime ,
          planInterval: null,
          isUrgent: Boolean(item.isUrgent),
          zhidan:item.zhidan
        });
        return item
      });
      this.tableTollData = JSON.parse(JSON.stringify(this.tableData))
      this.$closeLoading()
    },
    methods: {
      initAll() {
        Object.assign(this.$data, this.$options.data());
      },
      initPlanOptions() {
        Object.assign(this.$data.planOptions, this.$options.data().planOptions)
      },
      cancelEdit() {
        this.$emit('parentDailogClose')  //isOrderImporting
        this.$emit('update:isOrderImportingSetting', false);
        this.initAll();
        this.timesAndTasks={       //最终要提交计算的数据  关闭页面时清空
          workTimes:[],
          tasks:[],
        };
      },

      //详情
      showDetails(val){
        this.isDetailsShowing= true
        axiosFetch({
          url:planOrderDetailSelectUrl,
          data:{
            orderId:val.id,
          }
        }).then(response=>{
          if(response.data.result === 200){
            this.detailData = response.data.data;
            this.detailData.forEach(item=>{
              item.unSchedulingQuantity = item.quantity - item.schedulingQuantity
            })
          }else{
            this.$alertWarning(response.data.data)
          }
        }).catch(err=>{
          this.$alertWarning('未知错误')
        })
      },


      /**
       * 设置产线时间
       */
      handleTimeSetting(){
        this.isTimeSetting = true;
      },

      handleAddTime(){
        this.workTime.push({
          startTime:'',
          endTime:'',
        })
      },
      handleDeleteTime(val){
        if(this.choiceLine === ""){
          this.$alertWarning('请选择产线');
          return
        }
        if(this.lineDate === null || this.lineDate.length <= 0){
          this.$alertWarning('请选择日期');
          return
        }
        if(this.workTime.length <= 1){
          if(this.lineDate.length > 1){
            this.$alertWarning('只能清空单个日期的时间')
            return
          }else{
            this.handleClose(this.clicked)
          }
        }
        this.workTime.splice(val,1)
      },

      //清空时间设置页面的选项
      cleanOptions(){
        this.choiceLine= '';
        this.lineDate= '';
        this.workTime = [
          {
            startTime:'',       //产线每个时段的开始时间
            endTime:'',         //产线每个时段的结束时间
          },
          {
            startTime:'',
            endTime:'',
          },
        ]
      },

      //关闭/取消页面
      closeTimeSetting(){
        this.cleanOptions()
        this.choicedDate = [];
        this.isTimeSetting = false;
      },

      //选择产线
      handleChoiceLine(val){
        this.lineDate = [];
        this.choicedDate = [];
        //获取公共的时间设置
        axiosFetch({
          url:planLineTimeSelectUrl,
          data:{
            executorId:val,
            isDefault:true,
          }
        }).then(response=>{
          if(response.data.result === 200){
            if(response.data.data.length > 0){
              this.defaultData = response.data.data
              // this.isCommon = true
              this.defaultData.forEach(item=>{
                delete item.date
              })
            }
          }else{
            this.$alertWarning(response.data.data)
          }
        }).catch(err => {
          this.$alertDanger('未知错误')
        }).finally(() => {
        })

        //获取私有的时间设置
        axiosFetch({
          url:planLineTimeSelectUrl,
          data:{
            executorId:val,
            isDefault:false,
          }
        }).then(response=>{
          if(response.data.result === 200){
            if(response.data.data.length > 0) {
              this.unDefaultData = response.data.data
              this.isCommon = false;
              //取到的数据放到页面
              let obj = {}
              this.unDefaultData.forEach(item => {
                if (!(item.date in obj)) {
                  obj[item.date] = [{startTime: item.startTime, endTime: item.endTime}]
                } else {
                  obj[item.date].push({startTime: item.startTime, endTime: item.endTime})
                }
              })
              this.dateToTime = obj
              this.choicedDate = Object.keys(this.dateToTime).map(choiced=>choiced)

              //选择产线后，出现第一个日期的数据
              this.lineDate = [Object.keys(obj)[0]]
              this.workTime = obj[Object.keys(obj)[0]]
              this.clicked = Object.keys(obj)[0]

              //判断选择的产线在不在从数据库里取出的数据中 在的话，覆盖，不在则直接加
              if(this.timesAndTasks.workTimes.length >0){
                let dateArray = this.timesAndTasks.workTimes.map(item=>item.executorId)
                if (dateArray.indexOf(this.choiceLine) !== -1){
                  this.timesAndTasks.workTimes.forEach(item=> {
                    if (item.executorId === this.choiceLine) {
                      item.workTime = this.unDefaultData
                    }
                  })
                }else{
                    this.timesAndTasks.workTimes.push({executorId:this.choiceLine,workTime:this.unDefaultData})
                  }
              }else{
                this.timesAndTasks.workTimes.push({executorId:this.choiceLine,workTime:this.unDefaultData})
              }
            } else{
              this.workTime = this.defaultData
            }
          }
        }).catch(err => {
          console.log(err)
          this.$alertDanger('未知错误')
        }).finally(() => {
        })

        if(this.defaultData === [] && this.unDefaultData === []){
          this.workTime = [
            {
              startTime:'',       //产线每个时段的开始时间
              endTime:'',         //产线每个时段的结束时间
            },
            {
              startTime:'',
              endTime:'',
            },
          ]
        }
      },

      //选择日期
      handleChoiceDate(){
        if(this.choiceLine === ""){
          this.$alertWarning('请选择产线')
          return
        }
        if(this.lineDate === null){           //清空日期时，显示公共时间
          this.clicked = '';
          this.workTime = this.defaultData;
        }else{
          this.clicked = this.lineDate[0];
          if(Object.keys(this.dateToTime).indexOf(this.lineDate[0]) !== -1){
            this.workTime=this.dateToTime[this.lineDate[0]];
          } else {
            this.workTime = this.defaultData; //产线页面设置的公共时间
          }
          if(this.lineDate.length > 1){
            this.workTime = this.defaultData;
          }
        }
      },

      //补 0 函数
      addZero(s) {
        return s < 10 ? '0' + s : s;
      },

      //点击已选择过的日期  显示对应日期的时间设置
      handleShowDateTime(date){
        this.clicked = date
        this.lineDate = [date];
        this.workTime = this.dateToTime[date];
      },

      //删除日期
      handleClose(date){
        delete this.dateToTime[date]
        console.log(Object.keys(this.dateToTime).length)
        console.log(this.dateToTime)
        if(Object.keys(this.dateToTime).length > 0){
          this.choicedDate = Object.keys(this.dateToTime).map(item=>item)
          this.clicked = this.choicedDate[0]
          this.lineDate = [this.choicedDate[0]]
          this.workTime = this.dateToTime[this.clicked]
          //把没有删除的日期 放入
          let notEmpty=[]
          this.timesAndTasks.workTimes.forEach(item=>{
            if (item.executorId === this.choiceLine){
              item.workTime.forEach(timeItem=> {
                if (date.indexOf(timeItem.date) === -1) {
                  notEmpty.push(timeItem)
                }
                item.workTime = notEmpty
              })
            }
          })
        }else{
          let myDate = new Date();
          let defaultDate = myDate.getFullYear()+"-"+(this.addZero(Number(myDate.getMonth())+1))+"-"+myDate.getDate();
          console.log(defaultDate)
          this.lineDate = [defaultDate];
          this.choicedDate = [];
          this.workTime = []
          //删除全部日期的情况
          this.timesAndTasks.workTimes.forEach((item,i)=>{
            if(item.executorId === this.choiceLine ){
              this.timesAndTasks.workTimes.splice(i,1)
            }
          })
          console.log(this.timesAndTasks.workTimes)
        }
      },

      //提交设置的产线工作时长
      submitLineTimeSetting(){
        console.log(this.timesAndTasks.workTimes)
        //处理 timesAndTasks的 workTimes
        if(this.choiceLine === ""){
          this.$alertWarning('请选择产线');
          return
        }
        if(this.lineDate === null || this.lineDate.length <= 0){
          this.$alertWarning('请选择日期');
          return
        }
        //日期下 时间为空，则代表要删除该日期
        // if(this.workTime.length <= 0) {
        //   this.lineDate.forEach(dateItem=>{
        //     if(this.dateToTime[dateItem] === []){
        //       delete this.dateToTime[dateItem]
        //     }
        //   })
        //   let notEmpty=[]
        //   this.timesAndTasks.workTimes.forEach(item => {
        //     item.workTime.forEach(timeItem=> {
        //       if (this.lineDate.indexOf(timeItem.date) === -1) {
        //         notEmpty.push(timeItem)
        //       }
        //       item.workTime = notEmpty
        //     })
        //   })
        //   this.trueTimes=true
        // }




        if(this.workTime && this.workTime.length > 0){
          this.workTime.forEach((item,i)=>{
            if(item.endTime > item.startTime && item.endTime !=="" && item.startTime !==""){
              this.trueTimes= true
            }else{
              this.trueTimes= false
            }
          })
        }

        if(this.trueTimes === false){
          this.$alertWarning('请设置正确的时间')
          this.workTime=[
            {
              startTime:'',
              endTime:'',
            },
            {
              startTime:'',
              endTime:'',
            },
          ];
        }
        if(this.trueTimes === true){
          let workTime= [];                  //date and time array
          this.lineDate.map(item=>{
            this.workTime.map(ins=>{
              let obj= {
                date:item
              }
              Object.assign(obj,ins)
              workTime.push(obj)
            })
          })
          this.workTimesItems.executorId= this.choiceLine;  //workTimesItems:{executorId:.. ,worktime:{}}
          this.workTimesItems.workTime= workTime;
          let workTimesDeep=JSON.parse(JSON.stringify(this.workTimesItems))
          //排完时间的产线再排的话会覆盖
          if(!(this.timesAndTasks.workTimes.some(item=>item.executorId === workTimesDeep.executorId))){
            //不存在 直接加
            this.timesAndTasks.workTimes.push(workTimesDeep)
          }else{
            //存在 判断
            let sameExeId= {}
            let dates = workTimesDeep.workTime.map(item=>item.date);
            this.timesAndTasks.workTimes.forEach((item,i)=>{
              if(item.executorId === workTimesDeep.executorId){
                sameExeId = item
                this.timesAndTasks.workTimes.splice(i,1);
              }
            })
              sameExeId.workTime.forEach(timeItem=>{
                if(dates.indexOf(timeItem.date) === -1){
                  workTimesDeep.workTime.push(timeItem) //产线相同的情况下,如果日期相同,则覆盖同日期的那条的开始和结束时间{startTime:..,endTime:...,date:...}
                }
              })

            this.timesAndTasks.workTimes.push(workTimesDeep)
          }
          //提交到数据库 为了之后的 重拍已排产
          let options = {
            url: planLineTimeSettingUrl,
            data: {
              executorId:JSON.stringify(workTimesDeep.executorId),
              workTimes:JSON.stringify(workTimesDeep.workTime),
              isDefault:false,
            },
          };
          axiosFetch(options).then(response => {
            if (response.data.result === 200) {
              this.$alertSuccess('产线时间设置成功')
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            console.log(err)
            this.$alertDanger('未知错误')
          }).finally(() => {
            this.isPending = false;
            this.$closeLoading();
            // this.closeTimeSetting();
            // this.cleanOptions();
            // this.isTimeSetting = false;
            this.handleChoiceLine(this.choiceLine)
          })
        }
        this.eqLine = this.choiceLine  //保存起来  因为计算的时候获取不到（暂未找到原因，怀疑是引用类型问题）
      },

      //提交数据给后台计算
      submitOptions(){
        //计算!
        this.timesAndTasks.tasks = []
        let trueSchedulQun = true;
        this.tableData.forEach((item,i)=>{
          if (Number(item.unscheduledQuantity) < 0 ){
            trueSchedulQun = false
          }
          this.timesAndTasks.tasks.push({})
          if (item.id.toString().indexOf('-') !== -1){
            this.timesAndTasks.tasks[i].orderId = Number(item.id.split('-')[0])
          }else{
            this.timesAndTasks.tasks[i].orderId = Number(item.id)
          }
          this.timesAndTasks.tasks[i].executorId = this.tableEditData[item.id].line;
          this.timesAndTasks.tasks[i].switchConsumingTime = this.isReImport ? this.tableEditData[item.id].transferLineTime : this.tableEditData[item.id].lineChangeTime
          this.timesAndTasks.tasks[i].planQuantity = this.tableEditData[item.id].schedulingQuantity
          this.timesAndTasks.tasks[i].standardCapacity = item.capacity
          this.timesAndTasks.tasks[i].tagId =item.id   //用来验证，两条相同订单会出现  订单号 - 随机数 的情况
        })
        if(!trueSchedulQun){
          this.$alertWarning('请输入正确的排产数量(存在未排产数小于零)！')
          return
        }
        this.dateSort(this.timesAndTasks.workTimes)
        // this.timesAndTasks.tasks = []
        //   Object.keys(this.tableEditData).forEach((key,i)=>{
        //     this.timesAndTasks.tasks.push({})
        //     this.timesAndTasks.tasks.forEach((ins,index)=>{
        //       if(i === index){
        //         if (key.indexOf('-') !== -1){
        //           this.timesAndTasks.tasks[index].orderId = Number(key.split('-')[0])
        //         }else{
        //           this.timesAndTasks.tasks[index].orderId = Number(key)
        //         }
        //         this.timesAndTasks.tasks[index].executorId = this.tableEditData[key].line
        //         this.timesAndTasks.tasks[index].switchConsumingTime = this.tableEditData[key].lineChangeTime
        //         this.timesAndTasks.tasks[index].planQuantity = this.tableEditData[key].schedulingQuantity
        //         this.timesAndTasks.tasks[index].standardCapacity = this.tableEditData[key].capacity
        //         this.timesAndTasks.tasks[index].tagId =key   //用来验证，两条相同订单会出现  订单号 - 随机数 的情况
        //       }
        //     })
        //   })

          //处理 timesAndTasks的 tasks
          if(this.timesAndTasks.workTimes.length > 0 && this.timesAndTasks.tasks.length > 0){
            axiosFetch({
              url: planDetailsPlanCalculateUrl,
              data: {
                details:JSON.stringify(this.timesAndTasks)
              },
            }).then(response => {
              if (response.data.result === 200) {
                Object.keys(this.tableEditData).forEach((item,i)=>{
                  response.data.data.forEach(res=>{
                    //用来验证，两条相同订单会出现  订单号 - 随机数 的情况↓
                    // if(Number(item) === res.orderId){
                    if(item === res.tagId){
                      this.$set(this.tableEditData[item], 'planStartTime', res.estimatedStartTime);
                      this.$set(this.tableEditData[item], 'planCompleteTime', res.estimatedEndTime);
                      let pInterval = ''
                      if(res.estimatedProductionTime < 1){
                        pInterval = Math.floor(res.estimatedProductionTime*60)+'分';
                      }else {
                        let houMin = res.estimatedProductionTime.toString().split('.')
                        console.log(houMin)
                        if (typeof (houMin[1]) === 'undefined') {
                          houMin[1] = '0'
                        }
                        console.log(houMin[1])
                        // if((parseFloat('0.'+houMin[1])*60).toString().indexOf('.') !== -1 ){
                          pInterval = parseInt(houMin[0])+'小时'+(Math.floor(parseFloat('0.'+houMin[1])*60))+"分"
                        // }else{
                        //   pInterval = parseInt(houMin[0])+'小时'+(parseFloat('0.'+houMin[1])*60)+"分"
                        // }
                      }
                      this.$set(this.tableEditData[item], 'planInterval', pInterval);}
                  })
                })
                this.isImportable = true;
              } else {
                this.$alertWarning(response.data)
              }
            }).catch(err => {
              this.$alertDanger('计算有误,请刷新重试');
            }).finally(() => {
            })
          }else{
            this.$alertWarning('缺少参数')
          }
      },

      //按日期和开始时间排序
      dateSort(aa){
        aa.forEach(item=>{
          item.timeStemp = this.dateToStemp(item.date+" "+item.startTime+':00')
        })
        aa.sort(function(a, b) {
          return b.timeStemp> a.timeStemp ? -1 : 1;
        });
        aa.forEach(item=>{
          delete item.timeStemp
        })
        return aa
      },

      dateToStemp (str){
        return (new Date(str.replace(/-/g,'/'))).getTime(); //用/替换日期中的-是为了解决Safari的兼容
      },


      // getWorkingSchedule() {
      //   return new Promise(resolve => {
      //     axiosFetch({
      //       url: getWorkingScheduleUrl
      //     }).then(response => {
      //       if (response.data.result === 200) {
      //         let data = {};
      //         Object.keys(response.data.data).forEach(key => {
      //           data[parseInt(key) - 1] = response.data.data[key]
      //         });
      //         resolve(data);
      //       } else {
      //         this.$alertWarning(response.data.data)
      //       }
      //     }).catch(err => {
      //       this.$alertDanger('获取排班时间失败，请刷新重试');
      //     }).finally(() => {
      //       resolve();
      //     })
      //   });
      // },
      /*检查所选时间是否在排班日期内*/
      checkIsInSchedule(date, callback, cbParams) {
        const calcTime = (val) => {
          let group = val.split(':');
          return group[0] * 60 + parseInt(group[1])
        };
        let _date = calcTime(Moment(date).format('HH:mm'));
        let timeGroup = [];
        Object.keys(this.workingSchedule).forEach(key => {
          if (this.workingSchedule[key] !== null) {
            timeGroup.push(calcTime(this.workingSchedule[key]))
          }
        });
        for (let i = 0; i < timeGroup.length; i = i + 2) {
          if (_date >= timeGroup[i] && _date <= timeGroup[i + 1]) {
            return true
          }
        }
        if (callback) callback(cbParams)
      },
      calcWorkingTimePerDay() {
        let calcInterval = (st, et) => {
          let _st = st.split(':');
          let _et = et.split(':');
          let h = parseInt(_et[0]) - parseInt(_st[0]);
          let m = parseInt(_et[1]) - parseInt(_st[1]);
          return h * 60 + m
        };
        let interval = 0;
        for (let i = 0; i < Object.keys(this.workingSchedule).length; i = i + 2) {
          if (this.workingSchedule[i] && this.workingSchedule[i + 1]) {
            let t = calcInterval(this.workingSchedule[i], this.workingSchedule[i + 1]);
            interval += t;
            this.timeIntervalMinutes[(i / 2).toString()] = t;
          }
        }
        return interval;
      },
      /*计算*/
      calcOptions() {
        this.clearPredictTime();
        this.$refs['order-setting-form'].validate(valid => {
          if (valid) {
            if (this.tableData.length === 0) {
              this.$alertInfo('请重新选择订单');
              return;
            }
            /*计算每日开始时间距离0点的分钟数*/
            let startTimePerDayObj = new Date(this.planOptions.startTimePerDay);
            let onWorkTimeToZeroMins = startTimePerDayObj.getHours() * 60 + startTimePerDayObj.getMinutes();
            this.calcPredictTime();
            if (this.isAuto) {
              this.calcTotalPredictStartEndTime();
            } else {
            }
            this.isOptionsEditable = true;
            this.isImportable = true;
          } else {
            this.$alertInfo('请完善配置项')
          }
        })
      },
      /*根据排产数量与产能计算预计时长*/
      calcPredictTime() {
        Object.keys(this.tableEditData).forEach(key => {
          this.tableEditData[key].planInterval = Math.ceil(this.tableEditData[key].schedulingQuantity / this.tableEditData[key].capacity * 10) / 10
        })
      },
      /*手动模式根据预计开始时间计算结束时间*/
      manualCalcCompleteTime(time, index) {
        if (!this.checkIsInSchedule(time, this.clearDataItemStartTime, index)) {
          return;
        }
        let item = this.tableEditData[this.tableData[index].id];
        if (!item.planInterval) {
          this.$set(item, 'planStartTime', '');
          this.$alertInfo('请点击"计算"按钮');
          return
        }
        item.planCompleteTime = this.calcCompleteEndTime(time, this.tableEditData[this.tableData[index].id].planInterval)
      },
      /*根据首日时间及预计时长计算排产周期*/
      calcTotalPredictStartEndTime() {
        this.tableData.forEach((item, index) => {
          let startTime;
          if (index === 0) {
            startTime = Moment(this.planOptions.planStartTime).add(this.tableEditData[this.tableData[index].id].lineChangeTime, 'm').format('YYYY-MM-DD HH:mm:ss')
          } else {
            startTime = Moment(this.tableEditData[this.tableData[index - 1].id].planCompleteTime).add(this.tableEditData[this.tableData[index].id].lineChangeTime, 'm').format('YYYY-MM-DD HH:mm:ss')
          }
          this.$set(this.tableEditData[this.tableData[index].id], 'planStartTime', startTime);
          this.$set(this.tableEditData[this.tableData[index].id], 'planCompleteTime', this.calcCompleteEndTime(startTime, this.tableEditData[this.tableData[index].id].planInterval));
        })
      },
      /*计算首日开始时间距离当日下班时间时长 返回 分钟数*/
      calcStartTimeToOffWork(startTime) {
        const stObj = new Date(startTime);
        const mins = stObj.getHours() * 60 + stObj.getMinutes();
        const calcTime = (val) => {
          let group = val.split(':');
          return group[0] * 60 + parseInt(group[1])
        };
        const findKey = (val, valB, key) => {
          return val < valB ? key : undefined;
        };
        let scheduleKey = undefined;
        let s = Object.keys(this.workingSchedule);
        for (let i = 0; i < s.length; i++) {
          let key = s[i];
          if (this.workingSchedule[key]) {
            scheduleKey = findKey(mins, calcTime(this.workingSchedule[key]), key);
            if (scheduleKey || scheduleKey === 0) {
              break
            }
          }
        }
        let minsFromStart;
        switch (parseInt(scheduleKey)) {
          case 1:
            minsFromStart = mins - calcTime(this.workingSchedule[0]);
            break;
          case 3:
            minsFromStart = mins - calcTime(this.workingSchedule[2]) + this.timeIntervalMinutes[0];
            break;
          case 5:
            minsFromStart = mins - calcTime(this.workingSchedule[4]) + this.timeIntervalMinutes[0] + this.timeIntervalMinutes[1];
            break;
          case 7:
            minsFromStart = mins - calcTime(this.workingSchedule[6]) + this.timeIntervalMinutes[0] + this.timeIntervalMinutes[1] + this.timeIntervalMinutes[2];
            break;
        }
        return this.planOptions.planTimePerDay - minsFromStart;
      },
      /*根据最后一天的分钟数计算结束时间点 返回 时间字符串*/
      calcEndTime(lastDateTime) {
        let generateTime = (st, interaval) => {
          let _st = st.split(':');
          let _h = Math.floor(interaval / 60);
          let _m = interaval % 60;
          return {
            'h': (parseInt(_st[0]) + _h),
            'm': (parseInt(_st[1]) + _m)
          };
        };
        if (lastDateTime > 0) {
          if (lastDateTime < this.timeIntervalMinutes[0]) {
            return generateTime(this.workingSchedule[0], lastDateTime)
          }
          if (lastDateTime < this.timeIntervalMinutes[0] + this.timeIntervalMinutes[1]) {
            return generateTime(this.workingSchedule[2], lastDateTime - this.timeIntervalMinutes[0])
          }
          if (lastDateTime < this.timeIntervalMinutes[0] + this.timeIntervalMinutes[1] + this.timeIntervalMinutes[2]) {
            return generateTime(this.workingSchedule[4], lastDateTime - this.timeIntervalMinutes[0] - this.timeIntervalMinutes[1])
          }
          if (lastDateTime < this.timeIntervalMinutes[0] + this.timeIntervalMinutes[1] + this.timeIntervalMinutes[2] + this.timeIntervalMinutes[3]) {
            return generateTime(this.workingSchedule[6], lastDateTime - this.timeIntervalMinutes[0] - this.timeIntervalMinutes[1] - this.timeIntervalMinutes[2])
          }
        } else {
          return generateTime(this.workingSchedule[0], 0)
        }
      },
      /*根据开始时间与生产时长生成结束的完整时间
      * 开始时间  预计生产时间*/
      calcCompleteEndTime(startTime, planInterval) {
        let intervalMin = planInterval * 60;
        let firstDayInterval = this.calcStartTimeToOffWork(startTime);
        /*排除当天就能完成的情况*/
        if (firstDayInterval <= intervalMin) {
          let intervalDaysWithoutFirstEnd = Math.floor((intervalMin - firstDayInterval) / this.planOptions.planTimePerDay);
          let lastDayIntervalMin = (intervalMin - firstDayInterval) % this.planOptions.planTimePerDay;
          let etObj = this.calcEndTime(lastDayIntervalMin);
          return Moment(startTime).second(0).minute(0).hour(0).add(1, 'd').add(intervalDaysWithoutFirstEnd, 'd').set({
            'h': etObj.h,
            'm': etObj.m
          }).format('YYYY-MM-DD HH:mm:ss');
        } else {
          let etObj = this.calcEndTime(intervalMin + (this.planOptions.planTimePerDay - this.calcStartTimeToOffWork(startTime)));
          return Moment(startTime).second(0).minute(0).hour(0).set({
            'h': etObj.h,
            'm': etObj.m
          }).format('YYYY-MM-DD HH:mm:ss');
        }
      },

      /*修改转线时长*/
      changeLineChangeTime(){
        Object.values(this.tableEditData).forEach(item=>{
          if(item.lineChangeTime === ''){
            item.lineChangeTime = 0
          }
        })
        this.clearPredictTime()
      },
      /*修改条目数量*/
      changeQuantity(value, row) {
        let initUnschedul = ''
        this.tableTollData.forEach(item=>{     //为了得到 最开始的 未排产数，因为每次排产数量跟新，都需要用最开始的未排产数来减
          if (row.zhidan === item.zhidan){
            initUnschedul = item.unscheduledQuantity
          }
        })
        let scheduleSum = 0
        Object.values(this.tableEditData).forEach(item=>{
          if(row.zhidan === item.zhidan){
            scheduleSum += item.schedulingQuantity
          }
          if(item.schedulingQuantity === ''){
            item.schedulingQuantity = 0
          }
        })
        this.tableData.forEach(item=>{
          if(row.zhidan === item.zhidan) {
            item.unscheduledQuantity = initUnschedul - scheduleSum
          }
        })
        this.clearPredictTime();
      },
      /*当执行某些操作时清空现各订单的预计起始时间*/
      clearPredictTime() {
        Object.keys(this.tableEditData).forEach(key => {
          this.$set(this.tableEditData[key], 'planInterval', null);
          this.$set(this.tableEditData[key], 'planStartTime', '');
          this.$set(this.tableEditData[key], 'planCompleteTime', '');
        });
        this.isImportable = false;
      },
      /*type 0: 向上 1：向下*/
      changeOrderPos(type, index) {
        if (type === 0) {
          this.tableData.splice(index, 1, ...this.tableData.splice(index - 1, 1, this.tableData[index]))
        } else if (type === 1) {
          this.tableData.splice(index, 1, ...this.tableData.splice(index + 1, 1, this.tableData[index]))
        }
        if (this.isAuto) {
          this.clearPredictTime();
        }
      },

      insertOrder(index) {
        this.insertingPosition = index;
        this.isInserting = true;
      },
      orderInsertSelectionChange(val) {
        this.insertingOrder = JSON.parse(JSON.stringify(val))
      },
      submitInsert() {
        let item = this.insertingOrder;
        if (!item) {
          this.$alertInfo('请选择订单');
          return;
        }
        if (item.capacity === 0) {
          this.$alertInfo('所选订单产能为0，请前往产能管理模块添加信息');
          return;
        }
        if (item.unscheduledQuantity === 0) {
          this.$alertInfo('所选订单待排产数量为0');
          return;
        }
        let checkIdInGroup = function (group, id) {
          for (let i = 0; i < group.length; i++) {
            if (group[i].id === id) {
              return true
            }
          }
          return false;
        };
        let isExist = checkIdInGroup(this.tableData, item.id);
        let tempId = item.id;
        if (isExist) {
          item.id = item.id + '-' + Math.floor(Math.random() * 100000);
        }
        this.$set(this.tableEditData, item.id, {
          order: tempId,
          remark: null,
          schedulingQuantity: isExist ? 0 : item.unscheduledQuantity,
          line: item.line || this.lineGroup[0].id,
          capacity: item.capacity,
          processGroup: this.activeProcessGroup,
          planStartTime: '',
          planCompleteTime: '',
          rhythm: item.rhythm,
          personNumber: item.processPeopleQuantity,
          lineChangeTime: item.transferLineTime,
          planInterval: null,
          isUrgent: Boolean(item.isUrgent),
          zhidan:item.zhidan
        });
        this.tableData.splice(this.insertingPosition + 1, 0, item);
        this.closeInsert();
      },
      closeInsert() {
        this.insertingOrder = undefined;
        this.$refs['inserttablecomponent'].setCurrentRow();
        this.insertingPosition = null;
        this.isInserting = false;
      },
      deleteOrder(index) {
        let id = this.tableData[index].id;
        delete this.tableEditData[id];
        this.tableData.splice(index, 1)
      },

      //导入排产计划
      submitEdit() {
        let flag = false;
        Object.keys(this.tableEditData).forEach(key => {
          if (!this.tableEditData[key].planStartTime || !this.tableEditData[key].planCompleteTime) {
            flag = true
          }
        });
        if (flag) {
          this.$alertInfo('请完善信息');
          return
        }
        let trueTime = false;
        let planDataEndtime =[];
          Object.keys(this.tableEditData).forEach(item=>{
          planDataEndtime.push(this.tableEditData[item].planCompleteTime)
          if(planDataEndtime.indexOf('超出预期') !== -1){
            trueTime = false;
          }else{
            trueTime = true
          }
        })
        if(trueTime === false){
          this.$alertDanger('请设置正确排产计划')
          return;
        }
        MessageBox.confirm('请确认是否按此配置导入排产(请留意页面中可能存在的错误提示)', '提示', {
          confirmButtonText: '确认',
          cancelButtonText: '取消',
          type: 'warning'
        }).then(() => {
          this.$openLoading();
          this.isPending = true;
          let options = {
            url: this.isReImport ? planDetailsReAddUrl : planDetailsAddUrl,
            data: {
              settings: null
            }
          };
          let settingArray = Object.keys(this.tableEditData).map(key => {
            return this.tableEditData[key]
          });
          options.data.settings = JSON.stringify(settingArray);
          axiosFetch(options).then(response => {
            if (response.data.result === 200) {
              this.$alertSuccess("操作成功");
              this.cancelEdit();
              this.$emit('reload')
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.$alertDanger('未知错误')
          }).finally(() => {
            this.isPending = false;
            this.$closeLoading();
          });
        }).catch(err=>{
        })
      },
      setHighlight({row, index}) {
        /*时间异常高亮*/
        let rowCompleteTime = this.tableEditData[row.id].planCompleteTime;
        let totalPlanCompleteTime = Moment(this.planOptions.planStartTime).add(this.planOptions.planDays, 'd');
        /*数量异常高亮
        row.unscheduledQuantity < this.currentOrderQuantity[row.id]
        */
        if(this.tableEditData[row.id].planCompleteTime === "超出预期" || this.tableEditData[row.id].planStartTime === "超出预期"){
          return 'timeout-highlight'                   //字变色 (预计时间异常)
          // return 'quantity-out-highlight'           //行边框变色 (排产数量超额)
          // return 'quantity-out-timeout-highlight'   //行边框+文字都变色 (预计时间异常,排产数量异常)
        }
        if(this.tableEditData[row.id].schedulingQuantity > row.quantity){
          return 'quantity-out-highlight'           //行边框变色 (排产数量超额)
        }
        if(this.tableEditData[row.id].planCompleteTime === "超出预期" || this.tableEditData[row.id].planStartTime === "超出预期" && this.tableEditData[row.id].schedulingQuantity > row.quantity){
          return 'quantity-out-timeout-highlight'   //行边框+文字都变色 (预计时间异常,排产数量异常)
        }
        // let isReImportRow = Boolean(this.tableEditData[row.id].id !== undefined);
        // if (!!rowCompleteTime && Moment(rowCompleteTime).isAfter(totalPlanCompleteTime) && row.unscheduledQuantity < this.currentOrderQuantity[row.id] && !isReImportRow) {
        //   return 'quantity-out-timeout-highlight'
        // }
        // if (!!rowCompleteTime && Moment(rowCompleteTime).isAfter(totalPlanCompleteTime)) {
        //   return 'timeout-highlight'
        // }
        //
        // let id = this.tableEditData[row.id].order;
        // if (row.unscheduledQuantity < this.currentOrderQuantity[id] && !isReImportRow) {
        //   return 'quantity-out-highlight'
        // }
      },
      setExistHighlight({row, index}) {
        if (this.existOrders.indexOf(row.id) >= 0) {
          return 'exist-highlight'
        }
      },
      clearPlanStartTime () {
        this.planOptions.planStartTime = '';
        this.$alertInfo('请选择在排班时间内的日期')
      },
      clearDataItemStartTime() {
        this.tableEditData[this.tableData[index]].planStartTime = '';
        this.$alertInfo('请选择在排班时间内的日期')
      },
      clearDataItemEndTime(index) {
        this.tableEditData[this.tableData[index]].planCompleteTime = '';
        this.$alertInfo('请选择在排班时间内的日期')
      }
    }
  }
</script>

<style scoped>
  .dialog-container {

  }

  .order-setting {
    display: flex;
    flex-wrap: wrap;
    align-items: flex-end;
  }

  .order-setting-comp {
    margin-right: 10px;
  }

  .order-setting-comp .el-input {
    max-width: 180px;
  }

  .order-setting-comp.__text, .order-setting-comp .el-select {
    width: 128px;
  }

  .order-setting-comp.__btn {
    width: 140px;
    margin-bottom: 10px;
  }

  .order-setting-comp.__swicth {
    margin-bottom: 18px;
  }

  .order-setting-comp /deep/ .el-form-item__label {
    font-size: 12px;
    line-height: 18px;
    height: 18px;
    color: #5d6066;
  }

  /*表格部分*/
  .order-list-setting {
    font-size: 12px;
    max-height: 550px;
  }

  .list-setting-title {
    position: relative;
    display: flex;
    font-weight: bold;
    height: 100%;
  }

  .list-setting-title .el-col {
    border: 1px solid #999999;
    height: 100%;
    flex: 2 1 auto;
  }

  .table-column-date {
    width: 180px;
  }

  .el-table /deep/ .timeout-highlight {
    color: #ff6953;
    font-weight: bold;
  }

  .el-table /deep/ .exist-highlight {
    box-shadow: inset 0 0 20px 8px #e7ff86;
    font-weight: bold;
  }

  .el-table /deep/ .quantity-out-highlight {
    box-shadow: inset 0 0 11px 0 #ff6953;
  }

  .el-table /deep/ .quantity-out-timeout-highlight {
    box-shadow: inset 0 0 11px 0 #ff6953;
    color: #ff6953;
    font-weight: bold;
  }

  .insert-tips, .import-order-tips {
    margin:0 20px ;
    /*height: 24px;*/
    /*font-size: 12px;*/
    display: flex;
    align-items: center;
    font-weight: bold;
    cursor: default;
  }

  .insert-tips-container, .import-tips-container {
    width: 40px;
    height: 20px;
    border: 1px solid #e6e6e6;
    border-radius: 5px;
    margin-right: 4px;

    display: flex;
    justify-content: center;
    align-items: center;
  }

  .insert-tips-container.exist-highlight {
    box-shadow: inset 0 0 5px 3px #e7ff86;
  }

  .import-tips-container.timeout {
    color: #ff6953;
    font-weight: bold;
  }

  .import-tips-container.quantity-out {
    box-shadow: inset 0 0 5px 0 #ff6953;
    margin-left: 10px;
  }
  .setting-time{
    width: 40px;
    height: auto;
    margin-top: 10px;
  }
  .time-setting-tip{
    width: 80%;
    display: flex;
    justify-content: space-between;
  }

  .import-working-schedule {
    border-radius: 5px;
    width: 300px;
    height: 100px;
    border: 1px solid #e6e6e6;
    margin-bottom: -60px;
    margin-top: 10px;
    font-size: 12px;
    display: flex;
    flex-direction: column;
    flex-wrap: wrap;
    box-sizing: border-box;
    justify-content: center;
    align-items: center;
    position: relative;
    padding-top: 20px;
    color: #7c8088;
    box-shadow: 0 0 8px 0 #eaeaea;
  }

  .import-working-schedule .schedule-title {
    font-size: 14px;
    font-weight: bold;
    position: absolute;
    top: 2px;
    left: 5px;
  }

  .import-working-schedule .schedule-item {
    line-height: 18px;
    width: 140px;
  }

  .allreday-choice-date{
    min-height: 30px;
    max-width: 800px;
    margin-top: 20px;
  }
  .allreday-choice-date /deep/ .el-tag{
    margin-top: 5px;
    margin-right: 10px;
  }
  .allreday-choice-date /deep/ .el-tag:hover{
    cursor: pointer;
  }

</style>

