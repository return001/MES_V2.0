using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Configuration;
using System.Data.SqlClient;
using Print_Message;

namespace DataRelative.Param.DAL
{

    class DataRelativeSheetDAL
    {

        private static readonly string conStr = ConfigurationManager.ConnectionStrings["conn1"].ConnectionString;

        //根据IMEI获取关联数据
        public List<DataRelativeSheet> SelectByImeiDAL(string Imei1)
        {
            List<DataRelativeSheet> pm = new List<DataRelativeSheet>();
            SqlConnection conn1 = new SqlConnection(conStr);
            conn1.Open();
            SqlCommand command = conn1.CreateCommand();
            command.CommandText = "SELECT * FROM dbo.DataRelativeSheet WHERE IMEI1 ='"+Imei1+"'";
            SqlDataReader dr = command.ExecuteReader();
            while (dr.Read())
            {
                pm.Add(new DataRelativeSheet()
                {
                    IMEI2 = dr.IsDBNull(2) ? "" : dr.GetString(2),
                    IMEI3 = dr.IsDBNull(3) ? "" : dr.GetString(3),
                    IMEI4 = dr.IsDBNull(4) ? "" : dr.GetString(4),
                    IMEI6 = dr.IsDBNull(6) ? "" : dr.GetString(6),
                    IMEI7 = dr.IsDBNull(7) ? "" : dr.GetString(7),
                    IMEI8 = dr.IsDBNull(8) ? "" : dr.GetString(8),
                    IMEI9 = dr.IsDBNull(9) ? "" : dr.GetString(9)
                });
            }
            return pm;
        }

        //根据SIM号获取ICCID
        public string SelectIccidBySimDAL(string SIM)
        {
            SqlConnection conn1 = new SqlConnection(conStr);
            conn1.Open();
            string Iccid;
            SqlCommand command = conn1.CreateCommand();
            command.CommandText = "SELECT * FROM dbo.DataRelativeSheet WHERE IMEI3 ='" + SIM + "'";
            SqlDataReader dr = command.ExecuteReader();
            if (dr.Read())
            {
                Iccid = dr.IsDBNull(4) ? "" : dr.GetString(4);
            }
            else {
                Iccid = "";
            }
            return Iccid;
        }

        public string SelectSNByImeiDAL(string IMEI)
        {
            SqlConnection conn1 = new SqlConnection(conStr);
            conn1.Open();
            string Sn;
            SqlCommand command = conn1.CreateCommand();
            command.CommandText = "SELECT * FROM dbo.DataRelativeSheet WHERE IMEI1 = '" + IMEI + "'";
            SqlDataReader dr = command.ExecuteReader();
            if (dr.Read())
            {
                Sn = dr.IsDBNull(2) ? "" : dr.GetString(2);
            }
            else
            {
                Sn = "";
            }
            return Sn;
        }

        //检查IMEI号是否存在，存在返回1，否则返回0
        public int CheckIMEIDAL(string IMEInumber)
        {
            SqlConnection conn1 = new SqlConnection(conStr);
            conn1.Open();
            SqlCommand command = conn1.CreateCommand();
            command.CommandText = "SELECT * FROM dbo.DataRelativeSheet WHERE IMEI1='" + IMEInumber + "'";
            SqlDataReader dr = command.ExecuteReader();
            while (dr.Read())
            {
                return 1;
            }
            return 0;
        }

        //检查SIM号是否存在，存在返回1，否则返回0
        public int CheckSIMDAL(string SIMnumber)
        {
            SqlConnection conn1 = new SqlConnection(conStr);
            conn1.Open();
            SqlCommand command = conn1.CreateCommand();
            command.CommandText = "SELECT * FROM dbo.DataRelativeSheet WHERE IMEI3='" + SIMnumber + "'";
            SqlDataReader dr = command.ExecuteReader();
            while (dr.Read())
            {
                return 1;
            }
            return 0;
        }

        //检查IMEI检查SIM号是否存在，存在返回1，否则返回0
        public string CheckSIMByIMEIDAL(string IMEI)
        {
            SqlConnection conn1 = new SqlConnection(conStr);
            conn1.Open();
            string Sim;
            SqlCommand command = conn1.CreateCommand();
            command.CommandText = "SELECT * FROM dbo.DataRelativeSheet WHERE IMEI1='" + IMEI + "'";
            SqlDataReader dr = command.ExecuteReader();
            if (dr.Read())
            {
                Sim = dr.IsDBNull(3) ? "" : dr.GetString(3);
            }
            else
            {
                Sim = "";
            }
            return Sim;
        }

