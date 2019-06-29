<!--表单查看页面的条件过滤栏-->

<template>
  <div class="options-area">
    <div class="form-row">
      <div v-for="item in queryOptions" class="row no-gutters pl-3 pr-3">
        <component :opt="item" :is="item.type + '-comp'" :callback="thisFetch"></component>
      </div>
      <div class="form-group row align-items-end">
        <div class="btn btn-secondary ml-3 mr-4" @click="initForm($store.state.tableRouterApi)">清空条件</div>
      </div>
      <div class="form-group row align-items-end">
        <div class="btn btn-primary ml-3 mr-4" @click="thisFetch">查询</div>
      </div>
      <div class="form-group row align-items-end">
        <div class="btn btn-primary ml-3 mr-4" @click="downloadData">下载报表</div>
      </div>
    </div>
  </div>
</template>

<script>
  import {mapGetters, mapActions} from 'vuex';
  import {setRouterConfig, tableSelectUrl, getRequestUrl} from "../../../../config/preTableApiConfig";
  import {tableDownloadUrl} from "../../../../config/globalUrl";
  import {downloadFile} from "../../../../utils/fetchData";
  import {Settings} from 'luxon'
  import {Datetime} from 'vue-datetime'
  import 'vue-datetime/dist/vue-datetime.css'
  import _ from 'lodash'

  export default {
    name: "Options",
    components: {
      'text-comp': {
        props: ['opt', 'callback'],
        template: '<div class="form-group col pr-3"">\n' +
        '           <label :for="opt.id">{{opt.name}}</label>\n' +
        '           <input type="text" class="form-control" :id="opt.id" v-model="opt.model" @keyup.enter="callback" autocomplete="off">\n' +
        '          </div>'
      },
      'date-comp': {
        props: ['opt'],
        components: {
          Datetime
        },
        template: '<div class="row">\n' +
        '    <div class="form-group col pr-3">\n' +
        '      <label>{{opt.name}}  从：</label>\n' +
        '      <datetime v-model="opt.modelFrom" type="datetime" />\n' +
        '    </div>\n' +
        '    <div class="form-group col pr-3">\n' +
        '      <label>至：</label>\n' +
        '      <datetime v-model="opt.modelTo" type="datetime" />\n' +
        '    </div>\n' +
        '  </div>'

      },
      'range-comp': {
        props: ['opt'],
        template: '<div>\n' +
        '    <div class="form-group col pr-3">\n' +
        '      <label>{{opt.name}}</label>\n' +
        '      <select :name="opt.id" v-model="opt.selectModel" class="custom-select">\n' +
        '        <option v-for="item in opt.selection" :value="item.id">{{item.name}}</option>\n' +
        '      </select>\n' +
        '    </div>\n' +
        '    <div class="form-group col pr-3">\n' +
        '      <input type="text" v-model="opt.textModel" class="form-control">\n' +
        '    </div>\n' +
        '  </div>'
      }
    },
    data() {
      return {
        // pageSize: 2000,
        queryOptions: [],
        copyQueryOptions: [],
        queryString: "",
        isDownloading: false
      }
    },
    mounted() {
      Settings.defaultLocale = 'zh-CN';
      if (this.$route.query.type) {
        this.initForm(this.$route.query.type)
      }
    },
    computed: {
      ...mapGetters([
        'tableRouterApi'
      ]),
    },
    watch: {
      tableRouterApi: function (val) {
        this.initForm(val);
      }
    },
    methods: {
      ...mapActions(['setLoading']),
      initForm: function (opt) {
        let routerConfig = setRouterConfig(opt);
        this.queryOptions = JSON.parse(JSON.stringify(routerConfig.data.queryOptions));
        this.$router.push({
          url: '/table_old/details',
          query: {
            type: opt
          }
        })
      },
      createQueryString: function () {
        this.queryString = "";
        this.copyQueryOptions = this.queryOptions.filter((item) => {
          if ((item.modelFrom === '') ^ (item.modelTo === '')) {
            this.$alertInfo('日期选择不完整');
          }
          if (!(item.model === "" || item.modelFrom === "" || item.modelTo === "")) {
            return true;
          }
        });

        this.copyQueryOptions.map((item, index) => {
          if (item.type === 'text') {
            if (_.trim(item.model) !== "") {
              if (index === 0) {
                this.queryString += (item.id + "#=#" + _.trim(item.model))
              } else {
                this.queryString += ("#&#" + item.id + "#=#" + _.trim(item.model))
              }

            } else {
              this.$closeLoading()
            }
          } else if (item.type === 'date') {
            if (item.modelFrom !== '' && item.modelTo !== '') {
              let tempFrom = item.modelFrom.replace('T', ' ').replace('Z', '');
              let tempTo = item.modelTo.replace('T', ' ').replace('Z', '');
              if (this.compareDate(tempFrom, tempTo) >= 0) {
                if (index === 0) {
                  this.queryString += (item.id + "#>=#" + tempFrom + "#&#" + item.id + "#<=#" + tempTo)
                } else {
                  this.queryString += ("#&#" + item.id + "#>=#" + tempFrom + "#&#" + item.id + "#<=#" + tempTo)
                }
              } else {
                this.$alertWarning('日期格式错误');
                this.$closeLoading()
              }
            }
          } else if (item.type === 'range') {
            if (item.selectModel !== '' && _.trim(item.textModel) !== '') {
              if (index === 0) {
                this.queryString += (item.selectModel + "#=#" + _.trim(item.textModel))
              } else {
                this.queryString += ("#&#" + item.selectModel + "#=#" + _.trim(item.textModel))
              }
            }
          }

        })
      },
      fetchData: function () {
        let options = {
          url: getRequestUrl(this.$store.state.tableRouterApi),
          data: {
            table: this.$store.state.tableRouterApi,
            pageNo: 1,
            pageSize: 20
          }
        };
        if (this.queryString !== "") {
          options.data.filter = this.queryString
        }
        //this.setTableRouter(obj.type);
        this.$router.push('_empty');
        this.$router.replace({
          path: '/table_old/details',
          query: options
        })

      },
      thisFetch: function () {
        this.createQueryString();
        this.fetchData()
      },
      compareDate: function (dateFrom, dateTo) {
        let compFrom = new Date(dateFrom);
        let compTo = new Date(dateTo);
        return (compTo - compFrom);
      },
      downloadData: function () {
        if (this.isDownloading === false) {
          this.createQueryString();
          this.isDownloading = true;
          let thisTable = this.$store.state.tableRouterApi;

          let url = tableDownloadUrl,
            data = {
              table: thisTable,
              '#TOKEN#': this.$store.state.token
            };
          if (this.queryString !== "") {
            data.filter = this.queryString
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
            case 'JS_PrintTime':
              data.descBy = 'JS_PrintTime';
              data.table = 'Gps_ManuPrintParam';
              break; //修正机身/彩盒帖打印表
            case 'CH_PrintTime':
              data.descBy = 'CH_PrintTime';
              data.table = 'Gps_ManuPrintParam';
              break; //修正机身/彩盒帖打印表
          }
          downloadFile(url, data);
          let count = 0;
          let mark = setInterval(() => {
            count++
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

      }
    }
  }
</script>

<style scoped>
  .options-area {
    background: #fff;
    border: 1px solid #eeeeee;
    border-radius: 8px;
    padding: 10px;
  }
</style>
