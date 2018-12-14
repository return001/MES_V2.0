using OqcTestParam.BLL;
using OqcTestSystemSetting.BLL;
using PUsers.BLL;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Media;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using TestSystemSetting;
using ExcelHelper;
using OqcTestParam;
using System.IO;

namespace OqcTool
{
    public partial class Form1 : Form
    {

        //写日志函数
        public static void Log(string port,string msg, Exception e)
        {
            try
            {
                string path = AppDomain.CurrentDomain.BaseDirectory + "\\log\\" + port + System.DateTime.Now.ToString("yyyy-MM-dd") + ".log";
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
                string path = AppDomain.CurrentDomain.BaseDirectory + "\\log\\" + port + System.DateTime.Now.ToString("yyyy-MM-dd") + ".log";
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

        PUsersBLL PUB = new PUsersBLL();

        OqcTestSystemSettingBLL OTSSB = new OqcTestSystemSettingBLL();

        OqcTestParamBLL OTPB = new OqcTestParamBLL();

        List<OqcTestSystem> OTS = new List<OqcTestSystem>();

        List<OqcTestParam1> OTP = new List<OqcTestParam1>();

        //音频文件
        SoundPlayer player = new SoundPlayer(AppDomain.CurrentDomain.BaseDirectory + "失败.wav");

        public Form1()
        {
            InitializeComponent();
            Control.CheckForIllegalCrossThreadCalls = false;
        }

        //程序运行时
        private void Form1_Load(object sender, EventArgs e)
        {
            //this.dataGridView1.Rows.Add("软件版本", "AT^GT_CM=VERSION", "GT800");
            //this.dataGridView1.Rows.Add("SIM卡检测", "AT^GT_CM=SIM", "SIM_OK");
            for (int i = 0; i < this.dataGridView1.Columns.Count; i++)
            {
                this.dataGridView1.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;
            }
        }

        //更换数据库时调用
        public void refrezhidan()
        {
            PUB.refeshConBLL();
        }

        //记录指令被修改过
        int CommandAlterFlag=0;

        //记录需要发送指令的条数
        int CommandNum;
        //定义四个串口对象
        SerialPort SP1 = new SerialPort();
        SerialPort SP2 = new SerialPort();
        SerialPort SP3 = new SerialPort();
        SerialPort SP4 = new SerialPort();
        //定义四个串口连接标识
        bool IsConnected1 =false;
        bool IsConnected2 = false;
        bool IsConnected3 = false;
        bool IsConnected4 = false;
        bool TH1;
        bool TH2;
        bool TH3;
        bool TH4;
        bool SendFlag1;
        bool SendFlag2;
        bool SendFlag3;
        bool SendFlag4;
        //定义四个串口接收的IMEI
        string IMEI1;
        string IMEI2;
        string IMEI3;
        string IMEI4;

        //关闭程序
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            DialogResult dr = MessageBox.Show("确定退出？", "系统提示", MessageBoxButtons.OKCancel);
            if (dr == DialogResult.OK)
            {
                System.Environment.Exit(System.Environment.ExitCode);
            }
            else
            {
                e.Cancel=true;
            }
        }

        //登录
        private void OqcLogin_Click(object sender, EventArgs e)
        {
            login lo = new login(this);
            lo.ShowDialog();
        }

        //登录成功后解放页面
        public void LoginSuccess()
        {
            this.dataGridView1.ReadOnly = false;
            this.OqcLogin.Enabled = false;
            this.SqlConfig.Enabled = false;
            this.OqcLogout.Enabled = true;
            this.zhidan.Enabled = true;
            this.Lock.Enabled = true;
            this.TCUpdate.Enabled = true;
            this.RadioAll.Enabled = true;
            this.RadioFine.Enabled = true;
            this.RadioBad.Enabled = true;
            this.CheckData.Enabled = true;
            this.ReTextCheck.Enabled = true;
            this.ExportByzhidan.Enabled = true;
        }

        //退出登录按钮
        private void OqcLogout_Click(object sender, EventArgs e)
        {
            this.OqcLogin.Enabled = true;
            this.SqlConfig.Enabled = true;
            this.OqcLogout.Enabled = false;
            this.zhidan.Enabled = false;
            this.Lock.Enabled = false;
            this.TCUpdate.Enabled = false;
            this.RadioAll.Enabled = false;
            this.RadioFine.Enabled = false;
            this.RadioBad.Enabled = false;
            this.CheckData.Enabled = false;
            this.ReTextCheck.Enabled = false;
            this.ExportByzhidan.Enabled = false;
            this.dataGridView2.DataSource = "";
            this.OqcUser.Clear();
        }

        //制单下拉时获取制单号
        private void zhidan_DropDown(object sender, EventArgs e)
        {
            OTS = OTSSB.SelectZhidanNumBLL();
            foreach (OqcTestSystem a in OTS)
            {
                if (!this.zhidan.Items.Contains(a.SoftWare))
                {
                    this.zhidan.Items.Add(a.SoftWare);
                }
            }
        }
        
        //选择制单时
        private void zhidan_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.dataGridView1.Rows.Clear();
            this.S1Log.Clear();
            this.S2Log.Clear();
            this.S3Log.Clear();
            this.S4Log.Clear();
            this.Remind1.Text = "等待";
            this.Remind2.Text = "等待";
            this.Remind3.Text = "等待";
            this.Remind4.Text = "等待";
            List<string> Lcommand = OTSSB.SelectCommandBLL(zhidan.Text);
            for(int i = 0; i < Lcommand.Count; i++)
            {
                string[] sArray = Regex.Split(Lcommand[i], "@@", RegexOptions.IgnoreCase);
                this.dataGridView1.Rows.Add(sArray[0], sArray[2], sArray[1]);
            }
        }

        //锁定
        private void Lock_Click(object sender, EventArgs e)
        {
            //判断是否选择了制单
            if (this.zhidan.Text == "")
            {
                player.Play();
                MessageBox.Show("请先选择制单号");
                return;
            }
            //判断是否已设置了指令
            if (this.dataGridView1.Rows.Count <= 1)
            {
                player.Play();
                MessageBox.Show("请先配置指令");
                return;
            }
            if (Lock.Text == "锁定")
            {
                this.dataGridView1.ReadOnly = true;
                OqcLogout.Enabled = false;
                zhidan.Enabled = false;
                AutoConnection.Enabled = true;
                OqcLogin.Enabled = false;
                SerialPort1.Enabled = true;
                SerialPort2.Enabled = true;
                SerialPort3.Enabled = true;
                SerialPort4.Enabled = true;
                Connection1.Enabled = true;
                Connection2.Enabled = true;
                Connection3.Enabled = true;
                Connection4.Enabled = true;
                this.TCUpdate.Enabled = false;
                CommandNum = dataGridView1.RowCount - 1;
                Lock.Text = "解锁";
            }
            else
            {
                if(Connection1.Text=="关闭串口" || Connection2.Text == "关闭串口" || Connection3.Text == "关闭串口" || Connection4.Text == "关闭串口")
                {
                    MessageBox.Show("请先关闭所有串口");
                    return;
                }
                Unlock UL = new Unlock(this);
                UL.ShowDialog();
            }
        }

