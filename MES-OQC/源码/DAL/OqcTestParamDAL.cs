using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data.SqlClient;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OqcTestParam.DAL
{
    class OqcTestParamDAL
    {
        string conStr = ConfigurationManager.ConnectionStrings["conn1"].ConnectionString;

        public void refreshCon()
        {
            conStr = ConfigurationManager.ConnectionStrings["conn1"].ConnectionString;
        }

        //插入测试数据
        public int InsertTestResultDAL(string Zhidan,string IMEI,string SendCommand,string ReceiveValue,int TestResule,string TestUser)
        {
            using (SqlConnection conn1 = new SqlConnection(conStr))
            {
                conn1.Open();
                string DtNow = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss:fff");
                using (SqlCommand command = conn1.CreateCommand())
                {
                    command.CommandText = "SELECT [TestResule] FROM [dbo].[Gps_OqcTestParam] where IMEI ='" + IMEI + "'";
                    if (Convert.ToString(command.ExecuteScalar()) == "")
                    {
                        command.CommandText = "INSERT INTO [dbo].[Gps_OqcTestParam]([Zhidan],[IMEI],[SendCommand],[ReceiveValue],[TestResule],[TestUser],[TestTime]) VALUES ('" + Zhidan + "', '" + IMEI + "','" + SendCommand + "','" + ReceiveValue + "','" + TestResule + "','" + TestUser + "','" + DtNow + "');";
                        return command.ExecuteNonQuery();
                    }
                    else
                    {
                        command.CommandText = "UPDATE [dbo].[Gps_OqcTestParam] SET [SendCommand]='"+ SendCommand + "',[ReceiveValue]='"+ ReceiveValue + "', [ReTestNum] = [ReTestNum]+1,[ReTestTime]='" + DtNow+ "',TestResule='" + TestResule + "', TestUser='"+ TestUser + "' where IMEI='"+IMEI+"'";
                        return command.ExecuteNonQuery();
                    }
                }
            }
        }


        //检测IMEI是否存在
        public int CheckImeiDAL(string IMEI)
        {
            using (SqlConnection conn1 = new SqlConnection(conStr))
            {
                conn1.Open();
                using (SqlCommand command = conn1.CreateCommand())
                {
                    command.CommandText = "SELECT [IMEI] FROM [dbo].[Gps_OqcTestParam] where IMEI ='"+IMEI+ "' AND TestResule='1'";
                    if (Convert.ToString(command.ExecuteScalar()) == "")
                    {
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }

        public List<OqcTestParam1> SelectOqcTestDAL(string Sqlstr)
        {
            List<OqcTestParam1> ot = new List<OqcTestParam1>();
            using (SqlConnection conn1 = new SqlConnection(conStr))
            {
                conn1.Open();
                using (SqlCommand command = conn1.CreateCommand())
                {
                    command.CommandText = Sqlstr;
                    SqlDataReader dr = command.ExecuteReader();
                    while (dr.Read())
                    {
                        ot.Add(new OqcTestParam1()
                        {
                            Zhidan = dr.IsDBNull(1) ? "" : dr.GetString(1),
                            IMEI = dr.IsDBNull(2) ? "" : dr.GetString(2),
                            SendCommand = dr.IsDBNull(3) ? "" : dr.GetString(3),
                            ReceiveValue = dr.IsDBNull(4) ? "" : dr.GetString(4),
                            ReTestNum = dr.GetInt32(5),
                            ReTestTime = dr.IsDBNull(6) ? "" : dr.GetDateTime(6).ToString(),
                            TestResule = dr.GetInt32(7),
                            TestUser = dr.IsDBNull(8) ? "" : dr.GetString(8),
                            TestTime = dr.IsDBNull(9) ? "" : dr.GetDateTime(9).ToString()
                        });
                    }
                    return ot;
                }
            }
        }

    }
}
