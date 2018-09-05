﻿using System;
using System.IO;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Drawing;
using System.Linq;
using System.Text;
using Seagull.BarTender.Print;
using System.Windows.Forms;
using Print_Message;
using Print.Message.Bll;
using ManuOrder.Param.BLL;
using System.Runtime.InteropServices;
using System.Drawing.Printing;
using System.Text.RegularExpressions;
using System.Media;
using System.Threading;

namespace WindowsForms_print
{
    public partial class Form1 : Form
    {

        public static void Log(string msg, Exception e)
        {
            try
            {
                string path = AppDomain.CurrentDomain.BaseDirectory + "print.log";
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
                string path = AppDomain.CurrentDomain.BaseDirectory + "print.log";
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

        string outString;

        SoundPlayer player = new SoundPlayer(AppDomain.CurrentDomain.BaseDirectory + "失败.wav");
        SoundPlayer player1 = new SoundPlayer(AppDomain.CurrentDomain.BaseDirectory + "请先选择模板.wav");
        SoundPlayer player2 = new SoundPlayer(AppDomain.CurrentDomain.BaseDirectory + "请选择制单号.wav");
        SoundPlayer player3 = new SoundPlayer(AppDomain.CurrentDomain.BaseDirectory + "校验错误.wav");

        ManuOrderParamBLL MOPB = new ManuOrderParamBLL();

        PrintMessageBLL PMB = new PrintMessageBLL();

        InputExcelBLL IEB = new InputExcelBLL();

        List<Gps_ManuOrderParam> G_MOP = new List<Gps_ManuOrderParam>();

        List<PrintMessage> list = new List<PrintMessage>();

        //用于记录打印复选框的选择：c1为客供SN，c2为不打印校验码，c3为不打印SN号
        int c1, c2, c3;

        //记录打印模板路径
        string lj = "";
        Messages messages;
        int waitout = 10000;

        //记录模板打印份数
        int TN = 1;

        //记录SN号后缀位数
        int s;

        //记录打印时间
        string ProductTime = "";
        Engine btEngine = new Engine();
        public Form1()
        {
                InitializeComponent();
                this.WindowState = FormWindowState.Maximized;
                int wid = Screen.PrimaryScreen.WorkingArea.Width;
                int hei = Screen.PrimaryScreen.WorkingArea.Height;
                this.Height = hei;
                this.tabControl1.Width = wid;
                this.tabPage2.Width = wid;
        }
        [DllImport("kernel32.dll")]
        private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder returnvalue, int buffersize, string filepath);

        private string IniFilePath;
        private void GetValue(string section, string key, out string value)
        {
            IniFilePath = AppDomain.CurrentDomain.BaseDirectory + "PrintVariable.ini";
            StringBuilder stringBuilder = new StringBuilder();
            GetPrivateProfileString(section, key, "", stringBuilder, 1024, IniFilePath);
            value = stringBuilder.ToString();
        }     

        private void Form1_Load(object sender, EventArgs e)
        {
            PrintDocument print = new PrintDocument();
            string sDefault = print.PrinterSettings.PrinterName;//默认打印机名
            this.Printer1.Text = sDefault;
            foreach (string sPrint in PrinterSettings.InstalledPrinters)//获取所有打印机名称
            {
                Printer1.Items.Add(sPrint);
            }
            G_MOP = MOPB.SelectZhidanNumBLL();
            foreach (Gps_ManuOrderParam a in G_MOP)
            {
                this.CB_ZhiDan.Items.Add(a.ZhiDan);
            }
            string NowData = System.DateTime.Now.ToString("yyyy.MM.dd");
            this.ProductData.Text = NowData;

            //开启打印机引擎
            btEngine.Start();
        }

        private void CB_ZhiDan_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.reminder.Text = "";
            this.IMEI_Start.Clear();
            this.PrintNum.Clear();
            this.ProductData.Clear();
            this.Re_IMEINum.Clear();
            this.ReImeiNum1.Clear();
            this.ReImeiNum2.Clear();
            this.SnFromCustomer.Checked = false;
            this.NoCheckCode.Checked = false;
            this.NoSn.Checked = false;
            this.Re_Nocheckcode.Checked = false;
            this.RePrintOne.Checked = false;
            this.RePrintMore.Checked = false;
            this.Re_IMEINum.ReadOnly = true;
            this.ReImeiNum1.ReadOnly = true;
            this.ReImeiNum2.ReadOnly = true;
            string NowData = System.DateTime.Now.ToString("yyyy.MM.dd");
            this.ProductData.Text = NowData;
            string ZhidanNum = this.CB_ZhiDan.Text;
            G_MOP = MOPB.selectManuOrderParamByzhidanBLL(ZhidanNum);
            foreach (Gps_ManuOrderParam b in G_MOP)
            {
                s = b.SN2.Length;
                this.SoftModel.Text = b.SoftModel;
                this.SN1_num.Text = b.SN1 + b.SN2;
                this.SN2_num.Text = b.SN1 + b.SN3;
                this.IMEI_Present.Text = b.IMEIPrints;
                this.ProductNo.Text = b.ProductNo;
                this.SoftwareVersion.Text = b.Version;
                this.IMEI_num1.Text = b.IMEIStart;
                this.IMEI_num2.Text = b.IMEIEnd;
                this.SIM_num1.Text = b.SIMStart;
                this.SIM_num2.Text = b.SIMEnd;
                this.BAT_num1.Text = b.BATStart;
                this.BAT_num2.Text = b.BATEnd;
                this.VIP_num1.Text = b.VIPStart;
                this.VIP_num2.Text = b.VIPEnd;
                this.IMEI_Present.Text = b.IMEIPrints;
                this.Select_Template1.Text = b.JST_template;
                if (b.Remark1 != "")
                {
                    string rem = b.Remark1;
                    string[] remark = rem.Split('：');
                    this.Remake.Text = remark[1];
                }
                else
                {
                    this.Remake.Text = b.Remark1;
                }
                if (int.Parse(b.IMEIRel) == 0)
                {
                    this.IMEIRel.Text = "无绑定";
                }
                else if (int.Parse(b.IMEIRel) == 1)
                {
                    this.IMEIRel.Text = "与SIM卡绑定";
                }
                else if (int.Parse(b.IMEIRel) == 2)
                {
                    this.IMEIRel.Text = "与SIM&BAT绑定";
                }
                else if (int.Parse(b.IMEIRel) == 3)
                {
                    this.IMEIRel.Text = "与SIM&VIP绑定";
                }
                else if (int.Parse(b.IMEIRel) == 4)
                {
                    this.IMEIRel.Text = "与BAT绑定";
                }
            }
            string PresentImei = PMB.SelectPresentImeiByZhidanBLL(this.CB_ZhiDan.Text);
            if (PresentImei.Length == 15)
            {
                string PresentImei1 = PresentImei.Substring(0, 14);
                this.IMEI_Present.Text = (long.Parse(PresentImei1)+1).ToString();
            }
            else {
                this.IMEI_Present.Text = PresentImei;
            }

            string Presentsn = PMB.SelectPresentSnByZhidanBLL(this.CB_ZhiDan.Text);
            if (Presentsn != "")
            {
                char[] a = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
                char[] b = { '0','1','2','3','4', '5', '6', '7', '8', '9'};
                string sn2_aft = Presentsn.TrimStart(a);
                string sn1_pre = Presentsn.TrimEnd(b);
                this.SN1_num.Text = sn1_pre + (int.Parse(sn2_aft) + 1).ToString().PadLeft(sn2_aft.Length,'0');
            }
        }
        
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

        public string getimei15(string imei) {
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

        private void ProductData_MouseLeave(object sender, EventArgs e)
        {
            if (this.ProductData.Text != "")
            {
                if (!IsDate(this.ProductData.Text))
                {
                    player.Play();
                    this.ProductData.Text = System.DateTime.Now.ToString("yyyy-MM-dd");
                }
            }
        }

        private void ProductData_Leave(object sender, EventArgs e)
        {
            if (this.ProductData.Text != "")
            {
                if (!IsDate(this.ProductData.Text))
                {
                    player.Play();
                    this.ProductData.Text = System.DateTime.Now.ToString("yyyy.MM.dd");
                }
            }
        }

        //判断是否有中文字符
        public static bool HasChinese(string str)
        {
            return Regex.IsMatch(str, @"[\u4e00-\u9fa5]");
        }

        private void Open_Template1_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.ShowDialog();
            string path = ofd.FileName;
            string strExtension = "";
            int nIndex = path.LastIndexOf('.');
            if (nIndex > 0) {
                strExtension = path.Substring(nIndex);
                if (strExtension != ".btw")
                {
                    player.Play();
                    this.reminder.AppendText("请选择btw文件\r\n");
                }
                else {
                    this.Select_Template1.Text = path;
                }
            }

        }

