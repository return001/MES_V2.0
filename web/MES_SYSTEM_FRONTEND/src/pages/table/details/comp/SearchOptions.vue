<!--表单查看页面的条件过滤栏-->

<template>
  <div class="options-area">
    <div class="form-row">
      <div v-for="item in queryOptions" class="form-group">
        <component :opt="item" :is="item.type + '-comp'"
                   :pickerOptions="pickerOptions"></component>
      </div>
      <div style="width: 140px; margin-right: 10px"
           v-if="$route.query.type === 'DataRelativeSheet' || $route.query.type === 'Gps_CartonBoxTwenty_Result'">
        <el-checkbox size="small" id="rel-checkbox" v-model="isReferred">{{relTable}}</el-checkbox>
      </div>
      <div style="margin-right: 10px"
           v-if="$route.query.type === 'Gps_ManuCpParam'"
           class="form-group">
        <label>对比失败次数</label>
        <el-radio-group v-model="cpParamRadio" >
          <el-radio :label="0">任意</el-radio>
          <el-radio :label="1">无错误</el-radio>
          <el-radio :label="2">有错误</el-radio>
        </el-radio-group>
      </div>
      <div class="form-group-btn">
        <el-button size="small" type="info" @click="initForm($route.query.type, {isReferred: isReferred})">清空条件</el-button>
      </div>
      <div class="form-group-btn">
        <el-button size="small" type="primary" @click="thisFetch">查询</el-button>
      </div>
      <div class="form-group-btn" v-if="isDataExist">
        <el-button size="small" type="primary" @click="downloadData" v-if="!isReferred">下载报表</el-button>
        <el-button size="small" type="primary" @click="downloadData"
                   v-if="isReferred && $route.query.type === 'Gps_CartonBoxTwenty_Result'">下载报表
        </el-button>
      </div>
      <div class="form-group-btn"
           v-if="$route.query.type !== 'LTestLogMessage' && $route.query.type !== 'Gps_ManuCpParam' && !isReferred && checkDelPermission && isDataExist">
        <el-button size="small" type="warning" @click="validateVisible = true">删除所有</el-button>
        <!--<el-button type="warning" @click="deleteAll">删除所有</el-button>-->
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
  import {setRouterConfig, tableSelectUrl, getRequestUrl} from "../../../../config/tableApiConfig";
  import {tableDownloadUrl, tableDeleteUrl, validateUrl, tableCartonDownloadUrl} from "../../../../config/globalUrl";
  import {axiosFetch, axiosDownload} from "../../../../utils/fetchData";
  import {checkDelPermission} from "../../../../utils/utils";
  import {saveAs} from 'file-saver'


  export default {
    name: "Options",
    components: {
      'text-comp': {
        props: ['opt', 'callback'],
        template: '<div class="query-comp-container">\n' +
        '           <label :for="opt.id">{{opt.name}}</label>\n' +
        '           <el-input size="small" type="text" class="form-control" :id="opt.id" v-model.trim="opt.model" autocomplete="off"></el-input>\n' +
        '          </div>'
      },
      'date-comp': {
        props: ['opt', 'pickerOptions'],
        components: {
        },
        template: '<div class="row">\n' +
        '    <div class="form-group">\n' +
        '      <label>{{opt.name}}  从：</label>\n' +
        '      <el-date-picker\n' +
        '                size="small"\n' +
        '                v-model="opt.timeRange"\n' +
        '                type="datetimerange"\n' +
        '                :picker-options="pickerOptions"\n' +
        '                range-separator="-"\n' +
        '                prefix-icon="el-icon-date"\n' +
        '                start-placeholder="开始日期"\n' +
        '                end-placeholder="结束日期"\n' +
        '                value-format="yyyy-MM-dd HH:mm:ss">' +
        '</el-date-picker>\n' +
        '    </div>\n' +
        '  </div>'

      },
      'map-comp': {
        props: ['opt'],
        template: '<div>\n' +
        '    <div class="form-group">\n' +
        '      <label>{{opt.name}}</label>\n' +
        '      <el-select size="small" :name="opt.id" v-model="opt.selectModel" class="custom-select" style="width: 100%;">\n' +
        '        <el-option v-for="item in opt.selection" :value="item.id" :key="item.id" :label="item.name"></el-option>\n' +
        '      </el-select>\n' +
        '    </div>\n' +
        '    <div class="form-group" style="margin-top: 10px">\n' +
        '      <el-input size="small" type="text" v-model.trim="opt.textModel" class="form-control"></el-input>\n' +
        '    </div>\n' +
        '  </div>'
      },
      'range-comp': {
        props: ['opt'],
        template: '<div style="display: flex;">\n' +
        '    <div class="form-group">\n' +
        '      <label :for="opt.id + \'From\'">{{opt.name}} 从：</label>\n' +
        '      <el-input size="small" type="text" class="form-control" :id="opt.id + \'From\'" v-model.trim="opt.modelFrom" autocomplete="off"></el-input>\n' +
        '    </div>\n' +
        '    <div class="form-group" style="margin-left: 10px">\n' +
        '      <label :for="opt.id + \'To\'">至：</label>\n' +
        '      <el-input size="small" type="text" class="form-control" :id="opt.id + \'To\'" v-model.trim="opt.modelTo" autocomplete="off"></el-input>\n' +
        '    </div>\n' +
        '  </div>'
      }
    },
    data() {
      return {
        // pageSize: 2000,
        queryOptions: [],
        copyQueryOptions: [],
        isReferred: false,
        queryString: "",
        isPending: false,
        timeRange: [],
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
        dataCount: 0,
        cpParamRadio: 0
      }
    },
    mounted() {
      if (this.$route.query.type) {
        this.initForm(this.$route.query.type, {isReferred: this.isReferred})
      }
      eventBus.$off('setIsReferred');
      eventBus.$on('setIsReferred', data => {
        this.isReferred = data;
      });
      eventBus.$off('setTableDataCount');
      eventBus.$on('setTableDataCount', data => {
        this.dataCount = data;
      })
    },
    computed: {
      relTable: function () {
        if (this.$route.query.type === 'DataRelativeSheet') {
          return '是否关联打印表'
        } else if (this.$route.query.type === 'Gps_CartonBoxTwenty_Result') {
          return '是否关联关联表'
        }
      },
      checkDelPermission: function () {
        return checkDelPermission(this.$route.query.type)
      },
      isDataExist: function () {
        return this.dataCount > 0;
      }
    },
    watch: {
      $route: function (val) {
        this.initForm(val.query.type, {isReferred: this.isReferred});
      },
      isReferred: function (val) {
        this.initForm(this.$route.query.type, {isReferred: val});
        // this.thisFetch()
      },
    },
    methods: {
      initForm: function (opt, params) {
        let routerConfig = setRouterConfig(opt, params);
        this.queryOptions = JSON.parse(JSON.stringify(routerConfig.data.queryOptions));
      },
      createQueryString: function () {
        return new Promise((resolve, reject) => {
          this.queryString = "";
          this.copyQueryOptions = this.queryOptions.filter((item) => {
            if ((item.modelFrom === '') ^ (item.modelTo === '')) {
              reject('请检查数据完整性')
            }
            if (!(item.model === "" || item.modelFrom === "" || item.modelTo === "")) {
              return true;
            }
            if (item.timeRange) {
              return true;
            }
          });

          this.copyQueryOptions.map((item, index) => {
            switch (item.type) {
              case 'text':
                  if (index === 0) {
                    this.queryString += (item.id + "#like#" + item.model)
                  } else {
                    this.queryString += ("#&#" + item.id + "#like#" + item.model)
                  }
                break;
              case 'date':
                if (!!item.timeRange) {
                  let tempFrom = item.timeRange[0];
                  let tempTo = item.timeRange[1];
                  if (index === 0) {
                    this.queryString += (item.id + "#>=#" + tempFrom + "#&#" + item.id + "#<=#" + tempTo)
                  } else {
                    this.queryString += ("#&#" + item.id + "#>=#" + tempFrom + "#&#" + item.id + "#<=#" + tempTo)
                  }
                }
                break;
              case 'map':
                if (item.selectModel !== '' && item.textModel !== '') {
                  if (index === 0) {
                    this.queryString += (item.selectModel + "#=#" + item.textModel)
                  } else {
                    this.queryString += ("#&#" + item.selectModel + "#=#" + item.textModel)
                  }
                }
                break;
              case 'range':
                if (item.modelFrom !== '' && item.modelTo !== '') {
                  let tempFrom = item.modelFrom;
                  let tempTo = item.modelTo;
                  if (tempFrom - tempTo <= 0) {
                    if (index === 0) {
                      this.queryString += (item.id + "#>=#" + tempFrom + "#&#" + item.id + "#<=#" + tempTo)
                    } else {
                      this.queryString += ("#&#" + item.id + "#>=#" + tempFrom + "#&#" + item.id + "#<=#" + tempTo)
                    }
                  } else {
                    reject('请检查输入范围')
                  }
                }
            }

          });
          resolve()
        });

      },
      thisFetch: function () {
        this.createQueryString().then(data => {
          // this.$openLoading();
          let params = {
            isReferred: this.isReferred
          };
          if (this.cpParamRadio !== 0) {
            let radioQueryText = "";
            if (this.cpParamRadio === 1) {
              radioQueryText = "CPFalseCount#=#0"
            } else {
              radioQueryText = "CPFalseCount#>#0"
            }
            this.queryString === '' ? this.queryString += radioQueryText : this.queryString += ('#&#' + radioQueryText)
          }
          eventBus.$emit('tableQueryData', [this.queryString, params])
        }).catch(err => {
          this.$alertInfo(err)
        });
      },

      downloadData: function () {
        if (!this.isPending) {
          if (this.dataCount > 2000) {
            this.$alertWarning("所选数据过多(>2000)，无法生成报表");
            return;
          }
          this.isPending = true;
          this.$openLoading();
          this.createQueryString();
          let thisTable = this.$route.query.type;

          let url = tableDownloadUrl,
            data = {
              table: thisTable
            };
          if (this.setDatabase() === 0) {
            data.type = this.setDatabase()
          }

          if (this.cpParamRadio !== 0) {
            let radioQueryText = "";
            if (this.cpParamRadio === 1) {
              radioQueryText = "CPFalseCount#=#0"
            } else {
              radioQueryText = "CPFalseCount#>#0"
            }
            this.queryString === '' ? this.queryString += radioQueryText : this.queryString += ('#&#' + radioQueryText)
          }

          if (this.queryString !== "") {
            data.filter = this.queryString
          }
          if (this.$route.query.type === 'Gps_CartonBoxTwenty_Result' && this.isReferred) {
            data.isReferred = 1;
            url = tableCartonDownloadUrl
          }
          //根据不同页面修正参数
          switch (thisTable) {
            case 'Gps_AutoTest_Result':
            case 'Gps_AutoTest_Result2':
            case 'Gps_AutoTest_Result3':
            case 'Gps_CoupleTest_Result':
              data.descBy = 'TestTime';
              break;
            case 'Gps_OperRecord':
              data.descBy = 'OperTime';
              break;
            case 'Gps_ManuLdParam':
              data.descBy = 'LDTime';
              break;
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
      deleteAll: function () {
        if (this.$route.query.type === 'Gps_CartonBoxTwenty_Result' && this.queryString.indexOf('ZhiDan') === -1) {
          this.$alertInfo("必须经过制单号筛选才可以进行删除操作");
          return;
        }
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
            options.url = tableDeleteUrl;
            options.data.table = this.$route.query.type;
            options.data.type = this.setDatabase();
            options.data.filter = this.queryString;

            axiosFetch(options).then(res => {
              if (res.data.result === 200) {
                this.pageReload();
                this.$alertSuccess("成功删除");
              } else {
                this.$alertWarning(res.data.data)
              }
              this.validateVisible = false;
              this.$closeLoading();
              this.isPending = false
            })
          }).catch(action => {
            this.isPending = false;
          });

        }
      },

      validateDelete: function () {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          this.validateUser().then(() => {
            this.isPending = false;
            this.$closeLoading();
            this.deleteAll()
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
      }
    }
  }
</script>

<style scoped>
  .options-area {
    background: #fff;
    border: 1px solid #eeeeee;
    border-radius: 8px;
    padding: 10px 20px;
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

  .form-group /deep/ label {
    line-height: 32px;
    display: block;
  }

  .form-group .el-radio-group {
    display: flex;

  }
  .form-group .el-radio-group .el-radio {
    margin-right: 10px;
  }

  .options-area /deep/ .form-group .el-input {
    width: 200px;
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
