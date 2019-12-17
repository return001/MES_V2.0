<!--订单配置页面顶部条件过滤栏-->

<template>
  <div class="options-area">
    <div class="form-row">
      <div v-for="item in queryOptions">
        <component :opt="item" :is="item.type + '-comp'"></component>
      </div>
      <div class="form-group-btn">
        <el-button size="small" type="primary" @click="thisFetch">查询</el-button>
      </div>
      <div class="form-group-btn" v-if="$store.state.userType === 'SuperAdmin'">
        <el-button size="small" type="primary" @click="addOrder">新增</el-button>
      </div>
    </div>
  </div>
</template>

<script>
  import {getOrderConfig, orderSelectUrl} from "../../../../config/orderApiConfig";
  import eventBus from "../../../../utils/eventBus";

  export default {
    name: "Options",
    components: {
      'text-comp': {
        props: ['opt'],
        template: '<div class="form-group">\n' +
        '           <label class="form-label" :for="opt.id">{{opt.name}}</label>\n' +
        '           <el-input size="small" type="text"  :id="opt.id" v-model.trim="opt.model" clearable></el-input>\n' +
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
        isDownloading: false
      }
    },
    mounted: function () {
      this.initForm('order_manage')

    },
    methods: {
      initForm: function (opt) {
        let routerConfig = getOrderConfig(opt);
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
            }

          });
          resolve()
        })
      },
      thisFetch: function () {
        this.createQueryString().then(() => {
          eventBus.$emit('orderQueryData', this.queryString)
        }).catch(err => {
          this.$alertInfo(err)
        });
      },
      addOrder: function () {
        eventBus.$emit('editOrder', ['add', []])
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

  .form-group {
    width: 200px;
    margin-right: 20px;
  }

  .form-group /deep/ .form-label {
    line-height: 40px;
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
