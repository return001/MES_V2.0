<!--测试配置页面的统一编辑页面-->
<template>
  <div class="edit-panel-container ">
    <el-dialog
      :title="panelTitle"
      :visible.sync="isEditing"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @close="initData"
      width="90%">
      <div class="title-button" v-if="editType === 'add'">
        <div class="import-data">
          <span>{{fileName}}</span>&nbsp;&nbsp;
          <el-button  size="small" type="primary" @click="clickLoad">导入配置</el-button>
        </div>
        <input name="file" type="file" id="files" ref="refFile" style="display: none"  @change="importData">
      </div>
      <div class="setting-header" >
<!--        <div class="form-group" v-if="formData.SoftWare.value !== ''">-->
<!--          <label for="edit-software">软件版本:</label>-->
<!--          <el-input size="small" type="text" id="edit-software" placeholder="请填写软件版本"  autocomplete="off"-->
<!--                  v-model.trim="formData.SoftWare.value" :disabled="editType === 'edit'"  clearable></el-input>-->
<!--        </div>-->
        <div class="form-group">
        <label for="edit-software">软件版本:</label>
        <el-input size="small" type="text" id="edit-software" :placeholder="editType === 'edit' ? '' : '请填写软件版本'"  autocomplete="off"
                  v-model.trim="formData.soft_version.value" :disabled="editType === 'edit'"  clearable></el-input>
      </div>
        <div class="form-group">
          <label for="edit-machinename">订单号:</label>
          <el-input size="small" type="text" id="edit-zhidan" :placeholder="editType === 'edit' ? '' : '请填写订单号'"  autocomplete="off"
                    v-model.trim="formData.order_name.value" :disabled="editType === 'edit'" clearable></el-input>
        </div>
        <div class="form-group">
          <label for="edit-machinename">机型名:</label>
          <el-input size="small" type="text" id="edit-machinename" placeholder="请填写机型名"  autocomplete="off"
                    v-model.trim="formData.MachineName.value" clearable></el-input>
        </div>
        <div class="setting-operation" style="margin-left: auto" v-if="deleteHistory.length > 0 && copySrcIndex !== null">
          <div v-if="deleteHistory.length > 0">
            <el-button style="width: 100%" type="primary" size="mini" icon="el-icon-back" @click="restoreOneSetting">
              撤销删除
            </el-button>
          </div>
          <div style="margin-left: 10px" v-if="copySrcIndex !== null">
            <el-button style="width: 100%" type="primary" size="mini" @click="cancelCopySetting">
              取消复制
            </el-button>
          </div>
        </div>
      </div>

      <el-row style="margin-top: 20px" class="setting-title">
        <el-col :span="1">序号</el-col>
        <el-col :span="2">*工位</el-col>
        <el-col :span="4">*项目</el-col>
        <el-col :span="8">返回值</el-col>
        <el-col :span="2">可变参数</el-col>
        <el-col :span="5">AT指令</el-col>
        <el-col :span="2">操作</el-col>

      </el-row>
      <el-row class="setting-row" v-for="(item, index) in formData.SettingList" :key="index"
              :class="setCopyStyle(index)">
        <el-col :span="1">
          <el-input v-model.trim="index" disabled></el-input>
        </el-col>
        <el-col :span="2">
          <el-select v-model="item['1']" placeholder="请选择" style="width: 100%">
            <el-option value="共有指令"></el-option>
            <el-option value="功能测试"></el-option>
            <el-option value="IMEI域名" v-if="$route.query.type === '5'"></el-option>
            <el-option value="白卡测试" v-if="$route.query.type === '5'"></el-option>
          </el-select>
        </el-col>
        <el-col :span="4">
          <el-input v-model.trim="item['2']"></el-input>
        </el-col>
        <el-col :span="8">
          <el-input v-model.trim="item['3']"></el-input>
        </el-col>
        <el-col :span="2">
          <el-input v-model.trim="item['4']"></el-input>
        </el-col>
        <el-col :span="5">
          <el-input v-model.trim="item['5']"></el-input>
        </el-col>
        <el-col :span="2">
          <el-tooltip content="添加记录" placement="top">
            <el-button type="primary" circle icon="el-icon-plus" @click="addOneSetting(index)"></el-button>
          </el-tooltip>
          <el-tooltip content="删除记录" placement="top">
            <el-button type="primary" circle class="el-icon-minus" @click="deleteOneSetting(index)"></el-button>
          </el-tooltip>
          <el-tooltip content="复制" placement="top" v-if="copySrcIndex === null">
            <el-button type="primary" circle icon="el-icon-copy-document" @click="copyOneSetting(index)"></el-button>
          </el-tooltip>
          <el-tooltip content="在下方插入" placement="top" v-if="copySrcIndex !== null">
            <el-button circle icon="el-icon-full-screen
