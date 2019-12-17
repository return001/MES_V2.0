<!--订单配置页面表单-->
<template>
  <div class="main-details mt-1 mb-3">
    <el-table
      :data="tableData"
      max-height="560"
      ref="tablecomponent"
      stripe>
      <el-table-column v-for="(item, index) in tableColumns"
                       :key="index"
                       :prop=item.field
                       :label="item.title"
                       :min-width=item.colStyle.width
                       :formatter=item.formatter>
      </el-table-column>

      <el-table-column
        type="index"
        :index="indexMethod"
        fixed="left"
        width="60">
      </el-table-column>

      <el-table-column
        label="操作"
        width="110"
        fixed="left"
        v-if="$store.state.userType === 'SuperAdmin'"
      >
        <template slot-scope="scope">
          <el-button type="text" class="el-icon-t-edit" @click="editOrder('edit', scope.row)">
          </el-button>
          <el-button type="text" class="el-icon-t-copy" @click="editOrder('copy', scope.row)">
          </el-button>
          <el-button type="text" class="el-icon-t-menu" @click="editStatus(scope.row)" :disabled="scope.row.Status > 1">
          </el-button>
          <!--<el-button type="text" size="small" @click="editUser(scope.row)">-->
          <!--<icon name="bind" scale="1.8"></icon>-->
          <!--</el-button>-->
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
    <el-dialog class="status-panel" v-if="isStatusPanel" title="更改状态" :visible.sync="isStatusPanel" width="360px"
               :close-on-click-modal="false" :close-on-press-escape="false" :show-close="false">
      <div class="status-panel-container">
        <div class="status-select">
          <el-select size="small" id="status-select"
                     v-model="orderStatus">
            <el-option label="未开始" :value='0' disabled></el-option>
            <el-option label="进行中" :value='1' :disabled="thisRow.Status >= 1"></el-option>
            <el-option label="已完成" :value='2'></el-option>
            <el-option label="已作废" :value='3'></el-option>
          </el-select>
        </div>
        <div class="divider"></div>
        <div class="form-group-btn">
          <el-button size="small" type="info" @click="isStatusPanel = !isStatusPanel">取消</el-button>
          <el-button size="small" type="primary" @click="statusSubmit">提交</el-button>
        </div>
      </div>
    </el-dialog>
  </div>
</template>

<script>
  import {axiosFetch} from "../../../../utils/fetchData";
  import {mapGetters, mapActions} from 'vuex'
  import {getOrderConfig, orderSelectUrl, orderOperUrl} from "../../../../config/orderApiConfig";
  import EditPanel from './EditPanel';
  import {errHandler} from "../../../../utils/errorHandler";
  import eventBus from "../../../../utils/eventBus";

  export default {
    name: "Details",
    components: {
      EditPanel
    },
    inject: ['reload'],
    data() {
      return {
        tableData: [],
        tableColumns: [],
        isPending: false,
        paginationOptions: {
          currentPage: 1,
          pageSize: 20,
          total: 0
        },
        queryString: '',
        isStatusPanel: false,
        thisRow: {},
        orderStatus: 0,
      }
    },
    created() {
    },
    computed: {
      ...mapGetters([
        'tableRouterApi'
      ]),

    },
    mounted: function () {
      this.init();
      this.thisFetch();

      eventBus.$off('orderQueryData');
      eventBus.$on('orderQueryData', data => {
        this.paginationOptions.currentPage = 1;
        this.queryString = data;
        this.thisFetch();
      })
    },
    watch: {
      $route: function (route) {
        this.init();
        this.$openLoading();
        let options = {
          url: orderSelectUrl,
          data: {
            pageNo: this.paginationOptions.currentPage,
            pageSize: this.paginationOptions.pageSize,
            descBy: 'ID'
          }
        };
        this.fetchData(options)
      }
    },
    methods: {
      init: function () {
        Object.assign(this.$data, this.$options.data())
      },
      thisFetch: function (opt) {
        if (opt === 'sizeChange') {
          this.paginationOptions.currentPage = 1;
        }

        this.$openLoading();
        let options = {
          url: orderSelectUrl,
          data: {
            pageNo: this.paginationOptions.currentPage,
            pageSize: this.paginationOptions.pageSize,
            descBy: 'ID'
          }
        };
        this.fetchData(options)
      },
      fetchData: function (options) {
        if (this.queryString !== '') {
          options.data.filter = this.queryString
        }
        let routerConfig = getOrderConfig('order_manage');
        this.tableColumns = routerConfig.data.dataColumns;
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          axiosFetch(options).then(response => {
            if (response.data.result === 200) {
              this.tableData = response.data.data.list;
              this.paginationOptions = {
                currentPage: response.data.data.pageNumber,
                pageSize: response.data.data.pageSize,
                total: response.data.data.totalRow
              }
            } else {
              errHandler(response.data.result)
            }
          }).catch(err => {
            console.log(JSON.stringify(err));
            this.$alertDanger('请求超时，清刷新重试')
          }).finally(() => {
            this.$closeLoading();
            this.isPending = false;
          })
        } else {
          this.$closeLoading()
        }
      },


      /*编辑/复制条目 $on at @/pages/order/details/comp/EditPanel*/
      editOrder: function (type, val) {
        eventBus.$emit('editOrder', [type, val])
      },

      /*控制索引递增*/
      indexMethod: function (index) {
        return index + (this.paginationOptions.currentPage - 1) * this.paginationOptions.pageSize + 1
      },

      editStatus: function (val) {
        this.thisRow = JSON.parse(JSON.stringify(val));
        this.orderStatus = this.thisRow.Status;
        this.isStatusPanel = true;
      },

      statusSubmit: function () {
        if (this.orderStatus > this.thisRow.Status) {
          this.isPending = true;
          this.$openLoading();
          let thisStatus;
          switch (this.orderStatus) {
            case 1:
              thisStatus = '/start';
              break;
            case 2:
              thisStatus = '/finish';
              break;
            case 3:
              thisStatus = '/cancel';
              break;
          }
          let options = {
            url: orderOperUrl + thisStatus,
            data: {
              key: this.thisRow.Id
            }
          };
          axiosFetch(options).then(res => {
            if (res.data.result === 200) {
              this.$alertSuccess('更新成功');
              this.isStatusPanel = false;
              this.thisRow = {};
              this.orderStatus = 0;
              this.reload();
            } else {
              this.$alertWarning(res.data.data)
            }
          }).catch(err => {
            this.$alertDanger(err);
          }).finally(() => {
            this.isPending = false;
            this.$closeLoading();
          })

        } else {
          this.$alertInfo('请更改状态');
        }
      },
    }
  }
</script>

<style scoped>
  .main-details {
    background: #fff;
    border: 1px solid #eeeeee;
    border-radius: 8px;
    padding: 10px;
    min-height: 500px;
  }

  .page-pagination {
    display: flex;
    flex-wrap: wrap;
    margin: 20px 0;
    padding: 0 20px;
  }

  .status-panel-container {
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
  }

  .form-group-btn .el-button {
    width: 100px;
  }

  .divider {
    border-top: 1px solid #eee;
    width: 100%;
    margin: 20px 0;
  }

  .fade-enter-active, .fade-leave-active {
    transition: opacity .5s;
  }

  .fade-enter, .fade-leave-to {
    opacity: 0;
  }
</style>
