// The Vue build version to load with the `import` command
// (runtime-only or standalone) has been set in webpack.base.conf with an alias.
import Vue from 'vue'
import Vuex from 'vuex'
import App from './App'
import router from './router'
import axios from './config/http'
import store from './store'
// import 'bootstrap/dist/css/bootstrap.min.css'
// import 'bootstrap/dist/js/bootstrap.bundle.min'

//自定义注册图标
import './assets/icons/iconfont.css'

import {Table, TableColumn, Pagination, Button, MessageBox, DatePicker, Dialog, Row, Col, Input, Select, Option, Tooltip, CheckboxGroup, Checkbox, MenuItem, Menu, MenuItemGroup, Submenu, Tabs, TabPane, Radio} from 'element-ui'

import Datatable from '../static/js/datatable.min.js'
import locale from './locale/zh-cn'
import {alertDanger, alertInfo, alertSuccess,alertWarning} from "./utils/modal";
import {openLoading, closeLoading} from "./utils/loading";

Vue.prototype.$confirm = MessageBox.confirm;
Vue.prototype.$alertDanger = alertDanger;
Vue.prototype.$alertInfo = alertInfo;
Vue.prototype.$alertSuccess = alertSuccess;
Vue.prototype.$alertWarning = alertWarning;

Vue.prototype.$openLoading = openLoading;
Vue.prototype.$closeLoading = closeLoading;


Vue.use(Table)
Vue.use(TableColumn)
Vue.use(Pagination)
Vue.use(Button)
Vue.use(DatePicker)
Vue.use(Dialog)
Vue.use(Row)
Vue.use(Col)
Vue.use(Input)
Vue.use(Select)
Vue.use(Option)
Vue.use(Tooltip)
Vue.use(CheckboxGroup)
Vue.use(Checkbox)
Vue.use(MenuItemGroup)
Vue.use(Menu)
Vue.use(MenuItem)
Vue.use(Submenu)
Vue.use(Tabs)
Vue.use(TabPane)
Vue.use(Radio)

Vue.use(Datatable, {locale})
Vue.prototype.$axios = axios
Vue.config.productionTip = false
Vue.use(Vuex)
/* eslint-disable no-new */
new Vue({
  el: '#app',
  router,
  store,
  components: { App },
  template: '<App/>'
});

export default {
  router,
  store,
  Vue
}