        //解锁的内容
        public void Unlock_content()
        {
            this.dataGridView1.ReadOnly = false;
            OqcLogout.Enabled = true;
            zhidan.Enabled = true;
            AutoConnection.Enabled = false;
            OqcLogin.Enabled = false;
            SerialPort1.Enabled = false;
            SerialPort2.Enabled = false;
            SerialPort3.Enabled = false;
            SerialPort4.Enabled = false;
            Connection1.Enabled = false;
            Connection2.Enabled = false;
            Connection3.Enabled = false;
            Connection4.Enabled = false;
            this.TCUpdate.Enabled = true;
            CommandNum = 0;
            Lock.Text = "锁定";
        }

        //数据库配置按钮
        private void SqlConfig_Click(object sender, EventArgs e)
        {
            UpdateSqlConn us = new UpdateSqlConn(this);
            us.ShowDialog();
        }

        //单元格值发生更改时
        private void dataGridView1_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            CommandAlterFlag++;
        }

        //确定修改按钮函数
        private void TCUpdate_Click(object sender, EventArgs e)
        {
            //标识位等于3说明没有做更改
            if (CommandAlterFlag == 3)
            {
                MessageBox.Show("数据未更改");
                return;
            }
            List<string> AlertComm = new List<string>(80);
            string AlertSql = "UPDATE [dbo].[Gps_OqcTestSystemSetting] SET ";
            for(int i = 0; i < dataGridView1.Rows.Count-1; i++)
            {
                AlertSql += "[Setting" + i + "]='" + dataGridView1.Rows[i].Cells["TestName"].Value + "@@" + dataGridView1.Rows[i].Cells["ReturnValue"].Value + "@@" + dataGridView1.Rows[i].Cells["TestCommand"].Value + "',";
            }
            AlertSql = AlertSql.TrimEnd(',');
            AlertSql += " where SoftWare = '" + zhidan.Text + "'";
            if (OTSSB.UpdateCommandBLL(AlertSql) > 0)
            {
                MessageBox.Show("修改成功");
            }
            else
            {
                MessageBox.Show("修改失败");
            }
        }

        //以下串口1函数

        //串口1下拉时发生
        private void SerialPort1_DropDown(object sender, EventArgs e)
        {
            SerialPort1.Items.Clear();
            string[] ArryPort = SerialPort.GetPortNames();
            for (int i = 0; i < ArryPort.Length; i++)
            {
                SerialPort1.Items.Add(ArryPort[i]);
            }
        }

        //串口1自动检测连接函数
        private void AutoConcted1()
        {
            while (TH1)
            {
                if (!IsConnected1)
                {
                    try
                    {
                        SP1.PortName = this.SerialPort1.Text;
                        //波特率
                        SP1.BaudRate = 115200;
                        //数据位
                        SP1.DataBits = 8;
                        //检验位
                        SP1.Parity = Parity.None;
                        //停止位
                        SP1.StopBits = StopBits.One;
                        //缓冲区
                        SP1.ReadBufferSize = 1024;
                        SP1.Open();
                        Connection1.Text = "关闭串口";
                        SerialPort1.Enabled = false;
                        S1Log.AppendText(DateTime.Now.ToString("T") + " 串口1打开成功\r\n");
                        IsConnected1 = true;
                    }
                    catch (Exception)
                    {
                        SP1.Close();
                        S1Log.AppendText(DateTime.Now.ToString("T") + " 串口1打开失败\r\n");
                        Connection1.Text = "打开串口";
                        TH1 = false;
                    }
                    Thread.Sleep(400);
                }
                if (IsConnected1)
                {
                    if (!SP1.IsOpen)//检查是否断开
                    {
                        TH1 = false;
                        SP1.Close();
                        IsConnected1 = false;
                        Connection1.Text = "打开串口";
                        SerialPort1.Enabled = true;
                        S1Log.AppendText(DateTime.Now.ToString("T") + " 串口1异常关闭\r\n");
                        Remind1.Text = "等待";
                        Remind1.BackColor = Color.White;
                        this.SerialPort1.Text = "";
                    }
                    Thread.Sleep(1000);
                }
            }
        }

        //打开串口1按钮函数 
        private void Connection1_Click(object sender, EventArgs e)
        {
            if (SerialPort1.Text != "")
            {
                Thread SPConn1 = new Thread(new ThreadStart(AutoConcted1));
                SPConn1.Start();
                SPConn1.IsBackground = true;
                if (Connection1.Text == "打开串口")
                {
                    //检测串口是否异常插拔线程
                    TH1 = true;
                    IsConnected1 = false;
                    //开启写线程
                    Thread SPSend1 = new Thread(new ThreadStart(SerialPortSend1));
                    if (SPSend1.ThreadState != ThreadState.Running)
                    {
                        SPSend1.Start();
                        SPConn1.IsBackground = true;
                    }
                    SendFlag1 = true;
                    Connection1.Text = "关闭串口";
                }
                else
                {
                    TH1 = false;
                    SendFlag1 = false;
                    SP1.Close();
                    Connection1.Text = "打开串口";
                    SerialPort1.Enabled = true;
                    this.SerialPort1.Text = "";
                    S1Log.AppendText(DateTime.Now.ToString("T") + " 串口1关闭成功\r\n");
                }
            }
            else
            {
                S1Log.AppendText(DateTime.Now.ToString("T") + "请先选择串口1\r\n");
            }
        }

