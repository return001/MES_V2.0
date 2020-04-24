<template>
  <div id="process-setting">
    <div class="process-setting-main">
      <div class="query-comp">
        <!--选择器-->
        <div class="query-comp-container">
          <div class="query-comp-select">
            <label for="line-query-item">产线:</label>
            <el-select
                v-model="selectedLine"
                @change="fetchData"
                id="line-query-item"
                placeholder="请选择产线"
                size="small">
              <el-option v-for="listItem in lineSelectGroup"
                         :key="listItem.id"
                         :value="listItem.id"
                         :label="listItem.lineName"></el-option>
            </el-select>
          </div>
        </div>
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
            fixed="left"
            width="60">
        </el-table-column>

        <el-table-column
            label="操作"
            width="80"
            fixed="right"
            v-if="_permissionControl(['SopManager'])"
        >
          <template slot-scope="scope">
            <el-tooltip content="指派" placement="top">
              <el-button type="text" @click="showUser(scope.row)"
                         icon="el-icon-s-custom"></el-button>
            </el-tooltip>
          </template>
        </el-table-column>
      </el-table>
    </div>
    <!--dialog component-->
    <el-dialog
        title="人员指派"
        :visible.sync="isAssigning"
        :close-on-click-modal="false"
        :close-on-press-escape="false"
        @close="closeEditProcessPanel"
        top="5vh"
        width="80%">

      <div class="staff-import-stash-group">
        <div class="staff-import-stash-title">已选择员工:</div>
        <div class="staff-import-stash-content">
          <el-tag
              v-for="(item, index) in importingStaffGroup"
              :key="item.Id"
              closable
              size="small"
              @close="deleteStashStaff(index)">
            {{item.Name}}
          </el-tag>
        </div>
      </div>
      <rz-query-bar
          :list="staffQueryConfig"
          :button-group="staffButtonGroup"
          :data.sync="staffQueryOptions"/>
      <el-table
          :data="assignableStaffData"
          max-height="400"
          ref="subtablecomponent"
          size="mini"
          stripe>
        <el-table-column
            type="index"
            :index="staffIndexMethod"
            fixed="left"
            width="60">
        </el-table-column>
        <el-table-column v-for="(item, index) in assignableStaffColumns"
                         :key="index"
                         :prop="item.key"
                         :label="item.label"
                         :min-width="item['min-width']"
                         :formatter="item.formatter">
        </el-table-column>
        <el-table-column
            label="操作"
            width="80"
            fixed="right"
        >
          <template slot-scope="scope">
            <el-tooltip content="添加" placement="top">
              <el-button type="text" @click="addStaff(scope.row)"
                         icon="el-icon-plus"></el-button>
            </el-tooltip>
          </template>
        </el-table-column>
      </el-table>
      <!--分页控制-->
      <el-pagination
          background
          :page-sizes="[20]"
          :current-page.sync="staffGroupPaginationOptions.currentPage"
          :page-size.sync="staffGroupPaginationOptions.pageSize"
          layout="total, sizes, prev, pager, next, jumper"
          :total="staffGroupPaginationOptions.total"
          @current-change="showUser(queryingRow)"
          class="page-pagination">
      </el-pagination>
      <span slot="footer" class="dialog-footer">
        <el-button size="small" @click="closeEditProcessPanel" type="info">取消</el-button>
        <el-button size="small" @click="submitAssignStaff" type="primary">保存</el-button>
      </span>
    </el-dialog>
  </div>

</template>

