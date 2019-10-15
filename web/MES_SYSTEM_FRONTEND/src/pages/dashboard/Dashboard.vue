<template>
  <el-carousel indicator-position="none" height="100vh" :autoplay="false" ref="lineCarousel">
    <el-carousel-item>
      <div class="dashboard">
        <div class="dashboard-title">
          <span>产品组装状态看板</span>
        </div>
        <div class="dashboard-table">
          <el-table
            :data="tableData0"
            max-height="480"
            ref="tablecomponent0"
            :span-method="detailsTableSpanMethod0">
            <el-table-column v-for="(subItem, index) in tableColumns"
                             :key="'0' + index.toString()"
                             :prop="subItem.key"
                             :label="subItem.label"
                             :min-width="subItem.width"
                             :formatter="subItem.formatter">
            </el-table-column>

          </el-table>
        </div>
        <div class="dashboard-component-container">
          <div class="dashboard-component" style="width: 25%">
            <div class="dashboard-msg-title">{{errMsgTitle0}}</div>
            <div class="dashboard-msg-item">
              <div class="dashboard-msg-subtitle">不良原因</div>
              <div class="dashboard-msg-subtitle">不良数量</div>
            </div>
            <div class="dashboard-msg-content">
              <div class="dashboard-msg-item" v-for="item in chartData0.rows">
                <div class="dashboard-msg-subitem">{{item.name}}</div>
                <div class="dashboard-msg-subitem">{{item.count}}</div>
              </div>
            </div>
          </div>
          <div class="dashboard-component dashboard-component-chart" style="width: 55%">
            <ve-pie :data="chartData0" judge-width :settings="chartSetting"></ve-pie>
          </div>
          <div class="dashboard-component dashboard-component-info" style="width: 30%">
            <div class="info-item">
              <span class="info-item-title">生产线别: </span>
              <span class="info-item-content">组装</span>
            </div>
            <div class="info-item">
              <span class="info-item-title">生产: 刘业武</span>
              <span class="info-item-content"></span>
            </div>
            <div class="info-item">
              <span class="info-item-title">工程: 何斌</span>
              <span class="info-item-content"></span>
            </div>
            <div class="info-item">
              <span class="info-item-title">品质: 王权兴</span>
              <span class="info-item-content"></span>
            </div>
          </div>

        </div>
      </div>
    </el-carousel-item>
    <el-carousel-item>
      <div class="dashboard">
        <div class="dashboard-title">
          <span>产品测试状态看板</span>
        </div>
        <div class="dashboard-table">
          <el-table
            :data="tableData1"
            max-height="480"
            ref="tablecomponent1"
            :span-method="detailsTableSpanMethod1">
            <el-table-column v-for="(subItem, index) in tableColumns"
                             :key="'1' + index.toString()"
                             :prop="subItem.key"
                             :label="subItem.label"
                             :min-width="subItem.width"
                             :formatter="subItem.formatter">
            </el-table-column>

          </el-table>
        </div>
        <div class="dashboard-component-container">
          <div class="dashboard-component" style="width: 25%">
            <div class="dashboard-msg-title">{{errMsgTitle1}}</div>
            <div class="dashboard-msg-item">
              <div class="dashboard-msg-subtitle">不良原因</div>
              <div class="dashboard-msg-subtitle">不良数量</div>
            </div>
            <div class="dashboard-msg-content">
              <div class="dashboard-msg-item" v-for="item in chartData1.rows">
                <div class="dashboard-msg-subitem">{{item.name}}</div>
                <div class="dashboard-msg-subitem">{{item.count}}</div>
              </div>
            </div>
          </div>
          <div class="dashboard-component dashboard-component-chart" style="width: 55%">
            <ve-pie :data="chartData1" judge-width :settings="chartSetting"></ve-pie>
          </div>
          <div class="dashboard-component dashboard-component-info" style="width: 30%">
            <div class="info-item">
              <span class="info-item-title">生产线别: </span>
              <span class="info-item-content">测试</span>
            </div>
            <div class="info-item">
              <span class="info-item-title">生产: 谢仁露</span>
              <span class="info-item-content"></span>
            </div>
            <div class="info-item">
              <span class="info-item-title">工程: 任继鹏</span>
              <span class="info-item-content"></span>
            </div>
            <div class="info-item">
              <span class="info-item-title">品质: 王晓美</span>
              <span class="info-item-content"></span>
            </div>
          </div>

        </div>
      </div>
    </el-carousel-item>
    <el-carousel-item>
      <div class="dashboard">
        <div class="dashboard-title">
          <span>产品包装状态看板</span>
        </div>
        <div class="dashboard-table">
          <el-table
            :data="tableData2"
            max-height="480"
            ref="tablecomponent2"
            :span-method="detailsTableSpanMethod2">
            <el-table-column v-for="(subItem, index) in tableColumns"
                             :key="'2' + index.toString()"
                             :prop="subItem.key"
                             :label="subItem.label"
                             :min-width="subItem.width"
                             :formatter="subItem.formatter">
            </el-table-column>

          </el-table>
        </div>
        <div class="dashboard-component-container">
          <div class="dashboard-component" style="width: 25%">
            <div class="dashboard-msg-title">{{errMsgTitle2}}</div>
            <div class="dashboard-msg-item">
              <div class="dashboard-msg-subtitle">不良原因</div>
              <div class="dashboard-msg-subtitle">不良数量</div>
            </div>
            <div class="dashboard-msg-content">
              <div class="dashboard-msg-item" v-for="item in chartData2.rows">
                <div class="dashboard-msg-subitem">{{item.name}}</div>
                <div class="dashboard-msg-subitem">{{item.count}}</div>
              </div>
            </div>
          </div>
          <div class="dashboard-component dashboard-component-chart" style="width: 55%">
            <ve-pie :data="chartData2" judge-width :settings="chartSetting"></ve-pie>
          </div>
          <div class="dashboard-component dashboard-component-info" style="width: 30%">
            <div class="info-item">
              <span class="info-item-title">生产线别: </span>
              <span class="info-item-content">包装</span>
            </div>
            <div class="info-item">
              <span class="info-item-title">生产: 穆小燕</span>
              <span class="info-item-content"></span>
            </div>
            <div class="info-item">
              <span class="info-item-title">工程: 何斌</span>
              <span class="info-item-content"></span>
            </div>
            <div class="info-item">
              <span class="info-item-title">品质: 王淘路</span>
              <span class="info-item-content"></span>
            </div>
          </div>
        </div>
      </div>
    </el-carousel-item>
  </el-carousel>
