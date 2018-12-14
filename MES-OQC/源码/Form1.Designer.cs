namespace OqcTool
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle5 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle4 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle6 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle7 = new System.Windows.Forms.DataGridViewCellStyle();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label4 = new System.Windows.Forms.Label();
            this.TCUpdate = new System.Windows.Forms.Button();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.TestName = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.TestCommand = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ReturnValue = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.Remind1 = new System.Windows.Forms.Label();
            this.S1Log = new System.Windows.Forms.TextBox();
            this.Connection1 = new System.Windows.Forms.Button();
            this.SerialPort1 = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox7 = new System.Windows.Forms.GroupBox();
            this.Remind4 = new System.Windows.Forms.Label();
            this.S4Log = new System.Windows.Forms.TextBox();
            this.Connection4 = new System.Windows.Forms.Button();
            this.SerialPort4 = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.Remind3 = new System.Windows.Forms.Label();
            this.S3Log = new System.Windows.Forms.TextBox();
            this.Connection3 = new System.Windows.Forms.Button();
            this.SerialPort3 = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.Remind2 = new System.Windows.Forms.Label();
            this.S2Log = new System.Windows.Forms.TextBox();
            this.Connection2 = new System.Windows.Forms.Button();
            this.SerialPort2 = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.zhidan = new System.Windows.Forms.ComboBox();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.Index = new System.Windows.Forms.TabPage();
            this.Check = new System.Windows.Forms.TabPage();
            this.CheckData = new System.Windows.Forms.Button();
            this.RadioBad = new System.Windows.Forms.RadioButton();
            this.RadioFine = new System.Windows.Forms.RadioButton();
            this.RadioAll = new System.Windows.Forms.RadioButton();
            this.ExportByzhidan = new System.Windows.Forms.Button();
            this.dataGridView2 = new System.Windows.Forms.DataGridView();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.SqlConfig = new System.Windows.Forms.Button();
            this.AutoConnection = new System.Windows.Forms.Button();
            this.Lock = new System.Windows.Forms.Button();
            this.OqcUser = new System.Windows.Forms.TextBox();
            this.OqcLogout = new System.Windows.Forms.Button();
            this.OqcLogin = new System.Windows.Forms.Button();
            this.ReTextCheck = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.groupBox2.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox7.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.Index.SuspendLayout();
            this.Check.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView2)).BeginInit();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.TCUpdate);
            this.groupBox1.Controls.Add(this.dataGridView1);
            this.groupBox1.Location = new System.Drawing.Point(6, 6);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(463, 648);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "测试项";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(506, 369);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(47, 12);
            this.label4.TabIndex = 0;
            this.label4.Text = "串口1：";
            // 
            // TCUpdate
            // 
            this.TCUpdate.Enabled = false;
            this.TCUpdate.Location = new System.Drawing.Point(6, 607);
            this.TCUpdate.Name = "TCUpdate";
            this.TCUpdate.Size = new System.Drawing.Size(442, 38);
            this.TCUpdate.TabIndex = 1;
            this.TCUpdate.Text = "确认修改";
            this.TCUpdate.UseVisualStyleBackColor = true;
            this.TCUpdate.Click += new System.EventHandler(this.TCUpdate_Click);
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToResizeColumns = false;
            this.dataGridView1.AllowUserToResizeRows = false;
            this.dataGridView1.BackgroundColor = System.Drawing.Color.White;
            dataGridViewCellStyle1.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            dataGridViewCellStyle1.BackColor = System.Drawing.SystemColors.Control;
            dataGridViewCellStyle1.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            dataGridViewCellStyle1.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle1.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle1.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle1.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridView1.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle1;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.TestName,
            this.TestCommand,
            this.ReturnValue});
            this.dataGridView1.Location = new System.Drawing.Point(6, 20);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.ReadOnly = true;
            dataGridViewCellStyle5.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            dataGridViewCellStyle5.BackColor = System.Drawing.SystemColors.Control;
            dataGridViewCellStyle5.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            dataGridViewCellStyle5.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle5.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle5.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle5.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridView1.RowHeadersDefaultCellStyle = dataGridViewCellStyle5;
            this.dataGridView1.RowTemplate.Height = 23;
            this.dataGridView1.Size = new System.Drawing.Size(442, 581);
            this.dataGridView1.TabIndex = 0;
            this.dataGridView1.CellValueChanged += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellValueChanged);
            // 
            // TestName
            // 
            dataGridViewCellStyle2.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            this.TestName.DefaultCellStyle = dataGridViewCellStyle2;
            this.TestName.HeaderText = "测试项";
            this.TestName.Name = "TestName";
            this.TestName.ReadOnly = true;
            this.TestName.Width = 90;
            // 
            // TestCommand
            // 
            dataGridViewCellStyle3.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            this.TestCommand.DefaultCellStyle = dataGridViewCellStyle3;
            this.TestCommand.HeaderText = "测试指令";
            this.TestCommand.Name = "TestCommand";
            this.TestCommand.ReadOnly = true;
            this.TestCommand.Width = 150;
            // 
            // ReturnValue
            // 
            dataGridViewCellStyle4.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            this.ReturnValue.DefaultCellStyle = dataGridViewCellStyle4;
            this.ReturnValue.HeaderText = "返回值";
            this.ReturnValue.Name = "ReturnValue";
            this.ReturnValue.ReadOnly = true;
            this.ReturnValue.Width = 150;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.groupBox4);
            this.groupBox2.Controls.Add(this.groupBox7);
            this.groupBox2.Controls.Add(this.groupBox6);
            this.groupBox2.Controls.Add(this.groupBox5);
            this.groupBox2.Location = new System.Drawing.Point(475, 6);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(651, 648);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "测试区";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.Remind1);
            this.groupBox4.Controls.Add(this.S1Log);
            this.groupBox4.Controls.Add(this.Connection1);
            this.groupBox4.Controls.Add(this.SerialPort1);
            this.groupBox4.Controls.Add(this.label2);
            this.groupBox4.Location = new System.Drawing.Point(22, 20);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(251, 303);
            this.groupBox4.TabIndex = 0;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "串口1";
            // 
            // Remind1
            // 
            this.Remind1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Remind1.Font = new System.Drawing.Font("宋体", 36F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Remind1.Location = new System.Drawing.Point(0, 228);
            this.Remind1.Name = "Remind1";
            this.Remind1.Size = new System.Drawing.Size(251, 72);
            this.Remind1.TabIndex = 5;
            this.Remind1.Text = "等待";
            this.Remind1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // S1Log
            // 
            this.S1Log.BackColor = System.Drawing.Color.White;
            this.S1Log.Location = new System.Drawing.Point(0, 90);
            this.S1Log.Multiline = true;
            this.S1Log.Name = "S1Log";
            this.S1Log.ReadOnly = true;
            this.S1Log.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.S1Log.Size = new System.Drawing.Size(251, 140);
            this.S1Log.TabIndex = 3;
            // 
            // Connection1
            // 
            this.Connection1.Enabled = false;
            this.Connection1.Location = new System.Drawing.Point(160, 26);
            this.Connection1.Name = "Connection1";
            this.Connection1.Size = new System.Drawing.Size(75, 23);
            this.Connection1.TabIndex = 2;
            this.Connection1.Text = "打开串口";
            this.Connection1.UseVisualStyleBackColor = true;
            this.Connection1.Click += new System.EventHandler(this.Connection1_Click);
            // 
            // SerialPort1
            // 
            this.SerialPort1.Enabled = false;
            this.SerialPort1.FormattingEnabled = true;
            this.SerialPort1.Location = new System.Drawing.Point(61, 28);
            this.SerialPort1.Name = "SerialPort1";
            this.SerialPort1.Size = new System.Drawing.Size(86, 20);
            this.SerialPort1.TabIndex = 1;
            this.SerialPort1.DropDown += new System.EventHandler(this.SerialPort1_DropDown);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(15, 32);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(47, 12);
            this.label2.TabIndex = 0;
            this.label2.Text = "串口1：";
            // 
            // groupBox7
            // 
            this.groupBox7.Controls.Add(this.Remind4);
            this.groupBox7.Controls.Add(this.S4Log);
            this.groupBox7.Controls.Add(this.Connection4);
            this.groupBox7.Controls.Add(this.SerialPort4);
            this.groupBox7.Controls.Add(this.label6);
            this.groupBox7.Location = new System.Drawing.Point(340, 339);
            this.groupBox7.Name = "groupBox7";
            this.groupBox7.Size = new System.Drawing.Size(251, 303);
            this.groupBox7.TabIndex = 0;
            this.groupBox7.TabStop = false;
            this.groupBox7.Text = "串口4";
            // 
            // Remind4
            // 
            this.Remind4.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Remind4.Font = new System.Drawing.Font("宋体", 36F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Remind4.Location = new System.Drawing.Point(0, 231);
            this.Remind4.Name = "Remind4";
            this.Remind4.Size = new System.Drawing.Size(251, 72);
            this.Remind4.TabIndex = 5;
            this.Remind4.Text = "等待";
            this.Remind4.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // S4Log
            // 
            this.S4Log.BackColor = System.Drawing.Color.White;
            this.S4Log.Location = new System.Drawing.Point(0, 92);
            this.S4Log.Multiline = true;
            this.S4Log.Name = "S4Log";
            this.S4Log.ReadOnly = true;
            this.S4Log.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.S4Log.Size = new System.Drawing.Size(251, 140);
            this.S4Log.TabIndex = 3;
            // 
            // Connection4
            // 
            this.Connection4.Enabled = false;
            this.Connection4.Location = new System.Drawing.Point(159, 23);
            this.Connection4.Name = "Connection4";
            this.Connection4.Size = new System.Drawing.Size(75, 23);
            this.Connection4.TabIndex = 2;
            this.Connection4.Text = "打开串口";
            this.Connection4.UseVisualStyleBackColor = true;
            this.Connection4.Click += new System.EventHandler(this.Connection4_Click);
            // 
            // SerialPort4
            // 
            this.SerialPort4.Enabled = false;
            this.SerialPort4.FormattingEnabled = true;
            this.SerialPort4.Location = new System.Drawing.Point(62, 25);
            this.SerialPort4.Name = "SerialPort4";
            this.SerialPort4.Size = new System.Drawing.Size(86, 20);
            this.SerialPort4.TabIndex = 1;
            this.SerialPort4.DropDown += new System.EventHandler(this.SerialPort4_DropDown);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(16, 30);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(47, 12);
            this.label6.TabIndex = 0;
            this.label6.Text = "串口4：";
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.Remind3);
            this.groupBox6.Controls.Add(this.S3Log);
            this.groupBox6.Controls.Add(this.Connection3);
            this.groupBox6.Controls.Add(this.SerialPort3);
            this.groupBox6.Controls.Add(this.label5);
            this.groupBox6.Location = new System.Drawing.Point(22, 339);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(251, 303);
            this.groupBox6.TabIndex = 0;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "串口3";
            // 
            // Remind3
            // 
            this.Remind3.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Remind3.Font = new System.Drawing.Font("宋体", 36F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Remind3.Location = new System.Drawing.Point(0, 231);
            this.Remind3.Name = "Remind3";
            this.Remind3.Size = new System.Drawing.Size(251, 72);
            this.Remind3.TabIndex = 5;
            this.Remind3.Text = "等待";
            this.Remind3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // S3Log
            // 
            this.S3Log.BackColor = System.Drawing.Color.White;
            this.S3Log.Location = new System.Drawing.Point(0, 92);
            this.S3Log.Multiline = true;
            this.S3Log.Name = "S3Log";
            this.S3Log.ReadOnly = true;
            this.S3Log.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.S3Log.Size = new System.Drawing.Size(251, 140);
            this.S3Log.TabIndex = 3;
            // 
            // Connection3
            // 
            this.Connection3.Enabled = false;
            this.Connection3.Location = new System.Drawing.Point(160, 23);
            this.Connection3.Name = "Connection3";
            this.Connection3.Size = new System.Drawing.Size(75, 23);
            this.Connection3.TabIndex = 2;
            this.Connection3.Text = "打开串口";
            this.Connection3.UseVisualStyleBackColor = true;
            this.Connection3.Click += new System.EventHandler(this.Connection3_Click);
            // 
            // SerialPort3
            // 
            this.SerialPort3.Enabled = false;
            this.SerialPort3.FormattingEnabled = true;
            this.SerialPort3.Location = new System.Drawing.Point(61, 25);
            this.SerialPort3.Name = "SerialPort3";
            this.SerialPort3.Size = new System.Drawing.Size(86, 20);
            this.SerialPort3.TabIndex = 1;
            this.SerialPort3.DropDown += new System.EventHandler(this.SerialPort3_DropDown);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(15, 30);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(47, 12);
            this.label5.TabIndex = 0;
            this.label5.Text = "串口3：";
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.Remind2);
            this.groupBox5.Controls.Add(this.S2Log);
            this.groupBox5.Controls.Add(this.Connection2);
            this.groupBox5.Controls.Add(this.SerialPort2);
            this.groupBox5.Controls.Add(this.label3);
            this.groupBox5.Location = new System.Drawing.Point(340, 20);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(251, 303);
            this.groupBox5.TabIndex = 0;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "串口2";
            // 
            // Remind2
            // 
            this.Remind2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Remind2.Font = new System.Drawing.Font("宋体", 36F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Remind2.Location = new System.Drawing.Point(0, 228);
            this.Remind2.Name = "Remind2";
            this.Remind2.Size = new System.Drawing.Size(251, 72);
            this.Remind2.TabIndex = 5;
            this.Remind2.Text = "等待";
            this.Remind2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // S2Log
            // 
            this.S2Log.BackColor = System.Drawing.Color.White;
            this.S2Log.Location = new System.Drawing.Point(0, 90);
            this.S2Log.Multiline = true;
            this.S2Log.Name = "S2Log";
            this.S2Log.ReadOnly = true;
            this.S2Log.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.S2Log.Size = new System.Drawing.Size(251, 140);
            this.S2Log.TabIndex = 3;
            // 
            // Connection2
            // 
            this.Connection2.Enabled = false;
            this.Connection2.Location = new System.Drawing.Point(159, 26);
            this.Connection2.Name = "Connection2";
            this.Connection2.Size = new System.Drawing.Size(75, 23);
            this.Connection2.TabIndex = 2;
            this.Connection2.Text = "打开串口";
            this.Connection2.UseVisualStyleBackColor = true;
            this.Connection2.Click += new System.EventHandler(this.Connection2_Click);
            // 
            // SerialPort2
            // 
            this.SerialPort2.Enabled = false;
            this.SerialPort2.FormattingEnabled = true;
            this.SerialPort2.Location = new System.Drawing.Point(62, 28);
            this.SerialPort2.Name = "SerialPort2";
            this.SerialPort2.Size = new System.Drawing.Size(86, 20);
            this.SerialPort2.TabIndex = 1;
            this.SerialPort2.DropDown += new System.EventHandler(this.SerialPort2_DropDown);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(16, 32);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(47, 12);
            this.label3.TabIndex = 0;
            this.label3.Text = "串口2：";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(5, 23);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(63, 14);
            this.label1.TabIndex = 0;
            this.label1.Text = "订单号：";
            // 
            // zhidan
            // 
            this.zhidan.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
            this.zhidan.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
            this.zhidan.Enabled = false;
            this.zhidan.FormattingEnabled = true;
            this.zhidan.Location = new System.Drawing.Point(66, 20);
            this.zhidan.Name = "zhidan";
            this.zhidan.Size = new System.Drawing.Size(166, 20);
            this.zhidan.TabIndex = 1;
            this.zhidan.DropDown += new System.EventHandler(this.zhidan_DropDown);
            this.zhidan.SelectedIndexChanged += new System.EventHandler(this.zhidan_SelectedIndexChanged);
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.Index);
            this.tabControl1.Controls.Add(this.Check);
            this.tabControl1.ItemSize = new System.Drawing.Size(98, 45);
            this.tabControl1.Location = new System.Drawing.Point(2, 93);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(1134, 710);
            this.tabControl1.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
            this.tabControl1.TabIndex = 2;
            // 
            // Index
            // 
            this.Index.Controls.Add(this.groupBox1);
            this.Index.Controls.Add(this.groupBox2);
            this.Index.Location = new System.Drawing.Point(4, 49);
            this.Index.Name = "Index";
            this.Index.Padding = new System.Windows.Forms.Padding(3);
            this.Index.Size = new System.Drawing.Size(1126, 657);
            this.Index.TabIndex = 0;
            this.Index.Text = "主页";
            this.Index.UseVisualStyleBackColor = true;
            // 
            // Check
            // 
            this.Check.Controls.Add(this.ReTextCheck);
            this.Check.Controls.Add(this.CheckData);
            this.Check.Controls.Add(this.RadioBad);
            this.Check.Controls.Add(this.RadioFine);
            this.Check.Controls.Add(this.RadioAll);
            this.Check.Controls.Add(this.ExportByzhidan);
            this.Check.Controls.Add(this.dataGridView2);
            this.Check.Location = new System.Drawing.Point(4, 49);
            this.Check.Name = "Check";
            this.Check.Padding = new System.Windows.Forms.Padding(3);
            this.Check.Size = new System.Drawing.Size(1126, 657);
            this.Check.TabIndex = 1;
            this.Check.Text = "查询与导出";
            this.Check.UseVisualStyleBackColor = true;
            // 
            // CheckData
            // 
            this.CheckData.Enabled = false;
            this.CheckData.Location = new System.Drawing.Point(233, 16);
            this.CheckData.Name = "CheckData";
            this.CheckData.Size = new System.Drawing.Size(91, 23);
            this.CheckData.TabIndex = 5;
            this.CheckData.Text = "查询";
            this.CheckData.UseVisualStyleBackColor = true;
            this.CheckData.Click += new System.EventHandler(this.CheckData_Click);
            // 
            // RadioBad
            // 
            this.RadioBad.AutoSize = true;
            this.RadioBad.Enabled = false;
            this.RadioBad.Location = new System.Drawing.Point(153, 19);
            this.RadioBad.Name = "RadioBad";
            this.RadioBad.Size = new System.Drawing.Size(59, 16);
            this.RadioBad.TabIndex = 4;
            this.RadioBad.Text = "不良品";
            this.RadioBad.UseVisualStyleBackColor = true;
            // 
            // RadioFine
            // 
            this.RadioFine.AutoSize = true;
            this.RadioFine.Enabled = false;
            this.RadioFine.Location = new System.Drawing.Point(85, 19);
            this.RadioFine.Name = "RadioFine";
            this.RadioFine.Size = new System.Drawing.Size(47, 16);
            this.RadioFine.TabIndex = 3;
            this.RadioFine.Text = "良品";
            this.RadioFine.UseVisualStyleBackColor = true;
            // 
            // RadioAll
            // 
            this.RadioAll.AutoSize = true;
            this.RadioAll.Checked = true;
            this.RadioAll.Enabled = false;
            this.RadioAll.Location = new System.Drawing.Point(17, 19);
            this.RadioAll.Name = "RadioAll";
            this.RadioAll.Size = new System.Drawing.Size(47, 16);
            this.RadioAll.TabIndex = 2;
            this.RadioAll.TabStop = true;
            this.RadioAll.Text = "全部";
            this.RadioAll.UseVisualStyleBackColor = true;
            // 
            // ExportByzhidan
            // 
            this.ExportByzhidan.Enabled = false;
            this.ExportByzhidan.Location = new System.Drawing.Point(433, 16);
            this.ExportByzhidan.Name = "ExportByzhidan";
            this.ExportByzhidan.Size = new System.Drawing.Size(91, 23);
            this.ExportByzhidan.TabIndex = 1;
            this.ExportByzhidan.Text = "导出";
            this.ExportByzhidan.UseVisualStyleBackColor = true;
            this.ExportByzhidan.Click += new System.EventHandler(this.ExportByzhidan_Click);
            // 
            // dataGridView2
            // 
            this.dataGridView2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.dataGridView2.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.DisplayedCellsExceptHeaders;
            this.dataGridView2.BackgroundColor = System.Drawing.Color.White;
            dataGridViewCellStyle6.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            dataGridViewCellStyle6.BackColor = System.Drawing.SystemColors.Control;
            dataGridViewCellStyle6.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            dataGridViewCellStyle6.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle6.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle6.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle6.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridView2.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle6;
            this.dataGridView2.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            dataGridViewCellStyle7.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            dataGridViewCellStyle7.BackColor = System.Drawing.SystemColors.Window;
            dataGridViewCellStyle7.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            dataGridViewCellStyle7.ForeColor = System.Drawing.SystemColors.ControlText;
            dataGridViewCellStyle7.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle7.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle7.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
            this.dataGridView2.DefaultCellStyle = dataGridViewCellStyle7;
            this.dataGridView2.Location = new System.Drawing.Point(0, 53);
            this.dataGridView2.Name = "dataGridView2";
            this.dataGridView2.ReadOnly = true;
            this.dataGridView2.RowHeadersWidthSizeMode = System.Windows.Forms.DataGridViewRowHeadersWidthSizeMode.DisableResizing;
            this.dataGridView2.RowTemplate.Height = 23;
            this.dataGridView2.Size = new System.Drawing.Size(1123, 604);
            this.dataGridView2.TabIndex = 0;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.SqlConfig);
            this.groupBox3.Controls.Add(this.AutoConnection);
            this.groupBox3.Controls.Add(this.Lock);
            this.groupBox3.Controls.Add(this.OqcUser);
            this.groupBox3.Controls.Add(this.OqcLogout);
            this.groupBox3.Controls.Add(this.OqcLogin);
            this.groupBox3.Controls.Add(this.zhidan);
            this.groupBox3.Controls.Add(this.label1);
            this.groupBox3.Location = new System.Drawing.Point(2, 12);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(1127, 75);
            this.groupBox3.TabIndex = 3;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "初始化";
            // 
            // SqlConfig
            // 
            this.SqlConfig.Location = new System.Drawing.Point(691, 14);
            this.SqlConfig.Name = "SqlConfig";
            this.SqlConfig.Size = new System.Drawing.Size(98, 23);
            this.SqlConfig.TabIndex = 7;
            this.SqlConfig.Text = "数据库配置";
            this.SqlConfig.UseVisualStyleBackColor = true;
            this.SqlConfig.Click += new System.EventHandler(this.SqlConfig_Click);
            // 
            // AutoConnection
            // 
            this.AutoConnection.Enabled = false;
            this.AutoConnection.Location = new System.Drawing.Point(795, 43);
            this.AutoConnection.Name = "AutoConnection";
            this.AutoConnection.Size = new System.Drawing.Size(98, 23);
            this.AutoConnection.TabIndex = 6;
            this.AutoConnection.Text = "一键连接";
            this.AutoConnection.UseVisualStyleBackColor = true;
            this.AutoConnection.Click += new System.EventHandler(this.AutoConnection_Click);
            // 
            // Lock
            // 
            this.Lock.Enabled = false;
            this.Lock.Location = new System.Drawing.Point(795, 14);
            this.Lock.Name = "Lock";
            this.Lock.Size = new System.Drawing.Size(98, 23);
            this.Lock.TabIndex = 5;
            this.Lock.Text = "锁定";
            this.Lock.UseVisualStyleBackColor = true;
            this.Lock.Click += new System.EventHandler(this.Lock_Click);
            // 
            // OqcUser
            // 
            this.OqcUser.Location = new System.Drawing.Point(899, 44);
            this.OqcUser.Name = "OqcUser";
            this.OqcUser.ReadOnly = true;
            this.OqcUser.Size = new System.Drawing.Size(202, 21);
            this.OqcUser.TabIndex = 4;
            // 
            // OqcLogout
            // 
            this.OqcLogout.Enabled = false;
            this.OqcLogout.Location = new System.Drawing.Point(1003, 13);
            this.OqcLogout.Name = "OqcLogout";
            this.OqcLogout.Size = new System.Drawing.Size(98, 23);
            this.OqcLogout.TabIndex = 3;
            this.OqcLogout.Text = "退出登录";
            this.OqcLogout.UseVisualStyleBackColor = true;
            this.OqcLogout.Click += new System.EventHandler(this.OqcLogout_Click);
            // 
            // OqcLogin
            // 
            this.OqcLogin.Location = new System.Drawing.Point(899, 14);
            this.OqcLogin.Name = "OqcLogin";
            this.OqcLogin.Size = new System.Drawing.Size(98, 23);
            this.OqcLogin.TabIndex = 2;
            this.OqcLogin.Text = "登录";
            this.OqcLogin.UseVisualStyleBackColor = true;
            this.OqcLogin.Click += new System.EventHandler(this.OqcLogin_Click);
            // 
            // ReTextCheck
            // 
            this.ReTextCheck.Location = new System.Drawing.Point(333, 16);
            this.ReTextCheck.Name = "ReTextCheck";
            this.ReTextCheck.Size = new System.Drawing.Size(91, 23);
            this.ReTextCheck.TabIndex = 6;
            this.ReTextCheck.Text = "重测查询";
            this.ReTextCheck.UseVisualStyleBackColor = true;
            this.ReTextCheck.Click += new System.EventHandler(this.ReTextCheck_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1136, 802);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.tabControl1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "MES-OQC_1.0.0_201812141640";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox7.ResumeLayout(false);
            this.groupBox7.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.Index.ResumeLayout(false);
            this.Check.ResumeLayout(false);
            this.Check.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView2)).EndInit();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox zhidan;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage Index;
        private System.Windows.Forms.TabPage Check;
        private System.Windows.Forms.GroupBox groupBox3;
        public System.Windows.Forms.TextBox OqcUser;
        private System.Windows.Forms.Button OqcLogout;
        private System.Windows.Forms.Button OqcLogin;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.Button TCUpdate;
        private System.Windows.Forms.Button Lock;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.DataGridViewTextBoxColumn TestName;
        private System.Windows.Forms.DataGridViewTextBoxColumn TestCommand;
        private System.Windows.Forms.DataGridViewTextBoxColumn ReturnValue;
        private System.Windows.Forms.GroupBox groupBox7;
        private System.Windows.Forms.Button Connection4;
        private System.Windows.Forms.ComboBox SerialPort4;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.Button Connection3;
        private System.Windows.Forms.ComboBox SerialPort3;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.Button Connection2;
        private System.Windows.Forms.ComboBox SerialPort2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Button Connection1;
        private System.Windows.Forms.ComboBox SerialPort1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox S1Log;
        private System.Windows.Forms.TextBox S4Log;
        private System.Windows.Forms.TextBox S3Log;
        private System.Windows.Forms.TextBox S2Log;
        private System.Windows.Forms.Button AutoConnection;
        private System.Windows.Forms.Label Remind1;
        private System.Windows.Forms.Label Remind4;
        private System.Windows.Forms.Label Remind3;
        private System.Windows.Forms.Label Remind2;
        private System.Windows.Forms.Button SqlConfig;
        private System.Windows.Forms.DataGridView dataGridView2;
        private System.Windows.Forms.Button ExportByzhidan;
        private System.Windows.Forms.Button CheckData;
        private System.Windows.Forms.RadioButton RadioBad;
        private System.Windows.Forms.RadioButton RadioFine;
        private System.Windows.Forms.RadioButton RadioAll;
        private System.Windows.Forms.Button ReTextCheck;
    }
}

