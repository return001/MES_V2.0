<template>
  <el-dialog
      title="预排产"
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
        <el-form-item
            size="mini"
            label="排产预计开始时间"
            prop="planStartTime"
            class="order-setting-comp">
          <el-date-picker
              v-model="planOptions.planStartTime"
              type="datetime"
              value-format="yyyy-MM-dd HH:mm:ss"
              default-time="08:00:00"
              autocomplete="off"
              size="mini"
              prefix-icon="el-icon-date" :disabled="!isAuto"
          >
          </el-date-picker>
        </el-form-item>
        <el-form-item
            size="mini"
            label="预排产天数"
            prop="planDays"
            class="order-setting-comp __text">
          <el-input
              type="text"
              v-model="planOptions.planDays"
              autocomplete="off"
          ></el-input>
        </el-form-item>
        <el-form-item
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
        <!--<el-form-item
            size="mini"
            label="工作间休息时长(小时)"
            prop="restTime"
            class="order-setting-comp __text">
          <el-input
              type="text"
              v-model="planOptions.restTime"
              autocomplete="off"
          ></el-input>
        </el-form-item>-->
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
        </el-form-item>
        <el-switch
            size="mini"
            v-model="isAuto"
            active-text="自动排产"
            inactive-text="手动排产"
            active-color="#409EFF"
            inactive-color="#6CFF87"
            class="order-setting-comp __swicth"></el-switch>
        <div class="order-setting-comp __btn">
          <el-button size="mini" type="info" @click="initPlanOptions">重置</el-button>
          <el-button size="mini" type="primary" @click="calcOptions">计算</el-button>
        </div>
      </el-form>
      <div class="order-list-setting">
        <el-table
            :data="tableData"
            max-height="560"
            ref="list-table-component"
            :row-class-name="setTimeoutHighlight"
            size="mini"
            stripe>
          <el-table-column
              type="index"
              fixed="left"
              width="40">
          </el-table-column>
          <el-table-column
              label="产线"
              width="120"
          >
            <template slot-scope="scope">
              <el-select
                  :disabled="!isOptionsEditable"
                  v-model="tableEditData[scope.row.id].line"
                  placeholder="请选择产线"
                  size="mini">
                <el-option v-for="listItem in lineGroup"
                           :key="listItem.id"
                           :value="listItem.id"
                           :label="listItem.lineName"></el-option>
              </el-select>
            </template>
          </el-table-column>
          <el-table-column
              label="是否紧急"
              width="50"
          >
            <template slot-scope="scope">
              <el-checkbox
                  :disabled="!isOptionsEditable"
                  v-model="tableEditData[scope.row.id].isUrgent"></el-checkbox>
            </template>
          </el-table-column>
          <el-table-column
              label="转线时长(小时)"
              width="80"
          >
            <template slot-scope="scope">
              <el-input class="table-inner-input-text"
                        type="text"
                        size="mini"
                        :disabled="!isOptionsEditable"
                        @change="clearPredictTime"
                        v-model="tableEditData[scope.row.id].lineChangeTime"></el-input>
            </template>
          </el-table-column>
          <el-table-column
              label="排产数量"
              width="80"
          >
            <template slot-scope="scope">
              <el-input class="table-inner-input-text"
                        type="text"
                        size="mini"
                        :disabled="!isOptionsEditable"
                        @change="clearPredictTime"
                        v-model="tableEditData[scope.row.id].schedulingQuantity"></el-input>
            </template>
          </el-table-column>
          <el-table-column v-for="(item, index) in tableColumns"
                           :key="index"
                           :prop="item.key"
                           :label="item.label"
                           :min-width="item['min-width']">
          </el-table-column>

          <el-table-column
              label="预计生产时长(小时)"
              width="80"
              fixed="right"
          >
            <template slot-scope="scope">
              {{tableEditData[scope.row.id].planInterval}}
            </template>
          </el-table-column>
          <el-table-column
              label="预计开始时间"
              width="200"
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
                  :disabled="!isOptionsEditable"
                  @change="manualCalcCompleteTime(scope.$index)"
              >
              </el-date-picker>
              <span v-else>{{tableEditData[scope.row.id].planStartTime}}</span>

            </template>
          </el-table-column>
          <el-table-column
              label="预计结束时间"
              width="160"
              fixed="right"
          >
            <template slot-scope="scope">
              <span>{{tableEditData[scope.row.id].planCompleteTime}}</span>
            </template>
          </el-table-column>
          <el-table-column
              label="操作"
              width="140"
              fixed="right"
          >
            <template slot-scope="scope">
              <!--<el-tooltip content="插入" placement="top">
                <el-button type="text" icon="el-icon-plus"></el-button>
              </el-tooltip>-->
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
    <span slot="footer" class="dialog-footer">
          <el-button size="small" type="success" @click="calcOptions">解析</el-button>
          <el-button size="small" type="info" @click="cancelEdit">取消</el-button>
          <el-button size="small" type="primary" :disabled="!isImportable" @click="submitEdit">导入</el-button>
      </span>

  </el-dialog>
</template>

