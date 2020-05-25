/*vue-router 配置*/

import Vue from 'vue'
import Router from 'vue-router'
import store from '../store'
import Main from '../pages/Main'
import { closeLoading } from "../utils/loading";

const Login = () =>
    import ('../pages/user/Login');
const OrderManage = () =>
    import ('../pages/order/details/OrderManage');
const UserConfig = () =>
    import ('../pages/user/UserConfig');
/*红茶*/
const RedTea = () =>
    import ('../pages/redtea/RedTeaMain');
/*测试配置*/
const TestMain = () =>
    import ('../pages/test/TestMain');
const TestManage = () =>
    import ('../pages/test/details/TestManage');
/*表单查询*/
const TableMain = () =>
    import ( /* webpackChunkName: "group-table" */ '../pages/table/TableMain');
const TableModule = () =>
    import ( /* webpackChunkName: "group-table" */ '../pages/table/details/TableModule');
const TableModuleSP = () =>
    import ( /* webpackChunkName: "group-table" */ '../pages/table/details/TableModuleSP');
const TableBackups = () =>
    import ( /* webpackChunkName: "group-table" */ '../pages/table/details/Backups');
const MultiTableModule = () =>
    import ( /* webpackChunkName: "group-table" */ '../pages/table/details/MultiTableModule');
/*功能集合*/
const FuncMain = () =>
    import ( /* webpackChunkName: "group-func" */ '../pages/func/FuncMain');
const MacMain = () =>
    import ( /* webpackChunkName: "group-func" */ '../pages/func/mac/MacMain');
const IMEIMain = () =>
    import ( /* webpackChunkName: "group-func" */ '../pages/func/imei/IMEIMain');
/*排产计划*/

const PlanMain = () =>
    import ( /* webpackChunkName: "group-plan" */ '../pages/plan/PlanMain');
const FileType = () =>
    import ( /* webpackChunkName: "group-plan" */ '../pages/plan/details/FileType');
const OrderSetting = () =>
    import ( /* webpackChunkName: "group-plan" */ '../pages/plan/details/OrderSetting');
const ProcessSetting = () =>
    import ( /* webpackChunkName: "group-plan" */ '../pages/plan/details/ProcessSetting');
const ProcessGroupSetting = () =>
    import ( /* webpackChunkName: "group-plan" */ '../pages/plan/details/ProcessGroupSetting');
const LineSetting = () =>
    import ( /* webpackChunkName: "group-plan" */ '../pages/plan/details/LineSetting');
const CapacitySetting = () =>
    import ( /* webpackChunkName: "group-plan" */ '../pages/plan/details/CapacitySetting');
const PlanSetting = () =>
    import ( /* webpackChunkName: "group-plan" */ '../pages/plan/details/PlanSetting');
const PlanLogs = () =>
    import ( /* webpackChunkName: "group-plan" */ '../pages/plan/details/PlanLogs');
/*基础配置*/
const SettingMain = () =>
    import ( /* webpackChunkName: "group-setting" */ "../pages/setting/SettingMain");
const FactorySetting = () =>
    import ( /* webpackChunkName: "group-setting" */ "../pages/setting/details/FactorySetting");
const WorkshopSetting = () =>
    import ( /* webpackChunkName: "group-setting" */ "../pages/setting/details/WorkshopSetting");
const SiteSetting = () =>
    import ( /* webpackChunkName: "group-setting" */ "../pages/setting/details/SiteSetting");
const CustomerSetting = () =>
    import ( /* webpackChunkName: "group-setting" */ "../pages/setting/details/CustomerSetting");
const ModelSetting = () =>
    import ( /* webpackChunkName: "group-setting" */ "../pages/setting/details/ModelSetting");
const ProductSetting = () =>
    import ( /* webpackChunkName: "group-setting" */ "../pages/setting/details/ProductSetting");
/*E_SOP*/
const ESopMain = () =>
    import ( /* webpackChunkName: "group-e-sop" */ "../pages/e_sop/ESopMain");
const FileManager = () =>
    import ( /* webpackChunkName: "group-e-sop" */ "../pages/e_sop/details/FileManager");
const NoticeManager = () =>
    import ( /* webpackChunkName: "group-e-sop" */ "../pages/e_sop/details/NoticeManager");
const ESopActionLog = () =>
    import ( /* webpackChunkName: "group-e-sop" */ "../pages/e_sop/details/ESopActionLog");
const ESopLogs = () =>
    import ( /* webpackChunkName: "group-e-sop" */ "../pages/e_sop/details/ESopLogs");
const SiteInfo = () =>
    import ( /* webpackChunkName: "group-e-sop" */ "../pages/e_sop/details/SiteInfo");
const StaffAssign = () =>
    import ( /* webpackChunkName: "group-plan" */ '../pages/e_sop/details/StaffAssign');
/*看板*/
const Dashboard = () =>
    import ('../pages/dashboard/Dashboard');
/*修改密码*/
const EditPassword = () =>
    import ('../pages/user/EditPassword');
/*函数*/
import { permissionList } from "../utils/utils";

Vue.use(Router);

const index = new Router({
    base: '/mes_system',
    routes: [{
            path: '/_empty'
        },

        {
            path: '/',
            component: Main,
            redirect: '/table',
            meta: {
                requireAuth: true
            },
            children: [{
                    path: '/table',
                    name: 'Table',
                    component: TableMain,
                    children: [{
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
                    children: [{
                        path: 'test_manage',
                        component: TestManage
                    }]
                },
                {
                    path: '/func',
                    name: 'Func',
                    component: FuncMain,
                    children: [{
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
                    children: [{
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
                    children: [{
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
                            path: 'file_type',
                            component: FileType
                        },
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
                        },
                        {
                            path: 'log',
                            component: PlanLogs
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
                },
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

if (sessionStorage.getItem('factory')) {
  store.commit('setSessionFactory', sessionStorage.getItem('factory'))
}
if (sessionStorage.getItem('UserType')) {
    store.commit('setUserType', sessionStorage.getItem('UserType'))
}
if (sessionStorage.getItem('token')) {
    store.commit('setLoginToken', sessionStorage.getItem('token'))
}
if (sessionStorage.getItem('delPermission')) {
    store.commit('setDelPermission', sessionStorage.getItem('delPermission').split(','))
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

index.afterEach((to, from) => {
    closeLoading();
});


export default index;
