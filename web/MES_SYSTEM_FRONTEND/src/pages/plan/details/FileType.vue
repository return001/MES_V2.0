<template>
  <div id="plan-file-type">
    <div class="plan-file-type-main">
      <div class="query-comp">
        <div class="query-comp-container" v-for="(item, index) in fileTypeQueryOptions" :key="index">
          <!--纯文本框-->
          <div class="query-comp-text" v-if="item.type === 'text'">
            <label :for="item.key + index">{{item.label}}:</label>
            <el-input v-model="thisQueryOptions[item.key].value"
                      :id="item.key + index"
                      :placeholder="'请填写' + item.label"
                      size="small"
                      clearable></el-input>
          </div>
        </div>
        <div class="query-comp-container">
          <el-button type="info" @click="initQueryOptions" size="small">重置条件</el-button>
        </div>
        <div class="query-comp-container">
          <el-button type="primary" size="small" @click="queryData">查询</el-button>
        </div>
        <div class="query-comp-container">
          <el-button type="primary" size="small" @click="editData('add')">新增文件类型</el-button>
        </div>
      </div>
    </div>

    <div class="content-comp">
      <el-table
        :data="tableData"
        max-height="560"
        ref="tablecomponent"
        stripe>
        <el-table-column v-for="(item, index) in tableColumns"
                         :key="index"
                         :prop="item.key"
                         :label="item.label"
                         :min-width="item['min-width']"
                         :formatter="item.formatter">
        </el-table-column>

<!--        <el-table-column-->
<!--          type="index"-->
<!--          :index="indexMethod"-->
<!--          fixed="left"-->
<!--          width="60">-->
<!--        </el-table-column>-->
        <el-table-column
          label="操作"
          width="140"
          fixed="right"
        >
          <template slot-scope="scope">
            <el-tooltip content="编辑订单" placement="top">
              <el-button type="text" @click="editData('edit', scope.row)" icon="el-icon-t-edit"></el-button>
            </el-tooltip>
            <el-tooltip content="删除" placement="top">
              <el-button
                type="text"
                @click="deleteFileType(scope.row)"
                icon="el-icon-delete"></el-button>
            </el-tooltip>
          </template>
        </el-table-column>
      </el-table>

      <!--分页控制-->
      <el-pagination
        background
        :current-page.sync="paginationOptions.currentPage"
        :page-sizes="[20, 40, 80]"
        :page-size.sync="paginationOptions.pageSize"
        layout="total, sizes, prev, pager, next, jumper"
        :total="paginationOptions.total"
        @current-change="pageChange"
        @size-change="queryData"
        class="page-pagination">
      </el-pagination>

      <!-- 新增&&编辑-->
      <el-dialog
        :title="editPanelTitle"
        :visible.sync="isFileEdit"
        :close-on-click-modal="false"
        :close-on-press-escape="false"
        @closed="closeEditFilePanel"
        width="730px">

        <el-form
          ref="fileEditForm"
          :model="fileTypeOptionsData"
          class="file-edit-form"
          label-position="top"
          @submit.native.prevent
          :rules="fileEditOptionsRules">

          <el-form-item
            size="small"
            class="file-edit-form-comp"
            v-for="(item, index) in fileTypeEditOptions"
            :key="index"
            :label="item.label + '：'"
            :prop="item.key">
            <div class="file-edit-form-comp-text" v-if="item.type === 'text'">
              <el-input
                type="text"
                :id="'edit' + item.key + index"
                :placeholder="'请填写' + item.label"
                clearable
                autocomplete="off"
                v-model="fileTypeOptionsData[item.key]"></el-input>
            </div>
          </el-form-item>


        </el-form>
        <span slot="footer" class="dialog-footer">
        <el-button size="small" @click="closeEditFilePanel" type="info">取消</el-button>
        <el-button size="small" @click="submitEditFileType" type="primary">保存</el-button>
      </span>
      </el-dialog>
    </div>
  </div>

</template>

