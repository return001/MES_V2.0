using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Media;
using System.Windows.Forms;
using NetMarkIMEI.BLL;
using ManuOrderParam.BLL;
using HisNetMarkIMEI.BLL;
using NetLogo_Message;
using System.IO;

namespace NetLogoTool
{
    public partial class Form1 : Form
    {
        //记录网标前缀位数和后缀位数
        int netpre;
        int netsuf;

        //音频文件
        SoundPlayer player = new SoundPlayer(AppDomain.CurrentDomain.BaseDirectory + "失败.wav");
        SoundPlayer player1 = new SoundPlayer(AppDomain.CurrentDomain.BaseDirectory + "校验错误.wav");
        SoundPlayer player2 = new SoundPlayer(AppDomain.CurrentDomain.BaseDirectory + "重号.wav");

        NetMarkIMEIBLL NMIB = new NetMarkIMEIBLL();

        HisNetMarkIMEIBLL HNMIB = new HisNetMarkIMEIBLL();

        ManuOrderParamBLL MOPB = new ManuOrderParamBLL();

        List<Gps_ManuOrderParam> G_MOP = new List<Gps_ManuOrderParam>();

        List<NetMarkIMEIMes> NMIM = new List<NetMarkIMEIMes>();

        public Form1()
        {
            InitializeComponent();
            G_MOP = MOPB.SelectZhidanNumBLL();
            foreach (Gps_ManuOrderParam a in G_MOP)
            {
                this.Zhidan_comboBox.Items.Add(a.ZhiDan);
            }
        }

        private void Zhidan_comboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.IMEI_Start.Clear();
            this.IMEI_End.Clear();
            this.NetLogoPrefix.Clear();
            this.NetLogoSuffix.Clear();
            this.NetLogoDigits.Clear();
            this.ImeiInput.Clear();
            this.NetLogoInput.Clear();
            this.PresentImei.Clear();
            this.OnlyNetMark.Clear();
            this.StaticImei.Clear();
            G_MOP = MOPB.selectManuOrderParamByzhidanBLL(this.Zhidan_comboBox.Text);
            foreach (Gps_ManuOrderParam b in G_MOP)
            {
                this.IMEI_Start.Text = b.IMEIStart;
                this.IMEI_End.Text = b.IMEIEnd;
            }
            string PImei = NMIB.SelectPresentImeiBLL(this.Zhidan_comboBox.Text);
            if (PImei != "")
            {
                if (PImei.Length == this.IMEI_Start.Text.Length)
                {
                    this.PresentImei.Text = (long.Parse(PImei) + 1).ToString();
                }
                else
                {
                    this.PresentImei.Text = (long.Parse(PImei.Substring(0, 14)) + 1).ToString();
                }
            }
        }

