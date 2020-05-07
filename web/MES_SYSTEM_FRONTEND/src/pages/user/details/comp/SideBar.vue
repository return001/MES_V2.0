<template>
  <div id="plan-sidebar">
    <nav>
      <div class="sidebar-items">
        <div v-for="item in linkListValidated">
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
  import {mapGetters} from 'vuex'

  export default {
    name: "SideBar",
    data() {
      return {
        linkList: [
          {
            title: '用户管理',
            moduleName: 'user',
            subList: [
              {
                type: 'dept',
                name: '部门管理'
              },
              {
                type: 'char',
                name: '角色管理'
              },
              {
                type: 'users',
                name: '用户管理'
              },
              {
                type: 'action_log',
                name: '日志'
              },
            ]
          }
        ],
        linkListValidated: [],
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
    computed: {
      ...mapGetters([
        'charactersFuncMap'
      ]),
    },
    beforeMount() {
      /*根据权限初始化页面列表*/
      const pageList = this.charactersFuncMap.pageList;
      this.linkListValidated = this.linkList.map(item => {
        let _subList = item.subList.filter(subItem => {
          return pageList.indexOf(`user-${item.moduleName}-${subItem.type}`) >= 0;
        });
        if (_subList.length > 0) {
          item.subList = _subList;
          return item
        }
      }).filter(item => item !== undefined);
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
        if (!routerState[2] || val !== routerState[2]) {
          //this.$openLoading();
          this.$router.push({
            path: '/user/' + val
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
