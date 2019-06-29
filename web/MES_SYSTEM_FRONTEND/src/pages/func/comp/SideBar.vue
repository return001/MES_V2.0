<!--配置功能统一导航-->
<template>
  <!--<div class="mt-3 mb-3">-->
  <!--<nav>-->
  <!--<div class="sidebar-items">-->
  <!--&lt;!&ndash;订单配置&ndash;&gt;-->
  <!--<div class="sidebar-title">-->
  <!--<a class="subtitle" draggable="false" data-toggle="collapse" href="#collapsePenetrate" aria-expanded="false"-->
  <!--aria-controls="collapsePenetrate">MAC返工</a>-->
  <!--</div>-->
  <!--<div class="collapse show" id="collapsePenetrate">-->
  <!--<div @click="toggleState('mac')">-->
  <!--<div class="sidebar-link" @click="linkTo({link: '/func/mac', type: 'mac'})" :class="activeItem === 'mac' ? 'active' : ''">MAC返工</div>-->
  <!--</div>-->
  <!--</div>-->
  <!--</div>-->
  <!--</nav>-->

  <el-menu
    class="sidebar-container"
    :default-active="activeItem"
    :router="true">
    <el-submenu index="1">
      <template slot="title">
        <span>MAC返工</span>
      </template>
      <el-menu-item-group>
        <el-menu-item index="/func/mac">MAC返工</el-menu-item>
      </el-menu-item-group>
    </el-submenu>
    <el-submenu index="2">
      <template slot="title">
        <span>IMEI查询</span>
      </template>
      <el-menu-item-group>
        <el-menu-item index="/func/imei">查询未使用的IMEI</el-menu-item>
      </el-menu-item-group>
    </el-submenu>
  </el-menu>
  <!--</div>-->
</template>

<script>
  import {mapGetters, mapActions} from 'vuex'

  export default {
    data() {
      return {

        //控制列表active状态，当前已激活的项目
        activeItem: ""

      }
    },
    watch: {
      $route: function (val) {
        this.activeItem = val.path
      }
    },
    mounted: function () {
      this.activeItem = this.$route.path
    },
    computed: {
      ...mapGetters([
        'tableRouterApi',
        'isLoading'
      ]),
    },
    methods: {
      ...mapActions(['setTableRouter', 'setLoading']),
      toggleState: function (item) {
        this.activeItem = item;

      },
      linkTo: function (obj) {
        if (this.$route.fullPath !== obj.link) {
          this.$openLoading();
          this.$router.push({
            path: obj.link
          })
        }
      }
    }


  }
</script>

<style scoped>
  a {
    text-decoration: none;
    color: #000;
  }

  .sidebar-container /deep/ .el-submenu__title {
    height: 40px;
    line-height: 40px;
    background-color: #458aff;
    color: #fff;
    border-radius: 8px;
    margin: 8px 8px 0;
  }

  .sidebar-container /deep/ .el-menu-item {
    height: 40px;
    line-height: 40px;
  }

  .sidebar-container /deep/ i {
    color: #ffffff;
  }
</style>