</template>

<script>
  import {axiosFetch} from "../../utils/fetchData";
  import {
    dashboardSelectUrl,
    dashboardErrMsgSelectUrl,
    dashboardErrMsgSelectUrlCS,
    dashboardSelectUrlCS
  } from "../../config/globalUrl";
  import VePie from 'v-charts/lib/pie.common'
  import {getTime} from "../../utils/utils";

  export default {
    name: "Dashboard",
    components: {
      VePie
    },
    data() {
      return {
        tableColumns: [
          {
            key: 'time',
            label: '时间',
            width: '130px'
          },
          {
            key: 'softModel',
            label: '产品名称',
            width: '110px'
          },
          {
            key: 'zhidan',
            label: '工单号',
            width: '110px'
          },
          {
            key: 'planProduction',
            label: '标准产出'
          },
          {
            key: 'actualProduction',
            label: '实际产出'
          },
          {
            key: 'completionRate',
            label: '生产达成率'
          },
          {
            key: 'testingRate',
            label: '测试直通率'
          },
          {
            key: 'remark',
            label: '备注'
          },
        ],
        tableData0: [],
        tableData1: [],
        tableData2: [],
        errMsg0: [],
        errMsg1: [],
        errMsg2: [],
        errMsgTitle0: '',
        errMsgTitle1: '',
        errMsgTitle2: '',

        mainScrollingInterval: null,
        isPending: false,


        mergeData0: {},//合并行的记录
        mergePos0: {},//mergeData中每项的索引
        mergeData1: {},
        mergePos1: {},
        mergeData2: {},
        mergePos2: {},
        mergeProp: ['time'],
        mergeKeys: ['time'],

        currentLine: 0,

        //v-chart setting
        chartData0: {
          columns: ["name", "count"],
          rows: []
        },
        chartData1: {
          columns: ["name", "count"],
          rows: []
        },
        chartData2: {
          columns: ["name", "count"],
          rows: []
        },
        chartSetting: {
          radius: 86,
          offsetY: 150,
          label: {
            show: true,
            formatter: '{b} : {c} ({d}%)'
          }
        }
      }
    },
    mounted() {
      this.$nextTick(() => {
        this.fetchDashboardData();

        let loopSetErrMsg = () => {
          this.fetchErrMsg(0);
          this.fetchErrMsg(1);
          this.fetchErrMsg(2);
        };

        loopSetErrMsg();
        setInterval(() => {
          loopSetErrMsg()
        }, 1000 * 10 * 60);

        setInterval(() => {
          this.fetchLineData(0);
          this.fetchLineData(1);
          this.fetchLineData(2);
        }, 1000 * 15 * 60)

      });
    },
    methods: {
      setScrollingInterval() {
        setTimeout(() => {
          this.mainScrollingInterval = setInterval(() => {
            if (this.$refs['tablecomponent' + this.currentLine].bodyWrapper.scrollTop < (this.$refs['tablecomponent' + this.currentLine].bodyWrapper.scrollHeight - this.$refs['tablecomponent' + this.currentLine].bodyWrapper.clientHeight)) {
              this.$refs['tablecomponent' + this.currentLine].bodyWrapper.scrollTop += 1
            } else {
              clearInterval(this.mainScrollingInterval);
              setTimeout(() => {
                this.currentLine === 2 ? this.currentLine = 0 : this.currentLine += 1; //循环线
                this.$refs.lineCarousel.next(); //切换轮播
                this.currentLine === 0 ? this.$refs.tablecomponent2.bodyWrapper.scrollTop = 0 : this.$refs['tablecomponent' + (this.currentLine - 1)].bodyWrapper.scrollTop = 0; //重置前一个的滚动
                this.setScrollingInterval();
              }, 5000);
            }
          }, 50);
        }, 5000);
      },


      fetchLineData(line) {
        return new Promise((resolve, reject) => {
          this['mergeData' + line] = {};
          this['mergePos' + line] = {};
          axiosFetch({
            url: dashboardSelectUrlCS,
            data: {
              line: line
            }
          }).then(response => {
            if (response.data.result === 200) {
              this.getSpanArr(response.data.data, this.mergeKeys, line);
              this.$set(this.$data, 'tableData' + line, response.data.data);
              //this['tableData' + line] = response.data.data;
            } else {
              console.log(response.data.data)
            }
          }).catch(err => {
            console.log(err)
          }).finally(() => {
            resolve()
          })

        })
      },

      async fetchDashboardData() {
        // await this.fetchLineData(0);
        await this.preloadAll();
        this.setScrollingInterval();
      },

      fetchErrMsg(line) {
        axiosFetch({
          url: dashboardErrMsgSelectUrlCS,
          data: {
            line: line
          }
        }).then(response => {
          if (response.data.result === 200) {
            //this.$set(this.$data, 'tableData' + line, response.data.data);
            let dataArray = [];
            let dataGroup = Object.keys(response.data.data);
            for (let i = 1; i <= dataGroup.length / 2; i++) {
              dataArray.push({name: response.data.data['errorName' + i], count: response.data.data['errorNum' + i]})
            }
            this['chartData' + line].rows = dataArray;
            this.setErrMsgTitle(line);
          } else {
            console.log(response.data.data)
          }
        }).catch(err => {
          console.log(err)
        })
      },

      preloadAll() {
        return new Promise(resolve => {
          Promise.all([this.fetchLineData(0), this.fetchLineData(1), this.fetchLineData(2)]).then(() => {
            resolve();
          })
        })
      },

      /*详情表格行合并，获取合并记录*/
      getSpanArr(tableData, keyName, line) {
        keyName.forEach((kitem, k) => {
          tableData.forEach((data, i) => {
            if (i === 0) {
              this['mergeData' + line][kitem] = this['mergeData' + line][kitem] || [];
              this['mergeData' + line][kitem].push(1);
              this['mergePos' + line][kitem] = 0
            } else {
              // 判断当前元素与上一个元素是否相同
              if (data[kitem] === tableData[i - 1][kitem]) {
                this['mergeData' + line][kitem][this['mergePos' + line][kitem]] += 1;
                this['mergeData' + line][kitem].push(0)
              } else {
                this['mergeData' + line][kitem].push(1);
                this['mergePos' + line][kitem] = i
              }
            }
          })
        });
      },
      detailsTableSpanMethod0({row, column, rowIndex, columnIndex}) {
        if (this.mergeProp.includes(column.property)) {
          const _row = this.mergeData0[column.property][rowIndex];
          const _col = _row > 0 ? 1 : 0;
          return {
            rowspan: _row,
            colspan: _col
          }
        }
      },
      detailsTableSpanMethod1({row, column, rowIndex, columnIndex}) {
        if (this.mergeProp.includes(column.property)) {
          const _row = this.mergeData1[column.property][rowIndex];
          const _col = _row > 0 ? 1 : 0;
          return {
            rowspan: _row,
            colspan: _col
          }
        }
      },
      detailsTableSpanMethod2({row, column, rowIndex, columnIndex}) {
        if (this.mergeProp.includes(column.property)) {
          const _row = this.mergeData2[column.property][rowIndex];
          const _col = _row > 0 ? 1 : 0;
          return {
            rowspan: _row,
            colspan: _col
          }
        }
      },

      setErrMsgTitle(line) {
        let now = getTime();
        let begin = now.slice(0, 10);
        this['errMsgTitle' + line] = begin + ' 00:00:00 至\n' + now + ' 不良原因分布'
      },

    }
  }
