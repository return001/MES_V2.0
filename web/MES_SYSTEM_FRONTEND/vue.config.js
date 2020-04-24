module.exports = {
  publicPath: '/mes_system/',
  productionSourceMap: false,
  runtimeCompiler: true,

  /*chainWebpack: config => {
    config.plugins.delete('prefetch');

/!*    config.optimization.splitChunks({
      chunks: 'all'
    })*!/

  }*/
};
