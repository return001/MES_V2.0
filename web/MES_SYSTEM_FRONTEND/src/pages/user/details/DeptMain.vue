<template>
  <div id="dept-setting">
    <div class="query-comp">
      <rz-query-bar
          :list="queryConfig"
          :button-group="buttonGroup"
          :data.sync="queryCompData"/>
    </div>
    <div class="content-comp">
      <el-table
          :data="tableData"
          max-height="560"
          ref="tablecomponent"
          :span-method="spanMethod">
        <el-table-column v-for="(item, index) in tableColumns"
                         :key="index"
                         :prop="item.key"
                         :label="item.label"
                         :min-width="item['min-width']"
                         :formatter="item.formatter">
        </el-table-column>

        <el-table-column
            type="index"
            fixed="left"
            width="60">
        </el-table-column>

        <el-table-column
            label="操作"
            width="80"
            fixed="right"
        >
          <template slot-scope="scope">
            <el-tooltip content="编辑" placement="top">
              <el-button type="text" @click="editData(scope.row)" icon="el-icon-edit-outline"></el-button>
            </el-tooltip>
            <el-tooltip content="删除" placement="top">
              <el-button type="text" @click="deleteData(scope.row)" icon="el-icon-delete"></el-button>
            </el-tooltip>
          </template>
        </el-table-column>
      </el-table>
    </div>


    <el-dialog
        :title="editPanelTitle"
        :visible.sync="isEditing"
        :close-on-click-modal="false"
        :close-on-press-escape="false"
        @close="resetEditForm"
        width="500px">
      <rz-form-item
          :ref="formConfig.refName"
          v-if="isEditing"
          :config="formConfig"
          :list="formItemList"
          :rules="formRules"
          :data.sync="formData"/>
      <span slot="footer" class="dialog-footer">
         <el-button size="small" @click="closeEditPanel" type="info">取消</el-button>
         <el-button size="small" @click="submitFormEdit" type="primary">保存</el-button>
       </span>
    </el-dialog>
  </div>
</template>

