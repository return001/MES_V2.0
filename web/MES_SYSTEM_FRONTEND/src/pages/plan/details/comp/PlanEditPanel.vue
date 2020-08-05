<template>
  <div>
    <el-dialog
      title="编辑计划"
      :visible.sync="isEditing"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @closed="resetEditPanel"
      width="730px">
      <el-form
        ref="planEditForm"
        :model="planEditOptionsData"
        class="plan-edit-form"
        label-position="top"
        @submit.native.prevent
        :rules="planEditOptionsRules">
        <!--<el-form-item
          size="small"

          class="plan-edit-form-comp"
          label="是否紧急"
          prop="isUrgent">
          <el-select
            v-model="planEditOptionsData.isUrgent"
            class="plan-edit-form-comp-text"
            autocomplete="off">
            <el-option :value="false" label="否"></el-option>
            <el-option :value="true" label="是"></el-option>
          </el-select>
        </el-form-item>
        <el-form-item
          size="small"

          class="plan-edit-form-comp"
          label="产线"
          prop="line">
          <el-select
            v-model="planEditOptionsData.line"
            class="plan-edit-form-comp-text"
            autocomplete="off">
            <el-option v-for="listItem in lineSelectGroup"
                       :key="listItem.id"
                       :value="listItem.id"
                       :label="listItem.lineName"></el-option>
          </el-select>
        </el-form-item>
        <el-form-item
          size="small"

          class="plan-edit-form-comp plan-edit-form-comp-text"
          label="排产数量"
          prop="schedulingQuantity">
          <el-input
            type="text"
            clearable
            autocomplete="off"
            placeholder="请填写排产数量"
            v-model.number="planEditOptionsData.schedulingQuantity"></el-input>
        </el-form-item>
        <el-form-item
          size="small"

          class="plan-edit-form-comp plan-edit-form-comp-text"
          label="预计开始时间"
          prop="planStartTime">
          <el-date-picker
            v-model="planEditOptionsData.planStartTime"
            type="datetime"
            prefix-icon="el-icon-date"
            default-time="08:00:00"
            value-format="yyyy-MM-dd HH:mm:ss"
            autocomplete="off">
          </el-date-picker>
        </el-form-item>
        <el-form-item
          size="small"

          class="plan-edit-form-comp plan-edit-form-comp-text"
          label="预计完成时间"
          prop="planStartTime">
          <el-date-picker
            v-model="planEditOptionsData.planCompleteTime"
            type="datetime"
            prefix-icon="el-icon-date"
            default-time="08:00:00"
            value-format="yyyy-MM-dd HH:mm:ss"
            autocomplete="off">
          </el-date-picker>
        </el-form-item>

        <el-form-item
          size="small"
          v-if="permissionShow([true, false])"
          class="plan-edit-form-comp plan-edit-form-comp-text"
          label="转线时间(H)"
          prop="lineChangeTime">
          <el-input
            type="text"
            clearable
            autocomplete="off"
            placeholder="请填写转线时间"
            v-model="planEditOptionsData.lineChangeTime"></el-input>
        </el-form-item>


        <el-form-item
          size="small"
          v-if="permissionShow([false, true])"
          class="plan-edit-form-comp plan-edit-form-comp-text"
          label="产能"
          prop="capacity">
          <el-input
            type="text"
            clearable
            placeholder="请填写产能"
            autocomplete="off"
            v-model.number="planEditOptionsData.capacity"></el-input>
        </el-form-item>-->
        <el-form-item
          size="small"
          class="plan-edit-form-comp"
          label="是否完成"
          prop="isCompleted">
          <el-select
            v-model="planEditOptionsData.isCompleted"
            class="plan-edit-form-comp-text"
            autocomplete="off">
            <el-option :value="false" label="否"></el-option>
            <el-option :value="true" label="是"></el-option>
          </el-select>
        </el-form-item>
        <el-form-item
          size="small"
          class="plan-edit-form-comp plan-edit-form-comp-text"
          label="生产计划单号"
          prop="productionPlanningNumber">
          <el-input
            type="text"
            clearable
            autocomplete="off"
            placeholder="请填写单号"
            v-model="planEditOptionsData.productionPlanningNumber"></el-input>
        </el-form-item>
        <el-form-item
          size="small"
          class="plan-edit-form-comp plan-edit-form-comp-text"
          label="已完成数量"
          prop="producedQuantity">
          <el-input
            type="text"
            clearable
            :disabled="planEditOptionsData.activeProcessGroup === 1 || planEditOptionsData.activeProcessGroup === 2 || planEditOptionsData.activeProcessGroup === 3"
            autocomplete="off"
            placeholder="请填写已完成数量"
            v-model.number="planEditOptionsData.producedQuantity"></el-input>
        </el-form-item>
        <el-form-item
          size="small"
          class="plan-edit-form-comp plan-edit-form-comp-textarea"
          label="备注"
          prop="remark">
          <el-input type="textarea"
                    :rows="4"
                    clearable
                    autocomplete="off"
                    v-model="planEditOptionsData.remark"></el-input>
        </el-form-item>
        <el-form-item
          size="small"
          class="plan-edit-form-comp plan-edit-form-comp-textarea"
          label="未完成原因"
          prop="remainingReason">
          <el-input type="textarea"
                    :rows="4"
                    clearable
                    autocomplete="off"
                    v-model="planEditOptionsData.remainingReason"></el-input>
        </el-form-item>
      </el-form>
      <span slot="footer" class="dialog-footer">
         <el-button size="small" @click="isEditing = false" type="info">取消</el-button>
         <el-button size="small" @click="submitEdit" type="primary">保存</el-button>
       </span>
    </el-dialog>
  </div>
