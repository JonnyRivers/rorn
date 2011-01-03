#include "RenderManager.h"

#include <cassert>

#include "Camera.h"
#include "Light.h"
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
	: device_(NULL), 
	deviceContext_(NULL), 
	swapChain_(NULL), 
	renderTargetView_(NULL), 
	depthStencil_(NULL), 
	depthStencilView_(NULL),
	currentCamera_(NULL),
	mainLight_(NULL),
	ambientLightColor_(0.0f, 0.0f, 0.0f, 1.0f)
{
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
	std::list<std::unique_ptr<Model>>::iterator modelIter;
	for(modelIter = models_.begin() ; modelIter != models_ .end() ; ++modelIter)
		(*modelIter)->Release();

	models_.clear();
	modelInstances_.clear();
	cameras_.clear();

	UntexturedSurfaceFormat::GetInstance().Release();

	if( depthStencil_ != NULL )
		depthStencil_->Release();

	if( depthStencilView_ != NULL )
		depthStencilView_->Release();

	if( deviceContext_ != NULL )
		deviceContext_->ClearState();

    if( renderTargetView_ != NULL )
		renderTargetView_->Release();

    if( swapChain_ != NULL )
		swapChain_->Release();

    if( deviceContext_ != NULL )
		deviceContext_->Release();

    if( device_ != NULL )
		device_->Release();
}

XMFLOAT4 RenderManager::GetAmbientLightColor()
{
	return ambientLightColor_;
}

void RenderManager::SetAmbientLightColor(const XMFLOAT4& color)
{
	ambientLightColor_ = color;
}

Camera* RenderManager::CreateCamera(XMVECTOR eye, XMVECTOR target, XMVECTOR up)
{
	Camera* newCamera = new Camera(eye, target, up);
	cameras_.push_back(std::unique_ptr<Camera>(newCamera));

	return newCamera;
}

void RenderManager::SetCurrentCamera(Camera* camera)
{
	currentCamera_ = camera;
}

Light* RenderManager::CreateLight(const XMFLOAT4& direction, const XMFLOAT4& color)
{
	Light* newLight = new Light(direction, color);
	lights_.push_back(std::unique_ptr<Light>(newLight));

	return newLight;
}

Light* RenderManager::GetMainLight()
{
	return mainLight_;
}

void RenderManager::SetMainLight(Light* light)
{
	mainLight_ = light;
}

Model* RenderManager::LoadOrGetModel(LPCTSTR modelPathName)
{
	Model* newModel = new Model();
	newModel->LoadFromFile(modelPathName, device_);
	models_.push_back(std::unique_ptr<Model>(newModel));

	return newModel;
}

ModelInstance* RenderManager::CreateModelInstance(Model* model, CXMMATRIX instanceToWorldMatrix)
{
	ModelInstance* newModelInstance = new ModelInstance(model, instanceToWorldMatrix);
	modelInstances_.push_back(std::unique_ptr<ModelInstance>(newModelInstance));

	return newModelInstance;
}

void RenderManager::Step()
{
	// Just clear the backbuffer
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha
    deviceContext_->ClearRenderTargetView( renderTargetView_, ClearColor );

	// Clear the depth buffer to 1.0 (max depth)
	deviceContext_->ClearDepthStencilView( depthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	// Draw all of the registered mesh instances using the registered camera and light(s)
	
	// Setup the world_to_view and view_to_projection matrices based on the current camera (& other settings)
	assert(currentCamera_ != NULL);
	XMMATRIX worldToViewMatrix = XMMatrixLookAtLH( currentCamera_->Eye, currentCamera_->Target, currentCamera_->Up );
	XMMATRIX viewToProjectionMatrix = XMMatrixPerspectiveFovLH( XM_PIDIV4, aspectRatio_, 0.01f, 10000.0f );
	XMMATRIX worldToProjectionMatrix = XMMatrixMultiply(worldToViewMatrix, viewToProjectionMatrix);

	std::list<std::unique_ptr<ModelInstance>>::const_iterator instanceIter;
	for(instanceIter = modelInstances_.cbegin() ; instanceIter != modelInstances_.cend() ; ++instanceIter)
	{
		(*instanceIter)->Draw(deviceContext_, worldToProjectionMatrix);
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
    if( FAILED( hr ) )
        return hr;

    hr = device_->CreateRenderTargetView( backBuffer, NULL, &renderTargetView_ );
    backBuffer->Release();
    if( FAILED( hr ) )
        return hr;

	// Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory( &descDepth, sizeof(descDepth) );
    descDepth.Width = outputWidth_;
    descDepth.Height = outputHeight_;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = device_->CreateTexture2D( &descDepth, NULL, &depthStencil_ );
    if( FAILED( hr ) )
        return hr;

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory( &descDSV, sizeof(descDSV) );
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = device_->CreateDepthStencilView( depthStencil_, &descDSV, &depthStencilView_ );
    if( FAILED( hr ) )
        return hr;

    deviceContext_->OMSetRenderTargets( 1, &renderTargetView_, depthStencilView_ );

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
	HRESULT hr = UntexturedSurfaceFormat::GetInstance().Initialize(device_);
	if( FAILED(hr) )
		return hr;

	return S_OK;
}