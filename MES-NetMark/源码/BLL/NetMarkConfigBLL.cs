using NetLogo_Message;
using NetMarkConfig.DAL;
using System.Collections.Generic;

namespace NetMarkConfig.BLL
{
    class NetMarkConfigBLL
    {
        NetMarkConfigDAL NMCD = new NetMarkConfigDAL();

        public void refeshConBLL()
        {
            NMCD.refreshCon();
        }

        public bool InsertPrintRecordParamBLL(string Zhidan, string Temlpate, string IMEIPresent, string NetMarkPre, string NetMarkSuf, string NetMarkDig, string RFIDStart, string RFIDEnd, string RFIDPresent, int NoCheckCode, int InputAllMark, int RFIDAutoAddMark,int TemplateNum)
        {
            if (NMCD.InsertPrintRecordParamDAL(Zhidan, Temlpate, IMEIPresent, NetMarkPre, NetMarkSuf, NetMarkDig, RFIDStart, RFIDEnd, RFIDPresent, NoCheckCode, InputAllMark, RFIDAutoAddMark, TemplateNum) > 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        public List<G_NetMarkConfig> selectNetMarkParamByzhidanBLL(string zhidan)
        {
            return NMCD.selectNetMarkParamByzhidanDAL(zhidan);
        }

        public int UpdatePrensentIMEIAndRFIDBLL(string Zhidan, string IMEIPresent, string RFIDPresent)
        {
            return NMCD.UpdatePrensentIMEIAndRFIDDAL(Zhidan, IMEIPresent, RFIDPresent);
        }

    }
}