        //给串口1发送消息
        private void SerialPortSend1()
        {
            while (SendFlag1)
            {
                if (SP1.IsOpen)
                {
                    //检测到有机子接入即可开始下面的测试
                    for (int k = 0; k < 1;)
                    {
                        try
                        {
                            SP1.WriteLine("AT^GT_CM=TEST");
                            Log("port1", "发送指令：AT^GT_CM=TEST", null);
                        }
                        catch
                        {
                            return;
                        }
                        Thread.Sleep(500);
                        string TestValue = SerialPortReceive1();
                        if (TestValue.Contains("TEST_OK"))
                        {
                            Log("port1", "接收指令："+ TestValue, null);
                            Remind1.Text = "测试中";
                            Remind1.BackColor = Color.White;
                            k++;
                        }
                        Thread.Sleep(1000);
                    }
                    //发送检测IMEI指令
                    try
                    {
                        SP1.WriteLine("AT^GT_CM=IMEI");
                        S1Log.AppendText(DateTime.Now.ToString("T") + " 发送了AT^GT_CM=IMEI\r\n");
                        Log("port1", "发送指令：AT^GT_CM=IMEI", null);
                        Thread.Sleep(500);
                        string ReceiveImei = SerialPortReceive1();
                        if (ReceiveImei != "")
                        {
                            S1Log.AppendText(DateTime.Now.ToString("T") + " 接收了" + ReceiveImei + "\r\n");
                            Log("port1", "接收指令："+ ReceiveImei, null);
                            string[] sArray = ReceiveImei.Split(':');
                            if (sArray[1].Trim().Contains("Sensor_OK"))
                            {
                                IMEI1 = sArray[1].Split('S')[0].Replace("\r\n", "");
                            }
                            else
                            {
                                IMEI1 = sArray[1].Replace("\r\n", "");
                            }
                        }
                    }
                    catch
                    {
                        S1Log.AppendText(DateTime.Now.ToString("T") + "串口已关闭\r\n");
                        Remind1.Text = "失败";
                        Remind1.BackColor = Color.Red;
                        return;
                    }
                    //检测该机子是否已经测试过
                    if (!OTPB.CheckImeiBLL(IMEI1))
                    {
                        string SendCommand="";  //记录发送的指令
                        string ReceiveValue=""; //记录收到的返回值
                        int i;
                        for (i=0; i < CommandNum;)
                        {
                            for (int j = 0; j <= 2; j++)
                            {
                                //串口处于开启状态，将测试项文本发送
                                try
                                {
                                    SP1.WriteLine(dataGridView1.Rows[i].Cells["TestCommand"].Value.ToString());
                                    if (j == 0)
                                    {
                                        SendCommand += dataGridView1.Rows[i].Cells["TestCommand"].Value.ToString()+"@@";
                                    }
                                }
                                catch
                                {
                                    S1Log.AppendText(DateTime.Now.ToString("T") + "串口已关闭\r\n");
                                    Remind1.Text = "失败";
                                    Remind1.BackColor = Color.Red;
                                    return;
                                }
                                S1Log.AppendText(DateTime.Now.ToString("T") + " 发送了" + dataGridView1.Rows[i].Cells["TestCommand"].Value.ToString() + "\r\n");
                                Log("port1", "发送指令：" + dataGridView1.Rows[i].Cells["TestCommand"].Value.ToString(), null);
                                Remind1.Text = "测试中";
                                Remind1.BackColor = Color.White;
                                Thread.Sleep(200);
                                string RetVal = SerialPortReceive1();
                                if (RetVal.Contains(dataGridView1.Rows[i].Cells["ReturnValue"].Value.ToString()))
                                {
                                    S1Log.AppendText(DateTime.Now.ToString("T") + " 接收了" + RetVal + "\r\n");
                                    Log("port1", "接收指令：" + RetVal, null);
                                    ReceiveValue += RetVal + "@@";
                                    i++;
                                    break;
                                }
                                else
                                {
                                    ReceiveValue += RetVal + "@@";
                                    if (j == 2)
                                    {
                                        OTPB.InsertTestResultBLL(zhidan.Text, IMEI1, SendCommand, ReceiveValue, 0, OqcUser.Text);
                                        S1Log.AppendText(DateTime.Now.ToString("T") + " 机子异常\r\n");
                                        Log("port1", IMEI1+"测试失败。" , null);
                                        Remind1.Text = "失败";
                                        Remind1.BackColor = Color.Red;
                                        i = 999;
                                        break;
                                    }
                                }
                                Thread.Sleep(2000);
                            }
                        }
                        if (i != 999)
                        {
                            //接收对比通过后
                            if (OTPB.InsertTestResultBLL(zhidan.Text, IMEI1, SendCommand, ReceiveValue, 1, OqcUser.Text) > 0)
                            {
                                S1Log.AppendText(DateTime.Now.ToString("T") + " 串口读取完成\r\n");
                                Log("port1", IMEI1 + "测试成功。", null);
                                Remind1.Text = "成功";
                                Remind1.BackColor = Color.Green;
                            }
                        }
                    }
                    else
                    {
                        S1Log.AppendText(DateTime.Now.ToString("T") + " IMEI重号\r\n");
                        Log("port1", IMEI1 + "IMEI重号。", null);
                        Remind1.Text = "失败";
                        Remind1.BackColor = Color.Red;
                    }
                    //测试完成后检测该机子是否已断开
                    for (int l = 0; l < 1;)
                    {
                        try
                        {
                            SP1.WriteLine("AT^GT_CM=TEST");
                        }
                        catch
                        {
                            return;
                        }
                        Thread.Sleep(500);
                        string TestValue = SerialPortReceive1();
                        if (TestValue == "")
                        {
                            Remind1.Text = "等待";
                            Remind1.BackColor = Color.White;
                            break;
                        }
                        Thread.Sleep(1000);
                    }
                }
            }
        }

        //串口1接收消息
        private string SerialPortReceive1()
        {
            if (SP1.IsOpen)
            {
                return SP1.ReadExisting().Trim();
            }
            return "";
        }

        //以上串口1函数

        //以下串口2函数

        //串口2下拉时发生
        private void SerialPort2_DropDown(object sender, EventArgs e)
        {
            SerialPort2.Items.Clear();
            string[] ArryPort = SerialPort.GetPortNames();
            for (int i = 0; i < ArryPort.Length; i++)
            {
                SerialPort2.Items.Add(ArryPort[i]);
            }
        }

        //串口2自动检测连接函数
        private void AutoConcted2()
        {
            while (TH2)
            {
                if (!IsConnected2)
                {
                    try
                    {
                        SP2.PortName = this.SerialPort2.Text;
                        //波特率
                        SP2.BaudRate = 115200;
                        //数据位
                        SP2.DataBits = 8;
                        //检验位
                        SP2.Parity = Parity.None;
                        //停止位
                        SP2.StopBits = StopBits.One;
                        //缓冲区
                        SP2.ReadBufferSize = 1024;
                        SP2.Open();
                        Connection2.Text = "关闭串口";
                        SerialPort2.Enabled = false;
                        S2Log.AppendText(DateTime.Now.ToString("T") + " 串口2打开成功\r\n");
                        IsConnected2 = true;
                    }
                    catch (Exception)
                    {
                        SP2.Close();
                        S2Log.AppendText(DateTime.Now.ToString("T") + " 串口2打开失败\r\n");
                        TH2 = false;
                        Connection2.Text = "打开串口";
                    }
                    Thread.Sleep(400);
                }
                if (IsConnected2)
                {
                    if (!SP2.IsOpen)//检查是否断开
                    {
                        TH2 = false;
                        SP2.Close();
                        IsConnected2 = false;
                        Connection2.Text = "打开串口";
                        SerialPort2.Enabled = true;
                        S2Log.AppendText(DateTime.Now.ToString("T") + " 串口2异常关闭\r\n");
                        Remind2.Text = "等待";
                        Remind2.BackColor = Color.White;
                        this.SerialPort2.Text = "";
                    }
                    Thread.Sleep(1000);
                }
            }
        }

