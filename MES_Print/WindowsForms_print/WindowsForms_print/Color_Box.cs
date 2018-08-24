using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Text;
using Seagull.BarTender.Print;
using System.Windows.Forms;
using Print_Message;
using Print.Message.Bll;
using ManuOrder.Param.BLL;
using TestResult.Param.BLL;
using DataRelative.Param.BLL;
using System.Runtime.InteropServices;
using System.Drawing.Printing;
using System.Text.RegularExpressions;
using System.Media;

namespace WindowsForms_print
{
    public partial class Color_Box : Form
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

        SoundPlayer player = new SoundPlayer(AppDomain.CurrentDomain.BaseDirectory + "失败.wav");
        SoundPlayer player1 = new SoundPlayer(AppDomain.CurrentDomain.BaseDirectory + "请先选择模板.wav");
        SoundPlayer player2 = new SoundPlayer(AppDomain.CurrentDomain.BaseDirectory + "请选择制单号.wav");
        SoundPlayer player3 = new SoundPlayer(AppDomain.CurrentDomain.BaseDirectory + "校验错误.wav");
        SoundPlayer player4 = new SoundPlayer(AppDomain.CurrentDomain.BaseDirectory + "成功.wav");
        SoundPlayer player5 = new SoundPlayer(AppDomain.CurrentDomain.BaseDirectory + "前测试位漏测.wav");

        string outString;

        ManuOrderParamBLL MOPB = new ManuOrderParamBLL();

        PrintMessageBLL PMB = new PrintMessageBLL();

        InputExcelBLL IEB = new InputExcelBLL();

        TestResultBLL TRB = new TestResultBLL();

        DataRelativeSheetBLL DRSB = new DataRelativeSheetBLL();

        List<Gps_ManuOrderParam> G_MOP = new List<Gps_ManuOrderParam>();

        List<PrintMessage> list = new List<PrintMessage>();

        List<DataRelativeSheet> drs = new List<DataRelativeSheet>();

        Engine btEngine = new Engine();
        LabelFormatDocument btFormat;

        //g1-g7依次对应关联SIM、VIP、BAT、ICCID、蓝牙号、设备号、
        int g=0, g1, g2, g3, g4, g5, g6;

        //h1-h4依次对应查功能，查耦合，查写好，查参数
        int h = 15, h1, h2, h3, h4;

        //lj为打印模板路径
        string lj = "";
        string lj2 = "";
        //设置打印模板份数
        int TN = 1;

        //dj为打印机路径
        string dj = "";
        string dj2 = "";
        //打印时间
        string ProductTime = "";
        //打印参数
        Messages messages;
        int waitout = 10000;
        //SN参数
        string sn1_prefix;
        long sn1_suffix;
        string sn1 ;
        string ASS_sn;
        int Sn_mark = 0;

        //记录SN号后缀位数
        int s;

        public Color_Box()
        {
            InitializeComponent();
            int wid = Screen.PrimaryScreen.WorkingArea.Width;
            this.Width = wid;
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

        private void Color_Box_Load(object sender, EventArgs e)
        {
            PrintDocument print = new PrintDocument();
            string sDefault = print.PrinterSettings.PrinterName;//默认打印机名
            this.Printer2.Text = sDefault;
            foreach (string sPrint in PrinterSettings.InstalledPrinters)//获取所有打印机名称
            {
                Printer2.Items.Add(sPrint);
            }
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
            this.ChangeInput.Text = "IMEI";
            string NowData = System.DateTime.Now.ToString("yyyy.MM.dd");
            this.PrintDate.Text = NowData;
            //开启打印机引擎
            btEngine.Start();
        }

        private void open_template1_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.ShowDialog();
            string path = ofd.FileName;
            string strExtension = "";
            int nIndex = path.LastIndexOf('.');
            if (nIndex > 0)
            {
                strExtension = path.Substring(nIndex);
                if (strExtension != ".btw")
                {
                    player.Play();
                    this.reminder.AppendText("请选择btw文件\r\n");
                }
                else
                {
                    this.Select_Template1.Text = path;
                }
            }
        }

