module.exports = {
  publicPath: '/mes_system/',
  productionSourceMap: false,
  runtimeCompiler: true,

  chainWebpack: config => {
    config.optimization.splitChunks({
      chunks: 'all'
    })
  }
}