<!--测试配置页面的统一编辑页面-->
<template>
  <div class="edit-panel-container ">
    <div class="edit-panel">
      <div class="form-row">
        <div class="form-group col-2 mb-0">
          <h2>{{isUpdate === true ? '更新表单:' : ''}} {{isCreate === true ? '创建表单:' : ''}}</h2>
        </div>
      </div>
      <div class="form-row mb-3">
        <div class="form-group col-3">
          <label for="edit-SoftWare" class="col-form-label">版本号:{{formData[1].notNull === true ? '*' : ''}}</label>
          <input type="text" id="edit-SoftWare"
                 class="form-control"
                 v-model="formData[1].value"
                 :disabled="!isCreate">
        </div>
        <div class="form-group col-3">
          <label for="edit-MachineName" class="col-form-label">机型名:{{formData[2].notNull === true ? '*' : ''}}</label>
          <input type="text" id="edit-MachineName"
                 class="form-control"
                 v-model="formData[2].value">
        </div>
        <div class="form-group col-3">
          <label for="edit-RecordTime" class="col-form-label">记录时间:{{formData[3].notNull === true ? '*' : ''}}</label>
          <input type="text" id="edit-RecordTime"
                 class="form-control"
                 v-model="formData[3].value">
        </div>
        <div class="form-group col-12">
          <label for="edit-Station" class="col-form-label">配置:{{formData[4].notNull === true ? '*' : ''}}</label>
          <textarea contenteditable="true" id="edit-Station"
                 class="form-control"
                    v-model="formData[4].value" rows="3"></textarea>
          <!--<div contenteditable="true" id="edit-Station"-->
                    <!--class="form-control"-->
                     <!--@input="formData[5].value = $event.target.innerText">{{formData[5].value}}</div>-->
        </div>

      </div>
      <div class="dropdown-divider"></div>
      <div class="form-row">
        <div class="form-group col-2 mb-0" v-for="(item, index) in formData" v-if="index >= 6">
          <div >
            <label :for="'edit-' + item.field" class="col-form-label">{{item.title}}: {{item.notNull === true ? '*' :
              ''}}</label>
            <input type="text" :id="'edit-' + item.field" class="form-control" v-model="item.value">
          </div>
        </div>
        <p class="form-control-sm">* 为非空项目</p>
      </div>
      <div class="dropdown-divider"></div>
      <div class="form-row justify-content-around">
        <button type="button" class="btn btn-secondary col-2 text-white" @click="closePanel">取消</button>
        <button type="button" class="btn btn-primary col-2 text-white" @click="btnHandler">提交</button>
      </div>
    </div>
  </div>
</template>

