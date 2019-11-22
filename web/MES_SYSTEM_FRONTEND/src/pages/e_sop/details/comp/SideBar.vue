<template>
  <div id="plan-sidebar">
    <nav>
      <div class="sidebar-items">
        <div v-for="item in linkList">
          <div class="sidebar-title">
            <span class="subtitle" draggable="false">{{item.title}}</span>
          </div>
          <div @click="toggleState(subItem.type)" v-for="subItem in item.subList">
            <div class="sidebar-link" @click="linkTo(subItem.type)"
                 :class="activeItem === subItem.type ? 'active' : ''">
              {{subItem.name}}
            </div>
          </div>
        </div>
      </div>
    </nav>

  </div>
</template>

<script>
  import router from "../../../../router";

  export default {
    name: "SideBar",
    data() {
      return {
        linkList: [
          {
            title: '文件发放',
            subList: [
              {
                type: 'file',
                name: '文件管理&发放'
              },
              {
                type: 'notice',
                name: '通知管理'
              },
              {
                type: 'site',
                name: '站点信息'
              },
              {
                type: 'action_log',
                name: '操作日志'
              },
              {
                type: 'confirm_log',
                name: '确认日志'
              },
              {
                type: 'notice_log',
                name: '通知日志'
              },
            ]
          },
          {
            title: '客户端信息',
            subList: [
              {
                type: 'login_log',
                name: '日志'
              },
              {
                type: 'count_log',
                name: '产能记录'
              },
            ]
          }
        ],
        activeItem: '',
      }
    },
    watch: {
      $route: function (val) {
        let routerState = val.path.split('/');
        if (!!routerState[2]) {
          this.toggleState(routerState[2])
        } else {
          this.toggleState('')
        }
      }
    },
    mounted() {
      let routerState = this.$route.path.split('/');
      if (!!routerState[2]) {
        this.toggleState(routerState[2])
      } else {
        this.toggleState('')
      }
    },
    methods: {
      toggleState: function (item) {
        this.activeItem = item;
      },
      linkTo: function (val) {
        let routerState = this.$route.path.split('/');
        if (!!routerState[2] || val !== routerState[2]) {
          //this.$openLoading();
          this.$router.push({
            path: '/esop/' + val
          })
        }
      }
    }
  }
</script>

<style scoped>

  #plan-sidebar {
    margin: 20px 0;
    width: 160px;
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
    height: 40px;
    line-height: 40px;
    font-size: 20px;
    font-weight: 500;
    padding-left: 10px;
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
    height: 30px;
    line-height: 30px;
    padding-left: 16px;
    font-size: 16px;
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
