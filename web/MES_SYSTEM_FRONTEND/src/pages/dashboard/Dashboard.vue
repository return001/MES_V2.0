<template>
  <div id="dashboard" :style="'font-size:' + clientFontSize + 'px' " >
    <el-button id="call-fullscreen-btn"  @click="toggleFullScreen" circle icon="el-icon-full-screen"></el-button>
    <el-carousel indicator-position="none" height="100vh" :autoplay="false" ref="lineCarousel">
      <el-carousel-item>
        <div class="dashboard">
          <div class="dashboard-title" id="dashboard-title">
            <div>产品组装状态看板</div>
          </div>
          <div class="dashboard-table">
            <el-table
                :data="tableData0"
                :max-height="clientHeight * 0.48"
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
                <div class="dashboard-msg-item" v-for="item in chartData0.series[0].data">
                  <div class="dashboard-msg-subitem">{{item.name}}</div>
                  <div class="dashboard-msg-subitem">{{item.value}}</div>
                </div>
              </div>
            </div>
            <div class="dashboard-component dashboard-component-chart" style="width: 55%">
              <div id="dashboard-pie-0" style="width: 100%; height: 100%"></div>
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
                <span class="info-item-title">工程: 汤中虎</span>
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
            <div>产品测试状态看板</div>
          </div>
          <div class="dashboard-table">
            <el-table
                :data="tableData1"
                :max-height="clientHeight * 0.48"
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
                <div class="dashboard-msg-item" v-for="item in chartData1.series[0].data">
                  <div class="dashboard-msg-subitem">{{item.name}}</div>
                  <div class="dashboard-msg-subitem">{{item.value}}</div>
                </div>
              </div>
            </div>
            <div class="dashboard-component dashboard-component-chart" style="width: 55%">
              <div id="dashboard-pie-1" style="width: 100%; height: 100%"></div>
            </div>
            <div class="dashboard-component dashboard-component-info" style="width: 30%">
              <div class="info-item">
                <span class="info-item-title">生产线别: </span>
                <span class="info-item-content">测试</span>
              </div>
              <div class="info-item">
                <span class="info-item-title">生产: 李源飞</span>
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
            <div>产品包装状态看板</div>
          </div>
          <div class="dashboard-table">
            <el-table
                :data="tableData2"
                :max-height="clientHeight * 0.48"
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
                <div class="dashboard-msg-item" v-for="item in chartData2.series[0].data">
                  <div class="dashboard-msg-subitem">{{item.name}}</div>
                  <div class="dashboard-msg-subitem">{{item.value}}</div>
                </div>
              </div>
            </div>
            <div class="dashboard-component dashboard-component-chart" style="width: 55%">
              <div id="dashboard-pie-2" style="width: 100%; height: 100%"></div>
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
                <span class="info-item-title">工程: 汤中虎</span>
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
  </div>
</template>

