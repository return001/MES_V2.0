using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NetMarkIMEI.DAL;
using NetLogo_Message;

namespace NetMarkIMEI.BLL
{
    class NetMarkIMEIBLL
    {
        NetMarkIMEIDAL NMID = new NetMarkIMEIDAL();

        public int InsertNetMarkBLL(string Zhidan, string IMEI, string Netmark)
        {
            return NMID.InsertNetMarkDAL(Zhidan, IMEI, Netmark);
        }

        public int DeleteAllByNetLogoOrImerBLL(string NetLogoOrImer)
        {
            return NMID.DeleteAllByNetLogoOrImerDAL(NetLogoOrImer);
        }

        public int CheckIMEIBLL(string ImeiNum)
        {
            return NMID.CheckIMEIDAL(ImeiNum);
        }

        public int CheckNetMarkBLL(string NetMankNum)
        {
            return NMID.CheckNetMarkDAL(NetMankNum);
        }

        public List<NetMarkIMEIMes> SelectIMEIByNetLogoOrImerBLL(string NetLogoOrImer)
        {
            return NMID.SelectIMEIByNetLogoOrImerDAL(NetLogoOrImer);
        }

        public List<NetMarkIMEIMes> selectNetMarkIMEIByzhidanBLL(string ZhidanNum)
        {
            return NMID.selectNetMarkIMEIByzhidanDAL(ZhidanNum);
        }

        public List<NetMarkIMEIMes> selectNetMarkIMEIByImeiRangeBLL(string ImeiStart,string ImeiEnd)
        {
            return NMID.selectNetMarkIMEIByImeiRangeDAL(ImeiStart, ImeiEnd);
        }

        public string selectNetMarkByIMEIBLL(string IMEI)
        {
            return NMID.selectNetMarkByIMEIDAL(IMEI);
        }

        public string SelectPresentImeiBLL(string Zhidan)
        {
            return NMID.SelectPresentImeiDAL(Zhidan);
        }

        public string SelectResultByImeiOrNetmarkBLL(string NetLogoOrImer)
        {
            return NMID.SelectResultByImeiOrNetmarkDAL(NetLogoOrImer);
        }

    }
}