        private void tabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {
            tabControl1.SelectedTab.Refresh();
            if (tabControl1.SelectedTab == tabPage2)
            {
                Color_Box CB = new Color_Box();
                CB.TopLevel = false;
                tabPage2.Controls.Add(CB);
                CB.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
                CB.Show();
            }
            else if (tabControl1.SelectedTab == tabPage3) {
                CheckRePrint CRP = new CheckRePrint();
                CRP.TopLevel = false;
                tabPage3.Controls.Add(CRP);
                CRP.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
                CRP.Show();
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (MessageBox.Show("是否退出系统？", "系统提示", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.Cancel)
            {
                e.Cancel = true;
            }
            //结束打印引擎
            btEngine.Stop();
        }

        private void tabControl2_DrawItem(object sender, DrawItemEventArgs e)
        {
            Rectangle tabArea = tabControl2.GetTabRect(e.Index);//主要是做个转换来获得TAB项的RECTANGELF 
            RectangleF tabTextArea = (RectangleF)(tabControl2.GetTabRect(e.Index));
            Graphics g = e.Graphics;
            StringFormat sf = new StringFormat();//封装文本布局信息 
            sf.LineAlignment = StringAlignment.Center;
            sf.Alignment = StringAlignment.Center;
            Font font = this.tabControl2.Font;
            SolidBrush brush = new SolidBrush(Color.Black);//绘制边框的画笔 
            g.DrawString(((TabControl)(sender)).TabPages[e.Index].Text, font, brush, tabTextArea, sf);
        }

        private void tabControl2_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (tabControl2.SelectedTab == CheckAndDelete)
            {
                JST_CheckAndDelect JSTCAD = new JST_CheckAndDelect();
                JSTCAD.TopLevel = false;
                CheckAndDelete.Controls.Add(JSTCAD);
                JSTCAD.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
                JSTCAD.Show();
            }
            else if (tabControl2.SelectedTab == ExcelToPrint)
            {
                PrintFromExcel PFE = new PrintFromExcel();
                PFE.TopLevel = false;
                ExcelToPrint.Controls.Add(PFE);
                PFE.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
                PFE.Show();
                //MessageBox.Show("该功能暂建设中...");
            }
        }

        private void Debug_print_Click(object sender, EventArgs e)
        {
            try
            {
                if (this.Select_Template1.Text != "")
                {
                    lj = this.Select_Template1.Text;
                    LabelFormatDocument btFormat = btEngine.Documents.Open(lj);
                    //指定打印机名称
                    btFormat.PrintSetup.PrinterName = this.Printer1.Text;
                    string imei15 = getimei15(this.IMEI_num1.Text);
                    //对模板相应字段进行赋值
                    GetValue("Information", "IMEI", out outString);
                    btFormat.SubStrings[outString].Value = this.IMEI_num1.Text + imei15;
                    GetValue("Information", "SN", out outString);
                    btFormat.SubStrings[outString].Value = this.SN1_num.Text;
                    GetValue("Information", "型号", out outString);
                    btFormat.SubStrings[outString].Value = this.SoftModel.Text;
                    GetValue("Information", "产品编码", out outString);
                    btFormat.SubStrings[outString].Value = this.ProductNo.Text;
                    GetValue("Information", "软件版本", out outString);
                    btFormat.SubStrings[outString].Value = this.SoftwareVersion.Text;
                    GetValue("Information", "SIM卡号", out outString);
                    btFormat.SubStrings[outString].Value = this.SIM_num1.Text;
                    GetValue("Information", "服务卡号", out outString);
                    btFormat.SubStrings[outString].Value = this.VIP_num1.Text;
                    GetValue("Information", "备注", out outString);
                    btFormat.SubStrings[outString].Value = this.Remake.Text;
                    GetValue("Information", "生产日期", out outString);
                    btFormat.SubStrings[outString].Value = this.ProductData.Text;
                    //打印份数,同序列打印的份数
                    btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                    Messages messages;
                    int waitout = 10000;
                    btFormat.Print("标签打印软件", waitout, out messages);
                    Form1.Log("调试打印了机身贴IMEI号为" + this.IMEI_num1.Text + "的制单", null);
                }
                else
                {
                    player1.Play();
                    this.reminder.AppendText("请先选择模板\r\n");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Exception:" + ex.Message);
            }
        }

        private void PrintOne_Click(object sender, EventArgs e)
        {
            if (this.PrintOne.Checked == true)
            {
                this.PrintMore.Checked = false;
                this.IMEI_Start.ReadOnly = false;
                this.PrintNum.ReadOnly = true;
                this.PrintNum.Clear();
                this.IMEI_Start.Focus();
            }
            else
            {
                this.PrintOne.Checked = true;
                this.IMEI_Start.Focus();
            }
        }

        private void PrintMore_Click(object sender, EventArgs e)
        {
            if (this.PrintMore.Checked == true)
            {
                this.PrintOne.Checked = false;
                this.PrintNum.ReadOnly = false;
                this.IMEI_Start.ReadOnly = true;
                this.IMEI_Start.Clear();
                this.PrintNum.Focus();
            }
            else
            {
                this.PrintMore.Checked = true;
                this.PrintNum.Focus();
            }
        }

        private void RePrintOne_Click(object sender, EventArgs e)
        {
            if (this.RePrintOne.Checked == true)
            {
                this.RePrintMore.Checked = false;
                this.Re_IMEINum.ReadOnly = false;
                this.ReImeiNum1.ReadOnly = true;
                this.ReImeiNum2.ReadOnly = true;
                this.ReImeiNum1.Clear();
                this.ReImeiNum2.Clear();
                this.Re_IMEINum.Focus();
            }
            else
            {
                this.Re_IMEINum.ReadOnly = true;
            }
        }

        private void RePrintMore_Click(object sender, EventArgs e)
        {
            if (this.RePrintMore.Checked == true)
            {
                this.RePrintOne.Checked = false;
                this.Re_IMEINum.ReadOnly = true;
                this.ReImeiNum1.ReadOnly = false;
                this.ReImeiNum2.ReadOnly = false;
                this.Re_IMEINum.Clear();
                this.ReImeiNum1.Focus();
            }
            else
            {
                this.ReImeiNum1.ReadOnly = true;
                this.ReImeiNum2.ReadOnly = true;
            }
        }

        private void PrintNum_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13) {
                if (this.CB_ZhiDan.Text != "")
                {
                    if (this.PrintNum.Text != "" && IsNumeric(this.PrintNum.Text))
                    {
                        long between = long.Parse(this.IMEI_num2.Text) - long.Parse(this.IMEI_num1.Text);
                        if (int.Parse(this.PrintNum.Text) < 0)
                        {
                            player.Play();
                            this.reminder.AppendText(this.PrintNum.Text + "超出范围\r\n");
                            this.PrintNum.Clear();
                            this.PrintNum.Focus();
                            return;
                        }
                        else if (int.Parse(this.PrintNum.Text) > between)
                        {
                            player.Play();
                            this.reminder.AppendText(this.PrintNum.Text + "超出范围\r\n");
                            this.PrintNum.Clear();
                            this.PrintNum.Focus();
                            return;
                        }
                    }
                    else if (this.PrintNum.Text == "")
                    {
                        player.Play();
                        this.reminder.AppendText("请输入数字\r\n");
                        return;
                    }
                    else
                    {
                        player.Play();
                        this.reminder.AppendText("请输入数字\r\n");
                        this.PrintNum.Clear();
                        this.PrintNum.Focus();
                        return;
                    }
                }
                else
                {
                    player2.Play();
                    this.reminder.AppendText("请先选择制单\r\n");
                    this.PrintNum.Clear();
                    this.PrintNum.Focus();
                    return;
                }
                try
                {
                    if (this.Select_Template1.Text != "")
                    {
                        lj = this.Select_Template1.Text;
                        LabelFormatDocument btFormat = btEngine.Documents.Open(lj);
                        //对模板相应字段进行赋值
                        ValueToTemplate(btFormat);
                        //指定打印机名称
                        btFormat.PrintSetup.PrinterName = this.Printer1.Text;
                        //打印份数,同序列打印的份数
                        btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                        switch (c1 + c2 + c3)
                        {
                            case 4:
                                {
                                    long imei_begin;
                                    string imei15, sn_aft;
                                    if (this.IMEI_Present.Text != "")
                                    {
                                        imei_begin = long.Parse(this.IMEI_Present.Text);
                                    }
                                    else
                                    {
                                        imei_begin = long.Parse(this.IMEI_num1.Text);
                                    }
                                    sn_aft = SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s);
                                    imei15 = getimei15(imei_begin.ToString());
                                    string EndIMEI = (imei_begin + int.Parse(this.PrintNum.Text)).ToString() + getimei15((imei_begin + int.Parse(this.PrintNum.Text)).ToString());
                                    list = PMB.CheckRangeIMEIBLL(imei_begin.ToString() + imei15, EndIMEI);
                                    if (list.Count > 0)
                                    {
                                        foreach (PrintMessage a in list)
                                        {
                                            this.reminder.AppendText(a.IMEI.Substring(0, 14) + "重号\r\n");
                                        }
                                        this.PrintNum.Clear();
                                        this.PrintNum.Focus();
                                        return;
                                    }
                                    for (int i = 0; i < int.Parse(this.PrintNum.Text); i++)
                                    {
                                        imei15 = getimei15(imei_begin.ToString());
                                        btFormat.SubStrings["IMEI"].Value = imei_begin.ToString() + imei15;
                                        //记录打印信息日志
                                        ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                        list.Add(new PrintMessage()
                                        {
                                            Zhidan = this.CB_ZhiDan.Text.Trim(),
                                            IMEI = imei_begin.ToString() + imei15,
                                            IMEIStart = this.IMEI_num1.Text.Trim(),
                                            IMEIEnd = this.IMEI_num2.Text.Trim(),
                                            SN = "",
                                            IMEIRel = this.IMEIRel.Text.Trim(),
                                            SIM = "",
                                            VIP = "",
                                            BAT = "",
                                            SoftModel = this.SoftModel.Text.Trim(),
                                            Version = this.SoftwareVersion.Text.Trim(),
                                            Remark = this.Remake.Text.Trim(),
                                            JS_PrintTime = ProductTime,
                                            JS_TemplatePath = this.Select_Template1.Text.Trim(),
                                            CH_PrintTime = "",
                                            CH_TemplatePath1 = null,
                                            CH_TemplatePath2 = null,
                                            ICCID = "",
                                            MAC = "",
                                            Equipment = ""
                                        });
                                        if (PMB.InsertPrintMessageBLL(list))
                                        {
                                            Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                            Form1.Log("批量打印了IMEI号为" + imei_begin + imei15 + "的制单", null);
                                            imei_begin++;
                                        }
                                    }
                                    if (MOPB.UpdateSNnumberBLL(this.CB_ZhiDan.Text, sn_aft, imei_begin))
                                    {
                                        this.IMEI_Present.Text = imei_begin.ToString();
                                        this.PrintNum.Clear();
                                        this.PrintNum.Focus();
                                    }
                                }
                                break;
                            case 0:
                                {
                                    long imei_begin;
                                    string imei15, sn_bef, sn_aft, sn_laf;
                                    if (this.IMEI_Present.Text != "")
                                    {
                                        imei_begin = long.Parse(this.IMEI_Present.Text);
                                    }
                                    else
                                    {
                                        imei_begin = long.Parse(this.IMEI_num1.Text);
                                    }
                                    imei15 = getimei15(imei_begin.ToString());
                                    string EndIMEI = (imei_begin + int.Parse(this.PrintNum.Text)).ToString() + getimei15((imei_begin + int.Parse(this.PrintNum.Text)).ToString());
                                    list = PMB.CheckRangeIMEIBLL(imei_begin.ToString() + imei15, EndIMEI);
                                    if (list.Count > 0)
                                    {
                                        foreach (PrintMessage a in list)
                                        {
                                            this.reminder.AppendText(a.IMEI.Substring(0, 14) + "重号\r\n");
                                        }
                                        this.PrintNum.Clear();
                                        this.PrintNum.Focus();
                                        return;
                                    }
                                    if (this.SN1_num.Text != "")
                                    {
                                        sn_bef = this.SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                        sn_aft = this.SN1_num.Text.Remove(0, this.SN1_num.Text.Length - s);
                                        sn_laf = this.SN2_num.Text.Remove(0, this.SN2_num.Text.Length - s);
                                        for (int i = 0; i < int.Parse(this.PrintNum.Text); i++)
                                        {
                                            if (int.Parse(sn_aft) < int.Parse(sn_laf))
                                            {
                                                imei15 = getimei15(imei_begin.ToString());
                                                btFormat.SubStrings["IMEI"].Value = imei_begin.ToString() + imei15;
                                                btFormat.SubStrings["SN"].Value = sn_bef + sn_aft;
                                                //记录打印信息日志
                                                ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                                list.Add(new PrintMessage()
                                                {
                                                    Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                    IMEI = imei_begin.ToString() + imei15,
                                                    IMEIStart = this.IMEI_num1.Text.Trim(),
                                                    IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                    SN = sn_bef + sn_aft,
                                                    IMEIRel = this.IMEIRel.Text.Trim(),
                                                    SIM = "",
                                                    VIP = "",
                                                    BAT = "",
                                                    SoftModel = this.SoftModel.Text.Trim(),
                                                    Version = this.SoftwareVersion.Text.Trim(),
                                                    Remark = this.Remake.Text.Trim(),
                                                    JS_PrintTime = ProductTime,
                                                    JS_TemplatePath = this.Select_Template1.Text.Trim(),
                                                    CH_PrintTime = "",
                                                    CH_TemplatePath1 = null,
                                                    CH_TemplatePath2 = null,
                                                    ICCID = "",
                                                    MAC = "",
                                                    Equipment = ""
                                                });
                                                if (PMB.InsertPrintMessageBLL(list))
                                                {
                                                    Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                                    Form1.Log("批量打印了IMEI号为" + imei_begin + imei15 + "的制单", null);
                                                    imei_begin++;
                                                    sn_aft = (int.Parse(sn_aft) + 1).ToString().PadLeft(s, '0');
                                                }
                                            }
                                            else
                                            {
                                                player.Play();
                                                this.reminder.AppendText("SN号不足");
                                                return;
                                            }
                                        }
                                        if (MOPB.UpdateSNnumberBLL(this.CB_ZhiDan.Text, sn_aft, imei_begin))
                                        {
                                            this.SN1_num.Text = sn_bef + sn_aft;
                                            this.IMEI_Present.Text = imei_begin.ToString();
                                            this.PrintNum.Clear();
                                            this.PrintNum.Focus();
                                        }
                                    }
                                    else
                                    {
                                        for (int i = 0; i < int.Parse(this.PrintNum.Text); i++)
                                        {
                                            imei15 = getimei15(imei_begin.ToString());
                                            btFormat.SubStrings["IMEI"].Value = imei_begin.ToString() + imei15;
                                            btFormat.SubStrings["SN"].Value = "";
                                            //记录打印信息日志
                                            ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                            list.Add(new PrintMessage()
                                            {
                                                Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                IMEI = imei_begin.ToString() + imei15,
                                                IMEIStart = this.IMEI_num1.Text.Trim(),
                                                IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                SN = "",
                                                IMEIRel = this.IMEIRel.Text.Trim(),
                                                SIM = "",
                                                VIP = "",
                                                BAT = "",
                                                SoftModel = this.SoftModel.Text.Trim(),
                                                Version = this.SoftwareVersion.Text.Trim(),
                                                Remark = this.Remake.Text.Trim(),
                                                JS_PrintTime = ProductTime,
                                                JS_TemplatePath = this.Select_Template1.Text.Trim(),
                                                CH_PrintTime = "",
                                                CH_TemplatePath1 = null,
                                                CH_TemplatePath2 = null
                                            });
                                            if (PMB.InsertPrintMessageBLL(list))
                                            {
                                                Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                                Form1.Log("批量打印了IMEI号为" + imei_begin + imei15 + "的制单", null);
                                                imei_begin++;
                                            }
                                        }
                                        if (MOPB.UpdateSNnumberBLL(this.CB_ZhiDan.Text, "", imei_begin))
                                        {
                                            this.IMEI_Present.Text = imei_begin.ToString();
                                            this.PrintNum.Clear();
                                            this.PrintNum.Focus();
                                        }
                                    }
                                }
                                break;
                            case 1:
                                {
                                    long imei_begin;
                                    string imei15, sn_bef, sn_aft, sn_laf;
                                    if (this.IMEI_Present.Text != "")
                                    {
                                        imei_begin = long.Parse(this.IMEI_Present.Text);
                                    }
                                    else
                                    {
                                        imei_begin = long.Parse(this.IMEI_num1.Text);
                                    }
                                    imei15 = getimei15(imei_begin.ToString());
                                    string EndIMEI = (imei_begin + int.Parse(this.PrintNum.Text)).ToString() + getimei15((imei_begin + int.Parse(this.PrintNum.Text)).ToString());
                                    list = PMB.CheckRangeIMEIBLL(imei_begin.ToString() + imei15, EndIMEI);
                                    if (list.Count > 0)
                                    {
                                        foreach (PrintMessage a in list)
                                        {
                                            this.reminder.AppendText(a.IMEI.Substring(0, 14) + "重号\r\n");
                                        }
                                        this.PrintNum.Clear();
                                        this.PrintNum.Focus();
                                        return;
                                    }
                                    if (this.SN1_num.Text != "")
                                    {
                                        sn_bef = this.SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                        sn_aft = this.SN1_num.Text.Remove(0, this.SN1_num.Text.Length - s);
                                        sn_laf = this.SN2_num.Text.Remove(0, this.SN2_num.Text.Length - s);
                                        for (int i = 0; i < int.Parse(this.PrintNum.Text); i++)
                                        {
                                            if (int.Parse(sn_aft) < int.Parse(sn_laf))
                                            {
                                                imei15 = getimei15(imei_begin.ToString());
                                                btFormat.SubStrings["IMEI"].Value = imei_begin.ToString() + imei15;
                                                if (!PMB.CheckSNBLL(sn_bef + sn_aft))
                                                {
                                                    btFormat.SubStrings["SN"].Value = sn_bef + sn_aft;
                                                    //记录打印信息日志
                                                    ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                                    list.Add(new PrintMessage()
                                                    {
                                                        Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                        IMEI = imei_begin.ToString() + imei15,
                                                        IMEIStart = this.IMEI_num1.Text.Trim(),
                                                        IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                        SN = sn_bef + sn_aft,
                                                        IMEIRel = this.IMEIRel.Text.Trim(),
                                                        SIM = "",
                                                        VIP = "",
                                                        BAT = "",
                                                        SoftModel = this.SoftModel.Text.Trim(),
                                                        Version = this.SoftwareVersion.Text.Trim(),
                                                        Remark = this.Remake.Text.Trim(),
                                                        JS_PrintTime = ProductTime,
                                                        JS_TemplatePath = this.Select_Template1.Text.Trim(),
                                                        CH_PrintTime = "",
                                                        CH_TemplatePath1 = null,
                                                        CH_TemplatePath2 = null,
                                                        ICCID = "",
                                                        MAC = "",
                                                        Equipment = ""
                                                    });
                                                    if (PMB.InsertPrintMessageBLL(list))
                                                    {
                                                        Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                                        Form1.Log("批量打印了IMEI号为" + imei_begin + imei15 + "的制单", null);
                                                        imei_begin++;
                                                        sn_aft = (int.Parse(sn_aft) + 1).ToString().PadLeft(s, '0');
                                                    }
                                                }
                                                else
                                                {
                                                    //player.Play();
                                                    sn_aft = (int.Parse(sn_aft) + 1).ToString().PadLeft(s, '0');
                                                    i--;
                                                }
                                            }
                                            else
                                            {
                                                player.Play();
                                                this.reminder.AppendText("SN号不足");
                                                return;
                                            }
                                        }
                                        if (MOPB.UpdateSNnumberBLL(this.CB_ZhiDan.Text, sn_aft, imei_begin))
                                        {
                                            this.SN1_num.Text = sn_bef + sn_aft;
                                            this.IMEI_Present.Text = imei_begin.ToString();
                                            this.PrintNum.Clear();
                                            this.PrintNum.Focus();
                                        }
                                    }
                                    else
                                    {
                                        for (int i = 0; i < int.Parse(this.PrintNum.Text); i++)
                                        {
                                            imei15 = getimei15(imei_begin.ToString());
                                            btFormat.SubStrings["IMEI"].Value = imei_begin.ToString() + imei15;
                                            btFormat.SubStrings["SN"].Value = "";
                                            //记录打印信息日志
                                            ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                            list.Add(new PrintMessage()
                                            {
                                                Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                IMEI = imei_begin.ToString() + imei15,
                                                IMEIStart = this.IMEI_num1.Text.Trim(),
                                                IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                SN = "",
                                                IMEIRel = this.IMEIRel.Text.Trim(),
                                                SIM = "",
                                                VIP = "",
                                                BAT = "",
                                                SoftModel = this.SoftModel.Text.Trim(),
                                                Version = this.SoftwareVersion.Text.Trim(),
                                                Remark = this.Remake.Text.Trim(),
                                                JS_PrintTime = ProductTime,
                                                JS_TemplatePath = this.Select_Template1.Text.Trim(),
                                                CH_PrintTime = "",
                                                CH_TemplatePath1 = null,
                                                CH_TemplatePath2 = null
                                            });
                                            if (PMB.InsertPrintMessageBLL(list))
                                            {
                                                Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                                Form1.Log("批量打印了IMEI号为" + imei_begin + imei15 + "的制单", null);
                                                imei_begin++;
                                            }
                                        }
                                        if (MOPB.UpdateSNnumberBLL(this.CB_ZhiDan.Text, "", imei_begin))
                                        {
                                            this.IMEI_Present.Text = imei_begin.ToString();
                                            this.PrintNum.Clear();
                                            this.PrintNum.Focus();
                                        }
                                    }
                                }
                                break;
                            case 2:
                                {
                                    long imei_begin;
                                    string sn_bef, sn_aft, sn_laf;
                                    if (this.IMEI_Present.Text != "")
                                    {
                                        imei_begin = long.Parse(this.IMEI_Present.Text);
                                    }
                                    else
                                    {
                                        imei_begin = long.Parse(this.IMEI_num1.Text);
                                    }
                                    string EndIMEI = (imei_begin + int.Parse(this.PrintNum.Text)).ToString();
                                    list = PMB.CheckRangeIMEIBLL(imei_begin.ToString(), EndIMEI);
                                    if (list.Count > 0)
                                    {
                                        foreach (PrintMessage a in list)
                                        {
                                            this.reminder.AppendText(a.IMEI + "重号\r\n");
                                        }
                                        this.PrintNum.Clear();
                                        this.PrintNum.Focus();
                                        return;
                                    }
                                    if (this.SN1_num.Text != "")
                                    {
                                        sn_bef = this.SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                        sn_aft = this.SN1_num.Text.Remove(0, this.SN1_num.Text.Length - s);
                                        sn_laf = this.SN2_num.Text.Remove(0, this.SN2_num.Text.Length - s);
                                        for (int i = 0; i < int.Parse(this.PrintNum.Text); i++)
                                        {
                                            if (int.Parse(sn_aft) < int.Parse(sn_laf))
                                            {
                                                btFormat.SubStrings["IMEI"].Value = imei_begin.ToString();
                                                btFormat.SubStrings["SN"].Value = sn_bef + sn_aft;
                                                //记录打印信息日志
                                                ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                                list.Add(new PrintMessage()
                                                {
                                                    Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                    IMEI = imei_begin.ToString(),
                                                    IMEIStart = this.IMEI_num1.Text.Trim(),
                                                    IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                    SN = sn_bef + sn_aft,
                                                    IMEIRel = this.IMEIRel.Text.Trim(),
                                                    SIM = "",
                                                    VIP = "",
                                                    BAT = "",
                                                    SoftModel = this.SoftModel.Text.Trim(),
                                                    Version = this.SoftwareVersion.Text.Trim(),
                                                    Remark = this.Remake.Text.Trim(),
                                                    JS_PrintTime = ProductTime,
                                                    JS_TemplatePath = this.Select_Template1.Text.Trim(),
                                                    CH_PrintTime = "",
                                                    CH_TemplatePath1 = null,
                                                    CH_TemplatePath2 = null,
                                                    ICCID = "",
                                                    MAC = "",
                                                    Equipment = ""
                                                });
                                                if (PMB.InsertPrintMessageBLL(list))
                                                {
                                                    Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                                    Form1.Log("批量打印了IMEI号为" + imei_begin + "的制单", null);
                                                    imei_begin++;
                                                    sn_aft = (int.Parse(sn_aft) + 1).ToString().PadLeft(s, '0');
                                                }
                                            }
                                            else
                                            {
                                                player.Play();
                                                this.reminder.AppendText("SN号不足");
                                                return;
                                            }
                                        }
                                        if (MOPB.UpdateSNnumberBLL(this.CB_ZhiDan.Text, sn_aft, imei_begin))
                                        {
                                            this.SN1_num.Text = sn_bef + sn_aft;
                                            this.IMEI_Present.Text = imei_begin.ToString();
                                            this.PrintNum.Clear();
                                            this.PrintNum.Focus();
                                        }
                                    }
                                    else
                                    {
                                        for (int i = 0; i < int.Parse(this.PrintNum.Text); i++)
                                        {
                                            btFormat.SubStrings["IMEI"].Value = imei_begin.ToString();
                                            btFormat.SubStrings["SN"].Value = "";
                                            //记录打印信息日志
                                            ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                            list.Add(new PrintMessage()
                                            {
                                                Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                IMEI = imei_begin.ToString(),
                                                IMEIStart = this.IMEI_num1.Text.Trim(),
                                                IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                SN = "",
                                                IMEIRel = this.IMEIRel.Text.Trim(),
                                                SIM = "",
                                                VIP = "",
                                                BAT = "",
                                                SoftModel = this.SoftModel.Text.Trim(),
                                                Version = this.SoftwareVersion.Text.Trim(),
                                                Remark = this.Remake.Text.Trim(),
                                                JS_PrintTime = ProductTime,
                                                JS_TemplatePath = this.Select_Template1.Text.Trim(),
                                                CH_PrintTime = "",
                                                CH_TemplatePath1 = null,
                                                CH_TemplatePath2 = null
                                            });
                                            if (PMB.InsertPrintMessageBLL(list))
                                            {
                                                Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                                Form1.Log("批量打印了IMEI号为" + imei_begin + "的制单", null);
                                                imei_begin++;
                                            }
                                        }
                                        if (MOPB.UpdateSNnumberBLL(this.CB_ZhiDan.Text, "", imei_begin))
                                        {
                                            this.IMEI_Present.Text = imei_begin.ToString();
                                            this.PrintNum.Clear();
                                            this.PrintNum.Focus();
                                        }
                                    }
                                }
                                break;
                            case 3:
                                {
                                    long imei_begin;
                                    string sn_bef, sn_aft, sn_laf;
                                    if (this.IMEI_Present.Text != "")
                                    {
                                        imei_begin = long.Parse(this.IMEI_Present.Text);
                                    }
                                    else
                                    {
                                        imei_begin = long.Parse(this.IMEI_num1.Text);
                                    }
                                    string EndIMEI = (imei_begin + int.Parse(this.PrintNum.Text)).ToString();
                                    list = PMB.CheckRangeIMEIBLL(imei_begin.ToString(), EndIMEI);
                                    if (list.Count > 0)
                                    {
                                        foreach (PrintMessage a in list)
                                        {
                                            this.reminder.AppendText(a.IMEI + "重号\r\n");
                                        }
                                        this.PrintNum.Clear();
                                        this.PrintNum.Focus();
                                        return;
                                    }
                                    if (this.SN1_num.Text != "")
                                    {
                                        sn_bef = this.SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                        sn_aft = this.SN1_num.Text.Remove(0, this.SN1_num.Text.Length - s);
                                        sn_laf = this.SN2_num.Text.Remove(0, this.SN2_num.Text.Length - s);
                                        for (int i = 0; i < int.Parse(this.PrintNum.Text); i++)
                                        {
                                            if (int.Parse(sn_aft) < int.Parse(sn_laf))
                                            {
                                                btFormat.SubStrings["IMEI"].Value = imei_begin.ToString();
                                                if (!PMB.CheckSNBLL(sn_bef + sn_aft))
                                                {
                                                    btFormat.SubStrings["SN"].Value = sn_bef + sn_aft;
                                                    //记录打印信息日志
                                                    ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                                    list.Add(new PrintMessage()
                                                    {
                                                        Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                        IMEI = imei_begin.ToString(),
                                                        IMEIStart = this.IMEI_num1.Text.Trim(),
                                                        IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                        SN = sn_bef + sn_aft,
                                                        IMEIRel = this.IMEIRel.Text.Trim(),
                                                        SIM = "",
                                                        VIP = "",
                                                        BAT = "",
                                                        SoftModel = this.SoftModel.Text.Trim(),
                                                        Version = this.SoftwareVersion.Text.Trim(),
                                                        Remark = this.Remake.Text.Trim(),
                                                        JS_PrintTime = ProductTime,
                                                        JS_TemplatePath = this.Select_Template1.Text.Trim(),
                                                        CH_PrintTime = "",
                                                        CH_TemplatePath1 = null,
                                                        CH_TemplatePath2 = null,
                                                        ICCID = "",
                                                        MAC = "",
                                                        Equipment = ""
                                                    });
                                                    if (PMB.InsertPrintMessageBLL(list))
                                                    {
                                                        Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                                        Form1.Log("批量打印了IMEI号为" + imei_begin + "的制单", null);
                                                        imei_begin++;
                                                        sn_aft = (int.Parse(sn_aft) + 1).ToString().PadLeft(s, '0');
                                                    }
                                                }
                                                else
                                                {
                                                    //player.Play();
                                                    sn_aft = (int.Parse(sn_aft) + 1).ToString().PadLeft(s, '0');
                                                    i--;
                                                }
                                            }
                                            else
                                            {
                                                player.Play();
                                                this.reminder.AppendText("SN号不足");
                                                return;
                                            }
                                        }
                                        if (MOPB.UpdateSNnumberBLL(this.CB_ZhiDan.Text, sn_aft, imei_begin))
                                        {
                                            this.SN1_num.Text = sn_bef + sn_aft;
                                            this.IMEI_Present.Text = imei_begin.ToString();
                                            this.PrintNum.Clear();
                                            this.PrintNum.Focus();
                                        }
                                    }
                                    else
                                    {
                                        for (int i = 0; i < int.Parse(this.PrintNum.Text); i++)
                                        {
                                            btFormat.SubStrings["IMEI"].Value = imei_begin.ToString();
                                            btFormat.SubStrings["SN"].Value = "";
                                            //记录打印信息日志
                                            ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                            list.Add(new PrintMessage()
                                            {
                                                Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                IMEI = imei_begin.ToString(),
                                                IMEIStart = this.IMEI_num1.Text.Trim(),
                                                IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                SN = "",
                                                IMEIRel = this.IMEIRel.Text.Trim(),
                                                SIM = "",
                                                VIP = "",
                                                BAT = "",
                                                SoftModel = this.SoftModel.Text.Trim(),
                                                Version = this.SoftwareVersion.Text.Trim(),
                                                Remark = this.Remake.Text.Trim(),
                                                JS_PrintTime = ProductTime,
                                                JS_TemplatePath = this.Select_Template1.Text.Trim(),
                                                CH_PrintTime = "",
                                                CH_TemplatePath1 = null,
                                                CH_TemplatePath2 = null
                                            });
                                            if (PMB.InsertPrintMessageBLL(list))
                                            {
                                                Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                                Form1.Log("批量打印了IMEI号为" + imei_begin + "的制单", null);
                                                imei_begin++;
                                            }
                                        }
                                        if (MOPB.UpdateSNnumberBLL(this.CB_ZhiDan.Text, "", imei_begin))
                                        {
                                            this.IMEI_Present.Text = imei_begin.ToString();
                                            this.PrintNum.Clear();
                                            this.PrintNum.Focus();
                                        }
                                    }
                                }
                                break;
                            case 6:
                                {
                                    long imei_begin;
                                    string sn_aft;
                                    if (this.IMEI_Present.Text != "")
                                    {
                                        imei_begin = long.Parse(this.IMEI_Present.Text);
                                    }
                                    else
                                    {
                                        imei_begin = long.Parse(this.IMEI_num1.Text);
                                    }
                                    sn_aft = SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s);
                                    string EndIMEI = (imei_begin + int.Parse(this.PrintNum.Text)).ToString();
                                    list = PMB.CheckRangeIMEIBLL(imei_begin.ToString(), EndIMEI);
                                    if (list.Count > 0)
                                    {
                                        foreach (PrintMessage a in list)
                                        {
                                            this.reminder.AppendText(a.IMEI + "重号\r\n");
                                        }
                                        this.PrintNum.Clear();
                                        this.PrintNum.Focus();
                                        return;
                                    }
                                    for (int i = 0; i < int.Parse(this.PrintNum.Text); i++)
                                    {
                                        btFormat.SubStrings["IMEI"].Value = imei_begin.ToString();
                                        //记录打印信息日志
                                        ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                        list.Add(new PrintMessage()
                                        {
                                            Zhidan = this.CB_ZhiDan.Text.Trim(),
                                            IMEI = imei_begin.ToString(),
                                            IMEIStart = this.IMEI_num1.Text.Trim(),
                                            IMEIEnd = this.IMEI_num2.Text.Trim(),
                                            SN = "",
                                            IMEIRel = this.IMEIRel.Text.Trim(),
                                            SIM = "",
                                            VIP = "",
                                            BAT = "",
                                            SoftModel = this.SoftModel.Text.Trim(),
                                            Version = this.SoftwareVersion.Text.Trim(),
                                            Remark = this.Remake.Text.Trim(),
                                            JS_PrintTime = ProductTime,
                                            JS_TemplatePath = this.Select_Template1.Text.Trim(),
                                            CH_PrintTime = "",
                                            CH_TemplatePath1 = null,
                                            CH_TemplatePath2 = null,
                                            ICCID = "",
                                            MAC = "",
                                            Equipment = ""
                                        });
                                        if (PMB.InsertPrintMessageBLL(list))
                                        {
                                            Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                            Form1.Log("批量打印了IMEI号为" + imei_begin + "的制单", null);
                                            imei_begin++;
                                        }
                                    }
                                    if (MOPB.UpdateSNnumberBLL(this.CB_ZhiDan.Text, sn_aft, imei_begin))
                                    {
                                        this.IMEI_Present.Text = imei_begin.ToString();
                                        this.PrintNum.Clear();
                                        this.PrintNum.Focus();
                                    }
                                }
                                break;
                        }
                    }
                    else
                    {
                        player1.Play();
                        this.reminder.AppendText("请先选择模板\r\n");
                        this.PrintNum.Clear();
                        this.PrintNum.Focus();
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Exception:" + ex.Message);
                }
            }
        }

