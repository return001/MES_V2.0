namespace OqcTool
{
    partial class Unlock
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
            this.reminder = new System.Windows.Forms.TextBox();
            this.sure = new System.Windows.Forms.Button();
            this.Password = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // reminder
            // 
            this.reminder.BackColor = System.Drawing.SystemColors.Control;
            this.reminder.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.reminder.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.reminder.ForeColor = System.Drawing.Color.Red;
            this.reminder.Location = new System.Drawing.Point(13, 150);
            this.reminder.Margin = new System.Windows.Forms.Padding(2);
            this.reminder.Multiline = true;
            this.reminder.Name = "reminder";
            this.reminder.Size = new System.Drawing.Size(332, 30);
            this.reminder.TabIndex = 11;
            this.reminder.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // sure
            // 
            this.sure.Location = new System.Drawing.Point(75, 107);
            this.sure.Margin = new System.Windows.Forms.Padding(2);
            this.sure.Name = "sure";
            this.sure.Size = new System.Drawing.Size(190, 24);
            this.sure.TabIndex = 10;
            this.sure.Text = "确定";
            this.sure.UseVisualStyleBackColor = true;
            this.sure.Click += new System.EventHandler(this.sure_Click);
            // 
            // Password
            // 
            this.Password.Location = new System.Drawing.Point(75, 71);
            this.Password.Margin = new System.Windows.Forms.Padding(2);
            this.Password.Name = "Password";
            this.Password.PasswordChar = '*';
            this.Password.Size = new System.Drawing.Size(192, 21);
            this.Password.TabIndex = 9;
            this.Password.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Password_KeyPress);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(78, 41);
            this.label1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(136, 16);
            this.label1.TabIndex = 8;
            this.label1.Text = "请输入解锁密码：";
            // 
            // Unlock
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(358, 221);
            this.Controls.Add(this.reminder);
            this.Controls.Add(this.sure);
            this.Controls.Add(this.Password);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "Unlock";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Unlock";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox reminder;
        private System.Windows.Forms.Button sure;
        private System.Windows.Forms.TextBox Password;
        private System.Windows.Forms.Label label1;
    }
}