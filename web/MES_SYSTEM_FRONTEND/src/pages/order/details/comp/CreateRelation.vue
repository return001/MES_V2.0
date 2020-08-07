<template>
  <div class="rel-panel-container ">
    <div class="rel-panel">
      <div class="col-2 content-left">
        <h4 class="mb-4 mt-3">制单信息:</h4>
        <p>制单号：{{data.ZhiDan}}</p>
        <p>SIM位数/SIM前缀：{{data.SIM_digits}}/{{data.SIM_prefix}}</p>
        <p>VIP位数/VIP前缀：{{data.VIP_digits}}/{{data.VIP_prefix}}</p>
        <p>ICCID位数/ICCID前缀：{{data.ICCID_digits}}/{{data.ICCID_prefix}}</p>
        <p>MAC位数/MAC前缀：{{data.MAC_digits}}/{{data.MAC_prefix}}</p>
        <p>设备号位数/设备号前缀：{{data.Equipment_digits}}/{{data.Equipment_prefix}}</p>
        <p>BAT位数/BAT前缀：{{data.BAT_digits}}/{{data.BAT_prefix}}</p>
      </div>
      <div class="col-10">
        <h2 class="mb-4 mt-3">绑定关系:</h2>
        <div class="dropdown-divider"></div>
        <div class="select-box row">
          <div class="form-check  col-sm-6 col-md-4 col-lg-2" v-for="(item, index) in relationData">
            <input type="checkbox" class="form-check-input" :value="item.key" :id="'select-item-'+ index"
                   v-model="chosenKey" @input="formatCheckbox">
            <label :for="'select-item-'+ index" class="form-check-label">{{item.name}}</label>
          </div>
        </div>
        <div class="dropdown-divider"></div>
        <div class="form-group pl-3 pr-3">
          <div class="col-sm-6 col-md-4">
            <label for="input-item-imei" class="col-form-label">请输入IMEI号:</label>
            <input type="text" class="form-control" v-model="IMEIData" id="input-item-imei" @keyup.enter="jumpToNext">
            <span class="form-span" v-if="IMEIData.length === 0">*此项必填</span>
          </div>
        </div>
        <div class="form-group input-box row">
          <div class=" col-sm-6 col-md-4" v-for="(item, index) in relationData"
               v-if="chosenKey.indexOf(item.key) !== -1">
            <label :for="'input-item-'+ index" class="col-form-label">请输入{{item.name}}:</label>
            <input type="text" class="form-control" :id="'input-item-'+ index" v-model="item.value"
                   @keyup.enter="inputHandler(index)" @input="validate(item.type, item.value)">
            <span class="form-span">{{warningMsg[item.type]}}</span>
          </div>
        </div>
        <div class="dropdown-divider"></div>

        <div class="form-row justify-content-around">
          <button type="button" class="btn btn-secondary col-2 text-white" @click="closePanel">取消</button>
          <button type="button" class="btn btn-primary col-2 text-white" @click="postData">提交</button>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
  import Bus from '@/utils/eventBus'
  import {relationUpdateUrl} from "../../../../config/globalUrl";
  import {axiosFetch} from "../../../../utils/fetchData";

  export default {
    name: "CreateRelation",
    props: ['data'],
    data() {
      return {
        IMEIData: '',
        relationData: [
          {
            key: 'iMEI3',
            name: 'SIM号',
            type: 'SIM',
            value: ''
          },
          {
            key: 'iMEI4',
            name: 'ICCID号',
            type: 'ICCID',
            value: ''
          },
          {
            key: 'iMEI8',
            name: 'VIP号',
            type: 'VIP',
            value: ''
          },
          {
            key: 'iMEI9',
            name: 'BAT号',
            type: 'BAT',
            value: ''
          },
          {
            key: 'iMEI6',
            name: '蓝牙号',
            type: 'MAC',
            value: ''
          },
          {
            key: 'iMEI7',
            name: '设备号',
            type: 'Equipment',
            value: ''
          }
        ],
        chosenKey: [],
        isPending: false,
        warningMsg: {}
      }
    },
    mounted() {
    },
    methods: {
      closePanel: function () {
        Bus.$emit('closeRelPanel', false)
      },
      clearData: function () {
        for (let i in this.relationData) {
          this.relationData[i].value = '';
          this.warningMsg = {}
        }
      },
      formatCheckbox: function (e) {
        let isChecked = e.target.checked;
        let val = e.target.value;
        this.clearData();
        if (val === 'iMEI3' && this.chosenKey.indexOf('iMEI4') !== -1 && isChecked === true) {
          this.chosenKey.splice(this.chosenKey.indexOf('iMEI4'), 1);
        }
        if (val === 'iMEI4' && this.chosenKey.indexOf('iMEI3') !== -1 && isChecked === true) {
          this.chosenKey.splice(this.chosenKey.indexOf('iMEI3'), 1);
        }
      },
      jumpToNext: function () {
        let index;
        for (let i in this.relationData) {
          if (this.chosenKey.indexOf(this.relationData[i].key) !== -1) {
            index = i;
            document.getElementById('input-item-' + i).focus();
            return;
          }
        }
        if (!index) {
          this.postData()
        }
      },
      inputHandler: function (index) {
        //用于回车自动跳转光标
        if (index < this.chosenKey.length) {
          document.getElementById('input-item-' + (index + 1)).focus();
        } else if (index === this.chosenKey.length) {
          this.postData()
        }
      },
      postData: function () {
        //用于判断所填数值是否有含空项
        let isEmpty = false;
        for (let i in this.relationData) {
          for (let j in this.chosenKey) {
            if (this.relationData[i].key === this.chosenKey[j] && this.relationData[i].value === '') {
              isEmpty = true;
            }
          }
        }
        //用于判断是否存在异常数据
        let isWarning = false;
        if (JSON.stringify(this.warningMsg) !== '{}') {
          for (let i in this.warningMsg) {
            if (this.warningMsg[i] !== '') {
              isWarning = true
            }
          }
        }
        if (this.IMEIData === '' || isEmpty) {
          this.$alertWarning('存在不能为空项目');
          return;
        } else if (isWarning) {
          this.$alertWarning('存在格式异常，请检查格式');
          return;
        }
        if (!this.isPending) {
          //this.$openLoading();
          let options = {
            url: relationUpdateUrl,
            data: {
              zhiDan: this.data.ZhiDan,
              iMEI1: this.IMEIData
            }
          };
          for (let i in this.relationData) {
            for (let j in this.chosenKey) {
              if (this.relationData[i].key === this.chosenKey[j]) {
                options.data[this.relationData[i].key] = this.relationData[i].value
              }
            }
          }
          axiosFetch(options).then(response => {
            this.$closeLoading();
            this.isPending = false;
            if (response.data.result === 200){
              this.$alertSuccess("添加成功");
              this.IMEIData = '';
              this.clearData();
              document.getElementById('input-item-imei').focus()
            } else if (response.data.result === 412){
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.$closeLoading();
            this.isPending = false;
            console.log(JSON.stringify(err));
            this.$alertDanger('请求超时，清刷新重试')
          })

        }
      },
      validate: function (type, val) {
        if (val !== '') {
          if (val.indexOf(this.data[type + '_prefix']) !== 0) {
            this.warningMsg[type] = '格式有误(前缀与设定值不符)';
          } else if (val.length > 0 && val.length.toString() !== (this.data[type + '_digits'])) {
            this.warningMsg[type] = '格式有误(总长与设定值不符)';
          } else {
            this.warningMsg[type] = '';
          }
        } else {
          this.warningMsg[type] = '';
        }
      }
    }

  }
</script>

<style scoped>
  .rel-panel-container {
    position: fixed;
    /*display: flex;*/
    /*align-items: center;*/
    /*justify-content: center;*/
    /*height: 100%;*/
    padding: 20px;
    width: 100%;
    height: 100%;
    left: 0;
    top: 0;
    background: rgba(0, 0, 0, 0.1);
    z-index: 101;
  }

  .rel-panel {
    background: #ffffff;
    height: 100%;
    /*width: 100%;*/
    z-index: 102;
    border-radius: 10px;
    box-shadow: 3px 3px 20px 1px #bbb;
    padding: 30px 30px 10px;
    overflow: scroll;
    scroll-snap-type: inline;
    display: flex;
    margin: 0 20px;
  }

  .rel-panel::-webkit-scrollbar {
    width: 10px;
    height: 1px;
  }

  .rel-panel::-webkit-scrollbar-thumb {
    border-radius: 10px;
    /*-webkit-box-shadow: inset 0 0 5px rgba(0, 0, 0, 0.2);*/
    background: rgba(54, 54, 54, 0.14);
  }

  .rel-panel::-webkit-scrollbar-track {
    /*-webkit-box-shadow: inset 0 0 5px rgba(0, 0, 0, 0.2);*/
    border-radius: 10px;
    background: #EDEDED;
  }

  .content-left {
    border-right: 2px solid #EDEDED;
    line-height: 1.25rem;

  }

  .content-left p {
    padding-bottom: 0.5rem;
    border-bottom: 1px solid #EDEDED;
  }

  .select-box {
    margin: 10px 30px;
  }

  .input-box {
    padding: 20px 30px;
  }

  .form-span {
    display: block;
    height: 20px;
    line-height: 20px;
    font-size: 10px;
    color: darkred;
  }
</style>
