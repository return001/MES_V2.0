<template>
  <div class="user-options form-row">
    <el-dialog class="update-panel" v-if="isEditing" :visible.sync="isEditing" width="500px"
               :close-on-click-modal="false" :close-on-press-escape="false" :show-close="false" title="编辑信息">
      <div class="update-panel-container form-row">
        <div class="form-row">
          <div class="form-group">
            <label for="user-id" class="col-form-label">ID:</label>
            <el-input size="small" type="text" id="user-id" class="form-control" v-model="userData.id"
                      disabled></el-input>
          </div>
          <div class="form-group">
            <label for="user-des" class="col-form-label">*用户名:</label>
            <el-input size="small" type="text" id="user-name" class="form-control"
                      v-model.trim="userData.name"></el-input>
          </div>
          <div class="form-group">
            <label for="user-des" class="col-form-label">*用户描述:</label>
            <el-input size="small" type="text" id="user-des" class="form-control"
                      v-model.trim="userData.userDes"></el-input>
          </div>
          <div class="form-group">
            <label for="user-pwd" class="col-form-label">*密码:</label>
            <el-input size="small" type="password" id="user-pwd" class="form-control" v-model.trim="userData.password"
                      autocomplete="off" placeholder="(无更改)"></el-input>
          </div>
          <div class="form-group">
            <label for="type-select" class="col-form-label">*用户类型:</label>
            <el-select size="small" id="type-select" class="custom-select" v-model="userData.webUserType">
              <el-option value="" disabled selected label="请选择"></el-option>
              <el-option v-for="item in $store.state.userTypeList.webTypeList" :value="item.TypeId" :key="item.TypeId"
                         :label="item.TypeDes"></el-option>
            </el-select>
          </div>

          <!--当所选用户为工程管理员时提供删除表权限编辑-->
          <div class="form-group" v-if="userData.webUserType === 1">
            <label class="col-form-label">权限设置:</label>
            <el-button size="small" @click="isEditingPermission = true" style="width: 200px;">查看详细权限</el-button>
          </div>
          <div class="form-group">
            <label for="active-select" class="col-form-label">*是否启用:</label>
            <el-select size="small" id="active-select" class="custom-select" v-model="userData.inService">
              <el-option label="请选择" value="" disabled></el-option>
              <el-option label="禁用" value="0"></el-option>
              <el-option label="启用" value="1"></el-option>
            </el-select>
          </div>

          <div class="form-group">
            <label for="line-select" class="col-form-label">所属产线:</label>
            <el-select size="small" id="line-select" class="custom-select" v-model="userData.lineName">
              <el-option value="" selected label="未选择"></el-option>
              <el-option v-for="item in lineGroup" :value="item.lineName" :key="'line-item-'+item.id"
                         :label="item.lineName"></el-option>
            </el-select>
          </div>
          <div class="form-group">
            <label for="employee-select" class="col-form-label">是否正式员工:</label>
            <el-select size="small" id="employee-select" class="custom-select" v-model="userData.employeeType">
              <el-option value="" selected label="未选择"></el-option>
              <el-option value="正式工" label="正式工"></el-option>
              <el-option value="临时工" label="临时工"></el-option>
            </el-select>
          </div>
          <div class="form-group">
            <label for="process-select" class="col-form-label">主要操作工序:</label>
            <el-select size="small"
                       id="process-select"
                       class="custom-select"
                       v-model="userData.mainProcess"
                       @change="selectMainProcess">
              <el-option value="" selected label="未选择"></el-option>
              <el-option v-for="item in processGroup"
                         :value="item.processName"
                         :key="'process-item-'+item.id"
                         :label="item.processName"></el-option>
            </el-select>
          </div>
          <div class="form-group">
            <label for="proficiency-select" class="col-form-label">熟练程度:</label>
            <el-select size="small"
                       id="proficiency-select"
                       class="custom-select"
                       v-model="userData.proficiency"
                       :disabled="!userData.mainProcess">
              <el-option value="" selected label="未选择"></el-option>
              <el-option value="熟练" label="熟练"></el-option>
              <el-option value="一般" label="一般"></el-option>
              <el-option value="不熟" label="不熟"></el-option>
            </el-select>
          </div>
          <div class="form-group">
            <label for="other-skill">其它技能:</label>
            <el-select size="small"
                       id="other-skill"
                       class="custom-select"
                       v-model="userData.otherProcess"
                       multiple
                       :disabled="!userData.mainProcess">
              <el-option value="" selected label="未选择"></el-option>
              <el-option v-for="item in processSelectGroupSrcAfter"
                         :value="item.processName"
                         :key="'order-process-item-'+item.id"
                         :label="item.processName"></el-option>
            </el-select>
          </div>
        </div>
        <div class="divider"></div>
        <div class="user-operation-btn-group">
          <div class="form-group-btn">
            <el-button size="small" type="info" @click="isEditing = !isEditing">取消</el-button>
          </div>
          <div class="form-group-btn">
            <el-button size="small" type="primary" @click="updateSubmit">提交</el-button>
          </div>
        </div>
      </div>
      <el-dialog title="编辑权限" v-if="isEditingPermission" :visible.sync="isEditingPermission"
                 width="500px" :close-on-click-modal="false" :close-on-press-escape="false" :show-close="false"
                 append-to-body>


        <div class="checkbox-item-box">
          <div class="checkbox-item" v-for="(item, key) in permissionList">
            <el-checkbox size="small" :true-label="1" :false-label="0" v-model="tempPermission[key]">
              {{item.remark}}({{item.name}})
            </el-checkbox>
          </div>
        </div>

        <span slot="footer">
          <el-button size="small" type="primary" @click="isEditingPermission = !isEditingPermission">确认</el-button>
      </span>
      </el-dialog>
    </el-dialog>
  </div>
