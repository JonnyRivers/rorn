using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Rorn.Tools.Maths;

namespace Rorn.Tools.ModelCompiler
{
    public struct ModelHeader
    {
        public ModelHeader(BoundingBox boundingBox, int numRenderCommands, int numCompiledTextures)
        {
            FileIdentifier = new byte[8];
            FileIdentifier[0] = (byte)'R';
            FileIdentifier[1] = (byte)'O';
            FileIdentifier[2] = (byte)'R';
            FileIdentifier[3] = (byte)'N';
            FileIdentifier[4] = (byte)'M';
            FileIdentifier[5] = (byte)'O';
            FileIdentifier[6] = (byte)'D';
            FileIdentifier[7] = (byte)'L';
            VersionNumber = CurrentVersionNumber;
            BoundingBox = boundingBox;
            NumRenderCommands = numRenderCommands;
            NumCompiledTextures = numCompiledTextures;
        }

        public void Save(System.IO.BinaryWriter binaryWriter)
        {
            binaryWriter.Write(FileIdentifier);
            binaryWriter.Write(VersionNumber);
            BoundingBox.Save(binaryWriter);
            binaryWriter.Write(NumRenderCommands);
        }

        public static int CurrentVersionNumber = 1;

        public byte[] FileIdentifier;
        public int VersionNumber;
        public BoundingBox BoundingBox;
        public int NumRenderCommands;
        public int NumCompiledTextures;
    }
}