<script>
  import {DeptQueryConfig, DeptTableColumns, DeptFormItems, DeptFormRules} from "../../../config/UserConfig";
  import {axiosFetch} from "../../../utils/fetchData";
  import {eSopFactorySelectUrl, deptAddUrl, deptDeleteUrl, deptEditUrl, selectDeptUrl} from "../../../config/globalUrl";
  import {MessageBox} from 'element-ui'

  export default {
    name: "DeptMain",
    inject: ['reload'],
    data() {
      return {
        queryConfig: DeptQueryConfig,
        buttonGroup: [
          {
            label: '重置条件',
            size: 'small',
            type: 'info',
            callback() {
            }
          },
          {
            label: '查询',
            size: 'small',
            type: 'primary',
            callback() {
            }
          },
          {
            label: '新增',
            size: 'small',
            type: 'primary',
            callback() {
            }
          },
        ],
        queryCompData: {},
        tableColumns: DeptTableColumns,
        tableData: [],

        editPanelTitle: '',
        isEditing: false,
        editType: 0, //0: add, 1: edit

        formConfig: {
          refName: 'deptFormComp',
          size: 'small',
          identity: 'dept-item-'
        },
        formItemList: DeptFormItems,
        formRules: DeptFormRules,
        formData: {},

        /*链接重定义*/
        formAddUrl: deptAddUrl,
        formDeleteUrl: deptDeleteUrl,
        formUpdateUrl: deptEditUrl,

        isPending: false,

        mergeData: {},//合并行的记录
        mergePos: {},//mergeData中每项的索引
        mergeProp: ['companyName'],
        mergeKeys: ['companyName'],
      }
    },

    created() {
      this.$openLoading();
      this.getFactoryList().then(data => {
        let selectList = data.map(item => {
          return {
            key: item.id,
            label: item.abbreviation
          }
        });
        this.queryConfig[0].selectList = selectList;
        this.formItemList[0].selectList = selectList;
        this.$closeLoading();
      })
    },

    mounted() {
      this.buttonGroup[0].callback = function () {
        this.queryCompData = {};
      };
      this.buttonGroup[1].callback = this.queryData;
      this.buttonGroup[2].callback = this.addData;
    },

    methods: {
      queryData() {
        this.$openLoading();
        this.isPending = true;
        let options = {
          url: selectDeptUrl,
          data: {}
        };
        Object.keys(this.queryCompData).forEach(key => {
          if (!!this.queryCompData[key]) {
            options.data[key] = this.queryCompData[key]
          }
        });
        axiosFetch(options).then(response => {
          if (response.data.code === 200) {
            this.getSpanArr(response.data.data, this.mergeKeys);
            this.tableData = response.data.data
          } else {
            this.$alertWarning(response.data.data)
          }
        }).catch((err) => {
          console.log(err);
          this.$alertDanger('未知错误');
        }).finally(() => {
          this.isPending = false;
          this.$closeLoading();
        })
      },

      addData() {
        this.editType = 0;
        this.editPanelTitle = '新增';
        this.isEditing = true;
      },

      editData(row) {
        this.editType = 1;
        this.editPanelTitle = '编辑';
        this.formData.id = row.id;
        this.formItemList.forEach(item => {
          this.formData[item.prop] = row[item.prop] === null ? '' : row[item.prop];
        });
        this.isEditing = true;
      },

      deleteData(row) {
        MessageBox.confirm('将删除该项目，是否继续?', '提示', {
          confirmButtonText: '确认',
          cancelButtonText: '取消',
          type: 'warning'
        }).then(() => {
          this.$openLoading();
          axiosFetch({
            url: this.formDeleteUrl,
            data: {
              id: row.id
            }
          }).then(response => {
            if (response.data.result === 200) {
              this.$alertSuccess('删除成功');
              this.reload();
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.$alertDanger("未知错误")
          }).finally(() => {
            this.$closeLoading();
          })
        }).catch(() => {

        })
      },

      getFactoryList() {
        return new Promise(resolve => {
          axiosFetch({
            url: eSopFactorySelectUrl,
            data: {
              pageNo: 1,
              pageSize: 2147483647
            }
          }).then(response => {
            if (response.data.result === 200) {
              resolve(response.data.data.list);
            } else {
              this.$alertWarning(response.data.data);
            }
          }).catch(() => {
            this.$alertDanger(`获取工厂列表失败，请刷新重试`)
          })
        })
      },
      getSpanArr(tableData, keyName) {
        keyName.forEach((kitem, k) => {
          tableData.forEach((data, i) => {
            if (i === 0) {
              this.mergeData[kitem] = this.mergeData[kitem] || [];
              this.mergeData[kitem].push(1);
              this.mergePos[kitem] = 0
            } else {
              // 判断当前元素与上一个元素是否相同
              if (data[kitem] === tableData[i - 1][kitem]) {
                this.mergeData[kitem][this.mergePos[kitem]] += 1;
                this.mergeData[kitem].push(0)
              } else {
                this.mergeData[kitem].push(1);
                this.mergePos[kitem] = i
              }
            }
          })
        });
      },
      spanMethod({row, column, rowIndex, columnIndex}) {
        if (this.mergeProp.includes(column.property)) {
          const _row = this.mergeData[column.property][rowIndex];
          const _col = _row > 0 ? 1 : 0;
          return {
            rowspan: _row,
            colspan: _col
          }
        }
      },

      resetEditForm() {
        this.formData = {};
      },

      closeEditPanel() {
        this.isEditing = false;
      },

      submitFormEdit() {
        let url;
        if (this.editType === 0) {
          url = this.formAddUrl
        } else {
          url = this.formUpdateUrl
        }
        if (!this.isPending) {
          this.$refs[this.formConfig.refName].validate(isValid => {
            if (isValid) {
              this.isPending = true;
              this.$openLoading();
              axiosFetch({
                url: url,
                data: this.formData
              }).then(response => {
                if (response.data.result === 200) {
                  this.$alertSuccess('操作成功');
                  this.reload();
                } else {
                  this.$alertWarning(response.data.data)
                }
              }).catch((err) => {
                console.log(err);
                this.$alertDanger('未知错误');
              }).finally(() => {
                this.isPending = false;
                this.$closeLoading();
              })
            } else {
              this.$alertInfo('请检查输入内容')
            }
          })
        }
      }

    }
  }
</script>

<style scoped>
  #dept-setting {
    position: relative;
    width: 100%;
    min-height: 600px;
    height: 100%;
  }

  #dept-setting /deep/ .el-button i {
    font-size: 14px;
    font-weight: bold;
  }

  .content-comp {
    box-sizing: border-box;
    padding: 10px 20px;
    background: #fff;
    border: 1px solid #ededed;
    border-radius: 5px;
    margin-top: 10px;
  }

</style>