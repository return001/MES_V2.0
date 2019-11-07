<template>
  <el-date-picker
      :id="identity"
      :size="options.size"
      type="datetime"
      range-separator="-"
      prefix-icon="el-icon-date"
      placeholder="请选择日期"
      value-format="yyyy-MM-dd HH:mm:ss"

      :value="value"
      @input="setInputValue"></el-date-picker>
</template>

<script>
  import common from "../mixins/common";

  export default {
    name: "DateComp",
    mixins: [common],
    props: ['options', 'identity', 'value'],
    data() {
      return {}
    },
    created() {
    },
    methods: {}
  }
</script>

<style scoped>
</style>