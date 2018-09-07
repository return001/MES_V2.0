<template>
  <div class="redTea">
    <div class="form-area col">
      <div class="options-area">
        <div class="form-row">
          <div class="form-group">
            <label for="sn">SN号</label>
            <input type="text" name="sn" id="sn" class="form-control" v-model="sn">
          </div>
          <div class="form-group">
            <label for="imei">IMEI号</label>
            <input type="text" name="imei" id="imei" class="form-control" v-model="imei">
          </div>
          <div class="form-group">
            <label for="deviceModel">机型</label>
            <input type="text" name="deviceModel" id="deviceModel" class="form-control" v-model="deviceModel">
          </div>
          <div class="form-group row align-items-end">
            <div class="btn btn-secondary ml-3" @click="resetForm">清空条件</div>
          </div>
          <div class="form-group row align-items-end">
            <button type="button" class="btn btn-primary ml-3 mr-4" @click="getCId">查询</button>
          </div>
        </div>
      </div>
      <div class="result">
        <div class="form-group">
          <label for="cId">CId</label>
          <input type="text" name="cId" id="cId" class="form-control" v-model="cId" disabled="disabled">
        </div>
      </div>
    </div>
  </div>
</template>

<script>
  import {axiosFetch} from "../../utils/fetchData";
  import {redTeaUrl} from '../../config/globalUrl'
  export default {
    name: "RedTea",
    data() {
      return {
        sn:"",
        imei:"",
        deviceModel:"",
        cId:""
      }
    },
    methods: {
      resetForm:function(){
        this.sn = "";
        this.imei = "";
        this.deviceModel = "";
      },
      getCId:function(){
        let sn = this.sn;
        let imei = this.imei;
        let deviceModel = this.deviceModel
        if(sn != "" || imei !="" || deviceModel != ""){
          let options = {
            url: redTeaUrl,
            data:{
              sn:this.sn,
              imei:this.imei,
              deviceModel:this.deviceModel
            }
          };
          let that = this;
          axiosFetch(options).then(response => {
            console.log("jksahdcaksjch");
            if(response.data){
             let result = response.data.result;
              if(result === 200){
                let data = response.data.data;
                let obj = JSON.parse(data);
                if(obj.errorMessage !== "成功"){
                  that.cId = "";
                  alert(obj.errorMessage);
                }else{
                  let cId = obj.data.cid;
                  that.cId = cId
                }
              }else{
                alert("请求失败，请检查参数是否正确");
              }
            }
          })
          .catch(err => {
            console.log(err);
          })
        }else{
          alert("参数不能为空");
        }
      }
    }
  }
</script>

<style scoped>
  .redTea {
    position: absolute;
    height: 100%;
    width: 100%;
  }

  .form-area {
    position: relative;
    margin-left: 60px;
    width: calc(100% - 60px);
    padding: 40px 60px;
  }

  .options-area {
    background: #fff;
    border: 1px solid #eeeeee;
    border-radius: 8px;
    padding: 10px;
  }
  .form-group{
    margin-left:10px;
  }
  .result{
    width:30%;
    min-width:300px;
    margin:100px auto;
    padding:10px 20px;
    background:#fff;
    border-radius: 8px;
    border: 1px solid #eeeeee;
  }
</style>
