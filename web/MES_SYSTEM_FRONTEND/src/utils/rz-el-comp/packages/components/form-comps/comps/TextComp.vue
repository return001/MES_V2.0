<template>
  <el-input
      class="text-comp-input"
      :id="identity"
      :type="options.inputType || 'text'"
      :placeholder="'请输入' + options.label"
      :size="options.size"
      :clearable="options.clearable !== false"
      :rows="options.rows || 3"

      :minlength="!!options.limit && options.limit[0]"
      :maxlength="!!options.limit && options.limit[1]"
      :show-word-limit="!!options.limit"

      :value="value"
      @input="setInputValue"></el-input>
</template>

<script>
  import common from "../mixins/common";

  export default {
    name: "TextComp",
    mixins: [common],
    props: ['options', 'identity', 'value'],
    data() {
      return {}
    },
    methods: {}
  }
</script>

<style scoped>
  .text-comp-input {
    width: 220px;
  }
</style>