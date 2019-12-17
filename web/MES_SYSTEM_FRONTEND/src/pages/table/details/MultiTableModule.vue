<template>
  <div class="multi-table-module">
    <div class="options-area">
      <div class="form-row">
        <div class="form-group word-selector">
          <el-select size="small" name="选择所需查询字段" v-model="queryWord" @change="queryMode = 0" :disabled="isLocked">
            <el-option value="imei" label="IMEI号"></el-option>
            <el-option value="sn" label="SN号"></el-option>
            <el-option value="zhiDan" label="制单号"></el-option>
          </el-select>
          <div>
            <el-radio v-model="queryMode" :label="0" :disabled="isLocked">单项</el-radio>
            <el-radio v-model="queryMode" :label="1" :disabled="isLocked">散列</el-radio>
            <el-radio v-model="queryMode" :label="2" v-if="queryWord === 'imei'" :disabled="isLocked">连续</el-radio>
          </div>
        </div>
        <div class="form-group" v-if="queryMode === 0">
          <label class="form-label" for="single-item">请填写{{searchingItem}}:</label>
          <el-input size="small" id="single-item" v-model="singleItemValue" clearable :disabled="isLocked"></el-input>
        </div>
        <div class="form-group-btn" v-if="queryMode === 1">
          <el-button size="small" type="primary" @click="hashImportShowing = true">导入查询</el-button>
        </div>
        <div class="form-group-continuous" v-if="queryMode === 2">
          <div class="form-group">
            <label class="form-label" for="continuous-items-1">请填写{{searchingItem}} 从:</label>
            <el-input size="small" id="continuous-items-1" v-model="continuousValue[0]" clearable :disabled="isLocked"></el-input>
          </div>
          <div class="form-group">
            <label class="form-label" for="continuous-items-2">至:</label>
            <el-input size="small" id="continuous-items-2" v-model="continuousValue[1]" clearable :disabled="isLocked"></el-input>
          </div>
        </div>
        <div style="width: 160px; margin-right: 10px" v-if="queryMode === 2">
          <el-checkbox size="small" v-model="imeiIsHex">IMEI是否为16进制</el-checkbox>
        </div>
        <div class="form-group-btn" v-if="queryMode !== 1">
          <el-button size="small" type="primary" @click="queryData" :disabled="isLocked">查 询</el-button>
        </div>
        <div class="form-group-btn" v-if="isLocked">
          <el-button size="small" type="primary" @click="initData">解 锁</el-button>
        </div>
        <div class="form-group-btn" v-if="isLocked">
          <el-button size="small" type="primary" @click="exportTable">导出报表</el-button>
        </div>
        <div class="form-group-btn"
             v-if="isLocked && ($store.state.userType === 'SuperAdmin' || $store.state.userType === 'engineer')">
          <el-button size="small" type="warning" @click="showValidateDialog">删除数据</el-button>
        </div>
      </div>
    </div>
    <div class="result-area">
      <div class="result-tag" v-for="(item, index) in MultiTableConfig">
        <div class="result-tag-title">
          <span>{{item.title}}</span>
        </div>
        <div class="result-tag-details" v-if="tableData[item.title].length > 0">
          <el-table
            :data="tableData[item.title]"
            max-height="380"
            stripe>
            <el-table-column v-for="(subItem, subIndex) in item.columns"
                             :key="subIndex"
                             :prop=subItem.field
                             :label="subItem.title"
                             :min-width=subItem.colStyle.width
                             :formatter=subItem.formatter>
            </el-table-column>
          </el-table>
        </div>
        <div class="result-tag-details-none" v-else>
          <span>无数据</span>
        </div>
      </div>
    </div>

    <!--component-->
    <!--导入-->
    <el-dialog :visible.sync="hashImportShowing" title="批量导入" width="400px">
      <el-input type="textarea" row="4" v-model="hashItemValue"
                :placeholder="'请输入多个' + searchingItem + '(值与值直接使用半角逗号分隔)'" clearable></el-input>
      <span slot="footer" class="dialog-footer">
        <el-button type="primary" @click="queryData" :disabled="isLocked">查 询</el-button>
      </span>
    </el-dialog>

    <!--删除时用户校验及表选择-->
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
      <el-dialog
        width="500px"
        title="请选择需要删除的表"
        :visible.sync="deleteTableSubmitVisible"
        append-to-body
        :close-on-click-modal="false"
        :close-on-press-escape="false">
        <div class="checkbox-item-box">
          <div class="checkbox-item" v-for="(item, key) in deleteTableList">
            <el-checkbox :true-label="1" :false-label="0" v-model="deleteTableGroup[key]">
              {{item.remark}}({{item.name}})
            </el-checkbox>
          </div>
        </div>

        <span slot="footer">
          <el-button @click="deleteTableSubmitVisible = false">取 消</el-button>
          <el-button type="primary" @click="deleteData">删 除</el-button>
        </span>
      </el-dialog>
    </el-dialog>
  </div>