        //判断是否为纯数字
        static bool IsNumeric(string s)
        {
            double v;
            if (double.TryParse(s, out v))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        //获取IMEI的校验位
        public string getimei15(string imei)
        {
            if (imei.Length == 14)
            {
                Char[] imeiChar = imei.ToCharArray();
                int resultInt = 0;
                for (int i = 0; i < imeiChar.Length; i++)
                {
                    int a = int.Parse(imeiChar[i].ToString());
                    i++;
                    int temp = int.Parse(imeiChar[i].ToString()) * 2;
                    int b = temp < 10 ? temp : temp - 9;
                    resultInt += a + b;
                }
                resultInt %= 10;
                resultInt = resultInt == 0 ? 0 : 10 - resultInt;
                return resultInt + "";
            }
            else { return ""; }
        }

        //锁定按钮做的操作
        private void Tolock_Click(object sender, EventArgs e)
        {
            //锁定前先判断网标是否已经配置好
            if (this.NetLogoPrefix.Text == "" || this.NetLogoSuffix.Text == "" || this.NetLogoDigits.Text == "")
            {
                player.Play();
                this.Remind.AppendText("网标前/后缀及位数都不能为空\r\n");
                return;
            }
            //判断是否已经选择订单
            if(this.Zhidan_comboBox.Text=="" || this.IMEI_Start.Text=="" || this.IMEI_End.Text == "")
            {
                player.Play();
                this.Remind.AppendText("制单、IMEI起始位终止位都不能为空\r\n");
                return;
            }
            if (this.Tolock.Text == "锁定")
            {
                this.Tolock.Text = "解锁";
                this.Zhidan_comboBox.Enabled = false;
                this.NetLogoPrefix.ReadOnly = true;
                this.NetLogoSuffix.ReadOnly = true;
                this.NetLogoDigits.ReadOnly = true;
                this.IMEI_Start.ReadOnly = true;
                this.IMEI_End.ReadOnly = true;
                this.ImeiInput.ReadOnly = false;
                this.NetLogoInput.ReadOnly = false;
                this.NoCheck.Enabled = false;
                this.InputAll.Enabled = false;
                this.OutputTxt.Enabled = false;
                this.OutputByImei.Enabled = false;
                this.Logout.Enabled = false;
                this.ImeiInput.Focus();
                if (this.NetLogoPrefix.Text != "")
                {
                    netpre = this.NetLogoPrefix.Text.Length;
                }
                if (this.NetLogoSuffix.Text != "")
                {
                    netsuf = this.NetLogoSuffix.Text.Length;
                }
                if (this.InputAll.Checked == false)
                {
                    this.OnlyNetMark.ReadOnly = false;
                    this.OnlyNetMark.Focus();
                }
            }
            else
            {
                this.Tolock.Text = "锁定";
                this.Zhidan_comboBox.Enabled = true;
                this.NetLogoPrefix.ReadOnly = false;
                this.NetLogoSuffix.ReadOnly = false;
                this.NetLogoDigits.ReadOnly = false;
                this.IMEI_Start.ReadOnly = false;
                this.IMEI_End.ReadOnly = false;
                this.ImeiInput.ReadOnly = true;
                this.NetLogoInput.ReadOnly = true;
                this.OnlyNetMark.ReadOnly = true;
                this.NoCheck.Enabled = true;
                this.InputAll.Enabled = true;
                this.OutputTxt.Enabled = true;
                this.OutputByImei.Enabled = true;
                this.Logout.Enabled = true;
                this.ImeiInput.Clear();
                this.NetLogoInput.Clear();
            }
        }

        //根据制单号导出TXT文件按钮
        private void InputTxt_Click(object sender, EventArgs e)
        {
            if (this.Zhidan_comboBox.Text == "")
            {
                player.Play();
                this.Remind.AppendText("请先选择制单号\r\n");
                return;
            }
            NMIM = NMIB.selectNetMarkIMEIByzhidanBLL(this.Zhidan_comboBox.Text);
            if (NMIM.Count == 0)
            {
                player.Play();
                this.Remind.AppendText("该制单号无记录\r\n");
                return;
            }
            string NT = System.DateTime.Now.ToString("yyyy-MM-dd");
            //打开保存对话框
            SaveFileDialog dialog = new SaveFileDialog();
            dialog.Filter = "文本文件|*.txt";
            dialog.FileName = NT;//这里将要保存的文件默认为某个订单号

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                // 文件名
                string fileName = dialog.FileName;//FileName为用户输入的名字

                //创建文件准备写入
                FileStream fs = File.Open(fileName, FileMode.Create, FileAccess.Write);
                StreamWriter wr = new StreamWriter(fs);

                //逐行将我们需要的内容写入到文件中
                foreach (NetMarkIMEIMes a in NMIM)
                {
                    wr.WriteLine(a.IMEI+","+a.NetMark);
                    //wr.WriteLine("我是用来测试的语句2\r");
                }
                this.Remind.AppendText("导出成功\r\n");
                //关闭文件
                wr.Flush();
                wr.Close();
                fs.Close();
            }
        }

