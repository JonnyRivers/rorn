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
            SelectedOutputPathname = Application.StartupPath + "\\arial10regular.font";
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

        private void OnBrowseOutputPathname(object sender, EventArgs e)
        {
            string selectedPathname;
            if (SaveBrowseForFontFile(SelectedOutputPathname, out selectedPathname))
            {
                SelectedOutputPathname = selectedPathname;
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
                FontBuilder.Build(SelectedOutputPathname, SelectedFont, SelectedColor);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString(), "Unexpected error while trying to build font");
            }
        }

        private static bool SaveBrowseForFontFile(string initialDir, out string filePath)
        {
            return SaveBrowseForFile(initialDir, out filePath, "Save Font File", "font files (*.font)|*.font");
        }

        private static bool SaveBrowseForFile(string initialDir, out string filePath, string dialogTitle, string filter)
        {
            using (SaveFileDialog saveFileDlg = new SaveFileDialog())
            {
                saveFileDlg.Title = dialogTitle;
                saveFileDlg.InitialDirectory = initialDir;
                saveFileDlg.Filter = filter;

                DialogResult result = saveFileDlg.ShowDialog();

                if (result == DialogResult.Cancel)
                {
                    filePath = "";
                    return false;
                }

                filePath = saveFileDlg.FileName;
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

        private string SelectedOutputPathname
        {
            get { return selectedOutputPathname_; }
            set
            {
                selectedOutputPathname_ = value;
                textBoxOutputPathname.Text = selectedOutputPathname_;
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
        private string selectedOutputPathname_;
        private Color selectedColor_;
    }
}
