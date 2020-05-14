<template>
  <div id="order-setting">
    <div class="order-setting-main">
      <div class="query-comp">
        <div class="query-comp-container" v-for="(item, index) in queryOptions" :key="index">
          <!--纯文本框-->
          <div class="query-comp-text" v-if="item.type === 'text'">
            <label :for="item.key + index">{{item.label}}:</label>
            <el-input v-model="thisQueryOptions[item.key].value" :id="item.key + index"
                      :placeholder="'请填写' + item.label" size="small" clearable
                      autocomplete="off"></el-input>
          </div>
          <!--选择器-->
          <div class="query-comp-select" v-else-if="item.key === 'factoryId'">
            <label :for="item.key + index">{{item.label}}:</label>
            <el-select v-model="thisQueryOptions[item.key].value" :id="item.key + index"
                       autocomplete="off"
                       :placeholder="'请选择' + item.label" size="small">
              <el-option v-for="listItem in factoryList"
                         :key="listItem.id"
                         :value="listItem.id"
                         :label="listItem.abbreviation"></el-option>
            </el-select>
          </div>
          <div class="query-comp-select" v-else-if="item.type === 'select'">
            <label :for="item.key + index">{{item.label}}:</label>
            <el-select v-model="thisQueryOptions[item.key].value" :id="item.key + index"
                       autocomplete="off"
                       :placeholder="'请选择' + item.label" size="small">
              <el-option v-for="listItem in item.list"
                         :key="listItem.value"
                         :value="listItem.value"
                         :label="listItem.label"></el-option>
            </el-select>
          </div>
          <!--时间范围-->
          <div class="query-comp-timerange" v-else-if="item.type === 'timeRange'">
            <label :for="item.key + index">{{item.label}}:</label>
            <el-date-picker
              :id="item.key + index"
              v-model="thisQueryOptions[item.key].value"
              type="daterange"
              :picker-options="timePickerOptions"
              prefix-icon="el-icon-date"
              range-separator="至"
              start-placeholder="开始日期"
              end-placeholder="结束日期"
              value-format="yyyy-MM-dd"
              autocomplete="off"
              size="small">
            </el-date-picker>
          </div>
        </div>
        <div class="query-comp-container" style="padding-top: 36px;">
          <el-checkbox v-model="isRework">返工订单</el-checkbox>
        </div>
        <div class="query-comp-container">
          <el-button type="info" @click="initQueryOptions" size="small">重置条件</el-button>
        </div>
        <div class="query-comp-container">
          <el-button type="primary" size="small" @click="queryData">查询</el-button>
        </div>
        <div class="query-comp-container" v-if="permissionControl(['schedulingSZPC'])">
          <el-button type="primary" size="small" @click="isOrderUploading = true">导入订单</el-button>
        </div>
        <div class="query-comp-container" v-if="permissionControl(['schedulingSZPC'])">
          <el-button type="primary" size="small" @click="editData('add')">新增订单</el-button>
        </div>
        <div class="query-comp-container" v-if="permissionControl(['schedulingSZPC'])">
          <el-button type="primary" size="small" @click="editData('rework',reworkOrder)" v-if="reworkOrder !== ''">返工</el-button>
          <el-button type="info" @click="unselected" size="small" v-else>返工</el-button>
        </div>
      </div>
      <div class="content-comp">
        <el-table
          @row-click="choiceReworkRow"
          :data="tableData"
          max-height="560"
          ref="tablecomponent"
          highlight-current-row
          stripe>
          <el-table-column v-for="(item, index) in tableColumns"
                           :key="index"
                           :prop="item.key"
                           :label="item.label"
                           v-if="$store.state.userType !== item.notShowTo"
                           :min-width="item['min-width']"
                           :formatter="item.formatter">
          </el-table-column>

          <el-table-column
            type="index"
            :index="indexMethod"
            fixed="left"
            width="60">
          </el-table-column>

          <el-table-column
            label="操作"
            width="155"
            fixed="right"
            v-if="$store.state.userType !== 'operator'"
          >
            <template slot-scope="scope">
              <el-tooltip content="订单详情" placement="top"
                          v-if="permissionControl(['schedulingSZPC', 'engineer','schedulingJMPMC'])">
                <el-button type="text" icon="el-icon-t-table" @click="showDetails(scope.row)"></el-button>
              </el-tooltip>
              <el-tooltip content="编辑订单" placement="top"
                          v-if="permissionControl(['schedulingSZPC', 'schedulingJMPMC'])">
                <el-button type="text" @click="editData('edit', scope.row)" icon="el-icon-t-edit" :disabled="scope.row.editable === false"></el-button>
              </el-tooltip>
              <el-tooltip content="复制订单" placement="top"
                          v-if="permissionControl(['schedulingSZPC', 'schedulingJMPMC'])">
                <el-button type="text" @click="editData('copy', scope.row)" icon="el-icon-t-copy"></el-button>
              </el-tooltip>
              <el-tooltip content="异常结单" placement="top"
                          v-if="permissionControl(['schedulingSZPC', 'schedulingJMPMC'])">
                <el-button type="text" @click="abnormalEndData(scope.row)" icon="el-icon-t-delete" :disabled="scope.row.abnoable === false"></el-button>
              </el-tooltip>
              <el-tooltip content="确认订单" placement="top"
                          v-if="permissionControl(['schedulingSZPC', 'schedulingJMPMC'])">
                <el-button type="text" @click="confirmOrder(scope.row)" icon="el-icon-check" :disabled="scope.row.orderConfirm === false"></el-button>
              </el-tooltip>

