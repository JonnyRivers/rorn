#include "RenderManager.h"

#include <cassert>

#include "../../../Shared/RornMaths/Constants.h"

#include "../DiagnosticsManager/DiagnosticsManager.h"
#include "../TextureManager/TextureManager.h"
#include "../FontManager/FontManager.h"

#include "Camera.h"
#include "FreeCamera.h"
#include "LookAtCamera.h"
#include "LookToCamera.h"
#include "Model.h"
#include "ModelInstance.h"

#include "UntexturedSurfaceFormat.h"
#include "DiffuseOnlySurfaceFormat.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

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
	currentCamera_(NULL)
{
}

HRESULT RenderManager::Startup(HWND hwnd)
{
	DiagnosticsManager::GetInstance().GetLoggingStream() << "The Render Manager is starting up." << std::endl;

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

	hr = TextureManager::GetInstance().Startup(device_);
	if ( FAILED(hr) )
		return hr;

	hr = FontManager::GetInstance().Startup();
	if ( FAILED(hr) )
		return hr;

	DiagnosticsManager::GetInstance().GetLoggingStream() << "The Render Manager started up successfully." << std::endl;

	return S_OK;
}

void RenderManager::Shutdown()
{
	DiagnosticsManager::GetInstance().GetLoggingStream() << "The Render Manager is shutting down." << std::endl;

	FontManager::GetInstance().Shutdown();

	TextureManager::GetInstance().Shutdown();

	std::list<std::unique_ptr<Model>>::iterator modelIter;
	for(modelIter = models_.begin() ; modelIter != models_ .end() ; ++modelIter)
		(*modelIter)->Release();

	models_.clear();
	modelInstances_.clear();
	cameras_.clear();

	DiffuseOnlySurfaceFormat::GetInstance().Release();
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

	DiagnosticsManager::GetInstance().GetLoggingStream() << "The Render Manager shut down successfully." << std::endl;
}

FreeCamera* RenderManager::CreateFreeCamera(const Vector3& position, const EulerAngles& angles)
{
	FreeCamera* newCamera = new FreeCamera(position, angles);
	cameras_.push_back(std::unique_ptr<Camera>(newCamera));

	return newCamera;
}

LookAtCamera* RenderManager::CreateLookAtCamera(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	LookAtCamera* newCamera = new LookAtCamera(eye, target, up);
	cameras_.push_back(std::unique_ptr<Camera>(newCamera));

	return newCamera;
}

LookToCamera* RenderManager::CreateLookToCamera(const Vector3& eye, const Vector3& direction, const Vector3& up)
{
	LookToCamera* newCamera = new LookToCamera(eye, direction, up);
	cameras_.push_back(std::unique_ptr<Camera>(newCamera));

	return newCamera;
}

Vector3 RenderManager::GetCurrentCameraEyeDir() const
{
	Matrix4x4 worldToViewMatrix = currentCamera_->BuildWorldToViewMatrix();

	return Vector3::GetUnitVector(Vector3(worldToViewMatrix.M31, worldToViewMatrix.M32, worldToViewMatrix.M33));
}

void RenderManager::SetCurrentCamera(Camera* camera)
{
	currentCamera_ = camera;
}

Model* RenderManager::LoadOrGetModel(const wchar_t* modelPathName)
{
	Model* newModel = new Model();
	if(newModel->LoadFromFile(modelPathName, device_))
	{
		models_.push_back(std::unique_ptr<Model>(newModel));
		return newModel;
	}

	return NULL;
}

ModelInstance* RenderManager::CreateModelInstance(Model* model, const Matrix4x4& instanceToWorldMatrix)
{
	ModelInstance* newModelInstance = new ModelInstance(model, instanceToWorldMatrix);
	modelInstances_.push_back(std::unique_ptr<ModelInstance>(newModelInstance));

	return newModelInstance;
}

