<!--订单配置页面的统一编辑页面-->
<template>
  <el-dialog id="edit-panel" v-if="isEditing" :visible.sync="isEditing" :title="isCreate ? '创建表单:' : '更新表单'" width="90%" :close-on-click-modal="false" :close-on-press-escape="false" :show-close="false">
    <div class="edit-panel-container">
      <div class="form-row mb-3">
        <div class="form-group col-3">
          <label for="edit-ZhiDan" class="col-form-label">制单号:{{!!formData[0].notNull ? '*' : ''}}</label>
          <el-input size="small"
                    type="text" id="edit-ZhiDan"
                    class="form-control"
                    v-model.trim="formData[0].value"
                    :disabled="!isCreate" clearable></el-input>
        </div>
        <div class="form-group col-3" v-if="isUpdate">
          <label for="edit-Status" class="col-form-label">状态:{{!!formData[1].notNull ? '*' : ''}}</label>
          <el-select size="small"  id="edit-Status" class="custom-select"
                     v-model="formData[1].value" :disabled="!isCreate">
            <el-option v-for="item in formData[1].valueList" :value="item.key" :label="item.label" :key="item.key"></el-option>
          </el-select>
        </div>

      </div>
      <div class="divider"></div>
      <div class="form-row">
        <div class="form-group" v-for="(item, index) in formData" v-if="index >= 2">
          <div v-if="item.type === 'text'">
            <label :for="'edit-' + item.field" class="col-form-label">{{item.title}}: {{!!item.notNull ? '*' :
              ''}}</label>
            <el-input size="small" type="text" :id="'edit-' + item.field" class="form-control" v-model.trim="item.value"
                      :disabled="(!isCreate) && (formData[1].value !== 0)" clearable></el-input>
          </div>
          <div v-if="item.type === 'select'">
            <label :for="'edit-' + item.field" class="col-form-label">{{item.title}}: {{!!item.notNull ? '*' :
              ''}}</label>
            <el-select size="small" type="text" :id="'edit-' + item.field" class="form-control" v-model="item.value"
                       :disabled="(!isCreate) && (formData[1].value !== 0)">
              <el-option value="" disabled>请选择</el-option>
              <el-option v-for="i in item.valueList" :key="i.key" :value="i.key" :label="i.label"></el-option>
            </el-select>
          </div>
        </div>
        <p class="form-control-sm">* 为非空项目</p>

      </div>
      <div class="divider"></div>
      <div class="edit-panel-btn-group">
        <el-button size="small" type="info" @click="initData">取消</el-button>
        <el-button size="small" type="primary" @click="editData"
                   :disabled="(!isCreate) && (formData[1].value !== 0)">提交
        </el-button>
      </div>
    </div>
  </el-dialog>
</template>

<script>
  import {orderOperUrl, getOrderEditOptions} from "../../../../config/orderApiConfig";
  import {axiosFetch} from "../../../../utils/fetchData";
  import eventBus from "../../../../utils/eventBus";

  export default {
    name: "EditPanel",
    inject: ['reload'],
    data() {
      return {
        isEditing: false,
        isCreate: false,
        isUpdate: false,
        editType: '',
        sourceData: [],
        formData: [],
        isPending: false
      }
    },
    created: function () {
    },
    mounted: function () {

      /*edit data $emit at src/pages/order/details/comp/TableDetails*/
      eventBus.$off('editOrder');
      eventBus.$on('editOrder', data => {
        //data: [editType, originData]
        this.editType = data[0];
        this.sourceData = data[1];
        if (data[0] === 'edit') {
          this.isCreate = false;
          this.isUpdate = true;
          this.formData = JSON.parse(JSON.stringify(getOrderEditOptions()));
          for (let i = 0; i < this.formData.length; i++) {
            this.formData[i].value = data[1][this.formData[i].field]
          }
        } else if (data[0] === 'copy') {
          this.isCreate = true;
          this.isUpdate = false;
          this.formData = JSON.parse(JSON.stringify(getOrderEditOptions()));
          for (let i = 0; i < this.formData.length; i++) {
            this.formData[i].value = data[1][this.formData[i].field]
          }
        } else if (data[0] === 'add') {
          this.isCreate = true;
          this.isUpdate = false;
          this.formData = JSON.parse(JSON.stringify(getOrderEditOptions()));
        }
        this.isEditing = true;
      })

    },
    methods: {
      initData: function () {
        Object.assign(this.$data, this.$options.data())
      },
      editData: function () {
        this.isPending = true;
        this.$openLoading();
        let tempData = {};
        if (this.editType === 'edit') {
          tempData.id = this.sourceData.Id
        } else {
          tempData.zhiDan = this.formData[0].value
        }
        for (let i = 2; i < this.formData.length; i++) {
          if (!!this.formData[i].notNull) {
            if (this.formData[i].value !== "") {
              tempData[this.formatCase(this.formData[i].field)] = this.formData[i].value;
            } else {
              this.$alertWarning("存在不能为空数据");
              this.isPending = false;
              this.$closeLoading();
              return
            }
          } else {
            tempData[this.formatCase(this.formData[i].field)] = this.formData[i].value;
          }
        }
        let options = {
          data: tempData
        };
        if (this.editType === 'edit') {
          options.url = orderOperUrl + '/update'
        } else {
          options.url = orderOperUrl + '/create'
        }
        axiosFetch(options).then(res => {
          if (res.data.result === 200) {
            this.$alertSuccess(res.data.data);
            this.reload();
            this.initData();
          } else {
            this.$alertWarning(res.data.data)
          }
        }).catch(err => {
          console.log(JSON.stringify(err));
          this.$alertDanger('请求超时，请刷新重试');
        }).finally(() => {
          this.isPending = false;
          this.$closeLoading();
        })
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

  .form-row {
    display: flex;
    flex-wrap: wrap;
    align-items: flex-end;
  }

  .form-group {
    width: 200px;
    margin-right: 20px;
  }

  .form-group /deep/ label {
    line-height: 30px;
  }
  .edit-panel-btn-group {
    width: 100%;
    display: flex;
    justify-content: center;
  }
  .edit-panel-btn-group .el-button {
    width: 100px;
  }

  .divider {
    border-top: 1px solid #eee;
    width: 100%;
    margin: 20px 0;
  }
</style>
