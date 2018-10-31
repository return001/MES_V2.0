using System;
using System.Collections.Generic;
using NetLogo_Message;
using ManuOrderParam.DAL;

namespace ManuOrderParam.BLL
{
    class ManuOrderParamBLL
    {
        ManuOrderParamDAL MOPD = new ManuOrderParamDAL();
        public List<Gps_ManuOrderParam> SelectZhidanNumBLL()
        {
            return MOPD.SelectZhidanNumDAL();
        }

        public List<Gps_ManuOrderParam> selectManuOrderParamByzhidanBLL(string ZhidanNum)
        {
            return MOPD.selectManuOrderParamByzhidanDAL(ZhidanNum);
        }



    }
}