</template>

<script>
  import {multiTableDeleteUrl, multiTableQueryUrl, validateUrl, multiTableDownloadUrl} from "../../../config/globalUrl";
  import {axiosFetch, axiosDownload} from "../../../utils/fetchData";
  import {MultiTableConfig} from "../../../config/multiTableConfig";

  export default {
    name: "MultiTableModule",
    data() {
      return {
        //查询词缀
        queryWord: 'imei',
        //查询模式 0:单项 1:散列 2:连续(imei only)
        queryMode: 0,
        //单项查询绑定值
        singleItemValue: '',
        //散列查询绑定值
        hashItemValue: '',
        hashImportShowing: false,
        //连续查询绑定值
        continuousValue: ['', ''],
        imeiIsHex: false,

        tableData: {
          "Gps_AutoTest_Result": [],
          "DataRelativeSheet": [],
          "DataRelativeUpdate": [],
          "NetMarkIMEI": [],
          "Gps_TestResult": [],
          "Gps_CoupleTest_Result": [],
          "DataRelativeUnique": [],
          "Gps_ManuSimDataParam": [],
          "Gps_ManuPrintParam": [],
          "Gps_ManuCpParam": [],
          "Gps_CartonBoxTwenty_Result": [],
          "Gps_AutoTest_Result2": [],
          "Gps_AutoTest_Result3": []
        },

        isPending: false,
        isLocked: false,
        validateVisible: false,
        validateInfo: {
          user: '',
          password: ''
        },
        deleteTableSubmitVisible: false,
        deleteTableGroup: [0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0],
        deleteTableList: [
          {
            name: 'DataRelativeSheet',
            remark: 'AMS数据关联表'
          },
          {
            name: 'DataRelativeUnique',
            remark: '数据关联表(工厂)'
          },
          {
            name: 'DataRelativeUpdate',
            remark: '数据关联表(关联更新)'
          },
          {
            name: 'Gps_AutoTest_Result',
            remark: '组装功能表'
          },
          {
            name: 'Gps_AutoTest_Result2',
            remark: 'SMT功能表'
          },
          {
            name: 'Gps_AutoTest_Result3',
            remark: '老化后功能表'
          },
          {
            name: 'Gps_CartonBoxTwenty_Result',
            remark: '卡通箱表'
          },
          {
            name: 'Gps_CoupleTest_Result',
            remark: '耦合表'
          },
          {
            name: 'Gps_ManuPrintParam',
            remark: '机身彩盒打印记录表'
          },
          {
            name: 'Gps_TestResult',
            remark: '总表'
          },
          {
            name: 'NetMarkIMEI',
            remark: '网标与IMEI绑定表'
          },
          {
            name: 'Gps_ManuSimDataParam',
            remark: '种子物联网卡表'
          }
        ]

      }
    },
    watch: {
      /*queryMode: function (val) {
        this.singleItemValue = "";
        this.hashItemValue = "";
        this.continuousValue = ["", ""]
      }*/
    },
    computed: {
      searchingItem: function () {
        switch (this.queryWord) {
          case 'imei':
            return 'IMEI号';
          case 'sn':
            return 'SN号';
          case 'zhiDan':
            return '制单号';
        }
      },
      MultiTableConfig: function () {
        return MultiTableConfig
      }
    },
    methods: {
      initData: function () {
        Object.assign(this.$data, this.$options.data())
      },
      checkFormat: function () {
        return new Promise((resolve, reject) => {
          if (this.queryMode === 0) {
            if (this.singleItemValue === '') {
              reject("请填写" + this.searchingItem)
            }
          }
          if (this.queryMode === 1) {
            let tempArray = this.hashItemValue.split(',');
            tempArray.forEach(item => {
              if (item === '') {
                reject('导入格式有误')
              }
            })
          }
          if (this.queryMode === 2) {
            if (this.continuousValue[0] === '' || this.continuousValue[1] === '') {
              reject('范围填写不完整')
            }
          }
          resolve();
        })
      },
      queryData: function () {
        this.checkFormat().then(() => {
          if (!this.isPending) {
            this.isPending = true;
            this.$openLoading();
            let options = {
              url: multiTableQueryUrl,
              data: {
                type: this.queryMode,
                isIMEIHex: this.imeiIsHex
              }
            };
            switch (this.queryMode) {
              case 0:
                options.data[this.queryWord] = this.singleItemValue;
                break;
              case 1:
                options.data[this.queryWord] = this.hashItemValue;
                break;
              case 2:
                options.data[this.queryWord] = this.continuousValue.toString();
                break;
            }
            axiosFetch(options).then(response => {
              if (response.data.result === 200) {
                Object.keys(response.data.data).forEach(key => {
                  //key首字母转大写
                  this.tableData[key.replace(/( |^)[a-z]/g, (L) => L.toUpperCase())] = JSON.parse(JSON.stringify(response.data.data[key]));
                  //有数据则锁定窗口
                  if (response.data.data[key].length > 0) {
                    this.isLocked = true;
                  }
                });
                if (!this.isLocked) {
                  this.$alertInfo('查无数据')
                }
              } else {
                this.$alertWarning(response.data.data)
              }
              this.hashImportShowing = false;
              this.isPending = false;
              this.$closeLoading();
            }).catch(err => {
              this.isPending = false;
              this.$closeLoading();
              this.$alertInfo(err)
            });
          }
        }).catch(err => {
          this.isPending = false;
          this.$closeLoading();
          this.$alertInfo(err)
        });
      },
      exportTable: function () {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          let url = multiTableDownloadUrl,
            data = {
              type: this.queryMode,
              isIMEIHex: this.imeiIsHex
            };
          switch (this.queryMode) {
            case 0:
              data[this.queryWord] = this.singleItemValue;
              break;
            case 1:
              data[this.queryWord] = this.hashItemValue;
              break;
            case 2:
              data[this.queryWord] = this.continuousValue.toString();
              break;
          }

          axiosDownload({
            url: url,
            data: data
          }).then(response => {
            let contentType = response.request.getResponseHeader('content-type');
            if (contentType === 'application/vnd.ms-excel') {
              let name = response.request.getResponseHeader('Content-Disposition').split('=')[1];
              saveAs(response.data, decodeURIComponent(name))
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

      showValidateDialog: function () {
        this.validateInfo = {
          user: '',
          password: ''
        };
        this.deleteTableGroup = [0, 0, 1, 1, 0, 1, 0, 1, 0, 0];
        this.validateVisible = true;
      },

      validateDelete: function () {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          this.validateUser().then(() => {
            this.deleteTableSubmitVisible = true;
            this.isPending = false;
            this.$closeLoading();
          }).catch(err => {
            this.isPending = false;
            this.$closeLoading();
            this.$alertInfo(err)
          })
        }
      },

      deleteData: function () {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          let options = {
            url: multiTableDeleteUrl,
            data: {
              type: this.queryMode,
              deleteTable: this.deleteTableGroup.toString()
            }
          };
          switch (this.queryMode) {
            case 0:
              options.data[this.queryWord] = this.singleItemValue;
              break;
            case 1:
              options.data[this.queryWord] = this.hashItemValue;
              break;
            case 2:
              options.data[this.queryWord] = this.continuousValue.toString();
              options.data.isIMEIHex = this.imeiIsHex;
              break;
          }

          axiosFetch(options).then(response => {
            if (response.data.result === 200) {
              this.$alertSuccess("删除成功")
            } else {
              this.$alertWarning(response.data.data)
            }
            this.isPending = false;
            this.$closeLoading();
            this.initData();
          }).catch(err => {
            this.isPending = false;
            this.$closeLoading();
            this.$alertInfo(err)
          });
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
    }
  }
</script>

<style scoped>
  .multi-table-module {
    padding: 10px 20px 0 0;
  }

  .options-area {
    background: #fff;
    border: 1px solid #eeeeee;
    border-radius: 8px;
    padding: 10px;
    margin-bottom: 10px;
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
    width: 200px;
  }

  .form-group .form-label {
    display: block;
    line-height: 28px;
  }

  .form-group-continuous {
    display: flex;
    padding: 10px 10px 0;
  }


  .word-selector {
    border-right: 1px solid #eee;
    padding: 10px 10px 0;
  }

  .form-group-btn {
    margin-top: 28px;
    margin-right: 20px;
  }

  .result-area {
    border-radius: 8px;
    border: 1px solid #eeeeee;
    background: #fafafa;
    min-height: 300px;
    max-height: 600px;
    overflow-y: auto;
  }

  .result-tag {
    margin: 20px;
  }

  .result-tag-title {
    border-radius: 5px 5px 0 0;
    border: 1px solid #eeeeee;
    border-bottom: none;
    width: 200px;
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

  .result-tag-details-none {
    border-radius: 0 5px 5px 5px;
    border: 1px solid #eeeeee;
    height: 30px;
    background: #ffffff;
  }

  .result-tag-details-none span {
    display: inline-block;
    width: 100%;
    line-height: 30px;
    font-size: 13px;
    color: #909399;
    text-align: center;
  }

  .checkbox-item-box {
    width: 100%;
    padding: 0 60px;
  }

  .checkbox-item {
    margin-bottom: 3px;
  }
</style>