        //打开串口2按钮函数 
        private void Connection2_Click(object sender, EventArgs e)
        {
            if (SerialPort2.Text != "")
            {
                Thread SPConn2 = new Thread(new ThreadStart(AutoConcted2));
                SPConn2.Start();
                SPConn2.IsBackground = true;
                if (Connection2.Text == "打开串口")
                {
                    //检测串口是否异常插拔线程
                    TH2 = true;
                    IsConnected2 = false;
                    //开启写线程
                    Thread SPSend2 = new Thread(new ThreadStart(SerialPortSend2));
                    SPSend2.Start();
                    SPSend2.IsBackground = true;
                    SendFlag2 = true;
                    Connection2.Text = "关闭串口";
                }
                else
                {
                    TH2 = false;
                    SendFlag2 = false;
                    SP2.Close();
                    Connection2.Text = "打开串口";
                    SerialPort2.Enabled = true;
                    this.SerialPort2.Text = "";
                    S2Log.AppendText(DateTime.Now.ToString("T") + " 串口2关闭成功\r\n");
                }
            }
            else
            {
                S2Log.AppendText(DateTime.Now.ToString("T") + " 请先选择串口2\r\n");
            }
        }

        //给串口2发送消息
        private void SerialPortSend2()
        {
            while (SendFlag2)
            {
                if (SP2.IsOpen)
                {
                    //检测到有机子接入即可开始下面的测试
                    for (int k = 0; k < 1;)
                    {
                        try
                        {
                            SP2.WriteLine("AT^GT_CM=TEST");
                            Log("port2", "发送指令：AT^GT_CM=TEST", null);
                        }
                        catch
                        {
                            return;
                        }
                        Thread.Sleep(500);
                        string TestValue = SerialPortReceive2();
                        if (TestValue.Contains("TEST_OK"))
                        {
                            Log("port2", "接收指令："+ TestValue, null);
                            Remind2.Text = "测试中";
                            Remind2.BackColor = Color.White;
                            k++;
                        }
                        Thread.Sleep(1000);
                    }
                    //发送检测IMEI指令
                    try
                    {
                        SP2.WriteLine("AT^GT_CM=IMEI");
                        S2Log.AppendText(DateTime.Now.ToString("T") + " 发送了AT^GT_CM=IMEI\r\n");
                        Log("port2", "发送指令：AT^GT_CM=IMEI", null);
                        Thread.Sleep(500);
                        string ReceiveImei = SerialPortReceive2();
                        if (ReceiveImei != "")
                        {
                            S2Log.AppendText(DateTime.Now.ToString("T") + " 接收了" + ReceiveImei + "\r\n");
                            Log("port2", "接收指令："+ ReceiveImei, null);
                            string[] sArray = ReceiveImei.Split(':');
                            if (sArray[1].Trim().Contains("Sensor_OK"))
                            {
                                IMEI2 = sArray[1].Split('S')[0].Replace("\r\n", "");
                            }
                            else
                            {
                                IMEI2 = sArray[1].Replace("\r\n", "");
                            }
                        }
                    }
                    catch
                    {
                        S2Log.AppendText(DateTime.Now.ToString("T") + "串口已关闭\r\n");
                        Remind2.Text = "失败";
                        Remind2.BackColor = Color.Red;
                        return;
                    }
                    //检测该机子是否已经测试过
                    if (!OTPB.CheckImeiBLL(IMEI2))
                    {
                        string SendCommand = "";  //记录发送的指令
                        string ReceiveValue = ""; //记录收到的返回值
                        int i;
                        for (i = 0; i < CommandNum;)
                        {
                            for (int j = 0; j <= 2; j++)
                            {
                                //串口处于开启状态，将测试项文本发送
                                try
                                {
                                    SP2.WriteLine(dataGridView1.Rows[i].Cells["TestCommand"].Value.ToString());
                                    if (j == 0)
                                    {
                                        SendCommand += dataGridView1.Rows[i].Cells["TestCommand"].Value.ToString()+"@@";
                                    }
                                }
                                catch
                                {
                                    S2Log.AppendText(DateTime.Now.ToString("T") + "串口已关闭\r\n");
                                    Remind2.Text = "失败";
                                    Remind2.BackColor = Color.Red;
                                    return;
                                }
                                S2Log.AppendText(DateTime.Now.ToString("T") + " 发送了" + dataGridView1.Rows[i].Cells["TestCommand"].Value.ToString() + "\r\n");
                                Log("port2", "发送指令："+ dataGridView1.Rows[i].Cells["TestCommand"].Value.ToString(), null);
                                Remind2.Text = "测试中";
                                Remind2.BackColor = Color.White;
                                Thread.Sleep(200);
                                string RetVal = SerialPortReceive2();
                                if (RetVal.Contains(dataGridView1.Rows[i].Cells["ReturnValue"].Value.ToString()))
                                {
                                    S2Log.AppendText(DateTime.Now.ToString("T") + " 接收了" + RetVal + "\r\n");
                                    Log("port2", "接收指令：" + RetVal, null);
                                    ReceiveValue += RetVal+"@@";
                                    i++;
                                    break;
                                }
                                else
                                {
                                    ReceiveValue += RetVal +"@@";
                                    if (j == 2)
                                    {
                                        OTPB.InsertTestResultBLL(zhidan.Text, IMEI2, SendCommand, ReceiveValue, 0, OqcUser.Text);
                                        S2Log.AppendText(DateTime.Now.ToString("T") + " 机子异常\r\n");
                                        Log("port2", IMEI2+ "机子异常", null);
                                        Remind2.Text = "失败";
                                        Remind2.BackColor = Color.Red;
                                        i = 999;
                                        break;
                                    }
                                }
                                Thread.Sleep(2000);
                            }
                        }
                        if (i != 999)
                        {
                            //接收对比通过后
                            if (OTPB.InsertTestResultBLL(zhidan.Text, IMEI2, SendCommand, ReceiveValue, 1, OqcUser.Text) > 0)
                            {
                                S2Log.AppendText(DateTime.Now.ToString("T") + " 串口读取完成\r\n");
                                Log("port2", IMEI2 + "测试成功", null);
                                Remind2.Text = "成功";
                                Remind2.BackColor = Color.Green;
                            }
                        }
                    }
                    else
                    {
                        S2Log.AppendText(DateTime.Now.ToString("T") + " IMEI重号\r\n");
                        Log("port2", IMEI2 + "IMEI重号", null);
                        Remind2.Text = "失败";
                        Remind2.BackColor = Color.Red;
                    }
                    //测试完成后检测该机子是否已断开
                    for (int l = 0; l < 1;)
                    {
                        try
                        {
                            SP2.WriteLine("AT^GT_CM=TEST");
                        }
                        catch
                        {
                            return;
                        }
                        Thread.Sleep(500);
                        string TestValue = SerialPortReceive2();
                        if (TestValue == "")
                        {
                            Remind2.Text = "等待";
                            Remind2.BackColor = Color.White;
                            break;
                        }
                        Thread.Sleep(1000);
                    }
                }
            }
        }

