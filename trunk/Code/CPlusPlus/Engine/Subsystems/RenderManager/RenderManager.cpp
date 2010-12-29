#include "RenderManager.h"

#include <cassert>

#include "Camera.h"
#include "Model.h"
#include "ModelInstance.h"
#include "UntexturedSurfaceFormat.h"

using namespace Rorn::Engine;

/*static*/ RenderManager& RenderManager::instance_ = RenderManager();// init static instance

/*static*/ RenderManager& RenderManager::GetInstance()
{
	return instance_;
}
RenderManager::RenderManager(void)
{
	currentCamera_ = NULL;
}


HRESULT RenderManager::Startup(HWND hwnd)
{
	SetupScreenCoordinates(hwnd);

	HRESULT hr = SetupDeviceAndSwapChain(hwnd);
	if ( FAILED(hr) )
		return hr;

	hr = SetupRenderTargetView();
	if ( FAILED(hr) )
		return hr;

    SetupViewport();

	hr = SetupSurfaceFormats();
	if ( FAILED(hr) )
		return hr;

	return S_OK;
}

void RenderManager::Shutdown()
{
	std::map<SurfaceFormat::Type, std::unique_ptr<SurfaceFormat>>::const_iterator surfaceFormatIter;
	for(surfaceFormatIter = surfaceFormats_.begin() ; surfaceFormatIter != surfaceFormats_.end() ; ++surfaceFormatIter)
		surfaceFormatIter->second->Shutdown();

	if( deviceContext_ ) 
		deviceContext_->ClearState();

    if( renderTargetView_ ) 
		renderTargetView_->Release();

    if( swapChain_ ) 
		swapChain_->Release();

    if( deviceContext_ ) 
		deviceContext_->Release();

    if( device_ ) 
		device_->Release();
}

Camera& RenderManager::CreateCamera(XMVECTOR eye, XMVECTOR target, XMVECTOR up)
{
	cameras_.push_back(Camera(eye, target, up));
	return cameras_.back();
}

void RenderManager::SetCurrentCamera(Camera& camera)
{
	currentCamera_ = &camera;
}

void RenderManager::Step()
{
	// Just clear the backbuffer
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha
    deviceContext_->ClearRenderTargetView( renderTargetView_, ClearColor );

	// Draw all of the registered mesh instances using the registered camera and light(s)
	
	// Setup the world_to_view and view_to_projection matrices based on the current camera (& other settings)
	assert(currentCamera_ != NULL);
	XMMATRIX worldToViewMatrix = XMMatrixLookAtLH( currentCamera_->Eye, currentCamera_->Target, currentCamera_->Up );
	XMMATRIX viewToProjectionMatrix = XMMatrixPerspectiveFovLH( XM_PIDIV4, aspectRatio_, 0.01f, 100.0f );
	XMMATRIX worldToProjectionMatrix = XMMatrixMultiply(worldToViewMatrix, viewToProjectionMatrix);

	std::list<ModelInstance>::const_iterator instanceIter;
	for(instanceIter = modelInstances_.cbegin() ; instanceIter != modelInstances_.cend() ; ++instanceIter)
	{
		instanceIter->Draw(deviceContext_, worldToProjectionMatrix);
	}

    swapChain_->Present( 0, 0 );
}

void RenderManager::SetupScreenCoordinates(HWND hwnd)
{
	RECT rc;
    GetClientRect( hwnd, &rc );
    outputWidth_ = rc.right - rc.left;
    outputHeight_ = rc.bottom - rc.top;
	aspectRatio_ = static_cast<FLOAT>(outputWidth_) / static_cast<FLOAT>(outputHeight_);
}

HRESULT RenderManager::SetupDeviceAndSwapChain(HWND hwnd)
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
    sd.BufferDesc.Width = outputWidth_;
    sd.BufferDesc.Height = outputHeight_;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

	HRESULT hr;
    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        driverType_ = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain( NULL, driverType_, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                                            D3D11_SDK_VERSION, &sd, &swapChain_, &device_, &featureLevel_, &deviceContext_ );
        if( SUCCEEDED( hr ) )
            return hr;
    }
    
	return hr;
}

HRESULT RenderManager::SetupRenderTargetView()
{
	ID3D11Texture2D* backBuffer = NULL;
    HRESULT hr = swapChain_->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&backBuffer );
    // TODO throw on fail

    hr = device_->CreateRenderTargetView( backBuffer, NULL, &renderTargetView_ );
    backBuffer->Release();
    // TODO throw on fail

    deviceContext_->OMSetRenderTargets( 1, &renderTargetView_, NULL );

	return hr;
}

void RenderManager::SetupViewport()
{
	D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)outputWidth_;
    vp.Height = (FLOAT)outputHeight_;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    deviceContext_->RSSetViewports( 1, &vp );
}

HRESULT RenderManager::SetupSurfaceFormats()
{
	surfaceFormats_[SurfaceFormat::Untextured] = std::unique_ptr<SurfaceFormat>(new UntexturedSurfaceFormat());
	HRESULT hr = surfaceFormats_[SurfaceFormat::Untextured]->Startup(device_);
	if( FAILED(hr) )
		return hr;

	return S_OK;
}