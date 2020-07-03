<!--订单配置页面根组件-->
<template>
  <div class="test-manager">
    <div v-if="$route.query.type !== '6'">
      <options/>
      <table-details/>
      <edit-func-panel/>
      <edit-panel/>
    </div>
    <div v-if="$route.query.type === '6'">
      <test-log/>
    </div>
  </div>


</template>

<script>
  import Options from './comp/Options'
  import TableDetails from './comp/TableDetails'
  import EditFuncPanel from './comp/EditFuncPanel'
  import EditPanel from './comp/EditPanel'
  import TestLog from './comp/TestLog'

  export default {
    name: "TestManage",
    data() {
      return {}
    },
    computed: {
    },
    components: {
      Options,
      TableDetails,
      EditFuncPanel,
      EditPanel,
      TestLog
    }
  }
</script>

<style scoped>
  .test-manager {
    padding-top: 20px;
    padding-right: 20px;
  }
</style>
