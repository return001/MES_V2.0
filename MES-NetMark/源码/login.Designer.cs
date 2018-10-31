namespace NetLogoTool
{
    partial class login
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.NetLogoUese = new System.Windows.Forms.TextBox();
            this.NetLogoPassword = new System.Windows.Forms.TextBox();
            this.SureLog = new System.Windows.Forms.Button();
            this.Cancel = new System.Windows.Forms.Button();
            this.LRemind = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(73, 57);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "用户名：";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(85, 92);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(41, 12);
            this.label2.TabIndex = 0;
            this.label2.Text = "密码：";
            // 
            // NetLogoUese
            // 
            this.NetLogoUese.Location = new System.Drawing.Point(132, 52);
            this.NetLogoUese.Name = "NetLogoUese";
            this.NetLogoUese.Size = new System.Drawing.Size(154, 21);
            this.NetLogoUese.TabIndex = 1;
            this.NetLogoUese.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.NetLogoUese_KeyPress);
            // 
            // NetLogoPassword
            // 
            this.NetLogoPassword.Location = new System.Drawing.Point(132, 87);
            this.NetLogoPassword.Name = "NetLogoPassword";
            this.NetLogoPassword.PasswordChar = '*';
            this.NetLogoPassword.Size = new System.Drawing.Size(154, 21);
            this.NetLogoPassword.TabIndex = 2;
            this.NetLogoPassword.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.NetLogoPassword_KeyPress);
            // 
            // SureLog
            // 
            this.SureLog.Location = new System.Drawing.Point(75, 141);
            this.SureLog.Name = "SureLog";
            this.SureLog.Size = new System.Drawing.Size(95, 23);
            this.SureLog.TabIndex = 3;
            this.SureLog.Text = "确定";
            this.SureLog.UseVisualStyleBackColor = true;
            this.SureLog.Click += new System.EventHandler(this.SureLog_Click);
            // 
            // Cancel
            // 
            this.Cancel.Location = new System.Drawing.Point(191, 141);
            this.Cancel.Name = "Cancel";
            this.Cancel.Size = new System.Drawing.Size(95, 23);
            this.Cancel.TabIndex = 4;
            this.Cancel.Text = "取消";
            this.Cancel.UseVisualStyleBackColor = true;
            this.Cancel.Click += new System.EventHandler(this.Cancel_Click);
            // 
            // LRemind
            // 
            this.LRemind.BackColor = System.Drawing.SystemColors.Menu;
            this.LRemind.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.LRemind.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.LRemind.ForeColor = System.Drawing.Color.Red;
            this.LRemind.Location = new System.Drawing.Point(75, 187);
            this.LRemind.Multiline = true;
            this.LRemind.Name = "LRemind";
            this.LRemind.Size = new System.Drawing.Size(211, 67);
            this.LRemind.TabIndex = 5;
            // 
            // login
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(379, 266);
            this.Controls.Add(this.LRemind);
            this.Controls.Add(this.Cancel);
            this.Controls.Add(this.SureLog);
            this.Controls.Add(this.NetLogoPassword);
            this.Controls.Add(this.NetLogoUese);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "login";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "登录";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox NetLogoUese;
        private System.Windows.Forms.TextBox NetLogoPassword;
        private System.Windows.Forms.Button SureLog;
        private System.Windows.Forms.Button Cancel;
        private System.Windows.Forms.TextBox LRemind;
    }
}