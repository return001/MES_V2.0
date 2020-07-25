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
      <el-table-column key="file_name"
                       prop='file_name'
                       label="文件名"
                       min-width=180
                       v-if="$route.query.type === '3' || $route.query.type === '4'">
      </el-table-column>


      <el-table-column
        type="index"
        :index="indexMethod"
        fixed="left"
        width="60">
      </el-table-column>

      <el-table-column
        label="文件操作"
        width="180"
        fixed="right"
        v-if="$route.query.type === '3' || $route.query.type === '4'"
      >
        <template slot-scope="scope" v-if="$route.query.type === '3' || $route.query.type === '4'">
          <span class="file-action-upload">
            <el-tooltip content="上传文件" placement="top">
              <el-button type="text" @click="showUpload(scope.row)" icon="el-icon-upload2"></el-button>
            </el-tooltip>
          </span>
          <span class="file-action-download">
            <el-tooltip content="下载文件" placement="top">
              <el-button type="text" @click="downloadFile(scope.row)" icon="el-icon-download" :disabled="!scope.row['file_name']"></el-button>
            </el-tooltip>
          </span>
        </template>
      </el-table-column>

      <el-table-column
        label="操作"
        width="180"
        fixed="left"
      >
        <template slot-scope="scope">
          <el-tooltip content="编辑" placement="top">
            <el-button type="text" @click="editData('edit', scope.row)" icon="el-icon-t-edit"></el-button>
          </el-tooltip>
          <el-tooltip content="复制" placement="top">
            <el-button type="text" @click="editData('copy', scope.row)" icon="el-icon-t-copy"></el-button>
          </el-tooltip>
          <span class="file-action-upload">
            <el-tooltip content="导出配置" placement="top">
              <el-button type="text" @click="exportTest(scope.row)" icon="el-icon-upload"></el-button>
            </el-tooltip>
          </span>
          <el-tooltip content="删除" placement="top">
            <el-button type="text" @click="delData(scope.row)" icon="el-icon-t-delete"></el-button>
          </el-tooltip>
        </template>
      </el-table-column>
    </el-table>

    <!--上传框-->
    <el-dialog
      title="上传文件"
      :visible.sync="isUpload"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @closed="clearOrderUploadFile"
      width="400px">
      <el-upload
        ref="fileUpload"
        :action="uploadGuideFile"
        :auto-upload="false"
        :http-request="uploadFile"
        :before-upload="beforeOrderUpload"
        accept="application/vnd.ms-excel,application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"
      >
        <el-button slot="trigger" size="small" type="info">选取文件</el-button>
        <el-button style="margin-left: 10px;" size="small" type="primary" @click="submitOrderUpload">上传</el-button>
