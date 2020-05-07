<template>
  <div class="user-config">
    <div class="form-area col">
      <div class="options-area">
        <div class="form-row">
          <!--根据配置生成查询栏-->
          <div v-for="item in queryOptions">
            <component :opt="item" :is="item.type + '-comp'"></component>
          </div>
          <div class="form-group-btn">
            <el-button size="small" type="info" @click="initForm">重置条件</el-button>
          </div>
          <div class="form-group-btn">
            <el-button size="small" type="primary" @click="addUser">添加用户</el-button>
          </div>
          <div class="form-group-btn">
            <el-button size="small" type="primary" @click="thisFetch">查询</el-button>
          </div>
        </div>
      </div>
      <user-details :lineGroup="lineSelectGroupSrc" :processGroup="processSelectGroupSrc"/>

      <!--新增用户面板-->
      <el-dialog class="add-panel" v-if="isAdding" title="新增用户" :visible.sync="isAdding" width="480px"
                 :close-on-click-modal="false" :close-on-press-escape="false" :show-close="false">
        <div class="form-row">
          <div class="form-group">
            <label for="user-name">*用户名:</label>
            <el-input size="small" type="text" id="user-name" class="form-control"
                      v-model.trim="userData.name"></el-input>
          </div>
          <div class="form-group">
            <label for="user-pwd" class="col-form-label">*密码:</label>
            <el-input size="small" type="password" id="user-pwd" class="form-control" v-model.trim="userData.password"
                      autocomplete="off"></el-input>
          </div>
          <div class="form-group">
            <label for="user-des" class="col-form-label">*用户描述:</label>
            <el-input size="small" type="text" id="user-des" class="form-control"
                      v-model.trim="userData.userDes"></el-input>
          </div>
          <div class="form-group">
            <label for="access-select" class="col-form-label">*用户类型:</label>
            <el-select size="small" id="access-select" class="custom-select" v-model="userData.webUserType">
              <el-option value="" disabled selected label="请选择"></el-option>
              <el-option v-for="item in $store.state.userTypeList.webTypeList" :value="item.TypeId" :key="item.TypeId"
                         :label="item.TypeDes"></el-option>
            </el-select>
          </div>
          <div class="form-group" v-if="userData.webUserType === 1">
            <label class="col-form-label">Web权限设置:</label>
            <el-button size="small" @click="isAddingPermission = true" style="width: 200px;">查看详细权限</el-button>
          </div>
          <div class="form-group">
            <label for="line-select" class="col-form-label">所属产线:</label>
            <el-select size="small" id="line-select" class="custom-select" v-model="userData.lineName">
              <el-option value="" selected label="未选择"></el-option>
              <el-option v-for="item in lineSelectGroupSrc" :value="item.lineName" :key="'line-item-'+item.id"
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
            <el-select size="small" id="process-select" class="custom-select" v-model="userData.mainProcess" @change="selectMainProcess">
              <el-option value="" selected label="未选择"></el-option>
              <el-option v-for="item in processSelectGroupSrc"
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
<!--            <el-input size="small" type="text" id="other-skill" class="form-control"-->
<!--                      v-model.trim="userData.otherProcess"></el-input>-->
            <el-select size="small" id="other-skill"
                       class="custom-select"
                       v-model="otherProcess"
                       multiple
                       :disabled="!userData.mainProcess">
              <el-option v-for="item in processSelectGroupSrcAfter"
                         :value="item.processName"
                         :key="'order-process-item-'+item.id"
                         :label="item.processName"></el-option>
            </el-select>

          </div>
          <div class="divider"></div>
          <div class="user-config-btn-group">
            <div class="form-group-btn">
              <el-button size="small" type="info" @click="isAdding = !isAdding">取消</el-button>
            </div>
            <div class="form-group-btn">
              <el-button size="small" type="primary" @click="addSubmit">提交</el-button>
            </div>
          </div>
        </div>
        <!--新增用户面板-权限子面板-->
        <el-dialog title="编辑权限" :visible.sync="isAddingPermission" v-if="isAddingPermission"
                   width="500px" :close-on-click-modal="false" :close-on-press-escape="false" :show-close="false"
                   append-to-body>

          <div class="checkbox-item-box">
            <!--根据配置生成权限checkbox列表-->
            <div class="checkbox-item" v-for="(item, key) in permissionList">
              <el-checkbox size="small" :true-label="1" :false-label="0" v-model="tempPermission[key]">
                {{item.remark}}({{item.name}})
              </el-checkbox>
            </div>
          </div>
          <span slot="footer">
            <el-button size="small" type="primary" @click="isAddingPermission = !isAddingPermission">确认</el-button>
          </span>
        </el-dialog>
      </el-dialog>

    </div>
  </div>
