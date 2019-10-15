<template>
  <div class="user-config">
    <loading v-if="$store.state.isLoading"/>
    <div class="form-area col">
      <div class="options-area">
        <div class="form-row">
          <div v-for="item in queryOptions">
            <component :opt="item" :is="item.type + '-comp'" :callback="thisFetch"></component>
          </div>
          <div class="form-group-btn">
            <el-button type="primary" @click="addUser">添加用户</el-button>
          </div>
          <div class="form-group-btn">
            <el-button type="primary" @click="thisFetch">查询</el-button>
          </div>
        </div>
      </div>
      <user-details/>
      <el-dialog class="add-panel" v-if="isAdding" title="新增用户" :visible.sync="isAdding" width="480px"
                 :close-on-click-modal="false" :close-on-press-escape="false" :show-close="false">
        <div class="form-row">
          <div class="form-group">
            <label for="user-name">*用户名:</label>
            <el-input type="text" id="user-name" class="form-control" v-model.trim="userData.name"></el-input>
          </div>
          <div class="form-group">
            <label for="user-pwd" class="col-form-label">*密码:</label>
            <el-input type="password" id="user-pwd" class="form-control" v-model.trim="userData.password"
                      autocomplete="off"></el-input>
          </div>
          <div class="form-group">
            <label for="user-des" class="col-form-label">*用户描述:</label>
            <el-input type="text" id="user-des" class="form-control" v-model.trim="userData.userDes"></el-input>
          </div>
          <div class="form-group">
            <label for="access-select" class="col-form-label">*用户类型:</label>
            <el-select id="access-select" class="custom-select" v-model="userData.webUserType">
              <el-option value="" disabled selected label="请选择"></el-option>
              <el-option v-for="item in $store.state.userTypeList.webTypeList" :value="item.TypeId" :key="item.TypeId"
                         :label="item.TypeDes"></el-option>
            </el-select>
          </div>
          <div class="form-group" v-if="userData.webUserType === 1">
            <label class="col-form-label">权限设置:</label>
            <el-button @click="isAddingPermission = true" style="width: 200px;">查看详细权限</el-button>
          </div>
          <div class="divider"></div>
          <div class="user-config-btn-group">
            <div class="form-group-btn">
              <el-button type="info" @click="isAdding = !isAdding">取消</el-button>
            </div>
            <div class="form-group-btn">
              <el-button type="primary" @click="addSubmit">提交</el-button>
            </div>
          </div>
        </div>
        <el-dialog title="编辑权限" :visible.sync="isAddingPermission" v-if="isAddingPermission"
                   width="500px" :close-on-click-modal="false" :close-on-press-escape="false" :show-close="false" append-to-body>

          <div class="checkbox-item-box">
            <div class="checkbox-item" v-for="(item, key) in permissionList">
              <el-checkbox :true-label="1" :false-label="0" v-model="tempPermission[key]">
                {{item.remark}}({{item.name}})
              </el-checkbox>
            </div>
          </div>
          <span slot="footer">
            <el-button type="primary" @click="isAddingPermission = !isAddingPermission">确认</el-button>
          </span>
        </el-dialog>
      </el-dialog>

    </div>
  </div>
</template>

