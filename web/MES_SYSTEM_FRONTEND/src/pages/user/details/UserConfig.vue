<template>
  <div id="user-setting">
    <div class="query-comp">
<!--      <rz-query-bar-->
<!--          :list="queryConfig"-->
<!--          :button-group="buttonGroup"-->
<!--          :data.sync="queryCompData"/>-->


      <div class="query-comp-container" v-for="(item, index) in queryConfig" :key="index">
        <!--纯文本框-->
        <div class="query-comp-text" v-if="item.type === 'text'">
          <label :for="item.prop + index">{{item.label}}:</label>
          <el-input v-model="queryCompData[item.prop]" :id="item.prop + index"
                    :placeholder="'请填写' + item.label" size="small"
                    autocomplete="off"
                    clearable></el-input>
        </div>
        <!--选择器-->
        <div class="query-comp-select" v-else-if="item.type === 'select'">
          <label :for="item.prop + index">{{item.label}}:</label>
          <el-select v-model="queryCompData[item.prop]" :id="item.prop + index"
                     :placeholder="'请选择' + item.label" size="small"
                     autocomplete="off"
                     @change="linkageSelect($event,item.prop)"
                     clearable
                     >
            <el-option v-for="listItem in item.selectList"
                       :key="listItem.key"
                       :value="listItem.key"
                       :label="listItem.label"></el-option>
          </el-select>
        </div>
<!--        <div class="query-comp-select" v-else-if="item.type === 'select' && item.prop === 'company'">-->
<!--          <label :for="item.prop + index">{{item.label}}:</label>-->
<!--          <el-select v-model="queryCompData[item.prop]" :id="item.prop + index"-->
<!--                     :placeholder="'请选择' + item.label" size="small"-->
<!--                     autocomplete="off"-->
<!--                     @change="linkageSelect"-->
<!--                     >-->
<!--            <el-option v-for="listItem in item.selectList"-->
<!--                       :key="listItem.key"-->
<!--                       :value="listItem.key"-->
<!--                       :label="listItem.label"></el-option>-->
<!--          </el-select>-->
<!--        </div>-->
      </div>
      <div class="query-comp-container">
        <el-button type="info" @click="queryCompData = {}" size="small">重置条件</el-button>
      </div>
      <div class="query-comp-container">
        <el-button type="primary" @click="queryData" size="small">查询</el-button>
      </div>
      <div class="query-comp-container">
        <el-button type="primary" @click="addData" size="small">新增</el-button>
      </div>
    </div>





    <div class="content-comp">
      <el-table
          :data="tableData"
          max-height="560"
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
            fixed="left"
            width="60">
        </el-table-column>

        <el-table-column
            label="操作"
            width="100"
            fixed="right"
        >
          <template slot-scope="scope">
            <el-tooltip content="权限" placement="top">
              <el-button type="text" @click="editSpecialAuthority(scope.row)" icon="el-icon-warning-outline"></el-button>
            </el-tooltip>
            <el-tooltip content="编辑" placement="top">
              <el-button type="text" @click="editData(scope.row)" icon="el-icon-edit-outline"></el-button>
            </el-tooltip>
            <el-tooltip content="删除" placement="top">
              <el-button type="text" @click="deleteData(scope.row)" icon="el-icon-delete"></el-button>
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
          @size-change="queryData"
          class="page-pagination">
      </el-pagination>
    </div>

    <el-dialog
        :title="editPanelTitle"
        :visible.sync="isEditing"
        :close-on-click-modal="false"
        :close-on-press-escape="false"
        @close="resetEditForm"
        width="500px">
      <el-form
          ref="userFormComp"
          :model="formData"
          class="user-edit-form"
          label-position="top"
          @submit.native.prevent
          :rules="formRules">
        <el-form-item
            size="small"
            label="用户名"
            prop="name"
        >
          <el-input
              type="text"
              placeholder="请填写登录时用户名"
              clearable
              autocomplete="off"
              v-model="formData.name"></el-input>
        </el-form-item>
        <el-form-item
            size="small"
            label="描述"
            prop="userDes"
        >
          <el-input
              type="text"
              placeholder="请填写用户描述"
              clearable
              autocomplete="off"
              v-model="formData.userDes"></el-input>
        </el-form-item>
        <el-form-item
            size="small"
            label="密码"
            prop="password"
        >
          <el-input
              type="text"
              placeholder="请填写密码"
              clearable
              autocomplete="off"
              v-model="formData.password"></el-input>
        </el-form-item>
        <el-form-item
            size="small"
            label="绑定角色"
            prop="role"
        >
          <el-cascader
              v-model="selectedAuthority"
              :options="authoritiesList"
              @change="setFormRole"
          ></el-cascader>
        </el-form-item>
        <el-form-item
            size="small"
            label="所属产线"
            prop="lineName"
        >
          <el-select
              v-model="formData.lineName"
              placeholder="请选择产线"
              size="small"
              clearable>
            <el-option v-for="listItem in srcLineList"
                       :key="'line-item-'+listItem.id"
                       :value="listItem.lineName"
                       :label="listItem.lineName"></el-option>
          </el-select>
        </el-form-item>
        <el-form-item
            size="small"
            label="主要工序"
            prop="mainProcess"
        >
          <el-select
              size="small"
              id="process-select"
              class="custom-select"
              v-model="formData.mainProcess"
              @change="selectMainProcess"
              clearable>
            <el-option value="" selected label="未选择"></el-option>
            <el-option v-for="item in srcProcessList"
                       :value="item.processName"
                       :key="'process-item-'+item.id"
                       :label="item.processName"></el-option>
          </el-select>
        </el-form-item>
        <el-form-item
            size="small"
            label="熟练度"
            prop="proficiency"
        >
          <el-select
              size="small"
              id="proficiency-select"
              class="custom-select"
              v-model="formData.proficiency"
              :disabled="!formData.mainProcess"
              clearable
          >
            <el-option value="" selected label="未选择"></el-option>
            <el-option value="熟练" label="熟练"></el-option>
            <el-option value="一般" label="一般"></el-option>
            <el-option value="不熟" label="不熟"></el-option>
          </el-select>
        </el-form-item>
