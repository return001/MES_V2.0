<!--表单查看页面的统一侧边栏导航-->

<template>
  <div class="sidebar">
    <nav>
      <div class="sidebar-items">
        <div class="sidebar-title">
          <a class="subtitle" draggable="false" data-toggle="collapse" href="#collapseDatatable" aria-expanded="false"
             aria-controls="collapseDatatable">数据表</a>
        </div>
        <div class="collapse show" id="collapseDatatable">
          <div v-for="data in database" @click="toggleState(data.type)">
            <div class="sidebar-link" @click="linkTo(data)" :class="activeItem === data.type ? 'active' : ''">
              {{data.name}}
            </div>
          </div>
        </div>
        <div class="sidebar-title">
          <a class="subtitle" draggable="false" data-toggle="collapse" href="#collapseMulti" aria-expanded="false"
             aria-controls="collapseMulti">多表查询</a>
        </div>
        <div class="collapse show" id="collapseMulti">
          <div @click="toggleState('multi')">
            <div class="sidebar-link" @click="routerPush('/multi')" :class="activeItem === 'multi' ? 'active' : ''">
              多表查询
            </div>
          </div>
        </div>
        <div class="sidebar-title">
          <a class="subtitle" draggable="false" data-toggle="collapse" href="#collapseBackup" aria-expanded="false"
             aria-controls="collapseBackup">备份</a>
        </div>
        <div class="collapse show" id="collapseBackup">
          <div @click="toggleState('backups')">
            <div class="sidebar-link" @click="routerPush('/backups')" :class="activeItem === 'backups' ? 'active' : ''">
              备份查询
            </div>
          </div>
        </div>
      </div>
    </nav>
  </div>
</template>

<script>
  import {mapGetters, mapActions} from 'vuex'

  export default {
    data() {
      return {
        database: [
          {
            type: "DataRelativeSheet",
            name: "AMS数据关联表"
          },
          {
            type: "DataRelativeUnique",
            name: "数据关联表(工厂)"
          },
          {
            type: "DataRelativeUpdate",
            name: "数据关联表(关联更新)"
          },
          {
            type: "Gps_AutoTest_Result",
            name: "组装功能表"
          },
          {
            type: "Gps_AutoTest_Result2",
            name: "SMT功能表"
          },
          {
            type: "Gps_AutoTest_Result3",
            name: "老化后功能表"
          },
          {
            type: "Gps_CartonBoxTwenty_Result",
            name: "卡通箱表"
          },
          {
            type: "Gps_CoupleTest_Result",
            name: "耦合表"
          },
          {
            type: "Gps_ManuCpParam",
            name: "比对表"
          },
          {
            type: "Gps_ManuPrintParam",
            name: "机身彩盒打印记录表",
            path: '/details_sp'
          },
          {
            type: "NetMarkIMEI",
            name: "网标与IMEI绑定表"
          },
          {
            type: "Gps_ManuSimDataParam",
            name: "种子物联网卡表",
            path: '/details_sp'
          },
          {
            type: "LTestLogMessage",
            name: "错误日志"
          },
          {
            type: "Gps_TestResult",
            name: "总表"
          }
        ],
        //控制列表active状态，当前已激活的项目
        activeItem: ""

      }
    },
    mounted: function () {
      this.toggleState(this.$route.query.type);
      if (this.$route.path === '/table/backups') {
        this.toggleState('backups');
      }
    },
    watch: {
      $route: function (val) {
        this.toggleState(this.$route.query.type);
        if (this.$route.path === '/table/backups') {
          this.toggleState('backups');
        }
      }
    },
    computed: {
      ...mapGetters([
        'tableRouterApi',
        'isLoading'
      ]),
    },
    methods: {
      ...mapActions(['setTableRouter', 'setLoading']),
      toggleState: function (item) {
        this.activeItem = item;
      },
      linkTo: function (obj) {
        let path = obj.path ? obj.path : '/details';
        if (this.$route.query.type !== obj.type) {
          this.setTableRouter(obj.type);
          this.$router.push({
            path: '/table' + path,
            query: {
              type: obj.type
            }
          })
        }
      },
      routerPush: function (val) {
        this.$router.push('/table' + val)
      }
    }


  }
</script>

<style scoped>
  a {
    text-decoration: none;
    color: #000;
  }

  .sidebar {
    display: flex;
    flex-direction: column;
    height: 90%;
    margin: 20px 0;
  }

  .sidebar-items {
    /*border: 1px solid #eeeeee;*/
    /*border-top: none;*/
    /*border-bottom: none;*/
    border: none;
    height: 100%;
    /*border-radius: 8px;*/
  }

  .sidebar-title {
    height: 2em;
    line-height: 2em;
    font-size: 1.2em;
    font-weight: 500;
    padding-left: 0.5em;
    border-bottom: 1px solid #eeeeee;
    background-color: #458aff;
    color: #fff;
    border-radius: 8px;
  }

  .sidebar-title a {
    color: #fff;
  }

  .sidebar-link {
    text-decoration: none;
    display: block;
    height: 2em;
    line-height: 2em;
    padding-left: 1.4em;
    font-size: 1em;
    border-bottom: 1px solid #eeeeee;
    font-weight: normal;
    background: #fff;
    cursor: pointer;
  }

  .sidebar-link:hover {
    background-color: #8bdaff;
    color: #fff;
    border-radius: 5px;
  }

  .sidebar-items .active {
    background-color: #7bbfff;
    box-shadow: 2px 4px 10px 1px #e5e7eb;
    color: #fff;
    border-radius: 5px;
  }

  .subtitle {
    cursor: pointer;
    display: block;
    width: 100%;
    height: 100%;
  }

  .trash-bin-btn {
    cursor: pointer;
    width: 48px;
    height: 48px;
    border-radius: 8px;
    border: 1px solid #7bbfff;
    display: flex;
    justify-content: center;
    align-items: center;
    background-color: #7bbfff;
  }

  #trash-bin {
    align-self: flex-end;
    position: relative;
    width: 100%;
    margin-top: auto;
    display: flex;
    justify-content: center;
  }
</style>
