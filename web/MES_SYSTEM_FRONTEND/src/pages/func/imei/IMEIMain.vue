<template>
  <div class="imei-main">

    <div class="search-component">
      <div class="search-container" style="padding: 10px 20px 20px; ">
        <div style="margin-right: 12px">
          <div class="search-input">
            <label for="search-item-zhidan">制单号:</label>
            <el-input type="text" id="search-item-zhidan" v-model.trim="Zhidan" autocomplete="off" clearable
                      placeholder="请填写制单号"></el-input>
          </div>
        </div>
        <div style="margin-right: 12px">
          <div class="search-btn">
            <el-button type="primary" @click="searchZhidanItem">
              查询
            </el-button>
          </div>
        </div>
      </div>
      <div class="result-main">

        <el-table
          :data="zhidanListData"
          max-height="560"
          stripe>
          <el-table-column v-for="(item, index) in zhidanListColumn"
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
          <el-table-column label="操作" width="80">
            <template slot-scope="scope">
              <el-button type="primary" size="mini" @click="showIMEIQueryDialog(scope.row)">查询</el-button>
            </template>
          </el-table-column>
        </el-table>
      </div>
    </div>
    <el-dialog
      title="查询未使用的IMEI号"
      :visible.sync="imeiQueryVisible"
      @close="clearCache"
      width="90%">
      <div class="search-container">
        <div style="margin-right: 12px">
          <div class="search-input">
            <label for="search-item-imei-begin">IMEI号 起始:</label>
            <el-input type="text" id="search-item-imei-begin" v-model.trim="IMEIBegin" autocomplete="off" clearable
                      placeholder="请填写起始IMEI号" :disabled="queryOptionsLock"></el-input>
          </div>
        </div>
        <div style="margin-right: 12px">
          <div class="search-input">
            <label for="search-item-imei-end">终止:</label>
            <el-input type="text" id="search-item-imei-end" v-model.trim="IMEIEnd" autocomplete="off" clearable
                      placeholder="请填写终止IMEI号" :disabled="queryOptionsLock"></el-input>
          </div>
        </div>
        <div style="margin-right: 12px">
          <div class="search-input">
            <label for="search-item-imei-zhidan">制单号:</label>
            <el-input type="text" id="search-item-imei-zhidan" v-model.trim="ZhiDanInIMEI" autocomplete="off"
                      clearable
                      placeholder="请填写制单号" :disabled="queryOptionsLock"></el-input>
          </div>
        </div>
        <div style="margin-right: 12px">
          <div class="search-btn">
            <el-button type="primary" @click="searchIMEIItem" :disabled="queryOptionsLock">
              查询
            </el-button>
          </div>
        </div>
        <div style="margin-right: 12px" v-if="queryOptionsLock">
          <div class="search-btn">
            <el-button type="primary" @click="unlockQueryOptions">
              解锁
            </el-button>
          </div>
        </div>
        <div style="margin-right: 12px" v-if="queryOptionsLock">
          <div class="search-btn">
            <el-button type="warning" @click="exportResult">
              导出结果
            </el-button>
          </div>
        </div>
      </div>

      <div class="result-main">
        <div class="imei-query-result-remark">
          共{{imeiListData.length}}条数据
        </div>
        <div class="imei-query-result-container">
          <div class="imei-query-result-item" v-for="item in imeiListData">
            <span>{{item}}</span>
          </div>
        </div>
      </div>
    </el-dialog>
  </div>
</template>

