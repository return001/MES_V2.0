<template>
  <el-select
      :id="identity"
      class="select-comp-select"
      :size="options.size"
      :value="value"
      @input="setInputValue"
      :clearable="options.clearable !== false">
    <el-option
        v-for="selectOption in options.selectList"
        :key="selectOption.key"
        :value="selectOption.key"
        :label="selectOption.label"></el-option>
  </el-select>
</template>

<script>
  import common from "../mixins/common";

  export default {
    name: "SelectComp",
    mixins: [common],
    props: ['options', 'identity', 'value'],
    data() {
      return {}
    },
    methods: {}
  }
</script>

<style scoped>
  .select-comp-select {
    width: 220px;
  }

</style>