namespace NetLogoTool
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
            this.label1 = new System.Windows.Forms.Label();
            this.Zhidan_comboBox = new System.Windows.Forms.ComboBox();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.PresentImei = new System.Windows.Forms.TextBox();
            this.OutputByImei = new System.Windows.Forms.Button();
            this.NetLogoDigits = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.Logout = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.OutputTxt = new System.Windows.Forms.Button();
            this.Tolock = new System.Windows.Forms.Button();
            this.Login = new System.Windows.Forms.Button();
            this.NetLogoSuffix = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.IMEI_End = new System.Windows.Forms.TextBox();
            this.NetLogoPrefix = new System.Windows.Forms.TextBox();
            this.IMEI_Start = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.CheckResult = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.Tocheck = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.NetUser = new System.Windows.Forms.TextBox();
            this.DeleteNetLogo = new System.Windows.Forms.Button();
            this.ImeiRangeDel = new System.Windows.Forms.Button();
            this.NetLogoOrImei = new System.Windows.Forms.TextBox();
            this.IMEI_num2 = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.IMEI_num1 = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.UserType = new System.Windows.Forms.Label();
            this.StaticImei = new System.Windows.Forms.TextBox();
            this.OnlyNetMark = new System.Windows.Forms.TextBox();
            this.NetLogoInput = new System.Windows.Forms.TextBox();
            this.L_down2 = new System.Windows.Forms.Label();
            this.InputAll = new System.Windows.Forms.CheckBox();
            this.ImeiInput = new System.Windows.Forms.TextBox();
            this.Remind = new System.Windows.Forms.TextBox();
            this.NoCheck = new System.Windows.Forms.CheckBox();
            this.L_down1 = new System.Windows.Forms.Label();
            this.L_up1 = new System.Windows.Forms.Label();
            this.L_up2 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(8, 11);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(91, 14);
            this.label1.TabIndex = 0;
            this.label1.Text = "选择订单号：";
            // 
            // Zhidan_comboBox
            // 
            this.Zhidan_comboBox.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
            this.Zhidan_comboBox.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
            this.Zhidan_comboBox.Enabled = false;
            this.Zhidan_comboBox.FormattingEnabled = true;
            this.Zhidan_comboBox.Location = new System.Drawing.Point(11, 31);
            this.Zhidan_comboBox.Name = "Zhidan_comboBox";
            this.Zhidan_comboBox.Size = new System.Drawing.Size(192, 20);
            this.Zhidan_comboBox.TabIndex = 1;
            this.Zhidan_comboBox.SelectedIndexChanged += new System.EventHandler(this.Zhidan_comboBox_SelectedIndexChanged);
            // 
            // splitContainer1
            // 
            this.splitContainer1.Location = new System.Drawing.Point(2, 0);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.BackColor = System.Drawing.Color.Linen;
            this.splitContainer1.Panel1.Controls.Add(this.PresentImei);
            this.splitContainer1.Panel1.Controls.Add(this.OutputByImei);
            this.splitContainer1.Panel1.Controls.Add(this.NetLogoDigits);
            this.splitContainer1.Panel1.Controls.Add(this.label6);
            this.splitContainer1.Panel1.Controls.Add(this.label3);
            this.splitContainer1.Panel1.Controls.Add(this.Logout);
            this.splitContainer1.Panel1.Controls.Add(this.label2);
            this.splitContainer1.Panel1.Controls.Add(this.OutputTxt);
            this.splitContainer1.Panel1.Controls.Add(this.Tolock);
            this.splitContainer1.Panel1.Controls.Add(this.Login);
            this.splitContainer1.Panel1.Controls.Add(this.NetLogoSuffix);
            this.splitContainer1.Panel1.Controls.Add(this.label8);
            this.splitContainer1.Panel1.Controls.Add(this.IMEI_End);
            this.splitContainer1.Panel1.Controls.Add(this.NetLogoPrefix);
            this.splitContainer1.Panel1.Controls.Add(this.IMEI_Start);
            this.splitContainer1.Panel1.Controls.Add(this.Zhidan_comboBox);
            this.splitContainer1.Panel1.Controls.Add(this.label5);
            this.splitContainer1.Panel1.Controls.Add(this.label1);
            this.splitContainer1.Panel1.Controls.Add(this.label4);
            this.splitContainer1.Panel1.RightToLeft = System.Windows.Forms.RightToLeft.No;
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.BackColor = System.Drawing.Color.White;
            this.splitContainer1.Panel2.Controls.Add(this.splitContainer2);
            this.splitContainer1.Panel2.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.splitContainer1.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.splitContainer1.Size = new System.Drawing.Size(1098, 588);
            this.splitContainer1.SplitterDistance = 124;
            this.splitContainer1.TabIndex = 2;
            // 
            // PresentImei
            // 
            this.PresentImei.Location = new System.Drawing.Point(666, 30);
            this.PresentImei.Name = "PresentImei";
            this.PresentImei.ReadOnly = true;
            this.PresentImei.Size = new System.Drawing.Size(192, 21);
            this.PresentImei.TabIndex = 22;
            // 
            // OutputByImei
            // 
            this.OutputByImei.Enabled = false;
            this.OutputByImei.Location = new System.Drawing.Point(930, 84);
            this.OutputByImei.Name = "OutputByImei";
            this.OutputByImei.Size = new System.Drawing.Size(157, 23);
            this.OutputByImei.TabIndex = 21;
            this.OutputByImei.Text = "根据IMEI范围导出TXT文件";
            this.OutputByImei.UseVisualStyleBackColor = true;
            this.OutputByImei.Click += new System.EventHandler(this.OutputByImei_Click);
            // 
            // NetLogoDigits
            // 
            this.NetLogoDigits.Location = new System.Drawing.Point(444, 86);
            this.NetLogoDigits.Name = "NetLogoDigits";
            this.NetLogoDigits.ReadOnly = true;
            this.NetLogoDigits.Size = new System.Drawing.Size(192, 21);
            this.NetLogoDigits.TabIndex = 7;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label6.Location = new System.Drawing.Point(663, 11);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(84, 14);
            this.label6.TabIndex = 4;
            this.label6.Text = "IMEI当前位:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label3.Location = new System.Drawing.Point(441, 11);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(84, 14);
            this.label3.TabIndex = 4;
            this.label3.Text = "IMEI终止位:";
            // 
            // Logout
            // 
            this.Logout.Enabled = false;
            this.Logout.Location = new System.Drawing.Point(992, 29);
            this.Logout.Name = "Logout";
            this.Logout.Size = new System.Drawing.Size(95, 23);
            this.Logout.TabIndex = 11;
            this.Logout.Text = "退出登录";
            this.Logout.UseVisualStyleBackColor = true;
            this.Logout.Click += new System.EventHandler(this.Logout_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2.Location = new System.Drawing.Point(224, 11);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(84, 14);
            this.label2.TabIndex = 4;
            this.label2.Text = "IMEI起始位:";
            // 
            // OutputTxt
            // 
            this.OutputTxt.Enabled = false;
            this.OutputTxt.Location = new System.Drawing.Point(767, 84);
            this.OutputTxt.Name = "OutputTxt";
            this.OutputTxt.Size = new System.Drawing.Size(157, 23);
            this.OutputTxt.TabIndex = 8;
            this.OutputTxt.Text = "根据制单号导出TXT文件";
            this.OutputTxt.UseVisualStyleBackColor = true;
            this.OutputTxt.Click += new System.EventHandler(this.InputTxt_Click);
            // 
            // Tolock
            // 
            this.Tolock.Enabled = false;
            this.Tolock.Location = new System.Drawing.Point(666, 84);
            this.Tolock.Name = "Tolock";
            this.Tolock.Size = new System.Drawing.Size(95, 23);
            this.Tolock.TabIndex = 20;
            this.Tolock.Text = "锁定";
            this.Tolock.UseVisualStyleBackColor = true;
            this.Tolock.Click += new System.EventHandler(this.Tolock_Click);
            // 
            // Login
            // 
            this.Login.Location = new System.Drawing.Point(876, 29);
            this.Login.Name = "Login";
            this.Login.Size = new System.Drawing.Size(95, 23);
            this.Login.TabIndex = 10;
            this.Login.Text = "登陆";
            this.Login.UseVisualStyleBackColor = true;
            this.Login.Click += new System.EventHandler(this.Login_Click);
            // 
            // NetLogoSuffix
            // 
            this.NetLogoSuffix.Location = new System.Drawing.Point(227, 86);
            this.NetLogoSuffix.Name = "NetLogoSuffix";
            this.NetLogoSuffix.ReadOnly = true;
            this.NetLogoSuffix.Size = new System.Drawing.Size(192, 21);
            this.NetLogoSuffix.TabIndex = 6;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label8.Location = new System.Drawing.Point(441, 68);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(70, 14);
            this.label8.TabIndex = 4;
            this.label8.Text = "网标位数:";
            // 
            // IMEI_End
            // 
            this.IMEI_End.Location = new System.Drawing.Point(444, 30);
            this.IMEI_End.Name = "IMEI_End";
            this.IMEI_End.ReadOnly = true;
            this.IMEI_End.Size = new System.Drawing.Size(192, 21);
            this.IMEI_End.TabIndex = 3;
            // 
            // NetLogoPrefix
            // 
            this.NetLogoPrefix.Location = new System.Drawing.Point(13, 86);
            this.NetLogoPrefix.Name = "NetLogoPrefix";
            this.NetLogoPrefix.ReadOnly = true;
            this.NetLogoPrefix.Size = new System.Drawing.Size(192, 21);
            this.NetLogoPrefix.TabIndex = 5;
            // 
            // IMEI_Start
            // 
            this.IMEI_Start.Location = new System.Drawing.Point(227, 30);
            this.IMEI_Start.Name = "IMEI_Start";
            this.IMEI_Start.ReadOnly = true;
            this.IMEI_Start.Size = new System.Drawing.Size(192, 21);
            this.IMEI_Start.TabIndex = 2;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label5.Location = new System.Drawing.Point(224, 68);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(70, 14);
            this.label5.TabIndex = 4;
            this.label5.Text = "网标后缀:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label4.Location = new System.Drawing.Point(10, 68);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(70, 14);
            this.label4.TabIndex = 4;
            this.label4.Text = "网标前缀:";
            // 
            // splitContainer2
            // 
            this.splitContainer2.Location = new System.Drawing.Point(0, 3);
            this.splitContainer2.Name = "splitContainer2";
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.BackColor = System.Drawing.Color.Linen;
            this.splitContainer2.Panel1.Controls.Add(this.CheckResult);
            this.splitContainer2.Panel1.Controls.Add(this.label11);
            this.splitContainer2.Panel1.Controls.Add(this.Tocheck);
            this.splitContainer2.Panel1.Controls.Add(this.label7);
            this.splitContainer2.Panel1.Controls.Add(this.NetUser);
            this.splitContainer2.Panel1.Controls.Add(this.DeleteNetLogo);
            this.splitContainer2.Panel1.Controls.Add(this.ImeiRangeDel);
            this.splitContainer2.Panel1.Controls.Add(this.NetLogoOrImei);
            this.splitContainer2.Panel1.Controls.Add(this.IMEI_num2);
            this.splitContainer2.Panel1.Controls.Add(this.label9);
            this.splitContainer2.Panel1.Controls.Add(this.IMEI_num1);
            this.splitContainer2.Panel1.Controls.Add(this.label10);
            this.splitContainer2.Panel1.Controls.Add(this.UserType);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.StaticImei);
            this.splitContainer2.Panel2.Controls.Add(this.OnlyNetMark);
            this.splitContainer2.Panel2.Controls.Add(this.NetLogoInput);
            this.splitContainer2.Panel2.Controls.Add(this.L_down2);
            this.splitContainer2.Panel2.Controls.Add(this.InputAll);
            this.splitContainer2.Panel2.Controls.Add(this.ImeiInput);
            this.splitContainer2.Panel2.Controls.Add(this.Remind);
            this.splitContainer2.Panel2.Controls.Add(this.NoCheck);
            this.splitContainer2.Panel2.Controls.Add(this.L_down1);
            this.splitContainer2.Panel2.Controls.Add(this.L_up1);
            this.splitContainer2.Panel2.Controls.Add(this.L_up2);
            this.splitContainer2.Size = new System.Drawing.Size(1095, 457);
            this.splitContainer2.SplitterDistance = 264;
            this.splitContainer2.TabIndex = 21;
            // 
            // CheckResult
            // 
            this.CheckResult.Location = new System.Drawing.Point(13, 369);
            this.CheckResult.Name = "CheckResult";
            this.CheckResult.ReadOnly = true;
            this.CheckResult.Size = new System.Drawing.Size(195, 21);
            this.CheckResult.TabIndex = 24;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(12, 354);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(107, 12);
            this.label11.TabIndex = 23;
            this.label11.Text = "对应的网标/IMEI：";
            // 
            // Tocheck
            // 
            this.Tocheck.Location = new System.Drawing.Point(13, 323);
            this.Tocheck.Name = "Tocheck";
            this.Tocheck.ReadOnly = true;
            this.Tocheck.Size = new System.Drawing.Size(195, 21);
            this.Tocheck.TabIndex = 22;
            this.Tocheck.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Tocheck_KeyPress);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(12, 304);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(233, 12);
            this.label7.TabIndex = 21;
            this.label7.Text = "请输入查询的IMEI/网标（按下Enter键）：";
            // 
            // NetUser
            // 
            this.NetUser.Location = new System.Drawing.Point(83, 423);
            this.NetUser.Name = "NetUser";
            this.NetUser.ReadOnly = true;
            this.NetUser.Size = new System.Drawing.Size(158, 21);
            this.NetUser.TabIndex = 13;
            // 
            // DeleteNetLogo
            // 
            this.DeleteNetLogo.Enabled = false;
            this.DeleteNetLogo.Location = new System.Drawing.Point(11, 80);
            this.DeleteNetLogo.Name = "DeleteNetLogo";
            this.DeleteNetLogo.Size = new System.Drawing.Size(197, 23);
            this.DeleteNetLogo.TabIndex = 15;
            this.DeleteNetLogo.Text = "删除";
            this.DeleteNetLogo.UseVisualStyleBackColor = true;
            this.DeleteNetLogo.Click += new System.EventHandler(this.DeleteNetLogo_Click);
            // 
            // ImeiRangeDel
            // 
            this.ImeiRangeDel.Enabled = false;
            this.ImeiRangeDel.Location = new System.Drawing.Point(13, 243);
            this.ImeiRangeDel.Name = "ImeiRangeDel";
            this.ImeiRangeDel.Size = new System.Drawing.Size(195, 23);
            this.ImeiRangeDel.TabIndex = 20;
            this.ImeiRangeDel.Text = "删除";
            this.ImeiRangeDel.UseVisualStyleBackColor = true;
            this.ImeiRangeDel.Click += new System.EventHandler(this.ImeiRangeDel_Click);
            // 
            // NetLogoOrImei
            // 
            this.NetLogoOrImei.Location = new System.Drawing.Point(11, 44);
            this.NetLogoOrImei.Name = "NetLogoOrImei";
            this.NetLogoOrImei.ReadOnly = true;
            this.NetLogoOrImei.Size = new System.Drawing.Size(197, 21);
            this.NetLogoOrImei.TabIndex = 14;
            // 
            // IMEI_num2
            // 
            this.IMEI_num2.Location = new System.Drawing.Point(13, 206);
            this.IMEI_num2.Name = "IMEI_num2";
            this.IMEI_num2.ReadOnly = true;
            this.IMEI_num2.Size = new System.Drawing.Size(195, 21);
            this.IMEI_num2.TabIndex = 19;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(11, 20);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(101, 12);
            this.label9.TabIndex = 16;
            this.label9.Text = "请输入IMEI或网标";
            // 
            // IMEI_num1
            // 
            this.IMEI_num1.Location = new System.Drawing.Point(13, 171);
            this.IMEI_num1.Name = "IMEI_num1";
            this.IMEI_num1.ReadOnly = true;
            this.IMEI_num1.Size = new System.Drawing.Size(195, 21);
            this.IMEI_num1.TabIndex = 18;
            this.IMEI_num1.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.IMEI_num1_KeyPress);
            this.IMEI_num1.Leave += new System.EventHandler(this.IMEI_num1_Leave);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(11, 147);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(125, 12);
            this.label10.TabIndex = 17;
            this.label10.Text = "请输入删除的IMEI范围";
            // 
            // UserType
            // 
            this.UserType.AutoSize = true;
            this.UserType.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.UserType.Location = new System.Drawing.Point(7, 426);
            this.UserType.Name = "UserType";
            this.UserType.Size = new System.Drawing.Size(49, 14);
            this.UserType.TabIndex = 12;
            this.UserType.Text = "用户：";
            // 
            // StaticImei
            // 
            this.StaticImei.Location = new System.Drawing.Point(347, 137);
            this.StaticImei.Name = "StaticImei";
            this.StaticImei.ReadOnly = true;
            this.StaticImei.Size = new System.Drawing.Size(189, 21);
            this.StaticImei.TabIndex = 18;
            // 
            // OnlyNetMark
            // 
            this.OnlyNetMark.Location = new System.Drawing.Point(347, 82);
            this.OnlyNetMark.Name = "OnlyNetMark";
            this.OnlyNetMark.ReadOnly = true;
            this.OnlyNetMark.Size = new System.Drawing.Size(189, 21);
            this.OnlyNetMark.TabIndex = 17;
            this.OnlyNetMark.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.OnlyNetMark_KeyPress);
            // 
            // NetLogoInput
            // 
            this.NetLogoInput.Location = new System.Drawing.Point(347, 137);
            this.NetLogoInput.Name = "NetLogoInput";
            this.NetLogoInput.ReadOnly = true;
            this.NetLogoInput.Size = new System.Drawing.Size(189, 21);
            this.NetLogoInput.TabIndex = 1;
            this.NetLogoInput.Visible = false;
            this.NetLogoInput.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.NetLogoInput_KeyPress);
            // 
            // L_down2
            // 
            this.L_down2.AutoSize = true;
            this.L_down2.Font = new System.Drawing.Font("宋体", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.L_down2.Location = new System.Drawing.Point(221, 137);
            this.L_down2.Name = "L_down2";
            this.L_down2.Size = new System.Drawing.Size(129, 20);
            this.L_down2.TabIndex = 2;
            this.L_down2.Text = "对应IMEI号：";
            // 
            // InputAll
            // 
            this.InputAll.AutoSize = true;
            this.InputAll.Enabled = false;
            this.InputAll.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.InputAll.Location = new System.Drawing.Point(419, 43);
            this.InputAll.Name = "InputAll";
            this.InputAll.Size = new System.Drawing.Size(110, 18);
            this.InputAll.TabIndex = 15;
            this.InputAll.Text = "扫IMEI和网标";
            this.InputAll.UseVisualStyleBackColor = true;
            this.InputAll.Click += new System.EventHandler(this.InputAll_Click);
            // 
            // ImeiInput
            // 
            this.ImeiInput.Location = new System.Drawing.Point(347, 82);
            this.ImeiInput.Name = "ImeiInput";
            this.ImeiInput.ReadOnly = true;
            this.ImeiInput.Size = new System.Drawing.Size(189, 21);
            this.ImeiInput.TabIndex = 0;
            this.ImeiInput.Visible = false;
            this.ImeiInput.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ImeiInput_KeyPress);
            // 
            // Remind
            // 
            this.Remind.BackColor = System.Drawing.Color.White;
            this.Remind.Font = new System.Drawing.Font("宋体", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Remind.ForeColor = System.Drawing.Color.Red;
            this.Remind.Location = new System.Drawing.Point(86, 186);
            this.Remind.Multiline = true;
            this.Remind.Name = "Remind";
            this.Remind.ReadOnly = true;
            this.Remind.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.Remind.Size = new System.Drawing.Size(643, 220);
            this.Remind.TabIndex = 3;
            // 
            // NoCheck
            // 
            this.NoCheck.AutoSize = true;
            this.NoCheck.Enabled = false;
            this.NoCheck.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.NoCheck.Location = new System.Drawing.Point(245, 42);
            this.NoCheck.Name = "NoCheck";
            this.NoCheck.Size = new System.Drawing.Size(138, 18);
            this.NoCheck.TabIndex = 14;
            this.NoCheck.Text = "不检查IMEI校验位";
            this.NoCheck.UseVisualStyleBackColor = true;
            this.NoCheck.Click += new System.EventHandler(this.NoCheck_Click);
            // 
            // L_down1
            // 
            this.L_down1.AutoSize = true;
            this.L_down1.Font = new System.Drawing.Font("宋体", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.L_down1.Location = new System.Drawing.Point(221, 137);
            this.L_down1.Name = "L_down1";
            this.L_down1.Size = new System.Drawing.Size(129, 20);
            this.L_down1.TabIndex = 2;
            this.L_down1.Text = "请扫入网标：";
            this.L_down1.Visible = false;
            // 
            // L_up1
            // 
            this.L_up1.AutoSize = true;
            this.L_up1.Font = new System.Drawing.Font("宋体", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.L_up1.Location = new System.Drawing.Point(221, 82);
            this.L_up1.Name = "L_up1";
            this.L_up1.Size = new System.Drawing.Size(129, 20);
            this.L_up1.TabIndex = 2;
            this.L_up1.Text = "请扫入IMEI：";
            this.L_up1.Visible = false;
            // 
            // L_up2
            // 
            this.L_up2.AutoSize = true;
            this.L_up2.Font = new System.Drawing.Font("宋体", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.L_up2.Location = new System.Drawing.Point(179, 82);
            this.L_up2.Name = "L_up2";
            this.L_up2.Size = new System.Drawing.Size(169, 20);
            this.L_up2.TabIndex = 16;
            this.L_up2.Text = "入网许可证编号：";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1101, 590);
            this.Controls.Add(this.splitContainer1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "MES-NetMark_1.1.1_201810311550";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel1.PerformLayout();
            this.splitContainer2.Panel2.ResumeLayout(false);
            this.splitContainer2.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox Zhidan_comboBox;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.TextBox NetLogoSuffix;
        private System.Windows.Forms.TextBox NetLogoPrefix;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox IMEI_End;
        private System.Windows.Forms.TextBox IMEI_Start;
        private System.Windows.Forms.Button OutputTxt;
        private System.Windows.Forms.Button Tolock;
        private System.Windows.Forms.TextBox Remind;
        private System.Windows.Forms.TextBox NetLogoInput;
        private System.Windows.Forms.TextBox ImeiInput;
        private System.Windows.Forms.Label L_down1;
        private System.Windows.Forms.Label L_up1;
        private System.Windows.Forms.TextBox NetLogoDigits;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button Logout;
        private System.Windows.Forms.Button Login;
        public System.Windows.Forms.TextBox NetUser;
        public System.Windows.Forms.Label UserType;
        private System.Windows.Forms.TextBox NetLogoOrImei;
        private System.Windows.Forms.Button DeleteNetLogo;
        private System.Windows.Forms.CheckBox NoCheck;
        private System.Windows.Forms.Button OutputByImei;
        private System.Windows.Forms.Button ImeiRangeDel;
        private System.Windows.Forms.TextBox IMEI_num2;
        private System.Windows.Forms.TextBox IMEI_num1;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.CheckBox InputAll;
        private System.Windows.Forms.Label L_up2;
        private System.Windows.Forms.Label L_down2;
        private System.Windows.Forms.TextBox StaticImei;
        private System.Windows.Forms.TextBox OnlyNetMark;
        private System.Windows.Forms.TextBox PresentImei;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox CheckResult;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TextBox Tocheck;
        private System.Windows.Forms.Label label7;
    }
}

