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

        internal void Build(string fontPathname, Font font, Color color)
        {
            glyphs_ = new List<Glyph>();
            foreach (char character in charactersToDraw_)
                glyphs_.Add(new Glyph(character));

            MeasureGlyphs(font);
            int outputTextureDimension = DetermineOutputTextureDimension();
            string pngPathname = System.IO.Path.ChangeExtension(System.IO.Path.GetTempFileName(), ".png");
            using (Bitmap outputImage = new Bitmap(outputTextureDimension, outputTextureDimension, System.Drawing.Imaging.PixelFormat.Format32bppArgb))
            {
                OutputGlyphsToImage(font, color, outputImage);
                outputImage.Save(pngPathname);
            }
            string ddsPathname = System.IO.Path.GetTempFileName();
            ConvertToDDS(pngPathname, ddsPathname);
            System.IO.File.Copy(pngPathname, System.IO.Path.ChangeExtension(fontPathname, ".png"));
            System.IO.File.Delete(pngPathname);
            byte[] ddsData = System.IO.File.ReadAllBytes(ddsPathname);
            System.IO.File.Copy(ddsPathname, System.IO.Path.ChangeExtension(fontPathname, ".dds"));
            System.IO.File.Delete(ddsPathname);
            WriteFontFile(fontPathname, ddsData);

            glyphs_ = null;
        }

        private void MeasureGlyphs(Font font)
        {
            Dictionary<uint, SizeF> glyphMeasurements = new Dictionary<uint, SizeF>();
            int sketchBitmapDimensions = (int)(font.SizeInPoints * 2);
            using (Bitmap sketchBitmap = new Bitmap(sketchBitmapDimensions, sketchBitmapDimensions))
            {
                using (Graphics graphics = Graphics.FromImage(sketchBitmap))
                {
                    foreach (Glyph glyph in glyphs_)
                    {
                        string stringToMeasure = "";
                        stringToMeasure += glyph.TextCharacter;
                        SizeF glyphSize = graphics.MeasureString(stringToMeasure, font);
                        glyph.Width = (uint)Math.Ceiling(glyphSize.Width);
                        glyph.Height = (uint)Math.Ceiling(glyphSize.Height);
                    }
                }
            }
        }

        private int DetermineOutputTextureDimension()
        {
            int outputTextureDimension = 64;

            while (true)
            {
                if (GlyphsWillFitOutputTextureDimension(outputTextureDimension))
                    return outputTextureDimension;
                else
                    outputTextureDimension *= 2;
            }
        }

        private bool GlyphsWillFitOutputTextureDimension(int outputTextureDimension)
        {
            uint nextGlyphX = 0;
            uint nextGlyphY = 0;
            foreach (Glyph glyph in glyphs_)
            {
                if (nextGlyphX + glyph.Width > outputTextureDimension)// check for overflow of width
                {
                    nextGlyphY += glyph.Height;
                    nextGlyphX = 0;
                }
                if (nextGlyphY + glyph.Height > outputTextureDimension)// check for full output size overflow
                {
                    return false;
                }
                nextGlyphX += glyph.Width;
            }

            return true;
        }

        private void OutputGlyphsToImage(Font font, Color color, Bitmap image)
        {
            using (Graphics graphics = Graphics.FromImage(image))
            {
                using (Brush textBrush = new SolidBrush(color))
                {
                    graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
                    graphics.TextRenderingHint = System.Drawing.Text.TextRenderingHint.AntiAlias;
                    uint currentGlyphX = 0;
                    uint currentGlyphY = 0;
                    foreach (Glyph glyph in glyphs_)
                    {
                        if (currentGlyphX + glyph.Width > image.Width)// check for overflow of width
                        {
                            currentGlyphY += glyph.Height;
                            currentGlyphX = 0;
                        }

                        string stringToDraw = "";
                        stringToDraw += glyph.TextCharacter;
                        graphics.DrawString(stringToDraw, font, textBrush, (float)currentGlyphX, (float)currentGlyphY);
                        glyph.X = currentGlyphX;
                        glyph.Y = currentGlyphY;

                        currentGlyphX += glyph.Width;
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

        private void WriteFontFile(string pathname, byte[] ddsData)
        {
            using (System.IO.BinaryWriter writer = new System.IO.BinaryWriter(System.IO.File.Create(pathname)))
            {
                writer.Write((byte)'R');
                writer.Write((byte)'O');
                writer.Write((byte)'R');
                writer.Write((byte)'N');
                writer.Write((byte)'F');
                writer.Write((byte)'O');
                writer.Write((byte)'N');
                writer.Write((byte)'T');

                writer.Write(CurrentVersionNumber);

                uint numGlyphs = (uint)glyphs_.Count;
                writer.Write(numGlyphs);

                foreach (Glyph glyph in glyphs_)
                {
                    writer.Write(glyph.CharacterCode);
                    writer.Write(glyph.X);
                    writer.Write(glyph.Y);
                    writer.Write(glyph.Width);
                    writer.Write(glyph.Height);
                }
                
                uint ddsDataLength = (uint)ddsData.Length;
                writer.Write(ddsDataLength);
                writer.Write(ddsData);
            }
        }

        private static uint CurrentVersionNumber = 1;

        private char[] charactersToDraw_ = new char[]
        {
            '1', '2', '3', '4', '5', '6', '7', '8', '9',
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
            '`', '¬', '!', '"', '£', '$', '%', '^', '&', '*', '(', ')', '-', '_', '=', '+', '[', '{', ']', '}', ';', ':', '\'', '@', '#', '~', '\\', '|', ',', '<', '.', '>', '/', '?'
        };

        private List<Glyph> glyphs_;
    }
}