<script>
  import {axiosFetch} from "../../utils/fetchData";
  import {
    dashboardSelectUrl,
    dashboardErrMsgSelectUrl,
    dashboardErrMsgSelectUrlCS,
    dashboardSelectUrlCS
  } from "../../config/globalUrl";
  import {getTime} from "../../utils/utils";

  import Echart from 'echarts'

  export default {
    name: "Dashboard",
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

        //EChart setting
        chartInstance0: undefined,
        chartInstance1: undefined,
        chartInstance2: undefined,
        chartData0: {
          textStyle: {
            fontSize: 12
          },
          color: ['#feffa6','#3eacdd', '#758ca8', '#d46b83', '#91c7ae','#749f83',  '#eea127', '#b89b97','#65676c', '#546570', '#c4ccd3'],
          label: {
            formatter: '{b} : {c} ({d}%)'
          },
          legend: {
            data: []
          },
          series: [{
            type: 'pie',
            radius: '68%',
            center: ['50%', '60%'],
            data: [],
          }]
        },
        chartData1: {
          textStyle: {
            fontSize: 12
          },
          color: ['#feffa6','#3eacdd', '#758ca8', '#d46b83', '#91c7ae','#749f83',  '#eea127', '#b89b97','#65676c', '#546570', '#c4ccd3'],
          label: {
            formatter: '{b} : {c} ({d}%)'
          },
          legend: {
            data: []
          },
          series: [{
            type: 'pie',
            radius: '68%',
            center: ['50%', '60%'],
            data: [],
          }]
        },
        chartData2: {
          textStyle: {
            fontSize: 12
          },
          color: ['#feffa6','#3eacdd', '#758ca8', '#d46b83', '#91c7ae','#749f83',  '#eea127', '#b89b97','#65676c', '#546570', '#c4ccd3'],
          label: {
            formatter: '{b} : {c} ({d}%)'
          },
          legend: {
            data: []
          },
          series: [{
            type: 'pie',
            radius: '68%',
            center: ['50%', '60%'],
            data: [],
          }]
        },
        clientWidth: document.body.clientWidth,
        clientHeight: document.body.clientHeight,
        clientFontSize: 16,
        dashboardChartShow: true
      }
    },
    mounted() {

      window.addEventListener('resize', () => {
        this.resetScale();
      });



      this.$nextTick(() => {
        const chartPie0 = this.chartInstance0 = Echart.init(document.getElementById('dashboard-pie-0'), 'light');
        const chartPie1 = this.chartInstance1 = Echart.init(document.getElementById('dashboard-pie-1'), 'light');
        const chartPie2 = this.chartInstance2 = Echart.init(document.getElementById('dashboard-pie-2'), 'light');



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
        }, 1000 * 15 * 60);

        this.resetScale();

      });
    },
    methods: {
      setScrollingInterval() {
        setTimeout(() => {
          this.mainScrollingInterval = setInterval(() => {
            if (this.$refs['tablecomponent' + this.currentLine].bodyWrapper.scrollTop < (this.$refs['tablecomponent' + this.currentLine].bodyWrapper.scrollHeight - this.$refs['tablecomponent' + this.currentLine].bodyWrapper.clientHeight)) {
              this.$refs['tablecomponent' + this.currentLine].bodyWrapper.scrollTop += ((this.clientWidth / 1920) < 1 ? 1 : (this.clientWidth / 1920))
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
            let legendArray = [];
            let dataGroup = Object.keys(response.data.data);
            for (let i = 1; i <= dataGroup.length / 2; i++) {
              dataArray.push({name: response.data.data['errorName' + i], value: response.data.data['errorNum' + i]});
              legendArray.push(response.data.data['errorName' + i])
            }
            this['chartData' + line].series[0].data = dataArray;
            this['chartData' + line].legend.data = legendArray;
            this['chartInstance' + line].setOption(this['chartData' + line]);
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

      toggleFullScreen() {
        /*支持chrome，做Firefox兼容*/
        if (!(document.fullscreenElement || document.mozFullScreen)) {
          if (!!document.documentElement.requestFullscreen) {
            document.documentElement.requestFullscreen({
              navigationUI: 'hide'
            })
          } else if (!!document.documentElement.mozRequestFullScreen){
            document.body.mozRequestFullScreen({
              navigationUI: 'hide'
            })
          }

        } else {
          document.exitFullscreen ? document.exitFullscreen() : document.mozCancelFullScreen();
        }
      },

      resetScale() {
        /*根据客户端DPI设置以确定页面字体缩放*/
        const designWidth = 1920;
        const currentWidth = document.body.clientWidth;
        this.clientFontSize = 16 * currentWidth / designWidth;

        this.clientHeight = document.body.clientHeight;
        this.clientWidth = document.body.clientWidth;

        for (let i = 0; i < 3; i ++) {
          this['chartData' + i].textStyle.fontSize = this.clientFontSize * 0.9;
          this['chartInstance' + i].setOption(this['chartData' + i]);
          this['chartInstance' + i].resize();
        }
      }

    }
  }
</script>

<style scoped>
  #dashboard {
    font-size: 16px;
    width: 100%;
    height: 100%;
  }

  #call-fullscreen-btn {
    position: absolute;
    right: 1em;
    top: 1em;
    z-index: 10;
    opacity: 0.1;
  }
  #call-fullscreen-btn:hover {
    opacity: 1;
  }

  .dashboard {
    position: absolute;
    width: 100%;
    height: 100%;
    padding: 1.6em;
    box-sizing: border-box;
    background: url("../../assets/dashboard_bg.png") fixed;
  }

  .dashboard-title {
    font-size: 1.6em;
    width: 100%;
    padding: 0.6em 2em;
    background: rgb(60, 111, 222);
    background: linear-gradient(90deg, rgb(55, 133, 230) 0%, rgba(69, 165, 229, 0.8) 80%);
    box-sizing: border-box;
    height: 10%;
    margin-bottom: 0.6em;
    box-shadow: 0 0 0.6em #b6b6b6;
    border-radius: 0.4em;
  }

  .dashboard-title div {
    font-size: 2em;
    width: 100%;
    height: 100%;
    font-weight: bold;
    color: #ffffff;
    display: flex;
    align-items: center;
    justify-content: center;
  }

  .dashboard-table {
    width: 100%;
    min-height: 5em;
    height: 56%;
    background: rgb(60, 111, 222);
    background: linear-gradient(90deg, rgb(55, 126, 222) 0%, rgba(69, 165, 229, 0.8) 80%);
    opacity: 0.9;
    padding: 0.6em 1.2em 1.2em;
    box-sizing: border-box;
    box-shadow: 0 0 0.6em #b6b6b6;
    border-radius: 0.4em;
    margin-bottom: 0.6em;
  }

  .dashboard-table /deep/ * {
    background-color: rgba(0, 0, 0, 0);
    color: #ffffff;
    font-size: 1em;
  }

  .dashboard-component-container {
    height: 32%;
    display: flex;
    justify-content: flex-start;
  }

  .dashboard-component {
    background: rgb(62, 131, 255);
    background: linear-gradient(135deg, rgb(59, 136, 235) 0%, rgba(82, 170, 232, 0.8) 60%);
    opacity: 0.9;
    height: 100%;
    box-sizing: border-box;
    box-shadow: 0 0 0.6em #b6b6b6;
    border-radius: 0.4em;
    margin-right: 0.6em;
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
    padding: 2em 2em;
    color: #5f6066;
    font-weight: bold;
  }

  .dashboard-component-chart {
    background: rgba(82, 170, 232, 0.8);
    background: linear-gradient(90deg, rgba(82, 170, 232, 0.8) 0%, rgba(228, 245, 255, 0.8) 30%, rgba(84, 191, 255, 0.8) 95%);
    padding: 0.6em 0;
  }

  .dashboard-msg-title, .dashboard-msg-item {
    color: #ffffff;
  }

  .dashboard-msg-title {
    padding: 1.6em 2em;
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
    padding: 0 2em 0.3em;
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
