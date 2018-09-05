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
        public int CheckOneStationDAL(string IMEInumber,string Station)
        {
            SqlConnection conn1 = new SqlConnection(conStr);
            conn1.Open();
            SqlCommand command = conn1.CreateCommand();
            command.CommandText = "SELECT * FROM dbo.Gps_TestResult  WHERE IMEI='" + IMEInumber + "' AND " + Station + "='1'";
            SqlDataReader dr = command.ExecuteReader();
            while (dr.Read())
            {
                return 1;
            }
            return 0;
        }

        //查两个工位
        public int CheckTwoStationDAL(string IMEInumber, string Station1,string Station2)
        {
            SqlConnection conn1 = new SqlConnection(conStr);
            conn1.Open();
            SqlCommand command = conn1.CreateCommand();
            command.CommandText = "SELECT * FROM dbo.Gps_TestResult  WHERE IMEI='" + IMEInumber + "' AND " + Station1 + "=1 AND "+Station2+" = 1 ";
            SqlDataReader dr = command.ExecuteReader();
            while (dr.Read())
            {
                return 1;
            }
            return 0;
        }

        //查三个工位
        public int CheckThreeStationDAL(string IMEInumber, string Station1, string Station2, string Station3)
        {
            SqlConnection conn1 = new SqlConnection(conStr);
            conn1.Open();
            SqlCommand command = conn1.CreateCommand();
            command.CommandText = "SELECT * FROM dbo.Gps_TestResult  WHERE IMEI='" + IMEInumber + "' AND " + Station1 + "=1 AND " + Station2 + " = 1 AND "+Station3+" = 1 ";
            SqlDataReader dr = command.ExecuteReader();
            while (dr.Read())
            {
                return 1;
            }
            return 0;
        }

        //查四个工位
        public int CheckFourStationDAL(string IMEInumber, string Station1, string Station2, string Station3,string Station4)
        {
            SqlConnection conn1 = new SqlConnection(conStr);
            conn1.Open();
            SqlCommand command = conn1.CreateCommand();
            command.CommandText = "SELECT * FROM dbo.Gps_TestResult  WHERE IMEI='" + IMEInumber + "' AND " + Station1 + "=1 AND " + Station2 + " = 1 AND " + Station3 + " = 1 AND "+Station4+" = 1 ";
            SqlDataReader dr = command.ExecuteReader();
            while (dr.Read())
            {
                return 1;
            }
            return 0;
        }

    }
}
