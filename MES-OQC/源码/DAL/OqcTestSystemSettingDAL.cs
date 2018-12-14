using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data.SqlClient;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TestSystemSetting;

namespace OqcTestSystemSetting.DAL
{
    class OqcTestSystemSettingDAL
    {
        string conStr = ConfigurationManager.ConnectionStrings["conn1"].ConnectionString;

        

        public void refreshCon()
        {
            conStr = ConfigurationManager.ConnectionStrings["conn1"].ConnectionString;
        }

        //返回制单号
        public List<OqcTestSystem> SelectZhidanNumDAL()
        {
            using (SqlConnection conn1 = new SqlConnection(conStr))
            {
                conn1.Open();
                List<OqcTestSystem> list = new List<OqcTestSystem>();
                using (SqlCommand command = conn1.CreateCommand())
                {
                    command.CommandText = "SELECT SoftWare FROM dbo.Gps_OqcTestSystemSetting";
                    SqlDataReader dr = command.ExecuteReader();
                    while (dr.Read())
                    {
                        list.Add(new OqcTestSystem()
                        {
                            SoftWare = dr.GetString(0)
                        });
                    }
                    return list;
                }
            }
        }

        public List<string> SelectCommandDAL(string zhidan)
        {
            using (SqlConnection conn1 = new SqlConnection(conStr))
            {
                conn1.Open();
                List<string> stringList = new List<string>(80);
                using (SqlCommand command = conn1.CreateCommand())
                {
                    command.CommandText = "SELECT * FROM dbo.Gps_OqcTestSystemSetting where SoftWare = '"+zhidan+"'";
                    SqlDataReader dr = command.ExecuteReader();
                    while (dr.Read())
                    {
                        for (int i = 1; i <= 68; i++)
                        {
                            if (!dr.IsDBNull(i))
                            {
                                stringList.Add(dr.GetString(i));
                            }
                        }
                    }
                    return stringList;
                }
            }
        }

        //更新指令
        public int UpdateCommandDAL(string UpdateSql)
        {
            using (SqlConnection conn1 = new SqlConnection(conStr))
            {
                conn1.Open();
                using (SqlCommand command = conn1.CreateCommand())
                {
                    command.CommandText = UpdateSql;
                    return command.ExecuteNonQuery();
                }
            }
        }

    }
}
