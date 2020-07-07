import axios from 'axios';
import Qs from 'qs';
import {setLoginToken} from "../store/actions";
import {redTeaUrl} from './globalUrl'
import store from "../store";
import index from '../router';
import {alertDanger, alertWarning} from "../utils/modal";
// axios.defaults.timeout = 5000;
// axios.defaults.baseURL = window.g.API_URL + '/mes_server/';

axios.interceptors.request.use(
  config => {
    //console.log(store.state.token)
    if (!config.header ) {
      if (store.state.token !== '') {
        if (config.data === "") {
          config.data += ("#TOKEN#=" + store.state.token);
        } else {
          config.data += ("&#TOKEN#=" + store.state.token);
        }
        //config.withCredentials = true;
        //console.log(config)
      }
    }
    return config;
  },
  error => {
    return Promise.reject(error)
  }
);


axios.interceptors.response.use(
  res => {
    if (res.data.result === 401){
      if(res.data.data === '未登录'){
        store.commit('setLoginToken', '');
        sessionStorage.removeItem('token');
        index.replace({
          path: '/login',
          query: {redirect: index.currentRoute.fullPath}
        });
      }
    }
    return res
  },
  error => {
    if (error.response) {
      console.log(JSON.stringify(error))
    }
    return Promise.reject(JSON.stringify(error))
  }
);

export default axios;
