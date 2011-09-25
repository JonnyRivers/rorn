#pragma once

#include "../../../Shared/RornMaths/Float4.h"
#include "../../../Shared/RornMaths/Matrix4x4.h"
#include "../../../Shared/RornMaths/Position.h"
#include "../../../Shared/RornMaths/UnitDirection.h"

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
			Maths::UnitDirection MainLightDir;
			Maths::Float4 MainLightColor;
			Maths::UnitDirection EyeDir;
			unsigned int NumActivePointLights;
			float pad1[3];
			Maths::Position PointLightPositions[16];
			Maths::Float4 PointLightColors[16];
			Maths::Float4 PointLightLuminosities[16];
		};
	}
}
