<template>
  <div class="rz-form-query-bar">
    <div v-for="(queryOption, key) in list"
         :key="'query-comp-bar-item-' + key"
         class="query-bar-item">
      <label
          class="query-comp-label"
          :class="getCompLabelStyle(queryOption.size)"
          :for="'query-comp-bar-item-' + key">{{queryOption.label}}</label>
      <component
          :value="data[queryOption.prop]"
          @input="setInputValue($event, queryOption.prop)"
          :is="queryOption.type + '-comp'"
          :options="queryOption"
          :identity="'query-comp-bar-item-' + key"></component>
    </div>
    <div class="query-bar-button"
         v-for="(buttonItem, key) in buttonGroup"
         :key="'query-comp-bar-button-' + key">
      <el-button
          :size="buttonItem.size"
          :type="buttonItem.type"
          @click="buttonItem.callback()">{{buttonItem.label}}
      </el-button>
    </div>
  </div>
</template>

<script>
  import TextComp from "./comps/TextComp";
  import DateComp from "./comps/DateComp";
  import SelectComp from "./comps/SelectComp";
  import DateRangeComp from "./comps/DateRangeComp";
  import {deepCopy} from "../../plugins/utils";


  export default {
    name: "RZFormQueryBar",
    props: ['list', 'buttonGroup', 'data'],
    components: {
      TextComp,
      DateComp,
      SelectComp,
      DateRangeComp,
    },
    methods: {
      setInputValue(val, prop) {
        let tempObj = deepCopy(this.data);
        tempObj[prop] = val;
        this.$emit('update:data', tempObj)
      },


      /*设置组件标题大小样式*/
      getCompLabelStyle (size) {
        switch (size) {
          case 'small':
            return 'query-comp-label__small';
          case 'mini':
            return 'query-comp-label__mini';
          default:
            return '';
        }
      },
    }
  }
</script>

<style scoped>
  .rz-form-query-bar {
    box-sizing: border-box;
    padding: 10px 20px;
    background: #ffffff;
    border: 1px solid #ededed;
    border-radius: 5px;

    display: flex;
    flex-wrap: wrap;
    align-items: flex-end;
  }

  .query-bar-item, .query-bar-button {
    margin: 0 5px;
  }

  .query-comp-label {
    font-size: 16px;
    line-height: 22px;
    padding: 4px 0;
    color: #5d6066;
    display: block;
  }

  .query-comp-label__small {
    font-size: 14px;
    line-height: 20px;
  }

  .query-comp-label__mini {
    font-size: 12px;
    line-height: 18px;
  }
</style>