<template>
  <el-date-picker
      :id="identity"
      :size="options.size"
      type="datetimerange"
      range-separator="-"
      prefix-icon="el-icon-date"
      start-placeholder="开始日期"
      end-placeholder="结束日期"
      :picker-options="pickerOptions"
      value-format="yyyy-MM-dd HH:mm:ss"

      :value="value"
      @input="setInputValue">
  </el-date-picker>
</template>

<script>
  import common from "../mixins/common";

  export default {
    name: "DateRangeComp",
    mixins: [common],
    props: ['options', 'identity', 'value'],
    data() {
      return {
        pickerOptions: {}
      }
    },
    created() {
      if (this.options.pickerList) {
        this.pickerOptions.shortcuts = this.options.pickerList.map(item => {
          return {
            text: item.label,
            onClick(picker) {
              const end = new Date();
              const start = new Date();
              start.setTime(start.getTime() - item.duringTime);
              picker.$emit('pick', [start, end]);
            }
          }
        })
      }
    },
    methods: {}
  }
</script>

<style scoped>

</style>