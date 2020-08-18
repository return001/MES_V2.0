// The Vue build version to load with the `import` command
// (runtime-only or standalone) has been set in webpack.base.conf with an alias.
import Vue from 'vue'
import Vuex from 'vuex'
import App from './App'
import index from './router'
import axios from './config/http'
import store from './store'


//自定义注册图标
import './assets/icons/iconfont.css'

import {
  Table,
  TableColumn,
  Pagination,
  Button,
  MessageBox,
  Message,
  DatePicker,
  Dialog,
  Row,
  Col,
  Input,
  Select,
  Option,
  Tooltip,
  CheckboxGroup,
  Checkbox,
  MenuItem,
  Menu,
  MenuItemGroup,
  Submenu,
  Tabs,
  TabPane,
  Radio,
  Form,
  FormItem,
  Upload,
  Carousel,
  CarouselItem,
  Autocomplete,
  Tag,
  Alert,
  Divider,
  RadioGroup,
  TimePicker,
  Switch,
  Cascader
} from 'element-ui'

import {RZSideBar, RZFormQueryBar, RZFormItem} from 'rz-el-comp'

[Table, TableColumn, Pagination, Button,DatePicker, Dialog, Row, Col, Input, Select, Option, Tooltip, CheckboxGroup, Checkbox, MenuItem, Menu, MenuItemGroup, Submenu, Tabs, TabPane, Radio, Form, FormItem, Upload, Carousel, CarouselItem, Autocomplete, Tag, Alert, Divider, RadioGroup, TimePicker, Switch, Cascader,

  RZSideBar, RZFormQueryBar, RZFormItem
].forEach(item => {
  Vue.use(item)
});

import {alertDanger, alertInfo, alertSuccess, alertWarning} from "./utils/modal";
import {openLoading, closeLoading} from "./utils/loading";



Vue.prototype.$confirm = MessageBox.confirm;
Vue.prototype.$alertDanger = alertDanger;
Vue.prototype.$alertInfo = alertInfo;
Vue.prototype.$alertSuccess = alertSuccess;
Vue.prototype.$alertWarning = alertWarning;
Vue.prototype.$message = Message;

Vue.prototype.$openLoading = openLoading;
Vue.prototype.$closeLoading = closeLoading;
// Vue.prototype.$ELEMENT = { size: 'small', zIndex: 3000 };


Vue.prototype.$axios = axios
Vue.config.productionTip = false
Vue.use(Vuex)
//生产环境关闭 console
if (process.env.NODE_ENV === 'production') {

  if (!global.consoleSwitch) {
    console.log = () => {};
  }
}



/* eslint-disable no-new */
new Vue({
  el: '#app',
  router: index,
  store,
  components: {App},
  template: '<App/>'
});

export default {
  router: index,
  store,
  Vue
}
