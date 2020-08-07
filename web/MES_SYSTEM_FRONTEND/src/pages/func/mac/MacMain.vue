<template>
  <div class="mac-main">
    <div class="search-component">
      <div class="search-container" style="padding: 10px 20px 30px; ">
        <div style="margin-right: 12px" v-if="!searchType">
          <div class="search-input">
            <label for="search-item-sn">SN号:</label>
            <el-input size="small" type="text" id="search-item-sn" v-model.trim="searchSN" autocomplete="off" clearable
                      placeholder="请填写SN号" :disabled="queryOptionsLock"></el-input>
          </div>
        </div>
        <div style="margin-right: 12px" v-if="!searchType">
          <div class="search-input">
            <label for="search-item-version">软件版本:</label>
            <el-input size="small" type="text" id="search-item-version" v-model.trim="searchVersion" autocomplete="off" clearable
                      placeholder="请填写版本号" :disabled="queryOptionsLock"></el-input>
          </div>
        </div>
        <div style="margin-right: 12px" v-if="!searchType">
          <div class="search-input">
            <label for="search-item-softmodel">机型:</label>
            <el-input size="small" type="text" id="search-item-softmodel" v-model.trim="searchSoftModel" autocomplete="off" clearable
                      placeholder="请填写软件版本" :disabled="queryOptionsLock"></el-input>
          </div>
        </div>
        <div style="margin-right: 12px" v-if="searchType">
          <div class="search-input">
            <label for="search-item-mac">MAC地址:</label>
            <el-input size="small" type="text" id="search-item-mac" v-model.trim="searchMac" autocomplete="off" clearable
                      placeholder="请填写MAC地址" :disabled="queryOptionsLock"></el-input>
          </div>
        </div>
        <div style="margin-right: 12px">
          <div class="search-btn">
            <el-button size="small" type="primary" @click="searchItem" :disabled="queryOptionsLock">
              查询
            </el-button>
          </div>
        </div>
        <div style="margin-right: 12px">
          <div class="search-btn">
            <el-button size="small" type="primary" @click="showImportDialog" :disabled="queryOptionsLock">
              批量查询
            </el-button>
          </div>
        </div>
        <div style="margin-right: 12px" v-if="queryOptionsLock">
          <div class="search-btn">
            <el-button size="small" type="primary" @click="unlockQueryOptions">
              解锁
            </el-button>
          </div>
        </div>
<!--        <div style="margin-right: 12px">-->
<!--          <div class="search-btn">-->
<!--            <el-button size="small" :disabled="queryOptionsLock" type="primary" @click="switchSearchType">-->
<!--              切换条件-->
<!--            </el-button>-->
<!--          </div>-->
<!--        </div>-->
        <div style="margin-right: 12px">
          <div class="search-btn">
            <el-button size="small" type="warning" @click="showReworkDialog">
              执行返工
            </el-button>
          </div>
        </div>
      </div>
    </div>
    <div class="result-main">
      <div class="result-tag">
        <div class="result-tag-title">
