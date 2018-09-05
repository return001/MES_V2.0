using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Print.Message.Bll;
using System.Drawing.Printing;
using Seagull.BarTender.Print;

namespace WindowsForms_print
{
    public partial class PrintFromExcel : Form
    {
        InputExcelBLL IEB = new InputExcelBLL();
        public PrintFromExcel()
        {
            InitializeComponent();
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

        private void RowNumber_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
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
                    if (IsNumeric(this.RowNumber.Text))
                    {
                        DataRow dr3 = dt.Rows[int.Parse(this.RowNumber.Text)];
                        dd.Rows.Add(1, dr3[0].ToString(), dr3[1].ToString(), dr3[2].ToString(), dr3[3].ToString());
                        dataGridView1.DataSource = dd;
                    }
                    else {
                        string[] range = this.RowNumber.Text.Split('-');
                        for (int j = int.Parse(range[0]); j <= int.Parse(range[1]); j++) {
                            DataRow dr3 = dt.Rows[j];
                            dd.Rows.Add(i, dr3[0].ToString(), dr3[1].ToString(), dr3[2].ToString(), dr3[3].ToString());
                            dataGridView1.DataSource = dd;
                            i++;
                        }
                    }
                }
            }
        }

        private void ExcelToPrint_Click(object sender, EventArgs e)
        {
            if (this.Select_Template.Text != "")
            {
                Engine btEngine = new Engine();
                btEngine.Start();
                string lj = "";
                lj = this.Select_Template.Text;
                LabelFormatDocument btFormat = btEngine.Documents.Open(lj);
                //指定打印机名称
                btFormat.PrintSetup.PrinterName = this.Printer.Text;

                DataTable dt = IEB.GetExcelDatatable(this.ImportPath.Text);
                DataRow dr3 = dt.Rows[int.Parse(this.RowNumber.Text) - 1];
                //对模板相应字段进行赋值
                btFormat.SubStrings["IMEI"].Value = dr3[1].ToString();

                //打印份数,同序列打印的份数
                btFormat.PrintSetup.IdenticalCopiesOfLabel = 1;
                Messages messages;
                int waitout = 10000;
                Result nResult1 = btFormat.Print("标签打印软件", waitout, out messages);
                Form1.Log("Excel打印了机身贴IMEI号为" + dr3[1].ToString() + "的制单", null);
                btFormat.PrintSetup.Cache.FlushInterval = CacheFlushInterval.PerSession;
            }
            else {
                MessageBox.Show("请先选择模板");
            }
        }

    }
}
