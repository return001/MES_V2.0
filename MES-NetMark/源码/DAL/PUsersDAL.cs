using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data.SqlClient;
using System.Configuration;

namespace PUsers.DAL
{
    class PUsersDAL
    {
        string conStr = ConfigurationManager.ConnectionStrings["conn1"].ConnectionString;

        public void refreshCon()
        {
            conStr = ConfigurationManager.ConnectionStrings["conn1"].ConnectionString;
        }

        //判断用户名密码是否匹配
        public string CheckUeseDAL(string UserName, string Password)
        {
            using (SqlConnection conn1 = new SqlConnection(conStr))
            {
                conn1.Open();
                using (SqlCommand command = conn1.CreateCommand())
                {
                    command.CommandText = "SELECT UserDes From Gps_User WHERE UserName='" + UserName + "' AND UserPwd = '"+ Password + "'";
                    return Convert.ToString(command.ExecuteScalar());
                    
                }
            }
        }

        //获取用户类型
        public string CheckUeseTypeDAL(string UserName)
        {
            using (SqlConnection conn1 = new SqlConnection(conStr))
            {
                conn1.Open();
                using (SqlCommand command = conn1.CreateCommand())
                {
                    command.CommandText = "SELECT UserType From Gps_User WHERE UserName='" + UserName + "'";
                    return Convert.ToString(command.ExecuteScalar());

                }
            }
        }

    }
}
