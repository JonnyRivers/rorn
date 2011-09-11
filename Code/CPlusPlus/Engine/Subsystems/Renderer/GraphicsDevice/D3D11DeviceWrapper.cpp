#pragma once

#include "D3D11DeviceWrapper.h"

#include "../../../Exceptions/initialisation_exception.h"

#include "../../../../Shared/ErrorCodes/HResultFormatter.h"

using namespace Rorn::Engine;

D3D11DeviceWrapper::D3D11DeviceWrapper(HWND applicationWindowHandle, IDiagnostics* diagnostics, UINT outputWidth, UINT outputHeight)
	: diagnostics_(diagnostics)
{
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
	sd.BufferDesc.Width = outputWidth;
	sd.BufferDesc.Height = outputHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = applicationWindowHandle;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

	bool deviceAndSwapChainCreated = false;
	HRESULT hr;
    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes && !deviceAndSwapChainCreated; driverTypeIndex++ )
    {
        D3D_DRIVER_TYPE driverType = driverTypes[driverTypeIndex];
		D3D_FEATURE_LEVEL featureLevel;
        hr = D3D11CreateDeviceAndSwapChain( NULL, driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                                            D3D11_SDK_VERSION, &sd, &SwapChain, &Device, &featureLevel, &DeviceContext );
        if( SUCCEEDED( hr ) )
		{
			diagnostics->GetLoggingStream() << "The D3D11 device and swap chain were successfully created." << std::endl;
			diagnostics->GetLoggingStream() << "The driver type is " << driverType << "." << std::endl;
			diagnostics->GetLoggingStream() << "The feature level is 0x" << std::hex << featureLevel << std::dec << "." << std::endl;
			deviceAndSwapChainCreated = true;
		}
    }

	if( !deviceAndSwapChainCreated )
	{
		diagnostics->GetLoggingStream() << "Unable to create D3D11 device and swap chain.  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
		throw initialisation_exception("Unable to create D3D11 device and swap chain.");
	}
}

D3D11DeviceWrapper::~D3D11DeviceWrapper()
{
	//DeviceContext->ClearState();// required?
	SwapChain->Release();
	DeviceContext->Release();
	Device->Release();
}