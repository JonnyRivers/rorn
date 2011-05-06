using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;

namespace FontBuilder
{
    internal class FontBuilder
    {
        internal static void Build(string fontPathname, Font font, Color color)
        {
            var glyphs = new List<Glyph>();
            foreach (char character in charactersToDraw_)
                glyphs.Add(new Glyph(character));

            MeasureGlyphs(glyphs, font);
            int outputTextureDimension = DetermineOutputTextureDimension(glyphs);
            string pngPathname = System.IO.Path.ChangeExtension(System.IO.Path.GetTempFileName(), ".png");
            using (Bitmap outputImage = new Bitmap(outputTextureDimension, outputTextureDimension, System.Drawing.Imaging.PixelFormat.Format32bppArgb))
            {
                OutputGlyphsToImage(glyphs, font, color, outputImage);
                outputImage.Save(pngPathname);
            }
            string ddsPathname = System.IO.Path.GetTempFileName();
            ConvertToDDS(pngPathname, ddsPathname);
#if _DEBUG_OUTPUT_TEMP_IMAGES
            //System.IO.File.Copy(pngPathname, System.IO.Path.ChangeExtension(fontPathname, ".png"));
            //System.IO.File.Copy(ddsPathname, System.IO.Path.ChangeExtension(fontPathname, ".dds"));
#endif
            System.IO.File.Delete(pngPathname);
            byte[] ddsData = System.IO.File.ReadAllBytes(ddsPathname);
            System.IO.File.Delete(ddsPathname);
            WriteFontFile(fontPathname, glyphs, ddsData);
        }

        private static void MeasureGlyphs(IEnumerable<Glyph> glyphs, Font font)
        {
            Dictionary<uint, SizeF> glyphMeasurements = new Dictionary<uint, SizeF>();
            int sketchBitmapDimensions = (int)(font.SizeInPoints * 2);
            using (Bitmap sketchBitmap = new Bitmap(sketchBitmapDimensions, sketchBitmapDimensions))
            {
                using (Graphics graphics = Graphics.FromImage(sketchBitmap))
                {
                    foreach (Glyph glyph in glyphs)
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

        private static int DetermineOutputTextureDimension(IEnumerable<Glyph> glyphs)
        {
            int outputTextureDimension = 64;

            while (true)
            {
                if (GlyphsWillFitOutputTextureDimension(glyphs, outputTextureDimension))
                    return outputTextureDimension;
                else
                    outputTextureDimension *= 2;
            }
        }

        private static bool GlyphsWillFitOutputTextureDimension(IEnumerable<Glyph> glyphs, int outputTextureDimension)
        {
            uint nextGlyphX = 0;
            uint nextGlyphY = 0;
            foreach (Glyph glyph in glyphs)
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

        private static void OutputGlyphsToImage(IEnumerable<Glyph> glyphs, Font font, Color color, Bitmap image)
        {
            using (Graphics graphics = Graphics.FromImage(image))
            {
                using (Brush textBrush = new SolidBrush(color))
                {
                    graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
                    graphics.TextRenderingHint = System.Drawing.Text.TextRenderingHint.AntiAlias;
                    uint currentGlyphX = 0;
                    uint currentGlyphY = 0;
                    foreach (Glyph glyph in glyphs)
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
                        glyph.StartU = (float)((double)glyph.X / (double)image.Width);
                        glyph.StartV = (float)((double)glyph.Y / (double)image.Height);
                        glyph.EndU = glyph.StartU + (float)((double)glyph.Width / (double)image.Width);
                        glyph.EndV = glyph.StartV + (float)((double)glyph.Height / (double)image.Height);

                        currentGlyphX += glyph.Width;
                    }
                }
            }
        }

        private static void ConvertToDDS(string sourcePathname, string ddsPathname)
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

        private static void WriteFontFile(string pathname, IEnumerable<Glyph> glyphs, byte[] ddsData)
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

                uint numGlyphs = (uint)glyphs.Count();
                writer.Write(numGlyphs);

                foreach (Glyph glyph in glyphs)
                {
                    writer.Write(glyph.CharacterCode);
                    writer.Write(glyph.X);
                    writer.Write(glyph.Y);
                    writer.Write(glyph.Width);
                    writer.Write(glyph.Height);
                    writer.Write(glyph.StartU);
                    writer.Write(glyph.StartV);
                    writer.Write(glyph.EndU);
                    writer.Write(glyph.EndV);
                }
                
                uint ddsDataLength = (uint)ddsData.Length;
                writer.Write(ddsDataLength);
                writer.Write(ddsData);
            }
        }

        private static uint CurrentVersionNumber = 1;

        private static char[] charactersToDraw_ = new char[]
        {
            '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
            '`', '¬', '!', '"', '£', '$', '%', '^', '&', '*', '(', ')', '-', '_', '=', '+', '[', '{', ']', '}', ';', ':', '\'', '@', '#', '~', '\\', '|', ',', '<', '.', '>', '/', '?', ' '
        };
    }
}
