using DataRelativeSheet.DAL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataRelativeSheet.BLL
{
    class DataRelativeSheetBLL
    {
        DataRelativeSheetDAL DRSD = new DataRelativeSheetDAL();

        public void refeshConBLL()
        {
            DRSD.refreshCon();
        }

        public int InsertRelativeSheetBLL(string IMEI,string RFID,string Zhidan,string netmark)
        {
            return DRSD.InsertRelativeSheetDAL(IMEI,RFID,Zhidan,netmark);
        }

        public string CheckRelativeSheetBLL(string IMEI, string RFID,string netmark)
        {
            return DRSD.CheckRelativeSheetDAL(IMEI, RFID,netmark);
        }

    }
}
