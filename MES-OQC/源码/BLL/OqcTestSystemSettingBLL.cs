using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OqcTestSystemSetting.DAL;
using TestSystemSetting;

namespace OqcTestSystemSetting.BLL
{
    class OqcTestSystemSettingBLL
    {
        OqcTestSystemSettingDAL OTSSD = new OqcTestSystemSettingDAL();


        public void refeshConBLL()
        {
            OTSSD.refreshCon();
        }

        public List<OqcTestSystem> SelectZhidanNumBLL()
        {
            return OTSSD.SelectZhidanNumDAL();
        }

        public List<string> SelectCommandBLL(string zhidan)
        {
            return OTSSD.SelectCommandDAL(zhidan);
        }

        public int UpdateCommandBLL(string UpdateSql)
        {
            return OTSSD.UpdateCommandDAL(UpdateSql);
        }

    }
}
