<template>
  <el-form
      :model="data"
      class="form-style"
      :ref="config.refName"
      label-position="top"
      :size="config.size"
      @submit.native.prevent
      :rules="rules">
    <el-form-item
        class="form-item-style"
        v-for="(formItem, formItemKey) in list"
        :label="formItem.label"
        :key="(config.identity || Math.floor(Math.random() * 100000)) + '-' + formItemKey"
        :prop="formItem.prop">
      <component
          :value="data[formItem.prop]"
          @input="setInputValue($event, formItem.prop)"
          :is="formItem.type + '-comp'"
          :options="formItem"></component>
    </el-form-item>
  </el-form>
</template>

<script>
  import TextComp from "./comps/TextComp";
  import DateComp from "./comps/DateComp";
  import SelectComp from "./comps/SelectComp";
  import SwitchComp from "./comps/SwitchComp";
  import {deepCopy} from "../../plugins/utils";

  export default {
    name: "RZFormItem",
    props: ['config', 'list', 'rules', 'data'],
    components: {
      TextComp,
      DateComp,
      SelectComp,
      SwitchComp,
    },
    data() {
      return {
        validate: undefined,
      }
    },
    mounted() {
      this.validate = this.$refs[this.config.refName].validate
    },
    methods: {
      setInputValue(val, prop) {
        let tempObj = deepCopy(this.data);
        tempObj[prop] = val;
        this.$emit('update:data', tempObj)
      },


    }
  }
</script>

<style scoped>
  .form-style {
    display: flex;
    flex-wrap: wrap;
  }

  .form-item-style {
    margin: 0 5px;
    padding-bottom: 14px;
  }

  .form-style /deep/ label {
    padding: 0;
  }
</style>
