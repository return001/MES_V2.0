<template>
  <div class="table-module-sp">
    <div class="options-area">
      <div class="form-row">
        <div class="form-group" v-if="$route.query.type === 'Gps_ManuPrintParam'">
          <label for="zhidan">制单号</label>
          <el-input type="text" class="form-control" id="zhidan" v-model="queryOptions.zhiDan"
                    autocomplete="off"></el-input>
        </div>
        <div class="form-group" v-if="$route.query.type === 'Gps_ManuSimDataParam'">
          <label for="rID">基带ID</label>
          <el-input type="text" class="form-control" id="rID" v-model="queryOptions.rID" autocomplete="off"></el-input>
        </div>
        <div class="form-group">
          <label for="startIMEI">IMEI号 从：</label>
          <el-input type="text" class="form-control" id="startIMEI" v-model="queryOptions.startIMEI"
                    autocomplete="off"></el-input>
        </div>
        <div class="form-group">
          <label for="endIMEI">至：</label>
          <el-input type="text" class="form-control" id="endIMEI" v-model="queryOptions.endIMEI"
                    autocomplete="off"></el-input>
        </div>
        <div style="width: 160px; margin-right: 10px">
          <el-checkbox v-model="imeiIsHex">IMEI是否为16进制</el-checkbox>
        </div>
        <div class="row no-gutters">
          <div class="row">
            <!--<div class="form-group col pr-1">-->
            <!--<label>时间范围 从：</label>-->
            <!--<datetime v-model="queryOptions.startTime" type="datetime"/>-->
            <!--</div>-->
            <!--<div class="form-group col pr-3">-->
            <!--<label>至：</label>-->
            <!--<datetime v-model="queryOptions.endTime" type="datetime"/>-->
            <!--</div>-->
            <div class="form-group">
              <label>时间范围</label>
              <el-date-picker
                size="large"
                v-model="timeRange"
                type="datetimerange"
                :picker-options="pickerOptions"
                range-separator="-"
                prefix-icon="el-icon-date"
                start-placeholder="开始日期"
                end-placeholder="结束日期"
                value-format="yyyy-MM-dd HH:mm:ss">
              </el-date-picker>
            </div>
          </div>
        </div>
        <div class="radio-box"
             style="margin-right: 14px"
             v-if="$route.query.type === 'Gps_ManuPrintParam' && (!!timeRange)">
          <label>打印类型</label>
          <div class="radio-content">
            <input type="radio" id="ch-print-radio" value="0" v-model="radioChecked">
            <label for="ch-print-radio">彩盒打印</label>
          </div>
          <div class="radio-content">
            <input type="radio" id="js-print-radio" value="1" v-model="radioChecked">
            <label for="js-print-radio">机身打印</label>
          </div>
        </div>
        <div class="form-group-btn">
          <el-button type="info" @click="initOptions()">清空条件</el-button>
        </div>
        <div class="form-group-btn">
          <el-button type="primary" @click="thisFetch('query')">查询</el-button>
        </div>
        <div class="form-group-btn"  v-if="paginationOptions.total > 0">
          <el-button type="primary" @click="downloadData">下载报表</el-button>
        </div>
        <div class="form-group-btn" v-if="checkDelPermission && paginationOptions.total > 0">
          <!--<el-button type="warning" @click="deleteAll">删除所有</el-button>-->
          <el-button type="warning" @click="callValidate('all')">删除所有</el-button>
        </div>
      </div>
    </div>
    <div class="main-details mt-1 mb-3">
      <el-table
        :data="tableData"
        max-height="560"
        stripe
        ref="tablespcomponent"
        @selection-change="selectionChangeHandler">
        <el-table-column
          type="selection"
          width="30"
          v-if="checkDelPermission">
        </el-table-column>
        <el-table-column v-for="(item, index) in tableColumns"
                         :key="index"
                         :prop=item.field
                         :label=item.title
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
        @size-change="thisFetch('sizeChange')"
        class="page-pagination">
      </el-pagination>
      <div class="select-box" v-show="deleteIdGroup.length !== 0">
        <div class="selected-content"><p>已选择主标识为 <span style="display: inline-block; font-weight: bold;"
                                                      v-for="(item, index) in deleteIdGroup">{{item.ID}}{{index !== deleteIdGroup.length - 1 ? ',' : ''}}</span>
          的项目</p></div>
        <div class="control-bar">
          <el-button type="info" size="mini" @click="toggleSelectionStatus">取消选中</el-button>
          <!--<el-button type="warning" size="mini" @click="deleteTableData">删除所选项</el-button>-->
          <el-button type="warning" size="mini" @click="callValidate('selected')">删除所选项</el-button>
        </div>
      </div>
    </div>
    <!--删除时用户校验-->
    <el-dialog
      title="请校验用户名与密码"
      :visible.sync="validateVisible"
      width="400px"
      :close-on-click-modal="false"
      :close-on-press-escape="false">

      <div class="">
        <label for="validate-user" style="line-height: 30px">用户名</label>
        <el-input type="text" id="validate-user" v-model.trim="validateInfo.user" autocomplete="off" clearable
                  placeholder="请填写用户名"></el-input>
      </div>
      <div class="">
        <label for="validate-pwd" style="line-height: 30px">密码</label>
        <el-input type="password" id="validate-pwd" v-model.trim="validateInfo.password" autocomplete="off" clearable
                  placeholder="请填写密码"></el-input>
      </div>
      <span slot="footer">
        <el-button @click="validateVisible = false">取 消</el-button>
        <el-button type="primary" @click="validateDelete">校 验</el-button>
      </span>
    </el-dialog>
  </div>
