namespace OpenIot.BoardSetup
{
    partial class MainForm
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            pbBackground = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)pbBackground).BeginInit();
            SuspendLayout();
            // 
            // pbBackground
            // 
            pbBackground.BackgroundImage = (System.Drawing.Image)resources.GetObject("pbBackground.BackgroundImage");
            pbBackground.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            pbBackground.Dock = System.Windows.Forms.DockStyle.Fill;
            pbBackground.ErrorImage = null;
            pbBackground.Location = new System.Drawing.Point(0, 0);
            pbBackground.Name = "pbBackground";
            pbBackground.Size = new System.Drawing.Size(960, 1080);
            pbBackground.TabIndex = 5;
            pbBackground.TabStop = false;
            // 
            // MainForm
            // 
            AutoScaleDimensions = new System.Drawing.SizeF(13F, 32F);
            AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            ClientSize = new System.Drawing.Size(960, 1080);
            Controls.Add(pbBackground);
            FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            Icon = (System.Drawing.Icon)resources.GetObject("$this.Icon");
            Name = "MainForm";
            StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            Text = "OpenIoT Flash Programmer";
            FormClosing += MainForm_FormClosing;
            ((System.ComponentModel.ISupportInitialize)pbBackground).EndInit();
            ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.PictureBox pbBackground;
    }
}
