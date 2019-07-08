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
      <div class="header">
        <el-row :gutter="20" type="flex">
          <el-col :span="4">
            <label for="edit-software">*软件版本:</label>
            <el-input type="text" id="edit-software" placeholder="请填写软件版本" clearable autocomplete="off"
                      v-model.trim="formData.SoftWare.value" :disabled="editType === 'edit'"></el-input>
          </el-col>
          <el-col :span="4">
            <label for="edit-machinename">*机型名:</label>
            <el-input type="text" id="edit-machinename" placeholder="请填写机型名" clearable autocomplete="off"
                      v-model.trim="formData.MachineName.value"></el-input>
          </el-col>
          <el-col :span="4" v-if="$route.query.type === '2'">
            <label for="edit-software">IMEI号段 从:</label>
            <el-input type="text" id="edit-software" clearable autocomplete="off"
                      v-model.trim="formData.MachineName.imeiFrom"></el-input>
          </el-col>
          <el-col :span="4" style="margin-left: -10px" v-if="$route.query.type === '2'">
            <label for="edit-software">至:</label>
            <el-input type="text" id="edit-software" clearable autocomplete="off"
                      v-model.trim="formData.MachineName.imeiTo"></el-input>
          </el-col>
          <el-col :span="2" style="align-self: flex-end; margin-left: auto" v-if="deleteHistory.length > 0">
            <el-button style="width: 100%" type="primary" size="mini" icon="el-icon-back
