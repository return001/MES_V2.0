using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data.SqlClient;
using System.Configuration;

namespace HisNetMarkIMEI.DAL
{
    class HisNetMarkIMEIDAL
    {
        private static readonly string conStr = ConfigurationManager.ConnectionStrings["conn2"].ConnectionString;

        //插入数据到NetMarkIMEI表
        public int InsertHisNetMarkDAL(string Zhidan, string IMEI, string Netmark)
        {
            SqlConnection conn2 = new SqlConnection(conStr);
            conn2.Open();
            using (SqlCommand command = conn2.CreateCommand())
            {
                string DtNow = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss:fff");
                command.CommandText = "INSERT INTO [dbo].[HisNetMarkIMEI]([NetMark], [IMEI], [Zhidan], [DLTime]) VALUES ('" + Netmark + "', '" + IMEI + "','" + Zhidan + "', '" + DtNow + "');";
                return command.ExecuteNonQuery();
            }
        }
    }
}