        //串口2接收消息
        private string SerialPortReceive2()
        {
            if (SP2.IsOpen)
            {
                return SP2.ReadExisting().Trim();
            }
            return "";
        }

        //以上串口2函数

        //以下串口3函数

        //串口3下拉时发生
        private void SerialPort3_DropDown(object sender, EventArgs e)
        {
            SerialPort3.Items.Clear();
            string[] ArryPort = SerialPort.GetPortNames();
            for (int i = 0; i < ArryPort.Length; i++)
            {
                SerialPort3.Items.Add(ArryPort[i]);
            }
        }

        //串口3自动检测连接函数
        private void AutoConcted3()
        {
            while (TH3)
            {
                if (!IsConnected3)
                {
                    try
                    {
                        SP3.PortName = this.SerialPort3.Text;
                        //波特率
                        SP3.BaudRate = 115200;
                        //数据位
                        SP3.DataBits = 8;
                        //检验位
                        SP3.Parity = Parity.None;
                        //停止位
                        SP3.StopBits = StopBits.One;
                        //缓冲区
                        SP3.ReadBufferSize = 1024;
                        SP3.Open();
                        Connection3.Text = "关闭串口";
                        SerialPort3.Enabled = false;
                        S3Log.AppendText(DateTime.Now.ToString("T") + " 串口3打开成功\r\n");
                        IsConnected3 = true;
                    }
                    catch (Exception)
                    {
                        SP3.Close();
                        S3Log.AppendText(DateTime.Now.ToString("T") + " 串口3打开失败\r\n");
                        TH3 = false;
                        Connection3.Text = "打开串口";
                    }
                    Thread.Sleep(400);
                }
                if (IsConnected3)
                {
                    if (!SP3.IsOpen)//检查是否断开
                    {
                        TH3 = false;
                        SP3.Close();
                        IsConnected3 = false;
                        Connection3.Text = "打开串口";
                        SerialPort3.Enabled = true;
                        S3Log.AppendText(DateTime.Now.ToString("T") + " 串口3异常关闭\r\n");
                        Remind3.Text = "等待";
                        Remind3.BackColor = Color.White;
                        this.SerialPort3.Text = "";
                    }
                    Thread.Sleep(1000);
                }
            }
        }

        //打开串口3按钮函数 
        private void Connection3_Click(object sender, EventArgs e)
        {
            if (SerialPort3.Text != "")
            {
                Thread SPConn3 = new Thread(new ThreadStart(AutoConcted3));
                SPConn3.Start();
                SPConn3.IsBackground = true;
                if (Connection3.Text == "打开串口")
                {
                    //检测串口是否异常插拔线程
                    TH3 = true;
                    IsConnected3 = false;
                    //开启写线程
                    Thread SPSend3 = new Thread(new ThreadStart(SerialPortSend3));
                    SPSend3.Start();
                    SPSend3.IsBackground = true;
                    SendFlag3 = true;
                    Connection3.Text = "关闭串口";
                }
                else
                {
                    TH3 = false;
                    SendFlag3 = false;
                    SP3.Close();
                    Connection3.Text = "打开串口";
                    SerialPort3.Enabled = true;
                    this.SerialPort3.Text = "";
                    S3Log.AppendText(DateTime.Now.ToString("T") + " 串口3关闭成功\r\n");
                }
            }
            else
            {
                S3Log.AppendText(DateTime.Now.ToString("T") + " 请先选择串口3\r\n");
            }
        }

        //给串口3发送消息
        private void SerialPortSend3()
        {
            while (SendFlag3)
            {
                if (SP3.IsOpen)
                {
                    //检测到有机子接入即可开始下面的测试
                    for (int k = 0; k < 1;)
                    {
                        try
                        {
                            SP3.WriteLine("AT^GT_CM=TEST");
                            Log("port3", "发送指令：AT^GT_CM=TEST", null);
                        }
                        catch
                        {
                            return;
                        }
                        Thread.Sleep(500);
                        string TestValue = SerialPortReceive3();
                        if (TestValue.Contains("TEST_OK"))
                        {
                            Log("port3", "接收指令："+ TestValue, null);
                            Remind3.Text = "测试中";
                            Remind3.BackColor = Color.White;
                            k++;
                        }
                        Thread.Sleep(1000);
                    }
                    //发送检测IMEI指令
                    try
                    {
                        SP3.WriteLine("AT^GT_CM=IMEI");
                        S3Log.AppendText(DateTime.Now.ToString("T") + " 发送了AT^GT_CM=IMEI\r\n");
                        Log("port3", "发送指令：AT^GT_CM=IMEI", null);
                        Thread.Sleep(500);
                        string ReceiveImei = SerialPortReceive3();
                        if (ReceiveImei != "")
                        {
                            S3Log.AppendText(DateTime.Now.ToString("T") + " 接收了" + ReceiveImei + "\r\n");
                            Log("port3", "接收指令："+ ReceiveImei, null);
                            string[] sArray = ReceiveImei.Split(':');
                            if (sArray[1].Trim().Contains("Sensor_OK"))
                            {
                                IMEI3 = sArray[1].Split('S')[0].Replace("\r\n", "");
                            }
                            else
                            {
                                IMEI3 = sArray[1].Replace("\r\n", "");
                            }
                        }
                    }
                    catch
                    {
                        S3Log.AppendText(DateTime.Now.ToString("T") + "串口已关闭\r\n");
                        Remind3.Text = "失败";
                        Remind3.BackColor = Color.Red;
                        return;
                    }
                    //检测该机子是否已经测试过
                    if (!OTPB.CheckImeiBLL(IMEI3))
                    {
                        string SendCommand = "";  //记录发送的指令
                        string ReceiveValue = ""; //记录收到的返回值
                        int i;
                        for (i = 0; i < CommandNum;)
                        {
                            for (int j = 0; j <= 2; j++)
                            {
                                //串口处于开启状态，将测试项文本发送
                                try
                                {
                                    SP3.WriteLine(dataGridView1.Rows[i].Cells["TestCommand"].Value.ToString());
                                    if (j == 0)
                                    {
                                        SendCommand += dataGridView1.Rows[i].Cells["TestCommand"].Value.ToString() + "@@";
                                    }
                                }
                                catch
                                {
                                    S3Log.AppendText(DateTime.Now.ToString("T") + "串口已关闭\r\n");
                                    Remind3.Text = "失败";
                                    Remind3.BackColor = Color.Red;
                                    return;
                                }
                                S3Log.AppendText(DateTime.Now.ToString("T") + " 发送了" + dataGridView1.Rows[i].Cells["TestCommand"].Value.ToString() + "\r\n");
                                Log("port3", "发送指令："+ dataGridView1.Rows[i].Cells["TestCommand"].Value.ToString(), null);
                                Remind3.Text = "测试中";
                                Remind3.BackColor = Color.White;
                                Thread.Sleep(200);
                                string RetVal = SerialPortReceive3();
                                if (RetVal.Contains(dataGridView1.Rows[i].Cells["ReturnValue"].Value.ToString()))
                                {
                                    S3Log.AppendText(DateTime.Now.ToString("T") + " 接收了" + RetVal + "\r\n");
                                    Log("port3", "接收指令：" + RetVal, null);
                                    ReceiveValue += RetVal + "@@";
                                    i++;
                                    break;
                                }
                                else
                                {
                                    ReceiveValue += RetVal + "@@";
                                    if (j == 2)
                                    {
                                        OTPB.InsertTestResultBLL(zhidan.Text, IMEI3, SendCommand, ReceiveValue, 0, OqcUser.Text);
                                        S3Log.AppendText(DateTime.Now.ToString("T") + " 机子异常\r\n");
                                        Log("port3", IMEI3+ "测试失败", null);
                                        Remind3.Text = "失败";
                                        Remind3.BackColor = Color.Red;
                                        i = 999;
                                        break;
                                    }
                                }
                                Thread.Sleep(2000);
                            }
                        }
                        if (i != 999)
                        {
                            //接收对比通过后
                            if (OTPB.InsertTestResultBLL(zhidan.Text, IMEI3, SendCommand, ReceiveValue, 1, OqcUser.Text) > 0)
                            {
                                S3Log.AppendText(DateTime.Now.ToString("T") + " 串口读取完成\r\n");
                                Log("port3", IMEI3 + "测试成功", null);
                                Remind3.Text = "成功";
                                Remind3.BackColor = Color.Green;
                            }
                        }
                    }
                    else
                    {
                        S3Log.AppendText(DateTime.Now.ToString("T") + " IMEI重号\r\n");
                        Log("port3", IMEI3 + "IMEI重号", null);
                        Remind3.Text = "失败";
                        Remind3.BackColor = Color.Red;
                    }
                    //测试完成后检测该机子是否已断开
                    for (int l = 0; l < 1;)
                    {
                        try
                        {
                            SP3.WriteLine("AT^GT_CM=TEST");
                        }
                        catch
                        {
                            return;
                        }
                        Thread.Sleep(500);
                        string TestValue = SerialPortReceive3();
                        if (TestValue == "")
                        {
                            Remind3.Text = "等待";
                            Remind3.BackColor = Color.White;
                            break;
                        }
                        Thread.Sleep(1000);
                    }
                }
            }
        }

