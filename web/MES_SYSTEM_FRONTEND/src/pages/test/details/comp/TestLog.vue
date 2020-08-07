<template>
  <div id="test-log">
    <div class="test-log-main">
      <div class="query-comp">
        <div class="query-comp-container" v-for="(item,index) in queryOptions" :key="index">
          <div class="query-comp-text" v-if="item.type === 'text'">
            <label :for="item.key + index">{{item.label}}</label>
            <el-input v-model.trim="thisQueryOptions[item.key]"
                      :id="item.key + index"
                      :placeholder="'请填写' + item.label"
                      size="small" autocomplete="false" clearable>
            </el-input>
          </div>

          <div class="query-comp-select" v-if="item.type === 'select'">
            <label :for="item.key + index">{{item.label}}</label>
            <el-select v-model="thisQueryOptions[item.key]" :id="item.key + index"
                       :placeholder="'请选择' + item.label"
                       autocomplete="false" size="small" clearable>
              <el-option v-for="(listItem,itemIndex) in item.list"
                         :key="itemIndex.value"
                         :value="listItem.value"
                         :label="listItem.label">
              </el-option>
            </el-select>
          </div>

          <div class="query-comp-timerange" v-if="item.type === 'timeRange'">
            <label :for="item.key + index">{{item.label}}</label>
            <el-date-picker
              :id="item.key + index"
              v-model="times"
              type="datetimerange"
              :picker-options="timePickerOptions"
              prefix-icon="el-icon-date"
              range-separator="至"
              start-placeholder="开始日期"
              end-placeholder="结束日期"
              value-format="yyyy-MM-dd HH:mm:ss"
              autocomplete="off"
              size="small"
              clearable>
            </el-date-picker>
          </div>
        </div>
        <div class="query-comp-container">
          <el-button type="info" size="small" @click="clearOptions">重置条件</el-button>
          <el-button type="primary" size="small" @click="fetchData">查询</el-button>
        </div>
      </div>

      <div class="content-comp" >
        <el-table
          max-height="650"
          :data="tableData"
          :header-cell-style="{'text-align':'center'}"
          :cell-style="{'text-align':'center'}"
          style="width:100%"
          stripe
        >
          <el-table-column
            label="序号"
            type="index"
            header-align="center"
            width="80"
            fixed="left">
          </el-table-column>
          <el-table-column v-for="(item,index) in tableColumns"
                           :key="index"
                           :label="item.label"
                           :prop="item.key"
                           :min-width="item['min-width']">
          </el-table-column>

          <el-table-column
            label="详情"
            min-width="70"
            header-align="center"
            fixed="right">
            <template slot-scope="scope">
              <el-tooltip content='查看详情' placement="top">
                <el-button type="text" icon="el-icon-t-table" @click="logDetail(scope.row)"></el-button>
              </el-tooltip>
            </template>

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
          @current-change="thisFetch()"
          @size-change="thisFetch('sizeChange')"
          class="page-pagination">
        </el-pagination>
      </div>

      <log-detail-panel v-if="isDetail === true"
                        :detail-data="detailData"
                        :is-detail.sync="isDetail"/>
    </div>
  </div>
</template>

