#pragma once

#include "../../../Shared/RornMaths/Float4.h"
#include "../../../Shared/RornMaths/Matrix4x4.h"
#include "../../../Shared/RornMaths/Vector4.h"

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
			float pad0[3];
			Maths::Vector4 MainLightDir;
			Maths::Float4 MainLightColor;
			Maths::Vector4 EyeDir;
			unsigned int NumActivePointLights;
			float pad1[3];
			Maths::Vector4 PointLightPositions[16];
			Maths::Float4 PointLightColors[16];
			Maths::Float4 PointLightLuminosities[16];
		};
	}
}
