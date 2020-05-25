<template>
  <div id="char-setting">
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
          ref="tablecomponent">
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
      <div class="edit-authority-btn">
        <el-button size="small" type="plain" @click="isEditingAuthority = true">编辑权限</el-button>
      </div>
      <span slot="footer" class="dialog-footer">
         <el-button size="small" @click="closeEditPanel" type="info">取消</el-button>
         <el-button size="small" @click="submitFormEdit" type="primary">保存</el-button>
       </span>

      <edit-char-comp
          v-if="isEditing"
          :is-editing-authority.sync="isEditingAuthority"
          :is-add-or-edit="editType"
          :editing-authorities="editingAuthorities"
          @update:authoritiesJSON="authoritiesJSON = $event"
      />
    </el-dialog>
  </div>
</template>

<script>
  import {
    charAddUrl,
    charDeleteUrl,
    charUpdateUrl,
    eSopFactorySelectUrl,
    selectCharUrl,
    getAuthorities,
    selectDeptUrl
  } from "../../../config/globalUrl";
  import {CharFormItems, CharFormRules, CharQueryConfig, CharTableColumns} from "../../../config/UserConfig";
  import {MessageBox} from 'element-ui';
  import {axiosFetch} from "../../../utils/fetchData";
  import EditCharComp from "./comp/EditCharComp";

  export default {
    name: "CharMain",
    components: {EditCharComp},
    inject: ['reload'],
    data() {
      return {
        queryConfig: CharQueryConfig,
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
        tableColumns: CharTableColumns,
        tableData: [],

        editPanelTitle: '',
        isEditing: false,
        editType: 0, //0: add, 1: edit

        formConfig: {
          refName: 'charFormComp',
          size: 'small',
          identity: 'char-item-'
        },
        formItemList: CharFormItems,
        formRules: CharFormRules,
        formData: {},

        /*链接重定义*/
        formAddUrl: charAddUrl,
        formDeleteUrl: charDeleteUrl,
        formUpdateUrl: charUpdateUrl,

        isPending: false,

        srcDeptList: [],
        deptList: [],

        /*权限相关*/
        isEditingAuthority: false,
        authoritiesJSON: [],
        editingAuthorities: [],

      }
    },
    watch: {
      queryCompData: function (val, oldVal) {
        if (val.company && val.company !== oldVal.company) {
          this.queryCompData.department = '';
          this.queryConfig[1].selectList = this.srcDeptList.filter(item => {
            return item.company === val.company
          }).map(item => {
            return {
              key: item.id,
              label: item.name
            }
          });
        }
      },
      formData: function (val, oldVal) {
        if (val.company && val.company !== oldVal.company) {
          this.formData.department = '';
          this.formItemList[1].selectList = this.srcDeptList.filter(item => {
            return item.company === val.company
          }).map(item => {
            return {
              key: item.id,
              label: item.name
            }
          });
        }
      }
    },

    async created() {
      this.$openLoading();
      await this.getFactoryList().then(data => {
        let selectList = data.map(item => {
          return {
            key: item.id,
            label: item.abbreviation
          }
        });
        this.queryConfig[0].selectList = selectList;
        this.formItemList[0].selectList = selectList;
      });
      await this.getDepartmentList().then(data => {
        this.srcDeptList = data;

        this.queryConfig[1].selectList = this.srcDeptList.map(item => {
          return {
            key: item.id,
            label: item.name
          }
        });
        this.formItemList[1].selectList = this.srcDeptList.map(item => {
          return {
            key: item.id,
            label: item.name
          }
        });
      });
      this.$closeLoading();
    },

    mounted() {
      this.buttonGroup[0].callback = () => {
        this.queryCompData = {};
      };
      this.buttonGroup[1].callback = this.queryData;
      this.buttonGroup[2].callback = this.addData;

      this.queryData();
    },

    methods: {
      queryData() {
        this.$openLoading();
        this.isPending = true;
        let options = {
          url: selectCharUrl,
          data: {}
        };
        Object.keys(this.queryCompData).forEach(key => {
          if (!!this.queryCompData[key]) {
            options.data[key] = this.queryCompData[key]
          }
        });
        axiosFetch(options).then(response => {
          if (response.data.result === 200) {
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

      async editData(row) {
        this.editType = 1;
        this.editPanelTitle = '编辑';
        this.formData.id = row.id;
        this.formItemList.forEach(item => {
          this.formData[item.prop] = row[item.prop] === null ? '' : row[item.prop];
        });
        this.editingAuthorities = await this.getCharAuthority(row.id);
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

      getDepartmentList() {
        return new Promise(resolve => {
          axiosFetch({
            url: selectDeptUrl,
            data: {
            }
          }).then(response => {
            if (response.data.result === 200) {
              resolve(response.data.data);
            } else {
              this.$alertWarning(response.data.data);
            }
          }).catch(() => {
            this.$alertDanger(`获取部门列表失败，请刷新重试`)
          })
        })
      },

      getCharAuthority(id) {
        return new Promise(resolve => {
          axiosFetch({
            url: getAuthorities,
            data: {
              role: id
            }
          }).then(response => {
            if (response.data.result === 200) {
              resolve(response.data.data['webAuthorities']);
            } else {
              this.$alertWarning(response.data.data);
            }
          }).catch(() => {
            this.$alertDanger(`获取部门列表失败，请刷新重试`)
          })
        })
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
              let options = {
                url: url,
                data: this.formData
              };
              if ((this.editType === 1 && this.authoritiesJSON.length > 0) || this.editType === 0) {
                options.data.authority = JSON.stringify(this.authoritiesJSON);
              }

              axiosFetch(options).then(response => {
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

  #char-setting {
    position: relative;
    width: 100%;
    min-height: 600px;
    height: 100%;
  }

  #char-setting /deep/ .el-button i {
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

  .edit-authority-btn {
    box-sizing: border-box;
    padding: 5px;
  }
  .edit-authority-btn .el-button {
    width: 100%;
  }

</style>