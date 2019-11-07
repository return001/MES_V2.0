<template>
  <div id="esop-logs">
    <div class="esop-logs-main">
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
    ConfirmLogTableColumns,
    ConfirmLogQueryConfig,
    ClientLogTableColumns,
    ClientLogQueryConfig,
    NoticeLogQueryConfig,
    NoticeLogTableColumns
  } from "../../../config/ESopConfig";
  import {
    eSopConfirmLogSelectUrl,
    eSopLoginLogSelectUrl,
    eSopNoticeLogSelectUrl
  } from "../../../config/globalUrl";
  import common from "./mixins/common";

  export default {
    name: "ESopLogs",
    inject: ['reload'],
    mixins: [common],
    data() {
      return {
        /*搜索框*/
        queryConfig: [],
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
        tableColumns: [],
        tableData: [],
        dataQueryUrl: ''
      }
    },
    created() {
      switch (this.$route.path.split('/')[2]) {
        case 'confirm_log':
          this.queryConfig = ConfirmLogQueryConfig;
          this.tableColumns = ConfirmLogTableColumns;
          this.dataQueryUrl = eSopConfirmLogSelectUrl;
          break;
        case 'login_log':
          this.queryConfig = ClientLogQueryConfig;
          this.tableColumns = ClientLogTableColumns;
          this.dataQueryUrl = eSopLoginLogSelectUrl;
          break;
        case 'notice_log':
          this.queryConfig = NoticeLogQueryConfig;
          this.tableColumns = NoticeLogTableColumns;
          this.dataQueryUrl = eSopNoticeLogSelectUrl;
          break;
      }
    },
    watch: {
      $route: function () {
        this.reload();
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
          url: this.dataQueryUrl,
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

    }
  }
</script>

<style scoped>

  @import "styles/common.css";

  #esop-logs {
    position: relative;
    width: 100%;
    min-height: 600px;
    height: 100%;
  }


</style>
