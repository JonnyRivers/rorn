#include "stdafx.h"

#include "..\Matrix4x4.h"
#include "..\Vector3.h"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace	Microsoft::VisualStudio::TestTools::UnitTesting;

namespace UnitTests
{
	[TestClass]
	public ref class Matrix4x4Tests
	{
	public: 
		[TestMethod]
		void TestBuildRotationMatrix()
		{
			Rorn::Maths::Vector3 xAxis(1.0f, 0.0f, 0.0f);
			Rorn::Maths::Vector3 yAxis(0.0f, 1.0f, 0.0f);
			Rorn::Maths::Vector3 zAxis(0.0f, 0.0f, 1.0f);
			for(int i = 0 ; i < 10000 ; ++i)
			{
				Rorn::Maths::Matrix4x4 rotationAboutY = Rorn::Maths::Matrix4x4::BuildRotationMatrix(yAxis, 0.01f);
				xAxis = xAxis * rotationAboutY;
				yAxis = yAxis * rotationAboutY;
				zAxis = zAxis * rotationAboutY;
			}
		}
	};
}
