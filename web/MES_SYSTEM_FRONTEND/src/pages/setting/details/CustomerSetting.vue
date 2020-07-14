<template>

  <div id="customer-setting">
    <div class="customer-setting-main">
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
  import {
    CustomerQueryConfig,
    CustomerTableColumns,
    CustomerFormItems,
    CustomerFormRules
  } from "../../../config/settingConfig";
  import {
    eSopCustomerSelectUrl,
    eSopCustomerAddUrl,
    eSopCustomerDeleteUrl,
    eSopCustomerEditUrl
  } from "../../../config/globalUrl";
  import common from "./mixins/common";

  export default {
    name: "CustomerSetting",
    mixins: [common],
    data() {
      return {
        /*搜索框*/
        queryConfig: CustomerQueryConfig,
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
        tableColumns: CustomerTableColumns,
        tableData: [],


        editPanelTitle: '',
        isEditing: false,
        editType: 0, //0: add, 1: edit

        formConfig: {
          refName: 'customerFormComp',
          size: 'small',
          identity: 'customer-item-'
        },
        formItemList: CustomerFormItems,
        formRules: CustomerFormRules,
        formData: {},

        /*链接重定义*/
        formAddUrl: eSopCustomerAddUrl,
        formDeleteUrl: eSopCustomerDeleteUrl,
        formUpdateUrl: eSopCustomerEditUrl,
      }
    },
    created() {
      this.$openLoading();
      this._getFactoryList().then(data => {
        this.queryConfig[2].selectList = data.map(item => {
          return {
            key: item.id,
            label: item.abbreviation
          }
        });
        this.formItemList[2].selectList = data.map(item => {
          return {
            key: item.id,
            label: item.abbreviation
          }
        });
        this.$closeLoading();
      })
    },
    mounted() {
      //传入当前是哪个页面，this.$store.state.limits 就会有相应页面的权限配置情况
      this.$store.commit('pageActionLimits',this.$store.state.charactersFuncMap.map.basic.basic.customer)
      /*注册按键*/
      this.buttonGroup[0].callback = this._initQueryOptions;
      this.buttonGroup[1].callback = this._queryData;
      this.buttonGroup.push(
        {
          label: '新增',
          size: 'small',
          type: 'primary',
          callback: this._addData
        });

      this._queryData();
    },
    methods: {
      fetchData() {
        this.$openLoading();
        let options = {
          url: eSopCustomerSelectUrl,
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

  #customer-setting {
    position: relative;
    width: 100%;
    min-height: 600px;
    height: 100%;
  }
  #customer-setting /deep/ .el-button i{
    font-size: 14px;
    font-weight: bold;
  }
</style>
