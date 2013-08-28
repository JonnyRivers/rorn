using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Rorn.Tools.Maths;

namespace Rorn.Tools.ModelCompiler
{
    internal abstract class PhysicsPrimitive
    {
        internal PhysicsPrimitive(float mass, Matrix4x4 nodeToSceneMatrix, PrimitiveType type)
        {
            // HACK to make compound shapes work for now
            // We need to properly update nodeToSceneMatrix to take the max > Rorn transform into account
            float m42 = nodeToSceneMatrix.M43;
            nodeToSceneMatrix.M43 = -nodeToSceneMatrix.M42;
            nodeToSceneMatrix.M42 = m42;

            Mass = mass;
            NodeToSceneMatrix = nodeToSceneMatrix;
            Type = type;
        }

        internal enum PrimitiveType
        {
            Box = 0,
            Cylinder = 1,
            Mesh = 2,
            Sphere = 3
        }

        internal void Transform(Matrix4x4 transform)
        {
            NodeToSceneMatrix = NodeToSceneMatrix * transform;
        }

        internal void Save(System.IO.BinaryWriter binaryWriter)
        {
            binaryWriter.Write(this.Mass);
            NodeToSceneMatrix.Save(binaryWriter);
            binaryWriter.Write((uint)this.Type);
            SaveInternal(binaryWriter);
        }

        internal float Mass { get; private set; }
        internal Matrix4x4 NodeToSceneMatrix { get; private set; }
        internal PrimitiveType Type { get; private set; }

        internal abstract void SaveInternal(System.IO.BinaryWriter binaryWriter);
    }
}
