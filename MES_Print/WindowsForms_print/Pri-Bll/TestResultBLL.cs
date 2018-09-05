using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Print_Message;
using TestResult.Param.DAL;

namespace TestResult.Param.BLL
{
    class TestResultBLL
    {
        TestResultDAL TRD = new TestResultDAL();


        public bool CheckOneBefStationBLL(string IMEInumber,string Station)
        {
            if (TRD.CheckOneStationDAL(IMEInumber,Station) > 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        public bool CheckTwoBefStationBLL(string IMEInumber, string Station1,string Station2)
        {
            if (TRD.CheckTwoStationDAL(IMEInumber, Station1,Station2) > 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        public bool CheckThreeBefStationBLL(string IMEInumber, string Station1,string Station2,string Station3)
        {
            if (TRD.CheckThreeStationDAL(IMEInumber, Station1,Station2,Station3) > 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        public bool CheckFourBefStationBLL(string IMEInumber, string Station1, string Station2, string Station3,string Station4)
        {
            if (TRD.CheckFourStationDAL(IMEInumber, Station1, Station2, Station3,Station4) > 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

    }
}
