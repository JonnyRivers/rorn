using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace FontBuilder
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private void OnLoadForm(object sender, EventArgs e)
        {
            SelectedFont = new Font("Arial", 10, FontStyle.Regular);
            SelectedOutputDirectory = Application.StartupPath;
            SelectedColor = Color.White;
        }

        private void OnSelectFont(object sender, EventArgs e)
        {
            using (FontDialog fontDialog = new FontDialog())
            {
                fontDialog.Font = SelectedFont;
                if (fontDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                {
                    SelectedFont = fontDialog.Font;
                }
            }
        }

        private void OnBrowseOutputDirectory(object sender, EventArgs e)
        {
            string selectedDirectory;
            if (OpenBrowseForDirectory(SelectedOutputDirectory, "Open Directory", "all files (*.*)|*.*", out selectedDirectory))
            {
                SelectedOutputDirectory = selectedDirectory;
            }
        }

        private void OnPickColour(object sender, EventArgs e)
        {
            using (ColorDialog colorDialog = new ColorDialog())
            {
                colorDialog.Color = SelectedColor;
                if (colorDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                {
                    SelectedColor = colorDialog.Color;
                }
            }
        }

        private void OnBuild(object sender, EventArgs e)
        {
            try
            {
                FontBuilder fontBuilder = new FontBuilder();
                fontBuilder.Build(SelectedOutputDirectory, SelectedFont, SelectedColor);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString(), "Unexpected error while trying to build font");
            }
        }

        private static bool OpenBrowseForDirectory(string initialDir, string description, string filter, out string selectedDirectory)
        {
            using (FolderBrowserDialog openFolderDlg = new FolderBrowserDialog())
            {
                openFolderDlg.SelectedPath = initialDir;
                openFolderDlg.ShowNewFolderButton = false;
                openFolderDlg.Description = description;

                DialogResult result = openFolderDlg.ShowDialog();

                if (result == DialogResult.Cancel)
                {
                    selectedDirectory = "";
                    return false;
                }

                selectedDirectory = openFolderDlg.SelectedPath;
            }

            return true;
        }

        private Font SelectedFont
        {
            get { return selectedFont_; }
            set
            {
                selectedFont_ = value;
                textBoxFont.Text = string.Format(
                    "{0} ({1}) {2}",
                    selectedFont_.FontFamily.Name,
                    selectedFont_.Style.ToString(),
                    selectedFont_.Size);
            }
        }

        private string SelectedOutputDirectory
        {
            get { return selectedOutputDirectory_; }
            set
            {
                selectedOutputDirectory_ = value;
                textBoxOutputDirectory.Text = selectedOutputDirectory_;
            }
        }

        private Color SelectedColor
        {
            get { return selectedColor_; }
            set
            {
                selectedColor_ = value;
                pictureBoxColour.BackColor = selectedColor_;
            }
        }

        private Font selectedFont_;
        private string selectedOutputDirectory_;
        private Color selectedColor_;
    }
}
