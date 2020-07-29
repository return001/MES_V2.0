<template>
  <div id="capacity-setting">
    <div class="capacity-setting-main">
      <div class="query-comp">

        <div class="query-comp-container" v-for="(item, index) in queryOptions" :key="index">
          <div class="query-comp-select" v-if="item.type === 'select'">
            <label :for="item.key + index">{{item.label}}:</label>
            <el-select v-model="thisQueryOptions[item.key].value" :id="item.key + index"
                       autocomplete="off"
                       :placeholder="'请选择' + item.label"
                       size="small" clearable>
              <el-option v-for="listItem in item.list"
                         :key="listItem.value"
                         :value="listItem.value"
                         :label="listItem.label"></el-option>
            </el-select>
          </div>
          <!--纯文本框-->
          <div class="query-comp-text" v-if="item.type === 'text'">
            <label :for="item.key + index">{{item.label}}:</label>
            <el-input v-model.trim="thisQueryOptions[item.key].value" :id="item.key + index"
                      :placeholder="'请填写' + item.label"
                      size="small" clearable></el-input>
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
              size="small" clearable>
              <el-option v-for="listItem in processGroupSelectWait"
                         :key="'wait'+listItem.id"
                         :value="listItem.id"
                         :label="listItem.groupName"></el-option>
            </el-select>
          </div>
        </div>

        <div class="query-comp-container">
          <el-button type="info" @click="initQueryOptions" size="small">重置条件</el-button>
        </div>
        <div class="query-comp-container">
          <el-button type="primary" size="small" @click="queryData()">查询</el-button>
        </div>
        <div class="query-comp-container">
          <el-button type="primary" size="small" @click="addCapacity('add')">新增</el-button>
<!--          <el-button type="primary" size="small" @click="editCapacity('add')">新增</el-button>-->
        </div>
      </div>
      <div class="content-comp">
<!--        :span-method="detailsTableSpanMethod"-->
        <el-table
          :data="tableData"
          max-height="560"
          :span-method="detailsTableSpanMethod"
          border
          ref="tablecomponent">
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
            width="60"
            fixed="right"
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
          >
            <template slot-scope="scope">
              <el-tooltip content="编辑" placement="top">
                <el-button type="text" @click="editCapacity('edit', scope.row)" icon="el-icon-edit-outline"></el-button>
              </el-tooltip>
              <el-tooltip content="复制" placement="top">
                <el-button type="text" @click="editCapacity('copy',scope.row)" icon="el-icon-t-copy"></el-button>
              </el-tooltip>
              <el-tooltip content="审核" placement="top">
                <el-button type="text" @click="checkCapacity(scope.row)" icon="el-icon-check" :disabled="scope.row.statusId !== 1 && scope.row.statusId !== null"></el-button>
              </el-tooltip>
            </template>
          </el-table-column>
        </el-table>
      </div>
    </div>

    <!-- 审核 -->
    <el-dialog
      title="产能审核"
      :visible.sync="isCapacityCheck"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @closed="closeCapacityCheckPanel"
      width="400px">
