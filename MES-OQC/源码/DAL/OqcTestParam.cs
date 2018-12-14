using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OqcTestParam
{
    class OqcTestParam1
    {
        //制单号
        public string Zhidan { get; set; }

        //IMEI号
        public string IMEI { get; set; }

        //发送的指令
        public string SendCommand { get; set; }

        //指令返回结果
        public string ReceiveValue { get; set; }

        //重测次数
        public int ReTestNum { get; set; }

        //最后一次重测时间
        public string ReTestTime { get; set; }

        //测试结果
        public int TestResule { get; set; }

        //测试者
        public string TestUser { get; set; }

        //测试时间
        public string TestTime { get; set; }


    }
}
