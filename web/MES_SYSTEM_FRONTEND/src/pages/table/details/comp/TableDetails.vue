<!--表单查看页面的表单详情 统一配置-->
<template>
  <div class="main-details mt-1 mb-3">
    <el-table
      :data="tableData"
      max-height="560"
      stripe
      ref="tablecomponent"
      @selection-change="selectionChangeHandler">
      <el-table-column
        type="selection"
        width="30"
        v-if="$route.query.type !== 'Gps_ManuCpParam' && $route.query.type !== 'LTestLogMessage' && !isReferred && checkDelPermission"
      >
      </el-table-column>
      <el-table-column v-for="(item, index) in getTableConfig.dataColumns"
                       :key="index"
                       :prop=item.field
                       :label="item.title + (item.field === getTableConfig.primaryKey ? '(主)' : '')"
                       :min-width=item.colStyle.width
                       :formatter=item.formatter>
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
      @size-change="sizeChange()"
      class="page-pagination">
    </el-pagination>
    <div class="select-box" v-show="deleteIdGroup.length !== 0">
      <div class="selected-content"><p>已选择主标识为 <span style="display: inline-block; font-weight: bold;"
                                                     v-for="(item, index) in deleteIdGroup">{{item[getTableConfig.primaryKey]}}{{index !== deleteIdGroup.length - 1 ? ',' : ''}}</span>
        的项目</p></div>
      <div class="control-bar">
        <el-button type="info" size="mini" @click="toggleSelectionStatus">取消选中</el-button>
        <!--<el-button type="warning" size="mini" @click="deleteTableData">删除所选项</el-button>-->
        <el-button type="warning" size="mini" @click="validateVisible = true">删除所选项</el-button>
        <el-button type="primary" size="mini" @click="cleanupItems" v-if="$route.query.type === 'DataRelativeSheet'">
          清空指定项目
        </el-button>
      </div>
    </div>
    <el-dialog
      width="500px"
      title="选择清空项目"
      :visible.sync="isRelCleanup"
      class="cleanup-dialog"
    >
      <el-checkbox-group v-model="cleanupChoseItems">
        <el-checkbox label="IMEI2">SN号</el-checkbox>
        <el-checkbox label="IMEI3">SIM卡号</el-checkbox>
        <el-checkbox label="IMEI4">ICCID</el-checkbox>
        <el-checkbox label="IMEI5">密码</el-checkbox>
        <el-checkbox label="IMEI6">MAC(蓝牙)</el-checkbox>
        <el-checkbox label="IMEI7">设备号</el-checkbox>
        <el-checkbox label="IMEI8">服务卡号</el-checkbox>
        <el-checkbox label="IMEI9">电池序列号</el-checkbox>
        <el-checkbox label="IMEI10">第二个锁ID</el-checkbox>
        <el-checkbox label="IMEI11">机器代码</el-checkbox>
        <el-checkbox label="IMEI12">IMSI</el-checkbox>
        <el-checkbox label="IMEI13">RFID</el-checkbox>
      </el-checkbox-group>
      <span slot="footer" class="dialog-footer">
          <el-button @click="submitCleanup">确认</el-button>
        </span>
    </el-dialog>
    <!--删除时用户校验-->
    <el-dialog
      title="请校验用户名与密码"
      :visible.sync="validateVisible"
      width="400px"
      :close-on-click-modal="false"
      :close-on-press-escape="false">

      <div class="">
        <label for="validate-user" style="line-height: 30px">用户名</label>
        <el-input size="small" type="text" id="validate-user" v-model.trim="validateInfo.user" autocomplete="off" clearable
                  placeholder="请填写用户名"></el-input>
      </div>
      <div class="">
        <label for="validate-pwd" style="line-height: 30px">密码</label>
        <el-input size="small" type="password" id="validate-pwd" v-model.trim="validateInfo.password" autocomplete="off" clearable
                  placeholder="请填写密码"></el-input>
      </div>
      <span slot="footer">
        <el-button size="small" @click="validateVisible = false">取 消</el-button>
        <el-button size="small" type="primary" @click="validateDelete">校 验</el-button>
      </span>
    </el-dialog>
  </div>

</template>

