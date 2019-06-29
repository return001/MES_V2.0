<!--表单查看根组件-->
<template>
  <div class="table-module">
    <options/>
    <table-details/>
  </div>
</template>

<script>
  import Options from './comp/SearchOptions'
  import TableDetails from './comp/TableDetails'

  export default {
    name: "Details",
    components: {
      Options,
      TableDetails
    }
  }
</script>

<style scoped>
  .table-module {
    padding-top: 10px;
    padding-right: 20px;
  }
</style>
