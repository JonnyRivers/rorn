#pragma once

#include "../../../Shared/RornMaths/Float4.h"
#include "../../../Shared/RornMaths/Matrix4x4.h"
#include "../../../Shared/RornMaths/Vector3.h"

namespace Rorn
{
	namespace Engine
	{
		struct LitGeometryConstantBuffer
		{
			Maths::Matrix4x4 ModelToWorldMatrix;
			Maths::Matrix4x4 WorldToProjectionMatrix;
			Maths::Float4 AmbientColor;
			Maths::Float4 DiffuseColor;
			Maths::Float4 SpecularColor;
			Maths::Float4 AmbientLightColor;
			float PhongExponent;
			Maths::Vector3 MainLightDir;
			Maths::Float4 MainLightColor;
			Maths::Vector3 EyeDir;
			unsigned int NumActivePointLights;
			Maths::Float4 PointLightPositions[16];// intensities are packed in the w here
			Maths::Float4 PointLightColors[16];
		};
	}
}
