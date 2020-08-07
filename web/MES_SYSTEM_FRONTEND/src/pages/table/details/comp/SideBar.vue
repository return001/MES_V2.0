<!--表单查看页面的统一侧边栏导航-->

<template>
  <div class="sidebar">
    <div class="sidebar-items">
      <!--订单配置-->
      <div v-for="item in linkListValidated">
        <div class="sidebar-title">
          <span class="subtitle" draggable="false">{{item.title}}</span>
        </div>
        <div @click="toggleState(subItem.type || subItem.activeKey)" v-for="subItem in item.subList">
          <div class="sidebar-link" @click="linkTo(subItem)"
               :class="activeItem === (subItem.type || subItem.activeKey) ? 'active' : ''">
            {{subItem.name}}
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
  import {mapGetters} from 'vuex'

  export default {
    data() {
      return {
        linkList: [
          {
            title: '数据表',
            moduleName: 'table',
            subList: [
              {
                type: "DataRelativeSheet",
                name: "AMS数据关联表"
              },
              {
                type: "DataRelativeUnique",
                name: "终端信息附表(工厂)"
              },
              {
                type: "DataRelativeUpdate",
                name: "终端信息附表(关联更新)"
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
                type: "GPSOQC_Result",
                name: "OQC测试结果"
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
            ]
          },
          {
            title: '多表查询',
            moduleName: 'multi',
            subList: [
              {
                activeKey: 'multi',
                name: '多表查询',
                path: '/multi',
                type: 'multi'
              }
            ]
          },
          {
            title: '备份',
            moduleName: 'multi',
            subList: [
              {
                activeKey: 'backups',
                name: '备份查询',
                path: '/backups',
                type: 'backup'
              }
            ]
          }
        ],
        //存储根据权限筛选后的列表
        linkListValidated: [],
        //控制列表active状态，当前已激活的项目
        activeItem: ""

      }
    },
    beforeMount() {
      /*根据权限初始化页面列表*/
      const pageList = this.charactersFuncMap.pageList;
      this.linkListValidated = this.linkList.map(item => {
        let _subList = item.subList.filter(subItem => {
          return pageList.indexOf(`table-${item.moduleName}-${subItem.type}`) >= 0;
        });
        if (_subList.length > 0) {
          item.subList = _subList;
          return item
        }
      }).filter(item => item !== undefined);
    },
    mounted: function () {
      if (this.$route.query.type) {
        this.toggleState(this.$route.query.type);
      }
      if (this.$route.path === '/table/backups') {
        this.toggleState('backups');
      } else if (this.$route.path === '/table/multi') {
        this.toggleState('multi');
      }
    },
    watch: {
      $route: function (val) {
        if (val.query.type) {
          this.toggleState(val.query.type);
        }
        if (val.path === '/table/backups') {
          this.toggleState('backups');
        } else if (val.path === '/table/multi') {
          this.toggleState('multi');
        }
      }
    },
    computed: {
      ...mapGetters([
        'isLoading',
        'charactersFuncMap'
      ]),
    },
    methods: {
      toggleState: function (item) {
        this.activeItem = item;
      },
      linkTo: function (obj) {
        if ((obj.path && obj.type) || (!obj.path && obj.type)) {
          if (this.$route.query.type !== obj.type) {
            this.$router.push({
              path: '/table' + (obj.path || '/details'),
              query: {
                type: obj.type
              }
            })
          }
        } else if (obj.path && !obj.type) {
          this.$router.push('/table' + obj.path)
        }
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

</style>
