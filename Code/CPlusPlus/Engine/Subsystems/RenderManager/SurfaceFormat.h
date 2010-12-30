#pragma once

#include <windows.h>

#include <d3d11.h>
#include <xnamath.h>

namespace Rorn
{
	namespace Engine
	{
		// This has to die
		class SurfaceFormat
		{
		public:
			enum Type
			{
				Untextured = 0
			};
			SurfaceFormat(void);
			~SurfaceFormat(void);
			
			virtual HRESULT Startup(ID3D11Device* device) = 0;
			virtual void Shutdown() = 0;
			virtual void SetupGPU(ID3D11DeviceContext* deviceContext, 
				CXMMATRIX instanceToWorldMatrix, CXMMATRIX worldToProjectionMatrix) const = 0;
		};
	}
}