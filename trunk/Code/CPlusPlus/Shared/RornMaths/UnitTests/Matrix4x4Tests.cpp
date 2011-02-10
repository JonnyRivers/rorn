#include "stdafx.h"

#include "..\Constants.h"
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
			
			Rorn::Maths::Matrix4x4 rotationAboutY1 = Rorn::Maths::Matrix4x4::BuildRotationMatrix(yAxis, Rorn::Maths::PiOver4);
			xAxis = Rorn::Maths::Vector3::GetUnitVector(xAxis * rotationAboutY1);
			yAxis = Rorn::Maths::Vector3::GetUnitVector(yAxis * rotationAboutY1);
			zAxis = Rorn::Maths::Vector3::GetUnitVector(zAxis * rotationAboutY1);

			Rorn::Maths::Matrix4x4 rotationAboutX1 = Rorn::Maths::Matrix4x4::BuildRotationMatrix(xAxis, Rorn::Maths::PiOver4);
			xAxis = Rorn::Maths::Vector3::GetUnitVector(xAxis * rotationAboutX1);
			yAxis = Rorn::Maths::Vector3::GetUnitVector(yAxis * rotationAboutX1);
			zAxis = Rorn::Maths::Vector3::GetUnitVector(zAxis * rotationAboutX1);

			Rorn::Maths::Matrix4x4 rotationAboutX2 = Rorn::Maths::Matrix4x4::BuildRotationMatrix(xAxis, -Rorn::Maths::PiOver4);
			xAxis = Rorn::Maths::Vector3::GetUnitVector(xAxis * rotationAboutX2);
			yAxis = Rorn::Maths::Vector3::GetUnitVector(yAxis * rotationAboutX2);
			zAxis = Rorn::Maths::Vector3::GetUnitVector(zAxis * rotationAboutX2);

			Rorn::Maths::Matrix4x4 rotationAboutY2 = Rorn::Maths::Matrix4x4::BuildRotationMatrix(yAxis, -Rorn::Maths::PiOver4);
			xAxis = Rorn::Maths::Vector3::GetUnitVector(xAxis * rotationAboutY2);
			yAxis = Rorn::Maths::Vector3::GetUnitVector(yAxis * rotationAboutY2);
			zAxis = Rorn::Maths::Vector3::GetUnitVector(zAxis * rotationAboutY2);
		}
	};
}
