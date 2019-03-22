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
        string conStr = ConfigurationManager.ConnectionStrings["conn2"].ConnectionString;

        public void refreshCon()
        {
            conStr = ConfigurationManager.ConnectionStrings["conn2"].ConnectionString;
        }

        //插入数据到NetMarkIMEI表
        public int InsertNetMarkDAL(string Zhidan,string IMEI,string Netmark,string RFID, string TemPlate)
        {
            using (SqlConnection conn2 = new SqlConnection(conStr))
            {
                conn2.Open();
                string DtNow = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss:fff");
                using (SqlCommand command = conn2.CreateCommand())
                {
                    command.CommandText = "SELECT Id FROM [dbo].[NetMarkIMEI] where NetMark = '"+Netmark+"'";
                    if (Convert.ToString(command.ExecuteScalar()) == "")
                    {
                        command.CommandText = "INSERT INTO [dbo].[NetMarkIMEI]([NetMark], [IMEI], [Zhidan], [NMTime],[RFID],[JS_PrintTime],[JS_TemPlate],[RePrintNum]) VALUES ('" + Netmark + "', '" + IMEI + "','" + Zhidan + "', '" + DtNow + "','" + RFID + "','" + DtNow + "','" + TemPlate + "','0');";
                        return command.ExecuteNonQuery();
                    }
                    else
                    {
                        command.CommandText = "UPDATE dbo.NetMarkIMEI SET Zhidan='"+Zhidan+"', NMTime='"+DtNow+"', RFID = '"+RFID+"',JS_PrintTime = '"+DtNow+"',JS_TemPlate='"+TemPlate+ "',RePrintNum ='0' where NetMark='"+Netmark+"'";
                        return command.ExecuteNonQuery();
                    }
                }
            }
        }

        //判断IMEI是否存在
        public int CheckIMEIDAL(string ImeiNum)
        {
            using (SqlConnection conn2 = new SqlConnection(conStr))
            {
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
        }

        //判断RFID是否存在
        public int CheckRFIDDAL(string RFID)
        {
            using (SqlConnection conn2 = new SqlConnection(conStr))
            {
                conn2.Open();
                using (SqlCommand command = conn2.CreateCommand())
                {
                    command.CommandText = "Select Id from NetMarkIMEI WHERE RFID='" + RFID + "'";
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
        }

        //判断网标是否存在
        public int CheckNetMarkDAL(string NetMankNum)
        {
            using (SqlConnection conn2 = new SqlConnection(conStr))
            {
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
        }

        //删除数据根据网标或IMEI
        public int DeleteAllByNetLogoOrImerDAL(string NetLogoOrImer)
        {
            using (SqlConnection conn2 = new SqlConnection(conStr))
            {
                conn2.Open();
                using (SqlCommand command = conn2.CreateCommand())
                {
                    command.CommandText = "Delete NetMarkIMEI  WHERE NetMark='" + NetLogoOrImer + "' OR IMEI = '" + NetLogoOrImer + "'";
                    return command.ExecuteNonQuery();
                }
            }
        }


        //根据IMEI或网标获取IMEI
        public List<NetMarkIMEIMes> SelectIMEIByNetLogoOrImerDAL(string NetLogoOrImer)
        {
            using (SqlConnection conn2 = new SqlConnection(conStr))
            {
                conn2.Open();
                List<NetMarkIMEIMes> list = new List<NetMarkIMEIMes>();
                using (SqlCommand command = conn2.CreateCommand())
                {
                    command.CommandText = "SELECT NetMark,IMEI,RFID FROM [dbo].[NetMarkIMEI] where NetMark ='" + NetLogoOrImer + "' OR IMEI = '" + NetLogoOrImer + "'";
                    SqlDataReader dr = command.ExecuteReader();
                    while (dr.Read())
                    {
                        list.Add(new NetMarkIMEIMes()
                        {
                            NetMark = dr.GetString(0),
                            //NetMark = dr.GetString(dr.GetOrdinal("NetMark")),
                            IMEI = dr.GetString(1),
                            RFID = dr.GetString(2)
                        });
                    }
                    return list;
                }
            }
        }

        //根据IMEI或RFID获取IMEI
        public string SelectIMEIDAL(string RFIDOrImer)
        {
            using (SqlConnection conn2 = new SqlConnection(conStr))
            {
                conn2.Open();
                using (SqlCommand command = conn2.CreateCommand())
                {
                    command.CommandText = "SELECT IMEI FROM [dbo].[NetMarkIMEI] where RFID ='" + RFIDOrImer + "' OR IMEI = '" + RFIDOrImer + "'";
                    return Convert.ToString(command.ExecuteScalar());
                }
            }
        }

        //根据制单号查找所有网标和IMEI
        public List<NetMarkIMEIMes> selectNetMarkIMEIByzhidanDAL(string ZhidanNum)
        {
            using (SqlConnection conn2 = new SqlConnection(conStr))
            {
                conn2.Open();
                List<NetMarkIMEIMes> list = new List<NetMarkIMEIMes>();
                using (SqlCommand command = conn2.CreateCommand())
                {
                    command.CommandText = "SELECT NetMark,IMEI FROM dbo.NetMarkIMEI WHERE Zhidan = '" + ZhidanNum + "'";
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
        }

        //根据IMEI范围查找所有网标和IMEI
        public List<NetMarkIMEIMes> selectNetMarkIMEIByImeiRangeDAL(string ImeiStart, string ImeiEnd)
        {
            using (SqlConnection conn2 = new SqlConnection(conStr))
            {
                conn2.Open();
                List<NetMarkIMEIMes> list = new List<NetMarkIMEIMes>();
                using (SqlCommand command = conn2.CreateCommand())
                {
                    command.CommandText = "SELECT NetMark,IMEI FROM dbo.NetMarkIMEI WHERE (IMEI>='" + ImeiStart + "' AND IMEI<='" + ImeiEnd + "')";
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
        }

        //根据IMEI找网标
        public string selectNetMarkByIMEIDAL(string IMEI)
        {
            using (SqlConnection conn2 = new SqlConnection(conStr))
            {
                conn2.Open();
                using (SqlCommand command = conn2.CreateCommand())
                {
                    command.CommandText = "SELECT NetMark FROM dbo.NetMarkIMEI WHERE IMEI = '" + IMEI + "'";
                    return Convert.ToString(command.ExecuteScalar());
                }
            }
        }

        //找到IMEI当前打印位
        public string SelectPresentImeiDAL(string Zhidan)
        {
            using (SqlConnection conn2 = new SqlConnection(conStr))
            {
                conn2.Open();
                using (SqlCommand command = conn2.CreateCommand())
                {
                    command.CommandText = "SELECT Top 1 IMEI FROM dbo.NetMarkIMEI WHERE Zhidan = '" + Zhidan + "' ORDER BY Id desc";
                    return Convert.ToString(command.ExecuteScalar());
                }
            }
        }

        //根据网标或IMEI查找对应的IMEI或网标
        public List<NetMarkIMEIMes> SelectResultByImeiOrNetmarkDAL(string NetLogoOrImer)
        {
            using (SqlConnection conn2 = new SqlConnection(conStr))
            {
                conn2.Open();
                using (SqlCommand command = conn2.CreateCommand())
                {
                    List<NetMarkIMEIMes> list = new List<NetMarkIMEIMes>();
                    command.CommandText = "SELECT NetMark,IMEI,Zhidan,RFID,JS_PrintTime,JS_TemPlate,RePrintNum,RePrintFirstTime,RePrintEndTime FROM dbo.NetMarkIMEI WHERE (IMEI='" + NetLogoOrImer + "' OR NetMark='" + NetLogoOrImer + "')";
                    SqlDataReader dr = command.ExecuteReader();
                    while (dr.Read())
                    {
                        list.Add(new NetMarkIMEIMes()
                        {
                            NetMark = dr.IsDBNull(0) ? "" : dr.GetString(0),
                            IMEI = dr.IsDBNull(1) ? "" : dr.GetString(1),
                            zhidan = dr.IsDBNull(2) ? "" : dr.GetString(2),
                            RFID = dr.IsDBNull(3) ? "" : dr.GetString(3),
                            JS_PrintTime = dr.IsDBNull(4) ? "" : dr.GetDateTime(4).ToString(),
                            JS_Template = dr.IsDBNull(5) ? "" : dr.GetString(5),
                            RePrintNum =  dr.GetInt32(6),
                            RePrintFirstTime = dr.IsDBNull(7) ? "" : dr.GetDateTime(7).ToString(),
                            RePrintEndTime = dr.IsDBNull(8) ? "" : dr.GetDateTime(8).ToString()
                        });
                    }
                    return list;
                }
            }
        }

        //更新重打时间
        public int UpdateRePrintDAL(string IMEInumber)
        {
            string RePrintTime = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss:fff");
            using (SqlConnection conn2 = new SqlConnection(conStr))
            {
                conn2.Open();
                using (SqlCommand command = conn2.CreateCommand())
                {
                    command.CommandText = "SELECT RePrintNum FROM dbo.NetMarkIMEI WHERE IMEI = '" + IMEInumber + "'";
                    if (Convert.ToInt32(command.ExecuteScalar()) == 0)
                    {
                        command.CommandText = "UPDATE dbo.NetMarkIMEI SET RePrintFirstTime ='" + RePrintTime + "',RePrintNum = RePrintNum+1 WHERE IMEI='" + IMEInumber + "'";
                    }
                    else
                    {
                        command.CommandText = "UPDATE dbo.NetMarkIMEI SET RePrintEndTime ='" + RePrintTime + "',RePrintNum = RePrintNum+1 WHERE IMEI='" + IMEInumber + "'";
                    }
                    return command.ExecuteNonQuery();
                }
            }
        }


        //根据网标获取IMEI
        public string SelectIMEIByNetMarkDAL(string netmark)
        {
            using (SqlConnection conn2 = new SqlConnection(conStr))
            {
                conn2.Open();
                using (SqlCommand command = conn2.CreateCommand())
                {
                    command.CommandText = "SELECT IMEI FROM [dbo].[NetMarkIMEI] where NetMark ='" + netmark + "'";
                    return Convert.ToString(command.ExecuteScalar());
                }
            }
        }

    }
}
