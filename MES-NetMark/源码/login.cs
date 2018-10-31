using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Security.Cryptography;
using PUsers.BLL;

namespace NetLogoTool
{
    public partial class login : Form
    {
        Form1 f1;
        public login(Form1 f)
        {
            InitializeComponent();
            this.f1 = f;
        }

        PUsersBLL PUB = new PUsersBLL();

        //确定按钮
        private void SureLog_Click(object sender, EventArgs e)
        {
            if(this.NetLogoUese.Text == "")
            {
                this.NetLogoUese.Clear();
                this.NetLogoPassword.Clear();
                this.NetLogoUese.Focus();
                this.LRemind.AppendText("用户名不能为空\r\n");
                return;
            }
            if (this.NetLogoPassword.Text == "")
            {
                this.NetLogoUese.Clear();
                this.NetLogoPassword.Clear();
                this.NetLogoUese.Focus();
                this.LRemind.AppendText("密码不能为空\r\n");
                return;
            }
            string Userdes;
            Userdes = PUB.CheckUeseBLL(this.NetLogoUese.Text, GetMd5(this.NetLogoPassword.Text));
            if (Userdes == "")
            {
                this.NetLogoUese.Clear();
                this.NetLogoPassword.Clear();
                this.NetLogoUese.Focus();
                this.LRemind.AppendText("用户名或密码错误\r\n");
            }
            else
            {
                string usertype = PUB.CheckUeseTypeBLL(this.NetLogoUese.Text);
                if (usertype == "admin")
                {
                    this.Close();
                    f1.UserType.Text = "管理员:";
                    f1.NetUser.Text = Userdes;
                    f1.LoginSuccess();
                }
                else if(usertype == "SuperAdmin")
                {
                    this.Close();
                    f1.UserType.Text = "超级管理员:";
                    f1.NetUser.Text = Userdes;
                    f1.LoginSuccess();
                    f1.SuperLogin();
                }
                else
                {
                    this.Close();
                    f1.UserType.Text = "用户:";
                    f1.NetUser.Text = Userdes;
                    f1.LoginSuccess();
                }
            }
        }

        //取消按钮
        private void Cancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }


        /// 获取MD5值
        public string GetMd5(string str)
        {
            StringBuilder sb = new StringBuilder();
            //创建一个计算MD5值对象
            using (MD5 md = MD5.Create())
            {
                //把字符串转成字节数组
                byte[] bytes = System.Text.Encoding.Default.GetBytes(str);
                //调用该对象的方法进行MD5计算
                byte[] md5bytes = md.ComputeHash(bytes);

                for (int i = 0; i < md5bytes.Length; i++)
                {
                    sb.Append(md5bytes[i].ToString("x2"));
                }
            }
            return sb.ToString();
        }

        //在用户名框按下Enter键
        private void NetLogoUese_KeyPress(object sender, KeyPressEventArgs e)
        {
            if(e.KeyChar == 13)
            {
                if (this.NetLogoUese.Text != "")
                {
                    this.NetLogoPassword.Focus();
                }
            }
        }

        //输入密码后按下Enter键
        private void NetLogoPassword_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                SureLog_Click(sender, e);
            }
        }
    }
}