        //更新IMEI通过SIM
        public int UpdateIMEIBySIMDAL(string IMEI, string SIM)
        {
            SqlConnection conn1 = new SqlConnection(conStr);
            conn1.Open();
            SqlCommand command = conn1.CreateCommand();
            command.CommandText = "UPDATE dbo.DataRelativeSheet SET  IMEI1='" + IMEI + "' WHERE IMEI3='" + SIM + "'";
            return command.ExecuteNonQuery();
        }

        //更新IMEI通过SIM
        public int UpdateSIMByIMEIDAL(string IMEI, string SIM)
        {
            SqlConnection conn1 = new SqlConnection(conStr);
            conn1.Open();
            SqlCommand command = conn1.CreateCommand();
            command.CommandText = "UPDATE dbo.DataRelativeSheet SET  IMEI3='" + SIM + "' WHERE IMEI1='" + IMEI + "'";
            return command.ExecuteNonQuery();
        }

        //更新关联表字段
        public int UpdateAssociatedDAL(string IMEI, string VIP, string BAT,  string MAC, string Equipment)
        {
            SqlConnection conn1 = new SqlConnection(conStr);
            conn1.Open();
            SqlCommand command = conn1.CreateCommand();
            command.CommandText = "UPDATE dbo.DataRelativeSheet SET  IMEI6 ='" + MAC + "', IMEI7 ='" + Equipment + "',IMEI8='" + VIP + "',IMEI9='" + BAT + "' WHERE IMEI1='" + IMEI + "'";
            return command.ExecuteNonQuery();
        }

        //更新VIP
        public int UpdateVIPDAL(string IMEI, string VIP)
        {
            SqlConnection conn1 = new SqlConnection(conStr);
            conn1.Open();
            SqlCommand command = conn1.CreateCommand();
            command.CommandText = "UPDATE dbo.DataRelativeSheet SET  IMEI8='" + VIP + "' WHERE IMEI1='" + IMEI + "'";
            return command.ExecuteNonQuery();
        }

        //更新VIP或者BAT
        public int UpdateVipAndBatDAL(string IMEI, string VIP, string BAT)
        {
            SqlConnection conn1 = new SqlConnection(conStr);
            conn1.Open();
            SqlCommand command = conn1.CreateCommand();
            if (VIP == "")
            {
                command.CommandText = "UPDATE dbo.DataRelativeSheet SET  IMEI9='" + BAT + "' WHERE IMEI1='" + IMEI + "'";
            }
            else {
                command.CommandText = "UPDATE dbo.DataRelativeSheet SET  IMEI8='" + VIP + "',IMEI9='" + BAT + "' WHERE IMEI1='" + IMEI + "'";
            }
            return command.ExecuteNonQuery();
        }

        //更新VIP或者BAT或者MAC
        public int UpdateVipAndBatAndMacDAL(string IMEI, string VIP, string BAT,string MAC)
        {
            SqlConnection conn1 = new SqlConnection(conStr);
            conn1.Open();
            SqlCommand command = conn1.CreateCommand();
            if (VIP == "")
            {
                if (BAT == "")
                {
                    command.CommandText = "UPDATE dbo.DataRelativeSheet SET  IMEI6 ='" + MAC + "' WHERE IMEI1='" + IMEI + "'";
                }
                else
                {
                    command.CommandText = "UPDATE dbo.DataRelativeSheet SET  IMEI6 ='" + MAC + "', IMEI9='" + BAT + "' WHERE IMEI1='" + IMEI + "'";
                }
            }
            else
            {
                if (BAT == "")
                {
                    command.CommandText = "UPDATE dbo.DataRelativeSheet SET  IMEI6 ='" + MAC + "', IMEI8='" + VIP + "' WHERE IMEI1='" + IMEI + "'";
                }
                else {
                    command.CommandText = "UPDATE dbo.DataRelativeSheet SET  IMEI6 ='" + MAC + "', IMEI8='" + VIP + "',IMEI9='" + BAT + "' WHERE IMEI1='" + IMEI + "'";
                }
            }
            return command.ExecuteNonQuery();
        }

