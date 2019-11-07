// The Vue build version to load with the `import` command
// (runtime-only or standalone) has been set in webpack.base.conf with an alias.
import Vue from 'vue'
import Vuex from 'vuex'
import App from './App'
import router from './router'
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
  Divider
} from 'element-ui'

import {RZSideBar, RZFormQueryBar, RZFormItem} from 'rz-el-comp'

[Table, TableColumn, Pagination, Button, DatePicker, Dialog, Row, Col, Input, Select, Option, Tooltip, CheckboxGroup, Checkbox, MenuItem, Menu, MenuItemGroup, Submenu, Tabs, TabPane, Radio, Form, FormItem, Upload, Carousel, CarouselItem, Autocomplete, Tag, Alert, Divider,

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

Vue.prototype.$openLoading = openLoading;
Vue.prototype.$closeLoading = closeLoading;


Vue.prototype.$axios = axios
Vue.config.productionTip = false
Vue.use(Vuex)
/* eslint-disable no-new */
new Vue({
  el: '#app',
  router,
  store,
  components: {App},
  template: '<App/>'
});

export default {
  router,
  store,
  Vue
}
