/*vue-router 配置*/

import Vue from 'vue'
import Router from 'vue-router'
import store from '../store'
import Main from '../pages/Main'
import TableMain from '../pages/table/TableMain'
import Login from '../pages/user/Login'
import TableModule from '../pages/table/details/TableModule'
import SettingMain from '../pages/setting/SettingMain'
import OrderManage from '../pages/setting/details/OrderManage'
import UserConfig from '../pages/user/UserConfig'
import RedTea from '../pages/redtea/RedTeaMain'
import TestMain from '../pages/test/TestMain'
import TestManage from '../pages/test/details/TestManage'

Vue.use(Router);

const router = new Router({
  base: '/mes_system',
  routes: [
    {
      path: '/_empty'
    },

    {
      path: '/',
      component: Main,
      redirect: '/table',
      meta: {
        requireAuth: true
      },
      children: [
        {
          path: '/table',
          name: 'Table',
          component: TableMain,
          children: [
            {
              path: 'details',
              component: TableModule
            }
          ]
        },
        {
          path: '/setting',
          name: 'Setting',
          component: SettingMain,
          children: [
            {
              path: 'order_manage',
              component: OrderManage
            }
          ]
        },
        {
          path: '/test',
          name: 'Test',
          component: TestMain,
          children: [
            {
              path: 'test_manage',
              component: TestManage
            }
          ]
        },
        {
          path: '/users',
          name: 'Users',
          component: UserConfig
        },
        {
          path: '/redtea',
          name: 'RedTea',
          component: RedTea
        }
      ]
    },

    {
      path: '/login',
      name: 'Login',
      component: Login
    }
  ]
})

if (localStorage.getItem('token')) {
  store.commit('setLoginToken', localStorage.getItem('token'))
}


router.beforeEach((to, from, next) => {
  if (to.path !== '/test/test_manage') {
    store.commit('setTestType', -1)
  }else {
    if (to.query.type){
      store.commit('setTestType', to.query.type)
    }
  }
  if (to.matched.some(r => r.meta.requireAuth)) {
    if (store.state.token) {
      next();
    } else {
      next({
        path: '/login',
        query: {redirect: to.fullPath}
      })
    }
  } else {
    next();
  }
});

export default router;
