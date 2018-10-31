using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NetLogo_Message
{
    public class Gps_ManuOrderParam
    {
        //制单号
        public string ZhiDan { get; set; }

        //IMEI起始位
        public string IMEIStart { get; set; }

        //IMEI终止位
        public string IMEIEnd { get; set; }

    }
}
