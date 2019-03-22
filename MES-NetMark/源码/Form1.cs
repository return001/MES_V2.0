using System;
using System.Collections.Generic;
using System.Media;
using System.Windows.Forms;
using NetMarkIMEI.BLL;
using ManuOrderParam.BLL;
using HisNetMarkIMEI.BLL;
using PUsers.BLL;
using NetLogo_Message;
using System.IO;
using System.Drawing.Printing;
using Seagull.BarTender.Print;
using NetMarkConfig.BLL;
using DataRelativeSheet.BLL;
using System.Drawing;

namespace NetLogoTool
{
    public partial class Form1 : Form
    {

        //写日志函数
        public static void Log(string msg, Exception e)
        {
            try
            {
                string path = AppDomain.CurrentDomain.BaseDirectory + "\\log\\" + System.DateTime.Now.ToString("yyyy-MM-dd") + ".log";
                if (!File.Exists(path))
                {
                    File.Create(path).Close();
                }
                StreamWriter writer = new StreamWriter(path, true);
                writer.WriteLine("");
                writer.WriteLine(System.DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss") + " " + msg);
                writer.Flush();
                writer.Close();
            }
            catch
            {
                string path = AppDomain.CurrentDomain.BaseDirectory + "\\log\\" + System.DateTime.Now.ToString("yyyy-MM-dd") + ".log";
                if (!Directory.Exists(path))
                {
                    File.Create(path).Close();
                }
                StreamWriter writer = File.AppendText(path);
                writer.WriteLine("");
                writer.WriteLine(System.DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss") + " " + msg + "错误：" + e.Message);
                writer.Flush();
                writer.Close();
            }
        }

        //记录网标前缀位数和后缀位数
        int netpre;
        int netsuf;
        //记录RFID前缀位数
        int RfidPreDig;

        //记录模板路劲
        string lj;
        //记录模板刷新次数
        int RefreshNum = 0;
        //设置打印模板份数
        int TN = 1;
        //记录无检验位时IMEI的前缀
        string IMEIPre;

        Engine btEngine = new Engine();

        //音频文件
        SoundPlayer player = new SoundPlayer(AppDomain.CurrentDomain.BaseDirectory + "失败.wav");
        SoundPlayer player1 = new SoundPlayer(AppDomain.CurrentDomain.BaseDirectory + "校验错误.wav");
        SoundPlayer player2 = new SoundPlayer(AppDomain.CurrentDomain.BaseDirectory + "重号.wav");

        NetMarkIMEIBLL NMIB = new NetMarkIMEIBLL();

        HisNetMarkIMEIBLL HNMIB = new HisNetMarkIMEIBLL();

        ManuOrderParamBLL MOPB = new ManuOrderParamBLL();

        NetMarkConfigBLL NMCB = new NetMarkConfigBLL();

        DataRelativeSheetBLL DRSB = new DataRelativeSheetBLL();

        PUsersBLL PUB = new PUsersBLL();

        List<Gps_ManuOrderParam> G_MOP = new List<Gps_ManuOrderParam>();

        List<NetMarkIMEIMes> NMIM = new List<NetMarkIMEIMes>();

        List<G_NetMarkConfig> NMC = new List<G_NetMarkConfig>();

        public Form1()
        {
            this.WindowState = FormWindowState.Maximized;
            int wid = Screen.PrimaryScreen.WorkingArea.Width;
            int hei = Screen.PrimaryScreen.WorkingArea.Height;
            this.Width = wid;
            InitializeComponent();
        }

        //程序加载时运行
        private void Form1_Load(object sender, EventArgs e)
        {
            string NowData = System.DateTime.Now.ToString("yyyy.MM.dd");
            this.PrintData.Text = NowData;
            PrintDocument print = new PrintDocument();
            string sDefault = print.PrinterSettings.PrinterName;//默认打印机名
            this.Printer.Text = sDefault;
            foreach (string sPrint in PrinterSettings.InstalledPrinters)//获取所有打印机名称
            {
                Printer.Items.Add(sPrint);
            }
            //开启打印机引擎
            btEngine.Start();
        }

        //更换数据库时调用
        public void refrezhidan()
        {
            PUB.refeshConBLL();
            MOPB.refeshConBLL();
            HNMIB.refeshConBLL();
            NMIB.refeshConBLL();
            DRSB.refeshConBLL();
            NMCB.refeshConBLL();
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
            this.ImeiPresent.Clear();
            this.OnlyNetMark.Clear();
            this.StaticImei.Clear();
            this.RFIDStart.Clear();
            this.RFIDDigts.Clear();
            this.RFIDPresent.Clear();
            //根据制单获取IMEI起始位终止位
            G_MOP = MOPB.selectManuOrderParamByzhidanBLL(this.Zhidan_comboBox.Text);
            foreach (Gps_ManuOrderParam b in G_MOP)
            {
                this.IMEI_Start.Text = b.IMEIStart;
                this.IMEI_End.Text = b.IMEIEnd;
                this.Template.Text = b.JST_template;
            }
            //到配置表读取上次配置内容
            NMC = NMCB.selectNetMarkParamByzhidanBLL(this.Zhidan_comboBox.Text);
            foreach (G_NetMarkConfig a in NMC)
            {
                if (NMC.Count != 0)
                {
                    this.NoCheck.Checked = Convert.ToBoolean(a.NoCheckCode);
                    this.RFIDAutoAdd.Checked = Convert.ToBoolean(a.RFIDAutoAddMark);
                    this.Template.Text = a.Temlpate;
                    lj = a.Temlpate;
                    this.ImeiPresent.Text = a.IMEIPresent;
                    this.NetLogoPrefix.Text = a.NetMarkPre;
                    this.NetLogoSuffix.Text = a.NetMarkSuf;
                    this.NetLogoDigits.Text = a.NetMarkDig;
                    this.RFIDStart.Text = a.RFIDStart;
                    this.RFIDDigts.Text = a.RFIDEnd;
                    this.RFIDPresent.Text = a.RFIDPresent;
                    this.TemplateNum.Text = a.TemplateNum.ToString();
                    TN = a.TemplateNum;
                    this.InputAll.Checked = Convert.ToBoolean(a.InputAllMark);
                    InputAll_Click(sender, e);
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

        //判断是否为日期格式的函数
        public bool IsDate(string strDate)
        {
            try
            {
                DateTime.Parse(strDate);
                return true;
            }
            catch
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
            if (this.Tolock.Text == "解锁")
            {
                Unlock ul = new Unlock(this);
                ul.ShowDialog();
                return;
            }
            //锁定前先判断网标是否已经配置好
            if (this.NetLogoPrefix.Text == "" || this.NetLogoSuffix.Text == "" || this.NetLogoDigits.Text == "")
            {
                player.Play();
                this.Remind.AppendText("网标前/后缀及位数都不能为空\r\n");
                return;
            }
            //判断是否已经选择订单
            if (this.Zhidan_comboBox.Text == "" || this.IMEI_Start.Text == "" || this.IMEI_End.Text == "")
            {
                player.Play();
                this.Remind.AppendText("制单、IMEI起始位终止位都不能为空\r\n");
                return;
            }
            //判断是否已经选择了模板
            if (this.Template.Text == "")
            {
                player.Play();
                this.Remind.AppendText("请先选择模板\r\n");
                return;
            }
            ////判断在DFID递增的情况下是否已经配置了RFID
            if (this.RFIDAutoAdd.Checked == true)
            {
                if (this.RFIDStart.Text == "" || this.RFIDDigts.Text == "")
                {
                    player.Play();
                    this.Remind.AppendText("RFID前缀和位数不能为空\r\n");
                    return;
                }
            }
            if(this.RFIDStart.Text != "")
            {
                RfidPreDig = this.RFIDStart.Text.Length;
            }
            //判断RFID位数有没有比前缀位数多
            if (this.RFIDStart.Text != "" && this.RFIDDigts.Text != "")
            {
                if (int.Parse(this.RFIDDigts.Text) <= RfidPreDig)
                {
                    player.Play();
                    this.Remind.AppendText("RFID位数不能少于等于前缀位数\r\n");
                    return;
                }
            }
            if (this.RFIDAutoAdd.Checked == true)
            {
                //如果设置了RFID当前位，判断RFID当前位是否在前缀和位数的范围内
                if (this.RFIDPresent.Text != "")
                {
                    if (this.RFIDPresent.Text.Length != int.Parse(this.RFIDDigts.Text))
                    {
                        player.Play();
                        this.Remind.AppendText("RFID当前位位数错误\r\n");
                        return;
                    }
                    if (this.RFIDPresent.Text.Substring(0, RfidPreDig) != this.RFIDStart.Text)
                    {
                        player.Play();
                        this.Remind.AppendText("RFID当前位前缀错误\r\n");
                        return;
                    }
                }
            }
            //判断打印日期是否为日期格式
            if (!IsDate(this.PrintData.Text))
            {
                player.Play();
                this.Remind.AppendText("打印日期格式不正确\r\n");
                string NowData = System.DateTime.Now.ToString("yyyy.MM.dd");
                this.PrintData.Text = NowData;
                return;
            }
            //如果设置了IMEI当前位，判断当前IMEI号是否在范围内
            if (this.ImeiPresent.Text != "")
            {
                if (this.ImeiPresent.Text.Length != this.IMEI_Start.Text.Length)
                {
                    player.Play();
                    this.Remind.AppendText("IMEI当前位范围错误\r\n");
                    return;
                }
                if (this.ImeiPresent.Text.CompareTo(this.IMEI_Start.Text) == -1 || this.ImeiPresent.Text.CompareTo(this.IMEI_End.Text) == 1)
                {
                    player.Play();
                    this.Remind.AppendText("IMEI当前位范围错误\r\n");
                    return;
                }
            }
            //锁定界面
            if (this.Tolock.Text == "锁定")
            {
                this.Tolock.Text = "解锁";
                this.Zhidan_comboBox.Enabled = false;
                this.NetLogoPrefix.ReadOnly = true;
                this.NetLogoSuffix.ReadOnly = true;
                this.NetLogoDigits.ReadOnly = true;
                this.IMEI_Start.ReadOnly = true;
                this.IMEI_End.ReadOnly = true;
                this.ImeiPresent.ReadOnly = true;
                this.RFIDStart.ReadOnly = true;
                this.RFIDDigts.ReadOnly = true;
                this.RFIDPresent.ReadOnly = true;
                this.TemplateNum.ReadOnly = true;
                this.PrintData.ReadOnly = true;
                this.ImeiInput.ReadOnly = false;
                this.NetLogoInput.ReadOnly = false;
                this.ReImei.ReadOnly = false;
                this.NoCheck.Enabled = false;
                this.InputAll.Enabled = false;
                this.RFIDAutoAdd.Enabled = false;
                this.CheckImeiByNetmark.Enabled = false;
                this.OutputTxt.Enabled = false;
                this.OutputByImei.Enabled = false;
                this.Logout.Enabled = false;
                this.ChooseTemplate.Enabled = false;
                this.Refresh_template.Enabled = false;
                this.Open_Template.Enabled = false;
                this.Printer.Enabled = false;
                this.ImeiInput.Focus();
                if (this.InputAll.Checked == false)
                {
                    this.OnlyNetMark.ReadOnly = false;
                    this.OnlyNetMark.Focus();
                }
                if (this.RFIDAutoAdd.Checked == false)
                {
                    this.StaticRFID.ReadOnly = false;
                }
                netpre = this.NetLogoPrefix.Text.Length;
                netsuf = this.NetLogoSuffix.Text.Length;
                IMEIPre = this.IMEI_Start.Text.Substring(0, IMEI_Start.Text.Length - 5);
            }
            //记录页面配置
            NMCB.InsertPrintRecordParamBLL(Zhidan_comboBox.Text, Template.Text, ImeiPresent.Text, NetLogoPrefix.Text, NetLogoSuffix.Text, NetLogoDigits.Text, RFIDStart.Text, RFIDDigts.Text, RFIDPresent.Text, Convert.ToInt32(NoCheck.Checked), Convert.ToInt32(InputAll.Checked), Convert.ToInt32(RFIDAutoAdd.Checked), int.Parse(this.TemplateNum.Text));
        }

        //解锁的内容
        public void Unlock_content()
        {
            this.Tolock.Text = "锁定";
            this.Zhidan_comboBox.Enabled = true;
            this.NetLogoPrefix.ReadOnly = false;
            this.NetLogoSuffix.ReadOnly = false;
            this.NetLogoDigits.ReadOnly = false;
            this.IMEI_Start.ReadOnly = false;
            this.IMEI_End.ReadOnly = false;
            this.ImeiPresent.ReadOnly = false;
            this.RFIDStart.ReadOnly = false;
            this.RFIDDigts.ReadOnly = false;
            this.RFIDPresent.ReadOnly = false;
            this.TemplateNum.ReadOnly = false;
            this.PrintData.ReadOnly = false;
            this.ImeiInput.ReadOnly = true;
            this.NetLogoInput.ReadOnly = true;
            this.OnlyNetMark.ReadOnly = true;
            this.StaticRFID.ReadOnly = true;
            this.ReImei.ReadOnly = true;
            this.NoCheck.Enabled = true;
            this.InputAll.Enabled = true;
            this.RFIDAutoAdd.Enabled = true;
            this.CheckImeiByNetmark.Enabled = true;
            this.OutputTxt.Enabled = true;
            this.OutputByImei.Enabled = true;
            this.Logout.Enabled = true;
            this.ChooseTemplate.Enabled = true;
            this.Refresh_template.Enabled = true;
            this.Open_Template.Enabled = true;
            this.CheckAll.Enabled = true;
            this.Printer.Enabled = true;
            this.ImeiInput.Clear();
            this.NetLogoInput.Clear();
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
            if (this.IMEI_Start.Text.Length == 14)
            {
                NMIM = NMIB.selectNetMarkIMEIByImeiRangeBLL(this.IMEI_Start.Text + getimei15(this.IMEI_Start.Text), this.IMEI_End.Text.Substring(0, 14) + getimei15(this.IMEI_End.Text));
            }
            else
            {
                NMIM = NMIB.selectNetMarkIMEIByImeiRangeBLL(this.IMEI_Start.Text, this.IMEI_End.Text);
            }
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
            this.ChooseTemplate.Enabled = true;
            this.Refresh_template.Enabled = true;
            this.Open_Template.Enabled = true;
            this.CheckAll.Enabled = true;
            this.Printer.Enabled = true;
            this.Zhidan_comboBox.Enabled = true;
            this.NetLogoPrefix.ReadOnly = false;
            this.NetLogoSuffix.ReadOnly = false;
            this.NetLogoDigits.ReadOnly = false;
            this.IMEI_Start.ReadOnly = false;
            this.IMEI_End.ReadOnly = false;
            this.ImeiPresent.ReadOnly = false;
            this.RFIDStart.ReadOnly = false;
            this.RFIDDigts.ReadOnly = false;
            this.RFIDPresent.ReadOnly = false;
            this.Tocheck.ReadOnly = false;
            this.TemplateNum.ReadOnly = false;
            this.PrintData.ReadOnly = false;
            this.NoCheck.Enabled = true;
            this.InputAll.Enabled = true;
            this.RFIDAutoAdd.Enabled = true;
            this.CheckImeiByNetmark.Enabled = true;
            this.SqlConfig.Enabled = false;
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
            this.SqlConfig.Enabled = true;
            this.OutputTxt.Enabled = false;
            this.OutputByImei.Enabled = false;
            this.ChooseTemplate.Enabled = false;
            this.Refresh_template.Enabled = false;
            this.Open_Template.Enabled = false;
            this.CheckAll.Enabled = false;
            this.Printer.Enabled = false;
            this.NetLogoPrefix.ReadOnly = true;
            this.NetLogoSuffix.ReadOnly = true;
            this.NetLogoDigits.ReadOnly = true;
            this.IMEI_Start.ReadOnly = true;
            this.IMEI_End.ReadOnly = true;
            this.ImeiPresent.ReadOnly = true;
            this.RFIDStart.ReadOnly = true;
            this.RFIDDigts.ReadOnly = true;
            this.TemplateNum.ReadOnly = true;
            this.Zhidan_comboBox.Enabled = false;
            this.NetLogoOrImei.ReadOnly = true;
            this.DeleteNetLogo.Enabled = false;
            this.IMEI_num1.ReadOnly = true;
            this.IMEI_num2.ReadOnly = true;
            this.Tocheck.ReadOnly = true;
            this.ReImei.ReadOnly = true;
            this.ImeiRangeDel.Enabled = false;
            this.NoCheck.Enabled = false;
            this.InputAll.Enabled = false;
            this.RFIDAutoAdd.Enabled = false;
            this.CheckImeiByNetmark.Enabled = false;
            this.NetUser.Clear();
            this.UserType.Text = "用户:";
            this.Zhidan_comboBox.Text = "";
            this.IMEI_Start.Clear();
            this.IMEI_End.Clear();
            this.ImeiPresent.Clear();
            this.NetLogoPrefix.Clear();
            this.NetLogoSuffix.Clear();
            this.NetLogoDigits.Clear();
            this.RFIDStart.Clear();
            this.RFIDDigts.Clear();
            this.RFIDPresent.Clear();
            this.Tocheck.Clear();
            this.Template.Clear();
        }

        //界面退出时
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            string path = AppDomain.CurrentDomain.BaseDirectory;
            if (Directory.Exists(path + "机身贴模板"))
            {
                if (File.GetAttributes(path + "机身贴模板") == FileAttributes.Directory)
                {
                    Directory.Delete(path + "机身贴模板", true);
                }
            }
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
                this.Sen_remind.AppendText("请输入IMEI或网标\r\n");
                this.NetLogoOrImei.Focus();
                return;
            }
            //判断是否已经选择了制单号
            if (this.Zhidan_comboBox.Text == "")
            {
                this.Sen_remind.AppendText("请先选择制单号\r\n");
                this.NetLogoOrImei.Clear();
                this.NetLogoOrImei.Focus();
                return;
            }
            //判断IMEI起始位终止位是否为空
            if(this.IMEI_Start.Text=="" || this.IMEI_End.Text == "")
            {
                this.Sen_remind.AppendText("IMEI起始位、终止位不能为空\r\n");
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
                                this.Sen_remind.AppendText("该号不在范围内\r\n");
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
                                this.Sen_remind.AppendText("该号不在范围内\r\n");
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
                this.Sen_remind.AppendText("找不到" + this.NetLogoOrImei.Text + "的记录\r\n");
                this.NetLogoOrImei.Clear();
                this.NetLogoOrImei.Focus();
                return;
            }
            int DelFlag = NMIB.DeleteAllByNetLogoOrImerBLL(this.NetLogoOrImei.Text);
            if (DelFlag > 0)
            {
                this.Sen_remind.AppendText("删除成功\r\n");
                this.NetLogoOrImei.Clear();
                this.NetLogoOrImei.Focus();
            }
            else
            {
                this.Sen_remind.AppendText("找不到" + this.NetLogoOrImei.Text + "的记录\r\n");
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
                    if (this.ImeiInput.Text != "")
                    {
                        if(this.ImeiInput.Text.Length != this.IMEI_Start.Text.Length)
                        {
                            player.Play();
                            this.Remind.AppendText("IMEI不在范围\r\n");
                            this.ImeiInput.Clear();
                            this.ImeiInput.Focus();
                            return;
                        }
                        if (this.ImeiInput.Text.CompareTo(this.IMEI_Start.Text)==-1 || this.ImeiInput.Text.CompareTo(this.IMEI_End.Text)==1)
                        {
                            player.Play();
                            this.Remind.AppendText("IMEI不在范围\r\n");
                            this.ImeiInput.Clear();
                            this.ImeiInput.Focus();
                            return;
                        }
                    }
                    else
                    {
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
                    if (this.RFIDAutoAdd.Checked == true)
                    {
                        //递增RFID
                        if (this.RFIDPresent.Text != "")
                        {
                            this.StaticRFID.Text = this.RFIDPresent.Text;
                            if (this.StaticRFID.Text.Length > int.Parse(this.RFIDDigts.Text))
                            {
                                player.Play();
                                this.Remind.AppendText("此订单号的RFID已全部使用完\r\n");
                                this.StaticImei.Clear();
                                this.StaticRFID.Clear();
                                this.OnlyNetMark.Clear();
                                this.OnlyNetMark.Focus();
                                return;
                            }
                        }
                        else
                        {
                            this.StaticRFID.Text = this.RFIDStart.Text.PadRight(RfidPreDig, '0');
                        }
                        //判断RFID是否重号
                        if (NMIB.CheckRFIDBLL(this.StaticRFID.Text) > 0)
                        {
                            player2.Play();
                            this.Remind.AppendText(this.StaticRFID.Text + "重号\r\n");
                            this.RFIDPresent.Text = this.RFIDStart.Text + (long.Parse(this.StaticRFID.Text.Remove(0, RfidPreDig)) + 1).ToString().PadLeft(int.Parse(this.RFIDDigts.Text) - RfidPreDig, '0');
                            this.NetLogoInput.Clear();
                            this.ImeiInput.Clear();
                            this.StaticRFID.Clear();
                            this.ImeiInput.Focus();
                            return;
                        }
                        //打印机身贴
                        LabelFormatDocument btFormat = btEngine.Documents.Open(lj);
                        //指定打印机名称
                        btFormat.PrintSetup.PrinterName = this.Printer.Text;
                        //打印份数,同序列打印的份数
                        btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                        //对模板相应字段进行赋值
                        btFormat.SubStrings["IMEI"].Value = this.ImeiInput.Text;
                        btFormat.SubStrings["RFID"].Value = this.StaticRFID.Text;
                        btFormat.SubStrings["ProductDate"].Value = this.PrintData.Text;
                        //判断关联表是否有重号
                        string RlsResult = DRSB.CheckRelativeSheetBLL(this.ImeiInput.Text, this.StaticRFID.Text, this.NetLogoInput.Text);
                        if (RlsResult != "")
                        {
                            player2.Play();
                            this.Remind.AppendText(RlsResult + "\r\n");
                            this.NetLogoInput.Clear();
                            this.ImeiInput.Clear();
                            this.StaticRFID.Clear();
                            this.ImeiInput.Focus();
                            return;
                        }
                        //记录到关联表
                        DRSB.InsertRelativeSheetBLL(this.ImeiInput.Text,this.StaticRFID.Text,this.Zhidan_comboBox.Text, this.NetLogoInput.Text);
                        //通过以上判断后可将数据记录到数据库
                        NMIB.InsertNetMarkBLL(this.Zhidan_comboBox.Text, this.ImeiInput.Text, this.NetLogoInput.Text, this.StaticRFID.Text, this.Template.Text);
                        btFormat.Print();
                        this.RFIDPresent.Text = this.RFIDStart.Text + (long.Parse(this.StaticRFID.Text.Remove(0, RfidPreDig)) + 1).ToString().PadLeft(int.Parse(this.RFIDDigts.Text) - RfidPreDig, '0');
                        NMCB.UpdatePrensentIMEIAndRFIDBLL(this.Zhidan_comboBox.Text, this.ImeiPresent.Text, this.RFIDPresent.Text);
                        Log("打印了IMEI为" + this.ImeiInput.Text +"的贴纸，网标为"+ this.NetLogoInput.Text, null);
                        this.ImeiInput.Clear();
                        this.NetLogoInput.Clear();
                        this.StaticRFID.Clear();
                        this.ImeiInput.Focus();
                    }
                    else
                    {
                        this.StaticRFID.Focus();
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
                    if (this.CheckImeiByNetmark.Checked == false)
                    {
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
                        if (this.ImeiPresent.Text != "")
                        {
                            this.StaticImei.Text = this.ImeiPresent.Text + getimei15(this.ImeiPresent.Text);
                        }
                        else
                        {
                            this.StaticImei.Text = this.IMEI_Start.Text + getimei15(this.IMEI_Start.Text);
                        }
                        if (long.Parse(this.StaticImei.Text.Substring(0, 14)) > long.Parse(this.IMEI_End.Text))
                        {
                            player.Play();
                            this.Remind.AppendText("该订单IMEI号已全部使用完\r\n");
                            this.StaticImei.Clear();
                            this.OnlyNetMark.Clear();
                            this.StaticRFID.Clear();
                            this.OnlyNetMark.Focus();
                            return;
                        }
                        //判断IMEI是否重号
                        if (NMIB.CheckIMEIBLL(this.StaticImei.Text) > 0)
                        {
                            player2.Play();
                            this.Remind.AppendText(this.StaticImei.Text + "重号\r\n");
                            this.ImeiPresent.Text = (long.Parse(this.StaticImei.Text.Substring(0, 14)) + 1).ToString();
                            this.StaticImei.Clear();
                            this.StaticRFID.Clear();
                            this.OnlyNetMark.Clear();
                            this.OnlyNetMark.Focus();
                            return;
                        }
                    }
                    else
                    {
                        this.StaticImei.Text = NMIB.SelectIMEIByNetMarkBLL(this.OnlyNetMark.Text);
                        if (this.StaticImei.Text == "")
                        {
                            player.Play();
                            this.Remind.AppendText(this.OnlyNetMark.Text+"，此网标找不到IMEI\r\n");
                            this.StaticImei.Clear();
                            this.StaticRFID.Clear();
                            this.OnlyNetMark.Clear();
                            this.OnlyNetMark.Focus();
                            return;
                        }
                        else
                        {
                            if (long.Parse(this.StaticImei.Text.Substring(0, 14)) > long.Parse(this.IMEI_End.Text) || long.Parse(this.StaticImei.Text.Substring(0, 14)) < long.Parse(this.IMEI_Start.Text))
                            {
                                player.Play();
                                this.Remind.AppendText(this.StaticImei.Text+"IMEI号不在范围内\r\n");
                                this.StaticImei.Clear();
                                this.OnlyNetMark.Clear();
                                this.StaticRFID.Clear();
                                this.OnlyNetMark.Focus();
                                return;
                            }
                        }
                    }
                    if (this.RFIDAutoAdd.Checked == true)
                    {
                        //递增RFID
                        if (this.RFIDPresent.Text != "")
                        {
                            this.StaticRFID.Text = this.RFIDPresent.Text;
                            if (this.StaticRFID.Text.Length > int.Parse(this.RFIDDigts.Text))
                            {
                                player.Play();
                                this.Remind.AppendText("此订单号的RFID已全部使用完\r\n");
                                this.StaticImei.Clear();
                                this.StaticRFID.Clear();
                                this.OnlyNetMark.Clear();
                                this.OnlyNetMark.Focus();
                                return;
                            }
                        }
                        else
                        {
                            this.StaticRFID.Text = this.RFIDStart.Text.PadRight(int.Parse(this.RFIDDigts.Text), '0');
                        }
                        //判断RFID是否重号
                        if (NMIB.CheckRFIDBLL(this.StaticRFID.Text) > 0)
                        {
                            player2.Play();
                            this.Remind.AppendText(this.StaticRFID.Text + "重号\r\n");
                            this.RFIDPresent.Text = this.RFIDStart.Text + (long.Parse(this.StaticRFID.Text.Remove(0,RfidPreDig))+1).ToString().PadLeft(int.Parse(this.RFIDDigts.Text)-RfidPreDig,'0');
                            this.StaticImei.Clear();
                            this.StaticRFID.Clear();
                            this.OnlyNetMark.Clear();
                            this.OnlyNetMark.Focus();
                            return;
                        }
                        //打印机身贴
                        LabelFormatDocument btFormat = btEngine.Documents.Open(lj);
                        //指定打印机名称
                        btFormat.PrintSetup.PrinterName = this.Printer.Text;
                        //打印份数,同序列打印的份数
                        btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                        //对模板相应字段进行赋值
                        btFormat.SubStrings["IMEI"].Value = this.StaticImei.Text;
                        btFormat.SubStrings["RFID"].Value = this.StaticRFID.Text;
                        btFormat.SubStrings["ProductDate"].Value = this.PrintData.Text;
                        //判断关联表是否有重号
                        string RlsResult = DRSB.CheckRelativeSheetBLL(this.StaticImei.Text, this.StaticRFID.Text, this.OnlyNetMark.Text);
                        if (RlsResult != "")
                        {
                            player2.Play();
                            this.Remind.AppendText(RlsResult + "\r\n");
                            this.StaticImei.Clear();
                            this.StaticRFID.Clear();
                            this.OnlyNetMark.Clear();
                            this.OnlyNetMark.Focus();
                            return;
                        }
                        //记录到关联表
                        DRSB.InsertRelativeSheetBLL(this.StaticImei.Text,this.StaticRFID.Text,this.Zhidan_comboBox.Text, this.OnlyNetMark.Text);
                        //通过以上判断后可将数据记录到数据库
                        NMIB.InsertNetMarkBLL(this.Zhidan_comboBox.Text, this.StaticImei.Text, this.OnlyNetMark.Text, this.StaticRFID.Text, this.Template.Text);
                        //记录后打印
                        btFormat.Print();
                        //打印完累加当前位并清空等待下一次扫入
                        this.ImeiPresent.Text = (long.Parse(this.StaticImei.Text.Substring(0, 14)) + 1).ToString();
                        this.RFIDPresent.Text = this.RFIDStart.Text + (long.Parse(this.StaticRFID.Text.Remove(0, RfidPreDig)) + 1).ToString().PadLeft(int.Parse(this.RFIDDigts.Text) - RfidPreDig, '0');
                        NMCB.UpdatePrensentIMEIAndRFIDBLL(this.Zhidan_comboBox.Text, this.ImeiPresent.Text, this.RFIDPresent.Text);
                        Log("打印了IMEI为" + this.StaticImei.Text + "的贴纸，网标为" + this.OnlyNetMark.Text, null);
                        this.StaticImei.Clear();
                        this.StaticRFID.Clear();
                        this.OnlyNetMark.Clear();
                        this.OnlyNetMark.Focus();
                    }
                    else
                    {
                        this.StaticRFID.Focus();
                    }
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
                    if (this.CheckImeiByNetmark.Checked == false)
                    {
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
                        if (this.ImeiPresent.Text != "")
                        {
                            this.StaticImei.Text = this.ImeiPresent.Text;
                        }
                        else
                        {
                            this.StaticImei.Text = this.IMEI_Start.Text;
                        }
                        if (this.StaticImei.Text.CompareTo(this.IMEI_End.Text) == 1)
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
                            string IMEISuf = this.StaticImei.Text.Remove(0, this.StaticImei.Text.Length - 5);
                            this.ImeiPresent.Text = IMEIPre + (long.Parse(IMEISuf) + 1).ToString().PadLeft(5, '0');
                            this.StaticImei.Clear();
                            this.OnlyNetMark.Clear();
                            this.OnlyNetMark.Focus();
                            return;
                        }
                    }
                    else
                    {
                        this.StaticImei.Text = NMIB.SelectIMEIByNetMarkBLL(this.OnlyNetMark.Text);
                        if (this.StaticImei.Text == "")
                        {
                            player.Play();
                            this.Remind.AppendText(this.OnlyNetMark.Text + "，此网标找不到IMEI\r\n");
                            this.StaticImei.Clear();
                            this.StaticRFID.Clear();
                            this.OnlyNetMark.Clear();
                            this.OnlyNetMark.Focus();
                            return;
                        }
                        else
                        {
                            if (this.StaticImei.Text.CompareTo(this.IMEI_Start.Text) == -1||this.StaticImei.Text.CompareTo(this.IMEI_End.Text) == 1)
                            {
                                player.Play();
                                this.Remind.AppendText(this.StaticImei.Text + "IMEI号不在范围内\r\n");
                                this.StaticImei.Clear();
                                this.OnlyNetMark.Clear();
                                this.StaticRFID.Clear();
                                this.OnlyNetMark.Focus();
                                return;
                            }
                        }
                    }
                    if (this.RFIDAutoAdd.Checked == true)
                    {
                        //递增RFID
                        if (this.RFIDPresent.Text != "")
                        {
                            this.StaticRFID.Text = this.RFIDPresent.Text;
                            if (this.StaticRFID.Text.Length > int.Parse(this.RFIDDigts.Text))
                            {
                                player.Play();
                                this.Remind.AppendText("此订单号的RFID已全部使用完\r\n");
                                this.StaticImei.Clear();
                                this.StaticRFID.Clear();
                                this.OnlyNetMark.Clear();
                                this.OnlyNetMark.Focus();
                                return;
                            }
                        }
                        else
                        {
                            this.StaticRFID.Text = this.RFIDStart.Text.PadRight(int.Parse(this.RFIDDigts.Text), '0');
                        }
                        //判断RFID是否重号
                        if (NMIB.CheckRFIDBLL(this.StaticRFID.Text) > 0)
                        {
                            player2.Play();
                            this.Remind.AppendText(this.StaticRFID.Text + "重号\r\n");
                            this.RFIDPresent.Text = this.RFIDStart.Text + (long.Parse(this.StaticRFID.Text.Remove(0, RfidPreDig)) + 1).ToString().PadLeft(int.Parse(this.RFIDDigts.Text) - RfidPreDig, '0');
                            this.StaticImei.Clear();
                            this.StaticRFID.Clear();
                            this.OnlyNetMark.Clear();
                            this.OnlyNetMark.Focus();
                            return;
                        }
                        //打印机身贴
                        LabelFormatDocument btFormat = btEngine.Documents.Open(lj);
                        //指定打印机名称
                        btFormat.PrintSetup.PrinterName = this.Printer.Text;
                        //打印份数,同序列打印的份数
                        btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                        //对模板相应字段进行赋值
                        btFormat.SubStrings["IMEI"].Value = this.StaticImei.Text;
                        btFormat.SubStrings["RFID"].Value = this.StaticRFID.Text;
                        btFormat.SubStrings["ProductDate"].Value = this.PrintData.Text;
                        //判断关联表是否有重号
                        string RlsResult = DRSB.CheckRelativeSheetBLL(this.StaticImei.Text, this.StaticRFID.Text, this.OnlyNetMark.Text);
                        if (RlsResult != "")
                        {
                            player2.Play();
                            this.Remind.AppendText(RlsResult + "\r\n");
                            this.StaticImei.Clear();
                            this.StaticRFID.Clear();
                            this.OnlyNetMark.Clear();
                            this.OnlyNetMark.Focus();
                            return;
                        }
                        //记录到关联表
                        DRSB.InsertRelativeSheetBLL(this.StaticImei.Text, this.StaticRFID.Text,this.Zhidan_comboBox.Text, this.OnlyNetMark.Text);
                        //通过以上判断后可将数据记录到数据库
                        NMIB.InsertNetMarkBLL(this.Zhidan_comboBox.Text, this.StaticImei.Text, this.OnlyNetMark.Text, this.StaticRFID.Text, this.Template.Text);
                        //记录后打印
                        btFormat.Print();
                        //打印完累加当前位并清空等待下一次扫入
                        string IMEISuf = this.StaticImei.Text.Remove(0, this.StaticImei.Text.Length - 5);
                        this.ImeiPresent.Text = IMEIPre + (long.Parse(IMEISuf) + 1).ToString().PadLeft(5,'0');
                        this.RFIDPresent.Text = this.RFIDStart.Text + (long.Parse(this.StaticRFID.Text.Remove(0, RfidPreDig)) + 1).ToString().PadLeft(int.Parse(this.RFIDDigts.Text) - RfidPreDig, '0');
                        NMCB.UpdatePrensentIMEIAndRFIDBLL(this.Zhidan_comboBox.Text, this.ImeiPresent.Text, this.RFIDPresent.Text);
                        Log("打印了IMEI为" + this.StaticImei.Text + "的贴纸，网标为" + this.OnlyNetMark.Text, null);
                        this.StaticImei.Clear();
                        this.StaticRFID.Clear();
                        this.OnlyNetMark.Clear();
                        this.OnlyNetMark.Focus();
                    }
                    else
                    {
                        this.StaticRFID.Focus();
                    }
                }
            }
        }

        //扫入RFID
        private void StaticRFID_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                if (this.InputAll.Checked == false)
                {
                    //判断扫入的RFID是否在范围内
                    if (this.RFIDDigts.Text != "")
                    {
                        if (this.StaticRFID.Text.Length != int.Parse(this.RFIDDigts.Text))
                        {
                            player.Play();
                            this.Remind.AppendText("RFID位数错误\r\n");
                            this.StaticImei.Clear();
                            this.StaticRFID.Clear();
                            this.OnlyNetMark.Clear();
                            this.OnlyNetMark.Focus();
                            return;
                        }
                    }
                    if (this.RFIDStart.Text != "")
                    {
                        if (this.StaticRFID.Text.Substring(0, RfidPreDig) != this.RFIDStart.Text)
                        {
                            player.Play();
                            this.Remind.AppendText("RFID前缀错误\r\n");
                            this.StaticImei.Clear();
                            this.StaticRFID.Clear();
                            this.OnlyNetMark.Clear();
                            this.OnlyNetMark.Focus();
                            return;
                        }
                    }
                    //判断RFID是否重号
                    if (NMIB.CheckRFIDBLL(this.StaticRFID.Text) > 0)
                    {
                        player2.Play();
                        this.Remind.AppendText(this.StaticRFID.Text + "重号\r\n");
                        //this.RFIDPresent.Text = this.RFIDStart.Text + (long.Parse(this.StaticRFID.Text.Remove(0, RfidPreDig)) + 1).ToString().PadLeft(int.Parse(this.RFIDDigts.Text) - RfidPreDig, '0');
                        this.StaticImei.Clear();
                        this.StaticRFID.Clear();
                        this.OnlyNetMark.Clear();
                        this.OnlyNetMark.Focus();
                        return;
                    }
                    //打印机身贴
                    LabelFormatDocument btFormat = btEngine.Documents.Open(lj);
                    //指定打印机名称
                    btFormat.PrintSetup.PrinterName = this.Printer.Text;
                    //打印份数,同序列打印的份数
                    btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                    //对模板相应字段进行赋值
                    btFormat.SubStrings["IMEI"].Value = this.StaticImei.Text;
                    btFormat.SubStrings["RFID"].Value = this.StaticRFID.Text;
                    btFormat.SubStrings["ProductDate"].Value = this.PrintData.Text;
                    //判断关联表是否有重号
                    string RlsResult = DRSB.CheckRelativeSheetBLL(this.StaticImei.Text, this.StaticRFID.Text, this.OnlyNetMark.Text);
                    if (RlsResult != "")
                    {
                        player2.Play();
                        this.Remind.AppendText(RlsResult + "\r\n");
                        this.StaticImei.Clear();
                        this.StaticRFID.Clear();
                        this.OnlyNetMark.Clear();
                        this.OnlyNetMark.Focus();
                        return;
                    }
                    //记录到关联表
                    DRSB.InsertRelativeSheetBLL(this.StaticImei.Text,this.StaticRFID.Text, this.Zhidan_comboBox.Text, this.OnlyNetMark.Text);
                    //通过以上判断后可将数据记录到数据库
                    NMIB.InsertNetMarkBLL(this.Zhidan_comboBox.Text, this.StaticImei.Text, this.OnlyNetMark.Text, this.StaticRFID.Text, this.Template.Text);
                    btFormat.Print();
                    if (this.NoCheck.Checked == false)
                    {
                        this.ImeiPresent.Text = (long.Parse(this.StaticImei.Text.Substring(0, 14)) + 1).ToString();
                    }
                    else
                    {
                        string IMEISuf = this.StaticImei.Text.Remove(0, this.StaticImei.Text.Length - 5);
                        this.ImeiPresent.Text = IMEIPre + (long.Parse(IMEISuf) + 1).ToString().PadLeft(5, '0');
                    }
                    //this.RFIDPresent.Text = this.RFIDStart.Text + (long.Parse(this.StaticRFID.Text.Remove(0, RfidPreDig)) + 1).ToString().PadLeft(int.Parse(this.RFIDDigts.Text) - RfidPreDig, '0');
                    NMCB.UpdatePrensentIMEIAndRFIDBLL(this.Zhidan_comboBox.Text, this.ImeiPresent.Text, this.RFIDPresent.Text);
                    Log("打印了IMEI为" + this.StaticImei.Text + "的贴纸，网标为" + this.OnlyNetMark.Text, null);
                    this.StaticImei.Clear();
                    this.StaticRFID.Clear();
                    this.OnlyNetMark.Clear();
                    this.OnlyNetMark.Focus();
                }
                else
                {
                    //判断扫入的RFID是否在范围内
                    if (this.RFIDDigts.Text != "")
                    {
                        if (this.StaticRFID.Text.Length != int.Parse(this.RFIDDigts.Text))
                        {
                            player.Play();
                            this.Remind.AppendText("RFID位数错误\r\n");
                            this.ImeiInput.Clear();
                            this.StaticRFID.Clear();
                            this.NetLogoInput.Clear();
                            this.ImeiInput.Focus();
                            return;
                        }
                    }
                    if (this.RFIDStart.Text != "")
                    {
                        if (this.StaticRFID.Text.Substring(0, RfidPreDig) != this.RFIDStart.Text)
                        {
                            player.Play();
                            this.Remind.AppendText("RFID前缀错误\r\n");
                            this.ImeiInput.Clear();
                            this.StaticRFID.Clear();
                            this.NetLogoInput.Clear();
                            this.ImeiInput.Focus();
                            return;
                        }
                    }
                    //判断RFID是否重号
                    if (NMIB.CheckRFIDBLL(this.StaticRFID.Text) > 0)
                    {
                        player2.Play();
                        this.Remind.AppendText(this.StaticRFID.Text + "重号\r\n");
                        //this.RFIDPresent.Text = this.RFIDStart.Text + (long.Parse(this.StaticRFID.Text.Remove(0, RfidPreDig)) + 1).ToString().PadLeft(int.Parse(this.RFIDDigts.Text) - RfidPreDig, '0');
                        this.ImeiInput.Clear();
                        this.StaticRFID.Clear();
                        this.NetLogoInput.Clear();
                        this.ImeiInput.Focus();
                        return;
                    }
                    //打印机身贴
                    LabelFormatDocument btFormat = btEngine.Documents.Open(lj);
                    //指定打印机名称
                    btFormat.PrintSetup.PrinterName = this.Printer.Text;
                    //打印份数,同序列打印的份数
                    btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                    //对模板相应字段进行赋值
                    btFormat.SubStrings["IMEI"].Value = this.ImeiInput.Text;
                    btFormat.SubStrings["RFID"].Value = this.StaticRFID.Text;
                    btFormat.SubStrings["ProductDate"].Value = this.PrintData.Text;
                    //判断关联表是否有重号
                    string RlsResult = DRSB.CheckRelativeSheetBLL(this.ImeiInput.Text, this.StaticRFID.Text, this.NetLogoInput.Text);
                    if (RlsResult != "")
                    {
                        player2.Play();
                        this.Remind.AppendText(RlsResult + "\r\n");
                        this.ImeiInput.Clear();
                        this.StaticRFID.Clear();
                        this.NetLogoInput.Clear();
                        this.ImeiInput.Focus();
                        return;
                    }
                    //记录到关联表
                    DRSB.InsertRelativeSheetBLL(this.ImeiInput.Text,this.StaticRFID.Text, this.Zhidan_comboBox.Text, this.NetLogoInput.Text);
                    //通过以上判断后可将数据记录到数据库
                    NMIB.InsertNetMarkBLL(this.Zhidan_comboBox.Text, this.ImeiInput.Text, this.NetLogoInput.Text, this.StaticRFID.Text, this.Template.Text);
                    btFormat.Print();
                    if (this.NoCheck.Checked == false)
                    {
                        this.ImeiPresent.Text = (long.Parse(this.ImeiInput.Text.Substring(0, 14)) + 1).ToString();
                    }
                    else
                    {
                        IMEIPre = this.ImeiInput.Text.Substring(0, this.ImeiInput.Text.Length - 5);
                        string IMEISuf = this.ImeiInput.Text.Remove(0, this.ImeiInput.Text.Length - 5);
                        this.ImeiPresent.Text = IMEIPre + (long.Parse(IMEISuf) + 1).ToString().PadLeft(5, '0');
                    }
                    //this.RFIDPresent.Text = this.RFIDStart.Text + (long.Parse(this.StaticRFID.Text.Remove(0, RfidPreDig)) + 1).ToString().PadLeft(int.Parse(this.RFIDDigts.Text) - RfidPreDig, '0');
                    NMCB.UpdatePrensentIMEIAndRFIDBLL(this.Zhidan_comboBox.Text, this.ImeiPresent.Text, this.RFIDPresent.Text);
                    Log("打印了IMEI为" + this.ImeiInput.Text + "的贴纸，网标为" + this.NetLogoInput.Text, null);
                    this.ImeiInput.Clear();
                    this.StaticRFID.Clear();
                    this.NetLogoInput.Clear();
                    this.ImeiInput.Focus();
                }
            }
        }

        //点击扫描IMEI和网标复选框时触发的按钮
        private void InputAll_Click(object sender, EventArgs e)
        {
            if(this.InputAll.Checked == true)
            {
                if (this.CheckImeiByNetmark.Checked == true)
                {
                    this.CheckImeiByNetmark.Checked = false;
                }
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
                        this.Sen_remind.AppendText("您输入的IMEI删除起始位不在范围内\r\n");
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
                        this.Sen_remind.AppendText("您输入的IMEI删除起始位不在范围内\r\n");
                        this.IMEI_num1.Clear();
                        this.IMEI_num1.Focus();
                        return;
                    }
                }
            }
            else
            {
                player.Play();
                this.Sen_remind.AppendText("请输入正确的IMEI格式\r\n");
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
                this.Sen_remind.AppendText("请先选择制单号，IMEI范围不能为空\r\n");
                this.IMEI_num1.Clear();
                this.IMEI_num2.Clear();
                this.IMEI_num1.Focus();
                return;
            }
            if (this.IMEI_num1.Text=="" || this.IMEI_num2.Text == "")
            {
                this.Sen_remind.AppendText("删除起始位和终止位都不能为空\r\n");
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
                    this.Sen_remind.AppendText("您输入的IMEI删除终止位不在范围内\r\n");
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
                    this.Sen_remind.AppendText("您输入的IMEI删除终止位不在范围内\r\n");
                    this.IMEI_num2.Clear();
                    this.IMEI_num2.Focus();
                    return;
                }
            }

