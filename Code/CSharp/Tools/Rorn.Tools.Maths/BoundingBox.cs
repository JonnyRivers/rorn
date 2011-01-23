using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Rorn.Tools.Maths
{
    public class BoundingBox
    {
        public BoundingBox()
        {
            MinX = 0;
            MinY = 0;
            MinZ = 0;
            MaxX = 0;
            MaxY = 0;
            MaxZ = 0;
            initialized_ = false;
        }

        public void AddPoint(Vector4 point)
        {
            if (initialized_)
            {
                if (point.X < MinX)
                    MinX = point.X;

                if (point.Y < MinY)
                    MinY = point.Y;

                if (point.Z < MinZ)
                    MinZ = point.Z;

                if (point.X > MaxX)
                    MaxX = point.X;

                if (point.Y > MaxY)
                    MaxY = point.Y;

                if (point.Z > MaxZ)
                    MaxZ = point.Z;
            }
            else
            {
                // This is the first point
                MinX = point.X;
                MinY = point.Y;
                MinZ = point.Z;
                MaxX = point.X;
                MaxY = point.Y;
                MaxZ = point.Z;
                initialized_ = true;
            }
        }

        public void Save(System.IO.BinaryWriter binaryWriter)
        {
            binaryWriter.Write(MinX);
            binaryWriter.Write(MinY);
            binaryWriter.Write(MinZ);
            binaryWriter.Write(MaxX);
            binaryWriter.Write(MaxY);
            binaryWriter.Write(MaxZ);
        }

        public float MinX { get; private set; }
        public float MinY { get; private set; }
        public float MinZ { get; private set; }
        public float MaxX { get; private set; }
        public float MaxY { get; private set; }
        public float MaxZ { get; private set; }

        private bool initialized_;
    }
}