<script>
  import Loading from '../../components/Loading'
  import UserDetails from './comp/UserDetails'
  import {axiosFetch} from "../../utils/fetchData";
  import {userAddUrl, getUserTypeUrl} from "../../config/globalUrl";
  import {errHandler} from "../../utils/errorHandler";
  import eventBus from "../../utils/eventBus";
  import store from "../../store";
  import {alertInfo} from "../../utils/modal";

  export default {
    name: "UserConfig",
    data() {
      return {
        isAdding: false,
        isAddingPermission: false,
        userData: {
          name: '',
          userDes: '',
          password: '',
          webUserType: '',
          inService: true
        },
        isPending: false,
        queryOptions: [
          {
            id: 'Name',
            name: '工号',
            model: '',
            type: 'text'
          },
          {
            id: 'UserDes',
            name: '描述',
            model: '',
            type: 'text'
          },
          {
            id: 'InService',
            name: '是否启用',
            model: '',
            type: 'select',
            list: [
              {
                value: false,
                string: '禁用状态'
              },
              {
                value: true,
                string: '启用状态'
              }
            ]
          }
        ],
        tempPermission: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
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
        ]
      }
    },
    components: {
      'text-comp': {
        props: ['opt', 'callback'],
        template: '<div class="form-group">\n' +
        '           <label :for="opt.id">{{opt.name}}：</label>\n' +
        '           <el-input type="text" class="form-control" :id="opt.id" v-model="opt.model" @keyup.enter="callback" autocomplete="off" clearable></el-input>\n' +
        '          </div>'
      },
      'select-comp': {
        props: ['opt'],
        template: '<div >\n' +
        '      <div class="form-group ">\n' +
        '        <label :for="opt.id">{{opt.name}}：</label>\n' +
        '        <el-select :id="opt.id" v-model="opt.model" class="custom-select" style="display: block; margin-right: 20px">\n' +
        '          <el-option value="" disabled label="请选择"></el-option>\n' +
        '          <el-option :value="item.value"  v-for="item in opt.list" :key="item.value" :label="item.string"></el-option>\n' +
        '        </el-select>\n' +
        '      </div>\n' +
        '    </div>'
      },
      UserDetails,
      Loading
    },
    watch: {
      $route: function (route) {
        Object.assign(this.$data, this.$options.data())
      },
    },
    mounted() {
      this.getUserType();
    },
    methods: {
      initForm: function () {
        this.queryOptions.map(item => {
          item.model = "";
        })
      },

      getUserType: function () {
        return new Promise(() => {
          this.isPending = true;
          axiosFetch({
            url: getUserTypeUrl
          }).then(async response => {
            if (response.data.result === 200) {
              store.commit('setUserTypeList', response.data.data)
            } else {
              alertInfo('获取账户权限列表失败，请刷新页面')
            }
          }).catch(err => {
            alertInfo('获取账户权限列表失败，请刷新页面')
          }).finally(() => {
            this.isPending = false;
          })
        })
      },


      addUser: function () {
        Object.assign(this.userData, this.$options.data().userData);
        this.tempPermission = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        this.isAdding = true;
      },
      addSubmit: function () {
        if (!this.isPending) {
          this.isPending = true;
          let user = this.userData;
          if (!(user.name === "" || user.password === "" || user.webUserType === "" || user.userDes === "")) {
            let optData = JSON.parse(JSON.stringify(user));
            let options = {
              url: userAddUrl,
              data: optData
            };
            if (user.webUserType === 1) {
              options.data.deletePermission = this.tempPermission.toString();
            }

            //userType patch
            options.data.userType = "&00&10";


            axiosFetch(options).then(response => {
              this.isPending = false;
              if (response.data.result === 200) {
                this.$alertSuccess(response.data.data);
                this.isAdding = false;
                let tempUrl = this.$route.path;
                this.$router.replace('/_empty');
                this.$router.replace(tempUrl);
              } else {
                this.$alertWarning(response.data.data);
              }
            })
              .catch(err => {
                this.isPending = false;
                console.log(JSON.stringify(err));
                this.$alertDanger('请求超时，清刷新重试')
              })
          } else {
            this.$alertWarning("内容不能为空");
            this.isPending = false;
            return;
          }
        }
      },
      createQueryString: function () {
        return new Promise((resolve, reject) => {
          this.queryString = "";
          this.copyQueryOptions = this.queryOptions.filter((item) => {
            if (!(item.model === "")) {
              return true;
            }
          });

          this.copyQueryOptions.map((item, index) => {
            if (item.type === 'text') {
              if (_.trim(item.model) !== "") {
                if (index === 0) {
                  this.queryString += (item.id + "#like#" + _.trim(item.model))
                } else {
                  this.queryString += ("#&#" + item.id + "#like#" + _.trim(item.model))
                }

              } else {
                this.$closeLoading()
              }
            } else if (item.type === 'select') {
              if (_.trim(item.model) !== "") {
                if (index === 0) {
                  this.queryString += (item.id + "#=#" + _.trim(item.model))
                } else {
                  this.queryString += ("#&#" + item.id + "#=#" + _.trim(item.model))
                }

              } else {
                this.$closeLoading()
              }
            }

          });
          resolve()
        });
      },
      thisFetch: function () {
        this.createQueryString().then(() => {
          eventBus.$emit('userQueryData', this.queryString)
        });
      },
    }
  }
</script>

<style scoped>
  .user-config {
    position: relative;
    height: 100%;
    width: calc(100% - 100px);
    padding: 20px 20px 20px 80px;
  }

  .form-area {
    position: relative;
    width: 100%;
    /*padding: 40px 30px 40px 90px;*/
  }

  .options-area {
    background: #fff;
    border: 1px solid #eeeeee;
    border-radius: 8px;
    padding: 10px;
    margin-bottom: 10px;
  }

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

  .user-config-btn-group {
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

  .add-panel {
  }

  .add-panel-container {
    background: #ffffff;
    min-height: 220px;
    max-width: 600px;
    z-index: 102;
    border-radius: 10px;
    box-shadow: 3px 3px 20px 1px #bbb;
    padding: 30px 60px 10px 60px;
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