<script>
  import {
    testLogGetUrl,
    testOperUrl,
    testSelectUrl,

    testLogQueryOptions,
    testLogTableOptions,
  } from "../../../../config/testApiConfig";
  import {axiosFetch} from "../../../../utils/fetchData";
  import LogDetailPanel from "./LogDetailPanel";

    export default {
      name: "TestLog",
      components:{
        LogDetailPanel
      },
      data(){
        return {
          isPending: false,
          queryOptions:testLogQueryOptions,
          tableColumns:testLogTableOptions,
          times:[],
          tableData:[
            // {
            //   softVersion: "PB88-V_11_S2_SC2",
            //   orderName:'JIMI-1020-2010',
            //   settingType:"组装功能测试",
            //   typeId:2,
            //   model: "PB76-V",
            //   operationType: "新增",
            //   operator: "张三",
            //   time:'2020-05-06 20:31:00',
            //   result:'成功',
            //   station: 'S340}}17功能测试软件版本@@[VERSION]@@@@AT^GT_CM=VERSION}}功能测试芯片ID@@CHIPID:@@@@AT^GT_CM=CHIPID}}功能测试读IMEI@@OK,@@@@AT^GT_CM=IMEI,READ}}功能测试VIBRATOR人工判断DL10000@@OK@@@@AT^GT_CM=VIBRATOR}}功能测试校准查询DL2000@@RF_BAR:OK@@@@AT^GT_CM=RF_BAR}}功能测试电压测试@@Charging,@@3.8,5@@AT^GT_CM=BMT}}功能测试BT测试DL8000@@BT:BT TESTING!Wait 20s to get bluetooth info...@@@@AT^GT_CM=BT}}功能测试震动测试@@GSENSOR:GSENSOR OK!@@@@AT^GT_CM=GSENSOR}}功能测试LED人工判断测试DL8000@@LED:OK@@@@AT^GT_CM=LED}}功能测试WIFI测试DL10000@@WIFITC:OK!Wait 5s to get wifi info...@@@@AT^GT_CM=WIFITC}}功能测试CSQ测试@@+CESQ:@@5,100@@AT+CESQ}}功能测试按键测试DL10000@@KEY@@4@@AT^GT_CM=KEY}}功能测试sim卡测试@@OK@@@@AT^GT_CM=SIM}}功能测试喇叭人工判断DL10000@@OK@@@@AT^GT_CM=SPEAKER}}功能测试MIC人工判断DL10000@@MIC:The machine begins to record and then it will be played!@@@@AT^GT_CM=MIC}}功能测试拨号人工判断DL20000@@TESTING@@@@AT^GT_CM=CALLTC}}功能测试GPS测试DL25000@@GPSTC:OK@@@@AT^GT_CM=GPSTC}}'
            // },
            // {
            //   softVersion: "PB88-V_11_S2_SC33",
            //   orderName:'JIMI-2020-2010',
            //   settingType:"SMT功能测试",
            //   typeId:3,
            //   model: "PB88-V",
            //   operationType: "新增",
            //   operator: "李四",
            //   time:'2020-05-06 20:32:00',
            //   result:'失败',
            //   station: 'NT912_R0_V01@@@@}}}}11共有指令切换为NB@@AT+MSMODE=2,0@@OK}}共有指令芯片ID@@AT^GT_CM=ID,1@@RID}}共有指令软件版本@@AT^GT_CM=VERSION@@[VERSION]}}IMEI域名IMEI写号@@AT^GT_CM=IMEI,1,@@OK!}}IMEI域名测试项@@AT^GT_CM=ESDS@@NVRAM执行成功！}}IMEI域名RFID@@AT^GT_CM=RF_BAR@@PASS}}IMEI域名IMEI检查@@AT+EGMR=0,7@@OK}}IMEI域名恢复出厂@@AT^GT_CM=FACTORYALL@@OK}}IMEI域名查IP及端口@@AT^GT_CM=SCXSZ@@112.35.78.50,7100}}共有指令清除参数@@AT^GT_CM=CLEAR@@OK}}共有指令耦合测试@@@@}}'
            // }
          ],
          thisQueryOptions:{},  //查询条件存储
          isDetail:false,
          detailData:[],
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

          paginationOptions: {
            currentPage: 1,
            pageSize: 20,
            total: 0
          },
        }
      },

      created(){
        this.fetchData();

      },
      methods:{
        clearOptions(){
          this.thisQueryOptions = {};
        },

        fetchData(){
          if(!this.isPending){
            this.isPending = true
            this.$nextTick(()=>{
              this.$openLoading();
            })

            let options = {
              url:testLogGetUrl,
              data:{
                pageNo:this.paginationOptions.currentPage,
                pageSize:this.paginationOptions.pageSize,
              }
            }
            if(this.times.length > 0){
              this.thisQueryOptions.startTime = this.times[0]
              this.thisQueryOptions.endTime = this.times[1]
            }
            Object.keys(this.thisQueryOptions).forEach(item=>{
              options.data[item] = JSON.parse(JSON.stringify(this.thisQueryOptions[item]))
            })
            axiosFetch(options).then(response=>{
              if(response.data.result === 200 ){
                response.data.data.list.forEach(item=>{
                  item.model = item.model.split("@@")[0]
                  switch (item.settingType) {
                    case 0:
                      item.settingTypeName = 'SMT功能测试'
                      break
                    case 1:
                      item.settingTypeName = '组装功能测试'
                      break
                    case 2:
                      item.settingTypeName = '组装耦合测试'
                      break
                    case 3:
                      item.settingTypeName = '研发功能测试'
                      break
                    case 4:
                      item.settingTypeName = '研发耦合测试'
                      break
                    case 5:
                      item.settingTypeName = 'OQC'
                      break
                  }
                })
                this.tableData = response.data.data.list
                this.paginationOptions.currentPage = response.data.data['pageNumber'];
                this.paginationOptions.total = response.data.data['totalRow'];
              }else{
                this.$alertWarning(response.data)
              }
            }).catch(err=>{
              console.log(err)
            }).finally(()=>{
              this.$closeLoading()
              this.isPending = false
              }
            )
          }
        },

        thisFetch: function (opt) {
          this.$openLoading();
          if (opt === 'sizeChange') {
            this.paginationOptions.currentPage = 1;
          }

          let options = {
            url: testSelectUrl,
            data: {
              pageNo: this.paginationOptions.currentPage,
              pageSize: this.paginationOptions.pageSize,
              descBy: 'RecordTime',
              type: this.$route.query.type
            }
          };
          this.fetchData(options)
        },

        logDetail(row){
          console.log(row)
          this.isDetail = true;
          this.detailData = row
        }
      }
    }
</script>

<style scoped>
  #test-logs /deep/ .el-button i{
    font-size: 17px;
    font-weight: bold;
  }

  #test-log {
    position: relative;
    width: 100%;
    min-height: 600px;
    height: 100%;
  }

  .test-log-main {
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
