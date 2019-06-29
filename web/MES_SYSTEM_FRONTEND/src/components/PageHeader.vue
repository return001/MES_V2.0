<!--顶栏组件-->

<template>
  <div class="navbar">
    <div class="container">
      <div class="navbar-brand">
        <img src="/mes_system/static/img/logo.png" class="nav-logo" draggable="false">
      </div>
      <span class="navbar-text" draggable="false">智能生产控制系统</span>
    </div>
  </div>
</template>

<script>
  export default {}
</script>

<style scoped>
  .navbar {
    background: #3875ff;
  }

  .container {
    display: flex;
    width: 400px;
    height: 58px;
  }

  .navbar-brand {
    margin: 0 6px;
  }

  .navbar-brand, .navbar-brand img {
    width: 54px;
  }

  .navbar-text {
    color: #ffffff;
    line-height: 58px;
    font-size: 24px;
  }
</style>
