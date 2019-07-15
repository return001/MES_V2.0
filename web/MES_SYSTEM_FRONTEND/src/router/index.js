/*vue-router 配置*/

import Vue from 'vue'
import Router from 'vue-router'
import store from '../store'
import Main from '../pages/Main'
import TableMainOld from '../pages/table_old/TableMain'
import Login from '../pages/user/Login'
import TableModuleOld from '../pages/table_old/details/TableModule'
import SettingMain from '../pages/setting/SettingMain'
import OrderManage from '../pages/setting/details/OrderManage'
import UserConfig from '../pages/user/UserConfig'
import RedTea from '../pages/redtea/RedTeaMain'
import TestMain from '../pages/test/TestMain'
import TestManage from '../pages/test/details/TestManage'
import TableMain from '../pages/table/TableMain'
import TableModule from '../pages/table/details/TableModule'
import TableModuleSP from '../pages/table/details/TableModuleSP'
import TableBackups from '../pages/table/details/Backups'
import MultiTableModule from '../pages/table/details/MultiTableModule'
import FuncMain from '../pages/func/FuncMain'
import MacMain from '../pages/func/mac/MacMain'
import IMEIMain from '../pages/func/imei/IMEIMain'


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
            },
            {
              path: 'backups',
              component: TableBackups
            },
            {
              path: 'details_sp',
              component: TableModuleSP
            },
            {
              path: 'multi',
              component: MultiTableModule
            }
          ]
        },
        {
          path: '/table_old',
          name: 'TableOld',
          component: TableMainOld,
          children: [
            {
              path: 'details',
              component: TableModuleOld
            }
          ]
        },
        {
          path: '/order',
          name: 'Order',
          component: OrderManage,
          // children: [
          //   {
          //     path: 'order_manage',
          //     component: OrderManage
          //   }
          // ]
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
          path: '/func',
          name: 'Func',
          component: FuncMain,
          children: [
            {
              path: 'mac',
              component: MacMain
            },
            {
              path: 'imei',
              component: IMEIMain
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
});

if (localStorage.getItem('UserType')) {
  store.commit('setUserType', localStorage.getItem('UserType'))
}
if (localStorage.getItem('token')) {
  store.commit('setLoginToken', localStorage.getItem('token'))
}
if (localStorage.getItem('delPermission')) {
  store.commit('setDelPermission', localStorage.getItem('delPermission').split(','))
}


router.beforeEach((to, from, next) => {
  if (to.path !== '/test/test_manage') {
    store.commit('setTestType', -1)
  } else {
    if (to.query.type) {
      store.commit('setTestType', to.query.type)
    }
  }
  if (to.matched.some(r => r.meta.requireAuth)) {
    if (store.state.token) {
      if (store.state.userType === 'administration' && to.path !== '/users') {
        next({
          path: '/users'
        })
      } else if (store.state.userType !== 'administration' && store.state.userType !== 'SuperAdmin' && to.path === '/users') {
        next({
          path: '/table'
        })
      } else {
        next();
      }
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
