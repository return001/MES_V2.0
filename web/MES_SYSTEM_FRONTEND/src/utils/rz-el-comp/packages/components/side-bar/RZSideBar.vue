<template>
  <div class="rz-side-bar">
    <el-menu
        class="side-bar-menu"
        :default-openeds="defaultOpenGroup"
        :collapse="config.collapse"
        :default-active="config.defaultActive || true">
      <template v-for="(sideBarItem, sideBarKey) in list">
        <el-submenu
            v-if="!sideBarItem.isLeaf"
            :key="'side-bar-item-' + sideBarKey"
            :index="sideBarKey.toString()">
          <template slot="title">
            <i :class="sideBarItem.icon"></i>
            <span>{{sideBarItem.label}}</span>
          </template>
          <el-menu-item
              v-for="(subMenuItem, subMenuItemIndex) in sideBarItem.leafList"
              :key="subMenuItem.label + subMenuItemIndex"
              :index="sideBarKey + '-' + subMenuItemIndex"
              @click="subMenuItem.callback()">
            <i :class="subMenuItem.icon"></i>
            <span slot="title">{{subMenuItem.label}}</span>
          </el-menu-item>
        </el-submenu>
        <el-menu-item
            v-else
            :key="'side-bar-item-' + sideBarKey"
            :index="sideBarKey.toString()"
            @click="sideBarItem.callback()">
          <i :class="sideBarItem.icon"></i>
          <span slot="title">{{sideBarItem.label}}</span>
        </el-menu-item>
      </template>
    </el-menu>
  </div>
</template>

<script>
  export default {
    name: "RZSideBar",
    props: ['list', 'config'],
    computed: {
      defaultOpenGroup(){
        if (this.config.defaultOpenAll) {
          let tempArray = [];
          this.list.forEach((item, index) => {
            if (!item.isLeaf) {
              tempArray.push(index.toString())
            }
          });
          return tempArray;
        } else {
          return [];
        }
      }
    }
  }
</script>

<style scoped>
  .side-bar-menu {
    height: 100%;
  }
</style>