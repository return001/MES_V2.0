using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Configuration;
using System.Data.SqlClient;
using NetLogo_Message;

namespace NetMarkIMEI.DAL
{
    class NetMarkIMEIDAL
    {
        private static readonly string conStr = ConfigurationManager.ConnectionStrings["conn2"].ConnectionString;

        //插入数据到NetMarkIMEI表
        public int InsertNetMarkDAL(string Zhidan,string IMEI,string Netmark)
        {
            SqlConnection conn2 = new SqlConnection(conStr);
            conn2.Open();
            using (SqlCommand command = conn2.CreateCommand())
            {
                string DtNow = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss:fff");
                command.CommandText = "INSERT INTO [dbo].[NetMarkIMEI]([NetMark], [IMEI], [Zhidan], [NMTime]) VALUES ('"+Netmark+"', '"+IMEI+"','"+Zhidan+"', '"+ DtNow + "');";
                return command.ExecuteNonQuery();
            }
        }

        //判断IMEI是否存在
        public int CheckIMEIDAL(string ImeiNum)
        {
            SqlConnection conn2 = new SqlConnection(conStr);
            conn2.Open();
            using (SqlCommand command = conn2.CreateCommand())
            {
                command.CommandText = "Select Id from NetMarkIMEI WHERE IMEI='" + ImeiNum + "'";
                if (Convert.ToString(command.ExecuteScalar()) != "")
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
        }

        //判断网标是否存在
        public int CheckNetMarkDAL(string NetMankNum)
        {
            SqlConnection conn2 = new SqlConnection(conStr);
            conn2.Open();
            using (SqlCommand command = conn2.CreateCommand())
            {
                command.CommandText = "Select Id from NetMarkIMEI WHERE NetMark='" + NetMankNum + "'";
                if (Convert.ToString(command.ExecuteScalar()) != "")
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
        }

        //删除数据根据网标或IMEI
        public int DeleteAllByNetLogoOrImerDAL(string NetLogoOrImer)
        {
            SqlConnection conn2 = new SqlConnection(conStr);
            conn2.Open();
            using (SqlCommand command = conn2.CreateCommand())
            {
                command.CommandText = "Delete NetMarkIMEI  WHERE NetMark='" + NetLogoOrImer + "' OR IMEI = '"+ NetLogoOrImer + "'";
                return command.ExecuteNonQuery();
            }
        }


        //根据IMEI或网标获取IMEI
        public List<NetMarkIMEIMes> SelectIMEIByNetLogoOrImerDAL(string NetLogoOrImer)
        {
            SqlConnection conn2 = new SqlConnection(conStr);
            conn2.Open();
            List<NetMarkIMEIMes> list = new List<NetMarkIMEIMes>();
            using (SqlCommand command = conn2.CreateCommand())
            {
                command.CommandText = "SELECT NetMark,IMEI FROM [dbo].[NetMarkIMEI] where NetMark ='" + NetLogoOrImer + "' OR IMEI = '" + NetLogoOrImer + "'";
                SqlDataReader dr = command.ExecuteReader();
                while (dr.Read())
                {
                    list.Add(new NetMarkIMEIMes()
                    {
                        NetMark = dr.GetString(0),
                        //NetMark = dr.GetString(dr.GetOrdinal("NetMark")),
                        IMEI = dr.GetString(1)
                    });
                }
                return list;
            }
        }

        //根据制单号查找所有网标和IMEI
        public List<NetMarkIMEIMes> selectNetMarkIMEIByzhidanDAL(string ZhidanNum)
        {
            SqlConnection conn2 = new SqlConnection(conStr);
            conn2.Open();
            List<NetMarkIMEIMes> list = new List<NetMarkIMEIMes>();
            using (SqlCommand command = conn2.CreateCommand())
            {
                command.CommandText = "SELECT NetMark,IMEI FROM dbo.NetMarkIMEI WHERE Zhidan = '"+ZhidanNum+"'";
                SqlDataReader dr = command.ExecuteReader();
                while (dr.Read())
                {
                    list.Add(new NetMarkIMEIMes()
                    {
                        NetMark = dr.GetString(0),
                        IMEI = dr.GetString(1)
                    });
                }
                return list;
            }
        }

        //根据IMEI范围查找所有网标和IMEI
        public List<NetMarkIMEIMes> selectNetMarkIMEIByImeiRangeDAL(string ImeiStart, string ImeiEnd)
        {
            SqlConnection conn2 = new SqlConnection(conStr);
            conn2.Open();
            List<NetMarkIMEIMes> list = new List<NetMarkIMEIMes>();
            using (SqlCommand command = conn2.CreateCommand())
            {
                command.CommandText = "SELECT NetMark,IMEI FROM dbo.NetMarkIMEI WHERE (IMEI>='"+ImeiStart+"' AND IMEI<='"+ImeiEnd+"')";
                SqlDataReader dr = command.ExecuteReader();
                while (dr.Read())
                {
                    list.Add(new NetMarkIMEIMes()
                    {
                        NetMark = dr.GetString(0),
                        IMEI = dr.GetString(1)
                    });
                }
                return list;
            }
        }

        //根据IMEI找网标
        public string selectNetMarkByIMEIDAL(string IMEI)
        {
            SqlConnection conn2 = new SqlConnection(conStr);
            conn2.Open();
            using (SqlCommand command = conn2.CreateCommand())
            {
                command.CommandText = "SELECT NetMark FROM dbo.NetMarkIMEI WHERE IMEI = '" + IMEI + "'";
                return Convert.ToString(command.ExecuteScalar());
            }
        }

        //找到IMEI当前打印位
        public string SelectPresentImeiDAL(string Zhidan)
        {
            SqlConnection conn2 = new SqlConnection(conStr);
            conn2.Open();
            using (SqlCommand command = conn2.CreateCommand())
            {
                command.CommandText = "SELECT Top 1 IMEI FROM dbo.NetMarkIMEI WHERE Zhidan = '"+ Zhidan + "' ORDER BY Id desc";
                return Convert.ToString(command.ExecuteScalar());
            }
        }

        //根据网标或IMEI查找对应的IMEI或网标
        public string SelectResultByImeiOrNetmarkDAL(string NetLogoOrImer)
        {
            SqlConnection conn2 = new SqlConnection(conStr);
            conn2.Open();
            String PResult = "";
            using (SqlCommand command = conn2.CreateCommand())
            {
                List<NetMarkIMEIMes> list = new List<NetMarkIMEIMes>();
                command.CommandText = "SELECT NetMark,IMEI FROM dbo.NetMarkIMEI WHERE (IMEI='" + NetLogoOrImer + "' OR NetMark='" + NetLogoOrImer + "')";
                SqlDataReader dr = command.ExecuteReader();
                while (dr.Read())
                {
                    if(dr.GetString(0)== NetLogoOrImer)
                    {
                        PResult = dr.GetString(1);
                    }
                    else
                    {
                        PResult = dr.GetString(0);
                    }
                    return PResult;
                }
                return PResult;
            }
        }

    }
}
