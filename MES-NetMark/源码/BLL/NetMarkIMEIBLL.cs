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


        public void refeshConBLL()
        {
            NMID.refreshCon();
        }


        public int InsertNetMarkBLL(string Zhidan, string IMEI, string Netmark, string RFID, string TemPlate)
        {
            return NMID.InsertNetMarkDAL(Zhidan, IMEI, Netmark,RFID,TemPlate);
        }

        public int DeleteAllByNetLogoOrImerBLL(string NetLogoOrImer)
        {
            return NMID.DeleteAllByNetLogoOrImerDAL(NetLogoOrImer);
        }

        public int CheckIMEIBLL(string ImeiNum)
        {
            return NMID.CheckIMEIDAL(ImeiNum);
        }

        public int CheckRFIDBLL(string RFIDNum)
        {
            return NMID.CheckRFIDDAL(RFIDNum);
        }

        public int CheckNetMarkBLL(string NetMankNum)
        {
            return NMID.CheckNetMarkDAL(NetMankNum);
        }

        public List<NetMarkIMEIMes> SelectIMEIByNetLogoOrImerBLL(string NetLogoOrImer)
        {
            return NMID.SelectIMEIByNetLogoOrImerDAL(NetLogoOrImer);
        }

        public string SelectIMEIBLL(string RFIDOrImer)
        {
            return NMID.SelectIMEIDAL(RFIDOrImer);
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

        public List<NetMarkIMEIMes> SelectResultByImeiOrNetmarkBLL(string NetLogoOrImer)
        {
            return NMID.SelectResultByImeiOrNetmarkDAL(NetLogoOrImer);
        }

        public int UpdateRePrintBLL(string IMEInumber)
        {
            return NMID.UpdateRePrintDAL(IMEInumber);
        }

        public string SelectIMEIByNetMarkBLL(string netmark)
        {
            return NMID.SelectIMEIByNetMarkDAL(netmark);
        }

    }
}
