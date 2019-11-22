<template>
  <div id="site-info">
    <div class="site-info-main">
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
        @selection-change="handleSelectionChange"
        stripe>
        <el-table-column
          v-if="_permissionControl(['SopManager', 'SopQcConfirmer'])"
          type="selection"
          width="55">
        </el-table-column>
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
          width="100"
          fixed="right"
        >
          <template slot-scope="scope">
            <el-tooltip content="预览" placement="top"
                        v-if="_permissionControl(['SopManager', 'SopReviewer', 'SopQcConfirmer'])">
              <el-button type="text" @click="previewSite(scope.row)" icon="el-icon-picture-outline"></el-button>
            </el-tooltip>
            <el-tooltip content="停止播放" placement="top"
                        v-if="_permissionControl(['SopManager', 'SopQcConfirmer'])">
              <el-button type="text" @click="showStopPlayingPanel(scope.row.id)" icon="el-icon-video-pause"></el-button>
            </el-tooltip>
            <el-tooltip content="状态" placement="top"
                        v-if="_permissionControl(['SopQcConfirmer'])">
              <el-button type="text"
                         @click="editStatus(scope.row)"
                         icon="el-icon-more"></el-button>
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

    <!--dialog-->
    <el-dialog
      :visible.sync="isPreviewing"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @closed="clearList"
      top="5vh"
      width="900px">
      <div class="preview-notice">
        <div class="preview-notice-item" v-for="(noticeItem, noticeIndex) in noticeList" :key="'notice-' + noticeIndex">
          <transition name="el-zoom-in-top">
            <el-alert
              v-if="showingNoticeIndex === noticeIndex"
              :title="noticeItem.title"
              type="info"
              :description="noticeItem.content"
              :closable="false"
              show-icon>
            </el-alert>
          </transition>
        </div>
      </div>

      <el-carousel
        :interval="interval * 1000"
        height="640px">
        <el-carousel-item v-for="(pic, picIndex) in pictureList" :key="'picture-' + picIndex">
          <img :src="'/' + pic.path" :alt="'pic-' + picIndex">

        </el-carousel-item>
      </el-carousel>
    </el-dialog>

    <el-dialog
      title="修改状态"
      :visible.sync="isStatusEditing"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @closed="resetStatusEdit"
      width="260px">
      <div class="edit-status">
        <el-select v-model="siteStatus"
                   placeholder="请选择状态"
                   size="small">
          <el-option value="审核通过" label="审核通过"></el-option>
          <el-option value="审核不通过" label="审核不通过"></el-option>
        </el-select>
      </div>
      <span slot="footer" class="dialog-footer">
          <el-button type="info" @click="isStatusEditing = false" size="small">取消</el-button>
          <el-button type="primary" @click="submitStatusEdit" size="small">保存</el-button>
      </span>
    </el-dialog>

    <el-dialog
      title="暂停播放"
      :visible.sync="isStoppingPlayingSite"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @close="resetStopping"
      width="260px">
      <div class="edit-status">
        <el-select v-model="stoppingType"
                   placeholder="请选择停播的内容"
                   size="small">
          <el-option :value="0" label="SOP文件内容"></el-option>
          <el-option :value="1" label="通知"></el-option>
          <el-option :value="2" label="SOP文件及通知"></el-option>
        </el-select>
      </div>
      <span slot="footer" class="dialog-footer">
          <el-button type="info" @click="isStoppingPlayingSite = false" size="small">取消</el-button>
          <el-button type="primary" @click="stopPlaying" size="small">确认</el-button>
      </span>
    </el-dialog>
  </div>
</template>