<!--              <el-tooltip content="删除订单" placement="top"-->
<!--                          v-if="permissionControl(['schedulingSZPC', 'schedulingJMPMC'])">-->
<!--                <el-button type="text" @click="deleteData(scope.row)" icon="el-icon-t-delete" :disabled="scope.row.endable === false"></el-button>-->
<!--              </el-tooltip>-->
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
          @current-change="pageChange"
          @size-change="queryData"
          class="page-pagination">
        </el-pagination>
      </div>
    </div>

    <!--dialog component-->
    <el-dialog
      :title="editPanelTitle"
      :visible.sync="isOrderEditing"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @closed="resetEditOrderForm"
      width="730px">

      <el-form
        ref="orderEditForm"
        :model="orderEditOptionsData"
        class="order-edit-form"
        label-position="top"
        @submit.native.prevent
        :rules="orderEditOptionsRules">


        <el-form-item
          size="small"
          class="order-edit-form-comp-select"
          label="所属工厂"
          prop="factory"
        >
          <el-select v-model="orderEditOptionsData['factory']" class="line-edit-form-comp-text"
                      placeholder="请选择工厂"
                      size="small"
                     :disabled="isDisabled === false"
                      clearable>
            <el-option v-for="listItem in factoryList"
                       :key="listItem.id"
                       :value="listItem.id"
                       :label="listItem.abbreviation"></el-option>
          </el-select>
        </el-form-item>


        <el-form-item
          size="small"
          class="order-edit-form-comp"
          v-for="(item, index) in orderEditOptions"
          :key="index"
          :label="item.label + '：'"
          :prop="item.key">
          <div class="order-edit-form-comp-text" v-if="item.type === 'text'">
            <el-input
              type="text"
              :disabled="isDisabled === false"
              :id="'edit' + item.key + index"
              :placeholder="'请填写' + item.label"
              clearable
              autocomplete="off"
              v-model="orderEditOptionsData[item.key]"></el-input>
          </div>
          <div class="order-edit-form-comp-text" v-if="item.type === 'date'">
            <el-date-picker
              :id="'edit' + item.key + index"
              :disabled="isDisabled === false"
              v-model="orderEditOptionsData[item.key]"
              type="date"
              placeholder="选择日期"
              value-format="yyyy-MM-dd"></el-date-picker>
          </div>
          <div class="order-edit-form-comp-textarea" v-if="item.type === 'textArea'">
            <el-input type="textarea"
                      width="210px"
                      :rows="3"
                      :id="'edit' + item.key + index" :placeholder="'请填写' + item.label"
                      clearable
                      autocomplete="off"
                      v-model="orderEditOptionsData[item.key]"></el-input>
          </div>
        </el-form-item>

        <!-- 返工数量和返工单号 -->
        <el-form-item
          v-if="isReworkEdit"
          size="small"
          type="text"
          class="order-edit-form-comp"
          v-for="(item, index) in orderReworkOptions"
          :key='"rework-"+index'
          :label="item.label + '：'"
          :prop="item.key">
          <div class="order-edit-form-comp-text-rework" v-if="item.type === 'text'" >
            <el-input
              type="text"
              :id="'edit' + item.key + index"
              :placeholder="'请填写' + item.label"
              clearable
              autocomplete="off"
              v-model="orderEditOptionsData[item.key]"></el-input>
          </div>
        </el-form-item>



        <el-form-item
          size="small"
          class="order-edit-form-comp"
          v-if="orderEditType === 'copy'"
          style="margin-left: auto">
          <el-checkbox v-model="isReworkEdit" >返工订单</el-checkbox>
        </el-form-item>

      </el-form>
      <span slot="footer" class="dialog-footer">
        <el-button size="small" @click="closeEditOrderPanel" type="info">取消</el-button>
        <el-button size="small" @click="submitEditOrder" type="primary">保存</el-button>
      </span>
    </el-dialog>

    <!--delete dialog-->
    <el-dialog
      title="提示"
      :visible.sync="isOrderDeleting"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @closed="closeDeleteOrderPanel"
      width="400px">
      <div class="order-delete-msg-title">正在作废订单<span>{{deletingItem.zhidan}}</span></div>
      <div class="order-delete-msg-content">
        <el-input type="textarea"
                  :rows="3"
                  autocomplete="off"
                  placeholder="请输入作废理由"
                  v-model="deleteReason">

        </el-input>
      </div>
      <span slot="footer" class="dialog-footer">
        <el-button size="small" @click="closeDeleteOrderPanel" type="info">取消</el-button>
        <el-button size="small" @click="submitDeleteOrder" type="warning">保存</el-button>
      </span>
    </el-dialog>

    <!-- 异常结单   -->
    <el-dialog
      title="提示"
      :visible.sync="isAbnormalEnd"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @closed="closeAbnormalEndPanel"
      width="400px">
      <div class="order-delete-msg-title">正在异常结单<span>{{abnormalEndItem.zhidan}}</span></div>
      <div class="order-delete-msg-content">
        <el-input type="textarea"
                  :rows="3"
                  autocomplete="off"
                  placeholder="请输入异常结单理由"
                  v-model="abnormanEndRemarks">

        </el-input>
      </div>
      <span slot="footer" class="dialog-footer">
        <el-button size="small" @click="closeAbnormalEndPanel" type="info">取消</el-button>
        <el-button size="small" @click="submitAbnormalEnd" type="warning">保存</el-button>
      </span>
    </el-dialog>

    <!-- 删除详情中订单   -->
    <el-dialog
      title="提示"
      :visible.sync="isDeleteFile"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @closed="closeDeleteFilePanel"
      width="400px">
      <div class="order-delete-msg-title">正在作废文件：<span>{{deleteFileItem.fileName}}</span></div>
      <div class="order-delete-msg-content">
        <el-input type="textarea"
                  :rows="3"
                  autocomplete="off"
                  placeholder="请输入作废理由"
                  v-model="deleteFileRemarks">

        </el-input>
      </div>
      <span slot="footer" class="dialog-footer">
        <el-button size="small" @click="closeDeleteFilePanel" type="info">取消</el-button>
        <el-button size="small" @click="submitDeleteFile" type="warning">保存</el-button>
      </span>
    </el-dialog>

    <!--导入订单 上传框-->
    <el-dialog
      title="上传订单"
      :visible.sync="isOrderUploading"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @closed="clearOrderUploadFile"
      width="400px">