</template>

<script>
  import {userUpdateUrl} from "../../../config/globalUrl";
  import {axiosFetch} from "../../../utils/fetchData";
  import eventBus from "../../../utils/eventBus";

  export default {
    name: "UserOperation",
    props: ['lineGroup', 'processGroup'],
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
          inService: '',
          lineName: '',
          employeeType: '',
          mainProcess: '',
          proficiency: '',
          otherProcess: []
        },
        tempPermission: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
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
          },
          {
            name: 'GPSOQC_Result',
            remark: 'OQC测试结果表'
          }
        ],
        isPending: false,
        processSelectGroupSrcAfter: null,
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
        this.userData.lineName = val.LineName;
        this.userData.employeeType = val.EmployeeType;
        this.userData.mainProcess = val.MainProcess;
        this.userData.proficiency = val.Proficiency;
        this.userData.otherProcess = !val.OtherProcess ? [] : val.OtherProcess.split('@@');
        let tempArray = [];
        val.DeletePermission.split(',').forEach(item => {
          tempArray.push(item === '1' ? 1 : 0)
        });
        this.tempPermission = tempArray
      },
      updateSubmit: function () {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();

          //空值判断
          let mark = false;
          Object.keys(this.userData).forEach(item => {
            if (!this.userData[item] &&
              !(item === 'password'
                || item === 'lineName'
                || item === 'employeeType'
                || item === 'mainProcess'
                || item === 'proficiency'
                || item === 'otherProcess')) {
              this.$alertInfo("存在不能为空数据");
              mark = true;
            }
          });
          if (!!this.userData['mainProcess'] && !this.userData['proficiency']) {
            this.$alertInfo('工序熟练度不能为空');
            mark = true;
          }
          if (mark) {
            this.isPending = false;
            this.$closeLoading();
            return;
          }

          let options = {
            url: userUpdateUrl,
            data: this.userData
          };


          options.data.otherProcess = this.userData.otherProcess.join('@@');

          if (this.userData.webUserType === 1) {
            options.data.deletePermission = this.tempPermission.toString();
          }
          if (!!!options.data.password) {
            delete options.data.password
          }

          //userType patch
          options.data.userType = "&00&10";

          axiosFetch(options).then(response => {
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
            console.log(JSON.stringify(err));
            this.$alertDanger('请求超时，清刷新重试')
          }).finally(() => {
            this.isPending = false;
            this.$closeLoading();

          })
        }

      },
      selectMainProcess: function (e) {
        if (!!e) {
          this.userData.proficiency = '熟练';
        } else {
          this.userData.proficiency = '';
        }
        this.userData.otherProcess = [];
        this.processSelectGroupSrcAfter = this.processGroup.filter(item => {
          return item.processName !== e

        })
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
