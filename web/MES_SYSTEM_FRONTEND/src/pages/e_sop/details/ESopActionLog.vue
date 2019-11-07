<template>
  <div id="esop-log">
    <div class="esop-log-main">
      <div class="query-comp">
        <rz-query-bar
          :list="queryConfig"
          :button-group="buttonGroup"
          :data.sync="queryCompData"/>
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
        <el-table-column :key="10086"
                         prop="resultCode"
                         label="结果码"
                         min-width="80px">
          <template slot-scope="scope">
            <span :class="getLogResultStyle(scope.row.resultCode)">{{scope.row.resultCode}}</span>
          </template>
        </el-table-column>

        <el-table-column
          type="index"
          :index="_indexMethod"
          fixed="left"
          width="60">
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
        @current-change="fetchData"
        @size-change="_queryData"
        class="page-pagination">
      </el-pagination>
    </div>
  </div>
</template>

<script>
  import {
    ActionLogQueryConfig,
    ActionLogTableColumns,
  } from "../../../config/ESopConfig";
  import {
    eSopActionLogSelectUrl,
  } from "../../../config/globalUrl";
  import common from "./mixins/common";

  export default {
    name: "ESopActionLog",
    mixins: [common],
    data() {
      return {
        /*搜索框*/
        queryConfig: ActionLogQueryConfig,
        buttonGroup: [
          {
            label: '重置条件',
            size: 'small',
            type: 'info',
            callback() {
            }
          },
          {
            label: '查询',
            size: 'small',
            type: 'primary',
            callback() {
            }
          },
        ],
        queryCompData: {},
        tableColumns: ActionLogTableColumns,
        tableData: [],
      }
    },
    mounted() {
      /*注册按键*/
      this.buttonGroup[0].callback = this._initQueryOptions;
      this.buttonGroup[1].callback = this._queryData;

      this._queryData();
    },
    methods: {
      /*查询、获取表格内容*/
      fetchData() {
        this.$openLoading();
        let options = {
          url: eSopActionLogSelectUrl,
          data: {
            pageNo: this.paginationOptions.currentPage,
            pageSize: this.paginationOptions.pageSize
          }
        };
        Object.keys(this.queryCompData).forEach(key => {
          if (!!this.queryCompData[key] || this.queryCompData[key] === 0) {
            if (key === 'time') {
              options.data.timeFrom = this.queryCompData[key][0];
              options.data.timeTo = this.queryCompData[key][1];
            } else {
              options.data[key] = this.queryCompData[key];
            }
          }
        });
        this._fetchData(options).then(data => {
          this.tableData = data.list;
        })
      },


      getLogResultStyle(code) {
        if (code === 200) {
          return 'result-code-success'
        } else {
          return 'result-code-failure'
        }
      }
    }
  }
</script>

<style scoped>

  @import "styles/common.css";

  #esop-log {
    position: relative;
    width: 100%;
    min-height: 600px;
    height: 100%;
  }

  .result-code-success {
    font-weight: bold;
    color: #00ed03;
  }
  .result-code-failure {
    color: red;
    font-weight: bold;
  }

</style>
