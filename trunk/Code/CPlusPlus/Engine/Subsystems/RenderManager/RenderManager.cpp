#include "RenderManager.h"

#include <cassert>

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


void RenderManager::Startup(HWND hwnd)
{
    RECT rc;
    GetClientRect( hwnd, &rc );
    outputWidth_ = rc.right - rc.left;
    outputHeight_ = rc.bottom - rc.top;
	aspectRatio_ = static_cast<FLOAT>(outputWidth_) / static_cast<FLOAT>(outputHeight_);

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
            break;
    }
    // TODO throw on fail

    // Create a render target view
    ID3D11Texture2D* backBuffer = NULL;
    hr = swapChain_->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&backBuffer );
    // TODO throw on fail

    hr = device_->CreateRenderTargetView( backBuffer, NULL, &renderTargetView_ );
    backBuffer->Release();
    // TODO throw on fail

    deviceContext_->OMSetRenderTargets( 1, &renderTargetView_, NULL );

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)outputWidth_;
    vp.Height = (FLOAT)outputHeight_;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    deviceContext_->RSSetViewports( 1, &vp );
}

void RenderManager::Shutdown()
{
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
	worldToViewMatrix_ = XMMatrixLookAtLH( currentCamera_->Eye, currentCamera_->Target, currentCamera_->Up );
	viewToProjectionMatrix_ = XMMatrixPerspectiveFovLH( XM_PIDIV4, aspectRatio_, 0.01f, 100.0f );

	// for each mesh instance
		// set the mesh_to_world matrix
		// for each render command
			// set vertex_shader
			// set vertex_shader_constant_buffers
			// set pixel_shader
			// set pixel_shader_constant_buffers
			// set the primitive_type
			// draw the primitives

    swapChain_->Present( 0, 0 );
}