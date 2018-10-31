using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using HisNetMarkIMEI.DAL;

namespace HisNetMarkIMEI.BLL
{
    class HisNetMarkIMEIBLL
    {
        HisNetMarkIMEIDAL HNMID = new HisNetMarkIMEIDAL();
        public int InsertHisNetMarkDAL(string Zhidan, string IMEI, string Netmark)
        {
            return HNMID.InsertHisNetMarkDAL(Zhidan, IMEI, Netmark);
        }
    }
}