<!--        <div slot="tip" class="upload-tip">请选择xls、xlsx文件</div>-->
      </el-upload>
    </el-dialog>

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
  import {axiosDownload, axiosFetch} from "../../../../utils/fetchData";
  import {getTestConfig, testSelectUrl} from "../../../../config/testApiConfig";
  import eventBus from "../../../../utils/eventBus";
  import {testOperUrl,uploadGuideFile,downloadGuideFile} from "../../../../config/testApiConfig";
  import {MessageBox} from 'element-ui'
  import {saveAs} from 'file-saver';
  import {planOrderImportUrl, planOrderTableDownloadUrl} from "../../../../config/globalUrl";

  export default {
    name: "Details",
    inject: ['reload'],
    data() {
      return {
        uploadGuideFile:uploadGuideFile,
        uploadFileData:null,
        tableData: [],
        tableColumns: [],
        //srcData: [],
        testIngeId:'',
        isUpload:false,         //上传框
        paginationOptions: {
          currentPage: 1,
          pageSize: 20,
          total: 0
        },
        isPending: false,
        queryString: '',
      }
    },
    computed: {},
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
          this.$openLoading();
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
            console.log(err);
            this.$alertDanger('请求超时，清刷新重试')
          }).finally(() => {
            this.$closeLoading();
            this.isPending = false;
          })
        } else {
          this.$closeLoading()
        }
      },
      indexMethod: function (index) {
        return index + (this.paginationOptions.currentPage - 1) * this.paginationOptions.pageSize + 1
      },
      showUpload(val){
        this.isUpload=true;
        this.testIngeId = val.SoftWare
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
            this.$openLoading();
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
              if (response.data.result === 200) {
                this.$alertSuccess('作废成功');
                let tempUrl = this.$route.fullPath;
                this.reload();
              } else {
                this.$alertWarning(response.data.data)
              }
            }).catch(err => {
              this.$alertDanger('请求超时，清刷新重试');
            }).finally(() => {
              this.$closeLoading();
              this.isPending = false;
            })
          }
        }).catch(() => {

        })
      },
      //导出配置
      exportTest(val){
        let filename
        switch (this.$route.query.type) {
          case '0':
            filename = 'SMT功能测试' + val.SoftWare
            break
          case '1':
            filename = '组装功能测试' + val.SoftWare
            break
          case '2':
            filename = '组装耦合测试' + val.SoftWare
            break
          case '3':
            filename = '研发功能测试' + val.SoftWare
            break
          case '4':
            filename = '研发耦合测试' + val.SoftWare
            break
          case '5':
            filename = 'OQC' + val.SoftWare
            break
        }
        let blob = new Blob([JSON.stringify(val)], {
          type: "text/plain;charset=utf-8"
        });
        saveAs(blob, filename + '.txt');
      },

      //上传文件
      uploadFile: function (params) {
        // this.uploadFileData.append(Math.floor(Math.random() * 1000), params.file)
        this.uploadFileData.append('uploadFile', params.file)
      },
      clearOrderUploadFile: function () {
        this.$refs.fileUpload.clearFiles();
      },
      beforeOrderUpload: function () {
        this.isPending = true;
        this.$openLoading();
      },

      submitOrderUpload: function () {
        this.uploadFileData = new FormData();
        this.$refs.fileUpload.submit();
        this.uploadFileData.append('#TOKEN#', this.$store.state.token);
        // this.uploadFileData.append('uploadFile', this.$store.state.token);
        this.uploadFileData.append('id', this.testIngeId);
        this.uploadFileData.append('type', this.$route.query.type);
        let config = {
          header: {
            'Content-Type': 'multipart/form-data'
          },
        };
        this.$axios.post(uploadGuideFile, this.uploadFileData, config).then(response => {
          if (response.data.result === 200) {
            this.$alertSuccess(response.data.data);
            // this.partlyReload();
            this.isUpload = false;
            setTimeout(()=>{
              this.reload();
            },600)
          } else {
            this.$alertWarning(response.data.data);
          }
        }).catch(err => {
        }).finally(() => {
          this.isPending = false;
          this.$closeLoading();
        });
      },

      //下载文件
      downloadFile: function (val) {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          axiosDownload({
            url: downloadGuideFile,
            data: {
              type:this.$route.query.type,
              id: val.SoftWare,
            }
          }).then(response => {
            let contentType = response.request.getResponseHeader('content-type');
            if (contentType === 'application/vnd.ms-excel' || contentType === 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet' || contentType === 'application/octet-stream') {
              let name = decodeURIComponent(escape(response.request.getResponseHeader('Content-disposition').split('=')[1]));
              let fileName = name.substring(1,name.length-1);

              saveAs(response.data, fileName)
            } else {
              let reader = new FileReader();
              reader.readAsText(response.data);
              reader.addEventListener('loadend', () => {
                this.$alertWarning(JSON.parse(reader.result).data)
              })
            }
          }).catch(err => {
            console.log(err)
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

  .file-action-upload /deep/ .el-button{
    display: inline-block;
    font-size: 20px;
    margin: 0 8px;
  }
  .file-action-download /deep/ .el-button{
    display: inline-block;
    font-size: 20px;
    margin-right: 5px;
  }

</style>
