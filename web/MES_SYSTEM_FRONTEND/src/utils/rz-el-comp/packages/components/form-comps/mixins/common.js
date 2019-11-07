export default  {
  methods: {
    /*向上响应输入内容*/
    setInputValue(value) {
      this.$emit('input', value)
    }
  }
}