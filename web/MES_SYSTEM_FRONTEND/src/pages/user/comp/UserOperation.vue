<template>
  <div class="user-options form-row">
    <!--<div class="btn pl-1 pr-1" title="编辑" @click="editUser(row)">-->
    <!--<icon name="edit" scale="1.8"></icon>-->
    <!--</div>-->
    <el-dialog class="update-panel" v-if="isEditing" :visible.sync="isEditing" width="500px"
               :close-on-click-modal="false" :close-on-press-escape="false" :show-close="false" title="编辑信息">
      <div class="update-panel-container form-row">
        <div class="form-row">
          <div class="form-group">
            <label for="user-id" class="col-form-label">ID:</label>
            <el-input type="text" id="user-id" class="form-control" v-model="userData.id" disabled></el-input>
          </div>
          <div class="form-group">
            <label for="user-des" class="col-form-label">*用户名:</label>
            <el-input type="text" id="user-name" class="form-control" v-model.trim="userData.name"></el-input>
          </div>
          <div class="form-group">
            <label for="user-des" class="col-form-label">*用户描述:</label>
            <el-input type="text" id="user-des" class="form-control" v-model.trim="userData.userDes"></el-input>
          </div>
          <div class="form-group">
            <label for="user-pwd" class="col-form-label">*密码:</label>
            <el-input type="password" id="user-pwd" class="form-control" v-model.trim="userData.password"
                      autocomplete="off" placeholder="(无更改)"></el-input>
          </div>
          <div class="form-group">
            <label for="type-select" class="col-form-label">*用户类型:</label>
            <el-select id="type-select" class="custom-select" v-model="userData.webUserType">
              <el-option value="" disabled selected label="请选择"></el-option>
              <el-option v-for="item in $store.state.userTypeList.webTypeList" :value="item.TypeId" :key="item.TypeId"
                         :label="item.TypeDes"></el-option>
            </el-select>
          </div>
          <!--<div class="form-row col-6 pl-2 pr-2">-->
          <!--<label for="plan-select" class="col-form-label">测试计划:</label>-->
          <!--<select id="plan-select" class="custom-select" v-model="userData.userTestPlan">-->
          <!--<option value="" disabled>请选择</option>-->
          <!--<option value="2">2</option>-->
          <!--<option value="3">3</option>-->
          <!--</select>-->
          <!--</div>-->
          <div class="form-group" v-if="userData.webUserType === 1">
            <label class="col-form-label">权限设置:</label>
            <el-button @click="isEditingPermission = true" style="width: 200px;">查看详细权限</el-button>
          </div>
          <div class="form-group">
            <label for="active-select" class="col-form-label">*是否启用:</label>
            <el-select id="active-select" class="custom-select" v-model="userData.inService">
              <el-option label="请选择" value="" disabled></el-option>
              <el-option label="禁用" value="0"></el-option>
              <el-option label="启用" value="1"></el-option>
            </el-select>
          </div>
        </div>
        <div class="divider"></div>
        <div class="user-operation-btn-group">
          <div class="form-group-btn">
            <el-button type="info" @click="isEditing = !isEditing">取消</el-button>
          </div>
          <div class="form-group-btn">
            <el-button type="primary" @click="updateSubmit">提交</el-button>
          </div>
        </div>
      </div>
      <el-dialog title="编辑权限" v-if="isEditingPermission" :visible.sync="isEditingPermission"
                 width="500px" :close-on-click-modal="false" :close-on-press-escape="false" :show-close="false" append-to-body>


        <div class="checkbox-item-box">
          <div class="checkbox-item" v-for="(item, key) in permissionList">
            <el-checkbox :true-label="1" :false-label="0" v-model="tempPermission[key]">
              {{item.remark}}({{item.name}})
            </el-checkbox>
          </div>
        </div>

        <span slot="footer">
          <el-button type="primary" @click="isEditingPermission = !isEditingPermission">确认</el-button>
      </span>
      </el-dialog>
    </el-dialog>
  </div>
</template>