        //更新VIP或者BAT或者MAC、Equipment
        public int UpdateVipAndBatAndMacAndEquDAL(string IMEI, string VIP, string BAT, string MAC, string Equipment)
        {
            SqlConnection conn1 = new SqlConnection(conStr);
            conn1.Open();
            SqlCommand command = conn1.CreateCommand();
            if (VIP == "")
            {
                if (BAT == "" && MAC == "")
                {
                    command.CommandText = "UPDATE dbo.DataRelativeSheet SET  IMEI7 ='" + Equipment + "' WHERE IMEI1='" + IMEI + "'";
                }
                else if (BAT != "" && MAC == "")
                {
                    command.CommandText = "UPDATE dbo.DataRelativeSheet SET IMEI7 ='" + Equipment + "',IMEI9='" + BAT + "' WHERE IMEI1='" + IMEI + "'";
                }
                else if (BAT == "" && MAC != "")
                {
                    command.CommandText = "UPDATE dbo.DataRelativeSheet SET  IMEI6 ='" + MAC + "', IMEI7 ='" + Equipment + "' WHERE IMEI1='" + IMEI + "'";
                }
                else
                {
                    command.CommandText = "UPDATE dbo.DataRelativeSheet SET  IMEI6 ='" + MAC + "', IMEI7 ='" + Equipment + "',IMEI9='" + BAT + "' WHERE IMEI1='" + IMEI + "'";
                }
            }
            else {
                if (BAT == "" && MAC == "")
                {
                    command.CommandText = "UPDATE dbo.DataRelativeSheet SET  IMEI7 ='" + Equipment + "',IMEI8='" + VIP + "' WHERE IMEI1='" + IMEI + "'";
                }
                else if (BAT != "" && MAC == "")
                {
                    command.CommandText = "UPDATE dbo.DataRelativeSheet SET  IMEI7 ='" + Equipment + "',IMEI8='" + VIP + "',IMEI9='" + BAT + "' WHERE IMEI1='" + IMEI + "'";
                }
                else if (BAT == "" && MAC != "")
                {
                    command.CommandText = "UPDATE dbo.DataRelativeSheet SET  IMEI6 ='" + MAC + "', IMEI7 ='" + Equipment + "',IMEI8='" + VIP + "' WHERE IMEI1='" + IMEI + "'";
                }
                else
                {
                    command.CommandText = "UPDATE dbo.DataRelativeSheet SET  IMEI6 ='" + MAC + "', IMEI7 ='" + Equipment + "',IMEI8='" + VIP + "',IMEI9='" + BAT + "' WHERE IMEI1='" + IMEI + "'";
                }
            }
            return command.ExecuteNonQuery();
        }

        //插入数据到关联表
        public int InsertRelativeSheetDAL(List<DataRelativeSheet> list)
        {
            SqlConnection conn1 = new SqlConnection(conStr);
            conn1.Open();
            SqlCommand command = conn1.CreateCommand();
            int i = list.Count;
            if (i > 0)
            {
                command.CommandText = "INSERT INTO dbo.DataRelativeSheet([SN], [IMEI1], [IMEI2], [IMEI3], [IMEI4], [IMEI5], [IMEI6], [IMEI7],[IMEI8], [IMEI9], [IMEI10], [IMEI11], [IMEI12], [ZhiDan], [TestTime], [_MASK_FROM_V2]) VALUES(NULL,'" + list[i - 1].IMEI1 + "','" + list[i - 1].IMEI2 + "','" + list[i - 1].IMEI3 + "','" + list[i - 1].IMEI4 + "','" + list[i - 1].IMEI5 + "','" + list[i - 1].IMEI6 + "','" + list[i - 1].IMEI7 + "','" + list[i - 1].IMEI8 + "','" + list[i - 1].IMEI9 + "','" + list[i - 1].IMEI10 + "','" + list[i - 1].IMEI11 + "','" + list[i - 1].IMEI12 + "','" + list[i - 1].ZhiDan+ "','"+ list[i - 1].TestTime + "',NULL)";
            }
            int httpstr = command.ExecuteNonQuery();
            return httpstr;
        }

        //根据SN或者IMEI2带出IMEI
        public string SelectIMEIBySnOrIMEI2DAL(string IMEI2Value)
        {
            SqlConnection conn1 = new SqlConnection(conStr);
            conn1.Open();
            string IMEI;
            SqlCommand command = conn1.CreateCommand();
            command.CommandText = "SELECT * FROM dbo.DataRelativeSheet WHERE (SN = '" + IMEI2Value + "' OR IMEI2 = '"+IMEI2Value+"')";
            SqlDataReader dr = command.ExecuteReader();
            if (dr.Read())
            {
                IMEI = dr.IsDBNull(1) ? "" : dr.GetString(1);
            }
            else
            {
                IMEI = "";
            }
            return IMEI;
        }

    }
}