<script>
  import Moment from 'moment'
  import {planDetailsAddUrl} from "../../../../config/globalUrl";
  import {axiosFetch} from "../../../../utils/fetchData";

  export default {
    name: "EditImportingOrderComp",
    props: ['isOrderImportingSetting', 'importingOrders', 'activeProcessGroup','lineGroup'],
    data() {
      return {
        planOptions: {
          planStartTime: '',
          planDays: null,
          planTimePerDay: null,
          // restTime: null,
          startTimePerDay: ''
        },
        planOptionsRules: {
          'planStartTime': [
            {required: true, message: '请输入预计开始时间', trigger: 'blur'}
          ],
          'planDays': [
            {required: true, message: '请输入预排产天数', trigger: 'blur'}
          ],
          'planTimePerDay': [
            {required: true, message: '请输入一天可排产时长', trigger: 'blur'}
          ],
          /*'restTime': [
            {required: true, message: '请输入工作间休息时长', trigger: 'blur'}
          ],*/
          'startTimePerDay': [
            {required: true, message: '请输入生产开始时间', trigger: 'blur'}
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
          {'label': '客户料号', 'key': 'customerMaterialNo', 'min-width': '100px'},
          {'label': '客户编号', 'key': 'customerNumber', 'min-width': '100px'},
          {'label': '客户名称', 'key': 'customerName', 'min-width': '100px'},
          {'label': '订单日期', 'key': 'orderDate', 'min-width': '100px'},
          {'label': '交货日期', 'key': 'deliveryDate', 'min-width': '100px'},
        ],
        tableData: [],
        tableEditData: {},
        isOptionsEditable: false,
        isAuto: true,
        planStartTime: null,
        isImportable: false,
        isPending: false,
      }
    },
    watch: {
      'importingOrders': function (val) {
        if (val) {
          this.tableData = val.map(item => {
            this.$set(this.tableEditData, item.id, {
              order: item.id,
              remark: null,
              schedulingQuantity: item.unscheduledQuantity,
              line: null,
              capacity: item.capacity,
              processGroup: this.activeProcessGroup,
              planStartTime: '',
              planCompleteTime: '',
              rhythm: item.rhythm,
              personNumber: item.processPeopleQuantity,
              lineChangeTime: item.transferLineTime,
              planInterval: null,
              isUrgent: Boolean(item.isUrgent)
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
      }
    },
    computed: {},
    mounted() {
      this.tableData = this.importingOrders.map(item => {
        this.$set(this.tableEditData, item.id, {
          order: item.id,
          remark: null,
          schedulingQuantity: item.unscheduledQuantity,
          line: null,
          capacity: item.capacity,
          processGroup: this.activeProcessGroup,
          planStartTime: '',
          planCompleteTime: '',
          rhythm: item.rhythm,
          personNumber: item.processPeopleQuantity,
          lineChangeTime: item.transferLineTime,
          planInterval: null,
          isUrgent: Boolean(item.isUrgent)
        });
        return item
      })
    },

    methods: {
      initAll() {
        Object.assign(this.$data, this.$options.data());
      },
      initPlanOptions() {
        Object.assign(this.$data.planOptions, this.$options.data().planOptions)
      },
      cancelEdit() {
        this.$emit('update:isOrderImportingSetting', false);
        this.initAll();
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

            this.calcPredictTime();

            if (this.isAuto) {
              let startTime = Moment(this.planOptions.planStartTime).add(this.tableEditData[this.tableData[0].id].lineChangeTime, 'h').format('YYYY-MM-DD HH:mm:ss');
              this.$set(this.tableEditData[this.tableData[0].id], 'planStartTime', startTime);
              this.planStartTime = startTime;
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
      manualCalcCompleteTime(index) {
        let item = this.tableEditData[this.tableData[index].id];
        if (!item.planInterval) {
          this.$set(item, 'planStartTime', '');
          this.$alertInfo('请点击"计算"按钮');
          return
        }

        item.planCompleteTime = Moment(item.planStartTime).add(Math.floor(item.planInterval / this.planOptions.planTimePerDay) * 24 + (item.planInterval % this.planOptions.planTimePerDay), 'h').format('YYYY-MM-DD HH:mm:ss')
      },

      /*根据首日时间及预计时长计算排产周期*/
      calcTotalPredictStartEndTime() {
        let firstItemId = this.tableData[0].id;
        let e = this.tableEditData;
        let o = this.planOptions;
        if (e[firstItemId].planStartTime) {
          this.tableData.forEach((item, index) => {
            let timeAdd = Math.floor(e[item.id].planInterval / o.planTimePerDay) * 24 + (e[item.id].planInterval % o.planTimePerDay);
            if (index === 0) {
              e[item.id].planCompleteTime = Moment(e[item.id].planStartTime).add(timeAdd, 'h').format('YYYY-MM-DD HH:mm:ss')
            } else {
              e[item.id].planStartTime = Moment(e[this.tableData[index - 1].id].planCompleteTime).add(e[item.id].lineChangeTime, 'h').format('YYYY-MM-DD HH:mm:ss');
              e[item.id].planCompleteTime = Moment(e[item.id].planStartTime).add(timeAdd, 'h').format('YYYY-MM-DD HH:mm:ss')
            }
          })
        }
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
        this.clearPredictTime();
      },

      deleteOrder(index) {
        let id = this.tableData[index].id;
        delete this.tableEditData[id];
        this.tableData.splice(index, 1)
      },

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
        if (!this.isPending) {
          this.$openLoading();
          this.isPending = true;
          let options = {
            url: planDetailsAddUrl,
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
        }
      },


      setTimeoutHighlight({row, index}) {
        let rowCompleteTime = this.tableEditData[row.id].planCompleteTime;
        let totalPlanCompleteTime = Moment(this.planOptions.planStartTime).add(this.planOptions.planDays, 'd');
        if (!!rowCompleteTime && Moment(rowCompleteTime).isAfter(totalPlanCompleteTime)) {
          return 'timeout-highlight'
        }
      },


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

  .order-setting-comp.__text {
    width: 128px;
  }

  .order-setting-comp.__btn {
    width: 140px;
    margin-bottom: 18px;
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
</style>