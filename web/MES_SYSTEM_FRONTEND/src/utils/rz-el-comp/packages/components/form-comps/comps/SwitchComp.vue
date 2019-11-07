<template>
  <el-switch
      class="switch-comp-input"
      :id="identity"
      :size="options.size"
      :value="value"
      @input="setInputValue"></el-switch>
</template>

<script>
  import common from "../mixins/common";

  export default {
    name: "SwitchComp",
    mixins: [common],
    props: ['options', 'identity', 'value'],
    data() {
      return {}
    },
    methods: {}
  }
</script>

<style scoped>
  .switch-comp-input {
    width: 220px;
  }
</style>