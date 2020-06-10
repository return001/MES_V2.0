<template>
  <div class="table-backups">
    <div class="options-area">
      <div class="form-row">
        <div v-for="item in queryOptions" class="row no-gutters pl-3 pr-3">
          <component :opt="item" :is="item.type + '-comp'"
                     :pickerOptions="pickerOptions"></component>
        </div>
        <div class="form-group-btn">
          <el-button size="small" type="primary" @click="thisFetch('query')">查询</el-button>
        </div>
      </div>
    </div>
    <div class="main-details">
      <el-table
        :data="tableData"
        max-height="560"
        stripe>
        <el-table-column
          fixed="right"
          label="操作"
          width="100">
          <template slot-scope="scope">
            <el-button type="text" size="small" @click="downloadData(scope.row)">下载</el-button>
          </template>
        </el-table-column>
        <el-table-column v-for="(item, index) in tableColumns"
                         :key="index"
                         :prop=item.field
                         :label=item.title
                         :width=item.width
                         :formatter=item.formatter>
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
  </div>
</template>

<script>
  import {tableHistoryDownloadUrl, tableHistoryQueryUrl} from "../../../config/globalUrl";
  import {axiosFetch, axiosDownload} from "../../../utils/fetchData";
  import {saveAs} from 'file-saver';

  export default {
    name: "Backups",
    components: {
      'text-comp': {
        props: ['opt', 'callback'],
        template: '<div class="form-group">\n' +
        '           <label :for="opt.id">{{opt.name}}</label>\n' +
        '           <el-input size="small" type="text" :id="opt.id" v-model="opt.model" @keyup.enter="callback" autocomplete="off"></el-input>\n' +
        '          </div>'
      },
      'date-comp': {
        props: ['opt', 'pickerOptions'],
        components: {
        },
        template: '<div class="row" style="margin-right: 20px">\n' +
        '    <div class="form-group">\n' +
        '      <label>{{opt.name}}  从：</label>\n' +
        '      <el-date-picker\n' +
        '                size="small"\n' +
        '                v-model="opt.timeRange"\n' +
        '                clearable\n' +
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
        queryOptions: [
          {
            id: 'FileName',
            name: '文件名',
            model: '',
            type: 'text'
          },
          {
            id: 'Time',
            name: '时间',
            timeRange: '',
            type: 'date'
          }
        ],
        tableColumns: [
          {field: 'ID', title: 'ID', width: '50px'},
          {field: 'FileName', title: '文件名', width: 'auto'},
          {field: 'Time', title: '时间', width: 'auto'},
        ],
        paginationOptions: {
          currentPage: 1,
          pageSize: 20,
          total: 0
        },
        tableData: [],
        queryString: '',
        isPending: false,
        isDownloading: false,
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
    mounted() {
      this.fetchData();
    },
    watch: {
      $route: function (route) {
        this.init();
        this.fetchData();
      }
    },
    methods: {
      init: function () {
        Object.assign(this.$data, this.$options.data())
      },
      initOptions: function () {
        this.queryString = "";
        this.queryOptions = JSON.parse(JSON.stringify(this.$options.data().queryOptions))
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
                  this.queryString += (item.id + "#=#" + item.model) //like
                } else {
                  this.queryString += ("#&#" + item.id + "#=#" + item.model)  //like
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
        });

      },
      thisFetch: function (opt) {
        if (opt === 'sizeChange' || opt === 'query') {
          this.paginationOptions.currentPage = 1;
        }
        this.$openLoading();
        this.createQueryString().then(data => {
          this.fetchData();
        }).catch(err => {
          this.$closeLoading();
          this.$alertInfo(err);
          console.log(err)

        });
      },
      fetchData: function () {
        if (!this.isPending) {
          this.isPending = true;
          let options = {
            url: tableHistoryQueryUrl,
            data: {
              pageNo: this.paginationOptions.currentPage,
              pageSize: this.paginationOptions.pageSize,
              descBy: 'Time'
            }
          };
          if (this.queryString !== "") {
            options.data.filter = this.queryString;
          }
          axiosFetch(options).then(res => {
            if (res.data.result === 200) {
              this.tableData = res.data.data.list;
              this.paginationOptions = {
                currentPage: res.data.data.pageNumber,
                pageSize: res.data.data.pageSize,
                total: res.data.data.totalRow
              }
            } else {
              this.$alertWarning(res.data.data)
            }
            this.queryString = "";
            this.isPending = false;
            this.$closeLoading();
          }).catch(err => {
            console.log(JSON.stringify(err));
            this.queryString = "";
            this.isPending = false;
            this.$closeLoading();
            this.$alertDanger('请求超时，请刷新重试')
          })
        }
      },
      compareDate: function (dateFrom, dateTo) {
        let compFrom = new Date(dateFrom);
        let compTo = new Date(dateTo);
        return (compTo - compFrom);
      },
      downloadData: function (val) {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          let data = {
            id: val.ID,
          };

          axiosDownload({
            url: tableHistoryDownloadUrl,
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
    }
  }
</script>

<style scoped>
  .table-backups {
    padding: 10px 20px 0 0;
  }

  .options-area {
    background: #fff;
    border: 1px solid #eeeeee;
    border-radius: 8px;
    padding: 10px 20px;
    margin-bottom: 10px;
  }

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

  .form-row {
    display: flex;
    flex-wrap: wrap;
    align-items: flex-end;
  }

  .form-group {
    min-width: 200px;
    margin-right: 20px;
  }

  .options-area /deep/ .form-group label {
    line-height: 32px;
    display: block;
  }

  .options-area /deep/ .form-group .el-input {
    width: 220px;
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