<!--        <el-form-item-->
<!--            size="small"-->
<!--            label="其它技能"-->
<!--            prop="otherProcess"-->
<!--        >-->
<!--          <el-select-->
<!--              size="small"-->
<!--              id="other-skill"-->
<!--              class="custom-select"-->
<!--              v-model="formData.otherProcess"-->
<!--              multiple-->
<!--              clearable-->
<!--              :disabled="!formData.mainProcess">-->
<!--&lt;!&ndash;            <el-option value="" selected label="未选择"></el-option>&ndash;&gt;-->
<!--            <el-option v-for="item in processSelectGroupSrcAfter"-->
<!--                       :value="item.id"-->
<!--                       :key="'order-process-item-'+item.id"-->
<!--                       :label="item.processName"></el-option>-->
<!--          </el-select>-->
<!--        </el-form-item>-->
        <el-form-item
          size="small"
          label="是否启用"
          prop="proficiency"
        >
          <el-select
            size="small"
            id="proficiency-select"
            class="custom-select"
            placeholder="选择是否启用"
            v-model="formData.inService"
            clearable
          >
            <el-option :value=true label="是"></el-option>
            <el-option :value=false label="否"></el-option>
          </el-select>
        </el-form-item>
      </el-form>
      <span slot="footer" class="dialog-footer">
         <el-button size="small" @click="closeEditPanel" type="info">取消</el-button>
         <el-button size="small" @click="submitFormEdit" type="primary">保存</el-button>
       </span>
    </el-dialog>
    <el-dialog
        title="权限"
        :visible.sync="isEditingSpecialAuthority"
        :close-on-click-modal="false"
        :close-on-press-escape="false"
        @close="resetEditAuthorityForm"
        width="500px">
      <div class="edit-authority-btn">
        <el-button size="small" type="plain" @click="isEditingAuthority = true">编辑权限</el-button>
      </div>
      <span slot="footer" class="dialog-footer">
         <el-button size="small" @click="closeEditAuthorityPanel" type="info">取消</el-button>
         <el-button size="small" @click="submitAuthorityEdit" type="primary" :disabled="editingAuthorities.length === 0">保存</el-button>
       </span>

      <edit-char-comp
          v-if="isEditingAuthority"
          :is-editing-authority.sync="isEditingAuthority"
          :is-add-or-edit="1"
          :editing-authorities="editingAuthorities"
          @update:authoritiesJSON="authoritiesJSON = $event"
      />
    </el-dialog>
  </div>
</template>

