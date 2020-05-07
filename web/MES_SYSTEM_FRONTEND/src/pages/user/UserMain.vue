<template>
  <div id="user-main" style="margin-left: 52px;">
    <div class="side-bar">
      <side-bar/>
    </div>
    <div class="router-style">
      <router-view v-if="isRouterAlive"/>
    </div>
  </div>
</template>

<script>
  import SideBar from './details/comp/SideBar'
  export default {
    name: "UserMain",
    components: {SideBar},
    provide() {
      return {
        reload: this.reload
      }
    },
    data() {
      return {
        isRouterAlive: true
      }
    },

    methods: {
      reload: function () {
        this.isRouterAlive = false;
        this.$nextTick(function () {
          this.isRouterAlive = true;
        })
      }
    }
  }
</script>

<style scoped>

  @media (min-width: 640px) {
    .side-bar {
      width: 160px;
    }

    .router-style {
      min-width: 290px;
    }
  }

  .side-bar {
    position: fixed;
    background: #fff;
    box-shadow: 5px 0 5px 0 #ddd;
    height: 100%;
    padding: 0 20px;
  }

  .main-style {
    margin: 0;
    height: 100%;
  }

  .router-style {
    padding: 20px 20px 10px 0;
    margin-left: 217px;
  }
</style>