<script>
  import {mapGetters, mapActions} from 'vuex'
  import {testOperUrl} from "../../../../config/testApiConfig";
  import {axiosFetch} from "../../../../utils/fetchData";
  import {errHandler} from "../../../../utils/errorHandler";
  import {getTestConfig} from "../../../../config/testApiConfig";
  import _ from 'lodash'

  export default {
    name: "EditPanel",
    //props: ['editData'],
    data() {
      return {
        isCreate: false,
        isUpdate: false,
        formData: [],
        isPending: false
      }
    },
    computed: {
      ...mapGetters(['editData', 'copyData', 'testType'])
    },
    created: function () {
      if (this.editData.length !== 0) {
        this.isCreate = false;
        this.isUpdate = true;
        this.formData = JSON.parse(JSON.stringify(this.editData))
      } else if (this.copyData.length !== 0) {
        this.isCreate = true;
        this.isUpdate = false;
        this.formData = JSON.parse(JSON.stringify(this.copyData));
        console.log(this.formData)
      } else {
        this.isCreate = true;
        this.isUpdate = false;

        let tempOrder = JSON.parse(JSON.stringify(getTestConfig('test_manage').data.dataColumns));
        tempOrder.splice(1, 1);
        for (let i = 0; i < tempOrder.length; i++) {
          tempOrder[i].value = '';
          delete tempOrder[i].colStyle;
        }
        this.formData = tempOrder
      }
    },
    watch: {
      $route: function (val) {
        this.closePanel()
      }
    },
    mounted: function () {


    },
    methods: {
      ...mapActions(['setEditing', 'setEditData', 'setCopyData']),
      closePanel: function () {
        this.setEditData([]);
        this.setCopyData([]);
        this.setEditing(false)
      },
      updateData: function () {
        this.isPending = true;
        let tempData = {
          id: this.formData[0].value

        };
        for (let i = 1; i < this.formData.length; i++) {
          if (this.formData[i].notNull === true) {
            if (_.trim(this.formData[i].value) !== "") {
              tempData[this.formatCase(this.formData[i].field)] = _.trim(this.formData[i].value);
            } else {
              this.$alertWarning("存在不能为空数据");
              return
            }
          }
          else {
            tempData[this.formatCase(this.formData[i].field)] = _.trim(this.formData[i].value);
          }
        }

        let options = {
          url: testOperUrl + '/update',
          data: tempData
        };
        options.data.type = this.testType;
        axiosFetch(options).then(res => {
          if (res.data.result === 200) {
            this.$alertSuccess('更新成功');
            this.setEditing(false);
            this.setEditData([]);
            let tempUrl = this.$route.fullPath;
            //console.log(this.$route.url)
            this.$router.replace('/_empty')
            this.$router.replace(tempUrl)
          } else if (res.data.result === 400) {
            this.$alertInfo('请检查格式并重试')
          } else {
            errHandler(res.data.result)
          }
        }).catch(err => {
          this.$alertDanger('请求超时，请刷新重试')
        })
      },
      createData: function () {
        this.isPending = true;
        let tempData = {};
        for (let i = 1; i < this.formData.length; i++) {
          if (this.formData[i].notNull === true) {
            if (_.trim(this.formData[i].value) !== "") {
              tempData[this.formatCase(this.formData[i].field)] = _.trim(this.formData[i].value);
            } else {
              console.log(this.formData[i])
              this.$alertWarning("存在不能为空数据");
              return
            }
          }
          else if (_.trim(this.formData[i].value) !== "") {
            tempData[this.formatCase(this.formData[i].field)] = _.trim(this.formData[i].value);
          }

        }
        let options = {
          url: testOperUrl + '/create',
          data: tempData
        };
        options.data.type = this.testType;
        axiosFetch(options).then(res => {
          if (res.data.result === 200) {
            this.$alertSuccess('添加成功');
            this.setEditing(false);
            this.setCopyData([]);
            let tempUrl = this.$route.fullPath;
            this.$router.replace('/_empty');
            this.$router.replace(tempUrl)
          } else if (res.data.result === 412) {
            this.$alertWarning('请检查格式并重试');
            this.setEditing(false);
          } else if (res.data.result === 400) {
            this.$alertWarning('该版本号已存在');
            this.setEditing(false);
          } else {
            errHandler(res.data.result)
          }
        }).catch(err => {
          console.log(JSON.stringify(err))
          this.$alertDanger('请求超时，请刷新重试');
        })
      },
      btnHandler: function () {
        if (!this.isPending) {
          if (this.isCreate) {
            this.createData();
            this.isPending = false;
          } else if (this.isUpdate) {
            this.updateData();
            this.isPending = false;
          }
        }
      },
      formatCase: function (str) {
        if (str.indexOf('_') > -1) {
          let string = str.toLowerCase();
          let strArray = string.split('_');
          for (let i = 1; i < strArray.length; i++) {
            strArray[i] = strArray[i].replace(/( |^)[a-z]/g, (L) => L.toUpperCase());
          }
          return strArray.join('');
        } else {
          return str.replace(/( |^)[A-Z]/g, (L) => L.toLowerCase());
        }
      }
    }
  }
</script>

<style scoped>
  .edit-panel-container {
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

  .edit-panel {
    background: #ffffff;
    height: 100%;
    width: 100%;
    z-index: 102;
    border-radius: 10px;
    box-shadow: 3px 3px 20px 1px #bbb;
    padding: 30px 60px 10px 60px;
    overflow: scroll;
    scroll-snap-type: inline;
  }
  .edit-panel::-webkit-scrollbar {
    width: 10px;
    height: 1px;
  }

  .edit-panel::-webkit-scrollbar-thumb {
    border-radius: 10px;
    /*-webkit-box-shadow: inset 0 0 5px rgba(0, 0, 0, 0.2);*/
    background: rgba(54, 54, 54, 0.14);
  }

  .edit-panel::-webkit-scrollbar-track {
    /*-webkit-box-shadow: inset 0 0 5px rgba(0, 0, 0, 0.2);*/
    border-radius: 10px;
    background: #EDEDED;
  }
</style>
