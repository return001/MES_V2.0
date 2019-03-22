using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NetLogo_Message
{
    class G_NetMarkConfig
    {
        //制单号
        public string ZhiDan { get; set; }

        //模板
        public string Temlpate { get; set; }

        //IMEI当前位
        public string IMEIPresent { get; set; }

        //网标前缀
        public string NetMarkPre { get; set; }

        //网标后缀
        public string NetMarkSuf { get; set; }

        //网标位数
        public string NetMarkDig { get; set; }

        //RFID起始位
        public string RFIDStart { get; set; }

        //RFID终止位
        public string RFIDEnd { get; set; }

        //RFID当前位
        public string RFIDPresent { get; set; }

        //检查检验位复选框
        public int NoCheckCode { get; set; }

        //扫入IMEI和网标复选框
        public int InputAllMark { get; set; }

        //RFID自动递增复选框
        public int RFIDAutoAddMark { get; set; }

        //模板份数
        public int TemplateNum { get; set; }
        

    }
}