<!--      <div class="order-delete-msg-title">正在审核产能<span>{{deletingItem.zhidan}}</span></div>-->
      <div class="order-delete-msg-content">
        <template>
          <el-radio v-model="checkResult" :label= 3>通过</el-radio>
          <el-radio v-model="checkResult" :label= 2>拒绝</el-radio>
        </template>
        <div style="height: 15px"></div>
        <el-input type="textarea"
                  :rows="3"
                  autocomplete="off"
                  placeholder="审核备注"
                  v-model="capacityCheckReason">
        </el-input>
      </div>
      <span slot="footer" class="dialog-footer">
        <el-button size="small" @click="closeCapacityCheckPanel" type="info">取消</el-button>
        <el-button size="small" @click="submitCheckCapacity" type="warning">保存</el-button>
      </span>
    </el-dialog>

    <!--dialog component (新增和复制）-->
    <el-dialog
      :title="editPanelTitle"
      :visible.sync="isCapacityAdd"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @closed="resetEditCapacityForm"
      width="1040px">
<!--      closeEditCapacityPanel-->
      <el-form
        ref="capacityEditForm"
        :model="capacityEditOptionsData"
        class="capacity-edit-form"
        label-position="top"
        @submit.native.prevent
        :rules="capacityEditOptionsRules">
          <el-form-item size="small" class="capacity-edit-form-comp" label="选择工厂" prop="factory" v-if="sessionFactory === '1' && capacityEditType === 'add'">
            <el-select v-model="getGroupInfoFactoryId" id="factory01"
                       autocomplete="off"
                       @change="changeGroupList"
                       placeholder="请选择工厂" size="small">
              <el-option v-for="listItem in factoryList"
                         :key="listItem.id"
                         :value="listItem.id"
                         :label="listItem.abbreviation"></el-option>
            </el-select>
        </el-form-item>
        <el-form-item size="small" class="capacity-edit-form-comp" label="客户编号" prop="customerNumber">
          <el-select v-model="capacityEditOptionsData.customerNumber"
                     class="capacity-edit-form-comp-text"
                     placeholder="请选择客户编号"
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

<!--        <el-form-item>-->
        <div style="width: 100% !important;">
          <el-table
            :data="sameGroupDatas"
            class="capacity-edit-table"
            max-height="480"
            :header-cell-style="{'text-align':'center'}"
            border
            ref="tablecomponent">
            <el-table-column
              label="序号"
              type="index"
              fixed="left"
              width="60">
            </el-table-column>
              <el-table-column v-for="(item, index) in capacityEditTableColumns"
                               :key="index"
                               :prop="item.key"
                               :label="item.label"
                               :min-width="item['min-width']"
                               :formatter="item.formatter">
                <template slot-scope="scope">
                  <div class="copy-capacity-data">
                    <el-select v-model="scope.row[item.key]"
                               v-if="scope.column.property === 'groupName'"
                               @visible-change="replaceProcessGroup($event,scope.row)"
                               @change="choiceGroup(scope.row,$event)"
                               placeholder="选择工序组">
                      <el-option v-for="item in processGroupChoice"
                                 :key="item.id"
                                 :value="item.id"
                                 :label="item.groupName"
                      ></el-option>
                    </el-select>
<!--                      <el-input v-model.number="scope.row[item.key]"-->
<!--                                type="number"-->
<!--                                @keydown.native="inputLimit(scope,$event)"-->
<!--                                v-else-if="scope.column.property === 'processPeopleQuantity'">-->
<!--                      </el-input>-->

                    <el-input v-model="scope.row[item.key]"
                              type="text"
                              :width="capacityEditType === 'edit' ? '600' : '550'"
                              v-else-if="scope.column.property === 'remark'">
                    </el-input>

                    <el-input v-model.number="scope.row[item.key]"
                              type="number"
                              @keydown.native="inputLimit(scope,$event)"
                              @change="compareZero(scope,$event)"
                              v-else>
                    </el-input>
                  </div>
                </template>
              </el-table-column>
              <el-table-column
                label="操作"
                float="right"
                width="80"
                v-if="capacityEditType !== 'edit'"
              >
                <template slot-scope="scope">
                  <el-tooltip content="删除" placement="top">
                    <el-button size="small" type="primary" circle class="el-icon-minus" @click="handleDeleteCapacity(scope.row,scope.$index)"></el-button>
                  </el-tooltip>
                </template>
              </el-table-column>
          </el-table>
        </div>
        <div style="margin-top: 20px" v-if="processGroupSelect.length > sameGroupDatas.length">
          <el-tooltip content="添加" placement="top">
            <el-button size="small" type="primary" circle icon="el-icon-plus" @click="handleAddCapacity"></el-button>
          </el-tooltip>
        </div>
<!--        </el-form-item>-->
      </el-form>
      <span slot="footer" class="dialog-footer">
        <el-button size="small" @click="closeEditCapacityPanel" type="info">取消</el-button>
        <el-button size="small" @click="submitEditCapacity('add')" type="primary">保存</el-button>
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
    capacityAddOptions,
  } from "../../../config/planConfig";
  import {
    planCapacityAddUrl,
    planCapacityDeleteUrl,
    planCapacityEditUrl,
    planCapacitySelectUrl,
    planProcessGroupGetUrl,
    planProcessGetUrl,
    eSopCustomerSelectUrl,
    eSopFactorySelectUrl,
    planOrderTableDeleteUrl,
    planCapacityCheckUrl,
  } from "../../../config/globalUrl";
  import {axiosFetch} from "../../../utils/fetchData";
  import {MessageBox} from "element-ui"
  import {closeLoading} from "../../../utils/loading";

  export default {
    name: "CapacitySetting",
    inject: ['reload'],
    data() {
      return {
        sessionFactory:sessionStorage.getItem('factory'),
        queryOptions: capacityQueryOptions,
        thisQueryOptions: {},
        processSelectSrc: [], //工序信息 源
        processGroupSelect: [], //工序组信息
        processGroupChoice: [], //新增编辑时选工序组信息
        choicedGroup:{},        //新增、复制时 当前选择的工序组
        processGroupSelectWait:[], // factory=0的情况下 新增页面必须选择工厂才能显示工序组，先存到这
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
        /*审核产能*/
        checkItem:[],  //点击审核的行信息
        isCapacityCheck:false,
        capacityCheckReason:"",
        checkResult:1,

        /*编辑新增产能信息*/
        factoryList:[],  //工厂
        sameGroupDatas:[],  //点击复制后信息存储
        capacityAddOptions: capacityAddOptions,
        capacityEditOptions: capacityEditOptions,
        capacityEditTableColumns: capacityEditTableColumns,
        capacityEditOptionsRules: capacityEditOptionsRules,
        capacityEditType: '',
        isCapacityAdd:false,
        increasable:true,    //加号的显示与否
        diminishbb:true,     //减号的显示与否
        capacityEditOptionsData: {
          process: '',
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
        }else if (this.capacityEditType === 'copy') {
          return '复制'
        }
      },

      oneList(){
        let iList = [];
        this.tableData.forEach((row, rowIndex) => {
          let eIndex = iList.findIndex(e =>e.customerNumber === row.customerNumber && e.softModel === row.softModel && e.customerModel === row.customerModel);
          if(eIndex !== -1){
            iList[eIndex].length += 1;
          } else {
            iList.push({
              customerNumber: row.customerNumber,
              softModel: row.softModel,
              customerModel: row.customerModel,
              length: 1,
              index: rowIndex
            })
          }
        })
        return iList
      },
    },
    created() {    //async
      this.$store.commit('pageActionLimits',this.$store.state.charactersFuncMap.map.basic.basic.capacity)
      this.initQueryOptions();
      this.dataPreload();   //await
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
        _partlyReload(['thisQueryOptions', 'capacityEditOptions', 'processSelectSrc', 'processGroupSelect','processGroupSelectWait'])
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
      fetchCustomer(factoryId = this.sessionFactory) {
        this.$openLoading();
        let options = {
          url: eSopCustomerSelectUrl,
          data: {
            factoryId:factoryId,
            pageNo: 1,
            pageSize: this.paginationOptions.pageSize
          }
        };
        axiosFetch(options).then(response => {
          if (response.data.result === 200) {
            this.customerDatas = response.data.data.list
            // this.customerDatas = []
            // response.data.data.list.forEach(item=>{
            //   if(item.factoryId === Number(this.sessionFactory)){
            //     this.customerDatas.push(item);
            //   }
            // })
          } else {
            this.$alertWarning(response.data.data)
          }
        }).catch(err => {
          console.log(err)
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
              this.processSelectSrc = response.data.data.list;
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
              if(this.sessionFactory === '1'){
                this.processGroupSelectWait = response.data.data.list
              }else {
                this.processGroupSelectWait =[];
                this.processGroupSelect =[];
                response.data.data.list.forEach(item => {
                  if (item.factoryId === Number(this.sessionFactory)) {
                    this.processGroupSelectWait.push(item)
                    this.processGroupSelect.push(item)
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
              pageNo: 1,
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
      changeGroupList(val){
        this.fetchCustomer(val)
        this.customerNames="";
        this.capacityEditOptionsData={
          process: '',
          customerNumber:"",
        },
        this.processGroupSelect = []
        this.sameGroupDatas = []
          this.processGroupSelectWait.forEach(item=>{
            if(item.factoryId === val){
              this.processGroupSelect.push(item)
            }
          })

        // this.processGroupSelect.forEach((item,i)=>{
        //   let arr ={processGroup:""}
        //   this.sameGroupDatas.push(arr)
        //   this.$set(this.sameGroupDatas[i],'groupName' , item.groupName);
        //   this.$set(this.sameGroupDatas[i],'processGroup' , item.id);
        // })
        //显示的时候只显示三条,且选择时没有前三条
        this.processGroupChoice = JSON.parse(JSON.stringify(this.processGroupSelect))
        this.processGroupChoice.forEach((item,i)=>{
          if(i <= 2){
            let arr ={processGroup:""}
            this.sameGroupDatas.push(arr)
            this.$set(this.sameGroupDatas[i],'groupName' , item.groupName);
            this.$set(this.sameGroupDatas[i],'processGroup' , item.id);
          }
        })
        if(this.sameGroupDatas.length >=3){
          this.processGroupChoice.splice(0,3)
        }
      },

      indexMethod: function (index) {
        return index + (this.paginationOptions.currentPage - 1) * this.paginationOptions.pageSize + 1
      },

      queryData: function () {
        this.paginationOptions.currentPage = 1;
        this.paginationOptions.total = 0;
        this.fetchData();
      },

      fetchData: function () {
        if(this.$store.state.limits.select !== true){
          this.$alertWarning('暂无查询权限');
          return
        }
        if (!this.isPending) {
          this.getGroupInfoFactoryId = this.sessionFactory === '1'?'0':this.sessionFactory
          this.isPending = true;
          this.$openLoading();
          this.tableData = [];
          this.mergeData = {};
          this.mergePos = {};
          // this.fetchProcessGroup()
          // console.log(this.paginationOptions.currentPage)
          // console.log(this.paginationOptions.pageSize)
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
          options.data.factory = this.sessionFactory === '1'?'0':this.sessionFactory
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
       **@description: 新增产能配置
       **@date: 2019/7/24 17:20
       **@author: DarkNin
       **@method:
       **@params: mod
       */
      addCapacity: function () {
        if(this.$store.state.limits.add !== true){
          this.$alertWarning('暂无新增权限');
          return
        }
        this.fetchCustomer()
        this.fetchProcessGroup()
        if(this.sessionFactory === '1'){
          this.fetchFactory()
          this.getGroupInfoFactoryId =""
        }
        //显示的时候只显示三条,且选择时没有前三条
        this.processGroupChoice = JSON.parse(JSON.stringify(this.processGroupSelect))
        this.processGroupChoice.forEach((item,i)=>{
          if(i <= 2){
            let arr ={processGroup:""}
            this.sameGroupDatas.push(arr)
            this.$set(this.sameGroupDatas[i],'groupName' , item.groupName);
            this.$set(this.sameGroupDatas[i],'processGroup' , item.id);
          }
        })
        if(this.sameGroupDatas.length >=3){
          this.processGroupChoice.splice(0,3)
        }

        this.capacityEditType = 'add';
        this.isCapacityAdd = true;
      },

      //审核产能
      checkCapacity(val){
        if(this.$store.state.limits.checkout !== true){
          this.$alertWarning('暂无审核权限');
          return
        }
        this.isCapacityCheck = true;
        this.checkItem = val
      },

      closeCapacityCheckPanel(){
        this.isCapacityCheck = false;
        this.checkResult = '';
        this.capacityCheckReason ="";
      },

      submitCheckCapacity(){
        if(this.checkResult === 1){
          this.$alertInfo('请对产能进行审核');
          return;
        }
        MessageBox.confirm('将对产能进行审核操作，是否继续?', '提示', {
          confirmButtonText: '确认',
          cancelButtonText: '取消',
          type: 'warning'
        }).then(() => {
            this.$openLoading();
            this.isPending = true;
            axiosFetch({
              url: planCapacityCheckUrl,
              data: {
                softModel:this.checkItem.softModel,
                customerNumber:this.checkItem.customerNumber,
                factoryId:this.checkItem.factoryId,
                // factory:sessionFactory,
                customerModel:this.checkItem.customerModel,
                reviewRemark: this.capacityCheckReason,
                statusId:this.checkResult,
              }
            }).then(response => {
              if (response.data.result === 200) {
                this.$alertSuccess('操作成功');
                this.partlyReload();
                this.isCapacityCheck = false;
              } else {
                this.$alertWarning(response.data.data)
              }
            }).catch(err => {
              this.$alertDanger("未知错误")
            })
            .finally(() => {
              this.$closeLoading();
              this.isPending = false;
            })
          })
      },

      //编辑、复制产能信息
      editCapacity(type,row){

        this.fetchCustomer(row.factoryId);
        if(type === 'edit'){
          this.capacityEditType = 'edit';
          if(this.$store.state.limits.update !== true){
            this.$alertWarning('暂无编辑权限');
            return
          }
          this.isCapacityAdd = true;
        }else if(type === 'copy'){
          if(this.$store.state.limits.add !== true){
            this.$alertWarning('暂无复制权限');
            return
          }
          this.capacityEditType = 'copy';
          this.isCapacityAdd = true;
        }
        let tempData = JSON.parse(JSON.stringify(this.tableData))
        tempData.forEach(item=>{
          if(item.customerNumber === row.customerNumber && item.softModel === row.softModel){
            this.sameGroupDatas.push(item)
          }
        })
        this.$set(this.capacityEditOptionsData,'customerNumber',this.sameGroupDatas[0].customerNumber)
        this.$set(this.capacityEditOptionsData,'softModel',this.sameGroupDatas[0].softModel)
        this.$set(this.capacityEditOptionsData,'customerModel',this.sameGroupDatas[0].customerModel)
        // this.$set(this.capacityEditOptionsData,'statusId',row.statusId)
        // this.$set(this.capacityEditOptionsData,'statusName',row.statusName)
        this.customerNames= this.sameGroupDatas[0].customerName

        // 编辑时 也能选更多的工序组
        if(this.processGroupSelectWait.length>0){
          this.processGroupSelect = this.processGroupSelectWait.filter(item=>item.factoryId === row.factoryId)
          this.processGroupChoice = JSON.parse(JSON.stringify(this.processGroupSelect))
          let existGroup = this.sameGroupDatas.map(item=>item.processGroup)
          this.processGroupChoice=this.processGroupChoice.filter(item=>existGroup.indexOf(item.id) === -1)
        }else{
          this.$alertWarning('没有可用工序组')
        }
        console.log(this.capacityEditType)
      },

      inputLimit(scope,val){
        let key = val.key
        // 不允许输入'e'和、'.'、'+'、'-'、'Shift'
        if(scope.column.property === 'processPeopleQuantity'){
          if (key === 'e' || key === '.' ||key === '+' || key === '-' || key === 'Shift') {
            this.$alertWarning('请输入正整数')
            val.returnValue = false
          }
        }else{
          if (key === 'e' || key === '+' || key === '-' || key === 'Shift') {
            this.$alertWarning('请输入数字')
            val.returnValue = false
          }
        }
      },

      //节拍、产能、转线时常 可以是float  需要验证0
      compareZero(scope,val){
        if(val <= 0 && val !== ''){
          scope.row[scope.column.property] = ''
          this.$alertWarning('数值必须大于0')
        }
      },

      //获取当前的工序组，更改时加回到列表中
      replaceProcessGroup(val,row){
        if(val){
          this.processGroupSelect.forEach((item,index)=>{
            if(row.processGroup === item.id){
              this.choicedGroup=item;
            }
          })
        }
      },


      choiceGroup(row,val){
        this.processGroupChoice.forEach((item,i)=>{
          if(item.id === val){
            row.processGroup = val
            row.groupName = item.groupName
            this.processGroupChoice.splice(i,1)
          }
        })
        this.processGroupChoice.push(this.choicedGroup)
      },

      handleDeleteCapacity(row,index){
        if(this.sameGroupDatas.length<=1){
          this.$alertWarning('不可清空')
          return
        }
        this.processGroupSelect.forEach(item=>{
          if(item.id === row.processGroup){
            this.processGroupChoice.unshift(item)
          }
        })
        this.sameGroupDatas.splice(index,1)
      },
      handleAddCapacity(){
        this.sameGroupDatas.push(
          {
            factoryId:this.sameGroupDatas[0].factoryId,
            remark:"",
            abbreviation:this.sameGroupDatas[0].abbreviation,
            statusId:this.sameGroupDatas[0].statusId,
            tempcolumn:this.sameGroupDatas[0].tempcolumn,
            reviewRemark:this.sameGroupDatas[0].reviewRemark,
            reviewerName:this.sameGroupDatas[0].reviewerName,
            statusName:this.sameGroupDatas[0].statusName,
            temprownumber:this.sameGroupDatas[0].temprownumber,
            reviewTime:this.sameGroupDatas[0].reviewTime,
          }
          )
      },


      closeEditCapacityPanel: function () {
        this.sameGroupDatas=[];
        this.isCapacityAdd =false;
      },
      //新增、编辑、复制
      submitEditCapacity: function () {
        this.Parameter = true
        if(this.sameGroupDatas.length <= 0){
          this.$alertWarning('该工厂不存在可用工序组')
          return
        }
        this.sameGroupDatas.forEach(item=>{
          item.customerNumber= this.capacityEditOptionsData.customerNumber;
          item.customerName= this.customerNames;
          item.softModel= this.capacityEditOptionsData.softModel;
          item.customerModel= this.capacityEditOptionsData.customerModel;
          !item.processGroup || !item.processPeopleQuantity || !item.capacity ? this.Parameter = false : this.Parameter = true
        })
        if(!this.Parameter){
          this.$alertWarning('工序组、人数和产能为必填项')
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
            } else if (this.capacityEditType === 'add' || this.capacityEditType === 'copy') {
              options.url = planCapacityAddUrl
            }
            if(this.getGroupInfoFactoryId){
              options.data.factory = this.getGroupInfoFactoryId
            }else{
              options.data.factory = this.sessionFactory
            }
            // this.sameGroupDatas.forEach(item=>{      //存数据的时候  没数据就为空 避免 0 undefind  的情况
            //   Object.keys(item).forEach(con => {
            //     if(item[con] === null || item[con] === undefined || item[con] === 0 || item[con] === "" || item[con] === "0"){
            //       console.log('冗余代码')
            //       item[con] = null
            //     }
            //   })
            // })
            options.data.modelCapacityString= JSON.stringify(this.sameGroupDatas)
            axiosFetch(options).then(response => {
              if (response.data.result === 200) {
                this.resetEditCapacityForm();
                this.closeEditCapacityPanel();
                this.$alertSuccess('操作成功');
                this.sameGroupDatas =[];
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
        if(this.sessionFactory === '1'){
          this.processGroupSelect = []
        }
        this.sameGroupDatas=[];
        this.capacityEditOptionsData = {};
        this.customerNames ="";
        this.isCapacityAdd =false;
        this.factoryList=[];
        this.getGroupInfoFactoryId= "";
        this.$refs['capacityEditForm'].clearValidate();
        this.initEditOptions();
      },


      deleteData: function (val) {
        if(this.$store.state.limits.delete !== true){
          this.$alertWarning('暂无删除权限');
          return
        }
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
        // if (columnIndex === 1) {
        //   if (this.mergeProp.includes(column.property)) {
        //     const _row = this.mergeData[column.property][rowIndex]; //0,2,0,2,0,2
        //     const _col = _row > 0 ? 1 : 0;
        //     return {
        //       rowspan: _row,
        //       colspan: _col
        //     }
        //   }
        // }
        if (column.label === '状态' || column.label === '审核人' || column.label === '审核时间' || column.label === '审核说明' ||
            column.label === '机型' || column.label === '客户编号' || column.label === '客户名称' || column.label === '所属工厂' || column.label === '客户型号/料号' || column.label === '操作') {
          let iIndex = this.oneList.findIndex(e => { return e.index === rowIndex});
          if (iIndex !== -1) {
            return [this.oneList[iIndex].length, 1];
          } else {
            return [0, 0];
          }
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
    text-align: center;
    outline: none;
    background:rgba(0,0,0,0);
  }
  .copy-capacity-data /deep/ .el-form-item__error{
    margin-top: -10px;
    margin-left: -10px;
  }
  .copy-capacity-data .cell{
    height: 20px !important;
  }
  /*处理input type = number的上下箭头*/
  .copy-capacity-data /deep/ input::-webkit-outer-spin-button,
  .copy-capacity-data /deep/ input::-webkit-inner-spin-button {
    -webkit-appearance: none;
  }
  .copy-capacity-data /deep/ input[type="number"]{
    -moz-appearance: textfield;
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
  .capacity-edit-table{
    max-width: 1000px;
  }

</style>
