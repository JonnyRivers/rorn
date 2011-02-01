using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Rorn.Tools.ModelCompiler
{
    internal class CompiledTexture
    {
        internal CompiledTexture(int compiledTextureId, byte[] textureData)
        {
            compiledTextureId_ = compiledTextureId;
            textureData_ = textureData;
        }

        internal void Save(System.IO.BinaryWriter binaryWriter)
        {
            binaryWriter.Write(compiledTextureId_);
            binaryWriter.Write(textureData_.Length);
            binaryWriter.Write(textureData_);
        }

        internal int Id { get { return compiledTextureId_; } }

        private int compiledTextureId_;
        private byte[] textureData_;
    }
}
