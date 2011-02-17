#pragma once

#include <windows.h>

#include <d3d11.h>

#include "../../../Shared/RornMaths/Float4.h"
#include "../../../Shared/RornMaths/Matrix4x4.h"
#include "../../../Shared/RornMaths/Vector3.h"

namespace Rorn
{
	namespace Engine
	{
		class UntexturedSurfaceFormat
		{
		public:
			static UntexturedSurfaceFormat& GetInstance();

			virtual HRESULT Initialize(ID3D11Device* device);
			virtual void Release();

			virtual void SetupGPU(
				ID3D11DeviceContext* deviceContext, 
				const Maths::Matrix4x4& instanceToWorldMatrix, 
				const Maths::Matrix4x4& worldToProjectionMatrix,
				const Maths::Float4& ambientColor,
				const Maths::Float4& diffuseColor,
				const Maths::Float4& specularColor,
				float phongExponent);
		private:
			static UntexturedSurfaceFormat& instance_;

			UntexturedSurfaceFormat(void);

			// This could be split up between the vertex shader and pixel shader
			struct ConstantBuffer
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

			ID3D11VertexShader* vertexShader_;
			ID3D11PixelShader* pixelShader_;
			ID3D11InputLayout* vertexLayout_;
			ID3D11Buffer* constantBuffer_;
		};
	}
}
