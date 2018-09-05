using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ExcelPrint.Param.DAL;
using Print_Message;

namespace ExcelPrint.Param.Bll 
{
    class ManuExcelPrintParamBLL
    {
        ManuExcelPrintParamDAL MEPPD = new ManuExcelPrintParamDAL();

        public bool CheckIMEIBLL(string IMEI1,string IMEI2)
        {
            if (MEPPD.CheckIMEI1OrIMEI2DAL(IMEI1,IMEI2) > 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        public bool InsertManuExcelPrintBLL(List<ManuExcelPrintParam> list)
        {
            if (MEPPD.InsertManuExcelPrintDAL(list) > 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        public List<ManuExcelPrintParam> SelectByImei1to5BLL(string IMEI)
        {
            return MEPPD.SelectByImei1to5DAL(IMEI);
        }

    }
}