</template>

<script>
  import eventBus from "../../../../utils/eventBus";
  import {
    planDetailsEditUrl,
    planProducedQuantitySelectUrl,
    planCheckCompleteTimeSelectUrl
  } from "../../../../config/globalUrl";
  import {axiosFetch} from "../../../../utils/fetchData";

  export default {
    name: "PlanEditPanel",
    props: ['row', 'pmcEditing', 'engineerEditing', 'lineSelectGroup', 'totallyEditing'],
    inject: ['reload'],
    data() {
      return {
        timePickerOptions: {
          shortcuts: [{
            text: '最近一周',
            onClick(picker) {
              const end = new Date();
              const start = new Date();
              start.setTime(start.getTime() - 3600 * 1000 * 24 * 7);
              picker.$emit('pick', [start, end]);
            }
          }, {
            text: '最近一个月',
            onClick(picker) {
              const end = new Date();
              const start = new Date();
              start.setTime(start.getTime() - 3600 * 1000 * 24 * 30);
              picker.$emit('pick', [start, end]);
            }
          }, {
            text: '最近三个月',
            onClick(picker) {
              const end = new Date();
              const start = new Date();
              start.setTime(start.getTime() - 3600 * 1000 * 24 * 90);
              picker.$emit('pick', [start, end]);
            }
          }]
        },
        pmcEdit: false,
        engineerEdit: false,

        planPMCEditOptionsData: {
          isUrgent: false,
          line: null,
          schedulingQuantity: null,
          lineChangeTime: null,
          remark: null,
          planStartTime: '',
          planCompleteTime: ''
        },
        planPMCEditTimeRange: '',
        planPMCEditOptionsRules: {
          // 'isUrgent': [
          //   {required: true, message: '请选择状态', trigger: 'blur'}
          // ],
          'line': [
            {required: true, message: '请选择线号', trigger: 'blur'}
          ],
          'schedulingQuantity': [
            {required: true, message: '请填写排产数量', trigger: 'blur'}
          ],
          // 'lineChangeTime': [
          //   {required: true, message: '请填写换线时间', trigger: 'blur'}
          // ],
          // 'planStartTime': [
          //   {required: true, message: '请选择时间', trigger: 'blur'}
          // ],
          // 'planCompleteTime': [
          //   {required: true, message: '请选择时间', trigger: 'blur'}
          // ],
        },

        planEngineerEditOptionsData: {
          isCompleted: false,
          productionPlanningNumber: null,
          capacity: null,
          producedQuantity: null,
          remainingReason: null
        },
        planEngineerEditOptionsRules: {
          // 'isCompleted': [
          //   {required: true, message: '请选择是否完成', trigger: 'blur'}
          // ],
          // 'productionPlanningNumber': [
          //   {required: true, message: '请填写生产计划单号', trigger: 'blur'}
          // ],
          // 'capacity': [
          //   {required: true, message: '请填写产能', trigger: 'blur'}
          // ],
          // 'producedQuantity': [
          //   {required: true, message: '请填写已完成数量', trigger: 'blur'}
          // ],
        },

        isEditing: false,
        planEditOptionsData: {},
        planEditOptionsRules: {},


        isPending: false
      }
    },
    computed: {},
    watch: {
      pmcEditing(val) {
        this.pmcEdit = val;
        this.engineerEdit = false;
        this.isEditing = val;
      },
      engineerEditing(val) {
        this.pmcEdit = false;
        this.engineerEdit = val;
        this.isEditing = val;
      },

      totallyEditing(val) {
        this.pmcEdit = false;
        this.engineerEdit = false;
        this.isEditing = val;
      },

      row(val) {
        // if (this.pmcEditing && !this.engineerEditing) {
        //   Object.assign(this.planEditOptionsData, this.$options.data().planPMCEditOptionsData);
        //   this.planEditOptionsData = {
        //     isUrgent: val.isUrgent,
        //     line: val.line,
        //     schedulingQuantity: val.schedulingQuantity,
        //     lineChangeTime: val.lineChangeTime,
        //     remark: val.remark,
        //     planStartTime: !!val.planStartTime ? val.planStartTime : '',
        //     planCompleteTime: !!val.planCompleteTime ? val.planCompleteTime : ''
        //   };
        //   Object.assign(this.planEditOptionsRules, this.planPMCEditOptionsRules);
        //
        // } else if (!this.pmcEditing && this.engineerEditing) {
        //   Object.assign(this.planEditOptionsData, this.$options.data().planEngineerEditOptionsData);
        //   this.setProducedQuantity(val.id);
        //
        //   this.planEditOptionsData = {
        //     isCompleted: false,
        //     productionPlanningNumber: val.productionPlanningNumber,
        //     capacity: val.capacity,
        //     producedQuantity: val.producedQuantity,
        //     remainingReason: val.remainingReason
        //   };
        //   Object.assign(this.planEditOptionsRules, this.planEngineerEditOptionsRules);
        //   this.setProducedQuantity(val.id);
        // }
        // else if (this.totallyEditing) {
          this.$set(this.$data, 'planEditOptionsData', {
            activeProcessGroup:val.activeProcessGroup,
            id:val.id,
            isCompleted: false,
            productionPlanningNumber: val.productionPlanningNumber,
            producedQuantity: val.producedQuantity,
            remainingReason: val.remainingReason,
            remark: val.remark,

            // capacity: val.capacity,
            // isStarting: val.isStarting,
            // isUrgent: val.isUrgent,
            // line: val.line,
            // schedulingQuantity: val.schedulingQuantity,
            // lineChangeTime: val.lineChangeTime,validMark
            // planStartTime: !!val.planStartTime ? val.planStartTime : '',
            // planCompleteTime: !!val.planCompleteTime ? val.planCompleteTime : ''
          });
        // isCompleted  productionPlanningNumber   producedQuantity remainingReason remark
          Object.assign(this.planEditOptionsRules, this.planPMCEditOptionsRules);
          Object.assign(this.planEditOptionsRules, this.planEngineerEditOptionsRules);
          if(this.planEditOptionsData.id){
            this.setProducedQuantity(val.id);
          }
        // }
      }
    },

    methods: {
      //控制表单项目显示 params: [pmcEdit, engineerEdit]
      // permissionShow(statusArray) {
      //   let userType = this.$store.state.userType;
      //   if (statusArray[0] && userType === 'schedulingJMPMC') {
      //     return true;
      //   } else if (statusArray[1] && userType === 'engineer') {
      //     return true;
      //   } else if (userType === 'SuperAdmin') {
      //     return true;
      //   }
      // },

      resetEditPanel() {
        this.planEditOptionsData = {}
        eventBus.$emit('closeEditPanel')
      },

      submitEdit() {
        this.$refs['planEditForm'].validate((valid) => {
          if (valid && !this.isPending) {
            this.isPending = true;
            this.$openLoading();
            let options = {
              url: planDetailsEditUrl,
              data: {
                id: this.row.id
              }
            };
            Object.keys(this.planEditOptionsData).forEach(item => {
              // if (this.planEditOptionsData[item] !== null || this.planEditOptionsData[item] !== '') {
              options.data[item] = this.planEditOptionsData[item]
              // }
            });
            axiosFetch(options).then(response => {
              if (response.data.result === 200) {
                this.$alertSuccess("修改成功");
                eventBus.$emit('partlyReload');
                //this.reload();
              } else {
                this.$alertWarning(response.data.data)
              }
            }).catch(err => {
              console.log(err);
              this.$alertDanger("未知错误")
            }).finally(() => {
              this.isPending = false;
              this.$closeLoading();
            })
          }
        })
      },

      // submitEdit1() {
      //   this.$refs['planEditForm'].validate((valid) => {
      //     if (valid && !this.isPending) {
      //       this.isPending = true;
      //       this.$openLoading();
      //       // if (this.planEditOptionsData.isCompleted === true) {
      //       //   if (this.planEditOptionsData.producedQuantity < row.schedulingQuantity) {
      //       //     return;
      //       //   }
      //       // }
      //       let options = {
      //         url: planDetailsEditUrl,
      //         data: {
      //           id: this.row.id
      //         }
      //       };
      //       Object.keys(this.planEditOptionsData).forEach(item => {
      //         console.log(item,":",this.planEditOptionsData[item])
      //         if (this.planEditOptionsData[item] !== null || this.planEditOptionsData[item] !== '') {
      //           options.data[item] = this.planEditOptionsData[item]
      //         }
      //       });
      //       this.submitData(options)
      //     }
      //   })
      // },
      //
      //
      // async submitData(options) {
      //   let validMark = false;
      //   if (this.pmcEditing || this.totallyEditing) {
      //     let validateOptions = {
      //       url: planCheckCompleteTimeSelectUrl,
      //       data: options.data
      //     };
      //     if (this.pmcEditing && !this.engineerEditing) {
      //       validateOptions.data.capacity = this.row.capacity;
      //     }
      //     await axiosFetch(validateOptions).then(response => {
      //       if (response.data.result === 200) {
      //         if (response.data.data === true) {
      //           validMark = true
      //         } else {
      //           this.$alertInfo('排产超出生产能力，请重新排产或调整产能');
      //           this.isPending = false;
      //           this.$closeLoading();
      //         }
      //       } else {
      //         this.$alertWarning(response.data.data);
      //         this.isPending = false;
      //         this.$closeLoading();
      //       }
      //     }).catch(err => {
      //       this.$alertDanger('未知错误');
      //       this.isPending = false;
      //       this.$closeLoading();
      //     })
      //   }
      //   // if (validMark || this.engineerEditing) {
      //   if (validMark) {
      //     axiosFetch(options).then(response => {
      //       if (response.data.result === 200) {
      //         this.$alertSuccess("修改成功");
      //         eventBus.$emit('partlyReload');
      //         //this.reload();
      //       } else {
      //         this.$alertWarning(response.data.data)
      //       }
      //     }).catch(err => {
      //       console.log(err);
      //       this.$alertDanger("未知错误")
      //     }).finally(() => {
      //       this.isPending = false;
      //       this.$closeLoading();
      //     })
      //   }
      // },

      setProducedQuantity(id) {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          axiosFetch({
            url: planProducedQuantitySelectUrl,
            data: {
              id: id
            }
          }).then(response => {
            if (response.data.result === 200) {
              this.$set(this.planEditOptionsData, 'producedQuantity', response.data.data.planProducedQuantity)
            } else {
              this.$alertWarning('完成数量计算失败:\n' + response.data.data)
            }
          }).catch(err => {
            this.$alertDanger('未知错误')
          }).finally(() => {
            this.isPending = false;
            this.$closeLoading();
          })
        }
      }
    }
  }
</script>

<style scoped>

  .plan-edit-form {
    display: flex;
    flex-wrap: wrap;
  }

  .plan-edit-form-comp {
    padding: 0 10px;
  }

  .plan-edit-form-comp-text {
    width: 210px;
  }

  .plan-edit-form-comp-textarea {
    width: 100%;
  }

  .plan-edit-form-comp-text /deep/ .el-input {
    width: 210px;
  }
</style>
