<template>
  <div class="table-backups">
    <div class="options-area">
      <div class="form-row">
        <div v-for="item in queryOptions" class="row no-gutters pl-3 pr-3">
          <component :opt="item" :is="item.type + '-comp'"></component>
        </div>
        <div class="form-group-btn">
          <el-button type="primary" @click="thisFetch">查询</el-button>
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
  import {Datetime} from 'vue-datetime';
  import {tableHistoryDownloadUrl, tableHistoryQueryUrl} from "../../../config/globalUrl";
  import {axiosFetch} from "../../../utils/fetchData";
  import {mapActions} from 'vuex'

  export default {
    name: "Backups",
    components: {
      'text-comp': {
        props: ['opt', 'callback'],
        template: '<div class="form-group">\n' +
        '           <label :for="opt.id">{{opt.name}}</label>\n' +
        '           <el-input type="text" :id="opt.id" v-model="opt.model" @keyup.enter="callback" autocomplete="off"></el-input>\n' +
        '          </div>'
      },
      'date-comp': {
        props: ['opt'],
        components: {
          Datetime
        },
        template: '<div class="row" style="margin-right: 20px">\n' +
        '    <div class="form-group">\n' +
        '      <label>{{opt.name}}  从：</label>\n' +
        '      <el-date-picker\n' +
        '                size="large"\n' +
        '                v-model="opt.timeRange"\n' +
        '                clearable\n' +
        '                type="datetimerange"\n' +
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
        isDownloading: false
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
      ...mapActions(['setLoading']),
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
          });

          this.copyQueryOptions.map((item, index) => {
            switch (item.type) {
              case 'text':
                if (_.trim(item.model) !== "") {
                  if (index === 0) {
                    this.queryString += (item.id + "#like#" + _.trim(item.model))
                  } else {
                    this.queryString += ("#&#" + item.id + "#like#" + _.trim(item.model))
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
        });

      },
      thisFetch: function (opt) {
        if (opt === 'sizeChange') {
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
        if (this.isDownloading === false) {
          this.isDownloading = true;
          let data = {
            id: val.ID,
            '#TOKEN#': this.$store.state.token
          };

          this.downloadFile(tableHistoryDownloadUrl, data, val.FileName);

        } else {
          this.$alertInfo('请稍后再试')
        }

      },
      downloadFile: function (url, data, fileName) {

        this.$axios(
          {
            method: 'get',
            url: url,
            params: data,
            responseType: 'blob'
          }).then(res => {
          if (res.data.type !== "application/vnd.ms-excel") {
            this.$alertWarning("获取文件失败")
          } else {
            this.$alertSuccess('请求成功，请等待下载');
            let url = window.URL.createObjectURL(res.data);
            let link = document.createElement('a');
            link.style.display = 'none';
            link.href = url;
            link.setAttribute('download', fileName);
            document.body.appendChild(link);
            link.click();
          }
          this.isDownloading = false;
        }).catch(err => {
          this.isDownloading = false;
          console.log(JSON.stringify(err));
          this.$alertDanger('请求超时，请刷新重试');
        })
      }
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
    padding: 10px;
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

  .options-area /deep/ .el-input__inner {
    height: 38px;
    line-height: 38px;
    border: 1px solid #ced4da;
    display: flex;
  }

  .form-row {
    display: flex;
    flex-wrap: wrap;
    align-items: flex-end;
  }

  .form-group {
    min-width: 220px;
    margin-right: 20px;
  }

  .form-group .el-input {
    width: 220px;
  }

  .form-group /deep/ label {
    line-height: 40px;
    display: block;
  }

  .options-area /deep/ .form-group .el-input {
    width: 220px;
  }

  .options-area /deep/ .form-group label {
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