        private void IMEI_Start_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13) {
                if (this.CB_ZhiDan.Text != "")
                {
                    if (this.NoCheckCode.Checked == false)
                    {
                        string imei14;
                        String imeiRes = "";
                        if (this.IMEI_Start.Text != "" && IsNumeric(this.IMEI_Start.Text) && this.IMEI_Start.Text.Length == 15)
                        {
                            imei14 = this.IMEI_Start.Text.Substring(0, 14);
                            long IMEI_Start = long.Parse(imei14);
                            if (IMEI_Start < long.Parse(this.IMEI_num1.Text))
                            {
                                player.Play();
                                this.reminder.AppendText(IMEI_Start+"IMEI不在范围内\r\n");
                                this.IMEI_Start.Clear();
                                this.IMEI_Start.Focus();
                                return;
                            }
                            else if (IMEI_Start > long.Parse(this.IMEI_num2.Text))
                            {
                                player.Play();
                                this.reminder.AppendText(IMEI_Start + "IMEI不在范围内\r\n");
                                this.IMEI_Start.Clear();
                                this.IMEI_Start.Focus();
                                return;
                            }
                            else
                            {
                                string imei15 = getimei15(imei14);
                                imeiRes = imei14 + imei15;
                                if (imeiRes != this.IMEI_Start.Text)
                                {
                                    player3.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "IMEI校验错误\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                    return;
                                }
                            }
                        }
                        else if (this.IMEI_Start.Text == "")
                        {
                            player.Play();
                            this.reminder.AppendText("请输入IMEI\r\n");
                            this.IMEI_Start.Focus();
                            return;
                        }
                        else
                        {
                            player.Play();
                            this.reminder.AppendText("IMEI格式错误\r\n");
                            this.IMEI_Start.Clear();
                            this.IMEI_Start.Focus();
                            return;
                        }
                    }
                    else
                    {
                        if (this.IMEI_Start.Text != "" && IsNumeric(this.IMEI_Start.Text))
                        {
                            long IMEI_Start = long.Parse(this.IMEI_Start.Text);
                            if (IMEI_Start < long.Parse(this.IMEI_num1.Text))
                            {
                                player.Play();
                                this.reminder.AppendText("IMEI不在范围内\r\n");
                                this.IMEI_Start.Clear();
                                this.IMEI_Start.Focus();
                                return;
                            }
                            else if (IMEI_Start > long.Parse(this.IMEI_num2.Text))
                            {
                                player.Play();
                                this.reminder.AppendText("IMEI不在范围内\r\n");
                                this.IMEI_Start.Clear();
                                this.IMEI_Start.Focus();
                                return;
                            }
                        }
                        else if (this.IMEI_Start.Text == "")
                        {
                            player.Play();
                            this.reminder.AppendText("请输入IMEI\r\n");
                            this.IMEI_Start.Focus();
                            return;
                        }
                        else
                        {
                            player.Play();
                            this.reminder.AppendText("IMEI格式错误\r\n");
                            this.IMEI_Start.Clear();
                            this.IMEI_Start.Focus();
                            return;
                        }
                    }
                }
                else {
                    player2.Play();
                    this.reminder.AppendText("请选择制单号\r\n");
                    this.IMEI_Start.Clear();
                    this.IMEI_Start.Focus();
                    return;
                }
                try
                {
                    if (this.Select_Template1.Text != "")
                    {
                        lj = this.Select_Template1.Text;
                        LabelFormatDocument btFormat = btEngine.Documents.Open(lj);
                        //指定打印机名称
                        btFormat.PrintSetup.PrinterName = this.Printer1.Text;
                        //打印份数,同序列打印的份数
                        btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                        switch (c1 + c2 + c3)
                        {
                            case 4:
                                {
                                    btFormat.SubStrings["IMEI"].Value = this.IMEI_Start.Text;
                                    if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                    {
                                        //对模板相应字段进行赋值
                                        ValueToTemplate(btFormat);
                                        //记录打印信息日志
                                        ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                        list.Add(new PrintMessage()
                                        {
                                            Zhidan = this.CB_ZhiDan.Text.Trim(),
                                            IMEI = this.IMEI_Start.Text.Trim(),
                                            IMEIStart = this.IMEI_num1.Text.Trim(),
                                            IMEIEnd = this.IMEI_num2.Text.Trim(),
                                            SN = "",
                                            IMEIRel = this.IMEIRel.Text.Trim(),
                                            SIM = "",
                                            VIP = "",
                                            BAT = "",
                                            SoftModel = this.SoftModel.Text.Trim(),
                                            Version = this.SoftwareVersion.Text.Trim(),
                                            Remark = this.Remake.Text.Trim(),
                                            JS_PrintTime = ProductTime,
                                            JS_TemplatePath = this.Select_Template1.Text.Trim(),
                                            CH_PrintTime = "",
                                            CH_TemplatePath1 = null,
                                            CH_TemplatePath2 = null,
                                            ICCID = "",
                                            MAC = "",
                                            Equipment = ""
                                        });
                                        if (PMB.InsertPrintMessageBLL(list))
                                        {
                                            long sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                            if (MOPB.UpdateSNnumberBLL(this.CB_ZhiDan.Text, sn1_suffix.ToString().PadLeft(s, '0'), long.Parse(this.IMEI_Start.Text.Substring(0, 14)) + 1))
                                            {
                                                Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                                Form1.Log("打印了IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                                long imei_star14 = long.Parse(this.IMEI_Start.Text.Substring(0, 14)) + 1;
                                                this.IMEI_Start.Clear();
                                                this.IMEI_Start.Focus();
                                                this.IMEI_Present.Text = imei_star14.ToString();
                                            }
                                        }
                                    }
                                    else
                                    {
                                        player.Play();
                                        this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                        this.IMEI_Start.Clear();
                                        this.IMEI_Start.Focus();
                                    }
                                }
                                break;
                            case 0:
                                {
                                    btFormat.SubStrings["IMEI"].Value = this.IMEI_Start.Text;
                                    if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                    {
                                        //对模板相应字段进行赋值
                                        ValueToTemplate(btFormat);
                                        //记录打印信息日志
                                        ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                        if (this.SN1_num.Text != "")
                                        {
                                            btFormat.SubStrings["SN"].Value = this.SN1_num.Text;
                                            list.Add(new PrintMessage()
                                            {
                                                Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                IMEI = this.IMEI_Start.Text.Trim(),
                                                IMEIStart = this.IMEI_num1.Text.Trim(),
                                                IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                SN = this.SN1_num.Text,
                                                IMEIRel = this.IMEIRel.Text.Trim(),
                                                SIM = "",
                                                VIP = "",
                                                BAT = "",
                                                SoftModel = this.SoftModel.Text.Trim(),
                                                Version = this.SoftwareVersion.Text.Trim(),
                                                Remark = this.Remake.Text.Trim(),
                                                JS_PrintTime = ProductTime,
                                                JS_TemplatePath = this.Select_Template1.Text.Trim(),
                                                CH_PrintTime = "",
                                                CH_TemplatePath1 = null,
                                                CH_TemplatePath2 = null,
                                                ICCID = "",
                                                MAC = "",
                                                Equipment = ""
                                            });
                                            if (PMB.InsertPrintMessageBLL(list))
                                            {
                                                string sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                long sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                string sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                string sn2_suffix = SN2_num.Text.Remove(0, (this.SN2_num.Text.Length) - s);
                                                if (MOPB.UpdateSNnumberBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0'), long.Parse(this.IMEI_Start.Text.Substring(0, 14)) + 1))
                                                {
                                                    Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                                    Form1.Log("打印了IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                                    this.SN1_num.Text = sn1;
                                                    long imei_star14 = long.Parse(this.IMEI_Start.Text.Substring(0, 14)) + 1;
                                                    this.IMEI_Start.Clear();
                                                    this.IMEI_Start.Focus();
                                                    this.IMEI_Present.Text = imei_star14.ToString();
                                                }
                                            }
                                        }
                                        else
                                        {
                                            btFormat.SubStrings["SN"].Value = "";
                                            list.Add(new PrintMessage()
                                            {
                                                Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                IMEI = this.IMEI_Start.Text.Trim(),
                                                IMEIStart = this.IMEI_num1.Text.Trim(),
                                                IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                SN = "",
                                                IMEIRel = this.IMEIRel.Text.Trim(),
                                                SIM = "",
                                                VIP = "",
                                                BAT = "",
                                                SoftModel = this.SoftModel.Text.Trim(),
                                                Version = this.SoftwareVersion.Text.Trim(),
                                                Remark = this.Remake.Text.Trim(),
                                                JS_PrintTime = ProductTime,
                                                JS_TemplatePath = this.Select_Template1.Text.Trim(),
                                                CH_PrintTime = "",
                                                CH_TemplatePath1 = null,
                                                CH_TemplatePath2 = null
                                            });
                                            if (PMB.InsertPrintMessageBLL(list))
                                            {
                                                if (MOPB.UpdateSNnumberBLL(this.CB_ZhiDan.Text, "", long.Parse(this.IMEI_Start.Text.Substring(0, 14)) + 1))
                                                {
                                                    Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                                    Form1.Log("打印了IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                                    long imei_star14 = long.Parse(this.IMEI_Start.Text.Substring(0, 14)) + 1;
                                                    this.IMEI_Start.Clear();
                                                    this.IMEI_Start.Focus();
                                                    this.IMEI_Present.Text = imei_star14.ToString();
                                                }
                                            }
                                        }
                                    }
                                    else if (PMB.CheckCHOrJSIMEIBLL(this.IMEI_Start.Text, 1))
                                    {
                                        ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                        list = PMB.SelectSnByIMEIBLL(this.IMEI_Start.Text);
                                        foreach (PrintMessage a in list)
                                        {
                                            btFormat.SubStrings["SN"].Value = a.SN;
                                        }
                                        if (MOPB.UpdateJSmesBLL(this.IMEI_Start.Text, ProductTime, lj))
                                        {
                                            Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                            Form1.Log("打印了机身贴IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                            this.IMEI_Start.Clear();
                                            this.IMEI_Start.Focus();
                                        }
                                    }
                                    else
                                    {
                                        player.Play();
                                        this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                        this.IMEI_Start.Clear();
                                        this.IMEI_Start.Focus();
                                    }
                                }
                                break;
                            case 1:
                                {
                                    btFormat.SubStrings["IMEI"].Value = this.IMEI_Start.Text;
                                    if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                    {
                                        //对模板相应字段进行赋值
                                        ValueToTemplate(btFormat);
                                        //记录打印信息日志
                                        ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                        if (this.SN1_num.Text != "")
                                        {
                                            if (!PMB.CheckSNBLL(this.SN1_num.Text))
                                            {
                                                btFormat.SubStrings["SN"].Value = this.SN1_num.Text;
                                                list.Add(new PrintMessage()
                                                {
                                                    Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                    IMEI = this.IMEI_Start.Text.Trim(),
                                                    IMEIStart = this.IMEI_num1.Text.Trim(),
                                                    IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                    SN = this.SN1_num.Text,
                                                    IMEIRel = this.IMEIRel.Text.Trim(),
                                                    SIM = "",
                                                    VIP = "",
                                                    BAT = "",
                                                    SoftModel = this.SoftModel.Text.Trim(),
                                                    Version = this.SoftwareVersion.Text.Trim(),
                                                    Remark = this.Remake.Text.Trim(),
                                                    JS_PrintTime = ProductTime,
                                                    JS_TemplatePath = this.Select_Template1.Text.Trim(),
                                                    CH_PrintTime = "",
                                                    CH_TemplatePath1 = null,
                                                    CH_TemplatePath2 = null,
                                                    ICCID = "",
                                                    MAC = "",
                                                    Equipment = ""
                                                });
                                                if (PMB.InsertPrintMessageBLL(list))
                                                {
                                                    string sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                    long sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                    string sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                    if (MOPB.UpdateSNnumberBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0'), long.Parse(this.IMEI_Start.Text.Substring(0, 14)) + 1))
                                                    {
                                                        Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                                        Form1.Log("打印了IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                                        this.SN1_num.Text = sn1;
                                                        long imei_star14 = long.Parse(this.IMEI_Start.Text.Substring(0, 14)) + 1;
                                                        this.IMEI_Start.Clear();
                                                        this.IMEI_Start.Focus();
                                                        this.IMEI_Present.Text = imei_star14.ToString();
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                player.Play();
                                                string sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                long sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0'));
                                                this.SN1_num.Text = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                this.IMEI_Start.Clear();
                                                this.IMEI_Start.Focus();
                                            }
                                        }
                                        else
                                        {
                                            btFormat.SubStrings["SN"].Value = "";
                                            list.Add(new PrintMessage()
                                            {
                                                Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                IMEI = this.IMEI_Start.Text.Trim(),
                                                IMEIStart = this.IMEI_num1.Text.Trim(),
                                                IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                SN = "",
                                                IMEIRel = this.IMEIRel.Text.Trim(),
                                                SIM = "",
                                                VIP = "",
                                                BAT = "",
                                                SoftModel = this.SoftModel.Text.Trim(),
                                                Version = this.SoftwareVersion.Text.Trim(),
                                                Remark = this.Remake.Text.Trim(),
                                                JS_PrintTime = ProductTime,
                                                JS_TemplatePath = this.Select_Template1.Text.Trim(),
                                                CH_PrintTime = "",
                                                CH_TemplatePath1 = null,
                                                CH_TemplatePath2 = null
                                            });
                                            if (PMB.InsertPrintMessageBLL(list))
                                            {
                                                if (MOPB.UpdateSNnumberBLL(this.CB_ZhiDan.Text, "", long.Parse(this.IMEI_Start.Text.Substring(0, 14)) + 1))
                                                {
                                                    Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                                    Form1.Log("打印了IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                                    long imei_star14 = long.Parse(this.IMEI_Start.Text.Substring(0, 14)) + 1;
                                                    this.IMEI_Start.Clear();
                                                    this.IMEI_Start.Focus();
                                                    this.IMEI_Present.Text = imei_star14.ToString();
                                                }
                                            }
                                        }
                                    }
                                    else if (PMB.CheckCHOrJSIMEIBLL(this.IMEI_Start.Text, 1))
                                    {
                                        ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                        list = PMB.SelectSnByIMEIBLL(this.IMEI_Start.Text);
                                        foreach (PrintMessage a in list)
                                        {
                                            btFormat.SubStrings["SN"].Value = a.SN;
                                        }
                                        if (MOPB.UpdateJSmesBLL(this.IMEI_Start.Text, ProductTime, lj))
                                        {
                                            Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                            Form1.Log("打印了机身贴IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                            this.IMEI_Start.Clear();
                                            this.IMEI_Start.Focus();
                                        }
                                    }
                                    else
                                    {
                                        player.Play();
                                        this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                        this.IMEI_Start.Clear();
                                        this.IMEI_Start.Focus();
                                    }
                                }
                                break;
                            case 2:
                                {
                                    btFormat.SubStrings["IMEI"].Value = this.IMEI_Start.Text;
                                    if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                    {
                                        //对模板相应字段进行赋值
                                        ValueToTemplate(btFormat);
                                        //记录打印信息日志
                                        ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                        if (this.SN1_num.Text != "")
                                        {
                                            btFormat.SubStrings["SN"].Value = this.SN1_num.Text;
                                            list.Add(new PrintMessage()
                                            {
                                                Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                IMEI = this.IMEI_Start.Text.Trim(),
                                                IMEIStart = this.IMEI_num1.Text.Trim(),
                                                IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                SN = this.SN1_num.Text,
                                                IMEIRel = this.IMEIRel.Text.Trim(),
                                                SIM = "",
                                                VIP = "",
                                                BAT = "",
                                                SoftModel = this.SoftModel.Text.Trim(),
                                                Version = this.SoftwareVersion.Text.Trim(),
                                                Remark = this.Remake.Text.Trim(),
                                                JS_PrintTime = ProductTime,
                                                JS_TemplatePath = this.Select_Template1.Text.Trim(),
                                                CH_PrintTime = "",
                                                CH_TemplatePath1 = null,
                                                CH_TemplatePath2 = null,
                                                ICCID = "",
                                                MAC = "",
                                                Equipment = ""
                                            });
                                            if (PMB.InsertPrintMessageBLL(list))
                                            {
                                                string sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                long sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                string sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                string sn2_suffix = SN2_num.Text.Remove(0, (this.SN2_num.Text.Length) - s);
                                                if (MOPB.UpdateSNnumberBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0'), long.Parse(this.IMEI_Start.Text) + 1))
                                                {
                                                    Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                                    Form1.Log("打印了IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                                    this.SN1_num.Text = sn1;
                                                    long imei_star14 = long.Parse(this.IMEI_Start.Text) + 1;
                                                    this.IMEI_Start.Clear();
                                                    this.IMEI_Start.Focus();
                                                    this.IMEI_Present.Text = imei_star14.ToString();
                                                }
                                            }
                                        }
                                        else
                                        {
                                            btFormat.SubStrings["SN"].Value = "";
                                            list.Add(new PrintMessage()
                                            {
                                                Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                IMEI = this.IMEI_Start.Text.Trim(),
                                                IMEIStart = this.IMEI_num1.Text.Trim(),
                                                IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                SN = "",
                                                IMEIRel = this.IMEIRel.Text.Trim(),
                                                SIM = "",
                                                VIP = "",
                                                BAT = "",
                                                SoftModel = this.SoftModel.Text.Trim(),
                                                Version = this.SoftwareVersion.Text.Trim(),
                                                Remark = this.Remake.Text.Trim(),
                                                JS_PrintTime = ProductTime,
                                                JS_TemplatePath = this.Select_Template1.Text.Trim(),
                                                CH_PrintTime = "",
                                                CH_TemplatePath1 = null,
                                                CH_TemplatePath2 = null
                                            });
                                            if (PMB.InsertPrintMessageBLL(list))
                                            {
                                                string sn2_suffix;
                                                if (this.SN2_num.Text != "")
                                                {
                                                    sn2_suffix = SN2_num.Text.Remove(0, (this.SN1_num.Text.Length) - s);
                                                }
                                                else
                                                {
                                                    sn2_suffix = "";
                                                }
                                                if (MOPB.UpdateSNnumberBLL(this.CB_ZhiDan.Text, "", long.Parse(this.IMEI_Start.Text) + 1))
                                                {
                                                    Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                                    Form1.Log("打印了IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                                    long imei_star14 = long.Parse(this.IMEI_Start.Text) + 1;
                                                    this.IMEI_Start.Clear();
                                                    this.IMEI_Start.Focus();
                                                    this.IMEI_Present.Text = imei_star14.ToString();
                                                }
                                            }
                                        }
                                    }
                                    else if (PMB.CheckCHOrJSIMEIBLL(this.IMEI_Start.Text, 1))
                                    {
                                        ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                        list = PMB.SelectSnByIMEIBLL(this.IMEI_Start.Text);
                                        foreach (PrintMessage a in list)
                                        {
                                            btFormat.SubStrings["SN"].Value = a.SN;
                                        }
                                        if (MOPB.UpdateJSmesBLL(this.IMEI_Start.Text, ProductTime, lj))
                                        {
                                            Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                            Form1.Log("打印了机身贴IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                            this.IMEI_Start.Clear();
                                            this.IMEI_Start.Focus();
                                        }
                                    }
                                    else
                                    {
                                        player.Play();
                                        this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                        this.IMEI_Start.Clear();
                                        this.IMEI_Start.Focus();
                                    }
                                }
                                break;
                            case 3:
                                {
                                    btFormat.SubStrings["IMEI"].Value = this.IMEI_Start.Text;
                                    if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                    {
                                        //对模板相应字段进行赋值
                                        ValueToTemplate(btFormat);
                                        //记录打印信息日志
                                        ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                        if (this.SN1_num.Text != "")
                                        {
                                            if (!PMB.CheckSNBLL(this.SN1_num.Text))
                                            {
                                                btFormat.SubStrings["SN"].Value = this.SN1_num.Text;
                                                list.Add(new PrintMessage()
                                                {
                                                    Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                    IMEI = this.IMEI_Start.Text.Trim(),
                                                    IMEIStart = this.IMEI_num1.Text.Trim(),
                                                    IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                    SN = this.SN1_num.Text,
                                                    IMEIRel = this.IMEIRel.Text.Trim(),
                                                    SIM = "",
                                                    VIP = "",
                                                    BAT = "",
                                                    SoftModel = this.SoftModel.Text.Trim(),
                                                    Version = this.SoftwareVersion.Text.Trim(),
                                                    Remark = this.Remake.Text.Trim(),
                                                    JS_PrintTime = ProductTime,
                                                    JS_TemplatePath = this.Select_Template1.Text.Trim(),
                                                    CH_PrintTime = "",
                                                    CH_TemplatePath1 = null,
                                                    CH_TemplatePath2 = null,
                                                    ICCID = "",
                                                    MAC = "",
                                                    Equipment = ""
                                                });
                                                if (PMB.InsertPrintMessageBLL(list))
                                                {
                                                    string sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                    long sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                    string sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                    if (MOPB.UpdateSNnumberBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0'), long.Parse(this.IMEI_Start.Text) + 1))
                                                    {
                                                        Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                                        Form1.Log("打印了IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                                        this.SN1_num.Text = sn1;
                                                        long imei_star14 = long.Parse(this.IMEI_Start.Text) + 1;
                                                        this.IMEI_Start.Clear();
                                                        this.IMEI_Start.Focus();
                                                        this.IMEI_Present.Text = imei_star14.ToString();
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                player.Play();
                                                string sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                long sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0'));
                                                this.SN1_num.Text = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                this.IMEI_Start.Clear();
                                                this.IMEI_Start.Focus();
                                            }
                                        }
                                        else
                                        {
                                            btFormat.SubStrings["SN"].Value = "";
                                            list.Add(new PrintMessage()
                                            {
                                                Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                IMEI = this.IMEI_Start.Text.Trim(),
                                                IMEIStart = this.IMEI_num1.Text.Trim(),
                                                IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                SN = "",
                                                IMEIRel = this.IMEIRel.Text.Trim(),
                                                SIM = "",
                                                VIP = "",
                                                BAT = "",
                                                SoftModel = this.SoftModel.Text.Trim(),
                                                Version = this.SoftwareVersion.Text.Trim(),
                                                Remark = this.Remake.Text.Trim(),
                                                JS_PrintTime = ProductTime,
                                                JS_TemplatePath = this.Select_Template1.Text.Trim(),
                                                CH_PrintTime = "",
                                                CH_TemplatePath1 = null,
                                                CH_TemplatePath2 = null
                                            });
                                            if (PMB.InsertPrintMessageBLL(list))
                                            {
                                                if (MOPB.UpdateSNnumberBLL(this.CB_ZhiDan.Text, "", long.Parse(this.IMEI_Start.Text) + 1))
                                                {
                                                    Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                                    Form1.Log("打印了IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                                    long imei_star14 = long.Parse(this.IMEI_Start.Text) + 1;
                                                    this.IMEI_Start.Clear();
                                                    this.IMEI_Start.Focus();
                                                    this.IMEI_Present.Text = imei_star14.ToString();
                                                }
                                            }
                                        }
                                    }
                                    else if (PMB.CheckCHOrJSIMEIBLL(this.IMEI_Start.Text, 1))
                                    {
                                        ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                        list = PMB.SelectSnByIMEIBLL(this.IMEI_Start.Text);
                                        foreach (PrintMessage a in list)
                                        {
                                            btFormat.SubStrings["SN"].Value = a.SN;
                                        }
                                        if (MOPB.UpdateJSmesBLL(this.IMEI_Start.Text, ProductTime, lj))
                                        {
                                            Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                            Form1.Log("打印了机身贴IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                            this.IMEI_Start.Clear();
                                            this.IMEI_Start.Focus();
                                        }
                                    }
                                    else
                                    {
                                        player.Play();
                                        this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                        this.IMEI_Start.Clear();
                                        this.IMEI_Start.Focus();
                                    }
                                }
                                break;
                            case 6:
                                {
                                    //对模板相应字段进行赋值
                                    ValueToTemplate(btFormat);
                                    btFormat.SubStrings["IMEI"].Value = this.IMEI_Start.Text;
                                    if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                    {
                                        //记录打印信息日志
                                        ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                        list.Add(new PrintMessage()
                                        {
                                            Zhidan = this.CB_ZhiDan.Text.Trim(),
                                            IMEI = this.IMEI_Start.Text.Trim(),
                                            IMEIStart = this.IMEI_num1.Text.Trim(),
                                            IMEIEnd = this.IMEI_num2.Text.Trim(),
                                            SN = "",
                                            IMEIRel = this.IMEIRel.Text.Trim(),
                                            SIM = "",
                                            VIP = "",
                                            BAT = "",
                                            SoftModel = this.SoftModel.Text.Trim(),
                                            Version = this.SoftwareVersion.Text.Trim(),
                                            Remark = this.Remake.Text.Trim(),
                                            JS_PrintTime = ProductTime,
                                            JS_TemplatePath = this.Select_Template1.Text.Trim(),
                                            CH_PrintTime = "",
                                            CH_TemplatePath1 = null,
                                            CH_TemplatePath2 = null,
                                            ICCID = "",
                                            MAC = "",
                                            Equipment = ""
                                        });
                                        if (PMB.InsertPrintMessageBLL(list))
                                        {
                                            long sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                            if (MOPB.UpdateSNnumberBLL(this.CB_ZhiDan.Text, sn1_suffix.ToString().PadLeft(s, '0'), long.Parse(this.IMEI_Start.Text) + 1))
                                            {
                                                Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                                Form1.Log("打印了IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                                long imei_star14 = long.Parse(this.IMEI_Start.Text) + 1;
                                                this.IMEI_Start.Clear();
                                                this.IMEI_Start.Focus();
                                                this.IMEI_Present.Text = imei_star14.ToString();
                                            }
                                        }
                                    }
                                    else
                                    {
                                        player.Play();
                                        this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                        this.IMEI_Start.Clear();
                                        this.IMEI_Start.Focus();
                                    }
                                }
                                break;
                        }
                    }
                    else
                    {
                        player1.Play();
                        this.reminder.AppendText("请选择模板\r\n");
                        this.IMEI_Start.Clear();
                        this.IMEI_Start.Focus();
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Exception:" + ex.Message);
                }
            }
        }

        private void ValueToTemplate(LabelFormatDocument btFormat)
        {
            //GetValue("Information", "型号", out outString);
            //btFormat.SubStrings[outString].Value = this.SoftModel.Text;
            GetValue("Information", "生产日期", out outString);
            btFormat.SubStrings[outString].Value = this.ProductData.Text;
            //GetValue("Information", "软件版本", out outString);
            //btFormat.SubStrings[outString].Value = this.SoftwareVersion.Text;
            //GetValue("Information", "备注", out outString);
            //btFormat.SubStrings[outString].Value = this.Remake.Text;
        }

        private void Re_IMEINum_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13) {
                if (this.Re_IMEINum.Text != "")
                {
                    if (this.Re_Nocheckcode.Checked == false)
                    {
                        if (IsNumeric(this.Re_IMEINum.Text))
                        {
                            if (this.Re_IMEINum.Text.Length != 15)
                            {
                                player.Play();
                                this.reminder.AppendText("请输入15位IMEI\r\n");
                                this.Re_IMEINum.Clear();
                                this.Re_IMEINum.Focus();
                                return;
                            }
                            else
                            {
                                string imeiRes;
                                string imei14 = this.Re_IMEINum.Text.Substring(0, 14);
                                string imei15 = getimei15(imei14);
                                imeiRes = imei14 + imei15;
                                if (imeiRes != this.Re_IMEINum.Text)
                                {
                                    player3.Play();
                                    this.reminder.AppendText("校验错误\r\n");
                                    this.Re_IMEINum.Clear();
                                    this.Re_IMEINum.Focus();
                                    return;
                                }
                            }
                        }
                        else
                        {
                            player.Play();
                            this.reminder.AppendText("IMEI格式错误\r\n");
                            this.Re_IMEINum.Clear();
                            this.Re_IMEINum.Focus();
                            return;
                        }
                    }
                    else {
                        if (!IsNumeric(this.Re_IMEINum.Text)) {
                            player.Play();
                            this.reminder.AppendText("IMEI格式错误\r\n");
                            this.Re_IMEINum.Clear();
                            this.Re_IMEINum.Focus();
                            return;
                        }
                    }
                }
                else
                {
                    player.Play();
                    this.reminder.AppendText("请输入重打IMEI\r\n");
                    this.Re_IMEINum.Focus();
                    return;
                }
                try
                {
                    if (this.Select_Template1.Text != "")
                    {
                        lj = this.Select_Template1.Text;
                        LabelFormatDocument btFormat = btEngine.Documents.Open(lj);
                        //指定打印机名称
                        btFormat.PrintSetup.PrinterName = this.Printer1.Text;
                        //对模板相应字段进行赋值
                        GetValue("Information", "生产日期", out outString);
                        btFormat.SubStrings[outString].Value = this.ProductData.Text;
                        //打印份数,同序列打印的份数
                        btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;

                        btFormat.SubStrings["IMEI"].Value = this.Re_IMEINum.Text;
                        if (PMB.CheckReCHOrJSIMEIBLL(this.Re_IMEINum.Text, 1))
                        {
                            list = PMB.SelectSnByIMEIBLL(this.Re_IMEINum.Text);
                            foreach (PrintMessage a in list)
                            {
                                btFormat.SubStrings["SN"].Value = a.SN;
                            }
                            //更新打印信息到数据表
                            string RE_PrintTime = System.DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss:fff");
                            if (PMB.UpdateRePrintBLL(this.Re_IMEINum.Text, RE_PrintTime, 1, lj, lj))
                            {
                                Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                Form1.Log("重打了IMEI号为" + this.Re_IMEINum.Text + "的制单", null);
                                this.Re_IMEINum.Clear();
                                this.Re_IMEINum.Focus();
                            }
                        }
                        else
                        {
                            player.Play();
                            this.reminder.AppendText(this.Re_IMEINum.Text + "无记录\r\n");
                            this.Re_IMEINum.Clear();
                            this.Re_IMEINum.Focus();
                        }
                    }
                    else
                    {
                        player1.Play();
                        this.reminder.AppendText("请先选择模板\r\n");
                        this.Re_IMEINum.Clear();
                        this.Re_IMEINum.Focus();
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Exception:" + ex.Message);
                }
            }
        }

        private void SnFromCustomer_Click(object sender, EventArgs e)
        {
            if (this.SnFromCustomer.Checked == true)
            {
                c1 = 1;
                if (this.NoSn.Checked == true)
                {
                    this.NoSn.Checked = false;
                    c3 = 0;
                }
            }
            else {
                c1 = 0;
            }
        }

        private void NoCheckCode_Click(object sender, EventArgs e)
        {
            if (this.NoCheckCode.Checked == true)
            {
                c2 = 2;
            }
            else
            {
                c2 = 0;
            }
        }

        private void NoSn_Click(object sender, EventArgs e)
        {
            if (this.NoSn.Checked == true)
            {
                c3 = 4;
                if (this.SnFromCustomer.Checked == true)
                {
                    this.SnFromCustomer.Checked = false;
                    c1 = 0;
                }
            }
            else
            {
                c3 = 0;
            }
        }

        private void ReImeiNum1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                if (this.ReImeiNum1.Text != "")
                {
                    if (this.Re_Nocheckcode.Checked == false)
                    {
                        if (IsNumeric(this.ReImeiNum1.Text))
                        {
                            if (this.ReImeiNum1.Text.Length != 15)
                            {
                                player.Play();
                                this.reminder.AppendText("请输入15位IMEI\r\n");
                                this.ReImeiNum1.Clear();
                                this.ReImeiNum1.Focus();
                                return;
                            }
                            else
                            {
                                string imeiRes;
                                string imei14 = this.ReImeiNum1.Text.Substring(0, 14);
                                string imei15 = getimei15(imei14);
                                imeiRes = imei14 + imei15;
                                if (imeiRes != this.ReImeiNum1.Text)
                                {
                                    player.Play();
                                    this.reminder.AppendText("IMEI校验错误\r\n");
                                    this.ReImeiNum1.Clear();
                                    this.ReImeiNum1.Focus();
                                    return;
                                }
                            }
                        }
                        else
                        {
                            player.Play();
                            this.reminder.AppendText("IMEI格式错误\r\n");
                            this.ReImeiNum1.Clear();
                            this.ReImeiNum1.Focus();
                            return;
                        }
                    }
                    else {
                        if (!IsNumeric(this.ReImeiNum1.Text))
                        {
                            player.Play();
                            this.reminder.AppendText("IMEI格式错误\r\n");
                            this.ReImeiNum1.Clear();
                            this.ReImeiNum1.Focus();
                            return;
                        }
                    }
                    this.ReImeiNum2.Focus();
                }
            }
        }

        private void ReImeiNum2_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                if (this.ReImeiNum2.Text != "")
                {
                    if (this.Re_Nocheckcode.Checked == false)
                    {
                        if (IsNumeric(this.ReImeiNum2.Text))
                        {
                            if (this.ReImeiNum2.Text.Length != 15)
                            {
                                player.Play();
                                this.reminder.AppendText("请输入15位IMEI\r\n");
                                this.ReImeiNum2.Clear();
                                this.ReImeiNum2.Focus();
                                return;
                            }
                            else
                            {
                                string imeiRes;
                                string imei14 = this.ReImeiNum2.Text.Substring(0, 14);
                                string imei15 = getimei15(imei14);
                                imeiRes = imei14 + imei15;
                                if (imeiRes != this.ReImeiNum2.Text)
                                {
                                    player3.Play();
                                    this.reminder.AppendText("IMEI校验错误\r\n");
                                    this.ReImeiNum2.Clear();
                                    this.ReImeiNum2.Focus();
                                    return;
                                }
                                else if (long.Parse(imei14) < long.Parse(this.ReImeiNum1.Text.Substring(0, 14)))
                                {
                                    player.Play();
                                    this.reminder.AppendText("IMEI小于重打起始位\r\n");
                                    this.ReImeiNum2.Clear();
                                    this.ReImeiNum2.Focus();
                                    return;
                                }
                            }
                        }
                        else
                        {
                            player.Play();
                            this.reminder.AppendText("IMEI格式错误\r\n");
                            this.ReImeiNum2.Clear();
                            this.ReImeiNum2.Focus();
                            return;
                        }
                    }
                    else {
                        if (!IsNumeric(this.ReImeiNum2.Text))
                        {
                            player.Play();
                            this.reminder.AppendText("IMEI格式错误\r\n");
                            this.ReImeiNum2.Clear();
                            this.ReImeiNum2.Focus();
                            return;
                        }
                    }
                }
                else {
                    player.Play();
                    this.reminder.AppendText("请输入重打终止位\r\n");
                    this.ReImeiNum2.Focus();
                    return;
                }
                try
                {
                    if (this.Select_Template1.Text != "")
                    {
                        lj = this.Select_Template1.Text;
                        LabelFormatDocument btFormat = btEngine.Documents.Open(lj);
                        //指定打印机名称
                        btFormat.PrintSetup.PrinterName = this.Printer1.Text;
                        //对模板相应字段进行赋值
                        GetValue("Information", "生产日期", out outString);
                        btFormat.SubStrings[outString].Value = this.ProductData.Text;
                        //打印份数,同序列打印的份数
                        btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                        
                        if (this.Re_Nocheckcode.Checked == false)
                        {
                            long Num2Imei14 = long.Parse(this.ReImeiNum2.Text.Substring(0, 14));
                            for (long Num1Imei14 = long.Parse(this.ReImeiNum1.Text.Substring(0, 14)); Num1Imei14 <= Num2Imei14; Num1Imei14++)
                            {
                                string Num1Imei15 = getimei15(Num1Imei14.ToString());
                                btFormat.SubStrings["IMEI"].Value = Num1Imei14.ToString() + Num1Imei15.ToString();
                                if (PMB.CheckReCHOrJSIMEIBLL(Num1Imei14.ToString() + Num1Imei15.ToString(), 1))
                                {
                                    list = PMB.SelectSnByIMEIBLL(Num1Imei14.ToString() + Num1Imei15.ToString());
                                    foreach (PrintMessage a in list)
                                    {
                                        btFormat.SubStrings["SN"].Value = a.SN;
                                    }
                                    //更新打印信息到数据表
                                    string RE_PrintTime = System.DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss:fff");
                                    if (PMB.UpdateRePrintBLL(Num1Imei14.ToString() + Num1Imei15.ToString(), RE_PrintTime, 1, lj, lj))
                                    {
                                        Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                        Form1.Log("批量重打了IMEI号为" + this.Re_IMEINum.Text + "的制单", null); 
                                    }
                                }
                                else
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.Re_IMEINum.Text+"无记录\r\n");
                                }
                            }
                            this.ReImeiNum1.Clear();
                            this.ReImeiNum2.Clear();
                            this.ReImeiNum1.Focus();
                        }
                        else {
                            long Num2Imei14 = long.Parse(this.ReImeiNum2.Text);
                            for (long Num1Imei14 = long.Parse(this.ReImeiNum1.Text); Num1Imei14 <= Num2Imei14; Num1Imei14++)
                            {
                                btFormat.SubStrings["IMEI"].Value = Num1Imei14.ToString();
                                if (PMB.CheckReCHOrJSIMEIBLL(Num1Imei14.ToString(), 1))
                                {
                                    list = PMB.SelectSnByIMEIBLL(Num1Imei14.ToString());
                                    foreach (PrintMessage a in list)
                                    {
                                        btFormat.SubStrings["SN"].Value = a.SN;
                                    }
                                    //更新打印信息到数据表
                                    string RE_PrintTime = System.DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss:fff");
                                    if (PMB.UpdateRePrintBLL(Num1Imei14.ToString(), RE_PrintTime, 1, lj, lj))
                                    {
                                        Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                        Form1.Log("批量重打了IMEI号为" + this.Re_IMEINum.Text + "的制单", null);
                                    }
                                }
                                else
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.Re_IMEINum.Text + "无记录\r\n");
                                }
                            }
                            this.ReImeiNum1.Clear();
                            this.ReImeiNum2.Clear();
                            this.ReImeiNum1.Focus();
                        }
                    }
                    else
                    {
                        player1.Play();
                        this.reminder.AppendText("请先选择模板\r\n");
                        this.ReImeiNum2.Clear();
                        this.ReImeiNum2.Focus();
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Exception:" + ex.Message);
                }
            }
        }

        private void Refresh_zhidan_Click(object sender, EventArgs e)
        {
            this.CB_ZhiDan.Items.Clear();
            G_MOP = MOPB.SelectZhidanNumBLL();
            foreach (Gps_ManuOrderParam a in G_MOP)
            {
                this.CB_ZhiDan.Items.Add(a.ZhiDan);
            }
            if (MOPB.CheckZhiDanBLL(this.CB_ZhiDan.Text))
            {
                CB_ZhiDan_SelectedIndexChanged(sender, e);
            }
            else
            {
                this.CB_ZhiDan.Text = "";
                this.Select_Template1.Clear();
                this.SoftModel.Clear();
                this.SN1_num.Clear();
                this.SN2_num.Clear();
                this.ProductNo.Clear();
                this.SoftwareVersion.Clear();
                this.IMEI_num1.Clear();
                this.IMEI_num2.Clear();
                this.IMEI_Present.Clear();
                this.SIM_num1.Clear();
                this.SIM_num2.Clear();
                this.BAT_num1.Clear();
                this.BAT_num2.Clear();
                this.VIP_num1.Clear();
                this.VIP_num2.Clear();
                this.Remake.Clear();
                this.IMEIRel.Clear();
                this.IMEI_Start.Clear();
                this.PrintNum.Clear();
                this.Re_IMEINum.Clear();
                this.ReImeiNum1.Clear();
                this.ReImeiNum2.Clear();
                this.SnFromCustomer.Checked = false;
                this.NoCheckCode.Checked = false;
                this.NoSn.Checked = false;
                this.Re_Nocheckcode.Checked = false;
                this.RePrintOne.Checked = false;
                this.RePrintMore.Checked = false;
                this.Re_IMEINum.ReadOnly = true;
                this.ReImeiNum1.ReadOnly = true;
                this.ReImeiNum2.ReadOnly = true;
            }
        }

        private void Unlock_Click(object sender, EventArgs e)
        {
            JS_Unlock ul = new JS_Unlock(this);
            ul.ShowDialog();
        }   

        public void Unlock_content()
        {
            this.Open_Template1.Enabled = true;
            this.Select_Template1.ReadOnly = false;
            this.Printer1.Enabled = true;
            this.CB_ZhiDan.Enabled = true;
            this.Open_file.Enabled = true;
            this.Debug_print.Enabled = true;
            this.Refresh_zhidan.Enabled = true;
            this.Refresh_template.Enabled = true;
            this.ToLock.Enabled = true;
            this.PrintOne.Enabled = true;
            this.PrintMore.Enabled = true;
            this.NoCheckCode.Enabled = true;
            this.SnFromCustomer.Enabled = true;
            this.NoSn.Enabled = true;
            this.RePrintOne.Enabled = true;
            this.RePrintMore.Enabled = true;
            this.Re_Nocheckcode.Enabled = true;
            this.ProductData.ReadOnly = false;
            this.TemplateNum.ReadOnly = false;
            if (this.PrintOne.Checked == true)
            {
                this.IMEI_Start.ReadOnly = false;
                this.IMEI_Start.Focus();
            }
            if (this.PrintMore.Checked == true)
            {
                this.PrintNum.ReadOnly = false;
                this.PrintNum.Focus();
            }
        }

        private void ToLock_Click(object sender, EventArgs e)
        {
            this.Open_Template1.Enabled = false;
            this.Select_Template1.ReadOnly = true;
            this.Printer1.Enabled = false;
            this.CB_ZhiDan.Enabled = false;
            this.Open_file.Enabled = false;
            this.Debug_print.Enabled = false;
            this.Refresh_template.Enabled = false;
            this.Refresh_zhidan.Enabled = false;
            this.ToLock.Enabled = false;
            this.PrintOne.Enabled = false;
            this.PrintMore.Enabled = false;
            this.NoCheckCode.Enabled = false;
            this.SnFromCustomer.Enabled = false;
            this.NoSn.Enabled = false;
            this.RePrintOne.Enabled = false;
            this.RePrintMore.Enabled = false;
            this.Re_Nocheckcode.Enabled = false;
            if (this.RePrintOne.Checked == true || this.RePrintMore.Checked == true)
            {
                this.IMEI_Start.ReadOnly = true;
                this.PrintNum.ReadOnly = true;
            }
            this.ProductData.ReadOnly = true;
            this.TemplateNum.ReadOnly = true;
            this.ToUnlock.Enabled = true;
        }

        private void TemplateNum_TextChanged(object sender, EventArgs e)
        {
            if (this.TemplateNum.Text != "")
            {
                if (IsNumeric(this.TemplateNum.Text))
                {
                    TN = int.Parse(this.TemplateNum.Text);
                }
                else
                {
                    this.reminder.AppendText("请输入数字\r\n");
                    this.TemplateNum.Clear();
                    this.TemplateNum.Focus();
                }
            }
        }

        private void TemplateNum_Leave(object sender, EventArgs e)
        {
            if (this.TemplateNum.Text == "")
            {
                 this.TemplateNum.Text=1.ToString();
            }
        }

        private void Open_file_Click(object sender, EventArgs e)
        {
            if (this.Select_Template1.Text == "")
            {
                player1.Play();
            }
            else
            {
                string path = this.Select_Template1.Text;
                if (File.Exists(path))
                {
                    System.Diagnostics.Process.Start(path);
                }
                else {
                    player.Play();
                }
            }
        }

        private void Refresh_template_Click(object sender, EventArgs e)
        {
            string path = this.Select_Template1.Text;
            string filename = Path.GetFileName(path);
            if (this.Select_Template1.Text != AppDomain.CurrentDomain.BaseDirectory + filename)
            {
                File.Copy(path, AppDomain.CurrentDomain.BaseDirectory + filename, true);
                this.Select_Template1.Text = AppDomain.CurrentDomain.BaseDirectory + filename;
            }
        }
    

    }
}
    