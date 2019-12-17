<!--订单配置页面根组件-->
<template>
  <div id="order-main" v-if="isRouterAlive">
    <options/>
    <table-details/>
    <edit-panel/>
    <!--<create-relation/>-->
  </div>
</template>

<script>
  import Options from './comp/Options'
  import TableDetails from './comp/TableDetails'
  import EditPanel from './comp/EditPanel'
  import CreateRelation from './comp/CreateRelation'
  import {mapGetters} from 'vuex'

  export default {
    name: "OrderManage",
    provide() {
      return {
        reload: this.reload
      }
    },
    data() {
      return {
        isRouterAlive: true
      }
    },
    computed: {
      ...mapGetters(['isEditing', 'editData'])
    },
    components: {
      Options,
      TableDetails,
      EditPanel,
      CreateRelation
    },

    methods: {
      reload: function () {
        this.isRouterAlive = false;
        this.$nextTick(function () {
          this.isRouterAlive = true;
        })
      }
    }
  }
</script>

<style scoped>
  #order-main {
    padding: 20px 20px 10px 80px;
  }

  .fade-enter-active, .fade-leave-active {
    transition: opacity .5s;
  }
  .fade-enter, .fade-leave-to {
    opacity: 0;
  }
</style>
