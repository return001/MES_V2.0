<!--登录组件-->
<template>
  <div>
    <!--标题栏头部-->
    <page-header/>

    <div class="login-container" :style="{height: pageHeight + 'px'}">
      <div class="login-panel">
        <form @submit.prevent="loginSubmit">
          <div class="form-item">
            <label for="login-username">用户名</label>
            <el-input type="text"
                      id="login-username"
                      class="form-control"
                      placeholder="用户名"
                      v-model="loginInfos.name"
                      @keyup.enter="loginSubmit"
                      ref="login-input"/>
          </div>
          <div class="form-item">
            <label for="login-password">密 码</label>
            <el-input type="password" id="login-password" class="form-control" placeholder="密码"
                      v-model="loginInfos.password" @keyup.enter="loginSubmit"/>
          </div>
          <div class="form-item">
            <el-button type="primary" style="width: 100%"  native-type="submit">登录</el-button>
          </div>
        </form>
      </div>
    </div>
  </div>
</template>

<script>
  import PageHeader from '../../../components/PageHeader'
  import {axiosFetch} from "../../../utils/fetchData";
  import {loginUrl} from "../../../config/globalUrl";
  import {mapActions} from "vuex";
  import {errHandler} from "../../../utils/errorHandler";
  import {initCharactersFuncMap, getCharactersFuncList} from "../../../config/charactersFunc";

  export default {
    name: "Login",
    components: {
      PageHeader
    },
    data() {
      return {
        pageHeight: 0,
        isPending: false,
        loginInfos: {
          "name": "",
          "password": "",
          //"#TOKEN#": ""
          //checked: false
        },
        token: ''
      }
    },
    mounted: function () {
      /*动态处理页面高度*/
      this.pageHeightCalc();
      window.onresize = () => {
        this.pageHeightCalc();
      };
      this.token = (sessionStorage.getItem('token'))
        ? sessionStorage.getItem('token')
        : '';
      this.$nextTick(() => {
        this.$refs['login-input'].focus();
      })
    },
    methods: {
      ...mapActions(['setLoginToken', 'setUserType', 'setDelPermission', 'setCharactersFuncMap','setSessionFactory']),
      pageHeightCalc: function () {
        this.pageHeight = (document.body.clientHeight > 460) ? document.body.clientHeight - 200 : 260;
      },
      /*登录处理*/

      loginSubmit: function () {
        if (!this.isPending) {
          this.$openLoading();
          this.isPending = true;
          let options = {
            url: loginUrl,
            data: this.loginInfos
          };
          axiosFetch(options).then(res => {
            if (res.data.result === 200) {
              sessionStorage.setItem('factory', res.data.data['roleVO']['company']);//人员的所属工厂
              // this.setSessionFactory(sessionStorage.getItem('factory'));
              sessionStorage.setItem('token', res.data.data["token"]);
              this.setLoginToken(sessionStorage.getItem('token'));
              sessionStorage.setItem('UserName', res.data.data['name']);


              /*生成权限列表*/
              let map = new getCharactersFuncList();
              let funcList = initCharactersFuncMap(map, res.data.data['webAuthorities']);
              this.setCharactersFuncMap(funcList);
              sessionStorage.setItem('charactersFuncMap', JSON.stringify(funcList));

              this.$router.replace('/');
            } else if (res.data.result === 412) {
              this.$alertWarning("请检查用户名或密码")
            } else if (res.data.result === 400) {
              this.$alertWarning("请勿重复登录");
              this.$router.replace('/')
            } else {
              errHandler(res.data.result)
            }
          }).catch(err => {
            this.isPending = false;
            console.log(err);
            this.$alertDanger(err);
          }).finally(() => {
            this.$closeLoading();
            this.isPending = false;
          })
        }
      }
    }
  }
</script>

<style scoped>
  .login-container {
    width: 100%;
    display: flex;
    justify-content: center;
    align-items: center;
  }

  .login-panel {
    background: #fff;
    border: 1px solid #999;
    border-radius: 5px;
    height: 200px;
    width: 320px;
    padding: 10px 20px;
  }

  .form-item {
    margin: 5px 0;
  }

  .form-item label {
    line-height: 30px;
  }
</style>
