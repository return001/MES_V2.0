<template>

  <div id="notice-manager">
    <div class="notice-manager-main">
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
          width="100"
          fixed="right"
        >
          <template slot-scope="scope">
            <el-tooltip content="编辑" placement="top">
              <el-button type="text" @click="editData(scope.row)" icon="el-icon-edit-outline"></el-button>
            </el-tooltip>
            <el-tooltip content="删除" placement="top">
              <el-button type="text" @click="_deleteData(scope.row)" icon="el-icon-delete"></el-button>
            </el-tooltip>
            <el-tooltip content="发放" placement="top">
              <el-button type="text" @click="dispatchNotice(scope.row)" icon="el-icon-upload2"></el-button>
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
      :title="editPanelTitle"
      :visible.sync="isEditing"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @close="resetEditForm"
      width="500px">
      <rz-form-item
        class="notice-edit-form"
        :ref="formConfig.refName"
        v-if="isEditing"
        :config="formConfig"
        :list="formItemList"
        :rules="formRules"
        :data.sync="formData"/>
      <span slot="footer" class="dialog-footer">
         <el-button size="small" @click="closeEditPanel" type="info">取消</el-button>
         <el-button size="small" @click="submitFormEdit" type="primary">保存</el-button>
       </span>
    </el-dialog>


    <el-dialog
      id="dispatching-dialog"
      title="通知发放"
      :visible.sync="isDispatching"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @close="resetDispatching"
      width="700px">
      <el-divider content-position="left">请选择产线</el-divider>
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
      <span slot="footer" class="dialog-footer">
         <el-button size="small" @click="isDispatching = false" type="info">取消</el-button>
         <el-button size="small" @click="saveDispatching" type="primary"
                    :disabled="siteList.length === 0">发放至该产线</el-button>
       </span>
    </el-dialog>

  </div>
</template>

<script>
  import {NoticeQueryConfig, NoticeFormItems, NoticeTableColumns, NoticeFormRules} from "../../../config/ESopConfig";
  import {
    eSopNoticeSelectUrl,
    eSopNoticeDeleteUrl,
    eSopNoticeAddUrl,
    eSopNoticeEditUrl,
    eSopFactorySelectUrl,
    eSopSiteSelectUrl,
    eSopFileDispatchUrl
  } from "../../../config/globalUrl";
  import {axiosFetch} from "../../../utils/fetchData";
  import common from "./mixins/common";
  import file_notice from "./mixins/file_notice";

  export default {
    name: "NoticeManager",
    inject: ['reload'],
    mixins: [common, file_notice],
    data() {
      return {
        /*搜索框*/
        queryConfig: NoticeQueryConfig,
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
        tableColumns: NoticeTableColumns,
        tableData: [],

        editPanelTitle: '',
        isEditing: false,
        editType: 0, //0: add, 1: edit

        formConfig: {
          refName: 'noticeFormComp',
          size: 'small',
          identity: 'notice-item-'
        },
        formItemList: NoticeFormItems,
        formRules: NoticeFormRules,
        formData: {},

        /*链接重定义*/
        formAddUrl: eSopNoticeAddUrl,
        formDeleteUrl: eSopNoticeDeleteUrl,
        formUpdateUrl: eSopNoticeEditUrl,

        /*通知发放*/
        dispatchingNoticeId: null,


      }
    },
    mounted() {
      /*注册按键*/
      this.buttonGroup[0].callback = this._initQueryOptions;
      this.buttonGroup[1].callback = this._queryData;
      this.buttonGroup.push(
        {
          label: '新增',
          size: 'small',
          type: 'primary',
          callback: this.addData
        });

      this._queryData();
    },
    methods: {
      /*局部刷新*/
      partlyReload() {
        this._partlyReload(['buttonGroup', 'queryCompData'])
      },
      /*查询、获取表格内容*/
      fetchData() {
        this.$openLoading();
        let options = {
          url: eSopNoticeSelectUrl,
          data: {
            pageNo: this.paginationOptions.currentPage,
            pageSize: this.paginationOptions.pageSize
          }
        };
        Object.keys(this.queryCompData).forEach(key => {
          if (!!this.queryCompData[key] || this.queryCompData[key] === 0) {
            if (key === 'startTime' || key === 'endTime') {
              options.data[key + 'From'] = this.queryCompData[key][0];
              options.data[key + 'To'] = this.queryCompData[key][1];
            } else {
              options.data[key] = this.queryCompData[key];
            }
          }
        });
        this._fetchData(options).then(data => {
          this.tableData = data.list;
        })
      },

      /*编辑数据时初始化表单数据*/
      initFormData(row) {
        this.formItemList.forEach(item => {
          this.formData[item.prop] = row[item.prop]
        })
      },

      /*新增、编辑操作*/
      addData() {
        this.editType = 0;
        this.editPanelTitle = '新增项目';
        this.isEditing = true;
      },

      editData(row) {
        this.editType = 1;
        this.editPanelTitle = '编辑项目';
        this.formData.id = row.id;
        this.initFormData(row);
        this.isEditing = true;
      },

      resetEditForm() {
        this.formData = {};
      },

      closeEditPanel() {
        this.isEditing = false;
      },

      submitFormEdit() {
        let url;
        if (this.editType === 0) {
          url = this.formAddUrl
        } else {
          url = this.formUpdateUrl
        }
        if (!this.isPending) {
          this.$refs[this.formConfig.refName].validate(isValid => {
            if (isValid) {
              this.isPending = true;
              this.$openLoading();
              axiosFetch({
                url: url,
                data: this.formData
              }).then(response => {
                if (response.data.result === 200) {
                  this.$alertSuccess('操作成功');
                  this.partlyReload();
                } else {
                  this.$alertWarning(response.data.data)
                }
              }).catch(() => {
                this.$alertDanger('未知错误');
              }).finally(() => {
                this.isPending = false;
                this.$closeLoading();
              })
            } else {
              this.$alertInfo('请检查输入内容')
            }
          })
        }
      },

      dispatchNotice(row) {
        if (new Date().getTime() - new Date(row.endTime) > 0) {
          this.$alertInfo('选择的通知播放时间已过期');
          return;
        }
        this.isPending = true;
        this.$openLoading();
        this.getDataList(eSopFactorySelectUrl, '厂区').then(data => {
          this.factoryList = data.map(item => {
            return {
              key: item.id,
              label: item.abbreviation,
            }
          })
        });
        this.dispatchingNoticeId = row.id;
        this.isDispatching = true;
        this.isPending = false;
        this.$closeLoading();
      },

      resetDispatching() {
        this.dispatchingNoticeId = null;
        this.lineQueryData = {
          factoryId: null,
          workshopId: null,
          lineId: null
        };
        this.factoryList = [];
        this.workshopList = [];
        this.lineList = [];
        this.siteList = [];
      },

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
          this.isPending = false;
          this.$closeLoading();
        })
      },
      saveDispatching() {
        this.isPending = true;
        this.$openLoading();
        let options = {
          url: eSopFileDispatchUrl,
          data: {},
        };
        options.data.list = JSON.stringify(this.siteList.map(siteItem => {
          return {
            id: siteItem.id,
            pictureList: [],
            noticeList: [this.dispatchingNoticeId],
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

    }
  }
</script>
<style scoped>
  @import "styles/common.css";

  #notice-manager {
    position: relative;
    width: 100%;
    min-height: 600px;
    height: 100%;
  }
  #notice-manager /deep/ .el-button i{
    font-size: 14px;
    font-weight: bold;
  }

  .notice-edit-form /deep/ .el-textarea {
    width: 450px;
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

</style>