<script>
  import eventBus from '@/utils/eventBus'
  import {tableDeleteByIdsUrl, cleanupRelUrl, validateUrl} from "../../../../config/globalUrl";
  import {axiosFetch} from "../../../../utils/fetchData";
  import {mapGetters, mapActions} from 'vuex'
  import {setRouterConfig, tableSelectUrl, getRequestUrl} from "../../../../config/tableApiConfig";
  import {errHandler} from "../../../../utils/errorHandler";
  import {checkDelPermission} from "../../../../utils/utils";
  import {alertDanger} from "../../../../utils/modal";

  export default {
    name: "Details",
    components: {},
    data() {
      return {
        isPending: false,
        thisRouter: '',
        //分页选项
        tableData: [],
        paginationOptions: {
          currentPage: 1,
          pageSize: 20,
          total: 0
        },
        queryString: '',
        deleteIdGroup: [],
        isReferred: false,
        isRelCleanup: false,
        cleanupChoseItems: [],
        validateVisible: false,
        validateInfo: {
          user: '',
          password: ''
        },
      }
    },
    created() {
    },
    computed: {
      ...mapGetters([
        'tableDeleteGroup'
      ]),
      getTableConfig: function () {
        return setRouterConfig(this.$route.query.type, {isReferred: this.isReferred}).data
      },
      checkDelPermission: function () {
        return checkDelPermission(this.$route.query.type)
      }
    },
    watch: {
      $route: function (route) {
        this.init();
        eventBus.$emit('setIsReferred', false);
        eventBus.$emit('setTableDataCount', 0);
        this.isReferred = false;
        /*this.$openLoading();
        if (route.query.type) {
          let options = {
            url: getRequestUrl(route.query.type),
            data: {
              table: route.query.type,
              pageNo: 1,
              pageSize: 20
            }
          };
          if (this.$route.query.type === 'DataRelativeSheet') {
            options.data.isReferred = false
          }
          this.fetchData(options);
          // this.setTableRouter(route.query.type);
          // this.thisRouter = route.query.type;
        }*/

      },
    },
    mounted() {
      this.init();
      if (this.$route.query.type) {
        eventBus.$emit('setIsReferred', false);
        this.isReferred = false;
        /*this.thisFetch();*/
      } else {
        this.$router.push('/table')
      }


      /*event register*/
      eventBus.$off('tableQueryData');
      eventBus.$on('tableQueryData', (data) => {
        this.paginationOptions.currentPage = 1;
        this.isReferred = data[1].isReferred;
        this.queryString = data[0];
        this.thisFetch(data[1])
      })

    },
    methods: {
      ...mapActions(['setTableDeleteGroup']),
      init: function () {
        Object.assign(this.$data, this.$options.data())
      },
      sizeChange: function () {
        this.paginationOptions.currentPage = 1;
        this.thisFetch();
      },
      thisFetch: function (params) {
        this.$openLoading();
        let query = this.$route.query;
        let options = {
          url: getRequestUrl(query.type),
          data: {
            table: query.type,
            pageNo: this.paginationOptions.currentPage,
            pageSize: this.paginationOptions.pageSize
          }
        };
        if (params) {
          for (let i in params) {
            options.data[i] = params[i];
          }
        }
        if (!params && this.$route.query.type === 'DataRelativeSheet') {
          options.data.isReferred = false
        }
        this.fetchData(options)
      },
      fetchData: function (options) {
        //加载查询过滤参数
        if (this.queryString !== '') {
          options.data.filter = this.queryString
        }
        options.data.type = this.setDatabase();
        let thisTable = options.data.table;

        //根据不同页面修正参数
        switch (thisTable) {
          case 'Gps_CartonBoxTwenty_Result':
            options.data.descBy = 'Gps_CartonBoxTwenty_Result.TestTime';
            break;
          case 'DataRelativeUnique':
          case 'DataRelativeSheet':
          case 'Gps_AutoTest_Result':
          case 'Gps_AutoTest_Result2':
          case 'Gps_AutoTest_Result3':
          case 'Gps_CoupleTest_Result':
          case 'LTestLogMessage':
            options.data.descBy = 'TestTime';
            break;
          case 'Gps_OperRecord':
            options.data.descBy = 'OperTime';
            break;
          case 'Gps_ManuLdParam':
            options.data.descBy = 'LDTime';
            break;
          case 'Gps_TestResult':
            options.data.descBy = 'RecordTime';
            break;
          case 'Gps_ManuCpParam':
            options.data.descBy = 'CPTIME';
            break;
        }
        if (!this.isPending) {
          this.isPending = true;
          axiosFetch(options).then(response => {
            this.$closeLoading();
            this.isPending = false;
            if (response.data.result === 200) {
              this.tableData = response.data.data.list;
              this.paginationOptions = {
                currentPage: response.data.data.pageNumber,
                pageSize: response.data.data.pageSize,
                total: response.data.data.totalRow
              };
              eventBus.$emit('setTableDataCount', response.data.data.totalRow)
            } else {
              errHandler(response.data.result)
            }
          })
            .catch(err => {
              this.isPending = false;
              console.log(JSON.stringify(err));
              this.$closeLoading();
              this.$alertDanger('请求超时，请刷新重试')
            })
        } else {
          this.$closeLoading()
        }
      },
      selectionChangeHandler: function (val) {
        this.deleteIdGroup = val
      },
      toggleSelectionStatus: function (val) {
        this.$refs.tablecomponent.clearSelection();
      },
      deleteTableData: function () {
        if (this.$route.query.type === 'Gps_CartonBoxTwenty_Result' && this.queryString.indexOf('ZhiDan') === -1) {
          this.$alertInfo("必须经过制单号筛选才可以进行删除操作");
          return;
        }
        if (this.deleteIdGroup.length !== 0) {
          if (!this.isPending) {
            this.isPending = true;
            this.$confirm('确认删除？', '警告!', {
              type: 'warning',
              confirmButtonText: '确定',
              confirmButtonClass: 'warningBtn',
              cancelButtonText: '取消'
            }).then(() => {
              this.$openLoading();
              let options = {url: '', data: {}};
              options.url = tableDeleteByIdsUrl;
              options.data.table = this.$route.query.type;
              options.data.type = this.setDatabase();
              options.data.filter = '';
              let tempArray = [];
              //根据主键删除
              this.deleteIdGroup.forEach((item, index) => {
                tempArray.push(item[this.getTableConfig.primaryKey]);
              });
              let quote = this.getTableConfig.type === "string" ? "'" : "";
              options.data.filter = this.getTableConfig.primaryKey + '#in#(' + quote + tempArray.toString() + quote + ')';
              axiosFetch(options).then(res => {
                if (res.data.result === 200) {
                  this.deleteIdGroup = [];
                  this.pageReload();
                  this.$alertSuccess("成功删除");
                } else {
                  this.$alertWarning(res.data.data)
                }
                this.$closeLoading();
                this.isPending = false
              })
            }).catch(action => {
              this.isPending = false;
            });

          }
        }
      },

      validateDelete: function () {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          this.validateUser().then(() => {
            this.isPending = false;
            this.$closeLoading();
            this.deleteTableData()
          }).catch(err => {
            this.isPending = false;
            this.$closeLoading();
            this.$alertInfo(err);
          })

        }
      },

      validateUser: function () {
        return new Promise((resolve, reject) => {
          if (this.validateInfo.user === "" || this.validateInfo.password === "") {
            reject("填写不完整")
          }
          axiosFetch({
            url: validateUrl,
            data: {
              name: this.validateInfo.user,
              password: this.validateInfo.password
            }
          }).then(async response => {
            if (response.data.result === 200) {
              resolve()
            } else {
              reject(response.data.data)
            }
          }).catch(err => {
            reject(err)
          })
        })
      },


      pageReload: function () {
        let path = this.$route.fullPath;
        this.$router.push('_empty');
        this.$router.replace(path);
      },
      setDatabase: function () {
        if (this.$route.query.type === 'NetMarkIMEI') {
          return 0
        }
      },


      /*关联表 - 根据IMEI1清空指定字段*/
      cleanupItems: function () {
        this.cleanupChoseItems = [];
        this.isRelCleanup = true;
      },

      submitCleanup: function () {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          let imeiGroup = [];
          this.deleteIdGroup.forEach(item => {
            imeiGroup.push(item.IMEI1)
          });

          let options = {
            url: cleanupRelUrl,
            data: {
              imei: imeiGroup.toString(),
              items: this.cleanupChoseItems.toString()
            }
          };

          axiosFetch(options).then(response => {
            this.isPending = false;
            this.$closeLoading();
            if (response.data.result === 200) {
              this.$alertSuccess('修改成功');
              this.isRelCleanup = false;
              this.cleanupChoseItems = [];
              this.thisFetch();
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.isPending = false;
            this.$closeLoading();
            this.$alertInfo('未知错误');
          })
        }
      }
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

  .select-box {
    position: fixed;
    right: 10px;
    bottom: 10px;
    box-shadow: 0 0 5px #dddddd;
    border-radius: 4px;
    width: auto;
    max-width: 420px;
    min-height: 120px;
    height: auto;
    background: #ffffff;
    z-index: 100;
    display: flex;
    flex-direction: column;
    color: #909399;
  }

  .control-bar {
    display: flex;
    justify-content: space-around;
    align-items: center;
    /*width: 100%;*/
    padding: 4px 5px;
  }

  .selected-content {

    padding: 10px 10px;
  }

  .main-details /deep/ .cleanup-dialog .el-checkbox {
    margin: 0 20px 7px 0;
    width: 90px;
  }

</style>