</template>

<script>
  import UserDetails from './comp/UserDetails'
  import {axiosFetch} from "../../../utils/fetchData";
  import {userAddUrl, getUserTypeUrl, planProcessGetUrl, planLineGetUrl} from "../../../config/globalUrl";
  import eventBus from "../../../utils/eventBus";

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
          inService: true,
          lineName: '',
          employeeType: '',
          mainProcess: '',
          proficiency: ''
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
          },
          {
            id: 'EmployeeType',
            name: '是否正式员工',
            model: '',
            type: 'select',
            list: [
              {
                value: '临时工',
                string: '临时工'
              },
              {
                value: '正式工',
                string: '正式工'
              }
            ]
          },
          {
            id: 'MainProcess',
            name: '主要工序',
            model: '',
            type: 'select',
            list: []
          },
          {
            id: 'Proficiency',
            name: '熟练程度',
            model: '',
            type: 'select',
            list: [
              {
                value: '熟练',
                string: '熟练'
              },
              {
                value: '一般',
                string: '一般'
              },
              {
                value: '不熟',
                string: '不熟'
              },
            ]
          },

          {
            id: 'LineName',
            name: '所属产线',
            model: '',
            type: 'select',
            list: []
          },
          {
            id: 'OtherProcess',
            name: '其他技能',
            model: '',
            type: 'text'
          },
        ],
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
        processSelectGroupSrc: null,
        processSelectGroupSrcAfter: null,
        lineSelectGroupSrc: null,
        otherProcess: []
      }
    },
    components: {
      'text-comp': {
        props: ['opt'],
        template: '<div class="form-group">\n' +
          '           <label :for="opt.id">{{opt.name}}：</label>\n' +
          '           <el-input size="small" type="text" class="form-control" :id="opt.id" v-model.trim="opt.model" autocomplete="off" clearable></el-input>\n' +
          '          </div>'
      },
      'select-comp': {
        props: ['opt'],
        template: '<div >\n' +
          '      <div class="form-group ">\n' +
          '        <label :for="opt.id">{{opt.name}}：</label>\n' +
          '        <el-select size="small" :id="opt.id" v-model="opt.model" class="custom-select" style="display: block; margin-right: 20px">\n' +
          '          <el-option value="" disabled label="请选择"></el-option>\n' +
          '          <el-option :value="item.value"  v-for="item in opt.list" :key="item.value" :label="item.string"></el-option>\n' +
          '        </el-select>\n' +
          '      </div>\n' +
          '    </div>'
      },
      UserDetails
    },
    watch: {
      $route: function (route) {
        Object.assign(this.$data.userData, this.$options.data().userData);
        Object.assign(this.$data.tempPermission, this.$options.data().tempPermission);
        Object.assign(this.$data.otherProcess, this.$options.data().otherProcess);
      },

    },
    async mounted() {
      this.$openLoading();
      await this.dataPreload().then(() => {
        this.queryOptions[4].list = this.processSelectGroupSrc.map(item => {
          return {
            value: item['processName'],
            string: item['processName']
          }
        });
        this.queryOptions[6].list = this.lineSelectGroupSrc.map(item => {
          return {
            value: item['lineName'],
            string: item['lineName']
          }
        })
      });
      eventBus.$emit('userQueryData');
    },
    methods: {
      initForm: function () {
        this.queryOptions.map(item => {
          item.model = "";
        })
      },
      dataPreload: async function () {
        return new Promise(resolve => {
          Promise.all([this.getUserType(), this.fetchProcess(), this.fetchLine()]).then(() => {
            resolve();
          })
        })
      },
      getUserType: function () {
        return new Promise(resolve => {
          this.isPending = true;
          axiosFetch({
            url: getUserTypeUrl
          }).then(response => {
            if (response.data.result === 200) {
              this.$store.commit('setUserTypeList', response.data.data)
            } else {
              this.$alertInfo('获取账户权限列表失败，请刷新页面')
            }
          }).catch(err => {
            this.$alertInfo('获取账户权限列表失败，请刷新页面')
          }).finally(() => {
            this.isPending = false;
            this.$closeLoading();
            resolve()
          })
        })
      },

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
      fetchLine: function () {
        return new Promise(resolve => {
          axiosFetch({
            url: planLineGetUrl
          }).then(response => {
            if (response.data.result === 200) {
              this.lineSelectGroupSrc = response.data.data.list;
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.$alertDanger('获取产线信息失败，请刷新重试');
          }).finally(() => {
            resolve();
          })
        });
      },

      addUser: function () {
        Object.assign(this.userData, this.$options.data().userData);
        this.tempPermission = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        this.isAdding = true;
      },
      addSubmit: function () {
        if (!this.isPending) {
          this.$openLoading();
          this.isPending = true;
          let user = this.userData;

          if (user['mainProcess'] !== '' && user['proficiency'] === '') {
            this.$alertWarning("工序熟练度不能为空");
            this.isPending = false;
            this.$closeLoading();
          } else if (!(user.name === "" || user.password === "" || user.webUserType === "" || user.userDes === "")) {
            let optData = JSON.parse(JSON.stringify(user));
            let options = {
              url: userAddUrl,
              data: optData
            };

            options.data.otherProcess = this.otherProcess.join('@@');

            if (user.webUserType === 1) {
              options.data.deletePermission = this.tempPermission.toString();
            }


            //userType patch
            options.data.userType = "&00&10";
            if (user.webUserType === 14 || user.webUserType === 16) {
              options.data.userType = "&1201&1202&1203";
            } else if (user.webUserType === 17) {
              options.data.userType = "&1202&1203";
            } else if (user.webUserType === 18) {
              options.data.userType = "&1204";
            }

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
            }).catch(err => {
              console.log(JSON.stringify(err));
              this.$alertDanger('请求超时，清刷新重试')
            }).finally(() => {
              this.$closeLoading();
              this.isPending = false;
            })
          } else {
            this.$alertWarning("内容不能为空");
            this.isPending = false;
            this.$closeLoading();
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
              if (item.model !== "") {
                if (index === 0) {
                  this.queryString += (item.id + "#like#" + item.model)
                } else {
                  this.queryString += ("#&#" + item.id + "#like#" + item.model)
                }

              } else {
                this.$closeLoading()
              }
            } else if (item.type === 'select') {
              if (item.model !== "") {
                if (index === 0) {
                  this.queryString += (item.id + "#=#" + item.model)
                } else {
                  this.queryString += ("#&#" + item.id + "#=#" + item.model)
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

      selectMainProcess: function (e) {
        if (!!e) {
          this.userData.proficiency = '熟练';
        } else {
          this.userData.proficiency = '';
        }
        this.otherProcess = [];
        this.processSelectGroupSrcAfter = this.processSelectGroupSrc.filter(item => {
          return item.processName !== e

        })
      }
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

  .checkbox-item-box {
    width: 100%;
    padding: 0 60px;
  }

  .checkbox-item {
    margin-bottom: 3px;
  }
</style>
