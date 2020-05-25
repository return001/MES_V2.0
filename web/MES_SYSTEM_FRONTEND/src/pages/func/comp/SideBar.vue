<!--配置功能统一导航-->
<template>

  <el-menu
    class="sidebar-container"
    :default-active="activeItem"
    :router="true">
    <el-submenu :index="index.toString()" v-for="(item, index) in linkListValidated">
      <template slot="title">
        <span>{{item.title}}</span>
      </template>
      <el-menu-item-group>
        <el-menu-item :index="subItem.path"
                      v-for="subItem in item.subList">{{subItem.name}}</el-menu-item>
      </el-menu-item-group>
    </el-submenu>
  </el-menu>
</template>

<script>
  import {mapGetters} from 'vuex'

  export default {
    data() {
      return {
        linkList: [
          {
            title: 'MAC返工',
            moduleName: 'mac',
            subList: [
              {
                activeKey: 'mac',
                name: 'MAC返工',
                path: '/func/mac',
                type: 'mac'
              }
            ]
          },
          {
            title: 'IMEI查询',
            moduleName: 'imei',
            subList: [
              {
                activeKey: 'imei',
                name: '备份查询',
                path: '/func/imei',
                type: 'imei'
              }
            ]
          },
          {
            title: '红茶',
            moduleName: 'redtea',
            subList: [
              {
                activeKey: 'redtea',
                name: '红茶查询',
                path: '/func/redtea',
                type: 'redtea'
              }
            ]
          }
        ],
        linkListValidated: [],
        //控制列表active状态，当前已激活的项目
        activeItem: ""

      }
    },
    watch: {
      $route: function (val) {
        this.activeItem = val.path
      }
    },
    computed: {
      ...mapGetters([
        'charactersFuncMap'
      ]),
    },
    beforeMount() {
      /*根据权限初始化页面列表*/
      const pageList = this.charactersFuncMap.pageList;
      this.linkListValidated = this.linkList.map(item => {
        let _subList = item.subList.filter(subItem => {
          return pageList.indexOf(`func-${item.moduleName}-${subItem.type}`) >= 0;
        });
        if (_subList.length > 0) {
          item.subList = _subList;
          return item
        }
      }).filter(item => item !== undefined);
    },
    mounted: function () {
      this.activeItem = this.$route.path
    },
    methods: {
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
