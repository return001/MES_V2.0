using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data.SqlClient;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataRelativeSheet.DAL
{
    class DataRelativeSheetDAL
    {
        string conStr = ConfigurationManager.ConnectionStrings["conn1"].ConnectionString;

        public void refreshCon()
        {
            conStr = ConfigurationManager.ConnectionStrings["conn1"].ConnectionString;
        }

        //插入数据到关联表
        public int InsertRelativeSheetDAL(string IMEI,string RFID,string Zhidan, string netmark )
        {
            using (SqlConnection conn1 = new SqlConnection(conStr))
            {
                conn1.Open();
                using (SqlCommand command = conn1.CreateCommand())
                {
                    string DtNow = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss:fff");
                    command.CommandText = "INSERT INTO [dbo].[DataRelativeSheet]([IMEI1],[IMEI13],[Zhidan],[TestTime],[NetMark]) VALUES ('" + IMEI + "', '" + RFID + "','" + Zhidan + "','" + DtNow + "','"+netmark+"');";
                    return command.ExecuteNonQuery();
                }
            }
        }

        //查询关联表数据是否有重复
        public string CheckRelativeSheetDAL(string IMEI, string RFID, string netmark)
        {
            using (SqlConnection conn1 = new SqlConnection(conStr))
            {
                string Result = "";
                conn1.Open();
                using (SqlCommand command = conn1.CreateCommand())
                {
                    command.CommandText = "SELECT IMEI1,IMEI13,NetMark FROM [dbo].[DataRelativeSheet] WHERE IMEI1='"+IMEI+"' OR IMEI13='"+RFID+ "' OR NetMark='"+netmark+"';";
                    SqlDataReader dr = command.ExecuteReader();
                    while (dr.Read())
                    {
                        if ((dr.IsDBNull(0) ? "" : dr.GetString(0)) == IMEI)
                        {
                            Result = "关联表IMEI重号！";
                        }
                        if ((dr.IsDBNull(1) ? "" : dr.GetString(1)) == RFID)
                        {
                            Result = "关联表RFID重号！";
                        }
                        if ((dr.IsDBNull(2) ? "" : dr.GetString(2)) == netmark)
                        {
                            Result = "关联表网标重号！";
                        }
                    }
                    return Result;
                }
            }
        }

    }
}
