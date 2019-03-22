using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NetLogo_Message
{
    public class NetMarkIMEIMes
    {
    
        //IMEI起始位
        public string NetMark { get; set; }

        //IMEI终止位
        public string IMEI { get; set; }

        //制单号
        public string zhidan { get; set; }

        //RFID
        public string RFID { get; set; }

        //机身打印时间
        public string JS_PrintTime { get; set; }

        //机身打印模板
        public string JS_Template { get; set; }

        //机身重打次数
        public int RePrintNum { get; set; }

        //机身首次重打印时间
        public string RePrintFirstTime { get; set; }

        //机身末次重打印时间
        public string RePrintEndTime { get; set; }
    }
}