</template>

<script>
  import {Settings} from 'luxon'
  import {Datetime} from 'vue-datetime'
  import 'vue-datetime/dist/vue-datetime.css'
  import {
    tablePrintDeleteUrl,
    tableSimDeleteUrl,
    tablePrintDownloadUrl,
    tableSimDownloadUrl,
    tableDeleteByIdsUrl,
    validateUrl
  } from "../../../config/globalUrl";
  import {getRequestUrl, setRouterConfigSP} from "../../../config/tableApiConfig";
  import {axiosFetch, axiosDownload} from "../../../utils/fetchData";
  import {checkDelPermission} from "../../../utils/utils";
  import {mapActions} from 'vuex'
  import {saveAs} from 'file-saver'

  export default {
    name: "SimTableModule",
    components: {
      datetime: Datetime
    },
    data() {
      return {
        queryOptions: {},
        radioChecked: '0',
        paginationOptions: {
          currentPage: 1,
          pageSize: 20,
          total: 0
        },
        tableData: [],
        isPending: false,
        timeRange: '',
        deleteIdGroup: [],
        pickerOptions: {
          shortcuts: [{
            text: '最近一天',
            onClick(picker) {
              const end = new Date();
              const start = new Date();
              start.setTime(start.getTime() - 3600 * 1000 * 24);
              picker.$emit('pick', [start, end]);
            }
          }, {
            text: '最近三天',
            onClick(picker) {
              const end = new Date();
              const start = new Date();
              start.setTime(start.getTime() - 3600 * 1000 * 24 * 3);
              picker.$emit('pick', [start, end]);
            }
          }, {
            text: '最近一周',
            onClick(picker) {
              const end = new Date();
              const start = new Date();
              start.setTime(start.getTime() - 3600 * 1000 * 24 * 7);
              picker.$emit('pick', [start, end]);
            }
          }]
        },
        validateVisible: false,
        validateInfo: {
          user: '',
          password: ''
        },
        deleteType: '',//all selected
        imeiIsHex: false
      }
    },
    mounted() {
      Settings.defaultLocale = 'zh-CN';
      // this.fetchData();
    },
    watch: {
      $route: function (route) {
        this.init();
        // this.fetchData();
      }
    },
    computed: {
      tableColumns: function () {
        return setRouterConfigSP(this.$route.query.type).data.dataColumns;
      },
      checkDelPermission: function () {
        return checkDelPermission(this.$route.query.type)
      }
    },
    methods: {
      ...mapActions(['setLoading']),
      init: function () {
        Object.assign(this.$data, this.$options.data())
      },
      initOptions: function () {
        this.queryOptions = JSON.parse(JSON.stringify(this.$options.data().queryOptions));
        this.timeRange = ""
      },
      checkFormat: function () {
        return new Promise((resolve, reject) => {
          let options = this.queryOptions;

          //当时间存在但不完整
          // if (!options.startTime ^ !options.endTime) {
          //   reject('请检查时间完整性')
          // }
          //当IMEI范围存在但不完整
          if (!options.startIMEI ^ !options.endIMEI) {
            reject('请检查IMEI范围')
          }

          //时间范围错误
          // if (options.startTime && options.endTime) {
          //   let tempFrom = options.startTime.replace('T', ' ').replace('Z', '');
          //   let tempTo = options.endTime.replace('T', ' ').replace('Z', '');
          //   if (this.compareDate(tempFrom, tempTo) < 0) {
          //     reject('请检查日期范围')
          //   } else {
          //     //修正格式
          //     options.startTime = options.startTime.replace('T', ' ').replace('Z', '');
          //     options.endTime = options.endTime.replace('T', ' ').replace('Z', '');
          //   }
          // }

          resolve();
        })
      },
      thisFetch: function (opt) {
        if (opt === 'sizeChange' || opt === 'query') {
          this.paginationOptions.currentPage = 1;
        }

        this.$openLoading();
        this.checkFormat().then(data => {
          this.fetchData();
        }).catch(err => {
          console.log(err);
          this.$alertInfo(err);
          this.$closeLoading();
        })
      },
      fetchData: function () {
        if (!this.isPending) {
          this.isPending = true;
          let options = {
            url: getRequestUrl(this.$route.query.type),
            data: JSON.parse(JSON.stringify(this.queryOptions))
          };
          options.data.pageNo = this.paginationOptions.currentPage;
          options.data.pageSize = this.paginationOptions.pageSize;
          options.data.isIMEIHex = this.imeiIsHex;

          if (!!this.timeRange) {
            options.data.startTime = this.timeRange[0];
            options.data.endTime = this.timeRange[1];
          }

          if (this.$route.query.type === 'Gps_ManuPrintParam') {
            options.data.printType = this.radioChecked;
          }


          axiosFetch(options).then(res => {
            if (res.data.result === 200) {
              this.tableData = res.data.data.list;
              this.paginationOptions = {
                currentPage: res.data.data.pageNumber,
                pageSize: res.data.data.pageSize,
                total: res.data.data.totalRow
              }
            } else {
              this.$alertWarning(res.data.data)
            }
            this.isPending = false;
            this.$closeLoading();
          }).catch(err => {
            console.log(JSON.stringify(err));
            this.isPending = false;
            this.$closeLoading();
            this.$alertDanger('请求超时，请刷新重试')
          })
        }
      },
      compareDate: function (dateFrom, dateTo) {
        let compFrom = new Date(dateFrom);
        let compTo = new Date(dateTo);
        return (compTo - compFrom);
      },
      downloadData: function () {
        if (!this.isPending) {
          if (this.paginationOptions.total > 2000) {
            this.$alertWarning("所选数据过多(>2000)，无法生成报表");
            return;
          }
          this.isPending = true;
          this.$openLoading();
          let thisTable = this.$route.query.type;
          let data = JSON.parse(JSON.stringify(this.queryOptions));
          data.table = thisTable;
          data.isIMEIHex = this.imeiIsHex;
          if (!!this.timeRange) {
            data.startTime = this.timeRange[0];
            data.endTime = this.timeRange[1];
          }
          let url;
          if (thisTable === 'Gps_ManuSimDataParam') {
            url = tableSimDownloadUrl
          } else if (thisTable === 'Gps_ManuPrintParam') {
            url = tablePrintDownloadUrl;
            data.printType = this.radioChecked;
          }
          axiosDownload({
            url: url,
            data: data
          }).then(response => {
            let contentType = response.request.getResponseHeader('content-type');
            if (contentType === 'application/vnd.ms-excel') {
              let name = response.request.getResponseHeader('Content-Disposition').split('=')[1];
              saveAs(response.data, name)
            } else {
              let reader = new FileReader();
              reader.readAsText(response.data);
              reader.addEventListener('loadend', () => {
                this.$alertWarning(JSON.parse(reader.result).data)
              })
            }
          }).catch(err => {
            this.$alertDanger('请求超时，请刷新重试')
          }).finally(() => {
            this.isPending = false;
            this.$closeLoading();
          })
        }

      },
      selectionChangeHandler: function (val) {
        this.deleteIdGroup = val
      },
      toggleSelectionStatus: function (val) {
        this.$refs.tablespcomponent.clearSelection();
      },
      deleteTableData: function () {
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
              options.data.isIMEIHex = this.imeiIsHex;
              options.data.table = this.$route.query.type;
              options.data.filter = '';
              let tempArray = [];
              this.deleteIdGroup.forEach((item, index) => {
                tempArray.push(item.ID);
              });
              options.data.filter = 'id#in#(' + tempArray.toString() + ')';
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
              }).catch(err => {
                console.log(JSON.stringify(err));
                this.isPending = false;
                this.$closeLoading();
                this.$alertDanger('请求超时，请刷新重试')
              })
            }).catch(action => {
              this.isPending = false;
            });

          }
        }
      },
      deleteAll: function () {
        if (!this.isPending) {
          this.isPending = true;
          this.$confirm('该操作将删除查询所得的所有项目，确认删除？', '警告!', {
            type: 'warning',
            confirmButtonText: '确定',
            confirmButtonClass: 'warningBtn',
            cancelButtonText: '取消'
          }).then(() => {
            this.$openLoading();
            let options = {url: '', data: {}};
            let thisTable = this.$route.query.type;
            if (thisTable === 'Gps_ManuSimDataParam') {
              options.url = tableSimDeleteUrl
            } else if (thisTable === 'Gps_ManuPrintParam') {
              options.url = tablePrintDeleteUrl
            }
            options.data.isIMEIHex = this.imeiIsHex;
            options.data = JSON.parse(JSON.stringify(this.queryOptions));
            if (!!this.timeRange) {
              options.data.startTime = this.timeRange[0];
              options.data.endTime = this.timeRange[1];
            }
            if (this.$route.query.type === 'Gps_ManuPrintParam') {
              options.data.printType = this.radioChecked;
            }
            axiosFetch(options).then(res => {
              if (res.data.result === 200) {
                this.pageReload();
                this.$alertSuccess("成功删除");
              } else {
                this.$alertWarning(res.data.data)
              }
              this.$closeLoading();
              this.isPending = false
            }).catch(err => {
              console.log(JSON.stringify(err));
              this.isPending = false;
              this.$closeLoading();
              this.$alertDanger('请求超时，请刷新重试')
            })
          }).catch(action => {
            this.isPending = false;
          });

        }
      },

      callValidate: function (type) {
        this.deleteType = type;
        this.validateVisible = true;
      },
      validateDelete: function () {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          this.validateUser().then(() => {
            this.isPending = false;
            this.$closeLoading();
            if (this.deleteType === 'all') {
              this.deleteAll()
            } else if (this.deleteType === 'selected') {
              this.deleteTableData()
            }
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
    }

  }
</script>

<style scoped>
  .table-module-sp {
    padding: 10px 20px 0 0;
  }

  .options-area {
    background: #fff;
    border: 1px solid #eeeeee;
    border-radius: 8px;
    padding: 10px;
    margin-bottom: 10px;
  }

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

  .radio-box {
    display: flex;
    flex-direction: column;
  }

  .radio-content {
    margin-left: 0.25rem;
    font-size: 12px;
    line-height: 12px;
    height: 20px;
  }

  .radio-content input {
    vertical-align: -15%;
  }

  .options-area /deep/ .el-input__inner {
    height: 38px;
    line-height: 38px;
    border: 1px solid #ced4da;
    display: flex;
  }

  .select-box {
    position: fixed;
    right: 10px;
    bottom: 10px;
    box-shadow: 0 0 5px #dddddd;
    border-radius: 4px;
    width: 300px;
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
    width: 100%;
    padding: 4px 5px;
  }

  .selected-content {

    padding: 10px 10px;
  }

  .form-row {
    display: flex;
    flex-wrap: wrap;
    align-items: flex-end;
  }

  .form-group {
    min-width: 200px;
    margin-right: 20px;
  }

  .form-group .el-input {
    width: 220px;
  }

  .form-group /deep/ label {
    line-height: 30px;
    display: block;
  }

  .form-group-btn {
    width: 120px;
    margin-right: 20px;
  }

  .form-group-btn .el-button {
    width: 100%;
    margin-top: 10px;
  }

</style>
