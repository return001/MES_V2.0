using System;
using System.Data;
using System.Windows.Forms;
using System.Collections.Generic;
using Print.Message.Bll;
using System.Drawing.Printing;
using Seagull.BarTender.Print;
using System.Text.RegularExpressions;
using ExcelPrint.Param.Bll;
using System.IO;
using Print_Message;

namespace WindowsForms_print
{
    public partial class PrintFromExcel : Form
    {
        InputExcelBLL IEB = new InputExcelBLL();
        ManuExcelPrintParamBLL MEPPB = new ManuExcelPrintParamBLL();
        List<ManuExcelPrintParam> mepp = new List<ManuExcelPrintParam>();
        Engine btEngine = new Engine();
        LabelFormatDocument btFormat;

        //打印参数
        int TN=1;
        Messages messages;
        int waitout = 10000;
        string lj = "";

        public PrintFromExcel()
        {
            InitializeComponent();
            int wid = Screen.PrimaryScreen.WorkingArea.Width;
            this.Width = wid;
            this.dataGridView1.Width = wid;
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

        private void PrintFromExcel_Load(object sender, EventArgs e)
        {
            PrintDocument print = new PrintDocument();
            string sDefault = print.PrinterSettings.PrinterName;//默认打印机名
            this.Printer.Text = sDefault;
            foreach (string sPrint in PrinterSettings.InstalledPrinters)//获取所有打印机名称
            {
                Printer.Items.Add(sPrint);
            }
            btEngine.Start();
        }

        private void Open_Template_Click(object sender, EventArgs e)
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
                    MessageBox.Show("请选择正确的btw文件！");
                }
                else
                {
                    this.Select_Template.Text = path;
                }
            }
        }

        private void Import_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.ShowDialog();
            string path = dialog.FileName;
            if (path != "")
            {
                string strExtension = path.Substring(path.LastIndexOf('.'));
                if (strExtension != ".xls" && strExtension != ".xlsx")
                {
                    MessageBox.Show("请选择xls文件！");
                }
                else
                {
                    this.dataGridView1.DataSource = "";
                    this.ImportPath.Text = path;
                    DataTable dt = IEB.GetExcelDatatable(path);
                    //DataRow dr2 = dt.Rows[0];
                    //DataTable dd = new DataTable();
                    //dd.Columns.Add(new DataColumn(dr2[0].ToString(), typeof(string)));
                    //dd.Columns.Add(new DataColumn(dr2[1].ToString(), typeof(string)));
                    //dd.Columns.Add(new DataColumn(dr2[2].ToString(), typeof(string)));
                    //dd.Columns.Add(new DataColumn(dr2[3].ToString(), typeof(string)));
                    dataGridView1.DataSource = dt;
                }
            }
        }

        public static bool IsTrue(string inString)
        {
            Regex regex = new Regex("^\\d+\\-+\\d");
            return regex.IsMatch(inString.Trim());
        }

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
                    MessageBox.Show("请输入数字\r\n");
                    this.TemplateNum.Text = 1.ToString();
                    this.TemplateNum.Focus();
                }
            }
        }

        private void RowNumber_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                if(this.ImportPath.Text == "")
                {
                    MessageBox.Show("请先导入Excel");
                    this.RowNumber.Clear();
                    this.RowNumber.Focus();
                    return;
                }
                if (this.RowNumber.Text != "")
                {
                    int i = 1;
                    this.dataGridView1.DataSource = "";
                    DataTable dt = IEB.GetExcelDatatable(this.ImportPath.Text);
                    DataRow dr2 = dt.Rows[0];
                    DataTable dd = new DataTable();
                    dd.Columns.Add(" ");
                    dd.Columns.Add(new DataColumn(dr2[0].ToString(), typeof(string)));
                    dd.Columns.Add(new DataColumn(dr2[1].ToString(), typeof(string)));
                    dd.Columns.Add(new DataColumn(dr2[2].ToString(), typeof(string)));
                    dd.Columns.Add(new DataColumn(dr2[3].ToString(), typeof(string)));
                    dd.Columns.Add(new DataColumn(dr2[4].ToString(), typeof(string)));
                    if (IsNumeric(this.RowNumber.Text))
                    {
                        DataRow dr3 = dt.Rows[int.Parse(this.RowNumber.Text)];
                        if (dr3[0].ToString() == "")
                        {
                            MessageBox.Show("您输入的行数大于Excel行数");
                            this.RowNumber.Clear();
                            this.RowNumber.Focus();
                            return;
                        }
                        dd.Rows.Add(1, dr3[0].ToString(), dr3[1].ToString(), dr3[2].ToString(), dr3[3].ToString(), dr3[4].ToString());
                        dataGridView1.DataSource = dd;
                    }
                    else
                    {
                        if (IsTrue(this.RowNumber.Text))
                        {
                            string[] range = this.RowNumber.Text.Split('-');
                            if(int.Parse(range[0])> int.Parse(range[1]))
                            {
                                MessageBox.Show("起始位不能大于终止位");
                                this.RowNumber.Clear();
                                this.RowNumber.Focus();
                                return;
                            }
                            for (int j = int.Parse(range[0]); j <= int.Parse(range[1]); j++)
                            {
                                DataRow dr3 = dt.Rows[j];
                                dd.Rows.Add(i, dr3[0].ToString(), dr3[1].ToString(), dr3[2].ToString(), dr3[3].ToString(), dr3[4].ToString());
                                dataGridView1.DataSource = dd;
                                i++;
                            }
                        }
                        else
                        {
                            MessageBox.Show("请请输入'数字-数字'格式");
                            this.RowNumber.Clear();
                            this.RowNumber.Focus();
                        }
                    }
                }
            }
        }

        private void ExcelToPrint_Click(object sender, EventArgs e)
        {
            if (this.Select_Template.Text != "")
            {
                if (IsNumeric(this.RowNumber.Text))
                {
                    DataTable dt = IEB.GetExcelDatatable(this.ImportPath.Text);
                    DataRow dr3 = dt.Rows[int.Parse(this.RowNumber.Text)];
                    if (!MEPPB.CheckIMEIBLL(dr3[0].ToString(), dr3[1].ToString()))
                    {
                        lj = this.Select_Template.Text;
                        LabelFormatDocument btFormat = btEngine.Documents.Open(lj);
                        //指定打印机名称
                        btFormat.PrintSetup.PrinterName = this.Printer.Text;
                        //对模板相应字段进行赋值
                        btFormat.SubStrings["IMEI1"].Value = dr3[0].ToString();
                        btFormat.SubStrings["IMEI2"].Value = dr3[1].ToString();
                        btFormat.SubStrings["IMEI3"].Value = dr3[2].ToString();
                        btFormat.SubStrings["IMEI4"].Value = dr3[3].ToString();
                        btFormat.SubStrings["IMEI5"].Value = dr3[4].ToString();
                        btFormat.SubStrings["ProductDate"].Value = DateTime.Now.ToString("yyyy.MM.dd");
                        //打印份数,同序列打印的份数
                        btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                        //记录数据
                        mepp.Add(new ManuExcelPrintParam()
                        {
                            IMEI1 = dr3[0].ToString(),
                            IMEI2 = dr3[1].ToString(),
                            IMEI3 = dr3[2].ToString(),
                            IMEI4 = dr3[3].ToString(),
                            IMEI5 = dr3[4].ToString(),
                            PrintTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff"),
                            Template = this.Select_Template.Text
                        });
                        if (MEPPB.InsertManuExcelPrintBLL(mepp))
                        {
                            Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                            Form1.Log("Excel打印了机身贴IMEI号为" + dr3[1].ToString() + "的制单", null);
                        }
                    }
                    else
                    {
                        this.remined.AppendText(dr3[0].ToString()+"或"+ dr3[1].ToString()+"重号\r\n");
                    }
                }
                else if (IsTrue(this.RowNumber.Text))
                {
                    lj = this.Select_Template.Text;
                    LabelFormatDocument btFormat = btEngine.Documents.Open(lj);
                    //指定打印机名称
                    btFormat.PrintSetup.PrinterName = this.Printer.Text;
                    string[] range = this.RowNumber.Text.Split('-');
                    DataTable dt = IEB.GetExcelDatatable(this.ImportPath.Text);
                    for (int i = int.Parse(range[0]);i<= int.Parse(range[1]); i++)
                    {
                        DataRow dr3 = dt.Rows[i];
                        if (!MEPPB.CheckIMEIBLL(dr3[0].ToString(), dr3[1].ToString()))
                        {
                            //对模板相应字段进行赋值
                            btFormat.SubStrings["IMEI1"].Value = dr3[0].ToString();
                            btFormat.SubStrings["IMEI2"].Value = dr3[1].ToString();
                            btFormat.SubStrings["IMEI3"].Value = dr3[2].ToString();
                            btFormat.SubStrings["IMEI4"].Value = dr3[3].ToString();
                            btFormat.SubStrings["IMEI5"].Value = dr3[4].ToString();
                            btFormat.SubStrings["ProductDate"].Value = DateTime.Now.ToString("yyyy.MM.dd");
                            //打印份数,同序列打印的份数
                            btFormat.PrintSetup.IdenticalCopiesOfLabel = TN;
                            //记录数据
                            mepp.Add(new ManuExcelPrintParam()
                            {
                                IMEI1 = dr3[0].ToString(),
                                IMEI2 = dr3[1].ToString(),
                                IMEI3 = dr3[2].ToString(),
                                IMEI4 = dr3[3].ToString(),
                                IMEI5 = dr3[4].ToString(),
                                PrintTime = System.DateTime.Now.ToString("yyyy.MM.dd HH:mm:ss:fff"),
                                Template = this.Select_Template.Text
                            });
                            if (MEPPB.InsertManuExcelPrintBLL(mepp))
                            {
                                Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                                Form1.Log("Excel打印了机身贴IMEI号为" + dr3[1].ToString() + "的制单", null);
                            }
                        }
                        else
                        {
                            this.remined.AppendText(dr3[0].ToString() + "或" + dr3[1].ToString() + "重号\r\n");
                        }
                    }
                }
                else
                {
                    MessageBox.Show("请输入正确的行数格式：数字-数字");
                    this.RowNumber.Clear();
                    this.RowNumber.Focus();
                    this.dataGridView1.DataSource = "";
                }
            }
            else {
                MessageBox.Show("请先选择模板");
            }
        }

        private void OpenExcel_Click(object sender, EventArgs e)
        {
            if (this.ImportPath.Text == "")
            {
                MessageBox.Show("请先导入Excel");
            }
            else
            {
                string path = this.ImportPath.Text;
                if (File.Exists(path))
                {
                    System.Diagnostics.Process.Start(path);
                }
                else
                {
                    MessageBox.Show("Excel不存在");
                }
            }
        }

        private void E_Check_Click(object sender, EventArgs e)
        {
            if (this.WhatToCheck.Text != "")
            {
                this.dataGridView1.DataSource = "";
                int co=1;
                mepp = MEPPB.SelectByImei1to5BLL(this.WhatToCheck.Text);
                if (mepp.Count == 0)
                {
                    this.remined.AppendText("查找不到关于" + this.WhatToCheck.Text + "的记录");
                    this.WhatToCheck.Clear();
                }
                else
                {
                    DataTable dd = new DataTable();
                    dd.Columns.Add(" ");
                    dd.Columns.Add("IMEI1");
                    dd.Columns.Add("IMEI2");
                    dd.Columns.Add("IMEI3");
                    dd.Columns.Add("IMEI4");
                    dd.Columns.Add("IMEI5");
                    dd.Columns.Add("打印时间");
                    dd.Columns.Add("打印模板");
                    foreach (ManuExcelPrintParam a in mepp)
                    {
                        dd.Rows.Add(co, a.IMEI1, a.IMEI2, a.IMEI3, a.IMEI4, a.IMEI5,a.PrintTime,a.Template);
                        co++;
                    }
                    dataGridView1.DataSource = dd;
                }
            }
        }
    }
}
