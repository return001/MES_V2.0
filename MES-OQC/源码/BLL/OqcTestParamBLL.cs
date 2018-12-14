using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OqcTestParam.DAL;

namespace OqcTestParam.BLL
{
    class OqcTestParamBLL
    {
        OqcTestParamDAL OTPD = new OqcTestParamDAL();


        public void refeshConBLL()
        {
            OTPD.refreshCon();
        }

        public bool CheckImeiBLL(string IMEI)
        {
            if (OTPD.CheckImeiDAL(IMEI) == 0)
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        public int InsertTestResultBLL(string Zhidan, string IMEI, string SendCommand, string ReceiveValue, int TestResule, string TestUser)
        {
            return OTPD.InsertTestResultDAL(Zhidan, IMEI, SendCommand, ReceiveValue, TestResule, TestUser);
        }

        public List<OqcTestParam1> SelectOqcTestBLL(string Sqlstr)
        {
            return OTPD.SelectOqcTestDAL(Sqlstr);
        }


    }
}
