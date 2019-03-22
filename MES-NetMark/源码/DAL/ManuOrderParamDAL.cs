using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NetLogo_Message;
using System.Configuration;
using System.Data.SqlClient;

namespace ManuOrderParam.DAL
{
    class ManuOrderParamDAL
    {
        string conStr = ConfigurationManager.ConnectionStrings["conn1"].ConnectionString;

        public void refreshCon()
        {
            conStr = ConfigurationManager.ConnectionStrings["conn1"].ConnectionString;
        }

        //返回制单号
        public List<Gps_ManuOrderParam> SelectZhidanNumDAL()
        {
            using (SqlConnection conn1 = new SqlConnection(conStr))
            {
                conn1.Open();
                List<Gps_ManuOrderParam> list = new List<Gps_ManuOrderParam>();
                using (SqlCommand command = conn1.CreateCommand())
                {
                    command.CommandText = "SELECT * FROM dbo.Gps_ManuOrderParam WHERE Status='1' OR Status='0' ORDER BY ZhiDan";
                    SqlDataReader dr = command.ExecuteReader();
                    while (dr.Read())
                    {
                        list.Add(new Gps_ManuOrderParam()
                        {
                            ZhiDan = dr.GetString(1)
                        });
                    }
                    return list;
                }
            }
        }

        //根据制单号返回该制单相关信息
        public List<Gps_ManuOrderParam> selectManuOrderParamByzhidanDAL(string ZhidanNum)
        {
            using (SqlConnection conn1 = new SqlConnection(conStr))
            {
                conn1.Open();
                List<Gps_ManuOrderParam> list = new List<Gps_ManuOrderParam>();
                using (SqlCommand command = conn1.CreateCommand())
                {
                    command.CommandText = "SELECT IMEIStart,IMEIEnd,JST_template FROM dbo.Gps_ManuOrderParam WHERE (Status='1' OR Status='0') AND ZhiDan='" + ZhidanNum + "'";
                    SqlDataReader dr = command.ExecuteReader();
                    while (dr.Read())
                    {
                        list.Add(new Gps_ManuOrderParam()
                        {
                            IMEIStart = dr.IsDBNull(0) ? "" : dr.GetString(0),
                            IMEIEnd = dr.IsDBNull(1) ? "" : dr.GetString(1),
                            JST_template = dr.IsDBNull(2)?"":dr.GetString(2)
                        });
                    }
                    return list;
                }
            }
        }

    }
}