<script>
  import {axiosFetch} from "../../../utils/fetchData";
  import {
    userAddUrl,
    getUserTypeUrl,
    planLineSelectUrl,
    planProcessSelectUrl,
    userUpdateUrl,
    userQueryUrl,
    eSopFactorySelectUrl, selectDeptUrl, selectCharUrl, getUserAuthorities, setTypicalChar,userDeleteUrl
  } from "../../../config/globalUrl";
  import {UserQueryConfig, UserTableColumns, UserFormRules} from "../../../config/UserConfig";
  import EditCharComp from "./comp/EditCharComp";
  import {MessageBox} from "element-ui";

  export default {
    name: "UserConfig",
    components: {EditCharComp},
    inject: ['reload'],
    data() {
      return {
        thisQueryOptions:[],
        queryConfig: UserQueryConfig,
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
          {
            label: '新增',
            size: 'small',
            type: 'primary',
            callback() {
            }
          },
        ],
        queryCompData: {},
        tableColumns: UserTableColumns,
        tableData: [],

        paginationOptions: {
          currentPage: 1,
          pageSize: 20,
          total: 0
        },

        editPanelTitle: '',
        isEditing: false,
        editType: 0, //0: add, 1: edit

        formRules: {
          name: [
            {required: true, message: '请输入用户名(工号)', trigger: 'blur'},
          ],
          userDes: [
            {required: true, message: '请输入用户描述', trigger: 'blur'},
          ],
          password: [
            {required: false, message: '请输入密码', trigger: 'blur'},
          ],
        },
        formData: {
          inService:true,
        },

        /*链接重定义*/
        formAddUrl: userAddUrl,
        formUpdateUrl: userUpdateUrl,

        isPending: false,

        companyList: [],
        formSelectedCompanyId: null,

        srcDeptList: [],
        deptList: [],

        srcAuthoritiesList: [],
        authoritiesList: [],
        selectedAuthority: [],

        srcProcessList: [],
        srcOtherProcessList: [],
        processSelectGroupSrcAfter: [],
        processList: [],

        srcLineList: [],
        lineList: [],

        isEditingSpecialAuthority: false,
        isEditingAuthority: false,
        editingUserId: null,
        editingAuthorities: [],
        authoritiesJSON: [],
      }
    },
    async created() {
      this.$openLoading();
      await this.preloadAll().then(dataGroup => {
        this.companyList = dataGroup[0];
        this.srcDeptList = dataGroup[1];
        this.srcAuthoritiesList = dataGroup[2];
        this.srcProcessList = dataGroup[3];
        this.srcLineList = dataGroup[4];

        /*初始化查询栏*/
        this.queryConfig[0].selectList = this.companyList.map(item => {
          return {
            key: item.id,
            label: item.abbreviation
          }
        });
        this.queryConfig[1].selectList = []
        // this.queryConfig[1].selectList = this.srcDeptList.map(item => {
        //   return {
        //     key: item.id,
        //     label: item.name
        //   }
        // });
        this.queryConfig[5].selectList = this.srcProcessList.map(item => {
          return {
            key: item.processName,
            label: item.processName
          }
        });
        this.queryConfig[7].selectList = this.srcLineList.map(item => {
          return {
            key: item.lineName,
            label: item.lineName
          }
        });

        /*初始化表单填写选项*/
        this.generateRoleOptions();
      });
      this.$closeLoading();
    },
    watch: {},
    mounted() {
      this.buttonGroup[0].callback = () => {
        this.queryCompData = {};
      };
      this.buttonGroup[1].callback = this.queryData;
      this.buttonGroup[2].callback = this.addData;
      this.queryData();
    },
    methods: {

      queryData() {
        this.paginationOptions.currentPage = 1;
        this.paginationOptions.total = 0;
        this.fetchData();
      },

      fetchData() {
        this.$openLoading();
        this.isPending = true;
        let options = {
          url: userQueryUrl,
          data: {
            pageNo: this.paginationOptions.currentPage,
            pageSize: this.paginationOptions.pageSize
          }
        };
        Object.keys(this.queryCompData).forEach(key => {
          if (!!this.queryCompData[key]) {
            options.data[key] = this.queryCompData[key]
          }
        });
        axiosFetch(options).then(response => {
          if (response.data.result === 200) {
            this.tableData = response.data.data.list;
          } else {
            this.$alertWarning(response.data.data)
          }
          this.paginationOptions.currentPage = response.data.data.pageNumber;
          this.paginationOptions.total = response.data.data.totalRow;
        }).catch((err) => {
          console.log(err);
          this.$alertDanger('未知错误');
        }).finally(() => {
          this.isPending = false;
          this.$closeLoading();
        })
      },

      linkageSelect(val,prop){
        console.log(prop)
        if(prop === 'company'){
          delete this.queryCompData.department
          delete this.queryCompData.roleName
          this.queryConfig[1].selectList = []
          this.queryConfig[2].selectList = []
          this.srcDeptList.forEach(item=>{
            if(item.company === val){
              this.queryConfig[1].selectList.push({
                key: item.id,
                label: item.name
              })
            }
          })
        }else if(prop === 'department'){
          delete this.queryCompData.roleName
          this.queryConfig[2].selectList = []
          this.srcAuthoritiesList.forEach(item=>{
            if(item.department === val){
              this.queryConfig[2].selectList.push({
                key: item.name,
                label: item.name
              })
            }
          })
        }else if(prop === 'mainProcess'){
          delete this.queryCompData.otherProcess
          this.queryConfig[6].selectList= []
          this.srcProcessList.forEach(item=>{
            if(val){
              if (item.processName !== val){
                this.queryConfig[6].selectList.push({
                  key: item.processName,
                  label: item.processName
                })
              }
            }
          })
        }
      },

      addData() {
        this.editType = 0;
        this.editPanelTitle = '新增';
        this.formRules.password[0].required = true;
        this.isEditing = true;
        this.queryData();
      },
      editData(row) {
        this.editType = 1;
        this.editPanelTitle = '编辑';
        this.formRules.password[0].required = false;
        this.formData = {
          'id': row.id,
          'inService': row.inService,
          'lineName': row.lineName,
          'loginTime': row.loginTime,
          'mainProcess': row.mainProcess,
          'name': row.name,
          'otherProcess': row.otherProcess,
          'proficiency': row.proficiency,
          'role': row.role,
          'userDes': row.userDes,
        };
        this.selectedAuthority = [row.company, row.department, row.role];
        this.isEditing = true;
      },

      //删除
      deleteData(row) {
        MessageBox.confirm('将删除该项目，是否继续?', '提示', {
          confirmButtonText: '确认',
          cancelButtonText: '取消',
          type: 'warning'
        }).then(() => {
          this.$openLoading();
          axiosFetch({
            url: userDeleteUrl,
            data: {
              id: row.id
            }
          }).then(response => {
            if (response.data.result === 200) {
              this.$alertSuccess('删除成功');
              this.reload();
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            console.log(err)
            this.$alertDanger("未知错误")
          }).finally(() => {
            this.$closeLoading();
          })
        }).catch(() => {

        })
      },

      preloadAll() {
        return Promise.all([this.getFactoryList(), this.getDepartmentList(), this.getCharAuthority(), this.getProcess(), this.getLine()])
      },

      getFactoryList() {
        return new Promise(resolve => {
          axiosFetch({
            url: eSopFactorySelectUrl,
            data: {
              pageNo: 1,
              pageSize: 2147483647
            }
          }).then(response => {
            if (response.data.result === 200) {
              resolve(response.data.data.list);
            } else {
              this.$alertWarning(response.data.data);
            }
          }).catch(() => {
            this.$alertDanger(`获取工厂列表失败，请刷新重试`)
          })
        })
      },

      getDepartmentList() {
        return new Promise(resolve => {
          axiosFetch({
            url: selectDeptUrl,
            data: {}
          }).then(response => {
            if (response.data.result === 200) {
              resolve(response.data.data);
            } else {
              this.$alertWarning(response.data.data);
            }
          }).catch(() => {
            this.$alertDanger(`获取部门列表失败，请刷新重试`)
          })
        })
      },

      getCharAuthority() {
        return new Promise(resolve => {
          axiosFetch({
            url: selectCharUrl,
            data: {}
          }).then(response => {
            if (response.data.result === 200) {
              resolve(response.data.data);
            } else {
              this.$alertWarning(response.data.data);
            }
          }).catch(() => {
            this.$alertDanger(`获取部门列表失败，请刷新重试`)
          })
        })
      },

      getProcess: function () {
        return new Promise(resolve => {
          axiosFetch({
            url: planProcessSelectUrl,
            data: {
              pageNo: 1,
              pageSize: 2147483647
            }
          }).then(response => {
            if (response.data.result === 200) {
              resolve(response.data.data.list);
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.$alertDanger('获取工序信息失败，请刷新重试');
          })
        });
      },

      getLine: function () {
        return new Promise(resolve => {
          axiosFetch({
            url: planLineSelectUrl,
            data: {
              pageNo: 1,
              pageSize: 2147483647
            }
          }).then(response => {
            if (response.data.result === 200) {
              resolve(response.data.data.list);
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.$alertDanger('获取产线信息失败，请刷新重试');
          })
        });
      },

      resetEditForm() {
        this.formData = {};
        this.selectedAuthority = [];
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
          this.$refs['userFormComp'].validate(isValid => {
            if (isValid) {
              this.isPending = true;
              this.$openLoading();
              let options = {
                url: url,
                data: this.formData
              };
              axiosFetch(options).then(response => {
                if (response.data.result === 200) {
                  this.$alertSuccess('操作成功');
                  this.reload();
                } else {
                  this.$alertWarning(response.data.data)
                }
              }).catch((err) => {
                console.log(err);
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
      selectMainProcess: function (e) {
        if (!!e) {
          this.formData.proficiency = '熟练';
        } else {
          this.formData.proficiency = '';
        }
        this.formData.otherProcess = [];
        this.processSelectGroupSrcAfter = this.srcProcessList.filter(item => {
          return item.processName !== e

        })
      },

      generateRoleOptions() {
        let tempGroup = [];
        let getKeyIndex = (array, value) => {
          for (let i = 0; i < array.length; i++) {
            if (array[i]['value'] === value && array[i]['children'] !== undefined) {
              return i
            }
          }
          return undefined;
        };
        this.srcAuthoritiesList.forEach(item => {
          if (item.company) {
            if (getKeyIndex(tempGroup, item.company) === undefined) {
              tempGroup.push({
                label: item.companyName,
                value: item.company,
                children: [{
                  label: item.departmentName,
                  value: item.department,
                  children: [{
                    label: item.name,
                    value: item.id
                  }]
                }]
              })
            } else {
              let companyIdx = getKeyIndex(tempGroup, item.company);
              if (getKeyIndex(tempGroup[companyIdx].children, item.department) === undefined) {
                tempGroup[companyIdx].children.push({
                  label: item.departmentName,
                  value: item.department,
                  children: [{
                    label: item.name,
                    value: item.id
                  }]
                });
              } else {
                let deptIdx = getKeyIndex(tempGroup[companyIdx].children, item.department);
                tempGroup[companyIdx].children[deptIdx].children.push({
                  label: item.name,
                  value: item.id
                })
              }
            }

          } else {
            tempGroup.push({
              label: item.name,
              value: item.id
            })
          }
        });
        this.authoritiesList = tempGroup;
      },

      setFormRole(e) {
        this.formData.role = e[e.length - 1]
      },

      editSpecialAuthority(row) {
        axiosFetch({
          url: getUserAuthorities,
          data: {
            user: row.id
          }
        }).then(response => {
          if (response.data.result === 200) {
            this.editingUserId = row.id;
            this.editingAuthorities = response.data.data['webAuthorities'];
            this.isEditingSpecialAuthority = true;
          } else {
            this.$alertWarning(response.data.data)
          }
        }).catch((err) => {
          console.log(err);
          this.$alertDanger('未知错误');
        }).finally(() => {
          this.isPending = false;
          this.$closeLoading();
        })
      },
      closeEditAuthorityPanel() {
        this.isEditingSpecialAuthority = false;
      },
      submitAuthorityEdit() {
        this.isPending = true;
        this.$openLoading();
        let options = {
          url: setTypicalChar,
          data: {
            user: this.editingUserId
          }
        };
        options.data.authority = JSON.stringify(this.authoritiesJSON);
        axiosFetch(options).then(response => {
          if (response.data.result === 200) {
            this.$alertSuccess('操作成功');
            this.reload();
          } else {
            this.$alertWarning(response.data.data)
          }
        }).catch((err) => {
          console.log(err);
          this.$alertDanger('未知错误');
        }).finally(() => {
          this.isPending = false;
          this.$closeLoading();
        })
      },
      resetEditAuthorityForm() {
        this.editingUserId = null;
        this.editingAuthorities = [];
        this.authoritiesJSON = [];
      }
    }
  }
</script>

<style scoped>
  #user-setting {
    position: relative;
    width: 100%;
    min-height: 600px;
    height: 100%;
  }

  #user-setting /deep/ .el-button i {
    font-size: 14px;
    font-weight: bold;
  }

  .content-comp {
    box-sizing: border-box;
    padding: 10px 20px;
    background: #fff;
    border: 1px solid #ededed;
    border-radius: 5px;
    margin-top: 10px;
  }
  .query-comp {
    position: relative;
    background-color: #ffffff;
    border: 1px solid #eeeeee;
    border-radius: 5px;
    padding: 10px 20px;
    display: flex;
    flex-wrap: wrap;
    overflow: hidden;
  }
  .query-comp label {
    display: block;
    line-height: 24px;
    font-size: 14px;
  }
  .query-comp-container {
    margin: 0 16px 10px 0;
  }
  .query-comp-container .el-button {
    margin-top: 24px;
    min-width: 80px;
  }

  .page-pagination {
    display: flex;
    flex-wrap: wrap;
    margin: 20px 0;
    padding: 0 20px;
  }

  .user-edit-form {
    display: flex;
    flex-wrap: wrap;
    justify-content: space-around;
  }

  .user-edit-form .el-form-item {
    width: 220px;
  }

</style>