<script>
  import {mapGetters, mapActions} from 'vuex'
  import {axiosFetch, downloadFile} from "../../../utils/fetchData";
  import {imeiDownloadUrl, imeiQueryUrl, imeiZhidanQueryUrl} from "../../../config/globalUrl";

  export default {
    name: "IMEIMain",
    data() {
      return {
        ZhiDanInIMEI: '',
        IMEIBegin: '',
        IMEIEnd: '',
        Zhidan: '',
        zhidanListData: [],
        zhidanListColumn: [
          {field: 'ZhiDan', title: '制单号', colStyle: {'width': '120px'}},
          {field: 'IMEIStart', title: '起始IMEI号', colStyle: {'width': '150px'}},
          {field: 'IMEIEnd', title: '终止IMEI号', colStyle: {'width': '150px'}},
        ],
        imeiListData: [],
        isPending: false,
        imeiQueryVisible: false,
        isDownloading: false,
      }
    },
    computed: {
      queryOptionsLock: function () {
        return this.imeiListData.length > 0
      }
    },
    mounted() {

    },
    methods: {
      ...mapActions(['setLoading']),
      initData: function () {
        Object.assign(this.$data, this.$options.data())
      },

      checkFormat: function () {
        return new Promise((resolve, reject) => {
          if (this.ZhiDanInIMEI === '' || this.IMEIBegin === '' || this.IMEIEnd === '') {
            reject('查询条件填写不完整')
          }
          let regx = new RegExp("^[1-9]\\d*$");
          if (!regx.test(this.IMEIBegin) || !regx.test(this.IMEIEnd)) {
            reject('IMEI格式不正确')
          }
          if (this.IMEIEnd - this.IMEIBegin < 0) {
            reject('IMEI范围有误')
          }
          resolve();
        })
      },

      searchZhidanItem: function () {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          let options = {
            url: imeiZhidanQueryUrl,
            data: {
              zhiDan: this.Zhidan
            }
          };
          axiosFetch(options).then(response => {
            this.isPending = false;
            this.$closeLoading();
            if (response.data.result === 200) {
              this.zhidanListData = response.data.data.list
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.isPending = false;
            this.$closeLoading();
            this.$alertDanger('未知错误')
          })
        }
      },
      searchIMEIItem: function () {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          this.checkFormat().then(() => {
            let options = {
              url: imeiQueryUrl,
              data: {
                zhiDan: this.ZhiDanInIMEI,
                startIMEI: this.IMEIBegin,
                endIMEI: this.IMEIEnd
              }
            };
            axiosFetch(options).then(response => {
              this.isPending = false;
              this.$closeLoading();
              if (response.data.result === 200) {
                this.imeiListData = response.data.data;
                if (response.data.data.length === 0) {
                  this.$alertInfo("没有未使用的IMEI号")
                }
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
            this.$alertInfo(err);
          })
        }
      },
      showIMEIQueryDialog: function (row) {
        this.ZhiDanInIMEI = row.ZhiDan;
        this.IMEIBegin = row.IMEIStart;
        this.IMEIEnd = row.IMEIEnd;
        this.imeiQueryVisible = true;
      },
      exportResult: function () {

        if (this.isDownloading === false) {
          this.isDownloading = true;
          let data = {
            startIMEI: this.IMEIBegin,
            endIMEI: this.IMEIEnd,
            zhiDan: this.ZhiDanInIMEI,
            '#TOKEN#': this.$store.state.token
          };
          downloadFile(imeiDownloadUrl, data);
          let count = 0;
          let mark = setInterval(() => {
            count++;
            if (count > 9) {
              count = 0;
              clearInterval(mark);
              this.isDownloading = false
            }
          }, 1000);
          this.$alertSuccess('请求成功，请等待下载');
        } else {
          this.$alertInfo('请稍后再试')
        }
      },
      unlockQueryOptions: function () {
        this.imeiListData = [];
      },
      clearCache: function () {
        this.IMEIBegin = this.IMEIEnd = this.ZhiDanInIMEI = '';
        this.imeiListData = [];
      }
    }
  }
</script>

<style scoped>

  .search-component {
    width: 100%;
    height: 100%;
    min-height: 120px;
    border-radius: 5px;
  }

  .result-main {
    background-color: #ffffff;
    border: 1px solid #eeeeee;
    border-radius: 5px;
    padding: 10px;
    margin-top: 10px;
  }

  .search-container {
    display: flex;
    align-items: flex-end;
    flex-wrap: wrap;
    margin-bottom: 10px;
    background-color: #ffffff;
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

  .imei-query-result-container {
    display: flex;
    flex-wrap: wrap;
    width: 100%;
  }

  .imei-query-result-item {
    width: 120px;
    margin: 5px 10px;
  }
  .imei-query-result-remark {
    font-size: 15px;
    line-height: 30px;
    color: #909399;
    padding: 20px;
  }
</style>