" @click="restoreOneSetting">撤销删除
            </el-button>
          </el-col>
        </el-row>
        <el-row style="margin-top: 20px" class="setting-title">
          <el-col :span="2">序号</el-col>
          <el-col :span="3">*工位</el-col>
          <el-col :span="6">*项目</el-col>
          <el-col :span="7">AT指令</el-col>
          <el-col :span="5">返回值</el-col>
          <el-col :span="1">操作</el-col>

        </el-row>
        <el-row class="setting-row" v-for="(item, index) in formData.SettingList" :key="index">
          <el-col :span="2">
            <el-input v-model.trim="index" disabled></el-input>
          </el-col>
          <el-col :span="3">
            <el-select v-model="item['1']" placeholder="请选择" style="width: 100%">
              <el-option value="共有指令"></el-option>
              <el-option value="IMEI域名"></el-option>
              <el-option value="白卡测试"></el-option>
            </el-select>
          </el-col>
          <el-col :span="6">
            <el-input v-model.trim="item['2']"></el-input>
          </el-col>
          <el-col :span="7">
            <el-input v-model.trim="item['3']"></el-input>
          </el-col>
          <el-col :span="5">
            <el-input v-model.trim="item['4']"></el-input>
          </el-col>
          <el-col :span="1">
            <div @click="deleteOneSetting(index)">
              <i class="el-icon-minus"></i>
            </div>
          </el-col>
        </el-row>
        <el-row type="flex" justify="center" style="margin-top: 30px">
          <el-tooltip content="添加一条记录" placement="top">
            <el-button type="primary" circle icon="el-icon-plus" @click="addSetting"></el-button>
          </el-tooltip>
        </el-row>
      </div>
      <span slot="footer" class="dialog-footer">
        <el-button @click="closePanel">取消</el-button>
        <el-button @click="submitEdit" v-if="editType !== 'copy'">保存</el-button>
        <el-button @click="isSaveAs = true" v-else>另存为</el-button>
      </span>
      <el-dialog
        width="300px"
        title="另存为"
        :visible.sync="isSaveAs"
        append-to-body
      >
        <el-select v-model="pageType" placeholder="请选择" style="width: 100%">
          <el-option value="2" label="组装测试配置"></el-option>
          <el-option value="4" label="研发测试配置"></el-option>
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
  import {errHandler} from "../../../../utils/errorHandler";
  import {getTestConfig} from "../../../../config/testApiConfig";
  import _ from 'lodash'
  import eventBus from "../../../../utils/eventBus";
  import {getTime} from "../../../../utils/utils";

  export default {
    name: "EditPanel",
    //props: ['editData'],
    data() {
      return {
        isCreate: false,
        isUpdate: false,
        formData: {
          'SoftWare': {
            value: ''
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

            }
          ]
        },
        isPending: false,
        isEditing: false,
        isSaveAs: false,
        editType: '',
        pageType: '',
        sourceData: [],
        deleteHistory: []
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
      eventBus.$off('editTest');
      eventBus.$on('editTest', data => {
        Object.assign(this.formData, this.$options.data().formData);
        this.editType = data[0];
        this.sourceData = data[1];
        if (this.editType === 'edit' || this.editType === 'copy') {
          this.isCreate = false;
          this.isUpdate = true;
          this.formData.SoftWare.value = this.sourceData.SoftWare;
          if (this.$route.query.type === '2') {
            this.$set(this.formData.MachineName, 'imeiFrom', '');
            this.$set(this.formData.MachineName, 'imeiTo', '');
            let array = this.sourceData.MachineName.replace('}}', '').split('@@');
            this.formData.MachineName.value = array[0];
            this.formData.MachineName.imeiFrom = array[1];
            this.formData.MachineName.imeiTo = array[2];
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
        } else if (this.editType === 'add') {
          this.isCreate = true;
          this.isUpdate = false;
          if (this.$route.query.type === '2') {
            this.$set(this.formData.MachineName, 'imeiFrom', '');
            this.$set(this.formData.MachineName, 'imeiTo', '');
          }
        }
        this.isEditing = true;
      })

    },
    methods: {
      ...mapActions(['setLoading']),

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
        if (this.formData.SoftWare.value === '' || this.formData.MachineName.value === '') {
          emptyMark = false;
        }

        if (this.$route.query.type === '2') {
          let from = this.formData.MachineName.imeiFrom;
          let to = this.formData.MachineName.imeiTo;

          /*均非空判断格式、范围*/
          if (from !== '' && to !== '') {
            let regx = new RegExp("^[1-9]\\d*$");
            if (!regx.test(from) || !regx.test(to)) {
              this.$alertInfo('IMEI填写格式有误');
              mark = false;
            }
            if (from - to > 0) {
              this.$alertInfo('IMEI范围有误');
              mark = false;
            }
          } else if (from === '' ^ to === '') {
            this.$alertInfo('IMEI填写不完整');
            mark = false;
          }
        }

        this.formData.SettingList.forEach((item, index) => {
          if (item['2'] === '') {
            emptyMark = false;
          }
          if (item[2].indexOf('@@') >= 0 || item[3].indexOf('@@') >= 0 || item[4].indexOf('@@') >= 0) {
            this.$alertInfo('第' + index + '项存在非法字符"@@"');
            mark = false;
          }
        });
        if (!emptyMark) {
          this.$alertInfo('存在不能为空项目')
        }
        return mark && emptyMark;
      },

      submitEdit: function (submitType) {
        if (this.checkFormat()) {
          this.isPending = true;
          this.$openLoading();
          let settingList = this.formData.SettingList;
          let options = {
            url: testOperUrl + (this.editType === 'edit' ? '/update' : '/create'),
            data: {
              softWare: this.formData.SoftWare.value,
              machineName: this.formData.MachineName.value,
              recordTime: getTime()
            }
          };

          /*station 项 machineName + setting*/
          let station = this.formData.MachineName.value + "}}" + (settingList.length >= 10 ? settingList.length : '0' + settingList.length);
          for (let i = 0; i < settingList.length; i++) {
            let setting = settingList[i]["1"] + settingList[i]["2"] + '@@' + settingList[i]["3"] + '@@' + settingList[i]["4"] + '}}';
            options.data['setting' + i] = setting;
            station += setting;
          }
          options.data.station = station;

          /*组装-耦合(type:2)的MachineName需要IMEI号段*/
          if (this.$route.query.type === '2') {
            options.data.machineName = this.formData.MachineName.value + '@@' + this.formData.MachineName.imeiFrom + '@@' + this.formData.MachineName.imeiTo + '}}'
          }

          /*配置空值填充*/
          for (let i = this.formData.SettingList.length; i < 68; i++) {
            options.data['setting' + i] = '';
          }

          options.data.type = this.$route.query.type;

          /*另存为*/
          if (submitType === 'saveAs') {
            if (this.pageType !== '') {
              options.data.type = this.pageType;
              if (this.$route.query.type === '2') {
                options.data.machineName = this.formData.MachineName.value;
              }
              if (this.pageType === '2' && this.$route.query.type === '4') {
                options.data.machineName = this.formData.MachineName.value + '@@@@}}';
              }
              if (this.pageType === '5') {
                options.data.machineName = this.formData.MachineName.value;

                let station = this.formData.MachineName.value + "}}" + (settingList.length >= 10 ? settingList.length : '0' + settingList.length);
                for (let i = 0; i < settingList.length; i++) {
                  let setting = settingList[i]["1"] + settingList[i]["2"] + '@@' + settingList[i]["4"] + '@@@@' + settingList[i]["3"] + '}}';
                  options.data['setting' + i] = setting;
                  station += setting;
                }
                options.data.station = station;
              }
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
              this.$alertSuccess(response.data.data);
              let tempUrl = this.$route.fullPath;
              this.$router.push('/_empty');
              this.$router.replace(tempUrl);
            } else {
              this.$alertWarning(response.data.data);
            }
            this.pageType = '';
          }).catch(err => {
            this.$closeLoading();
            this.isPending = false;
            this.$alertDanger('请求超时，请刷新重试')
          })
        }
      },

      addSetting: function () {
        let latestData = this.formData.SettingList[this.formData.SettingList.length - 1];
        if (this.formData.SettingList.length >= 68) {
          this.$alertInfo('已达配置数上限')
        }
        if (this.formData.SettingList.length > 0 && latestData['2'] === '') {
          this.$alertInfo('前一记录尚未填写完整');
          return;
        }
        this.formData.SettingList.push(
          {
            '0': latestData['0'] + 1,
            '1': '共有指令',
            '2': '',
            '3': '',
            '4': ''
          }
        )
      },

      deleteOneSetting: function (index) {
        this.deleteHistory.push({
          index: index,
          data: this.formData.SettingList[index]
        });
        this.formData.SettingList.splice(index, 1)
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

  .setting-row /deep/ .el-input__inner {
    border: 1px solid #dddddd;
    border-top: none;
    height: 32px;
    line-height: 32px;
    padding: 0 6px;
    border-radius: 0;
  }

  .setting-row .el-col:last-child {
    border: 1px solid #dddddd;
    border-top: none;
    border-left: none;
    height: 32px;
    line-height: 32px;
    border-radius: 0;
    display: flex;
    justify-content: center;
  }

  .setting-row .el-col:last-child div {
    display: flex;
    margin: 6px;
    width: 20px;
    height: 20px;
    border-radius: 50%;
    background-color: #66b1ff;
    justify-content: center;
    align-items: center;
  }

  .setting-row .el-col:last-child div i {
    color: #ffffff;
  }

  .setting-row .el-col:last-child div:hover {
    background-color: #409EFF;
    cursor: pointer;
  }

  .setting-row .el-col + .el-col /deep/ .el-input__inner {
    border-left: none;
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
