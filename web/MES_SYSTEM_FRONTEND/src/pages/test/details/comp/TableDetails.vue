<!--订单配置页面表单-->
<template>
  <div class="main-details mt-1 mb-3">
    <datatable
      v-bind="$data"
    ></datatable>
  </div>
</template>
<script>
  import Qs from 'qs'
  import {axiosFetch} from "../../../../utils/fetchData";
  import {mapGetters, mapActions} from 'vuex'
  import {getTestConfig, testSelectUrl} from "../../../../config/testApiConfig";
  import EditOptions from './EditOptions';
  import EditPanel from './EditPanel';
  import {errHandler} from "../../../../utils/errorHandler";

  export default {
    name: "Details",
    props: ['row'],
    components: {
      EditOptions,
      EditPanel
    },
    data() {
      return {
        fixHeaderAndSetBodyMaxHeight: 650,
        tblStyle: {
          'word-break': 'break-all',
          'table-layout': 'fixed',
          'white-space': 'pre-wrap'

        },
        HeaderSettings: false,
        pageSizeOptions: [20, 40, 80, 100],
        data: [],
        //srcData: [],
        columns: [],
        total: 0,
        query: {"limit": 20, "offset": 0},
        isPending: false
      }
    },
    created() {
      this.init();
      this.thisFetch(this.$route.query)
    },
    computed: {
      ...mapGetters([
        'testType'
      ]),

    },
    watch: {
      $route: function (route) {
        this.init();
        this.setLoading(true);
        if (route.query.type) {
          let options = {
            url: testSelectUrl,
            data: {
              pageNo: 1,
              pageSize: 20,
              descBy: 'RecordTime',
              type: route.query.type
            }
          };
          this.fetchData(options)
        } else if (!route.query.data) {
          this.thisFetch(route.query)
        } else {
          this.fetchData(route.query)
        }


      },
      query: {
        handler(query) {
          this.setLoading(true);
          this.dataFilter(query);
        },
        deep: true
      }
    },
    mounted: function () {
    },
    methods: {
      ...mapActions(['setTableRouter', 'setLoading']),
      init: function () {
        this.data = [];
        //this.srcData = [];
        this.columns = [];
        this.total = 0;
        this.query = {"limit": 20, "offset": 0}
      },
      thisFetch: function (opt) {
        let options = {
          url: testSelectUrl,
          data: {
            pageNo: 1,
            pageSize: 20,
            descBy: 'RecordTime',
            type: opt.type
          }
        };
        this.fetchData(options)
      },
      fetchData: function (options) {
        let routerConfig = getTestConfig('test_manage');
        this.columns = routerConfig.data.dataColumns;
        if (!this.isPending) {
          this.isPending = true;
          axiosFetch(options).then(response => {
            this.setLoading(false);
            this.isPending = false;
            if (response.data.result === 200) {
              //this.srcData = response.data.data.list;
              //this.data = response.data.data.list.slice(this.query.offset, this.query.offset + this.query.limit);
              this.data = response.data.data.list;
              this.data.map((item, index) => {
                item.showId = index + 1 + this.query.offset;
              });
              this.total = response.data.data.totalRow
            } else {
              errHandler(response.data.result)
            }
          })
            .catch(err => {
              this.isPending = false;
              console.log(JSON.stringify(err));
              this.$alertDanger('请求超时，清刷新重试')
            })
        } else {
          this.setLoading(false)
        }
      },
      dataFilter: function () {
        let options = {
          url: testSelectUrl,
          data: {
            type: this.testType,
            descBy: 'RecordTime'
          }
        };
        options.data.pageNo = this.query.offset / this.query.limit + 1;
        options.data.pageSize = this.query.limit;
        this.fetchData(options)
      }
    }
  }
</script>

<style scoped>
  .main-details {
    background: #fff;
    border: 1px solid #eeeeee;
    border-radius: 8px;
    padding: 10px;
    min-height: 500px;
  }

</style>
