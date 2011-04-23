namespace FontBuilder
{
    partial class MainForm
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
            this.textBoxFont = new System.Windows.Forms.TextBox();
            this.buttonSelectFont = new System.Windows.Forms.Button();
            this.buttonBrowseOutputPathname = new System.Windows.Forms.Button();
            this.textBoxOutputPathname = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.buttonBuild = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.pictureBoxColour = new System.Windows.Forms.PictureBox();
            this.buttonPickColour = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxColour)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 18);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(31, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Font:";
            // 
            // textBoxFont
            // 
            this.textBoxFont.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxFont.Location = new System.Drawing.Point(110, 15);
            this.textBoxFont.Name = "textBoxFont";
            this.textBoxFont.ReadOnly = true;
            this.textBoxFont.Size = new System.Drawing.Size(418, 20);
            this.textBoxFont.TabIndex = 1;
            this.textBoxFont.TabStop = false;
            // 
            // buttonSelectFont
            // 
            this.buttonSelectFont.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonSelectFont.Location = new System.Drawing.Point(534, 13);
            this.buttonSelectFont.Name = "buttonSelectFont";
            this.buttonSelectFont.Size = new System.Drawing.Size(75, 23);
            this.buttonSelectFont.TabIndex = 0;
            this.buttonSelectFont.Text = "Select...";
            this.buttonSelectFont.UseVisualStyleBackColor = true;
            this.buttonSelectFont.Click += new System.EventHandler(this.OnSelectFont);
            // 
            // buttonBrowseOutputPathname
            // 
            this.buttonBrowseOutputPathname.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonBrowseOutputPathname.Location = new System.Drawing.Point(534, 42);
            this.buttonBrowseOutputPathname.Name = "buttonBrowseOutputPathname";
            this.buttonBrowseOutputPathname.Size = new System.Drawing.Size(75, 23);
            this.buttonBrowseOutputPathname.TabIndex = 2;
            this.buttonBrowseOutputPathname.Text = "Browse...";
            this.buttonBrowseOutputPathname.UseVisualStyleBackColor = true;
            this.buttonBrowseOutputPathname.Click += new System.EventHandler(this.OnBrowseOutputPathname);
            // 
            // textBoxOutputPathname
            // 
            this.textBoxOutputPathname.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxOutputPathname.Location = new System.Drawing.Point(110, 44);
            this.textBoxOutputPathname.Name = "textBoxOutputPathname";
            this.textBoxOutputPathname.Size = new System.Drawing.Size(418, 20);
            this.textBoxOutputPathname.TabIndex = 1;
            this.textBoxOutputPathname.TextChanged += new System.EventHandler(this.OnOutputPathnameChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 47);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(92, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Output pathname:";
            // 
            // buttonBuild
            // 
            this.buttonBuild.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonBuild.Location = new System.Drawing.Point(534, 118);
            this.buttonBuild.Name = "buttonBuild";
            this.buttonBuild.Size = new System.Drawing.Size(75, 23);
            this.buttonBuild.TabIndex = 4;
            this.buttonBuild.Text = "Build";
            this.buttonBuild.UseVisualStyleBackColor = true;
            this.buttonBuild.Click += new System.EventHandler(this.OnBuild);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 76);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(63, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "Text colour:";
            // 
            // pictureBoxColour
            // 
            this.pictureBoxColour.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.pictureBoxColour.Location = new System.Drawing.Point(110, 70);
            this.pictureBoxColour.Name = "pictureBoxColour";
            this.pictureBoxColour.Size = new System.Drawing.Size(418, 40);
            this.pictureBoxColour.TabIndex = 8;
            this.pictureBoxColour.TabStop = false;
            // 
            // buttonPickColour
            // 
            this.buttonPickColour.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonPickColour.Location = new System.Drawing.Point(534, 71);
            this.buttonPickColour.Name = "buttonPickColour";
            this.buttonPickColour.Size = new System.Drawing.Size(75, 23);
            this.buttonPickColour.TabIndex = 3;
            this.buttonPickColour.Text = "Pick...";
            this.buttonPickColour.UseVisualStyleBackColor = true;
            this.buttonPickColour.Click += new System.EventHandler(this.OnPickColour);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(619, 152);
            this.Controls.Add(this.buttonPickColour);
            this.Controls.Add(this.pictureBoxColour);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.buttonBuild);
            this.Controls.Add(this.buttonBrowseOutputPathname);
            this.Controls.Add(this.textBoxOutputPathname);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.buttonSelectFont);
            this.Controls.Add(this.textBoxFont);
            this.Controls.Add(this.label1);
            this.MaximizeBox = false;
            this.MaximumSize = new System.Drawing.Size(1035, 190);
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(635, 190);
            this.Name = "MainForm";
            this.ShowIcon = false;
            this.Text = "Rorn Font Builder";
            this.Load += new System.EventHandler(this.OnLoadForm);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxColour)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxFont;
        private System.Windows.Forms.Button buttonSelectFont;
        private System.Windows.Forms.Button buttonBrowseOutputPathname;
        private System.Windows.Forms.TextBox textBoxOutputPathname;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button buttonBuild;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.PictureBox pictureBoxColour;
        private System.Windows.Forms.Button buttonPickColour;
    }
}

