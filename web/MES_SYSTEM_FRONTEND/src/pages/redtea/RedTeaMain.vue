<template>
  <div class="redTea">
    <div class="form-area col">
      <div class="options-area">
        <div class="form-row">
          <div class="form-group">
            <label for="sn">SN号</label>
            <el-input type="text" name="sn" id="sn" class="form-control" v-model="sn" clearable></el-input>
          </div>
          <div class="form-group">
            <label for="imei">IMEI号</label>
            <el-input type="text" name="imei" id="imei" class="form-control" v-model="imei" clearable></el-input>
          </div>
          <div class="form-group">
            <label for="deviceModel">机型</label>
            <el-input type="text" name="deviceModel" id="deviceModel" class="form-control" v-model="deviceModel"
                      clearable></el-input>
          </div>
          <div class="form-group-btn">
            <el-button type="primary" @click="getCId">查询</el-button>
          </div>
        </div>
      </div>
      <div class="result">
        <label for="cId">CId</label>
        <el-input type="text" name="cId" id="cId" class="form-control" v-model="cId" disabled="disabled"></el-input>
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
        sn: "",
        imei: "",
        deviceModel: "",
        cId: ""
      }
    },
    methods: {
      resetForm: function () {
        this.sn = "";
        this.imei = "";
        this.deviceModel = "";
      },
      getCId: function () {
        let sn = this.sn;
        let imei = this.imei;
        let deviceModel = this.deviceModel
        if (sn != "" || imei != "" || deviceModel != "") {
          let options = {
            url: redTeaUrl,
            data: {
              sn: this.sn,
              imei: this.imei,
              deviceModel: this.deviceModel
            }
          };
          let that = this;
          axiosFetch(options).then(response => {
            if (response.data) {
              let result = response.data.result;
              if (result === 200) {
                let data = response.data.data;
                let obj = JSON.parse(data);
                if (obj.errorMessage !== "成功") {
                  that.cId = "";
                  this.$alertWarning(obj.errorMessage);
                } else {
                  let cId = obj.data.cid;
                  that.cId = cId
                }
              } else {
                this.$alertWarning("请求失败，请检查参数是否正确");
              }
            }
          })
            .catch(err => {
              console.log(err);
            })
        } else {
          this.$alertWarning("参数不能为空");
        }
      }
    }
  }
</script>

<style scoped>
  .redTea {
    position: absolute;
    box-sizing: border-box;
  }

  .form-area {
    position: relative;
    width: 100% ;
  }

  .options-area {
    background: #fff;
    border: 1px solid #eeeeee;
    border-radius: 8px;
    padding: 10px;
  }

  .form-row {
    display: flex;
    flex-wrap: wrap;
    align-items: flex-end;
  }

  .form-group {
    margin-left: 10px;
  }

  .result {
    width: 30%;
    min-width: 300px;
    margin: 100px auto;
    padding: 10px 20px;
    background: #fff;
    border-radius: 8px;
    border: 1px solid #eeeeee;
  }

  .form-group {
    width: 200px;
    margin-right: 20px;
  }

  .form-group /deep/ label {
    line-height: 40px;
  }

  .result label {
    line-height: 40px;
  }

  .form-group-btn {
    width: 120px;
    margin-right: 20px;
  }

  .form-group-btn .el-button {
    width: 100%;
    margin-top: 10px;
  }
</style>
