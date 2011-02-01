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
		class DiffuseOnlySurfaceFormat
		{
		public:
			static DiffuseOnlySurfaceFormat& GetInstance();

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
			static DiffuseOnlySurfaceFormat& instance_;

			DiffuseOnlySurfaceFormat(void);

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
				float pad0;
			};

			ID3D11VertexShader* vertexShader_;
			ID3D11PixelShader* pixelShader_;
			ID3D11InputLayout* vertexLayout_;
			ID3D11Buffer* constantBuffer_;
			ID3D11SamplerState* sampler_;
		};
	}
}