void RenderManager::Step()
{
	TextureManager::GetInstance().Step();
	FontManager::GetInstance().Step();

	// Just clear the backbuffer
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha
    deviceContext_->ClearRenderTargetView( renderTargetView_, ClearColor );

	// Clear the depth buffer to 1.0 (max depth)
	deviceContext_->ClearDepthStencilView( depthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	// Draw all of the registered mesh instances using the registered camera and light(s)
	
	// Setup the world_to_view and view_to_projection matrices based on the current camera (& other settings)
	assert(currentCamera_ != NULL);
	Matrix4x4 worldToViewMatrix = currentCamera_->BuildWorldToViewMatrix();
	Matrix4x4 viewToProjectionMatrix = BuildViewToProjectionMatrix( PiOverFour, aspectRatio_, 0.1f, 10000.0f );
	Matrix4x4 worldToProjectionMatrix = worldToViewMatrix * viewToProjectionMatrix;

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
		{
			std::wofstream& loggingStream = DiagnosticsManager::GetInstance().GetLoggingStream();
			loggingStream << "The D3D11 device and swap chain were successfully created." << std::endl;
			loggingStream << "The driver type is " << driverType_ << "." << std::endl;
			loggingStream << "The feature level is 0x" << std::hex << featureLevel_ << std::dec << "." << std::endl;

            return hr;
		}
    }

	DiagnosticsManager::GetInstance().ReportError(hr, L"Error during creation of device and swap chain");
    
	return hr;
}

HRESULT RenderManager::SetupRenderTargetView()
{
	ID3D11Texture2D* backBuffer = NULL;
    HRESULT hr = swapChain_->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&backBuffer );
    if( FAILED( hr ) )
	{
		DiagnosticsManager::GetInstance().ReportError(hr, L"Error during retrieval of Swap Chain back buffer");
        return hr;
	}

    hr = device_->CreateRenderTargetView( backBuffer, NULL, &renderTargetView_ );
    backBuffer->Release();
    if( FAILED( hr ) )
	{
		DiagnosticsManager::GetInstance().ReportError(hr, L"Error during creation of Render Target View");
        return hr;
	}
	DiagnosticsManager::GetInstance().GetLoggingStream() << "Successfully created Render Target View" << std::endl;

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
	{
		DiagnosticsManager::GetInstance().ReportError(hr, L"Error during creation of Depth Stencil Texture");
        return hr;
	}
	DiagnosticsManager::GetInstance().GetLoggingStream() << "Successfully created Depth Stencil Texture" << std::endl;

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory( &descDSV, sizeof(descDSV) );
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = device_->CreateDepthStencilView( depthStencil_, &descDSV, &depthStencilView_ );
    if( FAILED( hr ) )
	{
		DiagnosticsManager::GetInstance().ReportError(hr, L"Error during creation of Depth Stencil View");
        return hr;
	}
	DiagnosticsManager::GetInstance().GetLoggingStream() << "Successfully created Depth Stencil View" << std::endl;

    deviceContext_->OMSetRenderTargets( 1, &renderTargetView_, depthStencilView_ );

	return S_OK;
}

void RenderManager::SetupViewport()
{
	D3D11_VIEWPORT viewport;
    viewport.Width = static_cast<FLOAT>(outputWidth_);
    viewport.Height = static_cast<FLOAT>(outputHeight_);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    deviceContext_->RSSetViewports( 1, &viewport );

	DiagnosticsManager::GetInstance().GetLoggingStream() << "Successfully set Device Context viewport" << std::endl;
}

HRESULT RenderManager::SetupSurfaceFormats()
{
	HRESULT hr = UntexturedSurfaceFormat::GetInstance().Initialize(device_);
	if( FAILED(hr) )
		return hr;
	DiagnosticsManager::GetInstance().GetLoggingStream() << "Successfully setup Untextured Surface Format" << std::endl;

	hr = DiffuseOnlySurfaceFormat::GetInstance().Initialize(device_);
	if( FAILED(hr) )
		return hr;
	DiagnosticsManager::GetInstance().GetLoggingStream() << "Successfully setup Diffuse Only Surface Format" << std::endl;

	return S_OK;
}

Matrix4x4 RenderManager::BuildViewToProjectionMatrix(
	float fovAngle, float aspectRatio, float nearClipZ, float farClipZ) const
{
	float halfFovAngle = fovAngle * 0.5f;
	float sinFov = sin(halfFovAngle);
	float cosFov = cos(halfFovAngle);
	float height = cosFov / sinFov;
	float width = height / aspectRatio;
	float m33 = farClipZ / (farClipZ - nearClipZ);

	return Matrix4x4(
		width,   0.0f, 0.0f,             0.0f,
		 0.0f, height, 0.0f,             0.0f,
		 0.0f,   0.0f,  m33,             1.0f,
		 0.0f,   0.0f, -m33 * nearClipZ, 1.0f);
}