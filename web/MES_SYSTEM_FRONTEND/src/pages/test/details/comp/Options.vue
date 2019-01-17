<!--测试配置页面顶部条件过滤栏-->

<template>
  <div class="options-area">
    <div class="form-row">
      <div v-for="item in queryOptions" class="row no-gutters pl-3 pr-3">
        <component :opt="item" :is="item.type + '-comp'" :callback="thisFetch"></component>
      </div>
      <div class="form-group row align-items-end">
        <div class="btn btn-secondary ml-3 mr-4" @click="initForm('test_manage')">清空条件</div>
      </div>
      <div class="form-group row align-items-end">
        <div class="btn btn-primary ml-3 mr-4" @click="thisFetch">查询</div>
      </div>
      <div class="form-group row align-items-end">
        <div class="btn btn-primary ml-3 mr-4" @click="addOrder">新增</div>
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

  export default {
    name: "Options",
    components: {
      'text-comp': {
        props: ['opt', 'callback'],
        template: '<div class="form-group col pr-3"">\n' +
        '           <label :for="opt.id">{{opt.name}}</label>\n' +
        '           <input type="text" class="form-control" :id="opt.id" v-model="opt.model" @keyup.enter="callback">\n' +
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
      // tableRouterApi: function (val) {
      //   this.initForm(val);
      // }
    },
    methods: {
      ...mapActions(['setLoading', 'setEditing', 'setEditData']),
      initForm: function (opt) {
        let routerConfig = getTestConfig(opt);
        this.queryOptions = JSON.parse(JSON.stringify(routerConfig.data.queryOptions));
      },
      createQueryString: function () {
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
                this.queryString += (item.id + "=" + _.trim(item.model))
              } else {
                this.queryString += ("&" + item.id + "=" + _.trim(item.model))
              }

            } else {
              this.setLoading(false)
            }
          } else if (item.type === 'date') {
            if (item.modelFrom !== '' && item.modelTo !== '') {
              let tempFrom = item.modelFrom.replace('T', ' ').replace('Z', '');
              let tempTo = item.modelTo.replace('T', ' ').replace('Z', '');
              if (this.compareDate(tempFrom, tempTo) >= 0) {
                if (index === 0) {
                  this.queryString += (item.id + '>=' + tempFrom + '&' + item.id + '<=' + tempTo)
                } else {
                  this.queryString += ('&' + item.id + '>=' + tempFrom + '&' + item.id + '<=' + tempTo)
                }
              } else {
                this.$alertWarning('日期格式错误');
                this.setLoading(false)
              }
            }
          }

        })
      },
      fetchData: function () {
        let options = {
          url: testSelectUrl,
          data: {
            pageNo: 1,
            pageSize: 20,
            type: this.testType,
            descBy: 'RecordTime'
          }
        };
        if (this.queryString !== "") {
          options.data.filter = this.queryString
        }
        //this.setTableRouter(obj.type);
        //this.$router.push('/_empty');
        this.$router.replace({
          path: '/test/test_manage',
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
      addOrder: function () {
        this.setEditData([]);
        this.setEditing(true)
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