<!--      <div style="width: 100%; margin-bottom: 20px">-->
<!--        <el-checkbox v-model="isReworkUpload">是否返工订单</el-checkbox>-->
<!--      </div>-->
      <!--<el-upload
        ref="orderUpload"
        :action="planOrderImportUrl"
        :auto-upload="false"
        :before-upload="beforeOrderUpload"
        :on-success="onOrderUploadSuccess"
        :on-error="onOrderUploadError"-->
      <el-upload
        ref="orderUpload"
        :action="planOrderImportUrl"
        :auto-upload="false"
        :http-request="uploadFile"
        :before-upload="beforeOrderUpload"
        accept="application/vnd.ms-excel,application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"
      >
        <el-button slot="trigger" size="small" type="info">选取文件</el-button>
        <el-button style="margin-left: 10px;" size="small" type="primary" @click="submitOrderUpload">上传</el-button>
        <div slot="tip" class="upload-tip">请选择xls、xlsx文件</div>
      </el-upload>
    </el-dialog>

    <!--订单详情-->
    <!--查看订单详情-->
    <el-dialog
      :title="detailsTitle"
      :visible.sync="isDetailsShowing"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @closed="closeDeleteOrderPanel"
      width="850px">
      <div class="details-header">
        <div class="details-header-container-mes" >
          <div class="details-header-item" v-for="item in showingItemInfoList">
            <label class="details-header-item-label">
              {{item.label}}:
            </label>
            <span class="details-header-item-content">
              {{!!showingItemInfo[item.key] ? showingItemInfo[item.key] : '无'}}
            </span>
          </div>
        </div>


        <div class="details-header-container-remark">
          <div class="details-header-item" v-for="item in showingItemRemarkList">
            <div class="details-header-item-label">
              {{item.label}}:
            </div>
            <div class="details-header-item-content">
              {{!!showingItemInfo[item.key] ? showingItemInfo[item.key] : '无'}}
            </div>
          </div>
        </div>
      </div>
      <div class="details-content">
        <el-table

          :data="detailsTableData"
          max-height="500"
          border
          size="small"
          ref="detailstablecomponent"
          :span-method="detailsTableSpanMethod"
          stripe>
          <el-table-column v-for="(item, index) in detailsTableColumns"
                           :key="index"
                           :prop="item.key"
                           :label="item.label"
                           :min-width="item['min-width']"
                           :formatter="item.formatter">
          </el-table-column>
          <el-table-column
            label="操作"
            width="110"
            fixed="right"
          >
            <template slot-scope="scope">
              <el-tooltip content="下载" placement="top">
                <el-button type="text" icon="el-icon-download" @click="downloadDetailsTable(scope.row)"></el-button>
              </el-tooltip>
              <el-tooltip content="删除" placement="top">
                <el-button type="text" icon="el-icon-delete" @click="deleteDetailsTable(scope.row)" :disabled="scope.row.fileEditable === false"></el-button>
              </el-tooltip>
              <span style="display:inline-block;width: 10px;"></span>
              <el-tooltip content="文件状态" placement="top">
                <el-button type="text" icon="el-icon-edit" @click="changeFileStatus(scope.row)" :disabled="scope.row.fileEditable === false"></el-button>
              </el-tooltip>
<!--              <el-tooltip :content="'文件状态: ' + fileStatus" placement="top">-->
<!--                <el-switch-->
<!--                  :width='30'-->
<!--                  v-model="fileStatus"-->
<!--                  active-color="#ff4949"-->
<!--                  inactive-color="#13ce66"-->
<!--                  active-value="异常"-->
<!--                  inactive-value="正常"-->
<!--                  @change="editDetailsTable(scope.row)"-->
<!--                  >-->
<!--                </el-switch>-->
<!--              </el-tooltip>-->
            </template>
          </el-table-column>
        </el-table>
      </div>
      <!--文件状态-->
      <el-dialog
        title="文件状态"
        :visible.sync="orderFileStatus"
        :close-on-click-modal="false"
        :close-on-press-escape="false"
        @closed="closeOrderStatusPanel"
        append-to-body
        width="400px">
        <template>
          <el-radio v-model="fileStatus" label="true">正常</el-radio>
          <el-radio v-model="fileStatus" label="false">异常</el-radio>
        </template>
        <div style="display: inline-block;width: 40px;"></div>
        <el-button size="small" @click="submitFileStatus()" type="primary">提交</el-button>
      </el-dialog>
      <span slot="footer" class="dialog-footer"
            v-if="permissionControl(['schedulingSZPC', 'engineer'])">
        <el-button size="small" @click="isOrderDetailsUploading = true" type="primary" :disabled="uploadAble === false">上传</el-button>
      </span>


      <el-dialog
        title="上传"
        :visible.sync="isOrderDetailsUploading"
        :close-on-click-modal="false"
        :close-on-press-escape="false"
        append-to-body
        @closed="clearOrderDetailsUploadFile"
        width="400px">
        <div style="width: 100%; margin-bottom: 20px">

            <el-select v-model="orderDetailsType"
                       autocomplete="off"
                       placeholder="请选择类型"
                       size="small"
                       clearable>
              <el-option v-for="listItem in fileTypeList"
                         :key="listItem.id"
                         :value="listItem.id"
                         :label="listItem.typeName"></el-option>
            </el-select>


<!--          <el-select v-model="orderDetailsType" placeholder="请选择类型" autocomplete="off" size="small">-->
<!--            <el-option :value="0" label="信息表"-->
<!--                       v-if="permissionControl(['schedulingSZPC'])"></el-option>-->
<!--            <el-option :value="1" label="BOM表"-->
<!--                       v-if="permissionControl(['schedulingSZPC'])"></el-option>-->
<!--            <el-option :value="2" label="SOP表"-->
<!--                       v-if="permissionControl(['engineer','schedulingSZPC'])"></el-option>-->
<!--          </el-select>-->
        </div>
        <el-upload
          :disabled="orderDetailsType === ''"
          ref="orderDetailsUpload"
          action="#"
          :auto-upload="false"
          :http-request="uploadFile"
          multiple
          :before-upload="beforeOrderUpload"
          accept="application/vnd.ms-excel,application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"
        >
          <el-button :disabled="orderDetailsType === ''" slot="trigger" size="small" type="info">选取文件</el-button>
          <el-button :disabled="orderDetailsType === ''" style="margin-left: 10px;" size="small" type="primary"
                     @click="submitDetailsOrderUpload">上传
          </el-button>
          <div slot="tip" class="upload-tip">请选择xls、xlsx文件</div>
        </el-upload>
      </el-dialog>
    </el-dialog>
  </div>
</template>

