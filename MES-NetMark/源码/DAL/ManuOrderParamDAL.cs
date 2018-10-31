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
        private static readonly string conStr = ConfigurationManager.ConnectionStrings["conn1"].ConnectionString;

        //返回制单号
        public List<Gps_ManuOrderParam> SelectZhidanNumDAL()
        {
            SqlConnection conn1 = new SqlConnection(conStr);
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

        //根据制单号返回该制单相关信息
        public List<Gps_ManuOrderParam> selectManuOrderParamByzhidanDAL(string ZhidanNum)
        {
            SqlConnection conn1 = new SqlConnection(conStr);
            conn1.Open();
            List<Gps_ManuOrderParam> list = new List<Gps_ManuOrderParam>();
            using (SqlCommand command = conn1.CreateCommand())
            {
                command.CommandText = "SELECT * FROM dbo.Gps_ManuOrderParam WHERE ZhiDan='" + ZhidanNum + "'";
                SqlDataReader dr = command.ExecuteReader();
                while (dr.Read())
                {
                    list.Add(new Gps_ManuOrderParam()
                    {
                        IMEIStart = dr.GetString(14),
                        IMEIEnd = dr.GetString(15),
                    });
                }
                return list;
            }
        }

    }
}
