using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;

namespace FontBuilder
{
    internal class FontBuilder
    {
        internal FontBuilder()
        {
        }

        internal void Build(string outputDirectory, Font font, Color color)
        {
            Dictionary<uint, SizeF> glyphMeasurements = MeasureGlyphs(font, glyphsToDraw_);
            int outputTextureDimension = DetermineOutputTextureDimension(glyphMeasurements);
            string pngPathname = string.Format("{0}\\temp.png", outputDirectory);
            using (Bitmap outputImage = new Bitmap(outputTextureDimension, outputTextureDimension, System.Drawing.Imaging.PixelFormat.Format32bppArgb))
            {
                OutputGlyphsToImage(glyphsToDraw_, font, color, outputImage);
                outputImage.Save(pngPathname);
            }
            string ddsPathname = System.IO.Path.ChangeExtension(pngPathname, ".dds");
            ConvertToDDS(pngPathname, ddsPathname);
            // last step - build proprietary font file
        }

        private Dictionary<uint, SizeF> MeasureGlyphs(Font font, IEnumerable<char> glyphsToMeasure)
        {
            Dictionary<uint, SizeF> glyphMeasurements = new Dictionary<uint, SizeF>();
            int sketchBitmapDimensions = (int)(font.SizeInPoints * 2);
            using (Bitmap sketchBitmap = new Bitmap(sketchBitmapDimensions, sketchBitmapDimensions))
            {
                using (Graphics graphics = Graphics.FromImage(sketchBitmap))
                {
                    foreach (char glyph in glyphsToMeasure)
                    {
                        string stringToMeasure = "";
                        stringToMeasure += glyph;
                        SizeF glyphSize = graphics.MeasureString(stringToMeasure, font);
                        glyphMeasurements.Add((uint)glyph, glyphSize);
                    }
                }
            }

            return glyphMeasurements;
        }

        private int DetermineOutputTextureDimension(Dictionary<uint, SizeF> glyphs)
        {
            int outputTextureDimension = 64;

            while (true)
            {
                if (GlyphsWillFitOutputTextureDimension(glyphs.Values, outputTextureDimension))
                    return outputTextureDimension;
                else
                    outputTextureDimension *= 2;
            }
        }

        private bool GlyphsWillFitOutputTextureDimension(IEnumerable<SizeF> glyphSizes, int outputTextureDimension)
        {
            int nextGlyphX = 0;
            int nextGlyphY = 0;
            foreach (SizeF glyphSize in glyphSizes)
            {
                int glyphWidth = (int)Math.Ceiling(glyphSize.Width);
                int glyphHeight = (int)Math.Ceiling(glyphSize.Height);
                if (nextGlyphX + glyphWidth > outputTextureDimension)// check for overflow of width
                {
                    nextGlyphY += glyphHeight;
                    nextGlyphX = 0;
                }
                if (nextGlyphY + glyphWidth > outputTextureDimension)// check for full output size overflow
                {
                    return false;
                }
                nextGlyphX += glyphWidth;
            }

            return true;
        }

        private void OutputGlyphsToImage(IEnumerable<char> glyphs, Font font, Color color, Bitmap image)
        {
            using (Graphics graphics = Graphics.FromImage(image))
            {
                using (Brush textBrush = new SolidBrush(color))
                {
                    graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
                    graphics.TextRenderingHint = System.Drawing.Text.TextRenderingHint.AntiAlias;
                    int glyphX = 0;
                    int glyphY = 0;
                    foreach (char glyph in glyphs)
                    {
                        string stringToDraw = "";
                        stringToDraw += glyph;
                        SizeF glyphSize = graphics.MeasureString(stringToDraw, font);

                        int glyphWidth = (int)Math.Ceiling(glyphSize.Width);
                        int glyphHeight = (int)Math.Ceiling(glyphSize.Height);
                        if (glyphX + glyphWidth > image.Width)// check for overflow of width
                        {
                            glyphY += glyphHeight;
                            glyphX = 0;
                        }

                        graphics.DrawString(stringToDraw, font, textBrush, (float)glyphX, (float)glyphY);

                        glyphX += glyphWidth;
                    }
                }
            }
        }

        private void ConvertToDDS(string sourcePathname, string ddsPathname)
        {
            // Run nvcompress, which must live alongside the executing assembly (for now)
            string modelCompilerPathName = System.Reflection.Assembly.GetExecutingAssembly().Location;
            string modelCompilerDirectory = System.IO.Path.GetDirectoryName(modelCompilerPathName);
            string nvcompressPathName = string.Format("{0}\\nvcompress.exe", modelCompilerDirectory);
            string tempTargetPathName = System.IO.Path.GetTempFileName();
            string arguments = string.Format("-alpha -fast -bc3 \"{0}\" \"{1}\"", sourcePathname, ddsPathname);
            var processStartInfo = new System.Diagnostics.ProcessStartInfo(nvcompressPathName, arguments);
            processStartInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
            var nvcompressProcess = System.Diagnostics.Process.Start(processStartInfo);
            nvcompressProcess.WaitForExit();
        }

        private char[] glyphsToDraw_ = new char[]
        {
            '1', '2', '3', '4', '5', '6', '7', '8', '9',
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
            '`', '¬', '!', '"', '£', '$', '%', '^', '&', '*', '(', ')', '-', '_', '=', '+', '[', '{', ']', '}', ';', ':', '\'', '@', '#', '~', '\\', '|', ',', '<', '.', '>', '/', '?'
        };
    }
}