<script>
  import {
    orderQueryOptions,
    orderTableColumns,
    orderEditOptions,
    orderEditOptionsRules,
    orderReworkOptions,
    orderDetailsTableColumns
  } from "../../../config/planConfig";
  import {FactoryTableColumns} from "../../../config/settingConfig";
  import {
    planOrderAddUrl,
    planOrderDeleteUrl,
    planOrderEditUrl,
    planOrderImportUrl,
    planOrderSelectUrl,
    planOrderTableDownloadUrl,
    planOrderTableImportUrl,
    planOrderDetailsSelectUrl,
    planOrderTableDeleteUrl,
    planDetailsExportUrl,
    planOrderTableEditUrl,
    planLineGetUrl,
    eSopFactorySelectUrl,
    planFileTypeSelectUrl,
  } from "../../../config/globalUrl";
  import {axiosFetch, axiosDownload} from "../../../utils/fetchData";
  import {MessageBox} from 'element-ui';
  import {saveAs} from 'file-saver';

  export default {
    name: "OrderSetting",
    inject: ['reload'],
    data() {
      return {
        queryOptions: orderQueryOptions,
        thisQueryOptions: {}, //将queryOptions中的键值对提取作为该对象元素
        tableData: [],
        tableColumns: orderTableColumns,
        paginationOptions: {
          currentPage: 1,
          pageSize: 20,
          total: 0
        },
        timePickerOptions: {
          shortcuts: [{
            text: '最近一周',
            onClick(picker) {
              const end = new Date();
              const start = new Date();
              start.setTime(start.getTime() - 3600 * 1000 * 24 * 7);
              picker.$emit('pick', [start, end]);
            }
          }, {
            text: '最近一个月',
            onClick(picker) {
              const end = new Date();
              const start = new Date();
              start.setTime(start.getTime() - 3600 * 1000 * 24 * 30);
              picker.$emit('pick', [start, end]);
            }
          }, {
            text: '最近三个月',
            onClick(picker) {
              const end = new Date();
              const start = new Date();
              start.setTime(start.getTime() - 3600 * 1000 * 24 * 90);
              picker.$emit('pick', [start, end]);
            }
          }]
        },
        isPending: false,
        queryString: '',

        isRework: false, //是否返工单
        /*返工数量和返工单号*/
        orderReworkOptions:orderReworkOptions,
        isDisabled:true,
        /*新增 编辑 复制订单*/
        orderEditOptions: orderEditOptions,
        orderEditOptionsRules: orderEditOptionsRules,
        isReworkEdit: false,
        isOrderEditing: false,
        orderEditType: '',
        orderEditOptionsData: {
          factory: undefined
        },
        uploadAble:true,
        reworkOrder:"",
        factoryList:[],//工厂列表
        /*作废订单*/
        isOrderDeleting: false,
        deletingItem: {},
        deleteReason: '',
        /*异常结单*/
        isAbnormalEnd:false,
        abnormalEndItem: {},
        abnormanEndRemarks:'',

        /*删除详情中订单*/
        isDeleteFile:false,
        deleteFileItem: {},
        deleteFileRemarks:'',
        /*上传订单*/
        isOrderUploading: false,
        uploadFileData: null,
        isReworkUpload: false,
        /*订单详情上传*/
        isOrderDetailsUploading: false,
        orderDetailsType: '',
        fileTypeList:"", //文件类型列表

        /*订单详情*/
        showingItem: {},
        isDetailsShowing: false,
        showingItemInfo: {},
        showingItemFileInfo: {},
        showingItemInfoList: [
          {
            label: '创建人',
            key: 'creatorName'
          },
          {
            label: '创建时间',
            key: 'orderCreateTime'
          },
          {
            label: '修改人',
            key: 'modifierName'
          },
          {
            label: '修改时间',
            key: 'orderModifyTime'
          }],
          showingItemRemarkList:[
          {
            label: '作废原因',
            key: 'abnormalEndReason'
          },
          {
            label: '作废人',
            key: 'abnormalEndPersonName'
          },
          {
            label: '作废时间',
            key: 'abnormalEndTime'
          },

          {
            label: 'PC业务备注',
            key: 'pcRemarks'
          },
          {
            label: '备注人',
            key: 'pc'
          },
          {
            label: '备注时间',
            key: 'pcRemarksTime'
          },

          {
            label: 'PMC备注',
            key: 'pmcRemarks'
          },
          {
            label: '备注人',
            key: 'pmc'
          },
          {
            label: '备注时间',
            key: 'pmcRemarksTime'
          },

          {
            label: '工程备注',
            key: 'engineerRemarks'
          },
          {
            label: '备注人',
            key: 'engineer'
          },
          {
            label: '备注时间',
            key: 'engineerRemarksTime'
          },

          {
            label: '生产备注',
            key: 'producerRemarks'
          },
          {
            label: '备注人',
            key: 'producer'
          },
          {
            label: '备注时间',
            key: 'producerRemarksTime'
          },
          {
            label: '品质备注',
            key: 'qcRemarks'
          },
          {
            label: '备注人',
            key: 'qc'
          },
          {
            label: '备注时间',
            key: 'qcRemarksTime'
          },
        ],
        detailsTableColumns: orderDetailsTableColumns,
        detailsTableData: [],
        fileStatus:"",  //单选按钮状态
        orderFileStatus:false,  //改变文件状态的对话框的显隐
        orderInfo:"", //选中订单
        mergeData: {},//合并行的记录
        mergePos: {},//mergeData中每项的索引
        isDeleted:false,  //删除的文件不能再删除
        mergeProp: ['typeName'],
        mergeKeys: ['typeName'],


      }
    },
    computed: {
      editPanelTitle: function () {
        if (this.orderEditType === 'edit') {
          return '编辑订单'
        } else if (this.orderEditType === 'add') {
          return '新增订单'
        } else if (this.orderEditType === 'rework') {
          return '返工订单'
        }else if (this.orderEditType === 'copy') {
          return '复制订单'
        }
      },
      planOrderImportUrl: function () {
        return planOrderImportUrl
      },
      detailsTitle: function () {
        return '详情 (订单：' + this.showingItem.zhidan + ')'
      },
    },
    created() {
      this.initQueryOptions();
      this.fetchFactory();
    },
    mounted() {
      this.fetchData()
    },
    methods: {
      /*局部刷新*/
      partlyReload() {
        this.isPending = false;
        let _partlyReload = stashData => {
          let obj = {};
          stashData.forEach(item => {
            obj[item] = this.$data[item]
          });
          this.$store.commit('setStashData', obj);
          Object.assign(this.$data, this.$options.data());
          Object.assign(this.$data, this.$store.state.stashData);
          this.queryData();
          this.$store.commit('setStashData', {});
        };
        _partlyReload(['thisQueryOptions'])
      },
      /**
       **@description: 权限控制-显示隐藏
       **@date: 2019/8/13 11:39
       **@author: DarkNin
       **@method: permissionControl
       **@params: Array[] 可显示的用户
       */
      permissionControl: function (userArray) {
        let thisUser = this.$store.state.userType;
        if (userArray.indexOf(thisUser) !== -1 || thisUser === 'SuperAdmin') {
          return true
        }
      },

      initQueryOptions: function () {
        this.queryOptions.forEach(item => {
          this.$set(this.thisQueryOptions, item.key, {
            type: item.type,
            value: ''
          })
        })
      },

      indexMethod: function (index) {
        return index + (this.paginationOptions.currentPage - 1) * this.paginationOptions.pageSize + 1
      },

      createQueryString: function () {
        let queryString = "";
        Object.keys(this.thisQueryOptions).forEach(item => {
          let subObj = this.thisQueryOptions[item];
          let itemCopy = item.replace(/([A-Z])/g, "_$1").toLowerCase(); //驼峰转下划线
          switch (subObj.type) {
            case 'text':
              //possible value: null [empty string] [string]
              if (!!subObj.value) {
                queryString += queryString.length === 0 ? (itemCopy + '#like#' + subObj.value) : ('#&#' + itemCopy + '#like#' + subObj.value)
              }
              break;
            case 'select':
              //possible value: [empty string] [string]
              if (subObj.value !== '') {
                queryString += queryString.length === 0 ? (itemCopy + '#=#' + subObj.value) : ('#&#' + itemCopy + '#=#' + subObj.value)
              }
              break;
            case 'timeRange':
              //possible value: null [array] [empty string]
              if (!!subObj.value) {
                queryString += (queryString.length === 0 ? (itemCopy + '#>=#' + subObj.value[0]) : ('#&#' + itemCopy + '#>=#' + subObj.value[0])) + '#&#' + itemCopy + '#<=#' + subObj.value[1]
              }
              break;
          }
        });
        this.queryString = queryString;
      },

      pageChange: function () {
        this.createQueryString();
        this.fetchData();
      },

      queryData: function () {
        this.paginationOptions.currentPage = 1;
        this.paginationOptions.total = 0;
        this.createQueryString();
        this.fetchData();
      },


      //获取工厂
      fetchFactory: function () {
        return new Promise(resolve => {
          axiosFetch({
            url: eSopFactorySelectUrl,
            data: {
              pageNo: this.paginationOptions.currentPage,
              pageSize: this.paginationOptions.pageSize
            }
          }).then(response => {
            if (response.data.result === 200) {
              this.factoryList = response.data.data.list;
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            console.log(err)
            this.$alertDanger('获取工厂信息失败，请刷新重试');
          }).finally(() => {
            resolve();
          })
        });
      },

      fetchData: function () {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          let options = {
            url: planOrderSelectUrl,
            data: {
              pageNo: this.paginationOptions.currentPage,
              pageSize: this.paginationOptions.pageSize,
              // descBy: 'orderDate',
              isRework: this.isRework
            }
          };
          Object.keys(this.thisQueryOptions).forEach(item => {
            options.data[item] = JSON.parse(JSON.stringify(this.thisQueryOptions[item])).value
          });
          options.data['deliveryDateFrom'] = options.data['deliveryDate'][0]
          options.data['deliveryDateTo'] = options.data['deliveryDate'][1]
          options.data['orderDateFrom'] = options.data['orderDate'][0]
          options.data['orderDateTo'] = options.data['orderDate'][1]
          // if (this.queryString !== '') {
            // options.data.filter = this.queryString;
          // }
          axiosFetch(options).then(response => {
            if (response.data.result === 200) {
              this.tableData = response.data.data.list;
              this.tableData.forEach(item=>{
                if(item.orderStatus !== 1){
                  item.orderConfirm = false;    //只有待确认的状态可以 确认订单
                }
                if(item.orderStatus !== 1 && item.orderStatus !== 2){  //不可以编辑的状态
                  item.editable = false;
                }
                if(item.orderStatus === 6 || item.orderStatus === 7 || item.orderStatus === 8){   //不可以异常结单的状态
                  item.abnoable = false
                }
                if(item.orderStatus === 3 || item.orderStatus === 6 || item.orderStatus === 7 || item.orderStatus === 8){ //不可以作废的状态
                  item.endable = false
                }

              })
              this.paginationOptions.currentPage = response.data.data.pageNumber;
              this.paginationOptions.total = response.data.data.totalRow;
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.$alertDanger('未知错误')
          }).finally(() => {
            this.isPending = false;
            this.$closeLoading();
          })
        }
      },

      choiceReworkRow(row){
        this.reworkOrder = row;
      },


      /**
       **@description: 新增编辑复制订单
       **@date: 2019/7/23 8:52
       **@author: DarkNin
       **@method:
       **@params: mod
       */
      editData: function (type, val) {

        if(val){
          this.$set(this.orderEditOptionsData,'factory',val.factoryId)
        }
        this.fetchFactory()
        this.orderEditOptions.forEach(item => {
          this.$set(this.orderEditOptionsData, item.key, '')
        });
        if (type === 'edit') {
          //获取备注
          axiosFetch({
            url: planOrderDetailsSelectUrl,
            data: {
              id: val.id
            }
          }).then(response => {
            if (response.data.result === 200) {
              val['pcRemarks']=response.data.data.orderUser.pcRemarks
              val['pmcRemarks']=response.data.data.orderUser.pmcRemarks
              val['producerRemarks']=response.data.data.orderUser.producerRemarks
              val['qcRemarks']=response.data.data.orderUser.qcRemarks
              val['engineerRemarks']=response.data.data.orderUser.engineerRemarks
            } else {
              this.$alertWarning(response.data.data);
            }
          }).catch(err => {
            this.$alertDanger('未知错误')
          }).finally(() => {
            this.isPending = false;
            this.$closeLoading();
          })
          if(val.reworkZhidan !== null){
            this.isReworkEdit = true
          }
          this.orderEditType = 'edit';
          if(val['reworkQuantity'] && val['reworkZhidan']) {
            this.$set(this.orderEditOptionsData, 'reworkQuantity', val['reworkQuantity'])
            this.$set(this.orderEditOptionsData, 'reworkZhidan', val['reworkZhidan'])
          }
          Object.keys(val).forEach(item => {
            this.orderEditOptions.forEach(option => {
              if (item === option.key && val[item] !== null) {
                this.$set(this.orderEditOptionsData, item, val[item])
              }
            })
          });
          this.$set(this.orderEditOptionsData, 'id', val.id)

          this.isOrderEditing = true;
        } else if (type === 'copy') {
          if(val.reworkZhidan !== null){
            this.isReworkEdit = true
          }
          this.orderEditType = 'copy';
          Object.keys(val).forEach(item => {
            this.orderEditOptions.forEach(option => {
              if (item === option.key && val[item] !== null) {
                this.$set(this.orderEditOptionsData, item, val[item])
              }
            })
          });
          this.$set(this.orderEditOptionsData, 'zhidan', '');
          this.$set(this.orderEditOptionsData, 'reworkZhidan', '');
          this.$set(this.orderEditOptionsData, 'reworkQuantity', '');
          this.isOrderEditing = true;
        } else if (type === 'add') {
          this.orderEditType = 'add';
          this.isOrderEditing = true;
        } else if (type === "rework"){
          this.isReworkEdit = true;
          this.orderEditType = 'rework';
          this.isDisabled = false;
          Object.keys(val).forEach(item => {
            this.orderEditOptions.forEach(option => {
              if (item === option.key && val[item] !== null) {
                this.$set(this.orderEditOptionsData, item, val[item])
              }
            })
          });
          this.$set(this.orderEditOptionsData, 'rework', '');
          this.isOrderEditing = true;
        }


      },



      //灰色返工按钮
      unselected:function(){
        // this.$message({
        //   message: '请选择需返工的订单',
        //   type: 'warning'
        // });
        this.$alertWarning('请选择需返工的订单');
      },

      closeEditOrderPanel: function () {
        this.isOrderEditing = false;
      },

      submitEditOrder: function () {
        this.$refs['orderEditForm'].validate((isValid) => {
          if (isValid) {
            this.isPending = true;
            this.$openLoading();
            let options = {
              url: '',
              data: this.orderEditOptionsData
            };
            if (this.orderEditType === 'edit') {
              options.url = planOrderEditUrl
              options.data.isRework = this.isRework
            } else if (this.orderEditType === 'add' || this.orderEditType === 'rework' || this.orderEditType === 'copy') {
              if(!options.data.reworkQuantity || Number(options.data.reworkQuantity) <= options.data.quantity){
                options.url = planOrderAddUrl;
                options.data.isRework = this.isReworkEdit;
              }else{
                this.$alertWarning('请输入正确的返工单数');
                this.$closeLoading();
                return
              }
            }
            axiosFetch(options).then(response => {
              if (response.data.result === 200) {
                this.$alertSuccess('操作成功');
                this.resetEditOrderForm();
                this.closeEditOrderPanel();
                this.partlyReload();
                this.isDisabled= true;
              } else {
                this.$alertWarning(response.data.data)
              }
            }).catch(err => {
              this.$alertDanger("未知错误")

            }).finally(() => {
              this.isPending = false;
              this.$closeLoading();
              this.isReworkEdit = false;
              this.$set(this.orderEditOptionsData,'factory',undefined)
              //this.reload();
            })
          } else {
            this.$alertInfo('请完善表单信息')
          }
        })
      },
      resetEditOrderForm: function () {
        this.isReworkEdit = false;
        this.isDisabled = true;
        this.orderEditOptionsData = {};
        this.$set(this.orderEditOptionsData,'factory',undefined);
        this.$refs['orderEditForm'].clearValidate();
      },
      deleteData: function (val) {
        this.deletingItem = val;
        this.isOrderDeleting = true;
      },
      submitDeleteOrder: function (val) {
        if (!this.deleteReason) {
          this.$alertInfo('请填写作废理由');
          return;
        }
        MessageBox.confirm('将作废该订单，是否继续?', '提示', {
          confirmButtonText: '确认',
          cancelButtonText: '取消',
          type: 'warning'
        }).then(() => {
          this.$openLoading();
          axiosFetch({
            url: planOrderDeleteUrl,
            data: {
              id: this.deletingItem.id,
              deleteReason: this.deleteReason
            }
          }).then(response => {
            if (response.data.result === 200) {
              this.$alertSuccess('作废成功');
              this.closeDeleteOrderPanel();
              this.partlyReload();
              //this.reload();
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.$alertDanger("未知错误")
          }).finally(() => {
            this.$closeLoading();
          })
        }).catch(() => {
        })
      },
      closeDeleteOrderPanel: function () {
        this.isOrderDeleting = false;
        this.deletingItem = {};
        this.deleteReason = "";
        this.uploadAble = true;

      },

      /*异常结单*/
      abnormalEndData: function (val) {
        this.abnormalEndItem = val;
        this.isAbnormalEnd = true;
      },
      submitAbnormalEnd: function () {

        if (!this.abnormanEndRemarks) {
          this.$alertInfo('请填写异常结单理由');
          return;
        }
        MessageBox.confirm('将异常结单，是否继续?', '提示', {
          confirmButtonText: '确认',
          cancelButtonText: '取消',
          type: 'warning'
        }).then(() => {
          this.$openLoading();
          axiosFetch({
            url: planOrderDeleteUrl,
            data: {
              // zhidan:this.abnormalEndItem.zhidan,
              // orderDate:this.abnormalEndItem.orderDate,
              // deliveryDate:this.abnormalEndItem.deliveryDate,
              // softModel:this.abnormalEndItem.softModel,
              // quantity:this.abnormalEndItem.quantity,
              id: this.abnormalEndItem.id,
              abnormalEndReason: this.abnormanEndRemarks
            }
          }).then(response => {
            if (response.data.result === 200) {
              this.$alertSuccess('异常结单成功');
              this.closeAbnormalEndPanel();
              this.partlyReload();
              //this.reload();
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.$alertDanger("未知错误")
          }).finally(() => {
            this.$closeLoading();
          })
        }).catch(() => {
        })
      },
      closeAbnormalEndPanel: function () {
        this.isAbnormalEnd = false;
        this.abnormalEndItem = {};
        this.abnormanEndRemarks = "";
      },

      //确认订单
      confirmOrder(val){
        MessageBox.confirm('将确认此订单，是否继续?', '提示', {
          confirmButtonText: '确认',
          cancelButtonText: '取消',
          type: 'warning'
        }).then(() =>{
          this.$openLoading();
          this.isPending = true;
          let options = {
            url: planOrderEditUrl,
            data: {}
          };
          Object.keys(val).forEach(item => {
            options.data[item] = JSON.parse(JSON.stringify(val[item]))
          });
          if (val.reworkZhidan || val.reworkQuantity){
            options.data.isRework = true
          }else{
            options.data.isRework = false
          }
          options.data.orderStatus = 2
          axiosFetch(options).then(response => {
            if (response.data.result === 200) {
              this.$alertSuccess('确认成功');
              this.partlyReload();
              //this.reload();
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.$alertDanger("未知错误")
          }).finally(() => {
            this.$closeLoading();
            this.isPending = false;
          })
        })
      },


      /**
       **@description: 上传导入订单
       **@date: 2019/7/23 8:51
       **@author: DarkNin
       **@method:
       **@params: mod
       */
      submitOrderUpload: function () {
        this.uploadFileData = new FormData();
        this.$refs.orderUpload.submit();
        this.uploadFileData.append('isRework', this.isReworkUpload);
        this.uploadFileData.append('#TOKEN#', this.$store.state.token);
        let config = {
          header: {
            'Content-Type': 'multipart/form-data'
          },
        };
        this.$axios.post(planOrderImportUrl, this.uploadFileData, config).then(response => {
          if (response.data.result === 200) {
            this.$alertSuccess(response.data.data);
            this.partlyReload();
            //this.reload();
          } else {
            this.$alertWarning(response.data.data);
          }

        }).catch(err => {
        }).finally(() => {
          this.isPending = false;
          this.$closeLoading();
        });

      },
      uploadFile: function (params) {
        this.uploadFileData.append(Math.floor(Math.random() * 1000), params.file)
      },
      clearOrderUploadFile: function () {
        this.$refs.orderUpload.clearFiles();
      },
      beforeOrderUpload: function () {
        this.isPending = true;
        this.$openLoading();
      },


      /**
       **@description: 订单详情
       **@date: 2019/7/25 10:38
       **@author: DarkNin
       **@method:
       **@params: mod
       */
      showDetails: function (val) {
        let orderStatus = val.orderStatus
        if(orderStatus === 6 || orderStatus === 7 || orderStatus === 8){  //已完成或结束的订单不能作废文件
          this.uploadAble = false
        }
        this.fetchFileType()
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          this.showingItem = val;
          this.mergeData = {};
          this.mergePos = {};
          this.detailsTableData = [];

          axiosFetch({
            url: planOrderDetailsSelectUrl,
            data: {
              id: val.id
            }
          }).then(response => {
            if (response.data.result === 200) {
              this.showingItemInfo = response.data.data.orderUser;
              this.isDetailsShowing = true;
              this.showingItemFileInfo = response.data.data.orderFileInfo;
              this.showingItemFileInfo.forEach(item=>{
                if(item.deleteName !== null || val.orderStatus === 7 ||  val.orderStatus === 6){  //完成及以后的订单，不能改变文件状态    已完成或结束的订单不能作废文件
                  item.fileEditable = false;
                  item.fileDeleteable = false;
                }
                // if(item.deleteName !== null || val.orderStatus !== 1 || val.orderStatus !== 2){
                //   item.fileDeleteable = false;
                // }
                if(item.isNormal === false){
                  item.isNormal ="异常"
                }else if(item.isNormal === true){
                  item.isNormal ="正常"
                }else{
                  item.isNormal ="未审核"
                }
              })
              this.detailsTableData = this.showingItemFileInfo;
              if (this.detailsTableData.length !== 0) {
                this.getSpanArr(this.detailsTableData, this.mergeKeys)
              }
            } else {
              this.$alertWarning(response.data.data);
            }
          }).catch(err => {
            console.log(err)
            this.$alertDanger('未知错误')
          }).finally(() => {
            this.isPending = false;
            this.$closeLoading();
          })
        }
      },

      /*详情表格行合并，获取合并记录*/
      getSpanArr: function (tableData, keyName) {
        keyName.forEach((kitem, k) => {
          tableData.forEach((data, i) => {
            if (i === 0) {
              this.mergeData[kitem] = this.mergeData[kitem] || [];
              this.mergeData[kitem].push(1);
              this.mergePos[kitem] = 0
            } else {
              // 判断当前元素与上一个元素是否相同
              if (data[kitem] === tableData[i - 1][kitem]) {
                this.mergeData[kitem][this.mergePos[kitem]] += 1;
                this.mergeData[kitem].push(0)
              } else {
                this.mergeData[kitem].push(1);
                this.mergePos[kitem] = i
              }
            }
          })
        });
      },
      detailsTableSpanMethod: function ({row, column, rowIndex, columnIndex}) {
        if (this.mergeProp.includes(column.property)) {
          const _row = this.mergeData[column.property][rowIndex];
          const _col = _row > 0 ? 1 : 0;
          return {
            rowspan: _row,
            colspan: _col
          }
        }
      },

      downloadDetailsTable: function (val) {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          axiosDownload({
            url: planOrderTableDownloadUrl,
            data: {
              id: val.id
            }
          }).then(response => {
            let contentType = response.request.getResponseHeader('content-type');
            if (contentType === 'application/vnd.ms-excel' || contentType === 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet' || contentType === 'application/octet-stream') {

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

      /*删除订单文件*/
      deleteDetailsTable(val){
        this.isDeleteFile= true;
        this.deleteFileItem= val;
      },
      submitDeleteFile: function () {
        if (!this.deleteFileRemarks) {
          this.$alertInfo('请填写文件作废理由');
          return;
        }
        MessageBox.confirm('将作废该文件，是否继续?', '提示', {
          confirmButtonText: '确认',
          cancelButtonText: '取消',
          type: 'warning'
        }).then(() => {
          this.$openLoading();
          this.isPending = true;
          axiosFetch({
            url: planOrderTableDeleteUrl,
            data: {
              id: this.deleteFileItem.id,
              deleteReason: this.deleteFileRemarks
            }
          }).then(response => {
            if (response.data.result === 200) {
              this.$alertSuccess('删除成功');
              this.partlyReload();
              //this.reload();
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.$alertDanger("未知错误")
          }).finally(() => {
            this.$closeLoading();
            this.isPending = false;
          })
        }).catch(() => {

        })
      },
      closeDeleteFilePanel: function () {
        this.isDeleteFile = false;
        this.deleteFileItem = {};
        this.deleteFileRemarks = "";
      },

      //打开改变文件状态的对话框
      changeFileStatus(val){
        this.orderFileStatus = true
        this.orderInfo = val;
      },

      /*提交订单中文件的状态*/
      submitFileStatus: function () {
          this.$openLoading();
          this.isPending = true;
          axiosFetch({
            url: planOrderTableEditUrl,
            data: {
              id: this.orderInfo.id,
              isNormal:this.fileStatus,
              // remark:"文件状态修改"
            }
          }).then(response => {
            if (response.data.result === 200) {
              this.$alertSuccess('修改成功');
              this.partlyReload();
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            console.log(err)
            this.$alertDanger("未知错误")
          }).finally(() => {
            this.$closeLoading();
            this.isPending = false;
          }).catch(() => {
              })
      },
      //关闭文件状态页
      closeOrderStatusPanel(){
        this.fileStatus = ""
      },

      //获取工厂
      fetchFileType: function () {
        return new Promise(resolve => {
          axiosFetch({
            url: planFileTypeSelectUrl,
            // data: {
            //   pageNo: this.paginationOptions.currentPage,
            //   pageSize: this.paginationOptions.pageSize
            // }
          }).then(response => {
            if (response.data.result === 200) {
              this.fileTypeList = response.data.data.list;
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            console.log(err)
            this.$alertDanger('获取文件类型信息失败，请刷新重试');
          }).finally(() => {
            resolve();
          })
        });
      },


      /**
       **@description: 订单详情 上传 部分复用订单上传
       **@date: 2019/7/29 10:52
       **@author: DarkNin
       **@method:
       **@params: mod
       */
      resetDetailsUploadDialog: function () {
        this.$refs.orderDetailsUpload.clearFiles();
        this.isOrderDetailsUploading = false;
        this.orderDetailsType = "";
      },

      clearOrderDetailsUploadFile: function () {
        this.$refs.orderDetailsUpload.clearFiles();
      },
      submitDetailsOrderUpload: function () {
        this.isPending = true;
        this.$openLoading();
        this.uploadFileData = new FormData();
        this.$refs.orderDetailsUpload.submit();
        this.uploadFileData.append('type', this.orderDetailsType);
        this.uploadFileData.append('id', this.showingItem.id);
        this.uploadFileData.append('#TOKEN#', this.$store.state.token);
        let config = {
          header: {
            'Content-Type': 'multipart/form-data'
          },
        };
        this.$axios.post(planOrderTableImportUrl, this.uploadFileData, config).then(response => {
          if (response.data.result === 200) {
            this.$alertSuccess(response.data.data);
            this.isPending = false;
            this.resetDetailsUploadDialog();
            this.showDetails(this.showingItem);
          } else {
            this.$alertWarning(response.data.data);
          }

        }).catch(err => {

        }).finally(() => {

          this.isPending = false;
          this.$closeLoading();
        });
      }
    }
  }
</script>

<style scoped>
  #order-setting {
    position: relative;
    width: 100%;
    min-height: 600px;
    height: 100%;
  }

  .order-setting-main {
  }

  .query-comp {
    background-color: #ffffff;
    border: 1px solid #eeeeee;
    border-radius: 5px;
    padding: 10px 20px;
    display: flex;
    flex-wrap: wrap;
    min-height: 60px;
  }

  .query-comp-container {
    margin: 0 16px 10px 0;
  }

  .query-comp label {
    display: block;
    line-height: 24px;
    font-size: 14px;
  }

  .query-comp-text {
    width: 220px;
  }

  .query-comp-select {
    width: 220px !important;
  }

  .query-comp-select .el-select {
    width: 100%;
  }

  .query-comp-select /deep/ input {
    padding-right: 0;
  }

  .query-comp-container .el-button {
    margin-top: 24px;
    width: 80px;
  }

  .content-comp {
    background-color: #ffffff;
    border: 1px solid #eeeeee;
    border-radius: 5px;
    padding: 10px;
    margin-top: 10px;
  }

  .order-edit-form {
    display: flex;
    flex-wrap: wrap;
  }

  .order-edit-form-comp {
    padding: 0 10px;
  }

  .order-edit-form-comp label {
    font-size: 15px;
    line-height: 24px;
    padding: 0;
  }

  .order-edit-form-comp-text {
    width: 210px;
  }

  .order-edit-form-comp-text-rework{
    width: 440px;
  }

  /*.order-edit-form-comp-text-rework /deep/ .el-input{*/
  /*  width: 210px;*/
  /*}*/
  .order-edit-form-comp-text /deep/ .el-input {
    width: 210px;
  }

  .order-edit-form-comp-select{
    width: 210px;
    margin: 0 10px;

  }

  .order-edit-form-comp-textarea {
    width: 320px;
  }

  .order-delete-msg-title {
    font-size: 16px;
    line-height: 26px;
    margin-bottom: 10px;
  }

  .order-delete-msg-title span {
    font-weight: bold;
  }

  .details-header {
    display: flex;
    flex-direction: column;
    min-height: 50px;
    flex-wrap: wrap;
    color: #909399;
    border-bottom: 1px solid #eeeeee;
    margin-bottom: 10px;
  }

  .details-header-container-mes {
    width: 100%;
    display: flex;
    margin-bottom: 25px;
    justify-content: space-between;
    margin-top: -25px;

  }

  .details-header-container-remark {
    width: 100%;
    display: flex;
    justify-content: space-between;
    flex-wrap: wrap;

  }

  .details-header-container-remark > .details-header-item {
    width: 30%;
    margin-bottom: 10px;
  }

  .details-header-item-label, .details-header-item-content {
    display: inline-block;
    width: auto;
    padding: 0 8px;
    line-height: 20px;
    font-size: 14px;
    word-wrap: break-word;
    word-break: normal
  }

  .details-header-item-content {
    /*width: 500px; */
    word-break: break-all;
    word-wrap: break-word;
  }

  .details-content /deep/ td {
    padding: 4px 0;
  }

  .page-pagination {
    display: flex;
    flex-wrap: wrap;
    margin: 20px 0;
    padding: 0 20px;
  }

</style>
