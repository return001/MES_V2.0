<!--订单配置页面表单-->
<template>
  <div class="main-details mt-1 mb-3">
    <el-table
      :data="tableData"
      max-height="560"
      ref="tablecomponent"
      stripe>
      <el-table-column v-for="(item, index) in tableColumns"
                       :key="index"
                       :prop=item.field
                       :label="item.title"
                       :min-width=item.colStyle.width
                       :formatter=item.formatter>
      </el-table-column>

      <el-table-column
        type="index"
        :index="indexMethod"
        fixed="left"
        width="60">
      </el-table-column>

      <el-table-column
        label="操作"
        width="120"
        fixed="left"
      >
        <template slot-scope="scope">
          <el-button type="text" @click="editData('edit', scope.row)" icon="el-icon-t-edit"></el-button>
          <el-button type="text" @click="editData('copy', scope.row)" icon="el-icon-t-copy"></el-button>
          <el-button type="text" @click="delData(scope.row)" icon="el-icon-t-delete"></el-button>
        </template>
      </el-table-column>
    </el-table>
    <!--分页控制-->
    <el-pagination
      background
      :current-page.sync="paginationOptions.currentPage"
      :page-sizes="[20, 40, 80]"
      :page-size.sync="paginationOptions.pageSize"
      layout="total, sizes, prev, pager, next, jumper"
      :total="paginationOptions.total"
      @current-change="thisFetch()"
      @size-change="thisFetch('sizeChange')"
      class="page-pagination">
    </el-pagination>
  </div>
</template>
<script>
  import Qs from 'qs'
  import {axiosFetch} from "../../../../utils/fetchData";
  import {mapGetters, mapActions} from 'vuex'
  import {getTestConfig, testSelectUrl} from "../../../../config/testApiConfig";
  import EditOptions from './EditOptions';
  import EditPanel from './EditFuncPanel';
  import {errHandler} from "../../../../utils/errorHandler";
  import eventBus from "../../../../utils/eventBus";
  import {testOperUrl} from "../../../../config/testApiConfig";
  import {MessageBox} from 'element-ui'

  export default {
    name: "Details",
    props: ['row'],
    components: {
      EditOptions,
      EditPanel
    },
    data() {
      return {
        tableData: [],
        tableColumns: [],
        //srcData: [],
        paginationOptions: {
          currentPage: 1,
          pageSize: 20,
          total: 0
        },
        isPending: false,
        queryString: '',
      }
    },
    computed: {
      ...mapGetters([
        'testType'
      ]),

    },
    watch: {
      $route: function (route) {
        this.init();
        this.$openLoading();
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
      }
    },
    mounted: function () {
      this.init();
      this.thisFetch();
      eventBus.$off('testQueryData');
      eventBus.$on('testQueryData', data => {
        this.paginationOptions.currentPage = 1;
        this.paginationOptions.pageSize = 20;
        this.queryString = data;
        this.thisFetch()
      })
    },
    methods: {
      ...mapActions(['setTableRouter', 'setLoading']),
      init: function () {
        Object.assign(this.$data, this.$options.data())
      },
      thisFetch: function (opt) {
        this.$openLoading();
        if (opt === 'sizeChange') {
          this.paginationOptions.currentPage = 1;
        }

        let options = {
          url: testSelectUrl,
          data: {
            pageNo: this.paginationOptions.currentPage,
            pageSize: this.paginationOptions.pageSize,
            descBy: 'RecordTime',
            type: this.$route.query.type
          }
        };
        this.fetchData(options)
      },
      fetchData: function (options) {
        if (this.queryString !== '') {
          options.data.filter = this.queryString
        }
        let routerConfig = getTestConfig();
        this.tableColumns = routerConfig.data.dataColumns;
        if (!this.isPending) {
          this.isPending = true;
          axiosFetch(options).then(response => {
            this.$closeLoading();
            this.isPending = false;
            if (response.data.result === 200) {
              this.tableData = response.data.data.list;
              this.paginationOptions = {
                currentPage: response.data.data.pageNumber,
                pageSize: response.data.data.pageSize,
                total: response.data.data.totalRow
              }
            } else {
              this.$alertWarning(response.data.data)
            }

          })
            .catch(err => {
              this.$closeLoading();
              this.isPending = false;
              console.log(err);
              this.$alertDanger('请求超时，清刷新重试')
            })
        } else {
          this.$closeLoading()
        }
      },
      indexMethod: function (index) {
        return index + (this.paginationOptions.currentPage - 1) * this.paginationOptions.pageSize + 1
      },

      /*编辑/复制条目 $on at @/pages/test/details/comp/EditPanel*/
      editData: function (operType, val) {
        if (this.$route.query.type === '2' || this.$route.query.type === '4') {
          eventBus.$emit('editTest', [operType, val])
        } else {
          eventBus.$emit('editTestFunc', [operType, val])
        }
      },

      delData: function (val) {
        MessageBox.confirm('确认作废该配置吗?', {
          confirmButtonText: '确定',
          cancelButtonText: '取消',
          type: 'warning'
        }).then(() => {

          if (!this.isPending) {
            this.isPending = true;
            let url = testOperUrl + '/cancel';
            let options = {
              url: url,
              data: {
                key: val.SoftWare,
                type: this.$route.query.type
              }
            };
            axiosFetch(options).then((response) => {
              this.isPending = false;
              if (response.data.result === 200) {
                this.$alertSuccess('作废成功');
                let tempUrl = this.$route.fullPath;
                this.$router.push('/_empty');
                this.$router.replace(tempUrl)
              } else {
                this.$alertWarning(response.data.data)
              }
            }).catch(err => {
              this.isPending = false;
              this.$alertDanger('请求超时，清刷新重试');
            })
          }
        }).catch(() => {

        })
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

  .page-pagination {
    display: flex;
    flex-wrap: wrap;
    margin: 20px 0;
    padding: 0 20px;
  }

</style>
