<!--测试配置页面顶部条件过滤栏-->

<template>
  <div class="options-area">
    <div class="form-row">
      <div v-for="item in queryOptions">
        <component :opt="item" :is="item.type + '-comp'"
                   :pickerOptions="pickerOptions"></component>
      </div>
      <div class="form-group-btn">
        <el-button size="small" type="primary" @click="thisFetch">查询</el-button>
      </div>
      <div class="form-group-btn">
        <el-button size="small" type="primary" @click="addOrder"  v-if="$route.query.type === '3' || $route.query.type === '4'">新增</el-button>
      </div>
    </div>
  </div>
</template>

<script>
  import {getTestConfig, testSelectUrl} from "../../../../config/testApiConfig";
  import eventBus from "../../../../utils/eventBus";

  export default {
    name: "Options",
    components: {
      'text-comp': {
        props: ['opt', 'callback'],
        template: '<div class="form-group">\n' +
          '           <label :for="opt.id">{{opt.name}}</label>\n' +
          '           <el-input size="small" type="text" class="form-control" :id="opt.id" v-model.trim="opt.model" clearable></el-input>\n' +
          '          </div>'
      },
      'date-comp': {
        props: ['opt', 'pickerOptions'],
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
    },
    data() {
      return {
        queryOptions: [],
        copyQueryOptions: [],
        queryString: "",
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
      }
    },
    mounted: function () {
      this.initForm('test_manage')
    },
    watch: {
      $route: function (val) {
        this.initForm('test_manage');
      }
    },
    methods: {
      initForm: function (opt) {
        let routerConfig = getTestConfig(opt);
        this.queryOptions = JSON.parse(JSON.stringify(routerConfig.data.queryOptions));
      },

      createQueryString: function () {
        return new Promise((resolve, reject) => {
          this.queryString = "";
          this.copyQueryOptions = this.queryOptions.filter((item) => {
            if (!(item.model === "")) {
              return true;
            }
            if (item.timeRange) {
              return true;
            }
          });

          this.copyQueryOptions.map((item, index) => {
            switch (item.type) {
              case 'text':
                if(item.id === 'MachineName' && this.$route.query.type === '2' || item.id === 'MachineName' && this.$route.query.type === '4'){
                  if (index === 0) {
                    this.queryString += (item.id + "#rightLike#" + item.model) //like
                  } else {
                    this.queryString += ("#&#" + item.id + "#rightLike#" + item.model) //like
                  }
                }else{
                  if (index === 0) {
                    this.queryString += (item.id + "#rightLike#" + item.model) //like
                  } else {
                    this.queryString += ("#&#" + item.id + "#rightLike#" + item.model) //like
                  }
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
            }

          });
          resolve()
        })
      },
      thisFetch: function () {
        this.createQueryString().then(() => {
          eventBus.$emit('testQueryData', this.queryString)
        }).catch(err => {
          this.$alertInfo(err);
        });
      },

      compareDate: function (dateFrom, dateTo) {
        let compFrom = new Date(dateFrom);
        let compTo = new Date(dateTo);
        return (compTo - compFrom);
      },

      addOrder: function () {
        if (this.$route.query.type === '2' || this.$route.query.type === '4') {
          eventBus.$emit('editTest', ['add', []])
        } else {
          eventBus.$emit('editTestFunc', ['add', []])
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
    margin-bottom: 10px;
  }

  .form-row {
    display: flex;
    flex-wrap: wrap;
    align-items: flex-end;
  }

  .options-area /deep/ .form-group {
    width: auto;
    margin-right: 10px;
  }

  .options-area /deep/ .form-group label {
    line-height: 40px;
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
