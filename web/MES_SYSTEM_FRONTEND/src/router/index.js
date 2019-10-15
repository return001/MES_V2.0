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
/*红茶*/
import RedTea from '../pages/redtea/RedTeaMain'
/*测试配置*/
import TestMain from '../pages/test/TestMain'
import TestManage from '../pages/test/details/TestManage'
/*表单查询*/
import TableMain from '../pages/table/TableMain'
import TableModule from '../pages/table/details/TableModule'
import TableModuleSP from '../pages/table/details/TableModuleSP'
import TableBackups from '../pages/table/details/Backups'
import MultiTableModule from '../pages/table/details/MultiTableModule'
/*功能集合*/
import FuncMain from '../pages/func/FuncMain'
import MacMain from '../pages/func/mac/MacMain'
import IMEIMain from '../pages/func/imei/IMEIMain'
/*排产计划*/
import PlanMain from '../pages/plan/PlanMain'
import OrderSetting from '../pages/plan/details/OrderSetting'
import ProcessSetting from '../pages/plan/details/ProcessSetting'
import ProcessGroupSetting from '../pages/plan/details/ProcessGroupSetting'
import LineSetting from '../pages/plan/details/LineSetting'
import CapacitySetting from '../pages/plan/details/CapacitySetting'
import PlanSetting from '../pages/plan/details/PlanSetting'

/*看板*/
import Dashboard from '../pages/dashboard/Dashboard'

/*函数*/
import {permissionList} from "../utils/utils";

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
          path: '/plan',
          name: 'Plan',
          component: PlanMain,
          children: [
            {
              path: 'order',
              component: OrderSetting
            },
            {
              path: 'process',
              component: ProcessSetting
            },
            {
              path: 'process_group',
              component: ProcessGroupSetting
            },
            {
              path: 'line',
              component: LineSetting
            },
            {
              path: 'capacity',
              component: CapacitySetting
            },
            {
              path: 'detail',
              component: PlanSetting
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
      path: '/dashboard',
      name: Dashboard,
      component: Dashboard
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
      if (permissionList().indexOf(to.path.split('/')[1]) === -1) {
        next({
          path: '/' + permissionList()[0]
        })
      } else {
        next();
      }
    } else {
      next({
        path: '/login'
      })
    }
  } else {
    next();
  }
});

export default router;
