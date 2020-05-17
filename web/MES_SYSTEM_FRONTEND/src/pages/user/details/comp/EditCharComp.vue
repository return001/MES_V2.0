<template>
  <el-dialog
      title="编辑权限"
      :visible="isEditingAuthority"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      :show-close="false"
      append-to-body
      width="96%"
  >

    <div class="edit-char-tips">
      <p>点击表头可全选指定列</p>
    </div>
    <el-table
        :data="tableData"
        border
        size="mini"
        stripe
        height="500"
        style="width: 100%; margin-top: 20px"
        @header-click="toggleColumnStatus"
        :span-method="spanMethod">
      <el-table-column
          v-for="(item, index) in tableColumns"
          :key="index"
          :prop="item.key"
          :label="item.label"
          :min-width="item['min-width']">
        <template slot-scope="scope" >
          <span class="table-item" v-if="item.slot && scope.row[item.key] !== undefined">
            <el-checkbox size="small" v-model="tableData[scope.$index][item.key]"></el-checkbox>
          </span>
          <span class="table-item" v-else>
            {{scope.row[item.key]}}
          </span>
        </template>
      </el-table-column>
    </el-table>
    <span slot="footer" class="dialog-footer">
         <el-button size="small" @click="clearSelection" type="info">清空选择</el-button>
         <el-button size="small" @click="closeComp" type="primary">确认</el-button>
       </span>
  </el-dialog>
</template>

<script>
  import {
    funcNameMap,
    moduleNameMap,
    pageNameMap,
    getCharactersFuncList,
    permissionIndexMap,
    reversePermissionIndexMap,
    initCharactersFuncMap
  } from "../../../../config/charactersFunc";
  import {CharAthoritiesColumns} from "../../../../config/UserConfig";

  export default {
    name: "EditCharComp",
    props: ['isEditingAuthority', 'isAddOrEdit', 'editingAuthorities'],
    data() {
      return {
        authoritiesMap: undefined,
        tableColumns: CharAthoritiesColumns,
        tableData: [],
        srcTableData: [],


        mergeData: {},//合并行的记录
        mergePos: {},//mergeData中每项的索引
        mergeProp: ['func', 'module'],
        mergeKeys: ['func', 'module'],
      }
    },
    computed: {},
    created() {
      let map = new getCharactersFuncList();
      if (this.isAddOrEdit === 0) {
        this.authoritiesMap = map;
      }
      if (this.isAddOrEdit === 1) {
        this.authoritiesMap = initCharactersFuncMap(map, editingAuthorities).map;
      }
    },
    mounted() {
      this.initTableData();
    },

    methods: {
      closeComp() {
        this.$emit('update:authoritiesJSON', this.generateAuthoritiesJSON());
        this.$emit('update:isEditingAuthority', false)
      },

      /**
      **@description: 根据结构生成表单内容
      **@date: 2020/5/17 16:32
      **@author: DarkNin
      **@method: initTableData
      **@params:
      */
      initTableData() {
        let tempGroup = [];
        let findKey = (val, obj, compare = (a, b) => a === b) => {
          return Object.keys(obj).find(k => compare(obj[k], val))
        };
        let m = this.authoritiesMap;
        Object.keys(m).forEach(funcVal => {
          let funcKey = findKey(funcVal, funcNameMap);
          Object.keys(m[funcVal]).forEach(moduleVal => {
            let moduleKey = findKey(moduleVal, moduleNameMap);
            Object.keys(m[funcVal][moduleVal]).forEach(pageVal => {
              let pageKey = findKey(pageVal, pageNameMap);
              let length = m[funcVal][moduleVal][pageVal].length;
              let tableDataItem = {
                'func': funcKey,
                'module': moduleKey,
                'page': pageKey
              };
              for (let i = 0; i < length; i++) {
                tableDataItem[i.toString()] = m[funcVal][moduleVal][pageVal][i]
              }

              tempGroup.push(tableDataItem)

            })
          })
        });
        this.tableData = JSON.parse(JSON.stringify(tempGroup));
        this.srcTableData = JSON.parse(JSON.stringify(tempGroup));
        this.getSpanArr(this.tableData, this.mergeKeys)
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

      toggleColumnStatus(col, e) {
        if (permissionIndexMap[col.label] !== undefined) {
          let uncheckItem = this.tableData.find(k => {
            return k[col.property] === false
          });
          if (!!uncheckItem) {
            this.tableData = this.tableData.map(item => {
              if (item[col.property] !== undefined) {
                item[col.property] = true;
              }
              return item
            })
          } else {
            this.tableData = this.tableData.map(item => {
              if (item[col.property] !== undefined) {
                item[col.property] = false;
              }
              return item
            })
          }
        }
      },

      generateAuthoritiesJSON() {
        let tempGroup = [];
        this.tableData.forEach(item => {
          Object.keys(reversePermissionIndexMap).forEach(key => {
            if (item[key] === true) {
              tempGroup.push({
                'system': item['func'],
                'model': item['module'],
                'page': item['page'],
                'operation': reversePermissionIndexMap[key]
              })
            }
          })
        });
        return tempGroup;
      },

      clearSelection(){
        this.$set(this.$data, 'tableData', JSON.parse(JSON.stringify(this.srcTableData)))
      }
    }
  }
</script>

<style scoped>

  .table-item {
    white-space: nowrap;
  }

  .edit-char-tips {
    font-size: 12px;
    display: flex;
    height: 24px;
    line-height: 24px;
    width: 100%;
    justify-content: flex-end;
  }
</style>