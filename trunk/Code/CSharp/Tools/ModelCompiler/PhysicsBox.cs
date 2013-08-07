using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Rorn.Tools.Maths;

namespace Rorn.Tools.ModelCompiler
{
    internal class PhysicsBox : PhysicsPrimitive
    {
        internal PhysicsBox(Matrix4x4 nodeToSceneMatrix, float width, float length, float height)
            : base(nodeToSceneMatrix, PrimitiveType.Box)
        {
            Width = width;
            Length = length;
            Height = height;
        }

        internal float Width { get; private set; }
        internal float Length { get; private set; }
        internal float Height { get; private set; }

        internal override void SaveInternal(System.IO.BinaryWriter binaryWriter)
        {
            binaryWriter.Write(Width);
            binaryWriter.Write(Length);
            binaryWriter.Write(Height);
        }
    }
}