            //判断输入的终止位有没有大于起始位
            if(long.Parse(IMEI_num2.Text) < long.Parse(IMEI_num1.Text))
            {
                player.Play();
                this.Sen_remind.AppendText("IMEI删除终止位不能小于删除起始位\r\n");
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
                        this.Sen_remind.AppendText("删除成功\r\n");
                    }
                    else
                    {
                        this.Sen_remind.AppendText("找不到" + Num1Imei14+ Num1Imei15 + "的记录\r\n");
                    }
                }
                this.Sen_remind.AppendText("删除完成\r\n");
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
                        this.Sen_remind.AppendText("删除成功\r\n");
                    }
                    else
                    {
                        this.Sen_remind.AppendText("找不到" + Num1Imei + "的记录\r\n");
                    }
                }
                this.Sen_remind.AppendText("删除完成\r\n");
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
                CheckAll_Click(sender, e);
            }
        }

        //点击查询按钮
        private void CheckAll_Click(object sender, EventArgs e)
        {
            //空值按下Enter无反应
            if (this.Tocheck.Text == "")
            {
                return;
            }
            NMIM = NMIB.SelectResultByImeiOrNetmarkBLL(this.Tocheck.Text);
            if (NMIM.Count == 0)
            {
                this.Sen_remind.AppendText("找不到" + this.Tocheck.Text + "的对应关系\r\n");
                this.dataGridView1.Rows.Clear();
                this.Tocheck.Clear();
                this.Tocheck.Focus();
            }
            else
            {
                this.dataGridView1.Rows.Clear();
                foreach (NetMarkIMEIMes a in NMIM)
                {
                    this.dataGridView1.Rows.Add(a.NetMark, a.IMEI, a.RFID, a.zhidan, a.JS_PrintTime, a.JS_Template, a.RePrintNum, a.RePrintFirstTime, a.RePrintEndTime);
                }
            }
        }

        //数据库配置
        private void SqlConfig_Click(object sender, EventArgs e)
        {
            UpdateSqlConn US = new UpdateSqlConn(this);
            US.ShowDialog();
        }

        //点击选择制单下拉框时
        private void Zhidan_comboBox_DropDown(object sender, EventArgs e)
        {
            G_MOP = MOPB.SelectZhidanNumBLL();
            foreach (Gps_ManuOrderParam a in G_MOP)
            {
                if (!this.Zhidan_comboBox.Items.Contains(a.ZhiDan))
                {
                    this.Zhidan_comboBox.Items.Add(a.ZhiDan);
                }
            }
        }

        //制单下拉框关闭时发生
        private void Zhidan_comboBox_DropDownClosed(object sender, EventArgs e)
        {
          // this.Zhidan_comboBox.Items.Clear();
        }

        //选择模板
        private void ChooseTemplate_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "文本文件|*.btw";
            ofd.ShowDialog();
            string path = ofd.FileName;
            this.Template.Text = path;
            lj = path;
        }

        //刷新模板
        private void Refresh_template_Click(object sender, EventArgs e)
        {
            if (this.Template.Text != "")
            {
                string path = this.Template.Text;
                string filename = Path.GetFileName(path);
                if (!Directory.Exists(AppDomain.CurrentDomain.BaseDirectory + "机身贴模板"))
                {
                    Directory.CreateDirectory(AppDomain.CurrentDomain.BaseDirectory + "\\机身贴模板");
                }
                if (!Directory.Exists(AppDomain.CurrentDomain.BaseDirectory + "机身贴模板\\" + RefreshNum))
                {
                    Directory.CreateDirectory(AppDomain.CurrentDomain.BaseDirectory + "\\机身贴模板\\" + RefreshNum);
                }
                File.Copy(path, AppDomain.CurrentDomain.BaseDirectory + "\\机身贴模板\\" + RefreshNum + "\\" + filename, true);
                lj = AppDomain.CurrentDomain.BaseDirectory + "\\机身贴模板\\" + RefreshNum + "\\" + filename;
                this.Remind.AppendText("刷新模板成功\r\n");
                RefreshNum++;
            }
            else
            {
                player.Play();
                this.Remind.AppendText("请先选择模板\r\n");
            }
        }

        //打开模板
        private void Open_Template_Click(object sender, EventArgs e)
        {
            if (this.Template.Text != "")
            {
                string path = this.Template.Text;
                if (File.Exists(path))
                {
                    System.Diagnostics.Process.Start(path);
                }
                else
                {
                    player.Play();
                    this.Remind.AppendText("该路径下找不到此模板\r\n");
                }
            }
        }

        //输入模板打印份数后焦点离开
        private void TemplateNum_Leave(object sender, EventArgs e)
        {
            if (this.TemplateNum.Text == "")
            {
                this.TemplateNum.Text = 1.ToString();
            }
            else
            {
                if (IsNumeric(this.TemplateNum.Text))
                {
                    TN = int.Parse(this.TemplateNum.Text);
                }
                else
                {
                    this.Remind.AppendText("请输入数字\r\n");
                    this.TemplateNum.Clear();
                    this.TemplateNum.Focus();
                }
            }
        }

        //将tabControl子页面的按钮文字横着显示
        private void TabControl_DrawItem(object sender, DrawItemEventArgs e)
        {
            Rectangle tabArea = TabControl.GetTabRect(e.Index);//主要是做个转换来获得TAB项的RECTANGELF 
            RectangleF tabTextArea = (RectangleF)(TabControl.GetTabRect(e.Index));
            Graphics g = e.Graphics;
            StringFormat sf = new StringFormat();//封装文本布局信息 
            sf.LineAlignment = StringAlignment.Center;
            sf.Alignment = StringAlignment.Center;
            Font font = new Font("微软雅黑", 10F);
            SolidBrush brush = new SolidBrush(Color.Black);//绘制边框的画笔 
            SolidBrush tabPageBlack = new SolidBrush(Color.Linen);//Tab选项卡背景颜色
            SolidBrush backgroundBlack = new SolidBrush(Color.Linen);//Tab整体背景颜色
            //填充背景色
            Rectangle rec3 = TabControl.ClientRectangle;
            e.Graphics.FillRectangle(backgroundBlack, rec3);
            //填充选项卡颜色
            Rectangle Rec0 = TabControl.GetTabRect(0);
            e.Graphics.FillRectangle(tabPageBlack, Rec0);
            Rectangle Rec1 = TabControl.GetTabRect(1);
            e.Graphics.FillRectangle(tabPageBlack, Rec1);
            //Rectangle Rec2 = TabControl.GetTabRect(2);
            //e.Graphics.FillRectangle(tabPageBlack, Rec2);
            //g.DrawString(((TabControl)(sender)).TabPages[e.Index].Text, font, brush, tabTextArea, sf);
            g.DrawString("扫描主页面", font, brush, Rec0, sf);
            g.DrawString("查询与删除", font, brush, Rec1, sf);
            //g.DrawString("重打", font, brush, Rec2, sf);
        }

        //重打区
        private void ReImei_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                if (this.ReImei.Text == "")
                {
                    this.ReImei.Focus();
                    return;
                }
                //扫入RFID或者IMEI都可以
                this.ReImei.Text = NMIB.SelectIMEIBLL(this.ReImei.Text);
                //判断重打的号码在不在字段范围内
                if (this.ReImei.Text.Length == 15 && this.IMEI_Start.Text.Length == 14)
                {
                    if (long.Parse(this.ReImei.Text.Substring(0, 14)) < long.Parse(this.IMEI_Start.Text) || long.Parse(this.ReImei.Text.Substring(0, 14)) > long.Parse(this.IMEI_End.Text))
                    {
                        player.Play();
                        this.Remind.AppendText(this.ReImei.Text + "IMEI不在范围内\r\n");
                        this.ReImei.Clear();
                        this.ReImei.Focus();
                        return;
                    }
                }
                else
                {
                    if (this.ReImei.Text.Length != this.IMEI_Start.Text.Length)
                    {
                        player.Play();
                        this.Remind.AppendText(this.ReImei.Text + "IMEI不在范围内\r\n");
                        this.ReImei.Clear();
                        this.ReImei.Focus();
                        return;
                    }
                    if (this.ReImei.Text.CompareTo(this.IMEI_Start.Text) == -1 || this.ReImei.Text.CompareTo(this.IMEI_End.Text) == 1)
                    {
                        player.Play();
                        this.Remind.AppendText(this.ReImei.Text + "IMEI不在范围内\r\n");
                        this.ReImei.Clear();
                        this.ReImei.Focus();
                        return;
                    }
                }
                NMIM = NMIB.SelectIMEIByNetLogoOrImerBLL(this.ReImei.Text);
                if (NMIM.Count <= 0)
                {
                    player.Play();
                    this.Remind.AppendText("无此数据，无法重打\r\n");
                    this.ReImei.Clear();
                    this.ReImei.Focus();
                    return;
                }
                //打印机身贴
                LabelFormatDocument btFormat = btEngine.Documents.Open(lj);
                //指定打印机名称
                btFormat.PrintSetup.PrinterName = this.Printer.Text;
                //打印份数,同序列打印的份数
                btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                //对模板相应字段进行赋值
                btFormat.SubStrings["IMEI"].Value = this.ReImei.Text;
                btFormat.SubStrings["ProductDate"].Value = this.PrintData.Text;
                foreach (NetMarkIMEIMes a in NMIM)
                {
                    btFormat.SubStrings["RFID"].Value = a.RFID;
                }
                btFormat.Print();
                //重打印后记录数据到数据表
                NMIB.UpdateRePrintBLL(this.ReImei.Text);
                Log("重打了" + this.ReImei.Text, null);
                this.ReImei.Clear();
                this.ReImei.Focus();
            }
        }

        private void CheckImeiByNetmark_Click(object sender, EventArgs e)
        {
            if (this.CheckImeiByNetmark.Checked == true)
            {
                if (this.InputAll.Checked == true)
                {
                    this.InputAll.Checked = false;
                    InputAll_Click(sender, e);
                }
            }
        }
        
    }
}