        //串口3接收消息
        private string SerialPortReceive3()
        {
            if (SP3.IsOpen)
            {
                return SP3.ReadExisting().Trim();
            }
            return "";
        }

        //以上串口3函数    

        //以下串口4函数

        //串口4下拉时发生
        private void SerialPort4_DropDown(object sender, EventArgs e)
        {
            SerialPort4.Items.Clear();
            string[] ArryPort = SerialPort.GetPortNames();
            for (int i = 0; i < ArryPort.Length; i++)
            {
                SerialPort4.Items.Add(ArryPort[i]);
            }
        }

        //串口4自动检测连接函数
        private void AutoConcted4()
        {
            while (TH4)
            {
                if (!IsConnected4)
                {
                    try
                    {
                        SP4.PortName = this.SerialPort4.Text;
                        //波特率
                        SP4.BaudRate = 115200;
                        //数据位
                        SP4.DataBits = 8;
                        //检验位
                        SP4.Parity = Parity.None;
                        //停止位
                        SP4.StopBits = StopBits.One;
                        //缓冲区
                        SP4.ReadBufferSize = 1024;
                        SP4.Open();
                        Connection4.Text = "关闭串口";
                        SerialPort4.Enabled = false;
                        S4Log.AppendText(DateTime.Now.ToString("T") + " 串口4打开成功\r\n");
                        IsConnected4 = true;
                    }
                    catch (Exception)
                    {
                        SP4.Close();
                        S4Log.AppendText(DateTime.Now.ToString("T") + " 串口4打开失败\r\n");
                        TH4 = false;
                        Connection4.Text = "打开串口";
                    }
                    Thread.Sleep(400);
                }
                if (IsConnected4)
                {
                    if (!SP4.IsOpen)//检查是否断开
                    {
                        TH4 = false;
                        SP4.Close();
                        IsConnected4 = false;
                        Connection4.Text = "打开串口";
                        SerialPort4.Enabled = true;
                        S4Log.AppendText(DateTime.Now.ToString("T") + " 串口4异常关闭\r\n");
                        Remind4.Text = "等待";
                        Remind4.BackColor = Color.White;
                        this.SerialPort4.Text = "";
                    }
                    Thread.Sleep(1000);
                }
            }
        }

        //打开串口4按钮函数 
        private void Connection4_Click(object sender, EventArgs e)
        {
            if (SerialPort4.Text != "")
            {
                Thread SPConn4 = new Thread(new ThreadStart(AutoConcted4));
                SPConn4.Start();
                SPConn4.IsBackground = true;
                if (Connection4.Text == "打开串口")
                {
                    //检测串口是否异常插拔线程
                    TH4 = true;
                    IsConnected4 = false;
                    //开启写线程
                    Thread SPSend4 = new Thread(new ThreadStart(SerialPortSend4));
                    SPSend4.Start();
                    SPSend4.IsBackground = true;
                    SendFlag4 = true;
                    Connection4.Text = "关闭串口";
                }
                else
                {
                    TH4 = false;
                    SendFlag4 = false;
                    SP4.Close();
                    Connection4.Text = "打开串口";
                    SerialPort4.Enabled = true;
                    this.SerialPort4.Text = "";
                    S4Log.AppendText(DateTime.Now.ToString("T") + " 串口4关闭成功\r\n");
                }
            }
            else
            {
                S4Log.AppendText(DateTime.Now.ToString("T") + " 请先选择串口4\r\n");
            }
        }

