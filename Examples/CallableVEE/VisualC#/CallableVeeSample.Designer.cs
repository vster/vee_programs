namespace CallableVeeSample
{
    partial class CallableVeeSample
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
            this.debugModeCheckBox = new System.Windows.Forms.CheckBox();
            this.delayValuelabel = new System.Windows.Forms.Label();
            this.delayValyeTextBox = new System.Windows.Forms.TextBox();
            this.executeTestButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // debugModeCheckBox
            // 
            this.debugModeCheckBox.AutoSize = true;
            this.debugModeCheckBox.Location = new System.Drawing.Point(12, 12);
            this.debugModeCheckBox.Name = "debugModeCheckBox";
            this.debugModeCheckBox.Size = new System.Drawing.Size(88, 17);
            this.debugModeCheckBox.TabIndex = 0;
            this.debugModeCheckBox.Text = "Debug Mode";
            this.debugModeCheckBox.UseVisualStyleBackColor = true;
            // 
            // delayValuelabel
            // 
            this.delayValuelabel.AutoSize = true;
            this.delayValuelabel.Location = new System.Drawing.Point(12, 32);
            this.delayValuelabel.Name = "delayValuelabel";
            this.delayValuelabel.Size = new System.Drawing.Size(142, 13);
            this.delayValuelabel.TabIndex = 1;
            this.delayValuelabel.Text = "Delay Value (Microseconds):";
            // 
            // delayValyeTextBox
            // 
            this.delayValyeTextBox.Location = new System.Drawing.Point(162, 29);
            this.delayValyeTextBox.Name = "delayValyeTextBox";
            this.delayValyeTextBox.Size = new System.Drawing.Size(61, 20);
            this.delayValyeTextBox.TabIndex = 2;
            this.delayValyeTextBox.Text = ".35";
            // 
            // executeTestButton
            // 
            this.executeTestButton.Location = new System.Drawing.Point(15, 55);
            this.executeTestButton.Name = "executeTestButton";
            this.executeTestButton.Size = new System.Drawing.Size(208, 23);
            this.executeTestButton.TabIndex = 3;
            this.executeTestButton.Text = "Execute Test";
            this.executeTestButton.UseVisualStyleBackColor = true;
            this.executeTestButton.Click += new System.EventHandler(this.executeTestButton_Click);
            // 
            // CallableVeeSample
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(233, 83);
            this.Controls.Add(this.executeTestButton);
            this.Controls.Add(this.delayValyeTextBox);
            this.Controls.Add(this.delayValuelabel);
            this.Controls.Add(this.debugModeCheckBox);
            this.Name = "CallableVeeSample";
            this.Text = "Callable VEE Sample";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox debugModeCheckBox;
        private System.Windows.Forms.Label delayValuelabel;
        private System.Windows.Forms.TextBox delayValyeTextBox;
        private System.Windows.Forms.Button executeTestButton;
    }
}

