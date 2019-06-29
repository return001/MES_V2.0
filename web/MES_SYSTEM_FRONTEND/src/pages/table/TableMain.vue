<!--表单-->
<template>
  <div>
    <!--<Header/>-->
    <loading v-if="$store.state.isLoading"/>
    <div class="main-container" style="margin-left: 52px;">
      <div class="main-style">
        <div class="side-bar">
          <side-bar/>
        </div>
        <div class="router-style" style="margin-left: 252px;">
          <router-view/>
        </div>
      </div>
    </div>

  </div>
</template>

<script>
  import Header from '@/components/PageHeader'
  import SideBar from './details/comp/SideBar'
  import Loading from '@/components/Loading'
  import SideSetting from '@/components/SideSetting'

  export default {
    name: "Main",
    components: {
      Header,
      SideBar,
      Loading,
      SideSetting
    },
    data() {
      return {
      }
    },
    mounted: function () {


    },
    methods: {}
  }
</script>

<style scoped>
  @media (min-width: 640px) {
    .side-bar {
      width: 250px;
    }

    .router-style {
      min-width: 340px;
    }
  }

  .side-bar {
    position: fixed;
    background: #fff;
    box-shadow: 5px 0 5px 0 #ddd;
    height: 100%;
    padding: 0 20px;
    overflow-y: auto;
    width: 200px;
  }

  .main-style {
    margin: 0;
    height: 100%;
  }

  .slide-enter-active, .slide-leave-active {
    transform: translate3d(0, 0, 0);
    margin-left: 0;
    transition: all .5s;
  }

  .slide-enter, .slide-leave-to {
    transform: translate3d(0, 0, 0);
    margin-left: -250px;
  }
</style>
