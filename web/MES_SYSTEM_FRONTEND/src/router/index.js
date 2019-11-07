/*vue-router 配置*/

import Vue from 'vue'
import Router from 'vue-router'
import store from '../store'
import Main from '../pages/Main'
import TableMainOld from '../pages/table_old/TableMain'
import Login from '../pages/user/Login'
import TableModuleOld from '../pages/table_old/details/TableModule'
import OrderMain from '../pages/order/OrderMain'
import OrderManage from '../pages/order/details/OrderManage'
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
/*基础配置*/
import SettingMain from "../pages/setting/SettingMain";
import FactorySetting from "../pages/setting/details/FactorySetting";
import WorkshopSetting from "../pages/setting/details/WorkshopSetting";
import SiteSetting from "../pages/setting/details/SiteSetting";
import CustomerSetting from "../pages/setting/details/CustomerSetting";
import ModelSetting from "../pages/setting/details/ModelSetting";
import ProductSetting from "../pages/setting/details/ProductSetting";
/*E_SOP*/
import ESopMain from "../pages/e_sop/ESopMain";
import FileManager from "../pages/e_sop/details/FileManager";
import NoticeManager from "../pages/e_sop/details/NoticeManager";
import ESopActionLog from "../pages/e_sop/details/ESopActionLog";
import ESopLogs from "../pages/e_sop/details/ESopLogs";
import SiteInfo from "../pages/e_sop/details/SiteInfo";
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
          path: '/esop',
          name: 'ESop',
          component: ESopMain,
          children: [
            {
              path: 'file',
              component: FileManager
            },
            {
              path: 'notice',
              component: NoticeManager
            },
            {
              path: 'site',
              component: SiteInfo
            },
            {
              path: 'action_log',
              component: ESopActionLog
            },
            {
              path: 'confirm_log',
              component: ESopLogs
            },
            {
              path: 'login_log',
              component: ESopLogs
            },
            {
              path: 'notice_log',
              component: ESopLogs
            },
          ]
        },
        {
          path: '/setting',
          name: 'Setting',
          component: SettingMain,
          children: [
            {
              path: 'factory',
              component: FactorySetting
            },
            {
              path: 'workshop',
              component: WorkshopSetting
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
              path: 'site',
              component: SiteSetting
            },
            {
              path: 'customer',
              component: CustomerSetting
            },
            {
              path: 'model',
              component: ModelSetting
            },
            {
              path: 'product',
              component: ProductSetting
            },
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

if (sessionStorage.getItem('UserType')) {
  store.commit('setUserType', sessionStorage.getItem('UserType'))
}
if (sessionStorage.getItem('token')) {
  store.commit('setLoginToken', sessionStorage.getItem('token'))
}
if (sessionStorage.getItem('delPermission')) {
  store.commit('setDelPermission', sessionStorage.getItem('delPermission').split(','))
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
