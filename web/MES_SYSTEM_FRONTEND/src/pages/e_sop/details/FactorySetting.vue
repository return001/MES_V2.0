<template>

  <div id="factory-setting">
    <div class="factory-setting-main">
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
              <el-button type="text" @click="editData('edit', scope.row)" icon="el-icon-edit-outline"></el-button>
            </el-tooltip>
            <el-tooltip content="删除" placement="top">
              <el-button type="text" @click="deleteData(scope.row)" icon="el-icon-delete"></el-button>
            </el-tooltip>
          </template>
        </el-table-column>
      </el-table>
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
        :config="formConfig"
        :list="formItemList"
        :rules="formRules"
        :data.sync="formData" />
       <span slot="footer" class="dialog-footer">
         <el-button size="small" @click="closeEditPanel" type="info">取消</el-button>
         <el-button size="small" @click="submitEdit" type="primary">保存</el-button>
       </span>
    </el-dialog>
  </div>
</template>

<script>
  import {FactoryQueryConfig, FactoryTableColumns, FactoryFormItems, FactoryFormRules} from "../../../config/esopConfig";
  import {
    eSopFactorySelectUrl,
    eSopFactoryDeleteUrl,
    eSopFactoryAddUrl,
    eSopFactoryEditUrl
  } from "../../../config/globalUrl";
  import common from "./mixins/common";

  export default {
    name: "FactorySetting",
    inject: ['reload'],
    mixins: [common],
    data() {
      return {
        /*搜索框*/
        queryConfig: FactoryQueryConfig,
        buttonGroup: [
          {
            label: '查询',
            size: 'small',
            type: 'primary',
            callback() {
            }
          },
          {
            label: '删除',
            size: 'small',
            callback() {
            }
          }
        ],
        queryCompData: {},
        tableColumns: FactoryTableColumns,
        tableData: [],

        paginationOptions: {
          currentPage: 1,
          pageSize: 65535,
          total: 0
        },

        editPanelTitle: '',
        isEditing: true,

        formConfig: {
          refName: 'factoryFormComp',
          size: 'small',
          identity: 'factory-item-'
        },
        formItemList: FactoryFormItems,
        formRules: FactoryFormRules,
        formData: {
          factoryAlias: 'sss'
        },
      }
    },
    mounted() {
      /*注册按键*/
      this.buttonGroup[0].callback = this.fetchData;

      this.$openLoading();
      this.fetchData();
    },
    methods: {
      /*查询、获取表格内容*/
      fetchData() {
        let options = {
          url: eSopFactorySelectUrl,
          data: {
            pageNo: 1,
            pageSize: 65535
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

      /*编辑指定项目*/
      editData() {

      },

      resetEditForm() {

      },

      closeEditPanel() {

      },

      submitEdit() {

      },

      /*删除指定项目*/
      deleteData(val) {
        this._deleteData(eSopFactoryDeleteUrl, val.id).then(() => {
          this.reload();
        })
      }
    }
  }
</script>
<style scoped>
  @import "styles/common.css";

  #factory-setting {
    position: relative;
    width: 100%;
    min-height: 600px;
    height: 100%;
  }


</style>