        //给串口4发送消息
        private void SerialPortSend4()
        {
            while (SendFlag4)
            {
                if (SP4.IsOpen)
                {
                    //检测到有机子接入即可开始下面的测试
                    for (int k = 0; k < 1;)
                    {
                        try
                        {
                            SP4.WriteLine("AT^GT_CM=TEST");
                            Log("port4", "发送指令：AT^GT_CM=TEST", null);
                        }
                        catch
                        {
                            return;
                        }
                        Thread.Sleep(500);
                        string TestValue = SerialPortReceive4();
                        if (TestValue.Contains("TEST_OK"))
                        {
                            Log("port4", "接收指令："+ TestValue, null);
                            Remind4.Text = "测试中";
                            Remind4.BackColor = Color.White;
                            k++;
                        }
                        Thread.Sleep(1000);
                    }
                    //发送检测IMEI指令
                    try
                    {
                        SP4.WriteLine("AT^GT_CM=IMEI");
                        S4Log.AppendText(DateTime.Now.ToString("T") + " 发送了AT^GT_CM=IMEI\r\n");
                        Log("port4", "发送指令：AT^GT_CM=IMEI", null);
                        Thread.Sleep(500);
                        string ReceiveImei = SerialPortReceive4();
                        if (ReceiveImei != "")
                        {
                            S4Log.AppendText(DateTime.Now.ToString("T") + " 接收了" + ReceiveImei + "\r\n");
                            Log("port4", "接收指令："+ ReceiveImei, null);
                            string[] sArray = ReceiveImei.Split(':');
                            if (sArray[1].Trim().Contains("Sensor_OK"))
                            {
                                IMEI4 = sArray[1].Split('S')[0].Replace("\r\n", "");
                            }
                            else
                            {
                                IMEI4 = sArray[1].Replace("\r\n", "");
                            }
                        }
                    }
                    catch
                    {
                        S4Log.AppendText(DateTime.Now.ToString("T") + "串口已关闭\r\n");
                        Remind4.Text = "失败";
                        Remind4.BackColor = Color.Red;
                        return;
                    }
                    //检测该机子是否已经测试过
                    if (!OTPB.CheckImeiBLL(IMEI4))
                    {
                        string SendCommand = "";  //记录发送的指令
                        string ReceiveValue = ""; //记录收到的返回值
                        int i;
                        for (i = 0; i < CommandNum;)
                        {
                            for (int j = 0; j <= 2; j++)
                            {
                                //串口处于开启状态，将测试项文本发送
                                try
                                {
                                    SP4.WriteLine(dataGridView1.Rows[i].Cells["TestCommand"].Value.ToString());
                                    if (j == 0)
                                    {
                                        SendCommand += dataGridView1.Rows[i].Cells["TestCommand"].Value.ToString() + "@@";
                                    }
                                }
                                catch
                                {
                                    S4Log.AppendText(DateTime.Now.ToString("T") + "串口已关闭\r\n");
                                    Remind4.Text = "失败";
                                    Remind4.BackColor = Color.Red;
                                    return;
                                }
                                S4Log.AppendText(DateTime.Now.ToString("T") + " 发送了" + dataGridView1.Rows[i].Cells["TestCommand"].Value.ToString() + "\r\n");
                                Log("port4", "发送指令："+ dataGridView1.Rows[i].Cells["TestCommand"].Value.ToString(), null);
                                Remind4.Text = "测试中";
                                Remind4.BackColor = Color.White;
                                Thread.Sleep(200);
                                string RetVal = SerialPortReceive4();
                                if (RetVal.Contains(dataGridView1.Rows[i].Cells["ReturnValue"].Value.ToString()))
                                {
                                    S4Log.AppendText(DateTime.Now.ToString("T") + " 接收了" + RetVal + "\r\n");
                                    Log("port4", "接收指令：" + RetVal, null);
                                    ReceiveValue += RetVal + "@@";
                                    i++;
                                    break;
                                }
                                else
                                {
                                    ReceiveValue += RetVal + "@@";
                                    if (j == 2)
                                    {
                                        OTPB.InsertTestResultBLL(zhidan.Text, IMEI4, SendCommand, ReceiveValue, 0, OqcUser.Text);
                                        S4Log.AppendText(DateTime.Now.ToString("T") + " 机子异常\r\n");
                                        Log("port4", IMEI4+"测试失败", null);
                                        Remind4.Text = "失败";
                                        Remind4.BackColor = Color.Red;
                                        i = 999;
                                        break;
                                    }
                                }
                                Thread.Sleep(2000);
                            }
                        }
                        if (i != 999)
                        {
                            //接收对比通过后
                            if (OTPB.InsertTestResultBLL(zhidan.Text, IMEI4, SendCommand, ReceiveValue, 1, OqcUser.Text) > 0)
                            {
                                S4Log.AppendText(DateTime.Now.ToString("T") + " 串口读取完成\r\n");
                                Log("port4", IMEI4 + "测试成功", null);
                                Remind4.Text = "成功";
                                Remind4.BackColor = Color.Green;
                            }
                        }
                    }
                    else
                    {
                        S4Log.AppendText(DateTime.Now.ToString("T") + " IMEI重号\r\n");
                        Log("port4", IMEI4 + "IMEI重号", null);
                        Remind4.Text = "失败";
                        Remind4.BackColor = Color.Red;
                    }
                    //测试完成后检测该机子是否已断开
                    for (int l = 0; l < 1;)
                    {
                        try
                        {
                            SP4.WriteLine("AT^GT_CM=TEST");
                        }
                        catch
                        {
                            return;
                        }
                        Thread.Sleep(500);
                        string TestValue = SerialPortReceive4();
                        if (TestValue == "")
                        {
                            Remind4.Text = "等待";
                            Remind4.BackColor = Color.White;
                            break;
                        }
                        Thread.Sleep(1000);
                    }
                }
            }
        }

        //串口4接收消息
        private string SerialPortReceive4()
        {
            if (SP4.IsOpen)
            {
                return SP4.ReadExisting().Trim();
            }
            return "";
        }

        //以上串口4函数

        //一键连接
        private void AutoConnection_Click(object sender, EventArgs e)
        {
            if (AutoConnection.Text == "一键连接")
            {
                Connection1.Enabled = false;
                Connection2.Enabled = false;
                Connection3.Enabled = false;
                Connection4.Enabled = false;
                SerialPort1.Enabled = false;
                SerialPort2.Enabled = false;
                SerialPort3.Enabled = false;
                SerialPort4.Enabled = false;
                Lock.Enabled = false;
                AutoConnection.Text = "一键断开";
                AutoAllocationPort();
                Connection1_Click(sender, e);
                Connection2_Click(sender, e);
                Connection3_Click(sender, e);
                Connection4_Click(sender, e);
            }
            else
            {
                Lock.Enabled = true;
                Connection1.Enabled = true;
                Connection2.Enabled = true;
                Connection3.Enabled = true;
                Connection4.Enabled = true;
                SerialPort1.Enabled = true;
                SerialPort2.Enabled = true;
                SerialPort3.Enabled = true;
                SerialPort4.Enabled = true;
                AutoConnection.Text = "一键连接";
                Connection1_Click(sender, e);
                Connection2_Click(sender, e);
                Connection3_Click(sender, e);
                Connection4_Click(sender, e);
            }
        }
        
