<template>

  <div id="file-manager">
    <div class="file-manager-main">
      <div class="query-comp">
        <rz-query-bar
          :list="queryConfig"
          :button-group="buttonGroup"
          :data.sync="queryCompData"/>
      </div>
    </div>

    <div class="content-comp">
      <el-table
        :data="tableData"
        max-height="560"
        ref="tablecomponent"
        stripe>
        <el-table-column v-for="(item, index) in tableColumns"
                         :key="index"
                         :prop="item.key"
                         :label="item.label"
                         :min-width="item['min-width']"
                         :formatter="item.formatter">
        </el-table-column>

        <el-table-column
          type="index"
          :index="_indexMethod"
          fixed="left"
          width="60">
        </el-table-column>

        <el-table-column
          label="操作"
          width="160"
          fixed="right"
        >
          <template slot-scope="scope">
            <el-tooltip content="状态" placement="top"
                        v-if="_permissionControl(['SopReviewer'])">
              <el-button type="text"
                         @click="editStatus(scope.row)"
                         icon="el-icon-more"
                         :disabled="scope.row.state !== '待审核'"></el-button>
            </el-tooltip>
            <el-tooltip content="发放" placement="top"
                        v-if="_permissionControl(['SopManager'])">
              <el-button type="text"
                         @click="preloadDispatching(scope.row.id)"
                         icon="el-icon-upload2"
                         :disabled="scope.row.state === '已作废' || scope.row.state === '待审核'"></el-button>
            </el-tooltip>
            <el-tooltip content="日志" placement="top"
                        v-if="_permissionControl(['SopManager', 'SopReviewer', 'SopQcConfirmer'])">
              <el-button type="text"
                         @click="showDispatchingLog(scope.row.id)"
                         icon="el-icon-document"></el-button>
            </el-tooltip>
            <el-tooltip content="下载" placement="top"
                        v-if="_permissionControl(['SopManager', 'SopReviewer'])">
              <el-button type="text"
                         @click="downloadSop(scope.row.id)"
                         icon="el-icon-download"
                         :disabled="scope.row.state === '已作废'"></el-button>
            </el-tooltip>
            <el-tooltip content="作废" placement="top"
                        v-if="_permissionControl(['SopManager', 'SopReviewer'])">
              <el-button type="text"
                         @click="_deleteData(scope.row)"
                         icon="el-icon-delete"
                         :disabled="scope.row.state === '已作废'"></el-button>
            </el-tooltip>
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
        @current-change="fetchData"
        @size-change="_queryData"
        class="page-pagination">
      </el-pagination>
    </div>
    <!--dialog component-->
    <el-dialog
      id="dispatching-dialog"
      title="文件发放"
      :visible.sync="isDispatching"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @close="resetDispatching"
      width="700px">
      <el-divider content-position="left">1、请选择产线</el-divider>
      <div class="dispatch-query">
        <div class="dispatch-query-item">
          <div class="query-label">厂区</div>
          <el-select
            size="mini"
            v-model="lineQueryData.factoryId"
            placeholder="请选择厂区"
            @change="setWorkshopList">
            <el-option
              v-for="item in factoryList"
              :value="item.key"
              :key="'factory-item-'+item.key"
              :label="item.label"></el-option>
          </el-select>
        </div>
        <div class="dispatch-query-item">
          <div class="query-label">车间</div>
          <el-select
            :disabled="lineQueryData.factoryId === null"
            size="mini"
            v-model="lineQueryData.workshopId"
            placeholder="请选择车间"
            @change="setLineList">
            <el-option
              v-for="item in workshopList"
              :value="item.key"
              :key="'workshop-item-'+item.key"
              :label="item.label"></el-option>
          </el-select>
        </div>
        <div class="dispatch-query-item">
          <div class="query-label">产线</div>
          <el-select
            :disabled="lineQueryData.workshopId === null"
            size="mini"
            v-model="lineQueryData.lineId"
            placeholder="请选择产线"
            @change="setSiteList">
            <el-option
              v-for="item in lineList"
              :value="item.key"
              :key="'line-item-'+item.key"
              :label="item.label"></el-option>
          </el-select>
        </div>
      </div>
      <el-divider content-position="left">2、请预览站点内容或重新分配站点内容</el-divider>
      <div class="dispatch-site">

        <el-table
          size="small"
          :data="siteListWithPic"
          max-height="400"
          ref="sitetablecomponent"
          stripe>
          <el-table-column v-for="(item, index) in siteColumns"
                           :key="index"
                           :prop="item.key"
                           :label="item.label"
                           :min-width="item['min-width']"
                           :formatter="item.formatter">
          </el-table-column>
          <el-table-column
            label="待分配页面"
            width="200px">
            <template slot-scope="scope">
              <el-tag
                size="small"
                closable
                v-for="(picItem, picIdx) in scope.row.picList"
                :key="'pic-item-' + picItem.pictureNumber"
                @close="removePicTag(scope.$index, picIdx)">
                {{picItem.pictureName.split('.')[0]}}
              </el-tag>
            </template>
          </el-table-column>

          <el-table-column
            label="操作"
            width="80"
            fixed="right"
          >
            <template slot-scope="scope">
              <el-tooltip content="预览" placement="top">
                <el-button type="text" @click="previewSite(scope.row)" icon="el-icon-picture-outline"></el-button>
              </el-tooltip>
              <el-tooltip content="添加" placement="top">
                <el-button type="text" @click="addPageToSite(scope.row, scope.$index)" icon="el-icon-plus"></el-button>
              </el-tooltip>
            </template>
          </el-table-column>
        </el-table>
      </div>
      <span slot="footer" class="dialog-footer">
         <el-button size="small" @click="isDispatching = false" type="info">取消</el-button>
         <el-button size="small" @click="reGenerateSitePicGroup">重新分配</el-button>
         <el-button size="small" @click="saveDispatching" type="primary">保存</el-button>
       </span>

      <el-dialog
        title="添加"
        :visible.sync="isImportingPage"
        :close-on-click-modal="false"
        :close-on-press-escape="false"
        @closed="resetPageImport"
        width="600px"
        append-to-body>
        <div class="page-import-content" v-if="isImportingPage">
          <div class="page-box">
            <div class="page-box-title">
              待分配页面
            </div>
            <div class="page-box-content">
              <div class="page-tag"
                   v-for="(remainItem, index) in remainingPageList"
                   :key="'remaining-' + index">
                {{remainItem.pictureName.split('.')[0]}}
                <el-button icon="el-icon-plus"
                           circle
                           class="mini-btn"
                           @click="addPageInSite(index)"></el-button>
              </div>
            </div>
          </div>
          <el-divider direction="vertical"></el-divider>
          <div class="page-box">
            <div class="page-box-title">
              {{importingSite.siteNumber}}
            </div>
            <div class="page-box-content">
              <div class="page-tag"
                   v-for="(sitePageItem, index) in importingSite.picList"
                   :key="'site-page-item' + index">
                {{sitePageItem.pictureName.split('.')[0]}}
                <el-button icon="el-icon-minus"
                           circle
                           class="mini-btn"
                           @click="removePicTagInSite(index)"></el-button>
              </div>
            </div>
          </div>
        </div>
        <span slot="footer" class="dialog-footer">
         <el-button size="small" @click="isImportingPage = false">确认</el-button>
       </span>
      </el-dialog>

      <el-dialog
        :visible.sync="isPreviewing"
        :close-on-click-modal="false"
        :close-on-press-escape="false"
        @closed="clearList"
        width="880px"
        append-to-body>
        <div class="carousel-title">正在预览：站点{{previewingSite.siteNumber}}</div>
        <el-carousel
          :interval="interval * 1000"
          height="560px">
          <el-carousel-item v-for="(pic, picIndex) in pictureList" :key="'picture-' + picIndex">
            <img :src="'/' + pic.path" :alt="'pic-' + picIndex">

          </el-carousel-item>
        </el-carousel>
      </el-dialog>
    </el-dialog>

    <el-dialog
      title="上传"
      :visible.sync="isUploading"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @closed="clearUploadFile"
      width="400px">
      <el-upload
        ref="fileUploadRef"
        action="#"
        :auto-upload="false"
        :http-request="uploadFile"
        :before-upload="beforeUpload"
        :on-change="checkFile"
        accept="application/vnd.ms-excel,application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"
      >
        <el-button slot="trigger" size="small" type="info">选取文件</el-button>
        <el-button style="margin-left: 10px;" size="small" type="primary"
                   @click="submitFileUpload">上传
        </el-button>
        <div slot="tip" class="upload-tip">请选择xls、xlsx文件</div>
      </el-upload>
    </el-dialog>

    <el-dialog
      title="修改状态"
      :visible.sync="isStatusEditing"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @closed="resetStatusEdit"
      width="260px">
      <div class="edit-status">
        <el-select v-model="fileStatus"
                   placeholder="请选择状态"
                   size="small"
                   :disabled="fileStatus === '已作废'">
          <el-option value="待审核" label="待审核"
                     :disabled="fileStatus === '已审核'"></el-option>
          <el-option value="已审核" label="已审核"></el-option>
          <!--          <el-option value="已作废" label="已作废"></el-option>-->
        </el-select>
      </div>
      <span slot="footer" class="dialog-footer">
          <el-button type="info" @click="isStatusEditing = false" size="small">取消</el-button>
          <el-button type="primary" @click="submitStatusEdit" size="small">保存</el-button>
      </span>
    </el-dialog>
    <el-dialog
      title="发放历史"
      :visible.sync="isHistoryShowing"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @closed="resetHistoryPage"
      class="history-dialog"
      width="700px">
      <div class="query-comp">
        <rz-query-bar
          :list="historyQueryConfig"
          :button-group="historyQueryButtonGroup"
          :data.sync="historyQueryData"/>
      </div>
      <div class="history-content">
        <el-table
          size="mini"
          :data="fileHistoryData"
          max-height="500"
          ref="historytablecomponent"
          stripe>
          <el-table-column v-for="(item, index) in fileHistoryColumns"
                           :key="index"
                           :prop="item.key"
                           :label="item.label"
                           :min-width="item['min-width']"
                           :formatter="item.formatter">
          </el-table-column>

          <el-table-column
            type="index"
            :index="_indexMethod"
            fixed="left"
            width="40">
          </el-table-column>

          <el-table-column
            label="操作"
            width="60"
            fixed="right"
          >
            <template slot-scope="scope">
              <el-tooltip content="详细" placement="top">
                <el-button type="text"
                           @click="queryHistoryDetailsData(scope.row.id)"
                           icon="el-icon-more"></el-button>
              </el-tooltip>
            </template>
          </el-table-column>
        </el-table>
      </div>

      <el-dialog
        title="详细"
        :visible.sync="isHistoryDetailsShowing"
        :close-on-click-modal="false"
        :close-on-press-escape="false"
        @closed="resetHistoryDetailsPage"
        width="700px"
        append-to-body>

        <div class="history-content">
          <el-table
            size="mini"
            :data="fileHistoryDetailsData"
            max-height="500"
            ref="historydetailstablecomponent"
            stripe>
            <el-table-column v-for="(item, index) in fileHistoryDetailsColumns"
                             :key="index"
                             :prop="item.key"
                             :label="item.label"
                             :min-width="item['min-width']"
                             :formatter="item.formatter">
            </el-table-column>

            <el-table-column
              type="index"
              :index="_indexMethod"
              fixed="left"
              width="40">
            </el-table-column>
          </el-table>
        </div>
      </el-dialog>
    </el-dialog>
  </div>
