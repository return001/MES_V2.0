<!--订单配置页面根组件-->
<template>
  <div class="test-manager">
    <options/>
    <table-details/>
    <edit-func-panel/>
    <edit-panel/>
  </div>
</template>

<script>
  import Options from './comp/Options'
  import TableDetails from './comp/TableDetails'
  import EditFuncPanel from './comp/EditFuncPanel'
  import EditPanel from './comp/EditPanel'

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
      EditPanel
    }
  }
</script>

<style scoped>
  .test-manager {
    padding-top: 20px;
    padding-right: 20px;
  }
</style>
