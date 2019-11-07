<!--订单配置页面根组件-->
<template>
  <div id="order-main">
    <loading v-if="$store.state.isLoading"/>
    <options/>
    <table-details/>
    <edit-panel/>
    <!--<create-relation/>-->
  </div>
</template>

<script>
  import Options from './comp/Options'
  import TableDetails from './comp/TableDetails'
  import EditPanel from './comp/EditPanel'
  import CreateRelation from './comp/CreateRelation'
  import {mapGetters, mapActions} from 'vuex'
  import Loading from '../../../components/Loading'

  export default {
    name: "OrderManage",
    data() {
      return {}
    },
    computed: {
      ...mapGetters(['isEditing', 'editData'])
    },
    components: {
      Options,
      TableDetails,
      EditPanel,
      Loading,
      CreateRelation
    }
  }
</script>

<style scoped>
  #order-main {
    padding: 20px 20px 10px 80px;
  }

  .fade-enter-active, .fade-leave-active {
    transition: opacity .5s;
  }
  .fade-enter, .fade-leave-to {
    opacity: 0;
  }
</style>
