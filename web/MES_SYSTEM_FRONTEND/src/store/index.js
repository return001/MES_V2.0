import Vue from 'vue'
import Vuex from 'vuex'
import * as mutations from './mutations'
import * as actions from './actions'
import * as getters from './getters'

Vue.use(Vuex);

const state = {
  token: '',
  factory:"",
  routerIn: 'table',
  isEditing: false,
  editData: [],
  copyData: [],
  testType: -1,
  userType: '',
  userTypeList: {},
  tableDeleteGroup: {
    'DataRelativeSheet': [],
    'DataRelativeUnique': [],
    'Gps_AutoTest_Result': [],
    'Gps_AutoTest_Result3': [],
    'Gps_CartonBoxTwenty_Result': [],
    'Gps_CoupleTest_Result': [],
    'Gps_ManuPrintParam': [],
    'NetMarkIMEI': [],
    'Gps_ManuSimDataParam': [],
    'Gps_TestResult': []
  },
  delPermission: [],

  stashData: {},
};

const store = new Vuex.Store({
  state,
  getters,
  actions,
  mutations
});

export default store;
