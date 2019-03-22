using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PUsers.DAL;

namespace PUsers.BLL
{
    class PUsersBLL
    {
        PUsersDAL PUD = new PUsersDAL();


        public void refeshConBLL()
        {
            PUD.refreshCon();
        }

        public string CheckUeseBLL(string UserName, string Password)
        {
            return PUD.CheckUeseDAL(UserName, Password);
        }

        public string CheckUeseTypeBLL(string UserName)
        {
            return PUD.CheckUeseTypeDAL(UserName);
        }

    }
}
