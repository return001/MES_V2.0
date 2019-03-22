using NetLogo_Message;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data.SqlClient;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NetMarkConfig.DAL
{
    class NetMarkConfigDAL
    {
        string conStr = ConfigurationManager.ConnectionStrings["conn2"].ConnectionString;

        public void refreshCon()
        {
            conStr = ConfigurationManager.ConnectionStrings["conn2"].ConnectionString;
        }

        //插入数据到NetMarkConfigDAL表
        public int InsertPrintRecordParamDAL(string Zhidan, string Temlpate, string IMEIPresent, string NetMarkPre, string NetMarkSuf, string NetMarkDig, string RFIDStart, string RFIDEnd, string RFIDPresent, int NoCheckCode, int InputAllMark, int RFIDAutoAddMark, int TemplateNum)
        {
            using (SqlConnection conn2 = new SqlConnection(conStr))
            {
                conn2.Open();
                int httpstr;
                using (SqlCommand command = conn2.CreateCommand())
                {
                    command.CommandText = "SELECT ID FROM dbo.NetMarkConfig WHERE Zhidan ='" + Zhidan + "'";
                    if (Convert.ToInt32(command.ExecuteScalar()) > 0)
                    {
                        command.CommandText = "UPDATE dbo.NetMarkConfig SET Temlpate ='" + Temlpate + "',IMEIPresent = '" + IMEIPresent + "',NetMarkPre ='" + NetMarkPre + "',NetMarkSuf = '" + NetMarkSuf + "',NetMarkDig ='" + NetMarkDig + "',RFIDStart = '" + RFIDStart + "',RFIDEnd ='" + RFIDEnd + "',RFIDPresent = '" + RFIDPresent + "',NoCheckCode ='" + NoCheckCode + "',InputAllMark = '" + InputAllMark + "',RFIDAutoAddMark ='" + RFIDAutoAddMark + "',TemplateNum = '"+ TemplateNum + "'WHERE Zhidan='" + Zhidan + "'";
                        httpstr = command.ExecuteNonQuery();
                    }
                    else
                    {
                        command.CommandText = "INSERT INTO dbo.NetMarkConfig(Zhidan,Temlpate,IMEIPresent,NetMarkPre,NetMarkSuf,NetMarkDig,RFIDStart,RFIDEnd,RFIDPresent,NoCheckCode,InputAllMark,RFIDAutoAddMark,TemplateNum) VALUES('" + Zhidan + "','" + Temlpate + "','" + IMEIPresent + "','" + NetMarkPre + "','" + NetMarkSuf + "','" + NetMarkDig + "','" + RFIDStart + "','" + RFIDEnd + "','" + RFIDPresent + "','" + NoCheckCode + "','" + InputAllMark + "','" + RFIDAutoAddMark + "','"+ TemplateNum + "')";
                        httpstr = command.ExecuteNonQuery();
                    }
                    return httpstr;
                }
            }
        }

        //根据制单号返回该制单相关信息
        public List<G_NetMarkConfig> selectNetMarkParamByzhidanDAL(string ZhidanNum)
        {
            SqlConnection conn2 = new SqlConnection(conStr);
            conn2.Open();
            List<G_NetMarkConfig> list = new List<G_NetMarkConfig>();
            using (SqlCommand command = conn2.CreateCommand())
            {
                command.CommandText = "SELECT * FROM dbo.NetMarkConfig WHERE Zhidan='" + ZhidanNum + "'";
                SqlDataReader dr = command.ExecuteReader();
                while (dr.Read())
                {
                    list.Add(new G_NetMarkConfig()
                    {
                        Temlpate = dr.IsDBNull(2)?"":dr.GetString(2),
                        IMEIPresent = dr.IsDBNull(3) ? "" : dr.GetString(3),
                        NetMarkPre = dr.IsDBNull(4) ? "" : dr.GetString(4),
                        NetMarkSuf = dr.IsDBNull(5) ? "" : dr.GetString(5),
                        NetMarkDig = dr.IsDBNull(6) ? "" : dr.GetString(6),
                        RFIDStart = dr.IsDBNull(7) ? "" : dr.GetString(7),
                        RFIDEnd = dr.IsDBNull(8) ? "" : dr.GetString(8),
                        RFIDPresent = dr.IsDBNull(9) ? "" : dr.GetString(9),
                        NoCheckCode = dr.GetInt32(10),
                        InputAllMark = dr.GetInt32(11),
                        RFIDAutoAddMark = dr.GetInt32(12),
                        TemplateNum = dr.GetInt32(13)
                    });
                }
                return list;
            }
        }

        public int UpdatePrensentIMEIAndRFIDDAL(string Zhidan, string IMEIPresent, string RFIDPresent)
        {
            using (SqlConnection conn2 = new SqlConnection(conStr))
            {
                conn2.Open();
                int httpstr;
                using (SqlCommand command = conn2.CreateCommand())
                {
                    command.CommandText = "UPDATE dbo.NetMarkConfig SET IMEIPresent = '" + IMEIPresent + "',RFIDPresent = '" + RFIDPresent + "'WHERE Zhidan='" + Zhidan + "'";
                    httpstr = command.ExecuteNonQuery();
                }
                return httpstr;
            }
        }

    }
}
