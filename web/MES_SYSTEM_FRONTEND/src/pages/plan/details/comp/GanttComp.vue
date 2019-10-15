<template>
  <el-dialog
    title="甘特图"
    :visible.sync="isShowing"
    :close-on-click-modal="false"
    :close-on-press-escape="false"
    @closed="resetData"
    width="95%">
    <el-table
      :data="tableData"
      max-height="560"
      size="mini"
      ref="tablecomponent">
      <el-table-column
        type="index"
        :index="indexMethod"
        fixed="left"
        width="60px">
      </el-table-column>
      <el-table-column
        v-for="(item, index) in tableColumns"
        :key="item.key + index"
        :prop="item.key"
        :width="!!item.width ? item.width : '90px'"
        fixed
        :label="item.label">

      </el-table-column>
      <el-table-column
        :label="timeLineYear"
        class-name="gantt-column">
        <el-table-column
          v-for="(item, index) in timeLine"
          :key="item.key + index"
          :prop="item.key"
          :label="item.label"
          width="50px"
          class-name="gantt-column">
          <template slot-scope="scope">
            <span :class="setColumnsColor(scope)"></span>
          </template>
        </el-table-column>
      </el-table-column>

    </el-table>
  </el-dialog>
</template>

<script>
  export default {
    name: "GanttComp",
    props: ['propTableData'],
    data() {
      return {
        isShowing: false,
        tableColumns: [
          {key: 'name', label: '工作名称', width: '120px'},
          {key: 'planProduction', label: '工程量(PCS)'},
          {key: 'intervalDay', label: '工期'},
          {key: 'startTime', label: '开始时间'},
          {key: 'endTime', label: '结束时间'},
          {key: 'completionRate', label: '完成率'},
          {key: 'completionQuantity', label: '完成量'},


        ],
        timeLine: [],
        tableData: [],
        timeLineYear: ''
      }
    },

    watch: {
      propTableData(val) {
        this.$set(this.$data, 'tableData', val);
        this.initGanttColumns();
        this.isShowing = true;
      }
    },
    mounted() {
    },

    methods: {
      indexMethod(index) {
        return index === 0 ? 1 : ('1.' + index)
      },


      resetData() {
        this.tableData = [];
        this.timeLine = [];
      },

      /**
       **@description: 根据行列信息设置时间线颜色填充
       * prop为列头部时间的毫秒值
       **@date: 2019/8/8 16:14
       **@author: DarkNin
       **@method:
       **@params:
       */
      setColumnsColor(scope) {
        let thatTime = scope.column.property; //timeLine[x].key 当日时间
        let endTime = new Date(scope.row.endTime).getTime();
        let startTime = new Date(scope.row.startTime).getTime();
        if (!!endTime && !!startTime) {
          if (endTime - thatTime >= 0 && startTime - thatTime <= 0) {
            //当起始时间均存在且判定日期包含在其中
            return 'gantt-color-fill'
          }
        } else if (!endTime && !!startTime) {
          if (new Date().getTime() - thatTime >= 0) {
            //当结束时间不存在而开始时间存在，填充至当前时间
            return 'gantt-color-fill'
          }
        }
      },

      /**
       **@description: 初始化时间线表头数据结构
       **@date: 2019/8/8 16:16
       **@author: DarkNin
       **@method:
       **@params:
       */
      initGanttColumns() {
        //设置时间线表头
        this.timeLine = [];
        let date = new Date();
        let endTime;
        if (this.tableData[0].endTime !== '-') {
          endTime = new Date(this.tableData[0].endTime).getTime();
        } else {
          endTime = new Date().getTime();
        }
        let startTime = new Date(this.tableData[0].startTime).getTime();
        if (!startTime) {
          return
        }
        let duringDay = Math.floor((endTime - startTime) / 1000 / 3600 / 24);
        for (let i = 0; i < duringDay + 3; i++) {
          let date = new Date(startTime + i * 24 * 3600 * 1000);
          let labelString = (date.getMonth() + 1) + '/' + date.getDate();
          this.timeLine.push({key: date.getTime().toString(), label: labelString}); //填充时间线表的表头结构，以当日时间的毫秒值作为key
        }

        //填充表头年份
        let endYear = new Date(this.tableData[0].endTime).getFullYear();
        let startYear = new Date(this.tableData[0].startTime).getFullYear();
        if (!endYear && !!startYear) {
          this.timeLineYear = startYear.toString();
        } else if (!!endYear && !!startYear) {
          this.timeLineYear = (startYear - endYear === 0 ? startYear : (startYear + ' - ' + endYear)).toString();
        }
      }
    }
  }
</script>

<style scoped>
  .gantt-main {
    height: 600px;
  }

  .el-table /deep/ .gantt-column {
    border-right: none;
    text-align: center;
  }

  .el-table /deep/ th.gantt-column {
    border-right: 1px solid #EBEEF5;
  }

  .el-table /deep/ .gantt-column .cell {
    line-height: unset;
    height: 20px;
    padding: 0;
  }

  .gantt-color-fill {
    background: #000;
    display: inline-block;
    height: 20px;
    width: 50px;
    line-height: unset;
    background: linear-gradient(135deg, #ffffff 25%, #54b6f6 0, #54b6f6 50%,
    #ffffff 0, #ffffff 75%, #54b6f6 0);
    background-size: 10px 10px;
  }


</style>
