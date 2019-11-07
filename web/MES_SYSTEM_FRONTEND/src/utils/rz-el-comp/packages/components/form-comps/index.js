import RZFormItem from "./RZFormItem";
import RZFormQueryBar from "./RZFormQueryBar";

RZFormItem.install = function (Vue) {
  Vue.component('rz-form-item', RZFormItem)
};

RZFormQueryBar.install = function (Vue) {
  Vue.component('rz-query-bar', RZFormQueryBar)
};

export {
  RZFormQueryBar,
  RZFormItem,
}