<script>
  import {SiteQueryConfig, SiteTableColumns,} from "../../../config/ESopConfig";
  import {
    eSopSiteSelectUrl,
    eSopSitePreviewUrl,
    eSopFileRecycleUrl,
    eSopSiteStatusEditUrl
  } from "../../../config/globalUrl";
  import common from "./mixins/common";
  import {axiosFetch} from "../../../utils/fetchData";
  import 'element-ui/lib/theme-chalk/base.css'

  export default {
    name: "SiteInfo",
    mixins: [common],
    data() {
      return {
        /*搜索框*/
        queryConfig: SiteQueryConfig,
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
        tableColumns: SiteTableColumns,
        tableData: [],

        /*通知、图片列表*/
        noticeList: [],
        pictureList: [],

        /*预览窗口*/
        isPreviewing: false,
        interval: 0,
        intervalId: null,
        showingNoticeIndex: 0,

        /*暂停相关*/
        selectionGroup: [],
        isStoppingPlayingSite: false,
        stoppingSingleSiteId: null,
        stoppingType: 0,

        /*修改状态*/
        isStatusEditing: false,
        siteStatus: '',
        statusEditingSiteId: null,



      }
    },
    mounted() {
      /*注册按键*/
      this.buttonGroup[0].callback = this._initQueryOptions;
      this.buttonGroup[1].callback = this._queryData;
      if (this._permissionControl(['SopManager', 'SopQcConfirmer'])) {
        this.buttonGroup.push(
          {
            label: '停播',
            size: 'small',
            callback: this.stopGroupPlaying
          })
      }
      this._queryData();
    },
    methods: {
      /*局部刷新*/
      partlyReload() {
        this._partlyReload(['buttonGroup', 'queryCompData', 'historyQueryButtonGroup'])
      },

      fetchData() {
        this.$openLoading();
        let options = {
          url: eSopSiteSelectUrl,
          data: {
            pageNo: this.paginationOptions.currentPage,
            pageSize: this.paginationOptions.pageSize
          }
        };
        Object.keys(this.queryCompData).forEach(key => {
          if (this.queryCompData[key] !== undefined && this.queryCompData[key] !== '') {
            options.data[key] = this.queryCompData[key];
          }
        });
        this._fetchData(options).then(data => {
          this.tableData = data.list;
        })
      },

      previewSite(row) {
        this.isPending = true;
        this.$openLoading();
        this.interval = row.switchInterval;
        axiosFetch({
          url: eSopSitePreviewUrl,
          data: {
            id: row.id
          }
        }).then(response => {
          if (response.data.result === 200) {
            this.noticeList = response.data.data.notices === null ? [] : response.data.data.notices;
            this.pictureList = response.data.data.pictures === null ? [] : response.data.data.pictures;

            if (this.noticeList.length === 0 && this.pictureList.length === 0) {
              this.$alertInfo('该站点未播放通知及SOP文件');
              return;
            }

            this.isPreviewing = true;
            if (this.noticeList.length > 1) {
              this.intervalId = setInterval(() => {
                if (this.showingNoticeIndex === this.noticeList.length - 1) {
                  this.showingNoticeIndex = 0
                } else {
                  this.showingNoticeIndex++;
                }

              }, this.interval * 1000)
            }
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

      clearList() {
        this.pictureList = [];
        this.noticeList = [];
        this.interval = 0;
        clearInterval(this.intervalId);
        this.intervalId = null;
      },

      handleSelectionChange(val) {
        this.selectionGroup = val;
      },

      showStopPlayingPanel(id) {
        if (this.selectionGroup.length !== 0) {
          this.$alertInfo('请先清空所选站点');
          return;
        }
        this.stoppingSingleSiteId = id;
        this.isStoppingPlayingSite = true;
      },

      stopGroupPlaying() {
        if (this.selectionGroup.length === 0) {
          this.$alertInfo('请选择站点')
        } else {
          this.isStoppingPlayingSite = true;
        }
      },

      stopPlaying() {
        this.isPending = true;
        this.$openLoading();
        let id;
        if (this.selectionGroup.length !== 0) {
          id = this.selectionGroup.map(item => item.id).toString()
        } else {
          id = this.stoppingSingleSiteId
        }
        axiosFetch({
          url: eSopFileRecycleUrl,
          data: {
            id: id,
            type: this.stoppingType
          }
        }).then(response => {
          if (response.data.result === 200) {
            this.$alertSuccess('停止成功');
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

      resetStopping() {
        this.$refs['tablecomponent'].clearSelection();
        this.stoppingSingleSiteId = null;
        this.stoppingType = 0;
      },

      /*编辑状态*/
      editStatus(row) {
        this.statusEditingSiteId = row.id;
        this.siteStatus = row.state;
        this.isStatusEditing = true;
      },

      resetStatusEdit() {
        this.statusEditingSiteId = null;
        this.siteStatus = '';
      },

      submitStatusEdit() {
        this.isPending = true;
        this.$openLoading();
        axiosFetch({
          url: eSopSiteStatusEditUrl,
          data: {
            id: this.statusEditingSiteId,
            state: this.siteStatus
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
    }
  }
</script>

<style scoped>
  @import "styles/common.css";

  #site-info {
    position: relative;
    width: 100%;
    min-height: 600px;
    height: 100%;
  }

  .preview-notice {
    height: 56px;
    overflow: hidden;
    margin-bottom: 10px;
  }

  .preview-notice-item {
    position: relative;
    top: 0;
    width: 100%;
    box-sizing: border-box;
  }

  .el-carousel img {
    width: 100%;
    height: 100%;
  }
</style>
