<!--系统主要功能页面根组件-->
<template>
  <div class="main-body">
    <side-setting/>
    <router-view/>
    <div id="user-info">
      <el-tooltip content="修改密码" effect="light">
        <span class="user-info-avatar" @click="editUser">
          <i class="el-icon-setting"></i>
        </span>
      </el-tooltip>
      <span class="user-info-name">
        {{userName}}
      </span>
    </div>
  </div>
</template>

<script>
  import SideSetting from '../components/SideSetting'
  import {getFunctionPermission} from "../config/charactersFunc";
  import eventBus from "../utils/eventBus";
  import store from "../store"

  export default {
    name: "Main",
    components: {
      SideSetting
    },
    data() {
      return {
        userName: sessionStorage.getItem('UserName') || ''
      }
    },
    provide() {
      return {
        _getFunctionPermission: this.getPermission
      }
    },
    mounted() {
    },

    methods: {
      editUser: function () {
        this.$router.push('/editPassword');
      },
      getPermission: function (functionType) {
        return getFunctionPermission(this.$route.path, this.$route.query, functionType, store.state.charactersFuncMap.map)
      }
    }
  }
</script>

<style scoped>
  #user-info {
    position: absolute;
    top: 10px;
    right: 0;
    height: 48px;
    width: 100px;
    background-color: #fff;
    box-shadow: 0 0 10px #9ca1ab;
    z-index: 2;
  }

  .user-info-avatar {
    display: inline-block;
    background-color: #fff;
    margin-left: -24px;
    border-radius: 50%;
    height: 48px;
    width: 48px;
    box-shadow: 0 0 10px #9ca1ab;
    z-index: 1;
    padding: 12px;
    box-sizing: border-box;
    cursor: pointer;
  }

  .user-info-avatar i {
    font-size: 24px;
    color: #458aff;
  }

  .user-info-name {
    overflow: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
    font-size: 14px;
    color: #458aff;
    line-height: 48px;
    display: inline-block;
    position: absolute;
    padding: 0 14px 0 7px;
    width: 66px;
    box-sizing: border-box;
  }
</style>