<script>
  import {
    fileTypeQueryOptions,
    fileTypeTableColumns,
    fileTypeEditOptions,
    fileEditOptionsRules,
    sessionFactory,
  } from "../../../config/planConfig";
  import {axiosFetch} from "../../../utils/fetchData";
  import {
    planFileTypeSelectUrl,
    planFileTypeDeleteUrl,
    planFileTypeEditUrl,
    planFileTypeAddUrl, planOrderDeleteUrl
  } from "../../../config/globalUrl";
  import {MessageBox} from 'element-ui'

  export default {
    name: "FileType",
    inject: ['reload'],
    data() {
      return {
        fileTypeQueryOptions: fileTypeQueryOptions,
        fileTypeEditOptions:fileTypeEditOptions,
        fileEditOptionsRules:fileEditOptionsRules,
        thisQueryOptions: {},
        tableData: [],
        tableColumns: fileTypeTableColumns,

        paginationOptions: {
          currentPage: 1,
          pageSize: 65535,
          total: 0
        },
        isFileEdit:false,
        isPending:false,
        fileTypeOptionsData:{},
        fileTypeTitle:""
      }
    },
    computed: {
      editPanelTitle: function () {
        if (this.fileTypeTitle === 'edit') {
          return '编辑'
        } else if (this.fileTypeTitle === 'add') {
          return '新增'
        }
      },
    },
    created() {
      this.initQueryOptions();
    },
    mounted() {
      this.fetchData()
    },
    methods: {
      initQueryOptions: function () {
        this.fileTypeQueryOptions.forEach(item => {
          this.$set(this.thisQueryOptions, item.key, {
            type: item.type,
            value: ''
          })
        })
      },


      indexMethod: function (index) {
        return index + (this.paginationOptions.currentPage - 1) * this.paginationOptions.pageSize + 1
      },

      createQueryString: function () {
        let queryString = "";
        Object.keys(this.thisQueryOptions).forEach(item => {
          let subObj = this.thisQueryOptions[item];
          let itemCopy = item.replace(/([A-Z])/g, "_$1").toLowerCase(); //驼峰转下划线
          switch (subObj.type) {
            case 'text':
              //possible value: null [empty string] [string]
              if (!!subObj.value) {
                queryString += queryString.length === 0 ? (itemCopy + '#like#' + subObj.value) : ('#&#' + itemCopy + '#like#' + subObj.value)
              }
              break;
            case 'select':
              //possible value: [empty string] [string]
              if (subObj.value !== '') {
                queryString += queryString.length === 0 ? (itemCopy + '#=#' + subObj.value) : ('#&#' + itemCopy + '#=#' + subObj.value)
              }
              break;
            case 'timeRange':
              //possible value: null [array] [empty string]
              if (!!subObj.value) {
                queryString += (queryString.length === 0 ? (itemCopy + '#>#' + subObj.value[0]) : ('#&#' + itemCopy + '#>#' + subObj.value[0])) + '#&#' + itemCopy + '#>#' + subObj.value[1]
              }
              break;
          }
        });
        this.queryString = queryString;
      },

      pageChange: function () {
        this.createQueryString();
        this.fetchData();
      },

      //点击查询
      queryData: function () {
        this.paginationOptions.pageNo = 1;
        this.paginationOptions.total = 0;
        this.createQueryString();
        this.fetchData();
      },

      fetchData: function () {
        if (!this.isPending) {
          this.isPending = true;
          this.$openLoading();
          let options = {
            url: planFileTypeSelectUrl,
            data: {
              factory:sessionFactory,
              pageNo: this.paginationOptions.currentPage,
              pageSize: this.paginationOptions.pageSize,
            }
          };
          if (this.queryString !== '') {
            options.data.filter = this.queryString;
          }
          options.data.factory=sessionFactory
          axiosFetch(options).then(response => {
            if (response.data.result === 200) {
              this.tableData = response.data.data.list;
              this.paginationOptions.currentPage = response.data.data.pageNumber;
              this.paginationOptions.total = response.data.data.totalRow;
            } else {
              this.$alertWarning(response.data.data)
            }
          }).catch(err => {
            this.$alertDanger('未知错误')
          }).finally(() => {
            this.isPending = false;
            this.$closeLoading();
          })
        }
      },

      editData: function (type, val) {
          if (type === 'edit') {
            this.fileTypeTitle = 'edit';
            Object.keys(val).forEach(item => {
              this.fileTypeEditOptions.forEach(option => {
                if (item === option.key && val[item] !== null) {
                  this.$set(this.fileTypeOptionsData, item, val[item])
                }
              })
            });
            this.$set(this.fileTypeOptionsData, 'id', val.id)
            this.isFileEdit = true;
          } else if (type === 'add') {
            this.fileTypeTitle = 'add';
            this.isFileEdit = true;
          }
      },

      //删除
      deleteFileType(val){
        MessageBox.confirm('将作删除该文件类型，是否继续?', '提示', {
          confirmButtonText: '确认',
          cancelButtonText: '取消',
          type: 'warning'
        }).then(() => {
          this.$openLoading();
          axiosFetch({
            url: planFileTypeDeleteUrl,
            data: {
              id: val.id,
            }
          }).then(response => {
            if (response.data.result === 200) {
              this.$alertSuccess('删除成功');
              this.partlyReload();
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

      //保存
      submitEditFileType(){
        this.$refs['fileEditForm'].validate((isValid) => {
          if(isValid){
            this.isPending = true;
            this.$openLoading();
            this.fileTypeOptionsData.factory = sessionFactory
            let options= {
              url:"",
              data: this.fileTypeOptionsData
            }

            if(this.fileTypeTitle === "edit"){
              options.url = planFileTypeEditUrl
            }else if (this.fileTypeTitle === 'add'){
              options.url = planFileTypeAddUrl
            }
            axiosFetch(options).then(response => {
              if (response.data.result === 200) {
                this.$alertSuccess('操作成功');
                this.closeEditFilePanel();
                this.isDisabled= true;
              } else {
                this.$alertWarning(response.data.data)
              }
            }).catch(err => {
              this.$alertDanger("未知错误")

            }).finally(() => {
              this.isPending = false;
              this.$closeLoading();
              this.partlyReload();
              //this.reload();
            })

          }else{
            this.$alertInfo('请完善表单信息')
          }
        })
      },

      //取消
      closeEditFilePanel(){
        this.isFileEdit = false;
        this.fileTypeOptionsData = {};
      },

      /*局部刷新*/
      partlyReload() {
        this.isPending = false;
        let _partlyReload = stashData => {
          let obj = {};
          stashData.forEach(item => {
            obj[item] = this.$data[item]
          });
          this.$store.commit('setStashData', obj);
          Object.assign(this.$data, this.$options.data());
          Object.assign(this.$data, this.$store.state.stashData);
          this.queryData();
          this.$store.commit('setStashData', {});
        };
        _partlyReload(['thisQueryOptions'])
      },




    }
  }
</script>

<style scoped>
  #plan-file-type /deep/ .el-button i{
    font-size: 17px;
    font-weight: bold;
  }

  #plan-file-type {
    position: relative;
    width: 100%;
    min-height: 600px;
    height: 100%;
  }

  .plan-file-type-main {
  }

  .query-comp {
    background-color: #ffffff;
    border: 1px solid #eeeeee;
    border-radius: 5px;
    padding: 10px 20px;
    display: flex;
    flex-wrap: wrap;
    min-height: 60px;
  }

  .query-comp-container {
    margin: 0 16px 10px 0;
  }

  .query-comp label {
    display: block;
    line-height: 24px;
    font-size: 14px;
  }

  .query-comp-text {
    width: 220px;
  }

  .query-comp-select {
    width: 220px !important;
  }

  .query-comp-select .el-select {
    width: 100%;
  }

  .query-comp-select /deep/ input {
    padding-right: 0;
  }

  .query-comp-container .el-button {
    margin-top: 24px;
    padding: 8px 20px;
    width: auto;
  }

  .content-comp {
    background-color: #ffffff;
    border: 1px solid #eeeeee;
    border-radius: 5px;
    padding: 10px;
    margin-top: 10px;
  }

  .file-edit-form {
    display: flex;
    flex-wrap: wrap;
  }

  .file-edit-form-comp {
    padding: 0 10px;
  }

  .file-edit-form-comp label {
    font-size: 15px;
    line-height: 24px;
    padding: 0;
  }

  .file-edit-form-comp-text {
    width: 210px;
  }

  .file-edit-form-comp-text /deep/ .el-input {
    width: 210px;
  }

</style>