</template>

<script>
  import {
    FileQueryConfig,
    FileFormItems,
    FileTableColumns,
    FileFormRules,
    FileHistoryDetailsTableColumns,
    FileHistoryQueryConfig,
    FileHistoryTableColumns
  } from "../../../config/ESopConfig";
  import {
    eSopFileSelectUrl,
    eSopFileDeleteUrl,
    eSopFileAddUrl,
    eSopFileEditUrl,
    eSopFileUploadUrl,
    eSopFilePictureSelectUrl,
    eSopFactorySelectUrl,
    eSopWorkshopSelectUrl,
    planLineGetUrl,
    eSopSiteSelectUrl,
    eSopFileDispatchUrl,
    eSopFileHistoryDetailsSelectUrl,
    eSopFileHistorySelectUrl,
    eSopFileDownloadUrl
  } from "../../../config/globalUrl";
  import {axiosFetch, axiosDownload} from "../../../utils/fetchData";
  import common from "./mixins/common";
  import file_notice from "./mixins/file_notice"

  export default {
    name: "FileManager",
    inject: ['reload'],
    mixins: [common, file_notice],
    data() {
      return {
        /*搜索框*/
        queryConfig: FileQueryConfig,
        buttonGroup: [
          {
            label: '重置条件',
            size: 'small',
            type: 'info',
            callback() {
            }
          },
          {
            label: '查询',
            size: 'small',
            type: 'primary',
            callback() {
            }
          },
        ],
        queryCompData: {},
        tableColumns: FileTableColumns,
        tableData: [],

        editPanelTitle: '',
        isEditing: false,
        editType: 0, //0: add, 1: edit

        formConfig: {
          refName: 'fileFormComp',
          size: 'small',
          identity: 'file-item-'
        },
        formItemList: FileFormItems,
        formRules: FileFormRules,
        formData: {},

        /*链接重定义*/
        formAddUrl: eSopFileAddUrl,
        formDeleteUrl: eSopFileDeleteUrl,
        formUpdateUrl: eSopFileEditUrl,

        /*文件上传*/
        isUploading: false,
        uploadFileData: null,

        /*状态修改*/
        statusEditingFileId: null,
        isStatusEditing: false,
        fileStatus: '',

        /*文件发放*/
        /*mixins/file_notice.js*/
        //站点详情
        siteListWithPic: [],
        siteColumns: [
          {
            'label': '站点编号',
            'key': 'siteNumber',
            'min-width': '60px'
          },
          {
            'label': '站点名称',
            'key': 'siteName',
            'min-width': '70px'
          },
          {
            'label': '工序顺序',
            'key': 'processOrder',
            'min-width': '60px'
          },
        ],
        //添加分配页面
        isImportingPage: false,
        importingSiteIndex: null,
        importingSite: {
          siteNumber: '',
          picList: [],
        },

        //预览页面
        isPreviewing: false,
        interval: 0,
        pictureList: [],
        previewingSite: {
          siteNumber: ''
        },

        //发放记录
        isHistoryShowing: false,
        isHistoryDetailsShowing: false,
        historyQueryConfig: FileHistoryQueryConfig,
        historyQueryButtonGroup: [
          {
            label: '重置条件',
            size: 'mini',
            type: 'info',
            callback() {
            }
          },
          {
            label: '查询',
            size: 'mini',
            type: 'primary',
            callback() {
            }
          }
        ],
        historyQueryData: {},
        fileHistoryColumns: FileHistoryTableColumns,
        fileHistoryDetailsColumns: FileHistoryDetailsTableColumns,
        fileHistoryData: [],
        fileHistoryDetailsData: [],
        queryingFileId: null,

      }
    },
    mounted() {
      /*注册按键*/
      this.buttonGroup[0].callback = this._initQueryOptions;
      this.buttonGroup[1].callback = this._queryData;
      if (this._permissionControl(['SopManager'])) {
        this.buttonGroup.push(
          {
            label: '上传',
            size: 'small',
            type: 'primary',
            callback: () => {
              this.isUploading = true
            }
          });
      }

      this.historyQueryButtonGroup[0].callback = this.initHistoryQueryOptions;
      this.historyQueryButtonGroup[1].callback = this.queryHistoryDataBtn;

      this._queryData();
    },
    methods: {
      /*局部刷新*/
      partlyReload() {
        this._partlyReload(['buttonGroup', 'queryCompData', 'historyQueryButtonGroup'])
      },

      /*查询、获取表格内容*/
      fetchData() {
        this.$openLoading();
        let options = {
          url: eSopFileSelectUrl,
          data: {
            pageNo: this.paginationOptions.currentPage,
            pageSize: this.paginationOptions.pageSize
          }
        };
        Object.keys(this.queryCompData).forEach(key => {
          if (!!this.queryCompData[key] || this.queryCompData[key] === 0) {
            if (key === 'reviewTime') {
              options.data.reviewStartTime = this.queryCompData[key][0];
              options.data.reviewEndTime = this.queryCompData[key][1];
            } else {
              options.data[key] = this.queryCompData[key];
            }
          }
        });
        this._fetchData(options).then(data => {
          this.tableData = data.list;
        })
      },

      /*编辑状态*/
      editStatus(row) {
        this.statusEditingFileId = row.id;
        this.fileStatus = row.state;
        this.isStatusEditing = true;
      },

      resetStatusEdit() {
        this.statusEditingFileId = null;
        this.fileStatus = '';
      },

      submitStatusEdit() {
        this.isPending = true;
        this.$openLoading();
        axiosFetch({
          url: this.formUpdateUrl,
          data: {
            id: this.statusEditingFileId,
            state: this.fileStatus
          }
        }).then(response => {
          if (response.data.result === 200) {
            this.$alertSuccess('更新成功');
            this.partlyReload();
          } else {
            this.$alertWarning(response.data.data)
          }
        }).catch(err => {
          this.$alertDanger("未知错误")
        }).finally(() => {
          this.$closeLoading();
        })
      },


      /*上传文件*/

      submitFileUpload() {
        this.isPending = true;
        this.$openLoading();
        this.uploadFileData = new FormData();
        this.$refs['fileUploadRef'].submit();
        this.uploadFileData.append('#TOKEN#', this.$store.state.token);
        let config = {
          header: {
            'Content-Type': 'multipart/form-data'
          },
        };
        this.$axios.post(eSopFileUploadUrl, this.uploadFileData, config).then(response => {
          if (response.data.result === 200) {
            this.$alertSuccess(response.data.data);
            this.isPending = false;
            this.resetUploadDialog();
            this.reload();
            this.showDetails(this.showingItem);
          } else {
            this.$alertWarning(response.data.data);
          }

        }).catch(err => {

        }).finally(() => {

          this.isPending = false;
          this.$closeLoading();
        });
      },

      uploadFile(params) {
        this.uploadFileData.append(Math.floor(Math.random() * 1000), params.file)
      },
      clearUploadFile() {
        this.$refs['fileUploadRef'].clearFiles();
      },
      beforeUpload() {
        this.isPending = true;
        this.$openLoading();
      },
      checkFile(file, fileList) {
        if (fileList.length > 1) {
          this.$refs['fileUploadRef'].uploadFiles.splice(0, fileList.length - 1)
        }
      },
      resetUploadDialog() {
        this.isUploading = false;
        this.uploadFileData = null;
      },

      /*文件下载*/
      downloadSop(fileId) {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          axiosDownload({
            url: eSopFileDownloadUrl,
            data: {
              id: fileId
            }
          }).then(response => {
            let contentType = response.request.getResponseHeader('content-type');
            if (contentType === 'application/vnd.ms-excel' || contentType === 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet') {
              let name = response.request.getResponseHeader('Content-Disposition').split('=')[1];
              saveAs(response.data, decodeURIComponent(name))
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

      /*文件分发*/
      /*点击指定项目分发，预加载厂区列表及文件图片信息*/
      preloadDispatching(id) {
        this.isPending = true;
        this.$openLoading();
        Promise.all([
          this.analyseFile(id),
          this.getDataList(eSopFactorySelectUrl, '厂区')
        ]).then(data => {
          this.factoryList = data[1].map(item => {
            return {
              key: item.id,
              label: item.abbreviation,
            }
          });
          this.isDispatching = true;
          this.isPending = false;
          this.$closeLoading();
        })
      },

      analyseFile(id) {
        return new Promise(resolve => {
          axiosFetch({
            url: eSopFilePictureSelectUrl,
            data: {
              pageNo: 1,
              pageSize: 10000,
              fileId: id
            }
          }).then(response => {
            if (response.data.result === 200) {
              this.pageList = [...response.data.data.list];
              this.remainingPageList = [...response.data.data.list];
              resolve();
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.$alertDanger("未知错误")
          })
        })
      },

      /*选择产线后显示所有站点及内容预分配*/
      setSiteList(val) {
        this.isPending = true;
        this.$openLoading();
        this.getDataList(eSopSiteSelectUrl, '站点', {
          lineId: val,
          orderByProcessOrder: true
        }).then(data => {
          if (data.length === 0) {
            this.$alertInfo('该产线下未设置站点')
          }
          this.siteList = data;
          this.generateSitePicGroup();
          this.isPending = false;
          this.$closeLoading();
        })
      },

      generateSitePicGroup() {
        this.siteListWithPic = JSON.parse(JSON.stringify(this.siteList));
        //let index = 0;
        let siteList = this.siteList;
        let pageList = this.pageList;
        for (let i = 0; i < siteList.length; i++) {
          this.siteListWithPic[i].picList = [];
          for (let j = 0; j < pageList.length; j++) {
            if (parseInt(pageList[j].pictureNumber) === siteList[i].processOrder) {
              this.siteListWithPic[i].picList.push(pageList[j]);
              //待分配列表删除对应页面
              for (let k = 0; k < this.remainingPageList.length; k++) {
                if (this.remainingPageList[k].pictureNumber === pageList[j].pictureNumber) {
                  this.remainingPageList.splice(k, 1);
                  break;
                }
              }
            }
          }
        }
      },

      removePicTag(rowIndex, picIndex) {
        //由于tag的v-for无法深度监听，所以重新set整行数据
        this.remainingPageList.push(this.siteListWithPic[rowIndex].picList[picIndex]);
        let tempRow = JSON.parse(JSON.stringify(this.siteListWithPic[rowIndex]));
        tempRow.picList.splice(picIndex, 1);
        this.$set(this.siteListWithPic, rowIndex, tempRow)
      },

      //站点-添加-移除已分配
      removePicTagInSite(index) {
        this.removePicTag(this.importingSiteIndex, index);
        this.importingSite.picList.splice(index, 1);
      },

      //站点-添加-将待分配移至已分配
      addPageInSite(index) {
        this.importingSite.picList.push(this.remainingPageList[index]);
        this.$set(this.siteListWithPic, this.importingSiteIndex, this.importingSite);
        this.remainingPageList.splice(index, 1);
      },

      addPageToSite(siteRow, index) {
        this.importingSiteIndex = index;
        this.importingSite = siteRow;
        this.isImportingPage = true;
      },

      resetPageImport() {
        this.importingSite = {
          siteNumber: '',
          picList: [],
        };
        this.importingSiteIndex = null;
      },

      reGenerateSitePicGroup() {
        this.remainingPageList = [...this.pageList];
        this.$set(this.$data, 'siteListWithPic', []);
        this.importingSiteIndex = null;
        this.importingSite = {
          siteNumber: '',
          picList: [],
        };
        this.generateSitePicGroup();
      },

      resetDispatching() {
        this.pageList = [];
        this.remainingPageList = [];
        this.lineQueryData = {
          factoryId: null,
          workshopId: null,
          lineId: null
        };
        this.factoryList = [];
        this.workshopList = [];
        this.lineList = [];
        this.siteList = [];
        this.siteListWithPic = [];
      },

      previewSite(row) {
        if (row.picList.length === 0) {
          this.$alertInfo('请先分配页面');
          return;
        }
        this.previewingSite = row;
        this.interval = row.switchInterval;
        this.pictureList = row.picList.map(item => {
          return {
            path: item.picturePath
          }
        });
        this.isPreviewing = true;

      },

      clearList() {
        this.pictureList = [];
        this.interval = 0;
        this.previewingSite = {};
      },

      saveDispatching() {
        if (this.siteListWithPic.length === 0 || this.pageList.length === this.remainingPageList.length) {
          this.$alertInfo('请先执行站点内容分配操作');
          return;
        }

        this.isPending = true;
        this.$openLoading();
        let options = {
          url: eSopFileDispatchUrl,
          data: {},
        };
        options.data.list = JSON.stringify(this.siteListWithPic.map(siteItem => {
          let picList = siteItem.picList.map(picItem => {
            return picItem.id
          });
          return {
            id: siteItem.id,
            pictureList: picList,
            noticeList: [],
          }
        }));
        axiosFetch(options).then(response => {
          if (response.data.result === 200) {
            this.$alertSuccess('发布成功');
            this.partlyReload();
          } else {
            this.$alertWarning(response.data.data)
          }
        }).catch(err => {
          this.$alertDanger("未知错误")
        }).finally(() => {
          this.isPending = false;
          this.$closeLoading();
        })
      },

      showDispatchingLog(fileId) {
        this.isPending = true;
        this.$openLoading();
        this.queryHistoryData(fileId).then(data => {
          this.queryingFileId = fileId;
          this.fileHistoryData = data;
          this.isPending = false;
          this.$closeLoading();
          this.isHistoryShowing = true;
        });
      },

      initHistoryQueryOptions() {
        this.historyQueryData = {}
      },

      queryHistoryDataBtn() {
        this.showDispatchingLog(this.queryingFileId);
      },

      queryHistoryData(id) {
        return new Promise(resolve => {
          let options = {
            url: eSopFileHistorySelectUrl,
            data: {
              pageNo: 1,
              pageSize: 65535,
              fileId: id
            }
          };
          if (!!this.historyQueryData.pushPerson) {
            options.data.pushPerson = this.historyQueryData.pushPerson
          }
          if (!!this.historyQueryData.time) {
            options.data.startTime = this.historyQueryData.time[0];
            options.data.endTime = this.historyQueryData.time[1];
          }
          axiosFetch(options).then(response => {
            if (response.data.result === 200) {
              resolve(response.data.data.list);
            } else {
              this.$alertWarning(response.data.data);
            }
          }).catch(() => {
            this.$alertDanger(`未知错误`)
          }).finally(() => {
            this.$closeLoading();
          })
        })
      },

      queryHistoryDetailsData(historyId) {
        this.isPending = true;
        this.$openLoading();
        axiosFetch({
          url: eSopFileHistoryDetailsSelectUrl,
          data: {
            fileHistoryId: historyId
          }
        }).then(response => {
          if (response.data.result === 200) {
            if (response.data.data) {
              this.fileHistoryDetailsData = response.data.data.map(historyItem => {
                historyItem.pictures = historyItem.pictures.map(item => {
                  return item.pictureName
                }).toString();
                return historyItem;
              });
              this.isHistoryDetailsShowing = true;
            } else {
              this.$alertInfo('无详细记录')
            }
          } else {
            this.$alertWarning(response.data.data);
          }
        }).catch(() => {
          this.$alertDanger(`未知错误`)
        }).finally(() => {
          this.isPending = false;
          this.$closeLoading();
        })

      },

      resetHistoryPage() {
        this.queryingFileId = null;
        this.historyQueryData = {};
        this.fileHistoryData = [];
        this.fileHistoryDetailsData = [];
      },

      resetHistoryDetailsPage() {
        this.fileHistoryDetailsData = [];
      }

    }
  }
</script>
<style scoped>
  @import "styles/common.css";

  #file-manager {
    position: relative;
    width: 100%;
    min-height: 600px;
    height: 100%;
  }

  #dispatching-dialog /deep/ .el-dialog__body {
    padding: 10px 20px;
  }

  .query-label {
    font-size: 14px;
    line-height: 20px;
    padding: 4px 0;
    color: #5d6066;
    display: block;
    cursor: default;
  }

  .dispatch-query {
    box-sizing: border-box;
    padding: 10px 20px;
    display: flex;

  }

  .dispatch-query-item {
    padding: 0 5px;
  }

  .dispatch-site {
    padding: 0 20px;
  }

  .dispatch-site .el-tag {
    margin: 2px;
  }

  .page-import-content {
    display: flex;
  }

  .page-import-content .el-divider {
    min-height: 200px;
    display: block;
  }

  .page-box {
    box-sizing: border-box;
    width: 48%;
    min-height: 200px;
  }

  .page-box-title {
    color: #5d6066;
    width: 100%;
    text-align: center;
  }

  .page-box-content {
    padding: 10px 5px;
  }

  .page-tag {
    display: inline-block;
    margin: 3px 4px;
    border: 1px solid #4f97ff;
    background: #4f97ff;
    color: #fff;
    box-sizing: border-box;
    padding: 2px 5px;
    border-radius: 4px;
    line-height: 18px;
    font-size: 14px;
  }

  .page-tag .mini-btn {
    padding: 2px;
    border: none;
    color: #4f97ff;
    font-weight: lighter;
    font-size: 12px;
    transform: scale(0.8);
  }

  .carousel-title {
    width: 100%;
    text-align: center;
    margin-bottom: 5px;
  }

  .el-carousel img {
    width: 100%;
    height: 100%;
  }

  .history-dialog .rz-form-query-bar {
    border: none;
  }

  .history-dialog /deep/ .el-dialog__body {
    padding-top: 10px;
  }

  .history-content {
    padding: 0 20px;
    box-sizing: border-box;
  }
</style>
