<template>
  <div>
    <el-dialog
      title="详情"
      width="90%"
      :visible="isDetail"
      :close-on-click-modal="false"
      :close-on-press-escape="false"
      @close="closeDialog"
      append-to-body>
      <div class="dialog-header">
        <div class="header-item" v-for="item in logDetailHeader">
          <span class="detail-label">{{item.label}}:</span>
          <span class="detail-value">{{dialogData[item.key]}}</span>
        </div>
      </div>

      <div class="dialog-table">
        <el-table
          max-height="650"
          :data="tableData"
          :header-cell-style="{'text-align':'center','background-color':'#eeeeee'}"
          :row-style="{'height':'30px'}"
          :cell-style="{'padding':'0px','text-align':'center'}"
          border
          stripe
        >
          <el-table-column
            type="index"
            label="序号"
            fixed="left"
            width="60"
          >
          </el-table-column>

          <el-table-column v-for="(item,index) in logDetailTable"
                           :label="item.label"
                           :min-width="item['min-width']"
                           :prop="item.key">
          </el-table-column>
        </el-table>
      </div>
    </el-dialog>
  </div>
</template>

<script>
  import {logDetailHeader,logDetailTable} from '../../../../config/testApiConfig'
    export default {
      name: "LogDetailPanel",
      props:['isDetail','detailData'],
      data(){
        return {
          dialogData:{},
          logDetailHeader:logDetailHeader,
          logDetailTable:logDetailTable,
          tableData:[],
        }
      },


      async mounted(){
        this.dialogData = JSON.parse(JSON.stringify(this.detailData))
        await this.sliceData()
      },

      methods:{
        closeDialog(){
          this.$emit('update:isDetail',false)
        },

        sliceData(){
          let dataItemList =JSON.parse(JSON.stringify(this.detailData.station)).split("}}");
          console.log(dataItemList)
          // this.dialogData.settingType = dataItemList.shift()
          //掐头（类型）去尾（空）
          dataItemList.pop()
          let arrLength = (dataItemList.length > 10 ? dataItemList.length : '0'+dataItemList.length).toString()
          dataItemList[0] = dataItemList[0].replace(arrLength,"")
          let placeStrList =['IMEI域名','共有指令','功能测试','白卡测试'];
          placeStrList.forEach((item)=>{
            dataItemList.forEach((dataItem)=>{
              if(dataItem.indexOf(item) !==-1 ){
                dataItem = dataItem.replace(item,"")
                dataItem = {
                  place:item,
                  pro:dataItem.split('@@')[0],
                  orderAT:dataItem.split('@@')[1],
                  response:dataItem.split('@@')[2]
                }
                this.tableData.push(dataItem)
              }
            })
          })
        },
      },

    }
</script>

<style scoped>
  .dialog-header{
    width: 90%;
    display: flex;
    justify-content: space-between;
    margin-bottom: 40px;
  }
  .header-item{
    min-width: 300px;
    height: 45px;
    display: flex;
    color: #000;
    flex-direction: column;
    justify-content: space-between;
  }
  .detail-label{
    font-size: 14px;
    color: #000;
  }
  .detail-value{
    font-size: 16px;
    color: #999;
  }

</style>