        private void open_template2_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.ShowDialog();
            string path = ofd.FileName;
            string strExtension = "";
            int nIndex = path.LastIndexOf('.');
            if (nIndex > 0)
            {
                strExtension = path.Substring(nIndex);
                if (strExtension != ".btw")
                {
                    player.Play();
                    this.reminder.AppendText("请选择btw文件\r\n");
                }
                else
                {
                    this.Select_Template2.Text = path;
                }
            }
        }

        private void CB_ZhiDan_SelectedIndexChanged(object sender, EventArgs e)
        {
            ClearAll();
            h = 15;
            g = 0;
            string NowData = System.DateTime.Now.ToString("yyyy.MM.dd");
            this.PrintDate.Text = NowData;
            string ZhidanNum = this.CB_ZhiDan.Text;
            G_MOP = MOPB.selectManuOrderParamByzhidanBLL(ZhidanNum);
            foreach (Gps_ManuOrderParam b in G_MOP)
            {
                s = b.SN2.Length;
                this.SoftModel.Text = b.SoftModel;
                this.SN1_num.Text = b.SN1 + b.SN2;
                this.SN2_num.Text = b.SN1 + b.SN3;
                this.ProductDate.Text = b.ProductDate;
                this.Color.Text = b.Color;
                this.Weight.Text = b.Weight;
                this.ProductNo.Text = b.ProductNo;
                this.SoftwareVersion.Text = b.Version;
                this.IMEI_num1.Text = b.IMEIStart;
                this.IMEI_num2.Text = b.IMEIEnd;
                this.SIM_num1.Text = b.SIMStart;
                this.SIM_num2.Text = b.SIMEnd;
                this.SIM_digits.Text = b.SIM_digits;
                this.SIM_prefix.Text = b.SIM_prefix;
                this.BAT_num1.Text = b.BATStart;
                this.BAT_num2.Text = b.BATEnd;
                this.BAT_digits.Text = b.BAT_digits;
                this.BAT_prefix.Text = b.BAT_prefix;
                this.VIP_num1.Text = b.VIPStart;
                this.VIP_num2.Text = b.VIPEnd;
                this.VIP_digits.Text = b.VIP_digits;
                this.VIP_prefix.Text = b.VIP_prefix;
                this.ICCID_digits.Text = b.ICCID_digits;
                this.ICCID_prefix.Text = b.ICCID_prefix;
                this.MAC_digits.Text = b.MAC_digits;
                this.MAC_prefix.Text = b.MAC_prefix;
                this.Equipment_digits.Text = b.Equipment_digits;
                this.Equipment_prefix.Text = b.Equipment_prefix;
                this.Select_Template1.Text = b.CHT_template1;
                this.Select_Template2.Text = b.CHT_template2;
                this.PrintDate.Text = b.ProductDate;
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
                if (b.status == 0)
                {
                    this.SN1_num.ReadOnly = false;
                    this.SN2_num.ReadOnly = false;
                    this.ProductDate.ReadOnly = false;
                    this.VIP_num1.ReadOnly = false;
                    this.VIP_num2.ReadOnly = false;
                    this.VIP_digits.ReadOnly = false;
                    this.VIP_prefix.ReadOnly = false;
                    this.BAT_num1.ReadOnly = false;
                    this.BAT_num2.ReadOnly = false;
                    this.BAT_digits.ReadOnly = false;
                    this.BAT_prefix.ReadOnly = false;
                    this.SIM_num1.ReadOnly = false;
                    this.SIM_num2.ReadOnly = false;
                    this.SIM_digits.ReadOnly = false;
                    this.SIM_prefix.ReadOnly = false;
                    this.ICCID_digits.ReadOnly = false;
                    this.ICCID_prefix.ReadOnly = false;
                    this.MAC_digits.ReadOnly = false;
                    this.MAC_prefix.ReadOnly = false;
                    this.Equipment_digits.ReadOnly = false;
                    this.Equipment_prefix.ReadOnly = false;
                    this.updata_inline.Visible = true;
                }
                else
                {
                    this.SN1_num.ReadOnly = true;
                    this.SN2_num.ReadOnly = true;
                    this.ProductDate.ReadOnly = true;
                    this.VIP_num1.ReadOnly = true;
                    this.VIP_num2.ReadOnly = true;
                    this.VIP_digits.ReadOnly = true;
                    this.VIP_prefix.ReadOnly = true;
                    this.BAT_num1.ReadOnly = true;
                    this.BAT_num2.ReadOnly = true;
                    this.BAT_digits.ReadOnly = true;
                    this.BAT_prefix.ReadOnly = true;
                    this.SIM_num1.ReadOnly = true;
                    this.SIM_num2.ReadOnly = true;
                    this.SIM_digits.ReadOnly = true;
                    this.SIM_prefix.ReadOnly = true;
                    this.ICCID_digits.ReadOnly = true;
                    this.ICCID_prefix.ReadOnly = true;
                    this.MAC_digits.ReadOnly = true;
                    this.MAC_prefix.ReadOnly = true;
                    this.Equipment_digits.ReadOnly = true;
                    this.Equipment_prefix.ReadOnly = true;
                    this.updata_inline.Visible = false;
                }
            }

            list = PMB.SelectPrintMesByZhiDanBLL(this.CB_ZhiDan.Text);
            foreach (PrintMessage a in list)
            {
                if (list.Count != 0)
                {
                    if (a.SIM != "")
                    {
                        this.choose_sim.Checked = true;
                        this.SIMStart.ReadOnly = false;
                        g = g + 1;
                    }
                    if (a.VIP != "")
                    {
                        this.choose_vip.Checked = true;
                        this.VIPStart.ReadOnly = false;
                        g = g + 2;
                    }
                    if (a.BAT != "")
                    {
                        this.choose_bat.Checked = true;
                        this.BATStart.ReadOnly = false;
                        g = g + 4;
                    }
                    if (a.MAC != "")
                    {
                        this.choose_mac.Checked = true;
                        this.MACStart.ReadOnly = false;
                        g = g + 16;
                    }
                    if (a.Equipment != "")
                    {
                        this.choose_Equipment.Checked = true;
                        this.EquipmentStart.ReadOnly = false;
                        g = g + 32;
                    }
                }
                else
                {
                    this.choose_sim.Checked = false;
                    this.choose_vip.Checked = false;
                    this.choose_bat.Checked = false;
                    this.choose_iccid.Checked = false;
                    this.choose_mac.Checked = false;
                    this.choose_Equipment.Checked = false;
                }
            }
        }

        private void ClearAll()
        {
            this.reminder.Text = "";
            this.IMEI_Start.Clear();
            this.IMEI_Start.Focus();
            this.Re_IMEINum.Clear();
            this.SIMStart.Clear();
            this.VIPStart.Clear();
            this.BATStart.Clear();
            this.MACStart.Clear();
            this.ICCIDStart.Clear();
            this.EquipmentStart.Clear();
            this.choose_sim.Checked = false;
            this.choose_vip.Checked = false;
            this.choose_bat.Checked = false;
            this.choose_iccid.Checked = false;
            this.choose_mac.Checked = false;
            this.choose_Equipment.Checked = false;
            this.NoPaper.Checked = false;
            this.NoCheckCode.Checked = false;
            this.SIMStart.ReadOnly = true;
            this.VIPStart.ReadOnly = true;
            this.BATStart.ReadOnly = true;
            this.ICCIDStart.ReadOnly = true;
            this.MACStart.ReadOnly = true;
            this.EquipmentStart.ReadOnly = true;
            this.AutoTest.Checked = true;
            this.WriteImei.Checked = true;
            this.ParamDownload.Checked = true;
            this.Couple.Checked = true;
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

        private void PrintDate_Leave(object sender, EventArgs e)
        {
            if (this.PrintDate.Text != "")
            {
                 if (!IsDate(this.PrintDate.Text))
                {
                    MessageBox.Show("请输入正确的日期格式");
                    this.PrintDate.Text = System.DateTime.Now.ToString("yyyy.MM.dd");
                }
            }
        }

        public static bool HasChinese(string str)
        {
            return Regex.IsMatch(str, @"[\u4e00-\u9fa5]");
        }

        private void VIP_num1_Leave(object sender, EventArgs e)
        {
            if (this.VIP_num1.Text != "")
            {
                if (HasChinese(this.VIP_num1.Text))
                {
                    player.Play();
                    this.VIP_num1.Clear();
                }
            }
        }

        private void VIP_num2_Leave(object sender, EventArgs e)
        {
            if (this.VIP_num2.Text != "")
            {
                if (HasChinese(this.VIP_num2.Text))
                {
                    player.Play();
                    this.VIP_num2.Clear();
                }
            }
        }

        private void Debug_print_Click(object sender, EventArgs e)
        {
            try
            {
                if (this.Select_Template1.Text != "" || this.Select_Template2.Text != "")
                {

                    if (this.Select_Template2.Text == "")
                    {
                        lj = this.Select_Template1.Text;
                        dj = this.Printer1.Text;
                    }
                    else
                    {
                        lj = this.Select_Template1.Text;
                        dj = this.Printer1.Text;
                        lj2 = this.Select_Template2.Text;
                        dj2 = this.Printer2.Text;
                    }
                    LabelFormatDocument btFormat = btEngine.Documents.Open(lj);
                    
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
                    GetValue("Information", "电池号", out outString);
                    btFormat.SubStrings[outString].Value = this.BAT_num1.Text;
                    GetValue("Information", "备注", out outString);
                    btFormat.SubStrings[outString].Value = this.Remake.Text;
                    GetValue("Information", "生产日期", out outString);
                    btFormat.SubStrings[outString].Value = this.PrintDate.Text;
                    //打印份数,同序列打印的份数
                    btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                    Messages messages;
                    int waitout = 10000;
                    //指定打印机名称
                    btFormat.PrintSetup.PrinterName = dj;
                    btFormat.Print("标签打印软件", waitout, out messages);
                    if (dj2 != "")
                    {
                        btFormat = btEngine.Documents.Open(lj2);
                        btFormat.PrintSetup.PrinterName = dj2;
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
                        GetValue("Information", "电池号", out outString);
                        btFormat.SubStrings[outString].Value = this.BAT_num1.Text;
                        GetValue("Information", "备注", out outString);
                        btFormat.SubStrings[outString].Value = this.Remake.Text;
                        GetValue("Information", "生产日期", out outString);
                        btFormat.SubStrings[outString].Value = this.PrintDate.Text;
                        //打印份数,同序列打印的份数
                        btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                        btFormat.Print("标签打印软件", waitout, out messages);
                    }
                    Form1.Log("调试打印了机身贴IMEI号为" + this.IMEI_num1.Text + "的制单", null);
                }
                
            }
            catch (Exception ex)
            {
                MessageBox.Show("Exception:" + ex.Message);
            }
        }

        private void choose_sim_Click(object sender, EventArgs e)
        {
            if (this.choose_sim.Checked == true)
            {
                this.SIMStart.ReadOnly = false;
                this.IMEI_Start.Clear();
                this.SIMStart.Clear();
                this.VIPStart.Clear();
                this.BATStart.Clear();
                this.ICCIDStart.Clear();
                this.MACStart.Clear();
                this.EquipmentStart.Clear();
                this.IMEI_Start.Focus();
                g1 = 1;
                g = g + g1;
            }
            else {
                this.SIMStart.ReadOnly = true;
                this.IMEI_Start.Clear();
                this.SIMStart.Clear();
                this.VIPStart.Clear();
                this.BATStart.Clear();
                this.ICCIDStart.Clear();
                this.MACStart.Clear();
                this.EquipmentStart.Clear();
                this.IMEI_Start.Focus();
                g1 = 0;
                g = g - 1;
                if (g == 0) {
                    this.NoPaper.Checked = false;
                }
            }
        }

        private void choose_vip_Click(object sender, EventArgs e)
        {
            if (this.choose_vip.Checked == true)
            {
                this.VIPStart.ReadOnly = false;
                this.IMEI_Start.Clear();
                this.SIMStart.Clear();
                this.VIPStart.Clear();
                this.BATStart.Clear();
                this.ICCIDStart.Clear();
                this.MACStart.Clear();
                this.EquipmentStart.Clear();
                this.IMEI_Start.Focus();
                g2 = 2;
                g = g + g2;
            }
            else
            {
                this.VIPStart.ReadOnly = true;
                this.IMEI_Start.Clear();
                this.SIMStart.Clear();
                this.VIPStart.Clear();
                this.BATStart.Clear();
                this.ICCIDStart.Clear();
                this.MACStart.Clear();
                this.EquipmentStart.Clear();
                this.IMEI_Start.Focus();
                g2 = 0;
                g = g - 2;
                if (g == 0)
                {
                    this.NoPaper.Checked = false;
                }
            }
        }

        private void choose_bat_Click(object sender, EventArgs e)
        {
            if (this.choose_bat.Checked == true)
            {
                this.BATStart.ReadOnly = false;
                this.IMEI_Start.Clear();
                this.SIMStart.Clear();
                this.VIPStart.Clear();
                this.BATStart.Clear();
                this.ICCIDStart.Clear();
                this.MACStart.Clear();
                this.EquipmentStart.Clear();
                this.IMEI_Start.Focus();
                g3 = 4;
                g = g + g3;
            }
            else
            {
                this.BATStart.ReadOnly = true;
                this.IMEI_Start.Clear();
                this.SIMStart.Clear();
                this.VIPStart.Clear();
                this.BATStart.Clear();
                this.ICCIDStart.Clear();
                this.MACStart.Clear();
                this.EquipmentStart.Clear();
                this.IMEI_Start.Focus();
                g3 = 0;
                g = g - 4;
                if (g == 0)
                {
                    this.NoPaper.Checked = false;
                }
            }
        }

        private void choose_iccid_Click(object sender, EventArgs e)
        {
            if (this.choose_iccid.Checked == true)
            {
                this.ICCIDStart.ReadOnly = false;
                this.IMEI_Start.Clear();
                this.SIMStart.Clear();
                this.VIPStart.Clear();
                this.BATStart.Clear();
                this.ICCIDStart.Clear();
                this.MACStart.Clear();
                this.EquipmentStart.Clear();
                this.IMEI_Start.Focus();
                g4 = 8;
                g = g + g4;
            }
            else {
                this.ICCIDStart.ReadOnly = true;
                this.IMEI_Start.Clear();
                this.SIMStart.Clear();
                this.VIPStart.Clear();
                this.BATStart.Clear();
                this.ICCIDStart.Clear();
                this.MACStart.Clear();
                this.EquipmentStart.Clear();
                this.IMEI_Start.Focus();
                g4 = 0;
                g = g - 8;
                if (g == 0)
                {
                    this.NoPaper.Checked = false;
                }
            }
        }

        private void choose_mac_Click(object sender, EventArgs e)
        {
            if (this.choose_mac.Checked == true)
            {
                this.MACStart.ReadOnly = false;
                this.IMEI_Start.Clear();
                this.SIMStart.Clear();
                this.VIPStart.Clear();
                this.BATStart.Clear();
                this.ICCIDStart.Clear();
                this.MACStart.Clear();
                this.EquipmentStart.Clear();
                this.IMEI_Start.Focus();
                g5 = 16;
                g = g + g5;
            }
            else
            {
                this.MACStart.ReadOnly = true;
                this.IMEI_Start.Clear();
                this.SIMStart.Clear();
                this.VIPStart.Clear();
                this.BATStart.Clear();
                this.ICCIDStart.Clear();
                this.MACStart.Clear();
                this.EquipmentStart.Clear();
                this.IMEI_Start.Focus();
                g5 = 0;
                g = g - 16;
                if (g == 0)
                {
                    this.NoPaper.Checked = false;
                }
            }
        }

        private void choose_Equipment_Click(object sender, EventArgs e)
        {
            if (this.choose_Equipment.Checked == true)
            {
                this.EquipmentStart.ReadOnly = false;
                this.IMEI_Start.Clear();
                this.SIMStart.Clear();
                this.VIPStart.Clear();
                this.BATStart.Clear();
                this.ICCIDStart.Clear();
                this.MACStart.Clear();
                this.EquipmentStart.Clear();
                this.IMEI_Start.Focus();
                g6 = 32;
                g = g + g6;
            }
            else
            {
                this.EquipmentStart.ReadOnly = true;
                this.IMEI_Start.Clear();
                this.SIMStart.Clear();
                this.VIPStart.Clear();
                this.BATStart.Clear();
                this.ICCIDStart.Clear();
                this.MACStart.Clear();
                this.EquipmentStart.Clear();
                this.IMEI_Start.Focus();
                g6 = 0;
                g = g - 32;
                if (g == 0)
                {
                    this.NoPaper.Checked = false;
                }
            }
        }

        private void choose_reprint_Click(object sender, EventArgs e)
        {
            if (this.choose_reprint.Checked == true)
            {
                this.Re_IMEINum.ReadOnly = false;
                this.IMEI_Start.ReadOnly = true;
                this.Re_IMEINum.Focus();
                this.Re_Tem1.Visible = true;
                this.Re_Tem2.Visible = true;
                if (this.choose_sim.Checked == true)
                {
                    this.choose_sim.Enabled = false;
                    this.SIMStart.ReadOnly = true;
                }
                if (this.choose_vip.Checked == true)
                {
                    this.choose_vip.Enabled = false;
                    this.VIPStart.ReadOnly = true;
                }
                if (this.choose_bat.Checked == true)
                {
                    this.choose_bat.Enabled = false;
                    this.BATStart.ReadOnly = true;
                }
                if (this.choose_mac.Checked == true)
                {
                    this.choose_mac.Enabled = false;
                    this.MACStart.ReadOnly = true;
                }
                if (this.choose_Equipment.Checked == true)
                {
                    this.choose_Equipment.Enabled = false;
                    this.EquipmentStart.ReadOnly = true;
                }
            }
            else {
                this.Re_IMEINum.ReadOnly = true;
                this.IMEI_Start.ReadOnly = false;
                this.IMEI_Start.Focus();
                this.Re_Tem1.Visible = false;
                this.Re_Tem2.Visible = false;
                if (this.choose_sim.Checked == true)
                {
                    this.choose_sim.Enabled = true;
                    this.SIMStart.ReadOnly = false;
                }
                if (this.choose_vip.Checked == true)
                {
                    this.choose_vip.Enabled = true;
                    this.VIPStart.ReadOnly = false;
                }
                if (this.choose_bat.Checked == true)
                {
                    this.choose_bat.Enabled = true;
                    this.BATStart.ReadOnly = false;
                }
                if (this.choose_mac.Checked == true)
                {
                    this.choose_mac.Enabled = true;
                    this.MACStart.ReadOnly = false;
                }
                if (this.choose_Equipment.Checked == true)
                {
                    this.choose_Equipment.Enabled = true;
                    this.EquipmentStart.ReadOnly = false;
                }
            }
        }

        private void NoPaper_Click(object sender, EventArgs e)
        {
            if (this.NoPaper.Checked == true)
            {
                if (g == 0) {
                    this.NoPaper.Checked = false;
                    player.Play();
                }
                this.IMEI_Start.Clear();
                this.SIMStart.Clear();
                this.VIPStart.Clear();
                this.BATStart.Clear();
                this.ICCIDStart.Clear();
                this.MACStart.Clear();
                this.EquipmentStart.Clear();
                this.IMEI_Start.Focus();
                //g7 = 64;
                //g = g + g7;
            }
            else
            {
                this.IMEI_Start.Clear();
                this.SIMStart.Clear();
                this.VIPStart.Clear();
                this.BATStart.Clear();
                this.ICCIDStart.Clear();
                this.MACStart.Clear();
                this.EquipmentStart.Clear();
                this.IMEI_Start.Focus();
                //g7 = 0;
                //g = g - 64;
            }
            
        }

        private void Re_IMEINum_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                if (this.Re_IMEINum.Text != "")
                {
                    if (this.ChangeInput.Text != "IMEI")
                    {
                        this.Re_IMEINum.Text = DRSB.SelectIMEIBySnOrIMEI2BLL(this.ChangeInput.Text, this.Re_IMEINum.Text);
                        if (this.Re_IMEINum.Text == "")
                        {
                            this.reminder.AppendText("找不到对应IMEI\r\n");
                            this.Re_IMEINum.Focus();
                            return;
                        }
                    }
                }
                else
                {
                    this.Re_IMEINum.Focus();
                    return;
                }
                try
                {
                    if (this.Re_Tem1.Checked == true || this.Re_Tem2.Checked == true)
                    {
                        if (PMB.CheckReCHOrJSIMEIBLL(this.Re_IMEINum.Text, 2))
                        {
                            string RE_PrintTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                            if (this.Re_Tem1.Checked == true)
                            {
                                if (this.Select_Template1.Text != "")
                                {
                                    LabelFormatDocument btFormat = btEngine.Documents.Open(this.Select_Template1.Text);
                                    //指定打印机名称
                                    btFormat.PrintSetup.PrinterName = this.Printer1.Text;
                                    //对模板相应字段进行赋值
                                    GetValue("Information", "生产日期", out outString);
                                    btFormat.SubStrings[outString].Value = this.PrintDate.Text;
                                    btFormat.SubStrings["IMEI"].Value = this.Re_IMEINum.Text;
                                    //打印份数,同序列打印的份数
                                    btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                                    list = PMB.SelectSnByIMEIBLL(this.Re_IMEINum.Text);
                                    foreach (PrintMessage a in list)
                                    {
                                        btFormat.SubStrings["SIM"].Value = a.SIM;
                                        btFormat.SubStrings["VIP"].Value = a.VIP;
                                        btFormat.SubStrings["BAT"].Value = a.BAT;
                                        btFormat.SubStrings["ICCID"].Value = a.ICCID;
                                        btFormat.SubStrings["MAC"].Value = a.MAC;
                                        btFormat.SubStrings["Equipment"].Value = a.Equipment;
                                        btFormat.SubStrings["SN"].Value = a.SN;
                                    }
                                    Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                    //更新打印信息到数据表
                                    if (PMB.UpdateRePrintBLL(this.Re_IMEINum.Text, RE_PrintTime, 2, this.Select_Template1.Text, this.Select_Template2.Text))
                                    {
                                        Form1.Log("重打了IMEI号为" + this.Re_IMEINum.Text + "的制单", null);
                                    }
                                }
                                else {
                                    player.Play();
                                    this.reminder.AppendText("请先选择模板1\r\n");
                                }
                            }
                            if(this.Re_Tem2.Checked == true)
                            {
                                if (this.Select_Template2.Text != "")
                                {
                                    LabelFormatDocument btFormat = btEngine.Documents.Open(this.Select_Template2.Text);
                                    //指定打印机名称
                                    btFormat.PrintSetup.PrinterName = this.Printer2.Text;
                                    //对模板相应字段进行赋值
                                    GetValue("Information", "生产日期", out outString);
                                    btFormat.SubStrings[outString].Value = this.PrintDate.Text;
                                    btFormat.SubStrings["IMEI"].Value = this.Re_IMEINum.Text;
                                    //打印份数,同序列打印的份数
                                    btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                                    list = PMB.SelectSnByIMEIBLL(this.Re_IMEINum.Text);
                                    foreach (PrintMessage a in list)
                                    {
                                        btFormat.SubStrings["SIM"].Value = a.SIM;
                                        btFormat.SubStrings["VIP"].Value = a.VIP;
                                        btFormat.SubStrings["BAT"].Value = a.BAT;
                                        btFormat.SubStrings["ICCID"].Value = a.ICCID;
                                        btFormat.SubStrings["MAC"].Value = a.MAC;
                                        btFormat.SubStrings["Equipment"].Value = a.Equipment;
                                        btFormat.SubStrings["SN"].Value = a.SN;
                                    }
                                    Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                    //更新打印信息到数据表
                                    if (PMB.UpdateRePrintBLL(this.Re_IMEINum.Text, RE_PrintTime, 2, this.Select_Template1.Text, this.Select_Template2.Text))
                                    {
                                        Form1.Log("重打了IMEI号为" + this.Re_IMEINum.Text + "的制单", null);
                                    }
                                }
                                else
                                {
                                    player.Play();
                                    this.reminder.AppendText("请先选择模板2\r\n");
                                }
                            }
                            this.Re_IMEINum.Clear();
                            this.Re_IMEINum.Focus();
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

        private void IMEI_Start_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                if (this.CB_ZhiDan.Text != "")
                {
                    if (this.ChangeInput.Text != "IMEI")
                    {
                        this.IMEI_Start.Text = DRSB.SelectIMEIBySnOrIMEI2BLL(this.ChangeInput.Text, this.IMEI_Start.Text);
                        if (this.IMEI_Start.Text == "")
                        {
                            this.reminder.AppendText("找不到对应IMEI\r\n");
                            this.IMEI_Start.Focus();
                            return;
                        }
                    }
                    if (this.NoCheckCode.Checked == false)
                    {
                        string imei14;
                        String imeiRes = "";
                        if (IsNumeric(this.IMEI_Start.Text) && this.IMEI_Start.Text.Length == 15)
                        {
                            imei14 = this.IMEI_Start.Text.Substring(0, 14);
                            long IMEI_Start = long.Parse(imei14);
                            if (IMEI_Start < long.Parse(this.IMEI_num1.Text) || IMEI_Start > long.Parse(this.IMEI_num2.Text))
                            {
                                player.Play();
                                this.reminder.AppendText(this.IMEI_Start.Text+"不在范围内\r\n");
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
                                    this.reminder.AppendText(this.IMEI_Start.Text + "校验错误\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                    return;
                                }
                            }
                        }
                        else if (this.IMEI_Start.Text == "")
                        {
                            player.Play();
                            this.reminder.AppendText("请输入IMEI");
                            this.IMEI_Start.Focus();
                            return;
                        }
                        else
                        {
                            player.Play();
                            this.reminder.AppendText("IMEI格式错误");
                            this.IMEI_Start.Clear();
                            this.IMEI_Start.Focus();
                            return;
                        }
                    }
                    else {
                        if (IsNumeric(this.IMEI_Start.Text) )
                        {
                            long IMEI_Start = long.Parse(this.IMEI_Start.Text);
                            if (IMEI_Start < long.Parse(this.IMEI_num1.Text) || IMEI_Start > long.Parse(this.IMEI_num2.Text))
                            {
                                player.Play();
                                this.reminder.AppendText(this.IMEI_Start.Text + "IMEI不在范围内\r\n");
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
                            this.reminder.AppendText(this.IMEI_Start.Text + "格式错误\r\n");
                            this.IMEI_Start.Clear();
                            this.IMEI_Start.Focus();
                            return;
                        }
                    }
                }
                else
                {
                    player2.Play();
                    this.reminder.AppendText("请先选择制单\r\n");
                    this.IMEI_Start.Clear();
                    this.IMEI_Start.Focus();
                    return;
                }
                switch (h)
                {
                    case 1:
                        {
                            if (!TRB.CheckOneBefStationBLL(this.IMEI_Start.Text, "AutoTestResult"))
                            {
                                player5.Play();
                                this.reminder.AppendText(this.IMEI_Start.Text + "漏测\r\n");
                                this.IMEI_Start.Clear();
                                this.IMEI_Start.Focus();
                                return;
                            }
                        }
                        break;
                    case 2:
                        {
                            if (!TRB.CheckOneBefStationBLL(this.IMEI_Start.Text, "CoupleResult"))
                            {
                                player5.Play();
                                this.reminder.AppendText(this.IMEI_Start.Text + "漏测\r\n");
                                this.IMEI_Start.Clear();
                                this.IMEI_Start.Focus();
                                return;
                            }
                        }
                        break;
                    case 4:
                        {
                            if (!TRB.CheckOneBefStationBLL(this.IMEI_Start.Text, "WriteImeiResult"))
                            {
                                player5.Play();
                                this.reminder.AppendText(this.IMEI_Start.Text + "漏测\r\n");
                                this.IMEI_Start.Clear();
                                this.IMEI_Start.Focus();
                                return;
                            }
                        }
                        break;
                    case 8:
                        {
                            if (!TRB.CheckOneBefStationBLL(this.IMEI_Start.Text, "ParamDownloadResult"))
                            {
                                player5.Play();
                                this.reminder.AppendText(this.IMEI_Start.Text + "漏测\r\n");
                                this.IMEI_Start.Clear();
                                this.IMEI_Start.Focus();
                                return;
                            }
                        }
                        break;
                    case 3:
                        {
                            if (!TRB.CheckTwoBefStationBLL(this.IMEI_Start.Text, "AutoTestResult", "CoupleResult"))
                            {
                                player5.Play();
                                this.reminder.AppendText(this.IMEI_Start.Text + "漏测\r\n");
                                this.IMEI_Start.Clear();
                                this.IMEI_Start.Focus();
                                return;
                            }
                        }
                        break;
                    case 5:
                        {
                            if (!TRB.CheckTwoBefStationBLL(this.IMEI_Start.Text, "AutoTestResult", "WriteImeiResult"))
                            {
                                player5.Play();
                                this.reminder.AppendText(this.IMEI_Start.Text + "漏测\r\n");
                                this.IMEI_Start.Clear();
                                this.IMEI_Start.Focus();
                                return;
                            }
                        }
                        break;
                    case 9:
                        {
                            if (!TRB.CheckTwoBefStationBLL(this.IMEI_Start.Text, "AutoTestResult", "ParamDownloadResult"))
                            {
                                player5.Play();
                                this.reminder.AppendText(this.IMEI_Start.Text + "漏测\r\n");
                                this.IMEI_Start.Clear();
                                this.IMEI_Start.Focus();
                                return;
                            }
                        }
                        break;
                    case 6:
                        {
                            if (!TRB.CheckTwoBefStationBLL(this.IMEI_Start.Text, "CoupleResult", "WriteImeiResult"))
                            {
                                player5.Play();
                                this.reminder.AppendText(this.IMEI_Start.Text + "漏测\r\n");
                                this.IMEI_Start.Clear();
                                this.IMEI_Start.Focus();
                                return;
                            }
                        }
                        break;
                    case 10:
                        {
                            if (!TRB.CheckTwoBefStationBLL(this.IMEI_Start.Text, "CoupleResult", "ParamDownloadResult"))
                            {
                                player5.Play();
                                this.reminder.AppendText(this.IMEI_Start.Text + "漏测\r\n");
                                this.IMEI_Start.Clear();
                                this.IMEI_Start.Focus();
                                return;
                            }
                        }
                        break;
                    case 12:
                        {
                            if (!TRB.CheckTwoBefStationBLL(this.IMEI_Start.Text, "WriteImeiResult", "ParamDownloadResult"))
                            {
                                player5.Play();
                                this.reminder.AppendText(this.IMEI_Start.Text + "漏测\r\n");
                                this.IMEI_Start.Clear();
                                this.IMEI_Start.Focus();
                                return;
                            }
                        }
                        break;
                    case 7:
                        {
                            if (!TRB.CheckThreeBefStationBLL(this.IMEI_Start.Text, "AutoTestResult",  "CoupleResult", "WriteImeiResult"))
                            {
                                player5.Play();
                                this.reminder.AppendText(this.IMEI_Start.Text + "漏测\r\n");
                                this.IMEI_Start.Clear();
                                this.IMEI_Start.Focus();
                                return;
                            }
                        }
                        break;
                    case 11:
                        {
                            if (!TRB.CheckThreeBefStationBLL(this.IMEI_Start.Text, "AutoTestResult", "CoupleResult", "ParamDownloadResult"))
                            {
                                player5.Play();
                                this.reminder.AppendText(this.IMEI_Start.Text + "漏测\r\n");
                                this.IMEI_Start.Clear();
                                this.IMEI_Start.Focus();
                                return;
                            }
                        }
                        break;
                    case 13:
                        {
                            if (!TRB.CheckThreeBefStationBLL(this.IMEI_Start.Text, "AutoTestResult", "WriteImeiResult", "ParamDownloadResult"))
                            {
                                player5.Play();
                                this.reminder.AppendText(this.IMEI_Start.Text + "漏测\r\n");
                                this.IMEI_Start.Clear();
                                this.IMEI_Start.Focus();
                                return;
                            }
                        }
                        break;
                    case 14:
                        {
                            if (!TRB.CheckThreeBefStationBLL(this.IMEI_Start.Text, "CoupleResult", "WriteImeiResult", "ParamDownloadResult"))
                            {
                                player5.Play();
                                this.reminder.AppendText(this.IMEI_Start.Text + "漏测\r\n");
                                this.IMEI_Start.Clear();
                                this.IMEI_Start.Focus();
                                return;
                            }
                        }
                        break;
                    case 15:
                        {
                            if (!TRB.CheckFourBefStationBLL(this.IMEI_Start.Text, "AutoTestResult", "CoupleResult", "WriteImeiResult", "ParamDownloadResult"))
                            {
                                player5.Play();
                                this.reminder.AppendText(this.IMEI_Start.Text + "漏测\r\n");
                                this.IMEI_Start.Clear();
                                this.IMEI_Start.Focus();
                                return;
                            }
                        }
                        break;
                }
                switch (g)
                {
                    case 0:
                        {
                            try
                            {
                                if (this.Select_Template1.Text != "" || this.Select_Template2.Text != "")
                                {
                                    ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                    if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                    {
                                        drs = DRSB.SelectByIMEIBLL(this.IMEI_Start.Text);
                                        if (drs.Count != 0)
                                        {
                                            foreach (DataRelativeSheet a in drs)
                                            {
                                                this.SIMStart.Text = a.IMEI3;
                                                this.ICCIDStart.Text = a.IMEI4;
                                                this.MACStart.Text = a.IMEI6;
                                                this.EquipmentStart.Text = a.IMEI7;
                                                this.VIPStart.Text = a.IMEI8;
                                                this.BATStart.Text = a.IMEI9;
                                                if (a.IMEI2 != "")
                                                {
                                                    ASS_sn = a.IMEI2;
                                                }
                                                else
                                                {
                                                    ASS_sn = this.SN1_num.Text;
                                                    Sn_mark = 1;
                                                }
                                            }
                                        }
                                        for (int S = 1; S <= 2; S++)
                                        {
                                            if (S == 1)
                                            {
                                                lj = this.Select_Template1.Text;
                                                btFormat = btEngine.Documents.Open(lj);
                                                //指定打印机名称
                                                btFormat.PrintSetup.PrinterName = this.Printer1.Text;
                                            }
                                            else
                                            {
                                                if (this.Select_Template2.Text == "")
                                                {
                                                    //记录打印信息日志
                                                    list.Add(new PrintMessage()
                                                    {
                                                        Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                        IMEI = this.IMEI_Start.Text.Trim(),
                                                        IMEIStart = this.IMEI_num1.Text.Trim(),
                                                        IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                        SN = ASS_sn,
                                                        IMEIRel = this.IMEIRel.Text.Trim(),
                                                        SIM = this.SIMStart.Text.Trim(),
                                                        VIP = this.VIPStart.Text.Trim(),
                                                        BAT = this.BATStart.Text.Trim(),
                                                        SoftModel = this.SoftModel.Text.Trim(),
                                                        Version = this.SoftwareVersion.Text.Trim(),
                                                        Remark = this.Remake.Text.Trim(),
                                                        JS_PrintTime = "",
                                                        JS_TemplatePath = "",
                                                        CH_PrintTime = ProductTime,
                                                        CH_TemplatePath1 = this.Select_Template1.Text,
                                                        CH_TemplatePath2 = this.Select_Template2.Text,
                                                        ICCID = this.ICCIDStart.Text,
                                                        MAC = this.MACStart.Text,
                                                        Equipment = this.EquipmentStart.Text
                                                    });
                                                    if (PMB.InsertPrintMessageBLL(list))
                                                    {
                                                        if (SN1_num.Text != "" && Sn_mark == 1)
                                                        {
                                                            sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                            sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                            sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                            string sn2_suffix = SN2_num.Text.Remove(0, (this.SN2_num.Text.Length) - s);
                                                            if (MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0')))
                                                            {
                                                                this.SN1_num.Text = sn1;
                                                                this.IMEI_Start.Clear();
                                                                this.SIMStart.Clear();
                                                                this.VIPStart.Clear();
                                                                this.BATStart.Clear();
                                                                this.ICCIDStart.Clear();
                                                                this.MACStart.Clear();
                                                                this.EquipmentStart.Clear();
                                                                this.ShowSN.Clear();
                                                                this.IMEI_Start.Focus();
                                                            }
                                                        }
                                                        else
                                                        {
                                                            Form1.Log("打印了彩盒贴IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                                            this.IMEI_Start.Clear();
                                                            this.SIMStart.Clear();
                                                            this.VIPStart.Clear();
                                                            this.BATStart.Clear();
                                                            this.ICCIDStart.Clear();
                                                            this.MACStart.Clear();
                                                            this.EquipmentStart.Clear();
                                                            this.IMEI_Start.Focus();
                                                        }
                                                    }
                                                    if (this.updata_inline.Visible == true)
                                                    {
                                                        MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                                        statusChange();
                                                    }
                                                    return;
                                                }
                                                else
                                                {
                                                    lj = this.Select_Template2.Text;
                                                    btFormat = btEngine.Documents.Open(lj);
                                                    //指定打印机名称
                                                    btFormat.PrintSetup.PrinterName = this.Printer2.Text;
                                                }
                                            }
                                            //对模板相应字段进行赋值;
                                            ValuesToTemplate(btFormat);
                                            btFormat.SubStrings["IMEI"].Value = this.IMEI_Start.Text;
                                            btFormat.SubStrings["SN"].Value = ASS_sn;
                                            this.ShowSN.Text = ASS_sn;
                                            //打印份数,同序列打印的份数
                                            btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                                            Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                            Form1.Log("打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                        }
                                        //记录打印信息日志
                                        list.Add(new PrintMessage()
                                        {
                                            Zhidan = this.CB_ZhiDan.Text.Trim(),
                                            IMEI = this.IMEI_Start.Text.Trim(),
                                            IMEIStart = this.IMEI_num1.Text.Trim(),
                                            IMEIEnd = this.IMEI_num2.Text.Trim(),
                                            SN = ASS_sn,
                                            IMEIRel = this.IMEIRel.Text.Trim(),
                                            SIM = this.SIMStart.Text.Trim(),
                                            VIP = this.VIPStart.Text.Trim(),
                                            BAT = this.BATStart.Text.Trim(),
                                            SoftModel = this.SoftModel.Text.Trim(),
                                            Version = this.SoftwareVersion.Text.Trim(),
                                            Remark = this.Remake.Text.Trim(),
                                            JS_PrintTime = "",
                                            JS_TemplatePath = "",
                                            CH_PrintTime = ProductTime,
                                            CH_TemplatePath1 = this.Select_Template1.Text,
                                            CH_TemplatePath2 = this.Select_Template2.Text,
                                            ICCID = this.ICCIDStart.Text,
                                            MAC = this.MACStart.Text,
                                            Equipment = this.EquipmentStart.Text
                                        });
                                        if (PMB.InsertPrintMessageBLL(list))
                                        {
                                            if (SN1_num.Text != "" && Sn_mark == 1)
                                            {
                                                sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                string sn2_suffix = SN2_num.Text.Remove(0, (this.SN2_num.Text.Length) - s);
                                                if (MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0')))
                                                {
                                                    this.SN1_num.Text = sn1;
                                                    this.IMEI_Start.Clear();
                                                    this.SIMStart.Clear();
                                                    this.VIPStart.Clear();
                                                    this.BATStart.Clear();
                                                    this.ICCIDStart.Clear();
                                                    this.MACStart.Clear();
                                                    this.EquipmentStart.Clear();
                                                    this.IMEI_Start.Focus();
                                                }
                                            }
                                            else
                                            {
                                                Form1.Log("打印了彩盒贴IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                                this.IMEI_Start.Clear();
                                                this.SIMStart.Clear();
                                                this.VIPStart.Clear();
                                                this.BATStart.Clear();
                                                this.ICCIDStart.Clear();
                                                this.MACStart.Clear();
                                                this.EquipmentStart.Clear();
                                                this.ShowSN.Clear();
                                                this.IMEI_Start.Focus();
                                            }
                                        }
                                        if (this.updata_inline.Visible == true)
                                        {
                                            MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                            statusChange();
                                        }
                                    }
                                    else if (PMB.CheckCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                    {
                                        drs = DRSB.SelectByIMEIBLL(this.IMEI_Start.Text);
                                        if (drs.Count != 0)
                                        {
                                            foreach (DataRelativeSheet a in drs)
                                            {
                                                this.SIMStart.Text = a.IMEI3;
                                                this.ICCIDStart.Text = a.IMEI4;
                                                this.MACStart.Text = a.IMEI6;
                                                this.EquipmentStart.Text = a.IMEI7;
                                                this.VIPStart.Text = a.IMEI8;
                                                this.BATStart.Text = a.IMEI9;
                                                if (a.IMEI2 != "")
                                                {
                                                    ASS_sn = a.IMEI2;
                                                }
                                                else
                                                {
                                                    ASS_sn = this.SN1_num.Text;
                                                    Sn_mark = 1;
                                                }
                                            }
                                        }
                                        for (int S = 1; S <= 2; S++)
                                        {
                                            if (S == 1)
                                            {
                                                lj = this.Select_Template1.Text;
                                                btFormat = btEngine.Documents.Open(lj);
                                                //指定打印机名称
                                                btFormat.PrintSetup.PrinterName = this.Printer1.Text;
                                            }
                                            else
                                            {
                                                if (this.Select_Template2.Text == "")
                                                {
                                                    this.IMEI_Start.Clear();
                                                    this.SIMStart.Clear();
                                                    this.VIPStart.Clear();
                                                    this.BATStart.Clear();
                                                    this.ICCIDStart.Clear();
                                                    this.MACStart.Clear();
                                                    this.EquipmentStart.Clear();
                                                    this.ShowSN.Clear();
                                                    this.IMEI_Start.Focus();
                                                    if (this.updata_inline.Visible == true)
                                                    {
                                                        MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                                        statusChange();
                                                    }
                                                    return;
                                                }
                                                else
                                                {
                                                    lj = this.Select_Template2.Text;
                                                    btFormat = btEngine.Documents.Open(lj);
                                                    //指定打印机名称
                                                    btFormat.PrintSetup.PrinterName = this.Printer2.Text;
                                                }
                                            }
                                            //对模板相应字段进行赋值
                                            ValuesToTemplate(btFormat);
                                            btFormat.SubStrings["IMEI"].Value = this.IMEI_Start.Text;
                                            list = PMB.SelectSnByIMEIBLL(this.IMEI_Start.Text);
                                            foreach (PrintMessage a in list)
                                            {
                                                if (a.SN != "")
                                                {
                                                    btFormat.SubStrings["SN"].Value = a.SN;
                                                    this.ShowSN.Text = a.SN;
                                                    if (S == 2 || this.Select_Template2.Text == "")
                                                    {
                                                        MOPB.UpdateCHAssociatedBLL(this.IMEI_Start.Text, ProductTime, this.Select_Template1.Text, this.Select_Template2.Text, this.SIMStart.Text, this.VIPStart.Text, this.BATStart.Text, this.ICCIDStart.Text, this.MACStart.Text, this.EquipmentStart.Text, a.SN);
                                                    }
                                                }
                                                else
                                                {
                                                    if (Sn_mark == 0)
                                                    {
                                                        btFormat.SubStrings["SN"].Value = ASS_sn;
                                                        this.ShowSN.Text = ASS_sn;
                                                        MOPB.UpdateCHAssociatedBLL(this.IMEI_Start.Text, ProductTime, this.Select_Template1.Text, this.Select_Template2.Text, this.SIMStart.Text, this.VIPStart.Text, this.BATStart.Text, this.ICCIDStart.Text, this.MACStart.Text, this.EquipmentStart.Text, ASS_sn);
                                                    }
                                                    else
                                                    {
                                                        btFormat.SubStrings["SN"].Value = this.SN1_num.Text;
                                                        this.ShowSN.Text = this.SN1_num.Text;
                                                        if (S == 2 || this.Select_Template2.Text == "")
                                                        {
                                                            MOPB.UpdateCHAssociatedBLL(this.IMEI_Start.Text, ProductTime, this.Select_Template1.Text, this.Select_Template2.Text, this.SIMStart.Text, this.VIPStart.Text, this.BATStart.Text, this.ICCIDStart.Text, this.MACStart.Text, this.EquipmentStart.Text, this.SN1_num.Text);
                                                            if (this.SN1_num.Text != "")
                                                            {
                                                                sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                                sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                                sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                                MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0'));
                                                                this.SN1_num.Text = sn1;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            //打印份数,同序列打印的份数
                                            btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                                            Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                        }
                                        Form1.Log("打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                        this.IMEI_Start.Clear();
                                        this.SIMStart.Clear();
                                        this.VIPStart.Clear();
                                        this.BATStart.Clear();
                                        this.ICCIDStart.Clear();
                                        this.MACStart.Clear();
                                        this.EquipmentStart.Clear();
                                        this.ShowSN.Clear();
                                        this.IMEI_Start.Focus();
                                        if (this.updata_inline.Visible == true)
                                        {
                                            MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                            statusChange();
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
                                else
                                {
                                    player1.Play();
                                    this.reminder.AppendText("请先选择模板\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                            }
                            catch (Exception ex)
                            {
                                MessageBox.Show("Exception:" + ex.Message);
                            }
                        }
                        break;
                    case 1:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 2:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                        }
                        break;
                    case 4:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.BATStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.BATStart.Focus();
                                }
                            }
                        }
                        break;
                    case 8:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.ICCIDStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.ICCIDStart.Focus();
                                }
                            }
                        }
                        break;
                    case 16:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.MACStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.MACStart.Focus();
                                }
                            }
                        }
                        break;
                    case 32:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.EquipmentStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.EquipmentStart.Focus();
                                }
                            }
                        }
                        break;
                    case 3:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 5:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 9:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 17:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 33:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 6:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                        }
                        break;
                    case 10:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                        }
                        break;
                    case 18:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                        }
                        break;
                    case 34:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                        }
                        break;
                    case 12:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.BATStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.BATStart.Focus();
                                }
                            }
                        }
                        break;
                    case 20:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.BATStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.BATStart.Focus();
                                }
                            }
                        }
                        break;
                    case 36:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.BATStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.BATStart.Focus();
                                }
                            }
                        }
                        break;
                    case 24:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.ICCIDStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.ICCIDStart.Focus();
                                }
                            }
                        }
                        break;
                    case 40:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.ICCIDStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.ICCIDStart.Focus();
                                }
                            }
                        }
                        break;
                    case 48:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.MACStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.MACStart.Focus();
                                }
                            }
                        }
                        break;
                    case 7:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 11:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 19:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 35:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 13:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 21:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 37:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 25:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 41:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 49:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 14:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                        }
                        break;
                    case 22:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                        }
                        break;
                    case 38:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                        }
                        break;
                    case 26:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                        }
                        break;
                    case 42:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                        }
                        break;
                    case 50:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                        }
                        break;
                    case 28:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.BATStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.BATStart.Focus();
                                }
                            }
                        }
                        break;
                    case 44:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.BATStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.BATStart.Focus();
                                }
                            }
                        }
                        break;
                    case 52:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.BATStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.BATStart.Focus();
                                }
                            }
                        }
                        break;
                    case 56:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.ICCIDStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.ICCIDStart.Focus();
                                }
                            }
                        }
                        break;
                    case 15:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 29:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 23:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 39:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 27:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 43:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 55:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 45:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 51:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 53:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 57:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 30:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                        }
                        break;
                    case 46:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                        }
                        break;
                    case 58:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                        }
                        break;
                    case 59:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 54:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.BATStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.BATStart.Focus();
                                }
                            }
                        }
                        break;
                    case 60:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.BATStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.BATStart.Focus();
                                }
                            }
                        }
                        break;
                    case 31:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 47:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 61:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                    case 62:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.VIPStart.Focus();
                                }
                            }
                        }
                        break;
                    case 63:
                        {
                            if (this.NoPaper.Checked == false)
                            {
                                if (PMB.CheckReCHOrJSIMEIBLL(this.IMEI_Start.Text, 2))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "重号\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                            else
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    player.Play();
                                    this.reminder.AppendText(this.IMEI_Start.Text + "未打印\r\n");
                                    this.IMEI_Start.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                else
                                {
                                    this.SIMStart.Focus();
                                }
                            }
                        }
                        break;
                }
            }
        }

        private void ValuesToTemplate(LabelFormatDocument btFormat)
        {
            GetValue("Information", "型号", out outString);
            btFormat.SubStrings[outString].Value = this.SoftModel.Text;
            GetValue("Information", "生产日期", out outString);
            btFormat.SubStrings[outString].Value = this.PrintDate.Text;
            //GetValue("Information", "产品编码", out outString);
            //btFormat.SubStrings[outString].Value = this.ProductNo.Text;
            //GetValue("Information", "软件版本", out outString);
            //btFormat.SubStrings[outString].Value = this.SoftwareVersion.Text;
            //GetValue("Information", "备注", out outString);
            //btFormat.SubStrings[outString].Value = this.Remake.Text;
            btFormat.SubStrings["SIM"].Value = this.SIMStart.Text;
            btFormat.SubStrings["VIP"].Value = this.VIPStart.Text;
            btFormat.SubStrings["BAT"].Value = this.BATStart.Text;
            btFormat.SubStrings["ICCID"].Value = this.ICCIDStart.Text;
            btFormat.SubStrings["MAC"].Value = this.MACStart.Text;
            btFormat.SubStrings["Equipment"].Value = this.EquipmentStart.Text;
        }

        private void SIMStart_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13) {
                if (this.SIMStart.Text != "")
                {
                    if (this.SIM_digits.Text != "" && this.SIM_prefix.Text != "")
                    {
                        int sim_width = this.SIMStart.Text.Length;
                        if (sim_width != int.Parse(this.SIM_digits.Text))
                        {
                            player.Play();
                            this.reminder.AppendText("SIM号不在范围内\r\n");
                            this.SIMStart.Clear();
                            this.SIMStart.Focus();
                            return;
                        }
                        else
                        {
                            int sim_prefix_width = this.SIM_prefix.Text.Length;
                            string sim_prefix = this.SIMStart.Text.Substring(0, sim_prefix_width);
                            if (sim_prefix != this.SIM_prefix.Text)
                            {
                                player.Play();
                                this.reminder.AppendText("SIM号不在范围内\r\n");
                                this.SIMStart.Clear();
                                this.SIMStart.Focus();
                                return;
                            }
                        }
                    }
                    else {
                        player.Play();
                        this.reminder.AppendText("该制单未绑定SIM范围\r\n");
                        this.SIMStart.Clear();
                        this.SIMStart.Focus();
                        return;
                    }
                    if (PMB.CheckSIMBLL(this.SIMStart.Text))
                    {
                        player.Play();
                        this.reminder.AppendText("SIM-"+this.SIMStart.Text+"重号\r\n");
                        this.SIMStart.Clear();
                        this.SIMStart.Focus();
                        return;
                    }

                    this.ICCIDStart.Text = DRSB.SelectIccidBySimAndImeiBLL(this.SIMStart.Text,this.IMEI_Start.Text);
                    if (ICCIDStart.Text == "")
                    {
                        this.reminder.AppendText("SIM号与IMEI号不对应或不存在\r\n");
                        this.SIMStart.Text = "";
                        this.SIMStart.Focus();
                        return;
                    }
                    switch (g)
                    {
                        case 1:
                            {
                                if (this.NoPaper.Checked == false)
                                {
                                    ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                    if (this.Select_Template1.Text != "" || this.Select_Template2.Text != "")
                                    {
                                        if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                        {
                                            ASS_sn = DRSB.SelectSNByImeiBLL(this.IMEI_Start.Text);
                                            if (ASS_sn == "")
                                            {
                                                 ASS_sn = this.SN1_num.Text;
                                                Sn_mark = 1;
                                            }
                                            for (int S = 1; S <= 2; S++)
                                            {
                                                if (S == 1)
                                                {
                                                    lj = this.Select_Template1.Text;
                                                    btFormat = btEngine.Documents.Open(lj);
                                                    //指定打印机名称
                                                    btFormat.PrintSetup.PrinterName = this.Printer1.Text;
                                                }
                                                else
                                                {
                                                    if (this.Select_Template2.Text == "")
                                                    {
                                                        //记录打印信息日志
                                                        list.Add(new PrintMessage()
                                                        {
                                                            Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                            IMEI = this.IMEI_Start.Text.Trim(),
                                                            IMEIStart = this.IMEI_num1.Text.Trim(),
                                                            IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                            SN = ASS_sn,
                                                            IMEIRel = this.IMEIRel.Text.Trim(),
                                                            SIM = this.SIMStart.Text.Trim(),
                                                            VIP = this.VIPStart.Text.Trim(),
                                                            BAT = this.BATStart.Text.Trim(),
                                                            SoftModel = this.SoftModel.Text.Trim(),
                                                            Version = this.SoftwareVersion.Text.Trim(),
                                                            Remark = this.Remake.Text.Trim(),
                                                            JS_PrintTime = "",
                                                            JS_TemplatePath = "",
                                                            CH_PrintTime = ProductTime,
                                                            CH_TemplatePath1 = this.Select_Template1.Text,
                                                            CH_TemplatePath2 = this.Select_Template2.Text,
                                                            ICCID = this.ICCIDStart.Text,
                                                            MAC = this.MACStart.Text,
                                                            Equipment = this.EquipmentStart.Text
                                                        });
                                                        if (PMB.InsertPrintMessageBLL(list))
                                                        {
                                                            if (SN1_num.Text != "" && Sn_mark==1)
                                                            {
                                                                Form1.Log("关联SIM打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                                                sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                                sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                                sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                                if (MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0')))
                                                                {
                                                                    this.SN1_num.Text = sn1;
                                                                    this.IMEI_Start.Clear();
                                                                    this.SIMStart.Clear();
                                                                    this.ICCIDStart.Clear();
                                                                    this.ShowSN.Clear();
                                                                    this.IMEI_Start.Focus();
                                                                }
                                                            }
                                                            else
                                                            {
                                                                Form1.Log("关联SIM打印了彩盒贴IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                                                this.IMEI_Start.Clear();
                                                                this.SIMStart.Clear();
                                                                this.ICCIDStart.Clear();
                                                                this.IMEI_Start.Focus();
                                                            }
                                                        }
                                                        if (this.updata_inline.Visible == true)
                                                        {
                                                            MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                                            statusChange();
                                                        }
                                                        return;
                                                    }
                                                    else
                                                    {
                                                        lj = this.Select_Template2.Text;
                                                        btFormat = btEngine.Documents.Open(lj);
                                                        //指定打印机名称
                                                        btFormat.PrintSetup.PrinterName = this.Printer2.Text;
                                                    }
                                                }
                                                //对模板相应字段进行赋值
                                                ValuesToTemplate(btFormat);
                                                btFormat.SubStrings["IMEI"].Value = this.IMEI_Start.Text;
                                                btFormat.SubStrings["SN"].Value = ASS_sn;
                                                this.ShowSN.Text = ASS_sn;
                                                //打印份数,同序列打印的份数
                                                btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                                                Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                            }
                                            //记录打印信息日志
                                            list.Add(new PrintMessage()
                                            {
                                                Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                IMEI = this.IMEI_Start.Text.Trim(),
                                                IMEIStart = this.IMEI_num1.Text.Trim(),
                                                IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                SN = ASS_sn,
                                                IMEIRel = this.IMEIRel.Text.Trim(),
                                                SIM = this.SIMStart.Text.Trim(),
                                                VIP = this.VIPStart.Text.Trim(),
                                                BAT = this.BATStart.Text.Trim(),
                                                SoftModel = this.SoftModel.Text.Trim(),
                                                Version = this.SoftwareVersion.Text.Trim(),
                                                Remark = this.Remake.Text.Trim(),
                                                JS_PrintTime = "",
                                                JS_TemplatePath = "",
                                                CH_PrintTime = ProductTime,
                                                CH_TemplatePath1 = this.Select_Template1.Text,
                                                CH_TemplatePath2 = this.Select_Template2.Text,
                                                ICCID = this.ICCIDStart.Text,
                                                MAC = this.MACStart.Text,
                                                Equipment = this.EquipmentStart.Text
                                            });
                                            if (PMB.InsertPrintMessageBLL(list))
                                            {
                                                if (SN1_num.Text != "" && Sn_mark==1)
                                                {
                                                    Form1.Log("关联SIM打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                                    sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                    sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                    sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                    if (MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0')))
                                                    {
                                                        this.SN1_num.Text = sn1;
                                                        this.IMEI_Start.Clear();
                                                        this.SIMStart.Clear();
                                                        this.ICCIDStart.Clear();
                                                        this.ShowSN.Clear();
                                                        this.IMEI_Start.Focus();
                                                    }
                                                }
                                                else
                                                {
                                                    Form1.Log("关联SIM打印了彩盒贴IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                                    this.IMEI_Start.Clear();
                                                    this.SIMStart.Clear();
                                                    this.ICCIDStart.Clear();
                                                    this.ShowSN.Clear();
                                                    this.IMEI_Start.Focus();
                                                }
                                            }
                                            if (this.updata_inline.Visible == true)
                                            {
                                                MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                                statusChange();
                                            }
                                        }
                                        else
                                        {
                                            ASS_sn = DRSB.SelectSNByImeiBLL(this.IMEI_Start.Text);
                                            if (ASS_sn == "")
                                            {
                                                ASS_sn = this.SN1_num.Text;
                                                Sn_mark = 1;
                                            }
                                            for (int S = 1; S <= 2; S++)
                                            {
                                                if (S == 1)
                                                {
                                                    lj = this.Select_Template1.Text;
                                                    btFormat = btEngine.Documents.Open(lj);
                                                    //指定打印机名称
                                                    btFormat.PrintSetup.PrinterName = this.Printer1.Text;
                                                }
                                                else
                                                {
                                                    if (this.Select_Template2.Text == "")
                                                    {
                                                        Form1.Log("关联SIM打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                                        this.IMEI_Start.Clear();
                                                        this.SIMStart.Clear();
                                                        this.ICCIDStart.Clear();
                                                        this.ShowSN.Clear();
                                                        this.IMEI_Start.Focus();
                                                        if (this.updata_inline.Visible == true)
                                                        {
                                                            MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                                            statusChange();
                                                        }
                                                        return;
                                                    }
                                                    else
                                                    {
                                                        lj = this.Select_Template2.Text;
                                                        btFormat = btEngine.Documents.Open(lj);
                                                        //指定打印机名称
                                                        btFormat.PrintSetup.PrinterName = this.Printer2.Text;
                                                    }
                                                }
                                                //对模板相应字段进行赋值
                                                ValuesToTemplate(btFormat);
                                                btFormat.SubStrings["IMEI"].Value = this.IMEI_Start.Text;

                                                list = PMB.SelectSnByIMEIBLL(this.IMEI_Start.Text);
                                                foreach (PrintMessage a in list)
                                                {
                                                    if (a.SN != "")
                                                    {
                                                        btFormat.SubStrings["SN"].Value = a.SN;
                                                        this.ShowSN.Text = a.SN;
                                                        if (S == 2 || this.Select_Template2.Text == "")
                                                        {
                                                            PMB.UpdateSN_SIM_ICCIDBLL(this.IMEI_Start.Text, ProductTime, this.Select_Template1.Text, this.Select_Template2.Text, this.SIMStart.Text, this.ICCIDStart.Text, a.SN);
                                                        }
                                                    }
                                                    else
                                                    {
                                                        if (Sn_mark == 1)
                                                        {
                                                            btFormat.SubStrings["SN"].Value = this.SN1_num.Text;
                                                            this.ShowSN.Text = this.SN1_num.Text;
                                                            if (S == 2 || this.Select_Template2.Text == "")
                                                            {
                                                                PMB.UpdateSN_SIM_ICCIDBLL(this.IMEI_Start.Text, ProductTime, this.Select_Template1.Text, this.Select_Template2.Text, this.SIMStart.Text, this.ICCIDStart.Text, this.SN1_num.Text);
                                                                if (this.SN1_num.Text != "")
                                                                {
                                                                    sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                                    sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                                    sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                                    MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0'));
                                                                    this.SN1_num.Text = sn1;
                                                                }
                                                            }
                                                        }
                                                        else
                                                        {
                                                            btFormat.SubStrings["SN"].Value = ASS_sn;
                                                            this.ShowSN.Text = ASS_sn;
                                                            PMB.UpdateSN_SIM_ICCIDBLL(this.IMEI_Start.Text, ProductTime, this.Select_Template1.Text, this.Select_Template2.Text, this.SIMStart.Text, this.ICCIDStart.Text, ASS_sn);
                                                        }
                                                    }
                                                }
                                                //打印份数,同序列打印的份数
                                                btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                                                Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                            }
                                            Form1.Log("关联SIM打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                            this.IMEI_Start.Clear();
                                            this.SIMStart.Clear();
                                            this.ICCIDStart.Clear();
                                            this.ShowSN.Clear();
                                            this.IMEI_Start.Focus();
                                            if (this.updata_inline.Visible == true)
                                            {
                                                MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                                statusChange();
                                            }
                                        }
                                    }
                                    else
                                    {
                                        player1.Play();
                                        this.reminder.AppendText("请先选择模板\r\n");
                                        this.SIMStart.Clear();
                                        this.SIMStart.Focus();
                                    }
                                }
                            }
                            break;
                        case 3:
                            {
                                this.VIPStart.Focus();
                            }
                            break;
                        case 5:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 9:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 17:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 33:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 7:
                            {
                                this.VIPStart.Focus();
                            }
                            break;
                        case 11:
                            {
                                this.VIPStart.Focus();
                            }
                            break;
                        case 19:
                            {
                                this.VIPStart.Focus();
                            }
                            break;
                        case 35:
                            {
                                this.VIPStart.Focus();
                            }
                            break;
                        case 15:
                            {
                                this.VIPStart.Focus();
                            }
                            break;
                        case 23:
                            {
                                this.VIPStart.Focus();
                            }
                            break;
                        case 39:
                            {
                                this.VIPStart.Focus();
                            }
                            break;
                        case 27:
                            {
                                this.VIPStart.Focus();
                            }
                            break;
                        case 43:
                            {
                                this.VIPStart.Focus();
                            }
                            break;
                        case 51:
                            {
                                this.VIPStart.Focus();
                            }
                            break;
                        case 31:
                            {
                                this.VIPStart.Focus();
                            }
                            break;
                        case 47:
                            {
                                this.VIPStart.Focus();
                            }
                            break;
                        case 55:
                            {
                                this.VIPStart.Focus();
                            }
                            break;
                        case 59:
                            {
                                this.VIPStart.Focus();
                            }
                            break;
                        case 63:
                            {
                                this.VIPStart.Focus();
                            }
                            break;
                        case 13:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 21:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 37:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 29:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 45:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 53:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 61:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 25:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 41:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 57:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 49:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                    }
                }
                else
                { this.SIMStart.Focus(); }
            }
        }

        private void VIPStart_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13) {
                if (this.VIPStart.Text != "")
                {
                    if (this.VIP_digits.Text != "" && this.VIP_prefix.Text != "")
                    {
                        int vip_width = this.VIPStart.Text.Length;
                        if (vip_width != int.Parse(this.VIP_digits.Text))
                        {
                            player.Play();
                            this.reminder.AppendText("VIP不在范围内\r\n");
                            this.VIPStart.Clear();
                            this.VIPStart.Focus();
                            return;
                        }
                        else
                        {
                            int vip_prefix_width = this.VIP_prefix.Text.Length;
                            string vip_prefix = this.VIPStart.Text.Substring(0, vip_prefix_width);
                            if (vip_prefix != this.VIP_prefix.Text)
                            {
                                player.Play();
                                this.reminder.AppendText("VIP不在范围内\r\n");
                                this.VIPStart.Clear();
                                this.VIPStart.Focus();
                                return;
                            }
                        }
                    }
                    else {
                        player.Play();
                        this.reminder.AppendText("该制单未绑定VIP范围\r\n");
                        this.VIPStart.Clear();
                        this.VIPStart.Focus();
                        return;
                    }
                    if (PMB.CheckVIPBLL(this.VIPStart.Text))
                    {
                        player.Play();
                        this.reminder.AppendText("VIP-"+this.VIPStart.Text+"重号\r\n");
                        this.VIPStart.Clear();
                        this.VIPStart.Focus();
                        return;
                    }

                    switch (g)
                    {
                        case 2:
                            {
                                if (this.NoPaper.Checked == false)
                                {
                                    ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                    if (this.Select_Template1.Text != "" || this.Select_Template2.Text != "")
                                    {
                                        if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                        {
                                            ASS_sn = DRSB.SelectSNByImeiBLL(this.IMEI_Start.Text);
                                            if (ASS_sn == "")
                                            {
                                                ASS_sn = this.SN1_num.Text;
                                                Sn_mark = 1;
                                            }
                                            for (int S = 1; S <= 2; S++)
                                            {
                                                if (S == 1)
                                                {
                                                    lj = this.Select_Template1.Text;
                                                    btFormat = btEngine.Documents.Open(lj);
                                                    //指定打印机名称
                                                    btFormat.PrintSetup.PrinterName = this.Printer1.Text;
                                                }
                                                else
                                                {
                                                    if (this.Select_Template2.Text == "")
                                                    {
                                                        //记录打印信息日志
                                                        list.Add(new PrintMessage()
                                                        {
                                                            Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                            IMEI = this.IMEI_Start.Text.Trim(),
                                                            IMEIStart = this.IMEI_num1.Text.Trim(),
                                                            IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                            SN = ASS_sn,
                                                            IMEIRel = this.IMEIRel.Text.Trim(),
                                                            SIM = this.SIMStart.Text.Trim(),
                                                            VIP = this.VIPStart.Text.Trim(),
                                                            BAT = this.BATStart.Text.Trim(),
                                                            SoftModel = this.SoftModel.Text.Trim(),
                                                            Version = this.SoftwareVersion.Text.Trim(),
                                                            Remark = this.Remake.Text.Trim(),
                                                            JS_PrintTime = "",
                                                            JS_TemplatePath = "",
                                                            CH_PrintTime = ProductTime,
                                                            CH_TemplatePath1 = this.Select_Template1.Text,
                                                            CH_TemplatePath2 = this.Select_Template2.Text,
                                                            ICCID = this.ICCIDStart.Text,
                                                            MAC = this.MACStart.Text,
                                                            Equipment = this.EquipmentStart.Text
                                                        });
                                                        if (PMB.InsertPrintMessageBLL(list))
                                                        {
                                                            DRSB.UpdateVIPBLL(this.IMEI_Start.Text, this.VIPStart.Text);
                                                            if (SN1_num.Text != "" && Sn_mark == 1)
                                                            {
                                                                Form1.Log("关联VIP打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                                                sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                                sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                                sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                                if (MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0')))
                                                                {
                                                                    this.SN1_num.Text = sn1;
                                                                    this.IMEI_Start.Clear();
                                                                    this.VIPStart.Clear();
                                                                    this.ShowSN.Clear();
                                                                    this.IMEI_Start.Focus();
                                                                }
                                                            }
                                                            else
                                                            {
                                                                Form1.Log("关联VIP打印了彩盒贴IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                                                this.IMEI_Start.Clear();
                                                                this.VIPStart.Clear();
                                                                this.ShowSN.Clear();
                                                                this.IMEI_Start.Focus();
                                                            }
                                                        }
                                                        if (this.updata_inline.Visible == true)
                                                        {
                                                            MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                                            statusChange();
                                                        }
                                                        return;
                                                    }
                                                    else
                                                    {
                                                        lj = this.Select_Template2.Text;
                                                        btFormat = btEngine.Documents.Open(lj);
                                                        //指定打印机名称
                                                        btFormat.PrintSetup.PrinterName = this.Printer2.Text;
                                                    }
                                                }
                                                //对模板相应字段进行赋值
                                                ValuesToTemplate(btFormat);
                                                btFormat.SubStrings["IMEI"].Value = this.IMEI_Start.Text;
                                                btFormat.SubStrings["SN"].Value = ASS_sn;
                                                this.ShowSN.Text = ASS_sn;
                                                //打印份数,同序列打印的份数
                                                btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                                                Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                            }
                                            //记录打印信息日志
                                            list.Add(new PrintMessage()
                                            {
                                                Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                IMEI = this.IMEI_Start.Text.Trim(),
                                                IMEIStart = this.IMEI_num1.Text.Trim(),
                                                IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                SN = ASS_sn,
                                                IMEIRel = this.IMEIRel.Text.Trim(),
                                                SIM = this.SIMStart.Text.Trim(),
                                                VIP = this.VIPStart.Text.Trim(),
                                                BAT = this.BATStart.Text.Trim(),
                                                SoftModel = this.SoftModel.Text.Trim(),
                                                Version = this.SoftwareVersion.Text.Trim(),
                                                Remark = this.Remake.Text.Trim(),
                                                JS_PrintTime = "",
                                                JS_TemplatePath = "",
                                                CH_PrintTime = ProductTime,
                                                CH_TemplatePath1 = this.Select_Template1.Text,
                                                CH_TemplatePath2 = this.Select_Template2.Text,
                                                ICCID = this.ICCIDStart.Text,
                                                MAC = this.MACStart.Text,
                                                Equipment = this.EquipmentStart.Text
                                            });
                                            if (PMB.InsertPrintMessageBLL(list))
                                            {
                                                DRSB.UpdateVIPBLL(this.IMEI_Start.Text, this.VIPStart.Text);
                                                if (SN1_num.Text != "" && Sn_mark == 1)
                                                {
                                                    Form1.Log("关联VIP打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                                    sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                    sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                    sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                    if (MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0')))
                                                    {
                                                        this.SN1_num.Text = sn1;
                                                        this.IMEI_Start.Clear();
                                                        this.VIPStart.Clear();
                                                        this.ShowSN.Clear();
                                                        this.IMEI_Start.Focus();
                                                    }
                                                }
                                                else
                                                {
                                                    Form1.Log("关联VIP打印了彩盒贴IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                                    this.IMEI_Start.Clear();
                                                    this.VIPStart.Clear();
                                                    this.ShowSN.Clear();
                                                    this.IMEI_Start.Focus();
                                                }
                                            }
                                        }
                                        else
                                        {
                                            ASS_sn = DRSB.SelectSNByImeiBLL(this.IMEI_Start.Text);
                                            if (ASS_sn == "")
                                            {
                                                ASS_sn = this.SN1_num.Text;
                                                Sn_mark = 1;
                                            }
                                            for (int S = 1; S <= 2; S++)
                                            {
                                                if (S == 1)
                                                {
                                                    lj = this.Select_Template1.Text;
                                                    btFormat = btEngine.Documents.Open(lj);
                                                    //指定打印机名称
                                                    btFormat.PrintSetup.PrinterName = this.Printer1.Text;
                                                }
                                                else
                                                {
                                                    if (this.Select_Template2.Text == "")
                                                    {
                                                        Form1.Log("关联VIP打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                                        this.IMEI_Start.Clear();
                                                        this.VIPStart.Clear();
                                                        this.ShowSN.Clear();
                                                        this.IMEI_Start.Focus();
                                                        if (this.updata_inline.Visible == true)
                                                        {
                                                            MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                                            statusChange();
                                                        }
                                                        return;
                                                    }
                                                    else
                                                    {
                                                        lj = this.Select_Template2.Text;
                                                        btFormat = btEngine.Documents.Open(lj);
                                                        //指定打印机名称
                                                        btFormat.PrintSetup.PrinterName = this.Printer2.Text;
                                                    }
                                                }
                                                //对模板相应字段进行赋值
                                                ValuesToTemplate(btFormat);
                                                btFormat.SubStrings["IMEI"].Value = this.IMEI_Start.Text;

                                                list = PMB.SelectSnByIMEIBLL(this.IMEI_Start.Text);
                                                foreach (PrintMessage a in list)
                                                {
                                                    if (a.SN != "")
                                                    {
                                                        btFormat.SubStrings["SN"].Value = a.SN;
                                                        this.ShowSN.Text = a.SN;
                                                        if (S == 2 || this.Select_Template2.Text=="")
                                                        {
                                                            DRSB.UpdateVIPBLL(this.IMEI_Start.Text, this.VIPStart.Text);
                                                            PMB.UpdateSN_VIPBLL(this.IMEI_Start.Text, ProductTime, this.Select_Template1.Text, this.Select_Template2.Text, this.VIPStart.Text, a.SN);
                                                        }
                                                    }
                                                    else
                                                    {
                                                        if (Sn_mark == 1)
                                                        {
                                                            btFormat.SubStrings["SN"].Value = this.SN1_num.Text;
                                                            this.ShowSN.Text = this.SN1_num.Text;
                                                            if (S == 2 || this.Select_Template2.Text == "")
                                                            {
                                                                DRSB.UpdateVIPBLL(this.IMEI_Start.Text, this.VIPStart.Text);
                                                                PMB.UpdateSN_VIPBLL(this.IMEI_Start.Text, ProductTime, this.Select_Template1.Text, this.Select_Template2.Text, this.VIPStart.Text, this.SN1_num.Text);
                                                                if (this.SN1_num.Text != "")
                                                                {
                                                                    sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                                    sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                                    sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                                    MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0'));
                                                                    this.SN1_num.Text = sn1;
                                                                }
                                                            }
                                                        }
                                                        else
                                                        {
                                                            btFormat.SubStrings["SN"].Value = ASS_sn;
                                                            this.ShowSN.Text = ASS_sn;
                                                            DRSB.UpdateVIPBLL(this.IMEI_Start.Text, this.VIPStart.Text);
                                                            PMB.UpdateSN_VIPBLL(this.IMEI_Start.Text, ProductTime, this.Select_Template1.Text, this.Select_Template2.Text, this.VIPStart.Text, ASS_sn);
                                                        }
                                                    }
                                                }
                                                //打印份数,同序列打印的份数
                                                btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                                                Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                            }
                                            Form1.Log("关联VIP打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                            this.IMEI_Start.Clear();
                                            this.VIPStart.Clear();
                                            this.ShowSN.Clear();
                                            this.IMEI_Start.Focus();
                                        }
                                        if (this.updata_inline.Visible == true)
                                        {
                                            MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                            statusChange();
                                        }
                                    }
                                    else
                                    {
                                        player1.Play();
                                        this.reminder.AppendText("请先选择模板\r\n");
                                        this.VIPStart.Clear();
                                        this.VIPStart.Focus();
                                    }
                                }
                                else
                                {
                                    //查询关联表，有数据则UPDATA进去，没有则INSERT
                                    if (DRSB.CheckIMEIBLL(this.IMEI_Start.Text))
                                    {
                                        DRSB.UpdateVIPBLL(this.IMEI_Start.Text, this.VIPStart.Text);
                                    }
                                    else
                                    {
                                        //记录关联数据信息到关联表
                                        drs.Add(new DataRelativeSheet()
                                        {
                                            IMEI1 = this.IMEI_Start.Text,
                                            IMEI2 = "",
                                            IMEI3 = "",
                                            IMEI4 = "",
                                            IMEI5 = "",
                                            IMEI6 = "",
                                            IMEI7 = "",
                                            IMEI8 = this.VIPStart.Text,
                                            IMEI9 = "",
                                            IMEI10 = "",
                                            IMEI11 = "",
                                            IMEI12 = "",
                                            ZhiDan = this.CB_ZhiDan.Text,
                                            TestTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff")
                                        });
                                        DRSB.InsertRelativeSheetBLL(drs);
                                    }
                                    //打印表，打印过了才更新关联数据
                                    if (PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                    {
                                        PMB.UpdateVIPBLL(this.IMEI_Start.Text, this.VIPStart.Text);
                                    }
                                    this.IMEI_Start.Clear();
                                    this.VIPStart.Clear();
                                    this.IMEI_Start.Focus();
                                    if (this.updata_inline.Visible == true)
                                    {
                                        MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                        statusChange();
                                    }
                                }
                            }
                            break;
                        case 3:
                            {
                                if (this.NoPaper.Checked == false)
                                {
                                    ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                                    if (this.Select_Template1.Text != "" || this.Select_Template2.Text != "")
                                    {
                                        if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                        {
                                            ASS_sn = DRSB.SelectSNByImeiBLL(this.IMEI_Start.Text);
                                            if (ASS_sn == "")
                                            {
                                                ASS_sn = this.SN1_num.Text;
                                                Sn_mark = 1;
                                            }
                                            for (int S = 1; S <= 2; S++)
                                            {
                                                if (S == 1)
                                                {
                                                    lj = this.Select_Template1.Text;
                                                    btFormat = btEngine.Documents.Open(lj);
                                                    //指定打印机名称
                                                    btFormat.PrintSetup.PrinterName = this.Printer1.Text;
                                                }
                                                else
                                                {
                                                    if (this.Select_Template2.Text == "")
                                                    {
                                                        //记录打印信息日志
                                                        list.Add(new PrintMessage()
                                                        {
                                                            Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                            IMEI = this.IMEI_Start.Text.Trim(),
                                                            IMEIStart = this.IMEI_num1.Text.Trim(),
                                                            IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                            SN = ASS_sn,
                                                            IMEIRel = this.IMEIRel.Text.Trim(),
                                                            SIM = this.SIMStart.Text.Trim(),
                                                            VIP = this.VIPStart.Text.Trim(),
                                                            BAT = this.BATStart.Text.Trim(),
                                                            SoftModel = this.SoftModel.Text.Trim(),
                                                            Version = this.SoftwareVersion.Text.Trim(),
                                                            Remark = this.Remake.Text.Trim(),
                                                            JS_PrintTime = "",
                                                            JS_TemplatePath = "",
                                                            CH_PrintTime = ProductTime,
                                                            CH_TemplatePath1 = this.Select_Template1.Text,
                                                            CH_TemplatePath2 = this.Select_Template2.Text,
                                                            ICCID = this.ICCIDStart.Text,
                                                            MAC = this.MACStart.Text,
                                                            Equipment = this.EquipmentStart.Text
                                                        });
                                                        if (PMB.InsertPrintMessageBLL(list))
                                                        {
                                                            DRSB.UpdateVIPBLL(this.IMEI_Start.Text, this.VIPStart.Text);
                                                            if (SN1_num.Text != "" && Sn_mark == 1)
                                                            {
                                                                Form1.Log("关联SIM && VIP打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                                                sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                                sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                                sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                                if (MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0')))
                                                                {
                                                                    this.SN1_num.Text = sn1;
                                                                    this.IMEI_Start.Clear();
                                                                    this.SIMStart.Clear();
                                                                    this.VIPStart.Clear();
                                                                    this.ICCIDStart.Clear();
                                                                    this.ShowSN.Clear();
                                                                    this.IMEI_Start.Focus();
                                                                }
                                                            }
                                                            else
                                                            {
                                                                Form1.Log("关联SIM && VIP打印了彩盒贴IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                                                this.IMEI_Start.Clear();
                                                                this.VIPStart.Clear();
                                                                this.SIMStart.Clear();
                                                                this.ICCIDStart.Clear();
                                                                this.ShowSN.Clear();
                                                                this.IMEI_Start.Focus();
                                                            }
                                                        }
                                                        if (this.updata_inline.Visible == true)
                                                        {
                                                            MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                                            statusChange();
                                                        }
                                                        return;
                                                    }
                                                    else
                                                    {
                                                        lj = this.Select_Template2.Text;
                                                        btFormat = btEngine.Documents.Open(lj);
                                                        //指定打印机名称
                                                        btFormat.PrintSetup.PrinterName = this.Printer2.Text;
                                                    }
                                                }
                                                //对模板相应字段进行赋值
                                                ValuesToTemplate(btFormat);
                                                btFormat.SubStrings["IMEI"].Value = this.IMEI_Start.Text;
                                                btFormat.SubStrings["SN"].Value = ASS_sn;
                                                this.ShowSN.Text = ASS_sn;
                                                //打印份数,同序列打印的份数
                                                btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                                                Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                            }
                                            //记录打印信息日志
                                            list.Add(new PrintMessage()
                                            {
                                                Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                IMEI = this.IMEI_Start.Text.Trim(),
                                                IMEIStart = this.IMEI_num1.Text.Trim(),
                                                IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                SN = ASS_sn,
                                                IMEIRel = this.IMEIRel.Text.Trim(),
                                                SIM = this.SIMStart.Text.Trim(),
                                                VIP = this.VIPStart.Text.Trim(),
                                                BAT = this.BATStart.Text.Trim(),
                                                SoftModel = this.SoftModel.Text.Trim(),
                                                Version = this.SoftwareVersion.Text.Trim(),
                                                Remark = this.Remake.Text.Trim(),
                                                JS_PrintTime = "",
                                                JS_TemplatePath = "",
                                                CH_PrintTime = ProductTime,
                                                CH_TemplatePath1 = this.Select_Template1.Text,
                                                CH_TemplatePath2 = this.Select_Template2.Text,
                                                ICCID = this.ICCIDStart.Text,
                                                MAC = this.MACStart.Text,
                                                Equipment = this.EquipmentStart.Text
                                            });
                                            if (PMB.InsertPrintMessageBLL(list))
                                            {
                                                DRSB.UpdateVIPBLL(this.IMEI_Start.Text, this.VIPStart.Text);
                                                if (SN1_num.Text != "" && Sn_mark == 1)
                                                {
                                                    Form1.Log("关联SIM && VIP打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                                    sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                    sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                    sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                    if (MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0')))
                                                    {
                                                        this.SN1_num.Text = sn1;
                                                        this.IMEI_Start.Clear();
                                                        this.SIMStart.Clear();
                                                        this.VIPStart.Clear();
                                                        this.ICCIDStart.Clear();
                                                        this.ShowSN.Clear();
                                                        this.IMEI_Start.Focus();
                                                    }
                                                }
                                                else
                                                {
                                                    Form1.Log("关联SIM && VIP打印了彩盒贴IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                                    this.IMEI_Start.Clear();
                                                    this.SIMStart.Clear();
                                                    this.VIPStart.Clear();
                                                    this.ICCIDStart.Clear();
                                                    this.ShowSN.Clear();
                                                    this.IMEI_Start.Focus();
                                                }
                                            }
                                        }
                                        else
                                        {
                                            ASS_sn = DRSB.SelectSNByImeiBLL(this.IMEI_Start.Text);
                                            if (ASS_sn == "")
                                            {
                                                ASS_sn = this.SN1_num.Text;
                                                Sn_mark = 1;
                                            }
                                            for (int S = 1; S <= 2; S++)
                                            {
                                                if (S == 1)
                                                {
                                                    lj = this.Select_Template1.Text;
                                                    btFormat = btEngine.Documents.Open(lj);
                                                    //指定打印机名称
                                                    btFormat.PrintSetup.PrinterName = this.Printer1.Text;
                                                }
                                                else
                                                {
                                                    if (this.Select_Template2.Text == "")
                                                    {
                                                        this.IMEI_Start.Clear();
                                                        this.SIMStart.Clear();
                                                        this.VIPStart.Clear();
                                                        this.ICCIDStart.Clear();
                                                        this.ShowSN.Clear();
                                                        this.IMEI_Start.Focus();
                                                        if (this.updata_inline.Visible == true)
                                                        {
                                                            MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                                            statusChange();
                                                        }
                                                        return;
                                                    }
                                                    else
                                                    {
                                                        lj = this.Select_Template2.Text;
                                                        btFormat = btEngine.Documents.Open(lj);
                                                        //指定打印机名称
                                                        btFormat.PrintSetup.PrinterName = this.Printer2.Text;
                                                    }
                                                }
                                                //对模板相应字段进行赋值
                                                ValuesToTemplate(btFormat);
                                                btFormat.SubStrings["IMEI"].Value = this.IMEI_Start.Text;

                                                list = PMB.SelectSnByIMEIBLL(this.IMEI_Start.Text);
                                                foreach (PrintMessage a in list)
                                                {
                                                    if (a.SN != "")
                                                    {
                                                        btFormat.SubStrings["SN"].Value = a.SN;
                                                        this.ShowSN.Text = a.SN;
                                                        if (S == 2 || this.Select_Template2.Text == "")
                                                        {
                                                            DRSB.UpdateVIPBLL(this.IMEI_Start.Text, this.VIPStart.Text);
                                                            PMB.UpdateSN_SIM_VIP_ICCIDBLL(this.IMEI_Start.Text, ProductTime, this.Select_Template1.Text, this.Select_Template2.Text, this.SIMStart.Text, this.VIPStart.Text, this.ICCIDStart.Text, a.SN);
                                                        }
                                                    }
                                                    else
                                                    {
                                                        if (Sn_mark == 1)
                                                        {
                                                            btFormat.SubStrings["SN"].Value = this.SN1_num.Text;
                                                            this.ShowSN.Text = this.SN1_num.Text;
                                                            if (S == 2 || this.Select_Template2.Text == "")
                                                            {
                                                                DRSB.UpdateVIPBLL(this.IMEI_Start.Text, this.VIPStart.Text);
                                                                PMB.UpdateSN_SIM_VIP_ICCIDBLL(this.IMEI_Start.Text, ProductTime, this.Select_Template1.Text, this.Select_Template2.Text, this.SIMStart.Text, this.VIPStart.Text, this.ICCIDStart.Text, this.SN1_num.Text);
                                                                if (this.SN1_num.Text != "")
                                                                {
                                                                    sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                                    sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                                    sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                                    MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0'));
                                                                    this.SN1_num.Text = sn1;
                                                                }
                                                            }
                                                        }
                                                        else {
                                                            btFormat.SubStrings["SN"].Value = ASS_sn;
                                                            this.ShowSN.Text = ASS_sn;
                                                            DRSB.UpdateVIPBLL(this.IMEI_Start.Text, this.VIPStart.Text);
                                                            PMB.UpdateSN_SIM_VIP_ICCIDBLL(this.IMEI_Start.Text, ProductTime, this.Select_Template1.Text, this.Select_Template2.Text, this.SIMStart.Text, this.VIPStart.Text, this.ICCIDStart.Text, ASS_sn);
                                                        }
                                                    }
                                                }
                                                //打印份数,同序列打印的份数
                                                btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                                                Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                            }
                                            Form1.Log("关联SIM && VIP打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                            this.IMEI_Start.Clear();
                                            this.SIMStart.Clear();
                                            this.VIPStart.Clear();
                                            this.ICCIDStart.Clear();
                                            this.ShowSN.Clear();
                                            this.IMEI_Start.Focus();
                                        }
                                        if (this.updata_inline.Visible == true)
                                        {
                                            MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                            statusChange();
                                        }
                                    }
                                    else
                                    {
                                        player1.Play();
                                        this.reminder.AppendText("请先选择模板\r\n");
                                        this.VIPStart.Clear();
                                        this.VIPStart.Focus();
                                    }
                                }
                                else
                                {
                                    //查询关联表，有数据则UPDATA进去，没有则INSERT
                                    if (DRSB.CheckIMEIBLL(this.IMEI_Start.Text))
                                    {
                                        DRSB.UpdateVIPBLL(this.IMEI_Start.Text, this.VIPStart.Text);
                                    }
                                    else
                                    {
                                        //记录关联数据信息到关联表
                                        drs.Add(new DataRelativeSheet()
                                        {
                                            IMEI1 = this.IMEI_Start.Text,
                                            IMEI2 = "",
                                            IMEI3 = this.SIMStart.Text,
                                            IMEI4 = this.ICCIDStart.Text,
                                            IMEI5 = "",
                                            IMEI6 = "",
                                            IMEI7 = "",
                                            IMEI8 = this.VIPStart.Text,
                                            IMEI9 = "",
                                            IMEI10 = "",
                                            IMEI11 = "",
                                            IMEI12 = "",
                                            ZhiDan = this.CB_ZhiDan.Text,
                                            TestTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff")
                                        });
                                        DRSB.InsertRelativeSheetBLL(drs);
                                    }
                                    //打印表，打印过了才更新关联数据
                                    if (PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                    {
                                        PMB.UpdateSimVipIccidBLL(this.IMEI_Start.Text, this.SIMStart.Text, this.VIPStart.Text, this.ICCIDStart.Text);
                                    }
                                    this.IMEI_Start.Clear();
                                    this.SIMStart.Clear();
                                    this.VIPStart.Clear();
                                    this.ICCIDStart.Clear();
                                    this.IMEI_Start.Focus();
                                    if (this.updata_inline.Visible == true)
                                    {
                                        MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                        statusChange();
                                    }
                                }
                            }
                            break;
                        case 6:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 7:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 10:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 11:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 18:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 19:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 34:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 35:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 14:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 22:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 38:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 15:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 23:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 39:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 30:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 46:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 54:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 31:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 47:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 55:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 62:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 63:
                            {
                                this.BATStart.Focus();
                            }
                            break;
                        case 26:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 42:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 27:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 43:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 58:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 59:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 50:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 51:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                    }
                }
                else  {
                    this.VIPStart.Focus();
                }
            }
        }

        private void BATStart_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                if (this.BATStart.Text != "")
                {
                    if (this.BAT_digits.Text != "" && this.BAT_prefix.Text != "")
                    {
                        int bat_width = this.BATStart.Text.Length;
                        if (bat_width != int.Parse(this.BAT_digits.Text))
                        {
                            player.Play();
                            this.reminder.AppendText("BAT不在范围内\r\n");
                            this.BATStart.Clear();
                            this.BATStart.Focus();
                            return;
                        }
                        else
                        {
                            int bat_prefix_width = this.BAT_prefix.Text.Length;
                            string bat_prefix = this.BATStart.Text.Substring(0, bat_prefix_width);
                            if (bat_prefix != this.BAT_prefix.Text)
                            {
                                player.Play();
                                this.reminder.AppendText("BAT不在范围内\r\n");
                                this.BATStart.Clear();
                                this.BATStart.Focus();
                                return;
                            }
                        }
                    }
                    else
                    {
                        player.Play();
                        this.reminder.AppendText("此制单未绑定BAT\r\n");
                        this.BATStart.Clear();
                        this.BATStart.Focus();
                        return;
                    }
                    if (PMB.CheckBATBLL(this.BATStart.Text))
                    {
                        player.Play();
                        this.reminder.AppendText("BAT-"+this.BATStart.Text+"重号\r\n");
                        this.BATStart.Clear();
                        this.BATStart.Focus();
                        return;
                    }

                    if (g >= 4 && g <= 7)
                    {
                        if (this.NoPaper.Checked == false)
                        {
                            ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                            if (this.Select_Template1.Text != "" || this.Select_Template2.Text != "")
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    ASS_sn = DRSB.SelectSNByImeiBLL(this.IMEI_Start.Text);
                                    if (ASS_sn == "")
                                    {
                                        ASS_sn = this.SN1_num.Text;
                                        Sn_mark = 1;
                                    }
                                    for (int S = 1; S <= 2; S++)
                                    {
                                        if (S == 1)
                                        {
                                            lj = this.Select_Template1.Text;
                                            btFormat = btEngine.Documents.Open(lj);
                                            //指定打印机名称
                                            btFormat.PrintSetup.PrinterName = this.Printer1.Text;
                                        }
                                        else
                                        {
                                            if (this.Select_Template2.Text == "")
                                            {
                                                //记录打印信息日志
                                                list.Add(new PrintMessage()
                                                {
                                                    Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                    IMEI = this.IMEI_Start.Text.Trim(),
                                                    IMEIStart = this.IMEI_num1.Text.Trim(),
                                                    IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                    SN = ASS_sn,
                                                    IMEIRel = this.IMEIRel.Text.Trim(),
                                                    SIM = this.SIMStart.Text.Trim(),
                                                    VIP = this.VIPStart.Text.Trim(),
                                                    BAT = this.BATStart.Text.Trim(),
                                                    SoftModel = this.SoftModel.Text.Trim(),
                                                    Version = this.SoftwareVersion.Text.Trim(),
                                                    Remark = this.Remake.Text.Trim(),
                                                    JS_PrintTime = "",
                                                    JS_TemplatePath = "",
                                                    CH_PrintTime = ProductTime,
                                                    CH_TemplatePath1 = this.Select_Template1.Text,
                                                    CH_TemplatePath2 = this.Select_Template2.Text,
                                                    ICCID = this.ICCIDStart.Text,
                                                    MAC = this.MACStart.Text,
                                                    Equipment = this.EquipmentStart.Text
                                                });
                                                if (PMB.InsertPrintMessageBLL(list))
                                                {
                                                    DRSB.UpdateVipAndBatBLL(this.IMEI_Start.Text, this.VIPStart.Text, this.BATStart.Text);
                                                    if (SN1_num.Text != "" && Sn_mark == 1)
                                                    {
                                                        Form1.Log("关联BAT打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                                        sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                        sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                        sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                        if (MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0')))
                                                        {
                                                            this.SN1_num.Text = sn1;
                                                            this.IMEI_Start.Clear();
                                                            this.SIMStart.Clear();
                                                            this.VIPStart.Clear();
                                                            this.BATStart.Clear();
                                                            this.ShowSN.Clear();
                                                            this.IMEI_Start.Focus();
                                                        }
                                                    }
                                                    else
                                                    {
                                                        Form1.Log("关联BAT打印了彩盒贴IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                                        this.IMEI_Start.Clear();
                                                        this.SIMStart.Clear();
                                                        this.VIPStart.Clear();
                                                        this.BATStart.Clear();
                                                        this.ShowSN.Clear();
                                                        this.IMEI_Start.Focus();
                                                    }
                                                }
                                                if (this.updata_inline.Visible == true)
                                                {
                                                    MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                                    statusChange();
                                                }
                                                return;
                                            }
                                            else
                                            {
                                                lj = this.Select_Template2.Text;
                                                btFormat = btEngine.Documents.Open(lj);
                                                //指定打印机名称
                                                btFormat.PrintSetup.PrinterName = this.Printer2.Text;
                                            }
                                        }
                                        //对模板相应字段进行赋值
                                        ValuesToTemplate(btFormat);
                                        btFormat.SubStrings["IMEI"].Value = this.IMEI_Start.Text;
                                        btFormat.SubStrings["SN"].Value = ASS_sn;
                                        this.ShowSN.Text = ASS_sn;
                                        //打印份数,同序列打印的份数
                                        btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                                        Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                    }
                                    //记录打印信息日志
                                    list.Add(new PrintMessage()
                                    {
                                        Zhidan = this.CB_ZhiDan.Text.Trim(),
                                        IMEI = this.IMEI_Start.Text.Trim(),
                                        IMEIStart = this.IMEI_num1.Text.Trim(),
                                        IMEIEnd = this.IMEI_num2.Text.Trim(),
                                        SN = ASS_sn,
                                        IMEIRel = this.IMEIRel.Text.Trim(),
                                        SIM = this.SIMStart.Text.Trim(),
                                        VIP = this.VIPStart.Text.Trim(),
                                        BAT = this.BATStart.Text.Trim(),
                                        SoftModel = this.SoftModel.Text.Trim(),
                                        Version = this.SoftwareVersion.Text.Trim(),
                                        Remark = this.Remake.Text.Trim(),
                                        JS_PrintTime = "",
                                        JS_TemplatePath = "",
                                        CH_PrintTime = ProductTime,
                                        CH_TemplatePath1 = this.Select_Template1.Text,
                                        CH_TemplatePath2 = this.Select_Template2.Text,
                                        ICCID = this.ICCIDStart.Text,
                                        MAC = this.MACStart.Text,
                                        Equipment = this.EquipmentStart.Text
                                    });
                                    if (PMB.InsertPrintMessageBLL(list))
                                    {
                                        DRSB.UpdateVipAndBatBLL(this.IMEI_Start.Text, this.VIPStart.Text, this.BATStart.Text);
                                        if (SN1_num.Text != "" && Sn_mark==1)
                                        {
                                            Form1.Log("关联BAT打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                            sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                            sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                            sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                            if (MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0')))
                                            {
                                                this.SN1_num.Text = sn1;
                                                this.IMEI_Start.Clear();
                                                this.SIMStart.Clear();
                                                this.VIPStart.Clear();
                                                this.BATStart.Clear();
                                                this.ShowSN.Clear();
                                                this.IMEI_Start.Focus();
                                            }
                                        }
                                        else
                                        {
                                            Form1.Log("关联BAT打印了彩盒贴IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                            this.IMEI_Start.Clear();
                                            this.SIMStart.Clear();
                                            this.VIPStart.Clear();
                                            this.BATStart.Clear();
                                            this.ShowSN.Clear();
                                            this.IMEI_Start.Focus();
                                        }
                                    }
                                }
                                else
                                {
                                    ASS_sn = DRSB.SelectSNByImeiBLL(this.IMEI_Start.Text);
                                    if (ASS_sn == "")
                                    {
                                        ASS_sn = this.SN1_num.Text;
                                        Sn_mark = 1;
                                    }
                                    for (int S = 1; S <= 2; S++)
                                    {
                                        if (S == 1)
                                        {
                                            lj = this.Select_Template1.Text;
                                            btFormat = btEngine.Documents.Open(lj);
                                            //指定打印机名称
                                            btFormat.PrintSetup.PrinterName = this.Printer1.Text;
                                        }
                                        else
                                        {
                                            if (this.Select_Template2.Text == "")
                                            {
                                                Form1.Log("关联BAT打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                                this.IMEI_Start.Clear();
                                                this.SIMStart.Clear();
                                                this.VIPStart.Clear();
                                                this.BATStart.Clear();
                                                this.ICCIDStart.Clear();
                                                this.ShowSN.Clear();
                                                this.IMEI_Start.Focus();
                                                if (this.updata_inline.Visible == true)
                                                {
                                                    MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                                    statusChange();
                                                }
                                                return;
                                            }
                                            else
                                            {
                                                lj = this.Select_Template2.Text;
                                                btFormat = btEngine.Documents.Open(lj);
                                                //指定打印机名称
                                                btFormat.PrintSetup.PrinterName = this.Printer2.Text;
                                            }
                                        }
                                        //对模板相应字段进行赋值
                                        ValuesToTemplate(btFormat);
                                        btFormat.SubStrings["IMEI"].Value = this.IMEI_Start.Text;

                                        list = PMB.SelectSnByIMEIBLL(this.IMEI_Start.Text);
                                        foreach (PrintMessage a in list)
                                        {
                                            if (a.SN != "")
                                            {
                                                btFormat.SubStrings["SN"].Value = a.SN;
                                                this.ShowSN.Text = a.SN;
                                                if (S == 2 || this.Select_Template2.Text == "")
                                                {
                                                    DRSB.UpdateVipAndBatBLL(this.IMEI_Start.Text, this.VIPStart.Text, this.BATStart.Text);
                                                    PMB.UpdateSN_SIM_VIP_BAT_ICCIDBLL(this.IMEI_Start.Text, ProductTime, this.Select_Template1.Text, this.Select_Template2.Text, this.SIMStart.Text, this.VIPStart.Text, this.BATStart.Text, this.ICCIDStart.Text, a.SN);
                                                }
                                            }
                                            else
                                            {
                                                if (Sn_mark == 1)
                                                {
                                                    btFormat.SubStrings["SN"].Value = this.SN1_num.Text;
                                                    this.ShowSN.Text = this.SN1_num.Text;
                                                    if (S == 2 || this.Select_Template2.Text == "")
                                                    {
                                                        DRSB.UpdateVipAndBatBLL(this.IMEI_Start.Text, this.VIPStart.Text, this.BATStart.Text);
                                                        PMB.UpdateSN_SIM_VIP_BAT_ICCIDBLL(this.IMEI_Start.Text, ProductTime, this.Select_Template1.Text, this.Select_Template2.Text, this.SIMStart.Text, this.VIPStart.Text, this.BATStart.Text, this.ICCIDStart.Text, this.SN1_num.Text);
                                                        if (this.SN1_num.Text != "")
                                                        {
                                                            sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                            sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                            sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                            MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0'));
                                                            this.SN1_num.Text = sn1;
                                                        }
                                                    }
                                                }
                                                else {
                                                    btFormat.SubStrings["SN"].Value = ASS_sn;
                                                    this.ShowSN.Text = ASS_sn;
                                                    DRSB.UpdateVipAndBatBLL(this.IMEI_Start.Text, this.VIPStart.Text, this.BATStart.Text);
                                                    PMB.UpdateSN_SIM_VIP_BAT_ICCIDBLL(this.IMEI_Start.Text, ProductTime, this.Select_Template1.Text, this.Select_Template2.Text, this.SIMStart.Text, this.VIPStart.Text, this.BATStart.Text, this.ICCIDStart.Text, ASS_sn);
                                                }
                                            }
                                        }
                                        //打印份数,同序列打印的份数
                                        btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                                        Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                    }
                                    Form1.Log("关联BAT打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                    this.IMEI_Start.Clear();
                                    this.SIMStart.Clear();
                                    this.VIPStart.Clear();
                                    this.BATStart.Clear();
                                    this.ICCIDStart.Clear();
                                    this.ShowSN.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                if (this.updata_inline.Visible == true)
                                {
                                    MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                    statusChange();
                                }
                            }
                            else
                            {
                                player1.Play();
                                this.reminder.AppendText("请先选择模板\r\n");
                                this.BATStart.Clear();
                                this.BATStart.Focus();
                            }
                        }
                        else
                        {
                            if (DRSB.CheckIMEIBLL(this.IMEI_Start.Text))
                            {
                               DRSB.UpdateVipAndBatBLL(this.IMEI_Start.Text, this.VIPStart.Text, this.BATStart.Text);
                            }
                            else
                            {
                                //记录关联数据信息到关联表
                                drs.Add(new DataRelativeSheet()
                                {
                                    IMEI1 = this.IMEI_Start.Text,
                                    IMEI2 = "",
                                    IMEI3 = this.SIMStart.Text,
                                    IMEI4 = this.ICCIDStart.Text,
                                    IMEI5 = "",
                                    IMEI6 = "",
                                    IMEI7 = "",
                                    IMEI8 = this.VIPStart.Text,
                                    IMEI9 = this.BATStart.Text,
                                    IMEI10 = "",
                                    IMEI11 = "",
                                    IMEI12 = "",
                                    ZhiDan = this.CB_ZhiDan.Text,
                                    TestTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff")
                                });
                                DRSB.InsertRelativeSheetBLL(drs);
                            }
                            if (PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                            {
                                PMB.UpdateVipAndBatBLL(this.IMEI_Start.Text, this.SIMStart.Text, this.VIPStart.Text, this.BATStart.Text, this.ICCIDStart.Text);
                            }
                            this.IMEI_Start.Clear();
                            this.SIMStart.Clear();
                            this.BATStart.Clear();
                            this.VIPStart.Clear();
                            this.ICCIDStart.Clear();
                            this.IMEI_Start.Focus();
                            if (this.updata_inline.Visible == true)
                            {
                                MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                statusChange();
                            }
                        }
                    }
                    switch (g)
                    {
                        case 12:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 20:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 36:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 13:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 21:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 37:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 14:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 22:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 38:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 15:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 23:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 39:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 28:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 44:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 29:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 45:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 30:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 46:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 31:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 47:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 60:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 61:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 62:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 63:
                            {
                                this.ICCIDStart.Focus();
                            }
                            break;
                        case 52:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 53:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 54:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 55:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                    }
                }
                else{
                    this.BATStart.Focus();
                }
            }
        }

        private void ICCIDStart_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                if (this.ICCIDStart.Text != "")
                {
                    if (this.ICCID_digits.Text != "" && this.ICCID_prefix.Text != "")
                    {
                        int iccid_width = this.ICCIDStart.Text.Length;
                        if (iccid_width != int.Parse(this.ICCID_digits.Text))
                        {
                            player.Play();
                            this.reminder.AppendText("ICCID不在范围内\r\n");
                            this.ICCIDStart.Clear();
                            this.ICCIDStart.Focus();
                            return;
                        }
                        else
                        {
                            int iccid_prefix_width = this.ICCID_prefix.Text.Length;
                            string iccid_prefix = this.ICCIDStart.Text.Substring(0, iccid_prefix_width);
                            if (iccid_prefix != this.ICCID_prefix.Text)
                            {
                                player.Play();
                                this.reminder.AppendText("ICCID不在范围内\r\n");
                                this.ICCIDStart.Clear();
                                this.ICCIDStart.Focus();
                                return;
                            }
                        }
                    }
                    else
                    {
                        player.Play();
                        this.reminder.AppendText("该制单未绑定ICCID\r\n");
                        this.ICCIDStart.Clear();
                        this.ICCIDStart.Focus();
                        return;
                    }
                    if (PMB.CheckICCIDBLL(this.ICCIDStart.Text))
                    {
                        player.Play();
                        this.reminder.AppendText("ICCID-"+ this.ICCIDStart.Text+"重号\r\n");
                        this.ICCIDStart.Clear();
                        this.ICCIDStart.Focus();
                        return;
                    }
                    if (g >= 8 && g <= 15)
                    {
                        if (this.NoPaper.Checked == false)
                        {
                            ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                            if (this.Select_Template1.Text != "" || this.Select_Template2.Text != "")
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    for (int S = 1; S <= 2; S++)
                                    {
                                        if (S == 1)
                                        {
                                            lj = this.Select_Template1.Text;
                                            btFormat = btEngine.Documents.Open(lj);
                                            //指定打印机名称
                                            btFormat.PrintSetup.PrinterName = this.Printer1.Text;
                                        }
                                        else
                                        {
                                            if (this.Select_Template2.Text == "")
                                            {
                                                //记录打印信息日志
                                                list.Add(new PrintMessage()
                                                {
                                                    Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                    IMEI = this.IMEI_Start.Text.Trim(),
                                                    IMEIStart = this.IMEI_num1.Text.Trim(),
                                                    IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                    SN = this.SN1_num.Text,
                                                    IMEIRel = this.IMEIRel.Text.Trim(),
                                                    SIM = this.SIMStart.Text.Trim(),
                                                    VIP = this.VIPStart.Text.Trim(),
                                                    BAT = this.BATStart.Text.Trim(),
                                                    SoftModel = this.SoftModel.Text.Trim(),
                                                    Version = this.SoftwareVersion.Text.Trim(),
                                                    Remark = this.Remake.Text.Trim(),
                                                    JS_PrintTime = "",
                                                    JS_TemplatePath = "",
                                                    CH_PrintTime = ProductTime,
                                                    CH_TemplatePath1 = this.Select_Template1.Text,
                                                    CH_TemplatePath2 = this.Select_Template2.Text,
                                                    ICCID = this.ICCIDStart.Text,
                                                    MAC = this.MACStart.Text,
                                                    Equipment = this.EquipmentStart.Text
                                                });
                                                if (PMB.InsertPrintMessageBLL(list))
                                                {
                                                    if (SN1_num.Text != "")
                                                    {
                                                        Form1.Log("关联ICCID打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                                        sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                        sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                        sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                        if (MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0')))
                                                        {
                                                            this.SN1_num.Text = sn1;
                                                            this.IMEI_Start.Clear();
                                                            this.SIMStart.Clear();
                                                            this.VIPStart.Clear();
                                                            this.BATStart.Clear();
                                                            this.ICCIDStart.Clear();
                                                            this.IMEI_Start.Focus();
                                                        }
                                                    }
                                                    else
                                                    {
                                                        Form1.Log("关联ICCID打印了彩盒贴IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                                        this.IMEI_Start.Clear();
                                                        this.SIMStart.Clear();
                                                        this.VIPStart.Clear();
                                                        this.BATStart.Clear();
                                                        this.ICCIDStart.Clear();
                                                        this.IMEI_Start.Focus();
                                                    }
                                                }
                                                if (this.updata_inline.Visible == true)
                                                {
                                                    MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                                    statusChange();
                                                }
                                                return;
                                            }
                                            else
                                            {
                                                lj = this.Select_Template2.Text;
                                                btFormat = btEngine.Documents.Open(lj);
                                                //指定打印机名称
                                                btFormat.PrintSetup.PrinterName = this.Printer2.Text;
                                            }
                                        }
                                        //对模板相应字段进行赋值
                                        ValuesToTemplate(btFormat);
                                        btFormat.SubStrings["IMEI"].Value = this.IMEI_Start.Text;
                                        btFormat.SubStrings["SN"].Value = this.SN1_num.Text;
                                        Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                    }
                                    //记录打印信息日志
                                    list.Add(new PrintMessage()
                                    {
                                        Zhidan = this.CB_ZhiDan.Text.Trim(),
                                        IMEI = this.IMEI_Start.Text.Trim(),
                                        IMEIStart = this.IMEI_num1.Text.Trim(),
                                        IMEIEnd = this.IMEI_num2.Text.Trim(),
                                        SN = this.SN1_num.Text,
                                        IMEIRel = this.IMEIRel.Text.Trim(),
                                        SIM = this.SIMStart.Text.Trim(),
                                        VIP = this.VIPStart.Text.Trim(),
                                        BAT = this.BATStart.Text.Trim(),
                                        SoftModel = this.SoftModel.Text.Trim(),
                                        Version = this.SoftwareVersion.Text.Trim(),
                                        Remark = this.Remake.Text.Trim(),
                                        JS_PrintTime = "",
                                        JS_TemplatePath = "",
                                        CH_PrintTime = ProductTime,
                                        CH_TemplatePath1 = this.Select_Template1.Text,
                                        CH_TemplatePath2 = this.Select_Template2.Text,
                                        ICCID = this.ICCIDStart.Text,
                                        MAC = this.MACStart.Text,
                                        Equipment = this.EquipmentStart.Text
                                    });
                                    if (PMB.InsertPrintMessageBLL(list))
                                    {
                                        if (SN1_num.Text != "")
                                        {
                                            Form1.Log("关联ICCID打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                            sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                            sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                            sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                            if (MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0')))
                                            {
                                                this.SN1_num.Text = sn1;
                                                this.IMEI_Start.Clear();
                                                this.SIMStart.Clear();
                                                this.VIPStart.Clear();
                                                this.BATStart.Clear();
                                                this.ICCIDStart.Clear();
                                                this.IMEI_Start.Focus();
                                            }
                                        }
                                        else
                                        {
                                            Form1.Log("关联ICCID打印了彩盒贴IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                            this.IMEI_Start.Clear();
                                            this.SIMStart.Clear();
                                            this.VIPStart.Clear();
                                            this.BATStart.Clear();
                                            this.ICCIDStart.Clear();
                                            this.IMEI_Start.Focus();
                                        }
                                    }
                                }
                                else
                                {
                                    for (int S = 1; S <= 2; S++)
                                    {
                                        if (S == 1)
                                        {
                                            lj = this.Select_Template1.Text;
                                            btFormat = btEngine.Documents.Open(lj);
                                            //指定打印机名称
                                            btFormat.PrintSetup.PrinterName = this.Printer1.Text;
                                        }
                                        else
                                        {
                                            if (this.Select_Template2.Text == "")
                                            {
                                                this.IMEI_Start.Clear();
                                                this.SIMStart.Clear();
                                                this.VIPStart.Clear();
                                                this.BATStart.Clear();
                                                this.ICCIDStart.Clear();
                                                this.IMEI_Start.Focus();
                                                if (this.updata_inline.Visible == true)
                                                {
                                                    MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                                    statusChange();
                                                }
                                                return;
                                            }
                                            else
                                            {
                                                lj = this.Select_Template2.Text;
                                                btFormat = btEngine.Documents.Open(lj);
                                                //指定打印机名称
                                                btFormat.PrintSetup.PrinterName = this.Printer2.Text;
                                            }
                                        }
                                        //对模板相应字段进行赋值
                                        ValuesToTemplate(btFormat);
                                        btFormat.SubStrings["IMEI"].Value = this.IMEI_Start.Text;

                                        list = PMB.SelectSnByIMEIBLL(this.IMEI_Start.Text);
                                        foreach (PrintMessage a in list)
                                        {
                                            if (a.SN != "")
                                            {
                                                btFormat.SubStrings["SN"].Value = a.SN;
                                                if (S == 2 || this.Select_Template2.Text=="")
                                                {
                                                    DRSB.UpdateAssociatedBLL(this.IMEI_Start.Text, this.VIPStart.Text, this.BATStart.Text, this.MACStart.Text, this.EquipmentStart.Text);
                                                    MOPB.UpdateCHAssociatedBLL(this.IMEI_Start.Text, ProductTime, this.Select_Template1.Text, this.Select_Template2.Text, this.SIMStart.Text, this.VIPStart.Text, this.BATStart.Text, this.ICCIDStart.Text, this.MACStart.Text, this.EquipmentStart.Text, a.SN);
                                                }
                                            }
                                            else
                                            {
                                                btFormat.SubStrings["SN"].Value = this.SN1_num.Text;
                                                if (S == 2 || this.Select_Template2.Text == "")
                                                {
                                                    DRSB.UpdateAssociatedBLL(this.IMEI_Start.Text, this.VIPStart.Text, this.BATStart.Text, this.MACStart.Text, this.EquipmentStart.Text);
                                                    MOPB.UpdateCHAssociatedBLL(this.IMEI_Start.Text, ProductTime, this.Select_Template1.Text, this.Select_Template2.Text, this.SIMStart.Text, this.VIPStart.Text, this.BATStart.Text, this.ICCIDStart.Text, this.MACStart.Text, this.EquipmentStart.Text, this.SN1_num.Text);
                                                    if (this.SN1_num.Text != "")
                                                    {
                                                        sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                        sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                        sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                        MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0'));
                                                        this.SN1_num.Text = sn1;
                                                    }
                                                }
                                            }
                                        }
                                        Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                    }
                                    Form1.Log("关联ICCID打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                    this.IMEI_Start.Clear();
                                    this.SIMStart.Clear();
                                    this.VIPStart.Clear();
                                    this.BATStart.Clear();
                                    this.ICCIDStart.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                if (this.updata_inline.Visible == true)
                                {
                                    MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                    statusChange();
                                }
                            }
                            else
                            {
                                player1.Play();
                                this.reminder.AppendText("请先选择模板\r\n");
                                this.ICCIDStart.Clear();
                                this.ICCIDStart.Focus();
                            }
                        }
                    }
                    switch (g) {
                        case 24:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 40:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 25:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 41:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 26:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 42:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 27:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 43:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 28:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 44:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 29:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 45:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 30:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 46:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 31:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 47:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 56:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 57:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 58:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 59:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 60:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 61:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 62:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                        case 63:
                            {
                                this.MACStart.Focus();
                            }
                            break;
                    }
                }
                else {
                    this.ICCIDStart.Focus();
                }
            }
        }

        private void MACStart_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                if (this.MACStart.Text != "")
                {
                    if (this.MAC_digits.Text != "" && this.MAC_prefix.Text != "")
                    {
                        int mac_width = this.MACStart.Text.Length;
                        if (mac_width != int.Parse(this.MAC_digits.Text))
                        {
                            player.Play();
                            this.reminder.AppendText("MAC不在范围内\r\n");
                            this.MACStart.Clear();
                            this.MACStart.Focus();
                            return;
                        }
                        else
                        {
                            int mac_prefix_width = this.MAC_prefix.Text.Length;
                            string mac_prefix = this.MACStart.Text.Substring(0, mac_prefix_width);
                            if (mac_prefix != this.MAC_prefix.Text)
                            {
                                player.Play();
                                this.reminder.AppendText("MAC不在范围内\r\n");
                                this.MACStart.Clear();
                                this.MACStart.Focus();
                                return;
                            }
                        }
                    }
                    else
                    {
                        player.Play();
                        this.reminder.AppendText("该制单未绑定MAC\r\n");
                        this.MACStart.Clear();
                        this.MACStart.Focus();
                        return;
                    }
                    if (PMB.CheckMACBLL(this.MACStart.Text))
                    {
                        player.Play();
                        this.reminder.AppendText("MAC"+this.MACStart.Text+"重号\r\n");
                        this.MACStart.Clear();
                        this.MACStart.Focus();
                        return;
                    }
                    if (g >= 16 && g <= 31)
                    {
                        if (this.NoPaper.Checked == false)
                        {
                            ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                            if (this.Select_Template1.Text != "" || this.Select_Template2.Text != "")
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    ASS_sn = DRSB.SelectSNByImeiBLL(this.IMEI_Start.Text);
                                    if (ASS_sn == "")
                                    {
                                        ASS_sn = this.SN1_num.Text;
                                        Sn_mark = 1;
                                    }
                                    for (int S = 1; S <= 2; S++)
                                    {
                                        if (S == 1)
                                        {
                                            lj = this.Select_Template1.Text;
                                            btFormat = btEngine.Documents.Open(lj);
                                            //指定打印机名称
                                            btFormat.PrintSetup.PrinterName = this.Printer1.Text;
                                        }
                                        else
                                        {
                                            if (this.Select_Template2.Text == "")
                                            {
                                                //记录打印信息日志
                                                list.Add(new PrintMessage()
                                                {
                                                    Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                    IMEI = this.IMEI_Start.Text.Trim(),
                                                    IMEIStart = this.IMEI_num1.Text.Trim(),
                                                    IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                    SN = ASS_sn,
                                                    IMEIRel = this.IMEIRel.Text.Trim(),
                                                    SIM = this.SIMStart.Text.Trim(),
                                                    VIP = this.VIPStart.Text.Trim(),
                                                    BAT = this.BATStart.Text.Trim(),
                                                    SoftModel = this.SoftModel.Text.Trim(),
                                                    Version = this.SoftwareVersion.Text.Trim(),
                                                    Remark = this.Remake.Text.Trim(),
                                                    JS_PrintTime = "",
                                                    JS_TemplatePath = "",
                                                    CH_PrintTime = ProductTime,
                                                    CH_TemplatePath1 = this.Select_Template1.Text,
                                                    CH_TemplatePath2 = this.Select_Template2.Text,
                                                    ICCID = this.ICCIDStart.Text,
                                                    MAC = this.MACStart.Text,
                                                    Equipment = this.EquipmentStart.Text
                                                });
                                                if (PMB.InsertPrintMessageBLL(list))
                                                {
                                                    DRSB.UpdateVipAndBatAndMacBLL(this.IMEI_Start.Text, this.VIPStart.Text, this.BATStart.Text, this.MACStart.Text);
                                                    if (SN1_num.Text != "" && Sn_mark==1)
                                                    {
                                                        Form1.Log("关联MAC打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                                        sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                        sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                        sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                        if (MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0')))
                                                        {
                                                            this.SN1_num.Text = sn1;
                                                            this.IMEI_Start.Clear();
                                                            this.SIMStart.Clear();
                                                            this.VIPStart.Clear();
                                                            this.BATStart.Clear();
                                                            this.ICCIDStart.Clear();
                                                            this.MACStart.Clear();
                                                            this.ShowSN.Clear();
                                                            this.IMEI_Start.Focus();
                                                        }
                                                    }
                                                    else
                                                    {
                                                        Form1.Log("关联MAC打印了彩盒贴IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                                        this.IMEI_Start.Clear();
                                                        this.SIMStart.Clear();
                                                        this.VIPStart.Clear();
                                                        this.BATStart.Clear();
                                                        this.ICCIDStart.Clear();
                                                        this.MACStart.Clear();
                                                        this.ShowSN.Clear();
                                                        this.IMEI_Start.Focus();
                                                    }
                                                }
                                                if (this.updata_inline.Visible == true)
                                                {
                                                    MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                                    statusChange();
                                                }
                                                return;
                                            }
                                            else
                                            {
                                                lj = this.Select_Template2.Text;
                                                btFormat = btEngine.Documents.Open(lj);
                                                //指定打印机名称
                                                btFormat.PrintSetup.PrinterName = this.Printer2.Text;
                                            }
                                        }
                                        //对模板相应字段进行赋值
                                        ValuesToTemplate(btFormat);
                                        btFormat.SubStrings["IMEI"].Value = this.IMEI_Start.Text;
                                        btFormat.SubStrings["SN"].Value = ASS_sn;
                                        this.ShowSN.Text = ASS_sn;
                                        //打印份数,同序列打印的份数
                                        btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                                        Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                    }
                                    //记录打印信息日志
                                    list.Add(new PrintMessage()
                                    {
                                        Zhidan = this.CB_ZhiDan.Text.Trim(),
                                        IMEI = this.IMEI_Start.Text.Trim(),
                                        IMEIStart = this.IMEI_num1.Text.Trim(),
                                        IMEIEnd = this.IMEI_num2.Text.Trim(),
                                        SN = ASS_sn,
                                        IMEIRel = this.IMEIRel.Text.Trim(),
                                        SIM = this.SIMStart.Text.Trim(),
                                        VIP = this.VIPStart.Text.Trim(),
                                        BAT = this.BATStart.Text.Trim(),
                                        SoftModel = this.SoftModel.Text.Trim(),
                                        Version = this.SoftwareVersion.Text.Trim(),
                                        Remark = this.Remake.Text.Trim(),
                                        JS_PrintTime = "",
                                        JS_TemplatePath = "",
                                        CH_PrintTime = ProductTime,
                                        CH_TemplatePath1 = this.Select_Template1.Text,
                                        CH_TemplatePath2 = this.Select_Template2.Text,
                                        ICCID = this.ICCIDStart.Text,
                                        MAC = this.MACStart.Text,
                                        Equipment = this.EquipmentStart.Text
                                    });
                                    if (PMB.InsertPrintMessageBLL(list))
                                    {
                                        DRSB.UpdateVipAndBatAndMacBLL(this.IMEI_Start.Text, this.VIPStart.Text, this.BATStart.Text, this.MACStart.Text);
                                        if (SN1_num.Text != "" && Sn_mark==1)
                                        {
                                            Form1.Log("关联MAC打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                            sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                            sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                            sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                            if (MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0')))
                                            {
                                                this.SN1_num.Text = sn1;
                                                this.IMEI_Start.Clear();
                                                this.SIMStart.Clear();
                                                this.VIPStart.Clear();
                                                this.BATStart.Clear();
                                                this.ICCIDStart.Clear();
                                                this.MACStart.Clear();
                                                this.ShowSN.Clear();
                                                this.IMEI_Start.Focus();
                                            }
                                        }
                                        else
                                        {
                                            Form1.Log("关联MAC打印了彩盒贴IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                            this.IMEI_Start.Clear();
                                            this.SIMStart.Clear();
                                            this.VIPStart.Clear();
                                            this.BATStart.Clear();
                                            this.ICCIDStart.Clear();
                                            this.MACStart.Clear();
                                            this.ShowSN.Clear();
                                            this.IMEI_Start.Focus();
                                        }
                                    }
                                }
                                else
                                {
                                    ASS_sn = DRSB.SelectSNByImeiBLL(this.IMEI_Start.Text);
                                    if (ASS_sn == "")
                                    {
                                        ASS_sn = this.SN1_num.Text;
                                        Sn_mark = 1;
                                    }
                                    for (int S = 1; S <= 2; S++)
                                    {
                                        if (S == 1)
                                        {
                                            lj = this.Select_Template1.Text;
                                            btFormat = btEngine.Documents.Open(lj);
                                            //指定打印机名称
                                            btFormat.PrintSetup.PrinterName = this.Printer1.Text;
                                        }
                                        else
                                        {
                                            if (this.Select_Template2.Text == "")
                                            {
                                                Form1.Log("关联MAC打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                                this.IMEI_Start.Clear();
                                                this.SIMStart.Clear();
                                                this.VIPStart.Clear();
                                                this.BATStart.Clear();
                                                this.ICCIDStart.Clear();
                                                this.MACStart.Clear();
                                                this.ShowSN.Clear();
                                                this.IMEI_Start.Focus();
                                                if (this.updata_inline.Visible == true)
                                                {
                                                    MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                                    statusChange();
                                                }
                                                return;
                                            }
                                            else
                                            {
                                                lj = this.Select_Template2.Text;
                                                btFormat = btEngine.Documents.Open(lj);
                                                //指定打印机名称
                                                btFormat.PrintSetup.PrinterName = this.Printer2.Text;
                                            }
                                        }
                                        //对模板相应字段进行赋值
                                        ValuesToTemplate(btFormat);
                                        btFormat.SubStrings["IMEI"].Value = this.IMEI_Start.Text;

                                        list = PMB.SelectSnByIMEIBLL(this.IMEI_Start.Text);
                                        foreach (PrintMessage a in list)
                                        {
                                            if (a.SN != "")
                                            {
                                                btFormat.SubStrings["SN"].Value = a.SN;
                                                this.ShowSN.Text = a.SN;
                                                if (S == 2 || this.Select_Template2.Text == "")
                                                {
                                                    DRSB.UpdateVipAndBatAndMacBLL(this.IMEI_Start.Text, this.VIPStart.Text, this.BATStart.Text, this.MACStart.Text);
                                                    PMB.UpdateSN_SIM_VIP_BAT_ICCID_MACBLL(this.IMEI_Start.Text, ProductTime, this.Select_Template1.Text, this.Select_Template2.Text, this.SIMStart.Text, this.VIPStart.Text, this.BATStart.Text, this.ICCIDStart.Text, this.MACStart.Text, a.SN);
                                                }
                                            }
                                            else
                                            {
                                                if (Sn_mark == 1)
                                                {
                                                    btFormat.SubStrings["SN"].Value = this.SN1_num.Text;
                                                    this.ShowSN.Text = this.SN1_num.Text;
                                                    if (S == 2 || this.Select_Template2.Text == "")
                                                    {
                                                        DRSB.UpdateVipAndBatAndMacBLL(this.IMEI_Start.Text, this.VIPStart.Text, this.BATStart.Text, this.MACStart.Text);
                                                        PMB.UpdateSN_SIM_VIP_BAT_ICCID_MACBLL(this.IMEI_Start.Text, ProductTime, this.Select_Template1.Text, this.Select_Template2.Text, this.SIMStart.Text, this.VIPStart.Text, this.BATStart.Text, this.ICCIDStart.Text, this.MACStart.Text, this.SN1_num.Text);
                                                        if (this.SN1_num.Text != "")
                                                        {
                                                            sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                            sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                            sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                            MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0'));
                                                            this.SN1_num.Text = sn1;
                                                        }
                                                    }
                                                }
                                                else
                                                {
                                                    btFormat.SubStrings["SN"].Value = ASS_sn;
                                                    this.ShowSN.Text = ASS_sn;
                                                    DRSB.UpdateVipAndBatAndMacBLL(this.IMEI_Start.Text, this.VIPStart.Text, this.BATStart.Text, this.MACStart.Text);
                                                    PMB.UpdateSN_SIM_VIP_BAT_ICCID_MACBLL(this.IMEI_Start.Text, ProductTime, this.Select_Template1.Text, this.Select_Template2.Text, this.SIMStart.Text, this.VIPStart.Text, this.BATStart.Text, this.ICCIDStart.Text, this.MACStart.Text, ASS_sn);
                                                }
                                            }
                                        }
                                        //打印份数,同序列打印的份数
                                        btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                                        Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                    }
                                    Form1.Log("关联MAC打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                    this.IMEI_Start.Clear();
                                    this.SIMStart.Clear();
                                    this.VIPStart.Clear();
                                    this.BATStart.Clear();
                                    this.ICCIDStart.Clear();
                                    this.MACStart.Clear();
                                    this.ShowSN.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                if (this.updata_inline.Visible == true)
                                {
                                    MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                    statusChange();
                                }
                            }
                            else
                            {
                                player1.Play();
                                this.reminder.AppendText("请先选择模板\r\n");
                                this.MACStart.Clear();
                                this.MACStart.Focus();
                            }
                        }
                        else
                        {
                            if (DRSB.CheckIMEIBLL(this.IMEI_Start.Text))
                            {
                                DRSB.UpdateVipAndBatAndMacBLL(this.IMEI_Start.Text, this.VIPStart.Text, this.BATStart.Text, this.MACStart.Text);
                            }
                            else
                            {
                                //记录关联数据信息到关联表
                                drs.Add(new DataRelativeSheet()
                                {
                                    IMEI1 = this.IMEI_Start.Text,
                                    IMEI2 = "",
                                    IMEI3 = this.SIMStart.Text,
                                    IMEI4 = this.ICCIDStart.Text,
                                    IMEI5 = "",
                                    IMEI6 = this.MACStart.Text,
                                    IMEI7 = "",
                                    IMEI8 = this.VIPStart.Text,
                                    IMEI9 = this.BATStart.Text,
                                    IMEI10 = "",
                                    IMEI11 = "",
                                    IMEI12 = "",
                                    ZhiDan = this.CB_ZhiDan.Text,
                                    TestTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff")
                                });
                                DRSB.InsertRelativeSheetBLL(drs);
                            }
                            if (PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                            {
                                PMB.UpdateVipAndBatAndMacBLL(this.IMEI_Start.Text, this.SIMStart.Text, this.VIPStart.Text, this.BATStart.Text, this.ICCIDStart.Text, this.MACStart.Text);
                            }
                            this.IMEI_Start.Clear();
                            this.SIMStart.Clear();
                            this.BATStart.Clear();
                            this.VIPStart.Clear();
                            this.ICCIDStart.Clear();
                            this.MACStart.Clear();
                            this.IMEI_Start.Focus();
                            if (this.updata_inline.Visible == true)
                            {
                                MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                statusChange();
                            }
                        }
                    }
                    switch (g) {
                        case 48:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 49:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 50:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 51:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 52:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 53:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 54:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 55:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 56:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 57:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 58:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 59:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 60:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 61:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 62:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                        case 63:
                            {
                                this.EquipmentStart.Focus();
                            }
                            break;
                    }
                }
                else {
                    this.MACStart.Focus();
                }
            }
        }

        private void EquipmentStart_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                if (this.EquipmentStart.Text != "")
                {
                    if (this.Equipment_digits.Text != "" && this.Equipment_prefix.Text != "")
                    {
                        int Equipment_width = this.EquipmentStart.Text.Length;
                        if (Equipment_width != int.Parse(this.Equipment_digits.Text))
                        {
                            player.Play();
                            this.reminder.AppendText("设备号不在范围内\r\n");
                            this.EquipmentStart.Clear();
                            this.EquipmentStart.Focus();
                            return;
                        }
                        else
                        {
                            int Equipment_prefix_width = this.Equipment_prefix.Text.Length;
                            string Equipment_prefix = this.EquipmentStart.Text.Substring(0, Equipment_prefix_width);
                            if (Equipment_prefix != this.Equipment_prefix.Text)
                            {
                                player.Play();
                                this.reminder.AppendText("设备号不在范围内\r\n");
                                this.EquipmentStart.Clear();
                                this.EquipmentStart.Focus();
                                return;
                            }
                        }
                    }
                    else
                    {
                        player.Play();
                        this.reminder.AppendText("该制单未绑定设备号\r\n");
                        this.EquipmentStart.Clear();
                        this.EquipmentStart.Focus();
                        return;
                    }
                    if (PMB.CheckEquipmentBLL(this.EquipmentStart.Text))
                    {
                        player.Play();
                        this.reminder.AppendText("设备号-"+this.EquipmentStart.Text+"重号\r\n");
                        this.EquipmentStart.Clear();
                        this.EquipmentStart.Focus();
                        return;
                    }
                    if (g >= 32 && g <= 63)
                    {
                        if (this.NoPaper.Checked == false)
                        {
                            ProductTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff");
                            if (this.Select_Template1.Text != "" || this.Select_Template2.Text != "")
                            {
                                if (!PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                                {
                                    ASS_sn = DRSB.SelectSNByImeiBLL(this.IMEI_Start.Text);
                                    if (ASS_sn == "")
                                    {
                                        ASS_sn = this.SN1_num.Text;
                                        Sn_mark = 1;
                                    }
                                    for (int S = 1; S <= 2; S++)
                                    {
                                        if (S == 1)
                                        {
                                            lj = this.Select_Template1.Text;
                                            btFormat = btEngine.Documents.Open(lj);
                                            //指定打印机名称
                                            btFormat.PrintSetup.PrinterName = this.Printer1.Text;
                                        }
                                        else
                                        {
                                            if (this.Select_Template2.Text == "")
                                            {
                                                //记录打印信息日志
                                                list.Add(new PrintMessage()
                                                {
                                                    Zhidan = this.CB_ZhiDan.Text.Trim(),
                                                    IMEI = this.IMEI_Start.Text.Trim(),
                                                    IMEIStart = this.IMEI_num1.Text.Trim(),
                                                    IMEIEnd = this.IMEI_num2.Text.Trim(),
                                                    SN = ASS_sn,
                                                    IMEIRel = this.IMEIRel.Text.Trim(),
                                                    SIM = this.SIMStart.Text.Trim(),
                                                    VIP = this.VIPStart.Text.Trim(),
                                                    BAT = this.BATStart.Text.Trim(),
                                                    SoftModel = this.SoftModel.Text.Trim(),
                                                    Version = this.SoftwareVersion.Text.Trim(),
                                                    Remark = this.Remake.Text.Trim(),
                                                    JS_PrintTime = "",
                                                    JS_TemplatePath = "",
                                                    CH_PrintTime = ProductTime,
                                                    CH_TemplatePath1 = this.Select_Template1.Text,
                                                    CH_TemplatePath2 = this.Select_Template2.Text,
                                                    ICCID = this.ICCIDStart.Text,
                                                    MAC = this.MACStart.Text,
                                                    Equipment = this.EquipmentStart.Text
                                                });
                                                if (PMB.InsertPrintMessageBLL(list))
                                                {
                                                    DRSB.UpdateVipAndBatAndMacAndEquBLL(this.IMEI_Start.Text, this.VIPStart.Text, this.BATStart.Text, this.MACStart.Text, this.EquipmentStart.Text);
                                                    if (SN1_num.Text != "")
                                                    {
                                                        Form1.Log("关联设备号打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                                        sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                        sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                        sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                        if (MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0')))
                                                        {
                                                            this.SN1_num.Text = sn1;
                                                            this.IMEI_Start.Clear();
                                                            this.SIMStart.Clear();
                                                            this.VIPStart.Clear();
                                                            this.BATStart.Clear();
                                                            this.ICCIDStart.Clear();
                                                            this.MACStart.Clear();
                                                            this.EquipmentStart.Clear();
                                                            this.ShowSN.Clear();
                                                            this.IMEI_Start.Focus();
                                                        }
                                                    }
                                                    else
                                                    {
                                                        Form1.Log("关联设备号打印了彩盒贴IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                                        this.IMEI_Start.Clear();
                                                        this.SIMStart.Clear();
                                                        this.VIPStart.Clear();
                                                        this.BATStart.Clear();
                                                        this.ICCIDStart.Clear();
                                                        this.MACStart.Clear();
                                                        this.EquipmentStart.Clear();
                                                        this.ShowSN.Clear();
                                                        this.IMEI_Start.Focus();
                                                    }
                                                }
                                                if (this.updata_inline.Visible == true)
                                                {
                                                    MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                                    statusChange();
                                                }
                                                return;
                                            }
                                            else
                                            {
                                                lj = this.Select_Template2.Text;
                                                btFormat = btEngine.Documents.Open(lj);
                                                //指定打印机名称
                                                btFormat.PrintSetup.PrinterName = this.Printer2.Text;
                                            }
                                        }
                                        //对模板相应字段进行赋值
                                        ValuesToTemplate(btFormat);
                                        btFormat.SubStrings["IMEI"].Value = this.IMEI_Start.Text;
                                        btFormat.SubStrings["SN"].Value = ASS_sn;
                                        this.ShowSN.Text = ASS_sn; 
                                        //打印份数,同序列打印的份数
                                        btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                                        Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                    }
                                    //记录打印信息日志
                                    list.Add(new PrintMessage()
                                    {
                                        Zhidan = this.CB_ZhiDan.Text.Trim(),
                                        IMEI = this.IMEI_Start.Text.Trim(),
                                        IMEIStart = this.IMEI_num1.Text.Trim(),
                                        IMEIEnd = this.IMEI_num2.Text.Trim(),
                                        SN = ASS_sn,
                                        IMEIRel = this.IMEIRel.Text.Trim(),
                                        SIM = this.SIMStart.Text.Trim(),
                                        VIP = this.VIPStart.Text.Trim(),
                                        BAT = this.BATStart.Text.Trim(),
                                        SoftModel = this.SoftModel.Text.Trim(),
                                        Version = this.SoftwareVersion.Text.Trim(),
                                        Remark = this.Remake.Text.Trim(),
                                        JS_PrintTime = "",
                                        JS_TemplatePath = "",
                                        CH_PrintTime = ProductTime,
                                        CH_TemplatePath1 = this.Select_Template1.Text,
                                        CH_TemplatePath2 = this.Select_Template2.Text,
                                        ICCID = this.ICCIDStart.Text,
                                        MAC = this.MACStart.Text,
                                        Equipment = this.EquipmentStart.Text
                                    });
                                    if (PMB.InsertPrintMessageBLL(list))
                                    {
                                        DRSB.UpdateVipAndBatAndMacAndEquBLL(this.IMEI_Start.Text, this.VIPStart.Text, this.BATStart.Text, this.MACStart.Text, this.EquipmentStart.Text);
                                        if (SN1_num.Text != "" && Sn_mark ==1)
                                        {
                                            Form1.Log("关联设备号打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                            sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                            sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                            sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                            if (MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0')))
                                            {
                                                this.SN1_num.Text = sn1;
                                                this.IMEI_Start.Clear();
                                                this.SIMStart.Clear();
                                                this.VIPStart.Clear();
                                                this.BATStart.Clear();
                                                this.ICCIDStart.Clear();
                                                this.MACStart.Clear();
                                                this.EquipmentStart.Clear();
                                                this.ShowSN.Clear();
                                                this.IMEI_Start.Focus();
                                            }
                                        }
                                        else
                                        {
                                            Form1.Log("关联设备号打印了彩盒贴IMEI号为" + this.IMEI_Start.Text + "的制单", null);
                                            this.IMEI_Start.Clear();
                                            this.SIMStart.Clear();
                                            this.VIPStart.Clear();
                                            this.BATStart.Clear();
                                            this.ICCIDStart.Clear();
                                            this.MACStart.Clear();
                                            this.EquipmentStart.Clear();
                                            this.ShowSN.Clear();
                                            this.IMEI_Start.Focus();
                                        }
                                    }
                                }
                                else
                                {
                                    ASS_sn = DRSB.SelectSNByImeiBLL(this.IMEI_Start.Text);
                                    if (ASS_sn == "")
                                    {
                                        ASS_sn = this.SN1_num.Text;
                                        Sn_mark = 1;
                                    }
                                    for (int S = 1; S <= 2; S++)
                                    {
                                        if (S == 1)
                                        {
                                            lj = this.Select_Template1.Text;
                                            btFormat = btEngine.Documents.Open(lj);
                                            //指定打印机名称
                                            btFormat.PrintSetup.PrinterName = this.Printer1.Text;
                                        }
                                        else
                                        {
                                            if (this.Select_Template2.Text == "")
                                            {
                                                this.IMEI_Start.Clear();
                                                this.SIMStart.Clear();
                                                this.VIPStart.Clear();
                                                this.BATStart.Clear();
                                                this.ICCIDStart.Clear();
                                                this.MACStart.Clear();
                                                this.EquipmentStart.Clear();
                                                this.ShowSN.Clear();
                                                this.IMEI_Start.Focus();
                                                if (this.updata_inline.Visible == true)
                                                {
                                                    MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                                    statusChange();
                                                }
                                                return;
                                            }
                                            else
                                            {
                                                lj = this.Select_Template2.Text;
                                                btFormat = btEngine.Documents.Open(lj);
                                                //指定打印机名称
                                                btFormat.PrintSetup.PrinterName = this.Printer2.Text;
                                            }
                                        }
                                        //对模板相应字段进行赋值
                                        ValuesToTemplate(btFormat);
                                        btFormat.SubStrings["IMEI"].Value = this.IMEI_Start.Text;

                                        list = PMB.SelectSnByIMEIBLL(this.IMEI_Start.Text);
                                        foreach (PrintMessage a in list)
                                        {
                                            if (a.SN != "")
                                            {
                                                btFormat.SubStrings["SN"].Value = a.SN;
                                                this.ShowSN.Text = a.SN;
                                                if (S == 2 || this.Select_Template2.Text == "")
                                                {
                                                    DRSB.UpdateVipAndBatAndMacAndEquBLL(this.IMEI_Start.Text, this.VIPStart.Text, this.BATStart.Text, this.MACStart.Text, this.EquipmentStart.Text);
                                                    PMB.UpdateSN_SIM_VIP_BAT_ICCID_MAC_EquipmentBLL(this.IMEI_Start.Text, ProductTime, this.Select_Template1.Text, this.Select_Template2.Text, this.SIMStart.Text, this.VIPStart.Text, this.BATStart.Text, this.ICCIDStart.Text, this.MACStart.Text, this.EquipmentStart.Text, a.SN);
                                                }
                                            }
                                            else
                                            {
                                                if (Sn_mark == 1)
                                                {
                                                    btFormat.SubStrings["SN"].Value = this.SN1_num.Text;
                                                    this.ShowSN.Text = this.SN1_num.Text;
                                                    if (S == 2 || this.Select_Template2.Text == "")
                                                    {
                                                        DRSB.UpdateVipAndBatAndMacAndEquBLL(this.IMEI_Start.Text, this.VIPStart.Text, this.BATStart.Text, this.MACStart.Text, this.EquipmentStart.Text);
                                                        PMB.UpdateSN_SIM_VIP_BAT_ICCID_MAC_EquipmentBLL(this.IMEI_Start.Text, ProductTime, this.Select_Template1.Text, this.Select_Template2.Text, this.SIMStart.Text, this.VIPStart.Text, this.BATStart.Text, this.ICCIDStart.Text, this.MACStart.Text, this.EquipmentStart.Text, this.SN1_num.Text);
                                                        if (this.SN1_num.Text != "")
                                                        {
                                                            sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
                                                            sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
                                                            sn1 = sn1_prefix + (sn1_suffix + 1).ToString().PadLeft(s, '0');
                                                            MOPB.UpdateSNAddOneBLL(this.CB_ZhiDan.Text, (sn1_suffix + 1).ToString().PadLeft(s, '0'));
                                                            this.SN1_num.Text = sn1;
                                                        }
                                                    }
                                                }
                                                else
                                                {
                                                    btFormat.SubStrings["SN"].Value = ASS_sn;
                                                    this.ShowSN.Text = ASS_sn;
                                                    DRSB.UpdateVipAndBatAndMacAndEquBLL(this.IMEI_Start.Text, this.VIPStart.Text, this.BATStart.Text, this.MACStart.Text, this.EquipmentStart.Text);
                                                    PMB.UpdateSN_SIM_VIP_BAT_ICCID_MAC_EquipmentBLL(this.IMEI_Start.Text, ProductTime, this.Select_Template1.Text, this.Select_Template2.Text, this.SIMStart.Text, this.VIPStart.Text, this.BATStart.Text, this.ICCIDStart.Text, this.MACStart.Text, this.EquipmentStart.Text, ASS_sn);
                                                }
                                            }
                                        }
                                        //打印份数,同序列打印的份数
                                        btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                                        Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                    }
                                    Form1.Log("关联设备号打印了IMEI号为" + this.IMEI_Start.Text + "的彩盒贴制单", null);
                                    this.IMEI_Start.Clear();
                                    this.SIMStart.Clear();
                                    this.VIPStart.Clear();
                                    this.BATStart.Clear();
                                    this.ICCIDStart.Clear();
                                    this.MACStart.Clear();
                                    this.EquipmentStart.Clear();
                                    this.ShowSN.Clear();
                                    this.IMEI_Start.Focus();
                                }
                                if (this.updata_inline.Visible == true)
                                {
                                    MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                    statusChange();
                                }
                            }
                            else
                            {
                                player1.Play();
                                this.reminder.AppendText("请先选择模板\r\n");
                                this.EquipmentStart.Clear();
                                this.EquipmentStart.Focus();
                            }
                        }
                        else
                        {
                            if (DRSB.CheckIMEIBLL(this.IMEI_Start.Text))
                            {
                               DRSB.UpdateVipAndBatAndMacAndEquBLL(this.IMEI_Start.Text, this.VIPStart.Text, this.BATStart.Text, this.MACStart.Text, this.EquipmentStart.Text);
                            }
                            else
                            {
                                //记录关联数据信息到关联表
                                drs.Add(new DataRelativeSheet()
                                {
                                    IMEI1 = this.IMEI_Start.Text,
                                    IMEI2 = "",
                                    IMEI3 = this.SIMStart.Text,
                                    IMEI4 = this.ICCIDStart.Text,
                                    IMEI5 = "",
                                    IMEI6 = this.MACStart.Text,
                                    IMEI7 = this.EquipmentStart.Text,
                                    IMEI8 = this.VIPStart.Text,
                                    IMEI9 = this.BATStart.Text,
                                    IMEI10 = "",
                                    IMEI11 = "",
                                    IMEI12 = "",
                                    ZhiDan = this.CB_ZhiDan.Text,
                                    TestTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff")
                                });
                                DRSB.InsertRelativeSheetBLL(drs);
                            }
                            if (PMB.CheckIMEIBLL(this.IMEI_Start.Text))
                            {
                                PMB.UpdateVipAndBatAndMacAndEquBLL(this.IMEI_Start.Text, this.SIMStart.Text, this.VIPStart.Text, this.BATStart.Text, this.ICCIDStart.Text, this.MACStart.Text, this.EquipmentStart.Text);
                            }
                            this.IMEI_Start.Clear();
                            this.SIMStart.Clear();
                            this.BATStart.Clear();
                            this.VIPStart.Clear();
                            this.ICCIDStart.Clear();
                            this.MACStart.Clear();
                            this.EquipmentStart.Clear();
                            this.IMEI_Start.Focus();
                            if (this.updata_inline.Visible == true)
                            {
                                MOPB.UpdateStatusByZhiDanBLL(this.CB_ZhiDan.Text);
                                statusChange();
                            }
                        }
                    }
                }
                else {
                    this.EquipmentStart.Focus();
                }
            }
        }

        private void Refresh_Click(object sender, EventArgs e)
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
                ClearAll();
                this.CB_ZhiDan.Text = "";
                this.Select_Template1.Clear();
                this.Select_Template2.Clear();
                this.SoftModel.Clear();
                this.SN1_num.Clear();
                this.SN2_num.Clear();
                this.ProductDate.Clear();
                this.Color.Clear();
                this.Weight.Clear();
                this.ProductNo.Clear();
                this.SoftwareVersion.Clear();
                this.IMEI_num1.Clear();
                this.IMEI_num2.Clear();
                this.SIM_num1.Clear();
                this.SIM_num2.Clear();
                this.SIM_digits.Clear();
                this.SIM_prefix.Clear();
                this.BAT_num1.Clear();
                this.BAT_num2.Clear();
                this.BAT_digits.Clear();
                this.BAT_prefix.Clear();
                this.VIP_num1.Clear();
                this.VIP_num2.Clear();
                this.VIP_digits.Clear();
                this.VIP_prefix.Clear();
                this.ICCID_digits.Clear();
                this.ICCID_prefix.Clear();
                this.MAC_digits.Clear();
                this.MAC_prefix.Clear();
                this.Equipment_digits.Clear();
                this.Equipment_prefix.Clear();
                this.Select_Template1.Clear();
                this.Select_Template2.Clear();
                this.PrintDate.Clear();
                this.Remake.Clear();
                this.IMEIRel.Clear();
                this.SN1_num.ReadOnly = true;
                this.SN2_num.ReadOnly = true;
                this.ProductDate.ReadOnly = true;
                this.VIP_num1.ReadOnly = true;
                this.VIP_num2.ReadOnly = true;
                this.VIP_digits.ReadOnly = true;
                this.VIP_prefix.ReadOnly = true;
                this.BAT_num1.ReadOnly = true;
                this.BAT_num2.ReadOnly = true;
                this.BAT_digits.ReadOnly = true;
                this.BAT_prefix.ReadOnly = true;
                this.SIM_num1.ReadOnly = true;
                this.SIM_num2.ReadOnly = true;
                this.SIM_digits.ReadOnly = true;
                this.SIM_prefix.ReadOnly = true;
                this.ICCID_digits.ReadOnly = true;
                this.ICCID_prefix.ReadOnly = true;
                this.MAC_digits.ReadOnly = true;
                this.MAC_prefix.ReadOnly = true;
                this.Equipment_digits.ReadOnly = true;
                this.Equipment_prefix.ReadOnly = true;
                this.updata_inline.Visible = false;
            }
        }

        private void Open_file1_Click(object sender, EventArgs e)
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
                else
                {
                    player.Play();
                }
            }
        }

        private void Open_file2_Click(object sender, EventArgs e)
        {
            if (this.Select_Template2.Text == "")
            {
                player1.Play();
            }
            else
            {
                string path = this.Select_Template2.Text;
                if (File.Exists(path))
                {
                    System.Diagnostics.Process.Start(path);
                }
                else
                {
                    player.Play();
                }
            }
        }

        private void TempalteNum_Leave(object sender, EventArgs e)
        {
            if (this.TempalteNum.Text == "")
            {
                this.TempalteNum.Text = 1.ToString();
            }
            else
            {
                if (IsNumeric(this.TempalteNum.Text))
                {
                    TN = int.Parse(this.TempalteNum.Text);
                }
                else
                {
                    this.reminder.AppendText("请输入数字\r\n");
                    this.TempalteNum.Clear();
                    this.TempalteNum.Focus();
                }
            }
        }

        private void updata_inline_Click(object sender, EventArgs e)
        {
            string sn1_prefix = SN1_num.Text.Substring(0, this.SN1_num.Text.Length - s);
            long sn1_suffix = long.Parse(SN1_num.Text.Remove(0, (this.SN1_num.Text.Length) - s));
            if (MOPB.UpdateInlineByZhiDanBLL(this.CB_ZhiDan.Text,sn1_prefix,sn1_suffix.ToString().PadLeft(s, '0'),this.ProductDate.Text, this.SIM_num1.Text,this.SIM_num2.Text,this.SIM_digits.Text,this.SIM_prefix.Text,this.VIP_num1.Text,this.VIP_num2.Text,this.VIP_digits.Text,this.VIP_prefix.Text,this.BAT_num1.Text,this.BAT_num2.Text,this.BAT_digits.Text,this.BAT_prefix.Text,this.ICCID_digits.Text,this.ICCID_prefix.Text,this.MAC_digits.Text,this.MAC_prefix.Text,this.Equipment_digits.Text,this.Equipment_prefix.Text) > 0)
            {
                player4.Play();
                this.PrintDate.Text = this.ProductDate.Text;
            }
        }

        private void statusChange()
        {
            this.SN1_num.ReadOnly = true;
            this.SN2_num.ReadOnly = true;
            this.ProductDate.ReadOnly = true;
            this.VIP_num1.ReadOnly = true;
            this.VIP_num2.ReadOnly = true;
            this.VIP_digits.ReadOnly = true;
            this.VIP_prefix.ReadOnly = true;
            this.BAT_num1.ReadOnly = true;
            this.BAT_num2.ReadOnly = true;
            this.BAT_digits.ReadOnly = true;
            this.BAT_prefix.ReadOnly = true;
            this.SIM_num1.ReadOnly = true;
            this.SIM_num2.ReadOnly = true;
            this.SIM_digits.ReadOnly = true;
            this.SIM_prefix.ReadOnly = true;
            this.ICCID_digits.ReadOnly = true;
            this.ICCID_prefix.ReadOnly = true;
            this.MAC_digits.ReadOnly = true;
            this.MAC_prefix.ReadOnly = true;
            this.Equipment_digits.ReadOnly = true;
            this.Equipment_prefix.ReadOnly = true;
            this.updata_inline.Visible = false;
        }

        private void AutoTest_Click(object sender, EventArgs e)
        {
            if (this.AutoTest.Checked == true)
            {
                h1 = 1;
                h = h + h1;
                this.IMEI_Start.Focus();
            }
            else
            {
                h1 = 0;
                h = h - 1;
                this.IMEI_Start.Focus();
            }
        }

        private void Couple_Click(object sender, EventArgs e)
        {
            if (this.Couple.Checked == true)
            {
                h2 = 2;
                h = h + h2;
                this.IMEI_Start.Focus();
            }
            else
            {
                h2 = 0;
                h = h - 2;
                this.IMEI_Start.Focus();
            }
        }

        private void WriteImei_Click(object sender, EventArgs e)
        {
            if (this.WriteImei.Checked == true)
            {
                h3 = 4;
                h = h + h3;
                this.IMEI_Start.Focus();
            }
            else
            {
                h3 = 0;
                h = h - 4;
                this.IMEI_Start.Focus();
            }
        }

        private void ParamDownload_Click(object sender, EventArgs e)
        {
            if (this.ParamDownload.Checked == true)
            {
                h4 = 8;
                h = h + h4;
                this.IMEI_Start.Focus();
            }
            else
            {
                h4 = 0;
                h = h - 8;
                this.IMEI_Start.Focus();
            }
        }

        private void ToLock_Click(object sender, EventArgs e)
        {
            this.open_template1.Enabled = false;
            this.open_template2.Enabled = false;
            this.Select_Template1.Enabled = false;
            this.Select_Template2.Enabled = false;
            this.Printer1.Enabled = false;
            this.Printer2.Enabled = false;
            this.CB_ZhiDan.Enabled = false;
            this.Refresh_zhidan.Enabled = false;
            this.Refresh_template.Enabled = false;
            this.Open_file1.Enabled = false;
            this.Open_file2.Enabled = false;
            this.Debug_print.Enabled = false;
            this.ToLock.Enabled = false;
            this.ToUnlock.Enabled = true;
            this.choose_sim.Enabled = false;
            this.choose_vip.Enabled = false;
            this.choose_bat.Enabled = false;
            this.choose_mac.Enabled = false;
            this.choose_Equipment.Enabled = false;
            this.choose_reprint.Enabled = false;
            this.NoCheckCode.Enabled = false;
            this.NoPaper.Enabled = false;
            this.AutoTest.Enabled = false;
            this.Couple.Enabled = false;
            this.WriteImei.Enabled = false;
            this.ParamDownload.Enabled = false;
            this.PrintDate.ReadOnly = true;
            this.TempalteNum.ReadOnly = true;

            this.SN1_num.Enabled = false;
            this.SN2_num.Enabled = false;
            this.ProductDate.Enabled = false;
            this.VIP_num1.Enabled = false;
            this.VIP_num2.Enabled = false;
            this.VIP_digits.Enabled = false;
            this.VIP_prefix.Enabled = false;
            this.BAT_num1.Enabled = false;
            this.BAT_num2.Enabled = false;
            this.BAT_digits.Enabled = false;
            this.BAT_prefix.Enabled = false;
            this.SIM_num1.Enabled = false;
            this.SIM_num2.Enabled = false;
            this.SIM_digits.Enabled = false;
            this.SIM_prefix.Enabled = false;
            this.ICCID_digits.Enabled = false;
            this.ICCID_prefix.Enabled = false;
            this.MAC_digits.Enabled = false;
            this.MAC_prefix.Enabled = false;
            this.Equipment_digits.Enabled = false;
            this.Equipment_prefix.Enabled = false;
            this.updata_inline.Enabled = false;
            this.ChangeInput.Enabled = false;

            if (this.choose_reprint.Checked == true)
            {
                this.Re_IMEINum.Focus();
            }
            else
            {
                this.IMEI_Start.Focus();
            }
        }

        private void ToUnlock_Click(object sender, EventArgs e)
        {
            CH_Unlock chunlock = new CH_Unlock(this);
            chunlock.ShowDialog();
        }

        public void unlock_content()
        {
            this.open_template1.Enabled = true;
            this.open_template2.Enabled = true;
            this.Select_Template1.Enabled = true;
            this.Select_Template2.Enabled = true;
            this.Printer1.Enabled = true;
            this.Printer2.Enabled = true;
            this.CB_ZhiDan.Enabled = true;
            this.Refresh_zhidan.Enabled = true;
            this.Refresh_template.Enabled = true;
            this.Open_file1.Enabled = true;
            this.Open_file2.Enabled = true;
            this.Debug_print.Enabled = true;
            this.ToLock.Enabled = true;
            this.choose_sim.Enabled = true;
            this.choose_vip.Enabled = true;
            this.choose_bat.Enabled = true;
            this.choose_mac.Enabled = true;
            this.choose_Equipment.Enabled = true;
            this.choose_reprint.Enabled = true;
            this.NoCheckCode.Enabled = true;
            this.NoPaper.Enabled = true;
            this.AutoTest.Enabled = true;
            this.Couple.Enabled = true;
            this.WriteImei.Enabled = true;
            this.ParamDownload.Enabled = true;
            this.PrintDate.ReadOnly = false;
            this.TempalteNum.ReadOnly = false;

            this.SN1_num.Enabled = true;
            this.SN2_num.Enabled = true;
            this.ProductDate.Enabled = true;
            this.VIP_num1.Enabled = true;
            this.VIP_num2.Enabled = true;
            this.VIP_digits.Enabled = true;
            this.VIP_prefix.Enabled = true;
            this.BAT_num1.Enabled = true;
            this.BAT_num2.Enabled = true;
            this.BAT_digits.Enabled = true;
            this.BAT_prefix.Enabled = true;
            this.SIM_num1.Enabled = true;
            this.SIM_num2.Enabled = true;
            this.SIM_digits.Enabled = true;
            this.SIM_prefix.Enabled = true;
            this.ICCID_digits.Enabled = true;
            this.ICCID_prefix.Enabled = true;
            this.MAC_digits.Enabled = true;
            this.MAC_prefix.Enabled = true;
            this.Equipment_digits.Enabled = true;
            this.Equipment_prefix.Enabled = true;
            this.updata_inline.Enabled = true;
            this.ChangeInput.Enabled = true;
        }

        private void Refresh_template_Click(object sender, EventArgs e)
        {
            string path = this.Select_Template1.Text;
            string filename = Path.GetFileName(path);
            if (Select_Template1.Text != AppDomain.CurrentDomain.BaseDirectory + filename)
            {
                File.Copy(path, AppDomain.CurrentDomain.BaseDirectory + filename, true);
                this.Select_Template1.Text = AppDomain.CurrentDomain.BaseDirectory + filename;
            }
            if (this.Select_Template2.Text != "")
            {
                string path1 = this.Select_Template2.Text;
                string filename1 = Path.GetFileName(path1);
                if (this.Select_Template2.Text != AppDomain.CurrentDomain.BaseDirectory + filename1)
                {
                    File.Copy(path, AppDomain.CurrentDomain.BaseDirectory + filename1, true);
                    this.Select_Template2.Text = AppDomain.CurrentDomain.BaseDirectory + filename1;
                }
            }
        }

        private void ChangeInput_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.RemindInputName.Text = "*请输入" + this.ChangeInput.Text + "号:";
            this.Re_RemindInputName.Text = "请输入重打"+ this.ChangeInput.Text + "号：";
        }

    }
}
