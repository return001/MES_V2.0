<!--配置页面总组件-->
<template>
  <div>
    <!--<Header/>-->
    <loading v-if="$store.state.isLoading"/>
    <div class="" style="margin-left: 52px;">
      <div class="row main-style">
          <div class="side-bar">
            <side-bar/>
          </div>
        <div class="router-style" style="margin-left: 217px;" v-if="$store.state.testType !== -1">
          <router-view/>
        </div>
      </div>
    </div>

  </div>
</template>
<script>
  import Header from '../../components/PageHeader'
  import SideBar from './details/comp/SideBar'
  import Loading from '@/components/Loading'
  import SideSetting from '../../components/SideSetting'

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
