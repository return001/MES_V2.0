<!--配置功能统一导航-->
<template>
  <div id="test-sidebar">
    <nav>
      <div class="sidebar-items">
        <!--订单配置-->
        <div class="sidebar-title">
          <a class="subtitle" draggable="false" data-toggle="collapse" href="#collapseTest0" aria-expanded="false"
             aria-controls="collapseTest0">SMT测试</a>
        </div>
        <div class="collapse show" id="collapseTest0">
          <div @click="toggleState(item.type)" v-for="item in linkList0">
            <div class="sidebar-link" @click="linkTo(item.type)" :class="activeItem === item.type ? 'active' : ''">
              {{item.name}}
            </div>
          </div>
        </div>
        <div class="sidebar-title">
          <a class="subtitle" draggable="false" data-toggle="collapse" href="#collapseTest1" aria-expanded="false"
             aria-controls="collapseTest1">组装测试</a>
        </div>
        <div class="collapse show" id="collapseTest1">
          <div @click="toggleState(item.type)" v-for="item in linkList1">
            <div class="sidebar-link" @click="linkTo(item.type)" :class="activeItem === item.type ? 'active' : ''">
              {{item.name}}
            </div>
          </div>
        </div>
        <div class="sidebar-title">
          <a class="subtitle" draggable="false" data-toggle="collapse" href="#collapseTest2" aria-expanded="false"
             aria-controls="collapseTest2">研发测试</a>
        </div>
        <div class="collapse show" id="collapseTest2">
          <div @click="toggleState(item.type)" v-for="item in linkList2">
            <div class="sidebar-link" @click="linkTo(item.type)" :class="activeItem === item.type ? 'active' : ''">
              {{item.name}}
            </div>
          </div>
        </div>
        <div class="sidebar-title">
          <a class="subtitle" draggable="false" data-toggle="collapse" href="#collapseTest3" aria-expanded="false"
             aria-controls="collapseTest3">OQC</a>
        </div>
        <div class="collapse show" id="collapseTest3">
          <div @click="toggleState(item.type)" v-for="item in linkList3">
            <div class="sidebar-link" @click="linkTo(item.type)" :class="activeItem === item.type ? 'active' : ''">
              {{item.name}}
            </div>
          </div>
        </div>
      </div>
    </nav>

  </div>
</template>

<script>
  import {mapGetters, mapActions} from 'vuex'
  import {testType} from "../../../../store/getters";

  export default {
    data() {
      return {

        linkList0: [
          {
            type: '0',
            name: '功能测试'
          },
        ],
        linkList1: [
          {
            type: '1',
            name: '功能测试'
          },
          {
            type: '2',
            name: '耦合测试'
          }
        ],
        linkList2: [
          {
            type: '3',
            name: '功能测试'
          },
          {
            type: '4',
            name: '耦合测试'
          }
        ],
        linkList3: [
          {
            type: '5',
            name: 'OQC'
          }
        ],


        //控制列表active状态，当前已激活的项目
        activeItem: ""

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
      ...mapActions(['setTableRouter', 'setLoading', 'setTestType']),
      toggleState: function (item) {
        this.activeItem = item;

      },
      linkTo: function (val) {
        if (val !== this.$route.query.type){
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
