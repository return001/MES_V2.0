<!--配置页面总组件-->
<template>
  <div>
    <div class="" style="margin-left: 52px;">
      <div class="row main-style">
        <div class="side-bar">
          <side-bar/>
        </div>
        <div class="router-style" style="margin-left: 217px;" v-if="$store.state.testType !== -1">
          <router-view v-if="isRouterAlive"/>
        </div>
      </div>
    </div>

  </div>
</template>
<script>
  import SideBar from './details/comp/SideBar'
  import SideSetting from '../../components/SideSetting'

  export default {
    name: "Main",
    components: {
      SideBar,
      SideSetting
    },
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
    mounted() {
    },
    methods: {
      reload: function () {
        console.log(this.$store.state.testType)
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

</style>