        //自动分配串口函数
        private void AutoAllocationPort()
        {
            string[] ArryPort = SerialPort.GetPortNames();
            if (ArryPort.Length > 0)
            {
                for (int i = 0; i < ArryPort.Length; i++)
                {
                    if (this.SerialPort1.Text == ArryPort[i])
                    {
                        continue;
                    }
                    if (this.SerialPort2.Text == ArryPort[i])
                    {
                        continue;
                    }
                    if (this.SerialPort3.Text == ArryPort[i])
                    {
                        continue;
                    }
                    if (this.SerialPort4.Text == ArryPort[i])
                    {
                        continue;
                    }
                    if (this.SerialPort1.Text == "")
                    {
                        this.SerialPort1.Text = ArryPort[i];
                    }
                    else if (this.SerialPort2.Text == "")
                    {
                        this.SerialPort2.Text = ArryPort[i];
                    }
                    else if (this.SerialPort3.Text == "")
                    {
                        this.SerialPort3.Text = ArryPort[i];
                    }
                    else if (this.SerialPort4.Text == "")
                    {
                        this.SerialPort4.Text = ArryPort[i];
                    }
                }
            }
        }

        //以下查询与导出页面

        //导出按钮
        private void ExportByzhidan_Click(object sender, EventArgs e)
        {
            if (this.dataGridView2.CurrentRow == null)
            {
                MessageBox.Show("没有可导出的数据!");
                return;
            }

            try
            {
                if (ExcelHelperForCs.GridViewToExcel(this.dataGridView2) != null)
                    MessageBox.Show("导出完成");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        //查询按钮
        private void CheckData_Click(object sender, EventArgs e)
        {
            if (this.zhidan.Text == "")
            {
                MessageBox.Show("请先选择制单");
                return;
            }
            this.dataGridView2.DataSource = "";
            string SqlStr="";
            int co = 1;
            DataTable dd = new DataTable();
            dd.Columns.Add(" ");
            dd.Columns.Add("制单号");
            dd.Columns.Add("IMEI");
            dd.Columns.Add("发送指令");
            dd.Columns.Add("返回值");
            dd.Columns.Add("测试结果");
            dd.Columns.Add("测试时间");
            dd.Columns.Add("测试员");
            dd.Columns.Add("重测次数");
            dd.Columns.Add("重测时间");
            if (this.RadioAll.Checked == true)
            {
                SqlStr += "SELECT * FROM [dbo].[Gps_OqcTestParam] where Zhidan ='" + zhidan.Text + "'";
            }
            else if(this.RadioFine.Checked == true)
            {
                SqlStr += "SELECT * FROM [dbo].[Gps_OqcTestParam] where Zhidan ='" + zhidan.Text + "' AND TestResule = '1'";
            }
            else
            {
                SqlStr += "SELECT * FROM [dbo].[Gps_OqcTestParam] where Zhidan ='" + zhidan.Text + "'  AND TestResule = '0'";
            }
            OTP = OTPB.SelectOqcTestBLL(SqlStr);
            if (OTP.Count <= 0)
            {
                MessageBox.Show("无相关数据");
                this.dataGridView2.DataSource = "";
                return;
            }
            foreach (OqcTestParam1 a in OTP)
            {
                string res;
                if(a.TestResule == 0)
                {
                    res = "失败";
                }
                else
                {
                    res = "成功";
                }
                dd.Rows.Add(co, a.Zhidan, a.IMEI, a.SendCommand.Replace("@@", ";").Replace("\r\n",""), a.ReceiveValue.Replace("@@", ";").Replace("\r\n", ""), res, a.TestTime, a.TestUser, a.ReTestNum, a.ReTestTime);
                co++;
            }
            dataGridView2.DataSource = dd;
            dataGridView2.Columns[0].Width = 20;
            dataGridView2.Columns[1].Width = 120;
            dataGridView2.Columns[2].Width = 120;
            dataGridView2.Columns[3].Width = 150;
            dataGridView2.Columns[4].Width = 150;
            dataGridView2.Columns[5].Width = 80;
            dataGridView2.Columns[6].Width = 130;
            dataGridView2.Columns[7].Width = 100;
            dataGridView2.Columns[8].Width = 80;
            dataGridView2.Columns[9].Width = 130;
        }

        //重测查询
        private void ReTextCheck_Click(object sender, EventArgs e)
        {
            if (this.zhidan.Text == "")
            {
                MessageBox.Show("请先选择制单");
                return;
            }
            this.dataGridView2.DataSource = "";
            string SqlStr = "";
            int co = 1;
            DataTable dd = new DataTable();
            dd.Columns.Add(" ");
            dd.Columns.Add("制单号");
            dd.Columns.Add("IMEI");
            dd.Columns.Add("发送指令");
            dd.Columns.Add("返回值");
            dd.Columns.Add("测试结果");
            dd.Columns.Add("测试时间");
            dd.Columns.Add("测试员");
            dd.Columns.Add("重测次数");
            dd.Columns.Add("重测时间");
            if (this.RadioAll.Checked == true)
            {
                SqlStr += "SELECT * FROM [dbo].[Gps_OqcTestParam] where Zhidan ='" + zhidan.Text + "'AND ReTestNum!='0'";
            }
            else if (this.RadioFine.Checked == true)
            {
                SqlStr += "SELECT * FROM [dbo].[Gps_OqcTestParam] where Zhidan ='" + zhidan.Text + "' AND ReTestNum!='0' AND TestResule = '1'";
            }
            else
            {
                SqlStr += "SELECT * FROM [dbo].[Gps_OqcTestParam] where Zhidan ='" + zhidan.Text + "' AND ReTestNum!='0' AND TestResule = '0'";
            }
            OTP = OTPB.SelectOqcTestBLL(SqlStr);
            if (OTP.Count <= 0)
            {
                MessageBox.Show("无相关数据");
                this.dataGridView2.DataSource = "";
                return;
            }
            foreach (OqcTestParam1 a in OTP)
            {
                string res;
                if (a.TestResule == 0)
                {
                    res = "失败";
                }
                else
                {
                    res = "成功";
                }
                dd.Rows.Add(co, a.Zhidan, a.IMEI, a.SendCommand.Replace("@@", ";").Replace("\r\n", ""), a.ReceiveValue.Replace("@@", ";").Replace("\r\n", ""), res, a.TestTime, a.TestUser, a.ReTestNum, a.ReTestTime);
                co++;
            }
            dataGridView2.DataSource = dd;
            dataGridView2.Columns[0].Width = 20;
            dataGridView2.Columns[1].Width = 120;
            dataGridView2.Columns[2].Width = 120;
            dataGridView2.Columns[3].Width = 150;
            dataGridView2.Columns[4].Width = 150;
            dataGridView2.Columns[5].Width = 80;
            dataGridView2.Columns[6].Width = 130;
            dataGridView2.Columns[7].Width = 100;
            dataGridView2.Columns[8].Width = 80;
            dataGridView2.Columns[9].Width = 130;
        }
    }
}
