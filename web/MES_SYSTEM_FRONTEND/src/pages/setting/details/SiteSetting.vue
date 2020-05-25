<template>

  <div id="site-setting">
    <div class="site-setting-main">
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
          width="80"
          fixed="right"
        >
          <template slot-scope="scope">
            <el-tooltip content="编辑" placement="top">
              <el-button type="text" @click="_editData(scope.row)" icon="el-icon-edit-outline"></el-button>
            </el-tooltip>
            <el-tooltip content="删除" placement="top">
              <el-button type="text" @click="_deleteData(scope.row)" icon="el-icon-delete"></el-button>
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
      @close="_resetEditForm"
      width="500px">
      <rz-form-item
        :ref="formConfig.refName"
        v-if="isEditing"
        :config="formConfig"
        :list="formItemList"
        :rules="formRules"
        :data.sync="formData"/>
      <span slot="footer" class="dialog-footer">
         <el-button size="small" @click="_closeEditPanel" type="info">取消</el-button>
         <el-button size="small" @click="_submitFormEdit" type="primary">保存</el-button>
       </span>
    </el-dialog>
  </div>
</template>

<script>
  import {SiteQueryConfig, SiteTableColumns, SiteFormItems, SiteFormRules} from "../../../config/settingConfig";
  import {eSopSiteSelectUrl, eSopSiteAddUrl, eSopSiteDeleteUrl, eSopSiteEditUrl} from "../../../config/globalUrl";
  import common from "./mixins/common";

  export default {
    name: "SiteSetting",
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



        editPanelTitle: '',
        isEditing: false,
        editType: 0, //0: add, 1: edit

        formConfig: {
          refName: 'siteFormComp',
          size: 'small',
          identity: 'site-item-'
        },
        formItemList: SiteFormItems,
        formRules: SiteFormRules,
        formData: {},

        /*链接重定义*/
        formAddUrl: eSopSiteAddUrl,
        formDeleteUrl: eSopSiteDeleteUrl,
        formUpdateUrl: eSopSiteEditUrl,

      }
    },
    mounted() {
      /*注册按键*/
      this.buttonGroup[0].callback = this._initQueryOptions;
      this.buttonGroup[1].callback = this._queryData;
      if (this._getFunctionPermission(1)) {
        this.buttonGroup.push(
          {
            label: '新增',
            size: 'small',
            type: 'primary',
            callback: this._addData
          })
      }

      this._queryData();
    },
    created() {
      this.$openLoading();
      this._getLineList().then(data => {
        this.queryConfig[2].selectList = data.map(item => {
          return {
            key: item.id,
            label: item.lineName
          }
        });
        this.formItemList[3].selectList = data.map(item => {
          return {
            key: item.id,
            label: item.lineName
          }
        });
        this.$closeLoading();
      })
    },
    methods: {
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
      }
    }
  }
</script>

<style scoped>
  @import "styles/common.css";

  #site-setting {
    position: relative;
    width: 100%;
    min-height: 600px;
    height: 100%;
  }
  #site-setting /deep/ .el-button i{
    font-size: 14px;
    font-weight: bold;
  }
</style>