<!--          <span>Gps_TestResult:</span>-->
          <span>总表:</span>
        </div>
        <div class="result-tag-details">
          <el-table
            :data="resultTableData"
            max-height="560"
            stripe>
            <el-table-column v-for="(item, index) in resultTableColumn"
                             :key="index"
                             :prop=item.field
                             :label="item.title"
                             :min-width=item.colStyle.width
                             :formatter=item.formatter>
            </el-table-column>

            <el-table-column
              type="index"
              fixed="left"
              width="60">
            </el-table-column>
          </el-table>
        </div>
      </div>
      <div class="result-tag">
        <div class="result-tag-title">
          <span>SMT功能表:</span>
        </div>
        <div class="result-tag-details">
          <el-table
            :data="result2TableData"
            max-height="560"
            stripe>
            <el-table-column v-for="(item, index) in result2TableColumn"
                             :key="index"
                             :prop=item.field
                             :label="item.title"
                             :min-width=item.colStyle.width
                             :formatter=item.formatter>
            </el-table-column>
          </el-table>
        </div>
      </div>
      <div class="result-tag">
        <div class="result-tag-title">
          <span>数据防重表:</span>
        </div>
        <div class="result-tag-details">
          <el-table
            :data="antiDupTableData"
            max-height="560"
            stripe>
            <el-table-column v-for="(item, index) in antiDupTableColumn"
                             :key="index"
                             :prop=item.field
                             :label="item.title"
                             :min-width=item.colStyle.width
                             :formatter=item.formatter>
            </el-table-column>

            <el-table-column
              type="index"
              fixed="left"
              width="60">
            </el-table-column>
          </el-table>
        </div>
      </div>
    </div>
    <el-dialog
      title="请校验用户名与密码"
      :visible.sync="validateVisible"
      width="400px">

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
        <el-button size="small" @click="validateVisible = false">取 消</el-button>
        <el-button size="small" type="primary" @click="executeRework">校 验</el-button>
      </span>
    </el-dialog>
    <el-dialog
      title="批量导入"
      :visible.sync="importVisible"
      width="30%">
      <el-input
        type="textarea"
        :rows="6"
        placeholder="请输入查询数据，每行一条，多条件使用半角逗号分隔
例:
SN号,版本号,软件版本
xxxxx,xxxxx,xxxxxx"
        v-model="importVal"></el-input>
      <span slot="footer">
        <el-button size="small" @click="importVisible = false">取 消</el-button>
        <el-button size="small" type="primary" @click="importSearch">查 询</el-button>
      </span>
    </el-dialog>
  </div>
</template>

