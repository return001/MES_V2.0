<!--配置功能统一导航-->
<template>
  <div id="test-sidebar">
    <div class="sidebar-items">
      <!--订单配置-->
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
  </div>
</template>

<script>
  import {mapGetters, mapActions} from 'vuex'
  import {testType} from "../../../../store/getters";

  export default {
    data() {
      return {
        linkList: [
          {
            title: 'SMT测试',
            subList: [
              {
                type: '0',
                name: '功能测试'
              },
            ]
          },
          {
            title: '组装测试',
            subList: [
              {
                type: '1',
                name: '功能测试'
              },
              {
                type: '2',
                name: '耦合测试'
              }
            ]
          },
          {
            title: '研发测试',
            subList: [
              {
                type: '3',
                name: '功能测试'
              },
              {
                type: '4',
                name: '耦合测试'
              }
            ]
          },
          {
            title: 'OQC',
            subList: [
              {
                type: '5',
                name: 'OQC'
              }
            ]
          }
        ],
        //控制列表active状态，当前已激活的项目
        activeItem: ""

      }
    },
    watch: {
      $route: function (val) {
        this.toggleState(this.testType)
      }
    },
    mounted: function () {
      this.toggleState(this.testType)
    },
    computed: {
      ...mapGetters([
        'testType',
        'isLoading'
      ]),
    },
    methods: {
      ...mapActions(['setTestType']),
      toggleState: function (item) {
        this.activeItem = item;

      },
      linkTo: function (val) {
        if (val !== this.$route.query.type) {
          this.$openLoading();
          this.setTestType(val);
          this.$router.push({
            path: '/test/test_manage',
            query: {
              type: val
            }
          })
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

  #test-sidebar {
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
    padding-left: 0.5em;
    font-size: 0.85em;
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
