<template>
  <div id="capacity-setting">
    <div class="capacity-setting-main">
      <div class="query-comp">
        <div class="query-comp-container" v-for="(item, index) in queryOptions" :key="index">
          <!--纯文本框-->
          <div class="query-comp-text" v-if="item.type === 'text'">
            <label :for="item.key + index">{{item.label}}:</label>
            <el-input v-model="thisQueryOptions[item.key].value" :id="item.key + index"
                      :placeholder="'请填写' + item.label" size="small"></el-input>
          </div>
        </div>
        <!--选择器-->
        <div class="query-comp-container" v-if="!!thisQueryOptions.processGroup">
          <div class="query-comp-select">
            <label for="process-query-item">工序组:</label>
            <el-select
              v-model="thisQueryOptions['processGroup'].value"
              id="process-query-item"
              placeholder="请选择工序"
              size="small">
              <el-option v-for="listItem in processGroupSelectGroup"
                         :key="listItem.id"
                         :value="listItem.id"
                         :label="listItem.groupName"></el-option>
            </el-select>
          </div>
        </div>
        <div class="query-comp-container">
          <el-button type="info" @click="initQueryOptions" size="small">重置条件</el-button>
        </div>
        <div class="query-comp-container">
          <el-button type="primary" size="small" @click="queryData">查询</el-button>
        </div>
        <div class="query-comp-container"
             v-if="permissionControl(['engineer'])">
          <el-button type="primary" size="small" @click="editData('add')">新增</el-button>
        </div>
      </div>
      <div class="content-comp">
