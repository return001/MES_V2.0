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
        label="操作"
        width="100"
      >
        <template slot-scope="scope">
          <el-button type="text" size="small" @click="editUser(scope.row)">编辑</el-button>
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
    <user-operation :lineGroup="lineGroup" :processGroup="processGroup"/>
  </div>
</template>

<script>
    import {userQueryUrl} from "../../../config/globalUrl";
    import {axiosFetch} from "../../../utils/fetchData";
    import UserOperation from "./UserOperation"
    import eventBus from "../../../utils/eventBus";
    import store from "../../../store"

    export default {
        name: "UserDetails",
        components: {
            UserOperation
        },
        props: ['lineGroup', 'processGroup'],
        data() {
            return {
                tableData: [],
                tableColumns: [
                    {field: 'Id', title: 'ID', colStyle: {'width': '100px'}},
                    {field: 'Name', title: '工号', colStyle: {'width': '100px'}},
                    {field: 'UserDes', title: '描述', colStyle: {'width': '100px'}},
                    {
                        field: 'WebUserType',
                        title: '用户类型',
                        colStyle: {'width': '100px'},
                        formatter(row, column, cellValue, index) {
                            let mark = false;
                            if (!!store.state.userTypeList.webTypeList) {
                                let list = store.state.userTypeList.webTypeList;
                                for (let i = 0; i < list.length; i++) {
                                    if (list[i].TypeId === cellValue) {
                                        mark = true;
                                        return list[i].TypeDes
                                    }
                                }
                                if (!mark) {
                                    return '未定义'
                                }
                            }
                        }
                    },
                    {field: 'LoginTime', title: '最后一次登录时间', colStyle: {'width': '100px'}},
                    {
                        field: 'InService',
                        title: '是否启用',
                        colStyle: {'width': '100px'},
                        formatter(row, column, cellValue, index) {
                            switch (cellValue) {
                                case true:
                                    return '是';
                                case false:
                                    return '否';
                            }
                        }
                    },
                    {field: 'LineName', title: '所属产线', colStyle: {'width': '100px'}},
                    {field: 'EmployeeType', title: '是否正式员工', colStyle: {'width': '100px'},},
                    {field: 'MainProcess', title: '主要工序', colStyle: {'width': '100px'}},
                    {field: 'Proficiency', title: '熟练程度', colStyle: {'width': '100px'}},
                    {field: 'OtherProcess', title: '其他技能', colStyle: {'width': '100px'},
                      formatter(row, column, cellValue, index) {
                        return !cellValue ? '' : cellValue.split('@@').join('、')
                      }},
                    // {field: 'IsOnline', title: '最后一次登录时间', colStyle: {'width': '100px'}},

                ],
                isPending: false,
                queryString: '',
                paginationOptions: {
                    currentPage: 1,
                    pageSize: 20,
                    total: 0
                },
            }
        },
        mounted() {
            this.init();
            //this.thisFetch();

            /*event register*/
            eventBus.$off('userQueryData');
            eventBus.$on('userQueryData', (data) => {
                Object.assign(this.paginationOptions, this.$options.data().paginationOptions);
                this.queryString = data;
                this.thisFetch()
            })
        },
        watch: {
            $route: function (route) {
                //this.init();
                this.$openLoading();
                let options = {
                    url: userQueryUrl,
                    data: {
                        table: 'LUserAccount',
                        pageNo: 1,
                        pageSize: 20
                    }
                };
                this.fetchData(options)
            }
        },
        methods: {
            init: function () {
                Object.assign(this.$data, this.$options.data())
            },
            thisFetch: function (opt) {
                if (opt === 'sizeChange') {
                    this.paginationOptions.currentPage = 1;
                }
                this.$openLoading();
                let options = {
                    url: userQueryUrl,
                    data: {
                        table: 'LUserAccount',
                        pageNo: this.paginationOptions.currentPage,
                        pageSize: this.paginationOptions.pageSize
                    }
                };
                this.fetchData(options);
            },
            fetchData: function (options) {
                //加载查询过滤参数
                if (this.queryString !== '') {
                    options.data.filter = this.queryString
                }
                if (!this.isPending) {
                    this.isPending = true;
                    axiosFetch(options).then(response => {
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
                    }).catch(err => {
                        this.$alertDanger('请求超时，清刷新重试')
                    }).finally(() => {
                        this.$closeLoading();
                        this.isPending = false;
                    })
                } else {
                    this.$closeLoading()
                }
            },
            editUser: function (val) {
                eventBus.$emit('editUser', val)
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