" @click="parseOneSetting(index)"></el-button>
          </el-tooltip>
        </el-col>
      </el-row>
      <span slot="footer" class="dialog-footer">
        <el-button size="small" @click="closePanel">取消</el-button>
        <el-button size="small" @click="submitEdit" v-if="editType !== 'copy'">保存</el-button>
        <el-button size="small" @click="isSaveAs = true" v-else>另存为</el-button>
      </span>
      <el-dialog
        width="300px"
        title="另存为"
        :visible.sync="isSaveAs"
        append-to-body
      >
        <el-select size="small" v-model="pageType" placeholder="请选择" style="width: 100%">
          <el-option value="0" label="SMT测试配置"></el-option>
          <el-option value="1" label="组装测试配置"></el-option>
          <el-option value="3" label="研发测试配置"></el-option>
          <el-option value="5" label="OQC"></el-option>
        </el-select>
        <span slot="footer" class="dialog-footer">
        <el-button @click="submitEdit('saveAs')">保存</el-button>
      </span>
      </el-dialog>
    </el-dialog>
  </div>
</template>

<script>
  import {mapGetters, mapActions} from 'vuex'
  import {testOperUrl} from "../../../../config/testApiConfig";
  import {axiosFetch} from "../../../../utils/fetchData";
  import eventBus from "../../../../utils/eventBus";
  import {getTime} from "../../../../utils/utils";

  export default {
    name: "EditFuncPanel",
    inject: ['reload'],
    data() {
      return {
        isCreate: false,
        isUpdate: false,
        fileName:"",
        formData: {
          // 'SoftWare': {
          //   value: ''
          // },
          'soft_version':{
            value:''
          },
          'order_name':{
            value:''
          },
          'MachineName': {
            value: ''
          },
          'SettingList': [
            {
              '1': '共有指令',
              '2': '',
              '3': '',
              '4': '',
              '5': '',
            }
          ]
        },
        isPending: false,
        isEditing: false,
        isSaveAs: false,
        editType: '',
        pageType: '',
        sourceData: [],
        deleteHistory: [],
        copySrcIndex: null
      }
    },
    computed: {
      panelTitle: function () {
        switch (this.editType) {
          case 'edit':
            return '编辑配置';
          case 'add':
            return '新增配置';
          case 'copy':
            return '复制配置';
        }
      }
    },
    mounted: function () {
      /*edit data $emit at @/pages/test/details/comp/TableDetails*/
      eventBus.$off('editTestFunc');
      eventBus.$on('editTestFunc', data => {
        Object.assign(this.formData, this.$options.data().formData);
        this.editType = data[0];
        this.sourceData = data[1];
        if (this.editType === 'edit' || this.editType === 'copy') {
          this.isCreate = false;
          this.isUpdate = true;
          this.formData.soft_version.value = this.sourceData.soft_version;
          this.formData.order_name.value = this.sourceData.order_name;
          if (this.$route.query.type === '2') {
            let array = this.sourceData.MachineName.replace('}}', '').split('@@');
            this.formData.MachineName.value = array[0];
          } else {
            this.formData.MachineName.value = this.sourceData.MachineName;
          }
          Object.keys(this.sourceData).forEach(item => {
            if (this.sourceData[item] === "") {
              return;
            }
            if (item.indexOf('Setting') >= 0) {
              let tempData;
              let no;
              if (!this.sourceData[item]) {
                return;
              }
              /*测试工位*/
              if (this.sourceData[item].indexOf('共有指令') >= 0) {
                no = Number(item.replace('Setting', ''));
                this.$set(this.formData.SettingList, no, {
                  "1": "共有指令",
                  "2": "",
                  "3": "",
                  "4": "",
                  "5": ""
                });
                tempData = this.sourceData[item].replace('共有指令', '').replace('}}', '');
              } else if (this.sourceData[item].indexOf('功能测试') >= 0) {
                no = Number(item.replace('Setting', ''));
                this.$set(this.formData.SettingList, no, {
                  "1": "功能测试",
                  "2": "",
                  "3": "",
                  "4": "",
                  "5": ""
                });
                tempData = this.sourceData[item].replace('功能测试', '').replace('}}', '');
              } else if (this.sourceData[item].indexOf('IMEI域名') >= 0) {
                no = Number(item.replace('Setting', ''));
                this.$set(this.formData.SettingList, no, {
                  "1": "IMEI域名",
                  "2": "",
                  "3": "",
                  "4": "",
                  "5": ""
                });
                tempData = this.sourceData[item].replace('IMEI域名', '').replace('}}', '');
              } else if (this.sourceData[item].indexOf('白卡测试') >= 0) {
                no = Number(item.replace('Setting', ''));
                this.$set(this.formData.SettingList, no, {
                  "1": "白卡测试",
                  "2": "",
                  "3": "",
                  "4": "",
                  "5": ""
                });
                tempData = this.sourceData[item].replace('白卡测试', '').replace('}}', '');
              }

              if (!!tempData) {
                let dataArray = tempData.split('@@');
                this.formData.SettingList[no]["2"] = dataArray[0];
                this.formData.SettingList[no]["3"] = dataArray[1];
                this.formData.SettingList[no]["4"] = dataArray[2];
                this.formData.SettingList[no]["5"] = dataArray[3];
              }
            }
          })
        } else if (this.editType === 'add') {
          this.isCreate = true;
          this.isUpdate = false;
        }
        this.isEditing = true;
      })

    },
    methods: {
      initData: function () {
        Object.assign(this.$data, this.$options.data())
      },
      closePanel: function () {
        this.initData();
      },

      checkFormat: function () {
        /*空值*/
        let emptyMark = true;
        let mark = true;
        if (this.formData.order_name.value === '' ||this.formData.soft_version.value === '' || this.formData.MachineName.value === '') {
          emptyMark = false;
        }
        this.formData.SettingList.forEach((item, index) => {
          if (item['2'] === '') {
            emptyMark = false;
          }
          if(typeof (item[5]) === 'undefined'){
            item[5] = ""
          }
          if (item[2].indexOf('@@') >= 0 || item[3].indexOf('@@') >= 0 || item[4].indexOf('@@') >= 0 ||item[5].indexOf('@@') >= 0) {
            this.$alertInfo('第' + index + '项存在非法字符"@@"');
            mark = false;
          }
        });
        if (!emptyMark) {
          this.$alertInfo('存在不能为空项目')
        }
        return mark && emptyMark;
      },

      //导入文件
      importData() {
        console.log(222)
        const selectedFile = this.$refs.refFile.files[0];
        if(selectedFile.type !== 'text/plain'){
          this.$alertWarning('请选择.tRxt文件')
          return
        }
        let modelName
        switch (this.$route.query.type) {
          case '0':
            modelName = 'SMT功能测试'
            break
          case '1':
            modelName = '组装功能测试'
            break
          case '2':
            modelName = '组装耦合测试'
            break
          case '3':
            modelName = '研发功能测试'
            break
          case '4':
            modelName = '研发耦合测试'
            break
          case '5':
            modelName = 'OQC'
            break
        }
        if(selectedFile.name.indexOf(modelName) === -1){
          this.$alertWarning('请选择当前模块所对应的.txt文件')
          return
        }
        this.fileName = this.$refs.refFile.files[0].name
        let reader = new FileReader();
        reader.readAsText(selectedFile);
        reader.onload = (e) => {
          this.sourceData = JSON.parse(e.target.result);
          Object.keys(this.sourceData).forEach(item => {
            if (this.sourceData[item] === "") {
              return;
            }
            if (item.indexOf('Setting') >= 0) {
              let tempData;
              let no;
              if (!this.sourceData[item]) {
                return;
              }
              /*测试工位*/
              if (this.sourceData[item].indexOf('共有指令') >= 0) {
                no = Number(item.replace('Setting', ''));
                this.$set(this.formData.SettingList, no, {
                  "1": "共有指令",
                  "2": "",
                  "3": "",
                  "4": ""
                });
                tempData = this.sourceData[item].replace('共有指令', '').replace('}}', '');
              } else if (this.sourceData[item].indexOf('IMEI域名') >= 0) {
                no = Number(item.replace('Setting', ''));
                this.$set(this.formData.SettingList, no, {
                  "1": "IMEI域名",
                  "2": "",
                  "3": "",
                  "4": ""
                });
                tempData = this.sourceData[item].replace('IMEI域名', '').replace('}}', '');
              } else if (this.sourceData[item].indexOf('白卡测试') >= 0) {
                no = Number(item.replace('Setting', ''));
                this.$set(this.formData.SettingList, no, {
                  "1": "白卡测试",
                  "2": "",
                  "3": "",
                  "4": ""
                });
                tempData = this.sourceData[item].replace('白卡测试', '').replace('}}', '');
              }

              if (!!tempData) {
                let dataArray = tempData.split('@@');
                this.formData.SettingList[no]["2"] = dataArray[0];
                this.formData.SettingList[no]["3"] = dataArray[1];
                this.formData.SettingList[no]["4"] = dataArray[2]
              }
            }
          })
        };
      },
      clickLoad() {
        this.$refs.refFile.dispatchEvent(new MouseEvent("click"));
      },


      submitEdit: function (submitType) {
        if (this.checkFormat()) {
          this.isPending = true;
          this.$openLoading();
          let settingList = this.formData.SettingList;
          let options = {
            url: testOperUrl + (this.editType === 'edit' ? '/update' : '/create'),
            data: {
              softWare: this.formData.soft_version.value + this.formData.order_name.value,
              softVersion: this.formData.soft_version.value,
              orderName: this.formData.order_name.value,
              machineName: this.formData.MachineName.value,
              recordTime: getTime()
            }
          };

          /*station 项 machineName + setting*/
          let station = this.formData.MachineName.value + "}}" + (settingList.length >= 10 ? settingList.length : '0' + settingList.length);
          for (let i = 0; i < settingList.length; i++) {
            let setting = settingList[i]["1"] + settingList[i]["2"] + '@@' + settingList[i]["3"] + '@@' + settingList[i]["4"] + '@@' + settingList[i]["5"] + '}}';
            options.data['setting' + i] = setting;
            station += setting;
          }
          options.data.station = station;


          /*配置空值填充*/
          for (let i = this.formData.SettingList.length; i < 68; i++) {
            options.data['setting' + i] = '';
          }

          options.data.type = this.$route.query.type;

          /*另存为*/
          if (submitType === 'saveAs') {
            if (this.pageType !== '') {
              options.data.type = this.pageType;
            } else {
              this.isPending = false;
              this.$closeLoading();
              this.$alertInfo('请选择保存位置');
              return;
            }
          }
          axiosFetch(options).then(response => {
            this.isPending = false;
            this.$closeLoading();
            if (response.data.result === 200) {
              this.isSaveAs = false;
              this.closePanel();
              this.$alertSuccess('更新成功');
              this.reload();
            } else {
              this.$alertWarning(response.data.data);
            }
            this.pageType = '';
          }).catch(err => {
            this.$alertDanger('请求超时，请刷新重试')
          }).finally(() => {
            this.isPending = false;
            this.$closeLoading();
          })
        }
      },

      addSetting: function () {
        let latestData = this.formData.SettingList[this.formData.SettingList.length - 1];
        if (this.formData.SettingList.length >= 68) {
          this.$alertInfo('已达配置数上限');
          return;
        }
        if (this.formData.SettingList.length > 0 && latestData['2'] === '') {
          this.$alertInfo('前一记录尚未填写完整');
          return;
        }
        this.formData.SettingList.push(
          {
            '1': '共有指令',
            '2': '',
            '3': '',
            '4': '',
            '5': ''
          }
        )
      },
      addOneSetting: function (index) {
        if (this.copySrcIndex !== null) {
          this.$alertInfo('请完成或取消复制');
          return;
        }
        if (this.formData.SettingList.length >= 68) {
          this.$alertInfo('已达配置数上限');
          return;
        }
        this.formData.SettingList.splice(index + 1, 0,
          {
            '1': '共有指令',
            '2': '',
            '3': '',
            '4': '',
            '5': ''
          }
        )
      },

      deleteOneSetting: function (index) {
        if (this.copySrcIndex !== null) {
          this.$alertInfo('请完成或取消复制');
          return;
        }
        this.deleteHistory.push({
          index: index,
          data: this.formData.SettingList[index]
        });
        this.formData.SettingList.splice(index, 1)
      },

      copyOneSetting: function (index) {
        this.copySrcIndex = index;
      },

      parseOneSetting: function (index) {
        if (this.formData.SettingList.length >= 68) {
          this.$alertInfo('已达配置数上限');
          return;
        }
        this.formData.SettingList.splice(index + 1, 0,
          this.formData.SettingList[this.copySrcIndex]
        );
        this.cancelCopySetting();
      },

      cancelCopySetting: function () {
        this.copySrcIndex = null
      },

      setCopyStyle(index) {
        if (index === this.copySrcIndex) {
          return "copy-row-style"
        }
      },

      restoreOneSetting: function () {
        if (this.deleteHistory.length > 0) {
          let latestData = this.deleteHistory[this.deleteHistory.length - 1];
          this.formData.SettingList.splice(latestData.index, 0, latestData.data);
          this.deleteHistory.pop();
        }
      }

    }
  }
