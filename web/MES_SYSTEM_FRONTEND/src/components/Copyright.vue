<template>
  <div id="copyright">
    <div class="text-container" v-if="offsetDistance > 500">
      <p>Copyright © 2018 {{(new Date()).getFullYear() === 2018 ? '' : ('- ' + new
        Date().getFullYear())}}</p>
      <span><img src="@/assets/JimiLogo.jpg" id="JimiLogo" draggable="false"></span>
      <p>All rights reserved.</p>
    </div>
  </div>
</template>

<script>
  export default {
    name: "Copyright",
    data() {
      return {
        offsetDistance: 0
      }
    },
    mounted() {
      this.$nextTick(function () {
        this.offsetDistance = document.getElementById('copyright').offsetTop
      })
    },
    watch: {
      $route: function (val) {
        this.$nextTick(function () {
          this.offsetDistance = document.getElementById('copyright').offsetTop
        })
      }
    }
  }
</script>

<style scoped>
  #copyright {
    position: relative;
    width: 100%;
    display: flex;
    justify-content: center;
    bottom: 10px;
  }

  .text-container {
    padding: 0 10px;
    border-radius: 20px;
    background-color: #fff;
    margin: 10px 0;
    line-height: 30px;
    font-size: 14px;
    border: 1px solid #eeeeee;
    display: flex;
    align-items: center;
    justify-content: center;
    height: 30px;
  }

  #JimiLogo {
    margin-top: -4px;
    height: 26px;
    display: block;
  }

</style>