</script>

<style scoped>

  .dashboard {
    position: absolute;
    width: 100%;
    height: 100vh;
    padding: 20px;
    box-sizing: border-box;
    background: url("../../assets/dashboard_bg.png") fixed;
  }

  .dashboard-title {
    width: 100%;
    padding: 10px 20px;
    background: rgb(60, 111, 222);
    background: linear-gradient(90deg, rgb(55, 133, 230) 0%, rgba(69, 165, 229, 0.8) 80%);
    box-sizing: border-box;
    min-height: 60px;
    margin-bottom: 10px;
    box-shadow: 0 0 10px #b6b6b6;
    border-radius: 5px;
  }

  .dashboard-title span {
    display: inline-block;
    width: 100%;
    text-align: center;
    height: 60px;
    line-height: 60px;
    font-size: 40px;
    font-weight: bold;
    /*color: #555555;*/
    color: #ffffff;
  }

  .dashboard-table {
    width: 100%;
    min-height: 200px;
    height: 58vh;
    background: rgb(60, 111, 222);
    background: linear-gradient(90deg, rgb(55, 126, 222) 0%, rgba(69, 165, 229, 0.8) 80%);
    opacity: 0.9;
    padding: 10px 20px 20px;
    box-sizing: border-box;
    box-shadow: 0 0 10px #b6b6b6;
    border-radius: 5px;
    margin-bottom: 10px;
  }

  .dashboard-table /deep/ * {
    background-color: rgba(0, 0, 0, 0);
    color: #ffffff;
    font-size: 18px;
  }

  .dashboard-component-container {
    height: 29vh;
    display: flex;
    justify-content: flex-start;
  }

  .dashboard-component {
    background: rgb(62, 131, 255);
    background: linear-gradient(135deg, rgb(59, 136, 235) 0%, rgba(82, 170, 232, 0.8) 60%);
    opacity: 0.9;
    height: 29vh;
    box-sizing: border-box;
    box-shadow: 0 0 10px #b6b6b6;
    border-radius: 5px;
    margin-right: 10px;
    position: relative;
  }

  .dashboard-component:last-child {
    margin: 0;
  }

  .dashboard-component-info {
    background: rgba(84, 191, 255, 0.8);
    background: linear-gradient(90deg, rgba(84, 191, 255, 0.8) 0%, rgba(169, 218, 255, 0.8) 60%);
    display: flex;
    flex-direction: column;
    justify-content: space-around;
    padding: 38px 40px;
    color: #5f6066;
    font-weight: bold;
  }

  .dashboard-component-chart {
    background: rgba(82, 170, 232, 0.8);
    background: linear-gradient(90deg, rgba(82, 170, 232, 0.8) 0%, rgba(228, 245, 255, 0.8) 30%, rgba(84, 191, 255, 0.8) 95%);
    padding: 10px 0;
  }

  .dashboard-msg-title, .dashboard-msg-item {
    color: #ffffff;
  }

  .dashboard-msg-title {
    padding: 20px 30px;
    font-weight: bold;
    white-space: pre-wrap;
  }

  .dashboard-msg-content {
    width: 100%;
    height: 60%;
    overflow-y: hidden;
  }

  .dashboard-msg-item {
    width: 100%;
    display: flex;
    padding: 0 30px 6px;
    box-sizing: border-box;

  }

  .dashboard-msg-subtitle {
    font-weight: bold;
    width: 70%;
  }

  .dashboard-msg-subtitle + .dashboard-msg-subtitle {
    width: 20%;
  }

  .dashboard-msg-subitem {
    width: 72%;
  }

  .dashboard-msg-subitem + .dashboard-msg-subitem {
    width: 18%;
  }

  /*.dashboard-msg-subtitle + .dashboard-msg-subtitle {*/
  /*margin-left: 100px;*/
  /*}*/

  /*.dashboard-msg-subitem + .dashboard-msg-subitem {*/
  /*margin-left: 100px;*/
  /*}*/


</style>