<!--        :span-method="detailsTableSpanMethod"-->
        <el-table
          :data="tableData"
          max-height="560"
          :span-method="detailsTableSpanMethod"
          border
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
            :index="indexMethod"
            fixed="left"
            width="60">
          </el-table-column>

          <el-table-column
            label="删除"
            width="100"
            fixed="right"
            v-if="permissionControl(['engineer'])"
          >
            <template slot-scope="scope">
              <el-tooltip content="删除" placement="top">
                <el-button type="text" @click="deleteData(scope.row)" icon="el-icon-delete"></el-button>
              </el-tooltip>
            </template>
          </el-table-column>
          <el-table-column
            label="操作"
            width="100"
            fixed="right"
            v-if="permissionControl(['engineer'])"
          >
            <template slot-scope="scope">
              <el-tooltip content="编辑" placement="top">
                <el-button type="text" @click="copyCapacity('edit', scope.row)" icon="el-icon-edit-outline"></el-button>
              </el-tooltip>
              <el-tooltip content="复制" placement="top">
                <el-button type="text" @click="copyCapacity('add',scope.row)" icon="el-icon-t-copy"></el-button>
              </el-tooltip>
            </template>
          </el-table-column>
        </el-table>
      </div>
    </div>

    <!--dialog component (新增和复制）-->
    <el-dialog
      :title="editPanelTitle"
      :visible.sync="isCapacityAdd"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @closed="resetEditCapacityForm"
      width="970px">
      <el-form
        ref="capacityEditForm"
        :model="capacityEditOptionsData"
        class="capacity-edit-form"
        label-position="top"
        @submit.native.prevent
        :rules="capacityEditOptionsRules">
<!--          <el-form-item size="small" class="capacity-edit-form-comp" label="选择工厂" prop="factory" v-if="sessionFactory === '0'">-->
<!--          <el-select v-model="getGroupInfoFactoryId" id="factory01"-->
<!--                     autocomplete="off"-->
<!--                     @change="changeGroupList"-->
<!--                     placeholder="请选择工厂" size="small">-->
<!--            <el-option v-for="listItem in factoryList"-->
<!--                       :key="listItem.id"-->
<!--                       :value="listItem.id"-->
<!--                       :label="listItem.abbreviation"></el-option>-->
<!--          </el-select>-->
<!--          </el-form-item>-->
        <el-form-item size="small" class="capacity-edit-form-comp" label="客户编号" prop="customerNumber">
          <el-select v-model="capacityEditOptionsData.customerNumber" placeholder="请选择客户编号" class="capacity-edit-form-comp-text"
                     @change="choiceCustomer">
            <el-option v-for="listItem in customerDatas"
                       :key="listItem.id"
                       :value="listItem.customerNumber"
                       :label="listItem.customerNumber"></el-option>
          </el-select>
        </el-form-item>
        <el-form-item
          size="small"
          class="capacity-edit-form-comp"
          label="客户名称">
          <div class="capacity-edit-form-comp-text">
            <el-input
              type="text"
              placeholder="客户名称"
              disabled
              autocomplete="off"
              v-model="customerNames"></el-input>
          </div>
        </el-form-item>

        <el-form-item
          size="small"
          class="capacity-edit-form-comp"
          v-for="(item, index) in capacityAddOptions"
          :key="index"
          :label="item.label + '：'"
          :prop="item.key">
          <div class="capacity-edit-form-comp-text" v-if="item.type === 'text'">
            <el-input
              type="text"
              :id="'edit' + item.key + index" :placeholder="'请填写' + item.label"
              clearable
              autocomplete="off"
              v-model="capacityEditOptionsData[item.key]"></el-input>
          </div>
        </el-form-item>


        <el-form-item>
          <el-table
            :data="sameGroupDatas"
            max-height="560"
            border
            ref="tablecomponent">
            <el-table-column
                             v-for="(item, index) in capacityEditTableColumns"
                             :key="index"
                             :prop="item.key"
                             :label="item.label"
                             :min-width="item['min-width']"
                             :formatter="item.formatter">
              <template slot-scope="scope">
                <div class="copy-capacity-data">
                  <el-input v-model="scope.row[scope.column.property]" :disabled="scope.column.property === 'groupName'"></el-input>
                </div>
              </template>
            </el-table-column>



            <el-table-column
              type="index"
              :index="indexMethod"
              fixed="left"
              width="60">
            </el-table-column>
          </el-table>
        </el-form-item>
      </el-form>
      <span slot="footer" class="dialog-footer">
        <el-button size="small" @click="closeEditCapacityPanel" type="info">取消</el-button>
        <el-button size="small" @click="submitEditCapacity('add')" type="primary">保存</el-button>
      </span>
    </el-dialog>

    <!-- 编辑dialog -->
    <el-dialog
      :title="editPanelTitle"
      :visible.sync="isCapacityEditing"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @closed="resetEditCapacityForm"
      width="730px">
      <el-form
        ref="capacityEditForm"
        :model="capacityEditOptionsData"
        class="capacity-edit-form"
        label-position="top"
        @submit.native.prevent
        :rules="capacityEditOptionsRules">
        <el-form-item size="small" class="capacity-edit-form-comp" label="工序组" prop="processGroup">
          <el-select v-model="capacityEditOptionsData.processGroup" class="capacity-edit-form-comp-text"
                     @change="processGroupEditChange" placeholder="请选择工序组" disabled>
            <el-option v-for="listItem in processGroupSelectGroup"
                       :key="listItem.id"
                       :value="listItem.id"
                       :label="listItem.groupName"></el-option>
          </el-select>
        </el-form-item>
        <el-form-item
          size="small"
          class="capacity-edit-form-comp"
          v-for="(item, index) in capacityEditOptions"
          :key="index"
          :label="item.label + '：'"
          :prop="item.key">
          <div class="capacity-edit-form-comp-text" v-if="item.type === 'text'" >
            <el-input
              type="text"
              :id="'edit' + item.key + index" :placeholder="'请填写' + item.label"
              clearable
              :disabled="item.key === 'softModel'"
              autocomplete="off"
              v-model="capacityEditOptionsData[item.key]"></el-input>
          </div>
          <div class="capacity-edit-form-comp-text" v-if="item.type === 'date'">
            <el-date-picker
              :id="'edit' + item.key + index"
              v-model="capacityEditOptionsData[item.key]"
              type="date"
              placeholder="选择日期"
              value-format="yyyy-MM-dd"></el-date-picker>
          </div>
          <div class="capacity-edit-form-comp-textarea" v-if="item.type === 'textArea'">
            <el-input type="textarea"
                      :rows="4"
                      :id="'edit' + item.key + index" :placeholder="'请填写' + item.label"
                      clearable
                      autocomplete="off"
                      v-model="capacityEditOptionsData[item.key]"></el-input>
          </div>
        </el-form-item>
      </el-form>
      <span slot="footer" class="dialog-footer">
        <el-button size="small" @click="closeEditCapacityPanel" type="info">取消</el-button>
        <el-button size="small" @click="submitEditCapacity('edit')" type="primary">保存</el-button>
      </span>
    </el-dialog>
    <el-pagination
      background
      :current-page.sync="paginationOptions.currentPage"
      :page-sizes="[18, 36, 54 ]"
      :page-size.sync="paginationOptions.pageSize"
      layout="total, sizes, prev, pager, next, jumper"
      :total="paginationOptions.total"
      @current-change="pageChange"
      @size-change="queryData"
      class="page-pagination">
    </el-pagination>
  </div>
</template>

<script>
  import {
    capacityQueryOptions,
    capacityTableColumns,
    capacityEditOptions,
    capacityEditOptionsRules,
    capacityEditTableColumns,
    capacityAddOptions, sessionFactory
  } from "../../../config/planConfig";
  import {
    planCapacityAddUrl,
    planCapacityDeleteUrl,
    planCapacityEditUrl,
    planCapacitySelectUrl,
    planProcessGroupGetUrl,
    planProcessGetUrl,
    eSopCustomerSelectUrl, eSopFactorySelectUrl
  } from "../../../config/globalUrl";
  import {axiosFetch} from "../../../utils/fetchData";
  import {MessageBox} from "element-ui"
  import {closeLoading} from "../../../utils/loading";

  export default {
    name: "CapacitySetting",
    inject: ['reload'],
    data() {
      return {
        sessionFactory:sessionFactory,
        queryOptions: capacityQueryOptions,
        thisQueryOptions: {},
        processSelectGroupSrc: [], //工序信息 源
        processSelectGroup: [], //工序信息
        processGroupSelectGroup: [], //工序组信息
        customerDatas:[], //客户信息
        customerNames:"",
        tableData: [],
        tableColumns: capacityTableColumns,
        getGroupInfoFactoryId:"",//超级管理员用来 选择给哪个工厂增加产能的  工厂id
        paginationOptions: {
          currentPage: 1,
          pageSize: 18,
          total: 0
        },
        /*编辑新增产能信息*/
        factoryList:[],  //工厂
        isCapacityEditing: false,
        sameGroupDatas:[],  //点击复制后信息存储
        capacityAddOptions: capacityAddOptions,
        capacityEditOptions: capacityEditOptions,
        capacityEditTableColumns: capacityEditTableColumns,
        capacityEditOptionsRules: capacityEditOptionsRules,
        capacityEditType: '',
        isCapacityAdd:false,
        capacityEditOptionsData: {
          'process': '',
          customerNumber:"",
        },
        Parameter:true,  //缺少参数


        mergeData: {},//合并行的记录
        mergePos: {},//mergeData中每项的索引
        // mergeProp: ['softModel','customerNumber','customerName'],
        mergeProp: ['softModel'],
        mergeKeys: ['softModel'],
      }
    },
    computed: {
      editPanelTitle: function () {
        if (this.capacityEditType === 'edit') {
          return '编辑'
        } else if (this.capacityEditType === 'add') {
          return '新增'
        }
      },
    },
    async created() {
      this.initQueryOptions();
      await this.dataPreload();
    },
    mounted() {
      this.fetchData();
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
        _partlyReload(['thisQueryOptions', 'capacityEditOptions', 'processSelectGroupSrc', 'processGroupSelectGroup'])
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
        });
        this.$set(this.thisQueryOptions, 'process', {
          type: 'select',
          value: ''
        })
        this.$set(this.thisQueryOptions, 'processGroup', {
          type: 'select',
          value: ''
        })
      },

      initEditOptions: function () {
        this.capacityEditOptions.forEach(item => {
          this.$set(this.capacityEditOptionsData, item.key, '')
        });
        this.$set(this.capacityEditOptionsData, 'processGroup', '');
        this.$set(this.capacityEditOptionsData, 'process', '');
      },

      dataPreload: async function () {
        return new Promise(resolve => {
          Promise.all([this.fetchProcessGroup(), this.fetchProcess()]).then(() => {
            resolve();
          })
        })
      },

      /*获取客户信息*/
      fetchCustomer() {
        this.$openLoading();
        let options = {
          url: eSopCustomerSelectUrl,
          data: {
            pageNo: this.paginationOptions.currentPage,
            pageSize: this.paginationOptions.pageSize
          }
        };
        axiosFetch(options).then(response => {
          if (response.data.result === 200) {
            this.customerDatas = response.data.data.list;
          } else {
            this.$alertWarning(response.data.data)
          }
        }).catch(err => {
          this.$alertDanger('未知错误')
        }).finally(() => {
          this.isPending = false;
          this.$closeLoading();

        })
      },
      choiceCustomer(val){
        this.customerDatas.forEach(item=>{
          if(item.customerNumber === val){
            this.customerNames=item.customerName
          }
        })
      },

      /*获取工序信息*/
      fetchProcess: function () {
        return new Promise(resolve => {
          axiosFetch({
            url: planProcessGetUrl
          }).then(response => {
            if (response.data.result === 200) {
              this.processSelectGroupSrc = response.data.data.list;
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.$alertDanger('获取工序信息失败，请刷新重试');
          }).finally(() => {
            resolve();
          })
        });
      },
      /*获取工序组信息*/
      fetchProcessGroup: function () {
        return new Promise(resolve => {
          axiosFetch({
            url: planProcessGroupGetUrl,

          }).then(response => {
            if (response.data.result === 200) {
              if(sessionFactory === '0'){
                this.processGroupSelectGroup = response.data.data.list
              }else {
                response.data.data.list.forEach(item => {
                  if (item.factoryId === Number(this.getGroupInfoFactoryId)) {
                    this.processGroupSelectGroup.push(item)
                  }
                })
              }
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.$alertDanger('获取工序组信息失败，请刷新重试');
          }).finally(() => {
            resolve();
          })
        });
      },

      //获取工厂
      fetchFactory: function () {
        return new Promise(resolve => {
          axiosFetch({
            url: eSopFactorySelectUrl,
            data: {
              pageNo: this.paginationOptions.currentPage,
              pageSize: this.paginationOptions.pageSize,
              // factory:sessionFactory
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

      // 翻页  条件查询
      pageChange: function () {
        this.fetchData();
      },

      //工厂更改  工序组即更改
      changeGroupList(){
        this.fetchProcessGroup()
      },

      indexMethod: function (index) {
        return index + (this.paginationOptions.currentPage - 1) * this.paginationOptions.pageSize + 1
      },

      queryData: function () {
        this.paginationOptions.pageNo = 1;
        this.paginationOptions.total = 0;
        this.fetchData();
      },

      fetchData: function () {

        if (!this.isPending) {
          this.getGroupInfoFactoryId = sessionFactory
          this.isPending = true;
          this.$openLoading();
          this.tableData = [];
          this.mergeData = {};
          this.mergePos = {};
          // this.fetchProcessGroup()
          let options = {
            url: planCapacitySelectUrl,
            data: {
              pageNo: this.paginationOptions.currentPage,
              pageSize: this.paginationOptions.pageSize,
            }
          };
          Object.keys(this.thisQueryOptions).forEach(item => {
            if (this.thisQueryOptions[item].value !== "") {
              options.data[item] = this.thisQueryOptions[item].value
            }
          });
          options.data.factory = sessionFactory
          axiosFetch(options).then(response => {
            if (response.data.result === 200) {
              this.getSpanArr(response.data.data.list, this.mergeKeys);
              this.tableData = response.data.data.list;
              this.tableData.forEach(item=>{      //取数据的时候  没数据就显示为空 避免 0 undefind  的情况
                Object.keys(item).forEach(con => {
                  if(item[con] === null || item[con] === undefined || item[con] === 0 || item[con] === ""){
                    item[con] = null
                  }
                })
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

      /**
       **@description: 编辑新增产线配置
       **@date: 2019/7/24 17:20
       **@author: DarkNin
       **@method:
       **@params: mod
       */
      editData: function (type, val) {
        this.fetchCustomer()
        this.fetchFactory()
        if (!!this.$refs['capacityEditForm']) {
          this.$refs['capacityEditForm'].clearValidate();
        }
        if(sessionFactory === '0'){
          this.getGroupInfoFactoryId =""
        }
        if (type === 'edit') {
          this.capacityEditType = 'edit';
          Object.keys(val).forEach(item => {
            this.capacityEditOptions.forEach(option => {
              if (item === option.key && val[item] !== null) {
                this.$set(this.capacityEditOptionsData, item, val[item])
              }
            })
          });
          //工序组、工序、id
          this.processSelectGroup = [];
          this.processSelectGroupSrc.forEach(item => {
            if (item.processGroup === val.processGroup) {
              this.processSelectGroup.push(item)
            }
          });
          this.$set(this.capacityEditOptionsData, 'processGroup', val.processGroup);
          this.$set(this.capacityEditOptionsData, 'process', val.process);
          this.$set(this.capacityEditOptionsData, 'id', val.id);

          // this.isCapacityEditing = true;
          this.isCapacityAdd = true;
        } else if (type === 'add') {
          this.processGroupSelectGroup.forEach((item,i)=>{
            let arr ={processGroup:""}
            this.sameGroupDatas.push(arr)

            this.$set(this.sameGroupDatas[i],'groupName' , item.groupName);
            this.$set(this.sameGroupDatas[i],'processGroup' , item.id);
          })

          // this.sameGroupDatas= 654
          // this.processGroupSelectGroup =457
          this.capacityEditType = 'add';
          this.isCapacityAdd = true;
        }
      },

      //复制产能信息
      copyCapacity(type,val){
        this.fetchCustomer();
        if(type === 'add'){
          this.capacityEditType = 'add';
        }else if(type === 'edit'){
          this.capacityEditType = 'edit';
        }
        let tempData = JSON.parse(JSON.stringify(this.tableData))
        tempData.forEach(item=>{
          if(item.customerNumber === val.customerNumber && item.softModel === val.softModel){
            this.sameGroupDatas.push(item)
          }
        })
        this.isCapacityAdd = true;
      },

      closeEditCapacityPanel: function () {
        this.isCapacityEditing = false;
        this.sameGroupDatas=[];
        this.isCapacityAdd =false;
      },
      //单条编辑
      submitEditCapacity: function () {
        this.sameGroupDatas.forEach(item=>{
          item.customerNumber= this.capacityEditOptionsData.customerNumber.toString();
          item.customerName= this.customerNames;
          item.softModel= this.capacityEditOptionsData.softModel;
          item.customerModel= this.capacityEditOptionsData.customerModel;
          item.capacity = Number(item.capacity)
          item.processPeopleQuantity = Number(item.processPeopleQuantity)
          item.transferLineTime = Number(item.transferLineTime)
          item.rhythm = Number(item.rhythm)
          if(item.capacity <= 0 || item.processPeopleQuantity <= 0){
            this.Parameter = false
          }
        })
        if(!this.Parameter){
          this.$alertWarning('设置数量有误')
          this.$closeLoading()
          return
        }
        this.$refs['capacityEditForm'].validate((isValid) => {
          if (isValid) {
            this.isPending = true;
            this.$openLoading();
            let options = {
              url: '',
              data:{
                modelCapacityString:[],
              },
            };
            if (this.capacityEditType === 'edit') {
              options.url = planCapacityEditUrl
              // options.data= this.capacityEditOptionsData  //只编辑单条产能
            } else if (this.capacityEditType === 'add') {
              options.url = planCapacityAddUrl
            }
            options.data.factory = sessionFactory
            this.sameGroupDatas.forEach(item=>{      //存数据的时候  没数据就为空 避免 0 undefind  的情况
              Object.keys(item).forEach(con => {
                if(item[con] === null || item[con] === undefined || item[con] === 0 || item[con] === ""){
                  item[con] = null
                }
              })
            })
            options.data.modelCapacityString= JSON.stringify(this.sameGroupDatas)
            axiosFetch(options).then(response => {
              if (response.data.result === 200) {
                this.resetEditCapacityForm();
                this.closeEditCapacityPanel();
                this.$alertSuccess('操作成功');
                this.partlyReload();
              }
              else {
                this.$alertWarning(response.data.data)
              }
            }).catch(err => {
              this.$alertDanger("未知错误")
            }).finally(() => {
              this.isPending = false;
              this.$closeLoading();
            })
          } else {
            this.$alertInfo('请完善表单信息')
          }
        })
      },

      resetEditCapacityForm: function () {
        this.processSelectGroup = [];
        this.capacityEditOptionsData = {};
        this.sameGroupDatas=[];
        this.customerNames ="";
        this.isCapacityAdd =false;
        // this.processGroupSelectGroup =[];
        this.factoryList=[];
        this.getGroupInfoFactoryId= "";
        this.$refs['capacityEditForm'].clearValidate();
        this.initEditOptions();
      },

      processGroupEditChange: function (val) {
        this.capacityEditOptionsData.process = '';
        this.processSelectGroup = [];
        if (val !== '') {
          this.processSelectGroupSrc.forEach(item => {
            if (item.processGroup === val) {
              this.processSelectGroup.push(item)
            }
          })
        }
      },

      deleteData: function (val) {
        MessageBox.confirm('将删除该配置，是否继续?', '提示', {
          confirmButtonText: '确认',
          cancelButtonText: '取消',
          type: 'warning'
        }).then(() => {
          this.$openLoading();
          this.isPending = true;
          axiosFetch({
            url: planCapacityDeleteUrl,
            data: {
              id: val.id
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
          const _row = this.mergeData[column.property][rowIndex]; //0,2,0,2,0,2
          const _col = _row > 0 ? 1 : 0;
          return {
            rowspan: _row,
            colspan: _col
          }
          // if(column.property === "copy"){
          //   const _row = this.mergeData[column.property][rowIndex];
          //   const _col = _row > 0 ? 1 : 0;
          //   return {
          //     rowspan: _row,
          //     colspan: _col
          //   }
          // }
        }

      },


      changePosition: function (val, direction) {
        let position;
        if (direction === 'up') {
          if (val.$index > 0) {
            position = this.tableData[val.$index - 1].id
          } else {
            return
          }
        } else if (direction === 'down') {
          if (val.$index < (this.tableData.length - 1)) {
            position = this.tableData[val.$index + 1].id
          } else {
            return
          }
        }
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          //初始化要提交的值，行内容填充
          this.capacityEditOptions.forEach(item => {
            this.$set(this.capacityEditOptionsData, item.key, '')
          });
          Object.keys(val.row).forEach(item => {
            this.capacityEditOptions.forEach(option => {
              if (item === option.key && val.row[item] !== null) {
                this.$set(this.capacityEditOptionsData, item, val.row[item])
              }
            })
          });
          this.$set(this.capacityEditOptionsData, 'processGroup', val.row.processGroup);
          this.$set(this.capacityEditOptionsData, 'process', val.row.process);
          this.$set(this.capacityEditOptionsData, 'id', val.row.id);
          this.$set(this.capacityEditOptionsData, 'position', position);
        }
        axiosFetch({
          url: planCapacityEditUrl,
          data: this.capacityEditOptionsData
        }).then(response => {
          if (response.data.result === 200) {
            this.$alertSuccess('操作成功');
            this.capacityEditOptionsData = {};
            this.isPending = false;
            this.queryData();
          } else {
            this.$alertWarning(response.data.data)
          }
        }).catch(err => {
          this.$alertDanger("未知错误")
        }).finally(() => {
          this.isPending = false;
          this.$closeLoading();
        })

      }
    }
  }
</script>

<style scoped>

  #capacity-setting /deep/ .el-button i{
    font-size: 17px;
    font-weight: bold;
  }
  #capacity-setting {
    position: relative;
    width: 100%;
    min-height: 600px;
    height: 100%;
  }

  .capacity-setting-main {
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

  .copy-capacity-data /deep/ input{
    border: 0;
    outline: none;
    background:rgba(0,0,0,0);
  }
  .content-comp {
    background-color: #ffffff;
    border: 1px solid #eeeeee;
    border-radius: 5px;
    padding: 10px;
    margin-top: 10px;
    /*height: 80vh;*/
  }
  .content-comp /deep/ .el-talbe{
    height: 80vh;
  }

  .capacity-edit-form {
    display: flex;
    flex-wrap: wrap;
  }

  .capacity-edit-form-comp {
    padding: 0 10px;
  }

  .capacity-edit-form-comp label {
    font-size: 15px;
    line-height: 24px;
    padding: 0;
  }

  .capacity-edit-form-comp-text {
    width: 210px;
  }

  .capacity-edit-form-comp-text /deep/ .el-input {
    width: 210px;
  }

</style>
