<template>
  <div id="app">
    <router-view/>
    <copyright v-if="$route.path !== '/dashboard'"/>
  </div>

</template>

<script>
  import eventBus from './utils/eventBus'
  import Copyright from "./components/Copyright";
  export default {
    name: 'App',
    components: {
      Copyright
    },
    data() {
      return {
      }
    },
    mounted: function () {
      console.log(process.env.NODE_ENV)
    }

  }
</script>

<style>
  #app {
    height: 100%;
  }

  body {
    background: url("assets/background.png") repeat;
    margin: 0;
  }

  html, body {
    height: 100%;
  }


  *::-webkit-scrollbar {
    width: 10px;
    height: 10px;

  }

  *::-webkit-scrollbar-thumb {
    border-radius: 10px;
    /*-webkit-box-shadow: inset 0 0 5px rgba(0, 0, 0, 0.2);*/
    background: rgba(225, 225, 225, 0.93);
  }

  *::-webkit-scrollbar-track {
    /*-webkit-box-shadow: inset 0 0 5px rgba(0, 0, 0, 0.2);*/
    border-radius: 10px;
    background: rgba(246, 246, 246, 0.59);
  }
</style>