<script>
  import {axiosFetch} from "../../../utils/fetchData";
  import {macReworkQueryUrl, setMacRework, validateUrl} from "../../../config/globalUrl";
  import {AntiDupColumn, AutoTestResultColumn, TestResultColumn} from "../../../config/funcConfig";

  export default {
    name: "MacMain",
    data() {
      return {
        searchSN: '',
        searchSoftModel: '',
        searchVersion: '',
        searchMac: '',
        antiDupTableData: [],
        resultTableData: [],
        result2TableData: [],

        /*false sn+version+softmodel  true mac*/
        searchType: false,
        isPending: false,
        validateVisible: false,
        validateInfo: {
          user: '',
          password: ''
        },
        importVisible: false,
        importVal: ''
      }
    },
    watch: {
      $route: function (val) {
        this.initData()
      }
    },
    computed: {
      queryOptionsLock: function () {
        return this.antiDupTableData.length !== 0 && this.result2TableData.length !== 0 && this.resultTableData.length !== 0
      },
      antiDupTableColumn: function () {
        return AntiDupColumn();
      },
      resultTableColumn: function () {
        return TestResultColumn();
      },
      result2TableColumn: function () {
        return AutoTestResultColumn();
      },
    },
    mounted() {

    },
    methods: {
      initData: function () {
        Object.assign(this.$data, this.$options.data())
      },
      searchItem: function () {
        if (!this.isPending) {
          this.$openLoading();
          this.isPending = true;
          let options = {
            url: macReworkQueryUrl,
            data: {}
          };
          if (!this.searchType) {
            if (this.searchSN !== "" && this.searchSoftModel !== "" && this.searchVersion !== "") {
              options.data = {
                sn: this.searchSN,
                version: this.searchVersion,
                softModel: this.searchSoftModel
              }
            } else {
              this.$closeLoading();
              this.isPending = false;
              this.$alertInfo("请填写完整的查询条件");
              return
            }
          } else {
            if (this.searchMac !== "") {
              options.data = {
                mac: this.searchMac
              }
            } else {
              this.$closeLoading();
              this.isPending = false;
              this.$alertInfo("请填写完整的查询条件");
              return
            }
          }
          axiosFetch(options).then(response => {
              if (response.data.result === 200) {
                this.antiDupTableData = response.data.data['gps_AutoTest_AntiDup'];
                this.resultTableData = response.data.data['gps_TestResult'];
                this.result2TableData = response.data.data['gps_AutoTest_Result2'];
              } else {
                this.$alertWarning(response.data.data)
              }
            }
          ).finally(() => {
            this.$closeLoading();
            this.isPending = false;
          })
        }

      },

      executeRework: async function () {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          this.validateUser().then(() => {
            let options = {
              url: setMacRework,
              data: {}
            };
            if (!this.searchType) {
              options.data = {
                sn: this.searchSN,
                version: this.searchVersion,
                softModel: this.searchSoftModel
              }
            } else {
              options.data = {
                mac: this.searchMac
              }
            }
            axiosFetch(options).then(response => {
              this.isPending = false;
              this.$closeLoading();
              if (response.data.result === 200) {
                this.$alertSuccess('修改成功');
                this.pageReload();
              } else {
                this.$alertWarning(response.data.data)
              }
            }).catch(err => {
              this.isPending = false;
              this.$closeLoading();
              this.$alertDanger('未知错误')
            })
          }).catch(err => {
            this.isPending = false;
            this.$closeLoading();
            this.$alertInfo(err)
          })
        }
      },

      switchSearchType: function () {
        this.searchSN = this.searchSoftModel = this.searchVersion = this.searchMac = "";
        this.searchType = !this.searchType;
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
      /*返工确认窗口*/
      showReworkDialog: function () {
        if (!this.queryOptionsLock) {
          this.$alertInfo('请先查询数据');
          return;
        }
        this.validateVisible = true;
      },
      /*显示导入窗口*/
      showImportDialog: function () {
        this.importVal = "";
        this.importVisible = true;
      },

      importSearch: function () {
        let rows = this.importVal.split(/[\n]/);
        if (this.searchType) {
          if (rows.split(',').length !== 1) {
            this.$alertInfo('数据格式有误');
            return;
          }
          this.searchMac = rows.toString();
        } else {
          let mark = false;
          let colSN = [], colVersion = [], colSoftModel = [];
          rows.forEach(item => {
            let col = item.split(',');
            if (col.length !== 3) {
              mark = true;
              return
            }
            colSN.push(col[0]);
            colVersion.push(col[1]);
            colSoftModel.push(col[2]);
          });
          if (mark) {
            this.$alertInfo('数据格式有误');
            return;
          }
          this.searchSN = colSN.toString();
          this.searchSoftModel = colSoftModel.toString();
          this.searchVersion = colVersion.toString();

          this.searchItem();
        }
      },
      unlockQueryOptions: function () {
        this.initData();
      },
      pageReload: function () {
        let path = this.$route.path;
        this.$router.push('/_empty');
        this.$router.replace(path);
      }
    }
  }
</script>

<style scoped>

  .search-component {
    background-color: #ffffff;
    width: 100%;
    height: 100%;
    min-height: 100px;
    border: 1px solid #eeeeee;
    border-radius: 5px;
  }

  .result-main {
    border: 1px solid #eeeeee;
    border-radius: 5px;
    margin-top: 10px;
    background: #fafafa;
    min-height: 300px;
    max-height: 600px;
    overflow-y: auto;
  }

  .search-container {
    display: flex;
    align-items: flex-end;
    flex-wrap: wrap;
  }

  .search-input {
    width: 240px;
  }

  .search-input label {
    line-height: 30px;
  }

  .search-btn {
    width: 100px;
    margin-top: 12px;
  }

  .search-btn .el-button {
    width: 100%;
  }

  .result-tag {
    margin: 20px;
  }

  .result-tag-title {
    border-radius: 5px 5px 0 0;
    border: 1px solid #eeeeee;
    border-bottom: none;
    width: 80px;
    padding: 5px 3px;
    line-height: 15px;
    font-size: 13px;
    color: #909399;
    background: #ffffff;
  }

  .result-tag-details {
    border-radius: 0 5px 5px 5px;
    border: 1px solid #eeeeee;
    min-height: 100px;
    background: #ffffff;
  }

  .result-tag-details /deep/ .el-table th {
    padding: 5px 0;
    font-weight: 500;
  }
</style>