</script>

<style scoped>
  .edit-panel-container {
  }

  .setting-header {
    display: flex;
    justify-content: space-between;
    align-items: flex-end;
  }

  .setting-header label {
    line-height: 24px;
  }

  .setting-header .form-group {
    width: 200px;
    margin-right: 10px;
  }

  .setting-header .setting-operation {
    display: flex;
  }


  .setting-title {
    background-color: #eeeeee;
  }

  .setting-title .el-col {
    border: 1px solid #dddddd;
    padding: 5px 6px;
  }

  .setting-title .el-col + .el-col {
    border: 1px solid #dddddd;
    padding: 5px 6px;
    border-left: none;
  }

  @media screen and (max-width: 1000px) {
    .setting-row {
      height: 64px;
    }
  }

  .title-button{
    width: 100%;
    height: 30px;

    margin-bottom: 5px;
  }
  .import-data{
    margin-right: 10px;
    float: right;
  }

  .setting-row .el-col {
    height: 100%;
  }

  .setting-row /deep/ .el-input, .setting-row /deep/ .el-select {
    height: 100%;
  }

  .setting-header{
    display: flex;
    justify-content: space-between;
    align-items: flex-end;
  }
  .setting-header .form-group{
    min-width: 260px;
  }
  .setting-header .form-group:first-child , .setting-header .form-group:nth-child(2) {
    width: 450px;
    margin-right: 20px;
  }
  .setting-row /deep/ .el-input__inner {
    background: unset;
    border: 1px solid #dddddd;
    border-top: none;
    height: 100%;
    min-height: 32px;
    line-height: 32px;
    padding: 0 6px;
    border-radius: 0;
  }

  .setting-row .el-col .el-button {
    margin: 0;
  }

  .setting-row .el-col:last-child {
    border: 1px solid #dddddd;
    border-top: none;
    border-left: none;
    height: 100%;
    min-height: 32px;
    line-height: 32px;
    border-radius: 0;
    display: flex;
    justify-content: space-around;
    align-items: center;
    flex-wrap: wrap;
  }

  .setting-row .el-col:last-child .el-button {
    height: 24px;
    line-height: 24px;
    width: 24px;
    min-width: 24px;
    padding: 0;
    font-size: 12px;
    display: inline-block;
    transform: scale(0.8);
  }

  .setting-row .el-col + .el-col /deep/ .el-input__inner {
    border-left: none;
  }

  .copy-row-style {
    box-shadow: #409eff 0 0 5px inset;
  }


</style>
