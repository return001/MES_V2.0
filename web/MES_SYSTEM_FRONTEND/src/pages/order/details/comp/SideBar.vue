<!--配置功能统一导航-->
<template>
  <div class="mt-3 mb-3">
    <nav>
      <div class="sidebar-items">
        <!--订单配置-->
        <div class="sidebar-title">
          <a class="subtitle" draggable="false" data-toggle="collapse" href="#collapsePenetrate" aria-expanded="false"
             aria-controls="collapsePenetrate">订单管理</a>
        </div>
        <div class="collapse show" id="collapsePenetrate">
          <div @click="toggleState('orderManage')">
            <div class="sidebar-link" @click="linkTo({link: '/order/order_manage', type: 'order_manage'})" :class="activeItem === 'orderManage' ? 'active' : ''">订单管理</div>
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

        //控制列表active状态，当前已激活的项目
        activeItem: ""

      }
    },
    mounted: function () {

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
        if (this.$route.fullPath !== (obj.link + '?type=' + obj.type)) {
          //this.setTableRouter(obj.type);
          this.$openLoading();
          this.$router.push({
            path: obj.link,
            query: {
              type: obj.type
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

  .sidebar {
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
