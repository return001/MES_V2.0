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
<!--
        <el-table-column
          label="操作"
          width="160"
          fixed="right"
          v-if="permissionControl(['engineer'])"
        >
          <template slot-scope="scope">
            <el-tooltip content="编辑" placement="top">
              <el-button type="text" @click="editData('edit', scope.row)" icon="el-icon-edit-outline"></el-button>
            </el-tooltip>
            <el-tooltip content="删除" placement="top">
              <el-button type="text" @click="deleteData(scope.row)" icon="el-icon-delete"></el-button>
            </el-tooltip>
          </template>
        </el-table-column>-->
      </el-table>
    </div>
    <!--dialog component-->
   <!-- <el-dialog
      :title="editPanelTitle"
      :visible.sync="isProcessEditing"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @close="resetEditProcessForm"
      width="730px">
      <el-form
        ref="processEditForm"
        :model="processEditOptionsData"
        class="process-edit-form"
        label-position="top"
        @submit.native.prevent
        :rules="processEditOptionsRules">
        <el-form-item class="process-edit-form-comp" label="工序组" prop="processGroup">
          <el-select v-model="processEditOptionsData.processGroup" class="process-edit-form-comp-text">
            <el-option value="" label="未选择" placeholder="请选择工序组" size="small"></el-option>
            <el-option v-for="listItem in processGroupSelectGroup"
                       :key="listItem.id"
                       :value="listItem.id"
                       :label="listItem.groupName"></el-option>
          </el-select>
        </el-form-item>
        <el-form-item
          class="process-edit-form-comp"
          v-for="(item, index) in processEditOptions"
          :key="index"
          :label="item.label + '：'"
          :prop="item.key">
          <div class="process-edit-form-comp-text">
            <el-input
              type="text"
              :id="'edit' + item.key + index" :placeholder="'请填写' + item.label"
              clearable
              autocomplete="off"
              v-model="processEditOptionsData[item.key]"></el-input>
          </div>
        </el-form-item>
      </el-form>
      <span slot="footer" class="dialog-footer">
        <el-button @click="closeEditProcessPanel" type="info">取消</el-button>
        <el-button @click="submitEditProcess" type="primary">保存</el-button>
      </span>
    </el-dialog>-->
  </div>
</template>

<script>
  import {SiteQueryConfig, SiteTableColumns} from "../../../config/esopConfig";
  import {eSopSiteSelectUrl} from "../../../config/globalUrl";
  import common from "./mixins/common";
export default {
    name: "SiteSetting",
    mixins: [common],
    data () {
      return {
        /*搜索框*/
        queryConfig: SiteQueryConfig,
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
        tableColumns: SiteTableColumns,
        tableData: [],

        paginationOptions: {
          currentPage: 1,
          pageSize: 65535,
          total: 0
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
      fetchData() {
        let options = {
          url: eSopSiteSelectUrl,
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
      }
    }
  }
</script>

<style scoped>
  #site-setting {
    position: relative;
    width: 100%;
    min-height: 600px;
    height: 100%;
  }
</style>