        //根据IMEI范围导出TXT文件按钮
        private void OutputByImei_Click(object sender, EventArgs e)
        {
            if (this.IMEI_Start.Text == "" || this.IMEI_End.Text=="")
            {
                player.Play();
                this.Remind.AppendText("请先输入IMEI号范围\r\n");
                return;
            }
            NMIM = NMIB.selectNetMarkIMEIByImeiRangeBLL(this.IMEI_Start.Text.Substring(0,14),this.IMEI_End.Text.Substring(0, 14));
            if (NMIM.Count == 0)
            {
                player.Play();
                this.Remind.AppendText("该范围内无记录\r\n");
                return;
            }
            string NT = System.DateTime.Now.ToString("yyyy-MM-dd");
            //打开保存对话框
            SaveFileDialog dialog = new SaveFileDialog();
            dialog.Filter = "文本文件|*.txt";
            dialog.FileName = NT;//这里将要保存的文件默认为某个订单号

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                // 文件名
                string fileName = dialog.FileName;//FileName为用户输入的名字

                //创建文件准备写入
                FileStream fs = File.Open(fileName, FileMode.Create, FileAccess.Write);
                StreamWriter wr = new StreamWriter(fs);

                //逐行将我们需要的内容写入到文件中
                foreach (NetMarkIMEIMes a in NMIM)
                {
                    wr.WriteLine(a.IMEI + "," + a.NetMark);
                    //wr.WriteLine("我是用来测试的语句2\r");
                }
                this.Remind.AppendText("导出成功\r\n");
                //关闭文件
                wr.Flush();
                wr.Close();
                fs.Close();
            }
        }

        //用户登录
        private void Login_Click(object sender, EventArgs e)
        {
            login lo = new login(this);
            lo.ShowDialog();
        }

        //登录成功后解放页面
        public void LoginSuccess()
        {
            this.Login.Enabled = false;
            this.Logout.Enabled = true;
            this.Tolock.Enabled = true;
            this.OutputTxt.Enabled = true;
            this.OutputByImei.Enabled = true;
            this.NetLogoPrefix.ReadOnly = false;
            this.NetLogoSuffix.ReadOnly = false;
            this.NetLogoDigits.ReadOnly = false;
            this.IMEI_Start.ReadOnly = false;
            this.IMEI_End.ReadOnly = false;
            this.Tocheck.ReadOnly = false;
            this.Zhidan_comboBox.Enabled = true;
            this.NoCheck.Enabled = true;
            this.InputAll.Enabled = true;
        }

        //超级管理员权限功能
        public void SuperLogin()
        {
            this.NetLogoOrImei.ReadOnly = false;
            this.DeleteNetLogo.Enabled = true;
            this.IMEI_num1.ReadOnly = false;
            this.IMEI_num2.ReadOnly = false;
            this.ImeiRangeDel.Enabled = true;
        }

        //退出登录按钮
        private void Logout_Click(object sender, EventArgs e)
        {
            this.Login.Enabled = true;
            this.Logout.Enabled = false;
            this.Tolock.Enabled = false;
            this.OutputTxt.Enabled = false;
            this.OutputByImei.Enabled = false;
            this.NetLogoPrefix.ReadOnly = true;
            this.NetLogoSuffix.ReadOnly = true;
            this.NetLogoDigits.ReadOnly = true;
            this.IMEI_Start.ReadOnly = true;
            this.IMEI_End.ReadOnly = true;
            this.Zhidan_comboBox.Enabled = false;
            this.NetLogoOrImei.ReadOnly = true;
            this.DeleteNetLogo.Enabled = false;
            this.IMEI_num1.ReadOnly = true;
            this.IMEI_num2.ReadOnly = true;
            this.Tocheck.ReadOnly = true;
            this.ImeiRangeDel.Enabled = false;
            this.NoCheck.Enabled = false;
            this.InputAll.Enabled = false;
            this.NetUser.Clear();
            this.UserType.Text = "用户:";
            this.Zhidan_comboBox.Text = "";
            this.IMEI_Start.Clear();
            this.IMEI_End.Clear();
            this.PresentImei.Clear();
            this.NetLogoPrefix.Clear();
            this.NetLogoSuffix.Clear();
            this.NetLogoDigits.Clear();
            this.Tocheck.Clear();
            this.CheckResult.Clear();
        }

        //界面退出时
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (MessageBox.Show("是否退出系统？", "系统提示", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.Cancel)
            {
                e.Cancel = true;
            }
        }

        //单个删除按钮函数
        private void DeleteNetLogo_Click(object sender, EventArgs e)
        {
            //未输入值就点删除按钮
            if (this.NetLogoOrImei.Text == "")
            {
                this.Remind.AppendText("请输入IMEI或网标\r\n");
                this.NetLogoOrImei.Focus();
                return;
            }
            //判断是否已经选择了制单号
            if (this.Zhidan_comboBox.Text == "")
            {
                this.Remind.AppendText("请先选择制单号\r\n");
                this.NetLogoOrImei.Clear();
                this.NetLogoOrImei.Focus();
                return;
            }
            //判断IMEI起始位终止位是否为空
            if(this.IMEI_Start.Text=="" || this.IMEI_End.Text == "")
            {
                this.Remind.AppendText("IMEI起始位、终止位不能为空\r\n");
                this.NetLogoOrImei.Clear();
                this.NetLogoOrImei.Focus();
                return;
            }
            //通过以上判断后判断输入的值是否在范围内
            NMIM = NMIB.SelectIMEIByNetLogoOrImerBLL(this.NetLogoOrImei.Text);
            if (NMIM.Count != 0)
            {
                foreach (NetMarkIMEIMes a in NMIM)
                {
                    string inputflag = a.IMEI;
                    if (inputflag != "")
                    {
                        if (this.IMEI_Start.Text.Length == 14)
                        {
                            if (long.Parse(inputflag.Substring(0, 14)) < long.Parse(this.IMEI_Start.Text) || long.Parse(inputflag.Substring(0, 14)) > long.Parse(this.IMEI_End.Text))
                            {
                                player.Play();
                                this.Remind.AppendText("该号不在范围内\r\n");
                                this.NetLogoOrImei.Clear();
                                this.NetLogoOrImei.Focus();
                                return;
                            }
                            HNMIB.InsertHisNetMarkDAL(this.Zhidan_comboBox.Text, a.IMEI, a.NetMark);
                        }
                        else
                        {
                            if (inputflag.CompareTo(this.IMEI_Start.Text) == -1 || inputflag.CompareTo(this.IMEI_End.Text) == 1)
                            {
                                player.Play();
                                this.Remind.AppendText("该号不在范围内\r\n");
                                this.NetLogoOrImei.Clear();
                                this.NetLogoOrImei.Focus();
                                return;
                            }
                            HNMIB.InsertHisNetMarkDAL(this.Zhidan_comboBox.Text, a.IMEI, a.NetMark);
                        }
                    }
                }
            }
            else
            {
                this.Remind.AppendText("找不到" + this.NetLogoOrImei.Text + "的记录\r\n");
                this.NetLogoOrImei.Clear();
                this.NetLogoOrImei.Focus();
                return;
            }
            int DelFlag = NMIB.DeleteAllByNetLogoOrImerBLL(this.NetLogoOrImei.Text);
            if (DelFlag > 0)
            {
                this.Remind.AppendText("删除成功\r\n");
                this.NetLogoOrImei.Clear();
                this.NetLogoOrImei.Focus();
            }
            else
            {
                this.Remind.AppendText("找不到" + this.NetLogoOrImei.Text + "的记录\r\n");
                this.NetLogoOrImei.Clear();
                this.NetLogoOrImei.Focus();
            }
        }

        //点击不校验按钮
        private void NoCheck_Click(object sender, EventArgs e)
        {
            this.ImeiInput.Focus();
        }

        //IMEI扫描框主函数
        private void ImeiInput_KeyPress(object sender, KeyPressEventArgs e)
        {
            if(e.KeyChar == 13)
            {
                //判断范围、格式、校验位等
                if(this.NoCheck.Checked == false)
                {
                    string imei14;
                    String imeiRes = "";
                    if (this.ImeiInput.Text != "" && IsNumeric(this.ImeiInput.Text) && this.ImeiInput.Text.Length == 15)
                    {
                        imei14 = this.ImeiInput.Text.Substring(0, 14);
                        long imeiinput = long.Parse(imei14);
                        if (imeiinput < long.Parse(this.IMEI_Start.Text) || imeiinput > long.Parse(this.IMEI_End.Text))
                        {
                            player.Play();
                            this.Remind.AppendText("IMEI不在范围\r\n");
                            this.ImeiInput.Clear();
                            this.ImeiInput.Focus();
                            return;
                        }
                        else
                        {
                            string imei15 = getimei15(imei14);
                            imeiRes = imei14 + imei15;
                            if (imeiRes != this.ImeiInput.Text)
                            {
                                player1.Play();
                                this.Remind.AppendText(this.ImeiInput.Text + "IMEI校验错误\r\n");
                                this.ImeiInput.Clear();
                                this.ImeiInput.Focus();
                                return;
                            }
                        }
                    }
                    else if (this.ImeiInput.Text == "")
                    {
                        this.ImeiInput.Focus();
                        return;
                    }
                    else
                    {
                        player.Play();
                        this.Remind.AppendText("IMEI格式错误\r\n");
                        this.ImeiInput.Clear();
                        this.ImeiInput.Focus();
                        return;
                    }
                }
                else
                {
                    if (this.ImeiInput.Text != "" && IsNumeric(this.ImeiInput.Text))
                    {
                        long imeiinput = long.Parse(this.ImeiInput.Text);
                        if (imeiinput < long.Parse(this.IMEI_Start.Text) || imeiinput > long.Parse(this.IMEI_End.Text))
                        {
                            player.Play();
                            this.Remind.AppendText("IMEI不在范围\r\n");
                            this.ImeiInput.Clear();
                            this.ImeiInput.Focus();
                            return;
                        }
                    }
                    else if (this.ImeiInput.Text == "")
                    {
                        this.ImeiInput.Focus();
                        return;
                    }
                    else
                    {
                        player.Play();
                        this.Remind.AppendText("IMEI格式错误\r\n");
                        this.ImeiInput.Clear();
                        this.ImeiInput.Focus();
                        return;
                    }
                }
                //判断IMEI是否重号
                if (NMIB.CheckIMEIBLL(this.ImeiInput.Text) > 0)
                {
                    player2.Play();
                    this.Remind.AppendText(this.ImeiInput.Text + "重号\r\n");
                    this.ImeiInput.Clear();
                    this.ImeiInput.Focus();
                    return;
                }
                this.NetLogoInput.Focus();
            }
        }

        //网标扫描框主函数
        private void NetLogoInput_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                if (this.NetLogoInput.Text != "")
                {
                    if (this.NetLogoPrefix.Text == "" || this.NetLogoSuffix.Text == "" || this.NetLogoDigits.Text == "")
                    {
                        player.Play();
                        this.Remind.AppendText("网标前/后缀及位数都不能为空\r\n");
                        this.NetLogoInput.Clear();
                        this.ImeiInput.Clear();
                        this.ImeiInput.Focus();
                        return;
                    }
                    if (this.NetLogoInput.Text.Length != int.Parse(this.NetLogoDigits.Text))
                    {
                        player.Play();
                        this.Remind.AppendText("网标位数错误\r\n");
                        this.NetLogoInput.Clear();
                        this.ImeiInput.Clear();
                        this.ImeiInput.Focus();
                        return;
                    }
                    if (this.NetLogoInput.Text.Substring(0, netpre) != this.NetLogoPrefix.Text)
                    {
                        player.Play();
                        this.Remind.AppendText("网标前缀错误\r\n");
                        this.NetLogoInput.Clear();
                        this.ImeiInput.Clear();
                        this.ImeiInput.Focus();
                        return;
                    }
                    int a = this.NetLogoInput.Text.Length;
                    if (this.NetLogoInput.Text.Remove(0, a-netsuf) != this.NetLogoSuffix.Text)
                    {
                        player.Play();
                        this.Remind.AppendText("网标后缀错误\r\n");
                        this.NetLogoInput.Clear();
                        this.ImeiInput.Clear();
                        this.ImeiInput.Focus();
                        return;
                    }
                    //判断网标是否重号
                    if (NMIB.CheckNetMarkBLL(this.NetLogoInput.Text) > 0)
                    {
                        player2.Play();
                        this.Remind.AppendText(this.NetLogoInput.Text+"网标重号\r\n");
                        this.NetLogoInput.Clear();
                        this.ImeiInput.Clear();
                        this.ImeiInput.Focus();
                        return;
                    }
                    //通过以上判断后可将数据记录到数据库
                    int InsertFlag = NMIB.InsertNetMarkBLL(this.Zhidan_comboBox.Text, this.ImeiInput.Text, this.NetLogoInput.Text);
                    if (InsertFlag > 0)
                    {
                        this.ImeiInput.Clear();
                        this.NetLogoInput.Clear();
                        this.ImeiInput.Focus();
                    }
                }
                else
                {
                    this.NetLogoInput.Focus();
                    return;
                }

            }
        }

        //只需扫入网标，IMEI递增，主函数
        private void OnlyNetMark_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                if (this.NoCheck.Checked == false)
                {
                    //判断前后缀和位数
                    if (this.OnlyNetMark.Text.Length != int.Parse(this.NetLogoDigits.Text))
                    {
                        player.Play();
                        this.Remind.AppendText("网标位数错误\r\n");
                        this.OnlyNetMark.Clear();
                        this.OnlyNetMark.Focus();
                        return;
                    }
                    if (this.OnlyNetMark.Text.Substring(0, netpre) != this.NetLogoPrefix.Text)
                    {
                        player.Play();
                        this.Remind.AppendText("网标前缀错误\r\n");
                        this.OnlyNetMark.Clear();
                        this.OnlyNetMark.Focus();
                        return;
                    }
                    int a = this.OnlyNetMark.Text.Length;
                    if (this.OnlyNetMark.Text.Remove(0, a - netsuf) != this.NetLogoSuffix.Text)
                    {
                        player.Play();
                        this.Remind.AppendText("网标后缀错误\r\n");
                        this.OnlyNetMark.Clear();
                        this.OnlyNetMark.Focus();
                        return;
                    }
                    //判断网标是否重号
                    if (NMIB.CheckNetMarkBLL(this.OnlyNetMark.Text) > 0)
                    {
                        player2.Play();
                        this.Remind.AppendText(this.OnlyNetMark.Text + "网标重号\r\n");
                        this.OnlyNetMark.Clear();
                        this.OnlyNetMark.Focus();
                        return;
                    }
                    //递增IMEI，如果当前位不为空就用当前位递增，否则用起始位
                    if (this.PresentImei.Text != "")
                    {
                        this.StaticImei.Text = this.PresentImei.Text + getimei15(this.PresentImei.Text);
                    }
                    else
                    {
                        this.StaticImei.Text = this.IMEI_Start.Text + getimei15(this.IMEI_Start.Text);
                    }
                    if (long.Parse(this.StaticImei.Text.Substring(0,14)) > long.Parse(this.IMEI_End.Text))
                    {
                        player.Play();
                        this.Remind.AppendText("该订单IMEI号已全部使用完\r\n");
                        this.StaticImei.Clear();
                        this.OnlyNetMark.Clear();
                        this.OnlyNetMark.Focus();
                        return;
                    }
                    //判断IMEI是否重号
                    if (NMIB.CheckIMEIBLL(this.StaticImei.Text) > 0)
                    {
                        player2.Play();
                        this.Remind.AppendText(this.StaticImei.Text + "重号\r\n");
                        this.PresentImei.Text = (long.Parse(this.StaticImei.Text.Substring(0,14)) + 1).ToString();
                        this.StaticImei.Clear();
                        this.OnlyNetMark.Clear();
                        this.OnlyNetMark.Focus();
                        return;
                    }
                    //通过以上判断后可将数据记录到数据库
                    NMIB.InsertNetMarkBLL(this.Zhidan_comboBox.Text, this.StaticImei.Text, this.OnlyNetMark.Text);
                    this.PresentImei.Text = (long.Parse(this.StaticImei.Text.Substring(0, 14)) + 1).ToString();
                    this.StaticImei.Clear();
                    this.OnlyNetMark.Clear();
                    this.OnlyNetMark.Focus();
                }
                else
                {
                    //判断前后缀和位数
                    if (this.OnlyNetMark.Text.Length != int.Parse(this.NetLogoDigits.Text))
                    {
                        player.Play();
                        this.Remind.AppendText("网标位数错误\r\n");
                        this.OnlyNetMark.Clear();
                        this.OnlyNetMark.Focus();
                        return;
                    }
                    if (this.OnlyNetMark.Text.Substring(0, netpre) != this.NetLogoPrefix.Text)
                    {
                        player.Play();
                        this.Remind.AppendText("网标前缀错误\r\n");
                        this.OnlyNetMark.Clear();
                        this.OnlyNetMark.Focus();
                        return;
                    }
                    int a = this.OnlyNetMark.Text.Length;
                    if (this.OnlyNetMark.Text.Remove(0, a - netsuf) != this.NetLogoSuffix.Text)
                    {
                        player.Play();
                        this.Remind.AppendText("网标后缀错误\r\n");
                        this.OnlyNetMark.Clear();
                        this.OnlyNetMark.Focus();
                        return;
                    }
                    //判断网标是否重号
                    if (NMIB.CheckNetMarkBLL(this.OnlyNetMark.Text) > 0)
                    {
                        player2.Play();
                        this.Remind.AppendText(this.OnlyNetMark.Text + "网标重号\r\n");
                        this.OnlyNetMark.Clear();
                        this.OnlyNetMark.Focus();
                        return;
                    }
                    //递增IMEI，如果当前位不为空就用当前位递增，否则用起始位
                    if (this.PresentImei.Text != "")
                    {
                        this.StaticImei.Text = this.PresentImei.Text;
                    }
                    else
                    {
                        this.StaticImei.Text = this.IMEI_Start.Text;
                    }
                    if (long.Parse(this.StaticImei.Text) > long.Parse(this.IMEI_End.Text))
                    {
                        player.Play();
                        this.Remind.AppendText("该订单IMEI号已全部使用完\r\n");
                        this.StaticImei.Clear();
                        this.OnlyNetMark.Clear();
                        this.OnlyNetMark.Focus();
                        return;
                    }
                    //判断IMEI是否重号
                    if (NMIB.CheckIMEIBLL(this.StaticImei.Text) > 0)
                    {
                        player2.Play();
                        this.Remind.AppendText(this.StaticImei.Text + "重号\r\n");
                        this.PresentImei.Text = (long.Parse(this.StaticImei.Text) + 1).ToString();
                        this.StaticImei.Clear();
                        this.OnlyNetMark.Clear();
                        this.OnlyNetMark.Focus();
                        return;
                    }
                    //通过以上判断后可将数据记录到数据库
                    NMIB.InsertNetMarkBLL(this.Zhidan_comboBox.Text, this.StaticImei.Text, this.OnlyNetMark.Text);
                    this.PresentImei.Text = (long.Parse(this.StaticImei.Text) + 1).ToString();
                    this.StaticImei.Clear();
                    this.OnlyNetMark.Clear();
                    this.OnlyNetMark.Focus();
                }
            }
        }

        //点击扫描IMEI和网标复选框时触发的按钮
        private void InputAll_Click(object sender, EventArgs e)
        {
            if(this.InputAll.Checked == true)
            {
                this.L_up2.Hide();
                this.L_down2.Hide();
                this.OnlyNetMark.Hide();
                this.StaticImei.Hide();
                this.L_up1.Show();
                this.L_down1.Show();
                this.ImeiInput.Show();
                this.NetLogoInput.Show();
            }
            else
            {
                this.L_up2.Show();
                this.L_down2.Show();
                this.OnlyNetMark.Show();
                this.StaticImei.Show();
                this.L_up1.Hide();
                this.L_down1.Hide();
                this.ImeiInput.Hide();
                this.NetLogoInput.Hide();
            }
        }

        //批量删除起始位光标离开时判断输入的格式等是否正确
        private void IMEI_num1_Leave(object sender, EventArgs e)
        {
            if (this.IMEI_num1.Text == "")
            {
                return;
            }
            if (this.IMEI_num1.Text.Length >= 14)
            {
                if (this.IMEI_Start.Text.Length == 14)
                {
                    string num1 = this.IMEI_num1.Text.Substring(0, 14);
                    long ImeiNum1 = long.Parse(num1);
                    if (ImeiNum1 < long.Parse(this.IMEI_Start.Text) || ImeiNum1 > long.Parse(this.IMEI_End.Text))
                    {
                        player.Play();
                        this.Remind.AppendText("您输入的IMEI删除起始位不在范围内\r\n");
                        this.IMEI_num1.Clear();
                        this.IMEI_num1.Focus();
                        return;
                    }
                }
                else
                {
                    if (this.IMEI_num1.Text.CompareTo(this.IMEI_Start.Text) == -1 || this.IMEI_num1.Text.CompareTo(this.IMEI_Start.Text) == 1)
                    {
                        player.Play();
                        this.Remind.AppendText("您输入的IMEI删除起始位不在范围内\r\n");
                        this.IMEI_num1.Clear();
                        this.IMEI_num1.Focus();
                        return;
                    }
                }
            }
            else
            {
                player.Play();
                this.Remind.AppendText("请输入正确的IMEI格式\r\n");
                this.IMEI_num1.Clear();
                this.IMEI_num1.Focus();
                return;
            }
        }

        //IMEI范围删除按钮函数
        private void ImeiRangeDel_Click(object sender, EventArgs e)
        {
            //删除前先判断是否已经选择了订单
            if (this.Zhidan_comboBox.Text == "" || this.IMEI_Start.Text == "" || this.IMEI_End.Text == "")
            {
                this.Remind.AppendText("请先选择制单号，IMEI范围不能为空\r\n");
                this.IMEI_num1.Clear();
                this.IMEI_num2.Clear();
                this.IMEI_num1.Focus();
                return;
            }
            if (this.IMEI_num1.Text=="" || this.IMEI_num2.Text == "")
            {
                this.Remind.AppendText("删除起始位和终止位都不能为空\r\n");
                this.IMEI_num1.Clear();
                this.IMEI_num2.Clear();
                this.IMEI_num1.Focus();
                return;
            }
            //判断输入的删除终止位在不在范围内
            if (this.IMEI_Start.Text.Length == 14)
            {
                string num2 = this.IMEI_num2.Text.Substring(0, 14);
                long ImeiNum2 = long.Parse(num2);
                if (ImeiNum2 < long.Parse(this.IMEI_Start.Text) || ImeiNum2 > long.Parse(this.IMEI_End.Text))
                {
                    player.Play();
                    this.Remind.AppendText("您输入的IMEI删除终止位不在范围内\r\n");
                    this.IMEI_num2.Clear();
                    this.IMEI_num2.Focus();
                    return;
                }
            }
            else
            {
                if (this.IMEI_num2.Text.CompareTo(this.IMEI_Start.Text) == -1 || this.IMEI_num2.Text.CompareTo(this.IMEI_Start.Text) == 1)
                {
                    player.Play();
                    this.Remind.AppendText("您输入的IMEI删除终止位不在范围内\r\n");
                    this.IMEI_num2.Clear();
                    this.IMEI_num2.Focus();
                    return;
                }
            }

            //判断输入的终止位有没有大于起始位
            if(long.Parse(IMEI_num2.Text) < long.Parse(IMEI_num1.Text))
            {
                player.Play();
                this.Remind.AppendText("IMEI删除终止位不能小于删除起始位\r\n");
                this.IMEI_num2.Clear();
                this.IMEI_num2.Focus();
                return;
            }
            //逐个判断是否在数据库中存在该条记录，有则删除，无则提示
            if (this.IMEI_Start.Text.Length == 14)
            {
                long Num2Imei14 = long.Parse(this.IMEI_num2.Text.Substring(0, 14));
                for(long Num1Imei14 = long.Parse(this.IMEI_num1.Text.Substring(0, 14)); Num1Imei14<= Num2Imei14; Num1Imei14++)
                {
                    string Num1Imei15 = getimei15(Num1Imei14.ToString());
                    if (NMIB.CheckIMEIBLL(Num1Imei14.ToString()+ Num1Imei15) > 0)
                    {
                        string Dnetmark = NMIB.selectNetMarkByIMEIBLL(Num1Imei14.ToString() + Num1Imei15);
                        HNMIB.InsertHisNetMarkDAL(this.Zhidan_comboBox.Text, Num1Imei14.ToString() + Num1Imei15, Dnetmark);
                        NMIB.DeleteAllByNetLogoOrImerBLL(Num1Imei14.ToString() + Num1Imei15);
                        this.Remind.AppendText("删除成功\r\n");
                    }
                    else
                    {
                        this.Remind.AppendText("找不到" + Num1Imei14+ Num1Imei15 + "的记录\r\n");
                    }
                }
                this.Remind.AppendText("删除完成\r\n");
                this.IMEI_num1.Clear();
                this.IMEI_num2.Clear();
                this.IMEI_num1.Focus();
            }
            else
            {
                long Num2Imei = long.Parse(this.IMEI_num2.Text);
                for (long Num1Imei = long.Parse(this.IMEI_num1.Text); Num1Imei <= Num2Imei; Num1Imei++)
                {
                    if (NMIB.CheckIMEIBLL(Num1Imei.ToString()) > 0)
                    {
                        string Dnetmark = NMIB.selectNetMarkByIMEIBLL(Num1Imei.ToString());
                        HNMIB.InsertHisNetMarkDAL(this.Zhidan_comboBox.Text, Num1Imei.ToString(), Dnetmark);
                        NMIB.DeleteAllByNetLogoOrImerBLL(Num1Imei.ToString());
                        this.Remind.AppendText("删除成功\r\n");
                    }
                    else
                    {
                        this.Remind.AppendText("找不到" + Num1Imei + "的记录\r\n");
                    }
                }
                this.Remind.AppendText("删除完成\r\n");
                this.IMEI_num1.Clear();
                this.IMEI_num2.Clear();
                this.IMEI_num1.Focus();
            }
        }

        //删除起始位按下Enter时的函数
        private void IMEI_num1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                if (this.IMEI_num1.Text != "")
                {
                    this.IMEI_num2.Focus();
                }
            }
        }

        //查询对应网标/IMEI框按下Enter键
        private void Tocheck_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                //空值按下Enter无反应
                if (this.Tocheck.Text == "")
                {
                    return;
                }
                string CResult = NMIB.SelectResultByImeiOrNetmarkBLL(this.Tocheck.Text);
                if (CResult == "")
                {
                    this.Remind.AppendText("找不到"+ this.Tocheck.Text+"的对应关系\r\n");
                    this.CheckResult.Clear();
                    this.Tocheck.Clear();
                    this.Tocheck.Focus();
                }
                else
                {
                    this.CheckResult.Text = CResult;
                }
            }
        }
    }
}