<script>
  import EditUser from './EditUser';
  import {userUpdateUrl} from "../../../config/globalUrl";
  import {axiosFetch} from "../../../utils/fetchData";
  import {errHandler} from "../../../utils/errorHandler";
  import eventBus from "../../../utils/eventBus";

  export default {
    name: "UserOperation",
    components: {
      EditUser
    },
    props: ['row'],
    data() {
      return {
        isEditing: false,
        isEditingPermission: false,
        userData: {
          id: '',
          name: '',
          userDes: '',
          password: '',
          webUserType: '',
          inService: ''
        },
        tempPermission: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0],
        permissionList: [
          {
            name: 'DataRelativeSheet',
            remark: 'AMS数据关联表'
          },
          {
            name: 'DataRelativeUnique',
            remark: '数据关联表(工厂)'
          },
          {
            name: 'DataRelativeUpdate',
            remark: '数据关联表(关联更新)'
          },
          {
            name: 'Gps_AutoTest_Result',
            remark: '组装功能表'
          },
          {
            name: 'Gps_AutoTest_Result2',
            remark: 'SMT功能表'
          },
          {
            name: 'Gps_AutoTest_Result3',
            remark: '老化后功能表'
          },
          {
            name: 'Gps_CartonBoxTwenty_Result',
            remark: '卡通箱表'
          },
          {
            name: 'Gps_CoupleTest_Result',
            remark: '耦合表'
          },
          {
            name: 'Gps_ManuPrintParam',
            remark: '机身彩盒打印记录表'
          },
          {
            name: 'Gps_TestResult',
            remark: '总表'
          },
          {
            name: 'NetMarkIMEI',
            remark: '网标与IMEI绑定表'
          },
          {
            name: 'Gps_ManuSimDataParam',
            remark: '种子物联网卡表'
          }
        ],
        isPending: false
      }
    },
    mounted() {
      eventBus.$off('editUser');
      eventBus.$on('editUser', (val) => {
        this.editUser(val)
      })
    },
    methods: {
      init: function () {

      },
      editUser: function (val) {
        this.isEditing = true;
        this.userData.password = "";
        this.userData.id = val.Id;
        this.userData.name = val.Name;
        this.userData.userDes = val.UserDes;
        this.userData.webUserType = val.WebUserType;
        this.userData.inService = val.InService ? "1" : "0";
        let tempArray = [];
        val.DeletePermission.split(',').forEach(item => {
          tempArray.push(item === '1' ? 1 : 0)
        });
        this.tempPermission = tempArray
      },
      updateSubmit: function () {
        if (!this.isPending) {
          this.isPending = true;

          //空值判断
          let mark = false;
          Object.keys(this.userData).forEach(item => {
            if (this.userData[item] === "" && item !== 'password') {
              this.$alertInfo("存在不能为空数据");
              mark = true;
            }
          });
          if (mark) {
            return;
          }

          let options = {
            url: userUpdateUrl,
            data: this.userData
          };
          if (this.userData.webUserType === 1) {
            options.data.deletePermission = this.tempPermission.toString();
          }
          if (!!!options.data.password) {
            delete options.data.password
          }

          //userType patch
          options.data.userType = "&00&10";

          axiosFetch(options).then(response => {
            this.isPending = false;
            if (response.data.result === 200) {
              this.$alertSuccess(response.data.data);
              this.isEditing = false;
              let tempUrl = this.$route.path;
              this.$router.replace('/_empty');
              this.$router.replace(tempUrl)
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.isPending = false;
            console.log(JSON.stringify(err));
            this.$alertDanger('请求超时，清刷新重试')
          })
        }

      }
    }
  }
</script>

<style scoped>

  .form-row {
    display: flex;
    flex-wrap: wrap;
    align-items: flex-end;
  }

  .form-group {
    width: 200px;
    margin: 0 10px;
  }

  .user-config /deep/ label {
    line-height: 40px;
  }

  .divider {
    border-top: 1px solid #eee;
    width: 100%;
    margin: 20px 0;
  }

  .user-operation-btn-group {
    display: flex;
    justify-content: space-around;
    width: 100%;
  }

  .form-group-btn {
    width: 120px;
    margin-right: 20px;
  }

  .form-group-btn .el-button {
    width: 100%;
    margin-top: 10px;
  }

  .fade-enter-active, .fade-leave-active {
    transition: opacity .5s;
  }

  .fade-enter, .fade-leave-to {
    opacity: 0;
  }

  .permission-btn {
    display: inline-block;
    width: 100%;
  }

  .permission-panel {
    word-break: normal;
    position: fixed;
    display: flex;
    align-items: center;
    justify-content: center;
    height: 100%;
    width: 100%;
    left: 0;
    top: 0;
    background: rgba(0, 0, 0, 0.1);
    z-index: 101;
  }

  .permission-panel-container {
    background: #ffffff;
    min-height: 220px;
    max-width: 600px;
    z-index: 102;
    border-radius: 10px;
    box-shadow: 3px 3px 20px 1px #bbb;
    padding: 30px 60px 10px 60px;
  }

  .permission-panel-container .permission-row {
    display: flex;
    align-items: center;
  }

  .permission-row label {
    display: block;
  }
  .checkbox-item-box {
    width: 100%;
    padding: 0 60px;
  }

  .checkbox-item {
    margin-bottom: 3px;
  }
</style>
