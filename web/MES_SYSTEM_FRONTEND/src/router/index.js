/*vue-router 配置*/

import Vue from 'vue'
import Router from 'vue-router'
import store from '../store'
import Main from '../pages/Main'
import {closeLoading} from "../utils/loading";

const Login = () => import ('../pages/user/details/Login');
const OrderManage = () => import ('../pages/order/details/OrderManage');

/*用户管理*/
const UserMain = () => import('../pages/user/UserMain');
const UserConfig = () => import ('../pages/user/details/UserConfig');
const DeptMain = () => import('../pages/user/details/DeptMain') ;
const CharMain = () => import('../pages/user/details/CharMain') ;
/*修改密码*/
const EditPassword = () => import('../pages/user/details/EditPassword');

/*红茶*/
const RedTea = () => import ('../pages/redtea/RedTeaMain');
/*测试配置*/
const TestMain = () => import ('../pages/test/TestMain');
const TestManage = () => import ('../pages/test/details/TestManage');
/*表单查询*/
const TableMain = () => import ('../pages/table/TableMain');
const TableModule = () => import ('../pages/table/details/TableModule');
const TableModuleSP = () => import ('../pages/table/details/TableModuleSP');
const TableBackups = () => import ('../pages/table/details/Backups');
const MultiTableModule = () => import ('../pages/table/details/MultiTableModule');
/*功能集合*/
const FuncMain = () => import ( '../pages/func/FuncMain');
const MacMain = () => import ( '../pages/func/mac/MacMain');
const IMEIMain = () => import ( '../pages/func/imei/IMEIMain');
/*排产计划*/
const PlanMain = () => import ('../pages/plan/PlanMain');
const FileType = () => import ('../pages/plan/details/FileType');
const OrderSetting = () => import ('../pages/plan/details/OrderSetting');
const ProcessSetting = () => import ('../pages/plan/details/ProcessSetting');
const ProcessGroupSetting = () => import ('../pages/plan/details/ProcessGroupSetting');
const LineSetting = () => import ('../pages/plan/details/LineSetting');
const CapacitySetting = () => import ('../pages/plan/details/CapacitySetting');
const PlanSetting = () => import ('../pages/plan/details/PlanSetting');
const PlanLogs = () => import ('../pages/plan/details/PlanLogs');
/*基础配置*/
const SettingMain = () => import ("../pages/setting/SettingMain");
const FactorySetting = () => import ("../pages/setting/details/FactorySetting");
const WorkshopSetting = () => import ("../pages/setting/details/WorkshopSetting");
const SiteSetting = () => import ("../pages/setting/details/SiteSetting");
const CustomerSetting = () => import ("../pages/setting/details/CustomerSetting");
const ModelSetting = () => import ("../pages/setting/details/ModelSetting");
const ProductSetting = () => import ("../pages/setting/details/ProductSetting");
/*E_SOP*/
const ESopMain = () => import ("../pages/e_sop/ESopMain");
const FileManager = () => import ("../pages/e_sop/details/FileManager");
const NoticeManager = () => import ("../pages/e_sop/details/NoticeManager");
const ESopActionLog = () => import ("../pages/e_sop/details/ESopActionLog");
const ESopLogs = () => import ("../pages/e_sop/details/ESopLogs");
const SiteInfo = () => import ("../pages/e_sop/details/SiteInfo");
const StaffAssign = () => import ('../pages/e_sop/details/StaffAssign');
/*看板*/
const Dashboard = () => import ('../pages/dashboard/Dashboard');
/*函数*/
import {permissionList} from "../utils/utils";
import {getModulePermission} from "../config/charactersFunc";

Vue.use(Router);

const index = new Router({
  base: '/mes_system',
  routes: [
    {
      path: '/_empty'
    },

    {
      path: '/',
      component: Main,
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
            },
            {
              path: 'redtea',
              component: RedTea
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
              path: 'assign',
              component: StaffAssign
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
            {
              path: 'count_log',
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
            {
              path: 'file_type',
              component: FileType
            },
            {
              path: 'capacity',
              component: CapacitySetting
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
           /* {
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
            },*/
            {
              path: 'detail',
              component: PlanSetting
            },
            {
              path: 'action_log',
              component: PlanLogs
            }
          ]
        },
        {
          path: '/user',
          name: 'Users',
          component: UserMain,
          children: [
            {
              path: 'users',
              component: UserConfig
            },
            {
              path: 'dept',
              component: DeptMain
            },
            {
              path: 'char',
              component: CharMain
            }
          ]
        },
        // {
        //   path: '/redtea',
        //   name: 'RedTea',
        //   component: RedTea
        // },
        {
          path: '/editPassword',
          name: 'EditPassword',
          component: EditPassword
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
if (sessionStorage.getItem('charactersFuncMap')) {
  store.commit('setCharactersFuncMap', JSON.parse(sessionStorage.getItem('charactersFuncMap')))
}


index.beforeEach((to, from, next) => {
  if (to.path !== '/test/test_manage') {
    store.commit('setTestType', -1)
  } else {
    if (to.query.type) {
      store.commit('setTestType', to.query.type)
    }
  }
  if (to.matched.some(r => r.meta.requireAuth)) {
    if (store.state.token) {
      if (to.path !== '/' && !getModulePermission(to.path, to.query, store.state.charactersFuncMap.pageList)) {
        next({
          path: '/'
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

index.afterEach((to, from) => {
  closeLoading();
});


export default index;
