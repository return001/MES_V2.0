<!--测试配置页面顶部条件过滤栏-->

<template>
  <div class="options-area">
    <div class="form-row">
      <div v-for="item in queryOptions">
        <component :opt="item" :is="item.type + '-comp'" :callback="thisFetch"></component>
      </div>
      <div class="form-group-btn">
        <el-button type="primary" @click="thisFetch">查询</el-button>
      </div>
      <div class="form-group-btn" v-if="$store.state.userType === 'SuperAdmin' || $store.state.userType === 'engineer'">
        <el-button type="primary" @click="addOrder">新增</el-button>
      </div>
    </div>
  </div>
</template>

<script>
  import {mapGetters, mapActions} from 'vuex';
  import {getTestConfig, testSelectUrl} from "../../../../config/testApiConfig";
  import {axiosFetch} from "../../../../utils/fetchData";
  import {Datetime} from 'vue-datetime'
  import 'vue-datetime/dist/vue-datetime.css'
  import _ from 'lodash'
  import eventBus from "../../../../utils/eventBus";

  export default {
    name: "Options",
    components: {
      'text-comp': {
        props: ['opt', 'callback'],
        template: '<div class="form-group">\n' +
        '           <label :for="opt.id">{{opt.name}}</label>\n' +
        '           <el-input type="text" class="form-control" :id="opt.id" v-model="opt.model" @keyup.enter="callback" clearable></el-input>\n' +
        '          </div>'
      },
      'date-comp': {
        props: ['opt'],
        components: {
          Datetime
        },
        template: '<div class="row">\n' +
        '    <div class="form-group col pr-3">\n' +
        '      <label>测试时间  从：</label>\n' +
        '      <datetime v-model="opt.modelFrom" type="datetime"/>\n' +
        '    </div>\n' +
        '    <div class="form-group col pr-3">\n' +
        '      <label>至：</label>\n' +
        '      <datetime v-model="opt.modelTo" type="datetime"/>\n' +
        '    </div>\n' +
        '  </div>'

      }
    },
    data() {
      return {
        queryOptions: [],
        copyQueryOptions: [],
        queryString: "",
      }
    },
    mounted: function () {
      this.initForm('test_manage')

    },
    computed: {
      ...mapGetters([
        'testType'
      ]),
    },
    watch: {
      $route: function (val) {
        this.initForm('test_manage');
      }
    },
    methods: {
      ...mapActions(['setLoading', 'setEditing', 'setEditData']),
      initForm: function (opt) {
        let routerConfig = getTestConfig(opt);
        this.queryOptions = JSON.parse(JSON.stringify(routerConfig.data.queryOptions));
      },


      createQueryString: function () {
        return new Promise((resolve, reject) => {

          this.queryString = "";
          this.copyQueryOptions = this.queryOptions.filter((item) => {
            if (!(item.model === "" || item.modelFrom === "" || item.modelTo === "")) {
              return true;
            }
          });

          this.copyQueryOptions.map((item, index) => {
            if (item.type === 'text') {
              if (_.trim(item.model) !== "") {

                if (index === 0) {
                  this.queryString += (item.id + "#like#" + _.trim(item.model))
                } else {
                  this.queryString += ("#&#" + item.id + "#like#" + _.trim(item.model))
                }

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
                  reject('日期格式错误');
                }
              }
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

  .form-group {
    width: 200px;
    margin-right: 20px;
  }

  .form-group /deep/ label {
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
