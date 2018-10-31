using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Configuration;
using System.Data.SqlClient;

namespace TestResult.Param.DAL
{
    class TestResultDAL
    {
        private static readonly string conStr = ConfigurationManager.ConnectionStrings["conn1"].ConnectionString;

        //查一个工位
        public int CheckOneStationDAL(string MissingSql)
        {
            using (SqlConnection conn1 = new SqlConnection(conStr))
            {
                conn1.Open();
                using (SqlCommand command = conn1.CreateCommand())
                {
                    command.CommandText = MissingSql;
                    if (Convert.ToInt32(command.ExecuteScalar()) > 0)
                    {
                        return 1;
                    }
                    else { return 0; }
                }
            }
        }

    }
}