<script>
  import {
    assignTableColumns, assignStaffTableColumns
  } from "../../../config/ESopConfig";
  import {axiosFetch} from "../../../utils/fetchData";
  import {MessageBox} from 'element-ui';
  import common from "./mixins/common";
  import {
    planLineGetUrl,
    eSopStationStatusSelectUrl,
    eSopAssignableStaffSelectUrl,
    eSopAssignStaffUrl
  } from "../../../config/globalUrl";

  export default {
    name: "StaffAssign",
    inject: ['reload'],
    mixins: [common],
    data() {
      return {
        tableData: [],
        tableColumns: assignTableColumns,
        lineSelectGroup: [],
        selectedLine: '',
        isAssigning: false,
        paginationOptions: {
          currentPage: 1,
          pageSize: 65535,
          total: 0
        },

        assignableStaffColumns: assignStaffTableColumns,
        staffGroupPaginationOptions: {
          currentPage: 1,
          pageSize: 20,
          total: 0
        },
        assignableStaffData: [],
        importingStaffGroup: [],
        queryingRow: null,
        staffQueryOptions: {},
        staffQueryConfig: [
          {
            type: 'text',
            label: '工号',
            size: 'small',
            prop: 'userName',
          },
          {
            type: 'text',
            label: '描述',
            size: 'small',
            prop: 'userDes',
          }
        ],
        staffButtonGroup: [
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
      }
    },
    computed: {},
    async created() {
      await this.fetchLine();
    },
    mounted() {
      this.staffButtonGroup[0].callback = () => {
        this.staffQueryOptions = {}
      };
      this.staffButtonGroup[1].callback = () => {
        this.showUser(this.queryingRow)
      }
    },
    methods: {
      /*局部刷新*/
      partlyReload() {
        this._partlyReload(['lineSelectGroup', 'selectedLine'])
      },

      /*获取产线信息*/
      fetchLine: function () {
        return new Promise(resolve => {
          axiosFetch({
            url: planLineGetUrl
          }).then(response => {
            if (response.data.result === 200) {
              this.lineSelectGroup = response.data.data.list;
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

      staffIndexMethod: function (index) {
        return index + (this.staffGroupPaginationOptions.currentPage - 1) * this.staffGroupPaginationOptions.pageSize + 1
      },

      queryData: function () {
        this.paginationOptions.pageNo = 1;
        this.paginationOptions.total = 0;
        this.fetchData();
      },

      fetchData: function (lineId) {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          let options = {
            url: eSopStationStatusSelectUrl,
            data: {
              lineId: lineId || this.selectedLine,
              pageNo: this.paginationOptions.currentPage,
              pageSize: this.paginationOptions.pageSize,
            }
          };
          axiosFetch(options).then(response => {
            if (response.data.result === 200) {
              if (response.data.data === null) {
                this.$alertInfo('查无数据');
                this.tableData = [];
              } else {
                this.tableData = response.data.data;
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
      showUser(row) {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          this.queryingRow = row;
          let options = {
            url: eSopAssignableStaffSelectUrl,
            data: {
              pageNo: this.staffGroupPaginationOptions.currentPage,
              pageSize: this.staffGroupPaginationOptions.pageSize,
              userName: !!this.staffQueryOptions.userName ? this.staffQueryOptions.userName : undefined,
              userDes: !!this.staffQueryOptions.userDes ? this.staffQueryOptions.userDes : undefined,
            }
          };
          if (row.postId) {
            options.data.postId = row.postId
          } else {
            options.data.processId = row.processId
          }
          axiosFetch(options).then(response => {
            if (response.data.result === 200) {
              if (response.data.data === null) {
                this.$alertInfo('查无数据');
              } else {
                this.assignableStaffData = response.data.data.list;
                this.staffGroupPaginationOptions.currentPage = response.data.data.pageNumber;
                this.staffGroupPaginationOptions.total = response.data.data.totalRow;
                this.isAssigning = true;
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
      closeEditProcessPanel: function () {
        this.isAssigning = false;
        this.assignableStaffData = [];
        this.importingStaffGroup = [];
        this.queryingRow = null;
        this.staffQueryOptions = {};
      },

      addStaff(row) {
        if (this.importingStaffGroup.length > 0) {
          this.$alertInfo('已选择员工');
          return;
        }
        /*for (let i = 0; i < this.importingStaffGroup.length; i++) {
          if (row.Id === this.importingStaffGroup[i].Id) {
            this.$alertInfo('已选择此员工');
            return;
          }
        }*/
        this.importingStaffGroup.push(row);
      },
      deleteStashStaff(index) {
        this.importingStaffGroup.splice(index, 1)
      },
      submitAssignStaff() {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          let options = {
            url: eSopAssignStaffUrl,
            data: {
              userId: this.importingStaffGroup.map(item => item.Id).toString()
            }
          };
          if (this.queryingRow.postId) {
            options.data.postId = this.queryingRow.postId
          } else {
            options.data.lineId = this.selectedLine;
            options.data.processId = this.queryingRow.processId;
            options.data.siteId = this.queryingRow.siteId;
          }
          axiosFetch(options).then(response => {
            if (response.data.result === 200) {
              this.$alertSuccess(response.data.data);
              this.closeEditProcessPanel();
              this.partlyReload();
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
      }
    }
  }
</script>

<style scoped>
  @import "styles/common.css";

  #process-setting /deep/ .el-button i {
    font-size: 17px;
    font-weight: bold;
  }

  #process-setting {
    position: relative;
    width: 100%;
    min-height: 600px;
    height: 100%;
  }

  .process-setting-main {
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

  .staff-import-stash-group {
    margin-bottom: 5px;
  }

  .staff-import-stash-title {
    font-weight: bold;
    color: #909399;
  }

  .staff-import-stash-content {
    width: 100%;
    min-height: 32px;
    border-radius: 5px;
    background-color: #f0f0f0;
    display: flex;
    align-items: center;
  }

  .el-tag {
    margin: 0 5px;
  }

</style>
