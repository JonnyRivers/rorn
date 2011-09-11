#pragma once

#include <cassert>
#include <vector>

#include <d3dcompiler.h>
#include <d3dx11async.h>

#include "../../../Exceptions/initialisation_exception.h"

#include "../../../../Shared/ErrorCodes/HResultFormatter.h"

#include "../Material/VertexFormatFlags.h"

#include "D3DGraphicsDevice.h"

using namespace Rorn::Engine;

D3DGraphicsDevice::D3DGraphicsDevice(HWND applicationWindowHandle, IDiagnostics* diagnostics) 
	: diagnostics_(diagnostics)
{
	diagnostics_->GetLoggingStream() << "D3DGraphicsDevice instance is being created." << std::endl;

	RECT rc;
    GetClientRect( applicationWindowHandle, &rc );
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
                                            D3D11_SDK_VERSION, &sd, &swapChain_, &device_, &featureLevel, &deviceContext_ );
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

#ifdef _DEBUG
	device_->QueryInterface<ID3D11Debug>(&debug_);
#endif

	ID3D11Texture2D* backBuffer = NULL;
    hr = swapChain_->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&backBuffer );
    if( FAILED( hr ) )
	{
        diagnostics->GetLoggingStream() << "Unable to retrieve swap chain back buffer.  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
		throw initialisation_exception("Unable to retrieve swap chain back buffer.");
	}

    hr = device_->CreateRenderTargetView( backBuffer, NULL, &renderTargetView_ );
    backBuffer->Release();
    if( FAILED( hr ) )
	{
		diagnostics->GetLoggingStream() << "Unable to create Render Target View.  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
		throw initialisation_exception("Unable to create Render Target View.");
	}
	diagnostics->GetLoggingStream() << "Successfully created Render Target View" << std::endl;

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
		diagnostics->GetLoggingStream() << "Unable to create Depth Stencil Texture.  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
		throw initialisation_exception("Unable to create Depth Stencil Texture.");
	}
	diagnostics->GetLoggingStream() << "Successfully created Depth Stencil Texture" << std::endl;

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory( &descDSV, sizeof(descDSV) );
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = device_->CreateDepthStencilView( depthStencil_, &descDSV, &depthStencilView_ );
    if( FAILED( hr ) )
	{
		diagnostics->GetLoggingStream() << "Unable to create Depth Stencil View.  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
		throw initialisation_exception("Unable to create Depth Stencil View.");
	}
	diagnostics->GetLoggingStream() << "Successfully created Depth Stencil View" << std::endl;

    deviceContext_->OMSetRenderTargets( 1, &renderTargetView_, depthStencilView_ );

	D3D11_BLEND_DESC blendStateDescription;
	memset(&blendStateDescription, 0, sizeof(blendStateDescription));
    blendStateDescription.RenderTarget[0].BlendEnable = true;
    blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = device_->CreateBlendState(&blendStateDescription, &blendState_);
	if( FAILED( hr ) )
	{
		diagnostics->GetLoggingStream() << "Unable to create Blend State.  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
		throw initialisation_exception("Unable to create Blend State.");
	}
	diagnostics->GetLoggingStream() << "Successfully created Blend State" << std::endl;

	float blendFactor[] = { 1.0f,1.0f,1.0f,1.0f };
    UINT sampleMask   = 0xffffffff;
    deviceContext_->OMSetBlendState(blendState_, blendFactor, sampleMask);

	D3D11_VIEWPORT viewport;
    viewport.Width = static_cast<FLOAT>(outputWidth_);
    viewport.Height = static_cast<FLOAT>(outputHeight_);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    deviceContext_->RSSetViewports( 1, &viewport );

	diagnostics->GetLoggingStream() << "Successfully set Device Context viewport" << std::endl;

	diagnostics_->GetLoggingStream() << "D3DGraphicsDevice instance was created successfully." << std::endl;
}

D3DGraphicsDevice::~D3DGraphicsDevice()
{
	diagnostics_->GetLoggingStream() << "D3DGraphicsDevice instance is being destroyed." << std::endl;

	vertexShaders_.clear();
	pixelShaders_.clear();
	constantBuffers_.clear();
	textures_.clear();
	vertexBuffers_.clear();
	indexBuffers_.clear();

	if( blendState_ != NULL )
		blendState_->Release();

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
	
#ifdef _DEBUG
	if( debug_ != NULL )
		debug_->Release();
#endif

    if( device_ != NULL )
		device_->Release();
}

ID3DBlob* D3DGraphicsDevice::CompileShaderFromFile(const wchar_t* fileName, const char* entryPoint, const char* shaderModel)
{
	ID3DBlob* result;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
    ID3DBlob* pErrorBlob;
    HRESULT hr = D3DX11CompileFromFile( fileName, NULL, NULL, entryPoint, shaderModel, 
        dwShaderFlags, 0, NULL, &result, &pErrorBlob, NULL );
    if( FAILED(hr) )
    {
        if( pErrorBlob != NULL )
		{
			diagnostics_->GetLoggingStream() << "Error during compilation of shader '" << entryPoint << "' in file '" << fileName << 
				"' using shader model '" << shaderModel << "'." << std::endl;
			diagnostics_->GetLoggingStream() << "Reason: " << static_cast<const char*>(pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
		}

		throw initialisation_exception("Unable to compile shader.");
    }

    if( pErrorBlob != NULL ) 
		pErrorBlob->Release();

	diagnostics_->GetLoggingStream() << "Successfully compiled shader '" << entryPoint << "' in file '" << fileName << "' using shader model '" << 
		shaderModel << "'." << std::endl;

    return result;
}

/*virtual*/ unsigned int D3DGraphicsDevice::CreateVertexShader(const wchar_t* shaderFilename, unsigned int vertexFormatFlags)
{
	ID3DBlob* pVSBlob = CompileShaderFromFile( shaderFilename, "VS", "vs_4_0" );
	
	ID3D11VertexShader* vertexShader;
	HRESULT hr = device_->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &vertexShader );
    if( FAILED( hr ) )
	{
		pVSBlob->Release();
		diagnostics_->GetLoggingStream() << "Unable to create vertex shader from data blob" << std::endl;
        throw initialisation_exception("Unable to create vertex shader from data blob.");
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;
	int vertexPositionStart = 0;
	if((vertexFormatFlags & VertexFormatFlags::Position) == VertexFormatFlags::Position )
	{
		D3D11_INPUT_ELEMENT_DESC positionInputDesc = { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, vertexPositionStart, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		vertexPositionStart += 16;
		inputElements.push_back(positionInputDesc);
	}
	if((vertexFormatFlags & VertexFormatFlags::Normal) == VertexFormatFlags::Normal )
	{
		D3D11_INPUT_ELEMENT_DESC normalInputDesc = { "NORMAL",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, vertexPositionStart, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		vertexPositionStart += 16;
		inputElements.push_back(normalInputDesc);
	}
	if((vertexFormatFlags & VertexFormatFlags::UV0) == VertexFormatFlags::UV0 )
	{
		D3D11_INPUT_ELEMENT_DESC uv0InputDesc = { "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, vertexPositionStart, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		vertexPositionStart += 8;
		inputElements.push_back(uv0InputDesc);
	}

	ID3D11InputLayout* vertexLayout;
	hr = device_->CreateInputLayout( &inputElements[0], static_cast<UINT>(inputElements.size()), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &vertexLayout );
	if( FAILED( hr ) )
	{
		pVSBlob->Release();
		diagnostics_->GetLoggingStream() << "Unable to create vertex input layout from data blob" << std::endl;
        throw initialisation_exception("Unable to create vertex input layout from data blob.");
	}

	pVSBlob->Release();

	unsigned int newId = static_cast<unsigned int>(vertexShaders_.size());
	vertexShaders_.insert(
		std::make_pair<unsigned int, std::unique_ptr<D3DVertexShader>>(
			newId, std::unique_ptr<D3DVertexShader>(new D3DVertexShader(vertexShader, vertexLayout))));

	return newId;
}

/*virtual*/ unsigned int D3DGraphicsDevice::CreatePixelShader(const wchar_t* shaderFilename)
{
	ID3DBlob* pPSBlob = CompileShaderFromFile( shaderFilename, "PS", "ps_4_0" );

	ID3D11PixelShader* pixelShader;
	HRESULT hr = device_->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &pixelShader );
	pPSBlob->Release();
    if( FAILED( hr ) )
	{
		diagnostics_->GetLoggingStream() << "Unable to create pixel shader from data blob" << std::endl;
        throw initialisation_exception("Unable to create pixel shader from data blob.");
	}

	unsigned int newId = static_cast<unsigned int>(pixelShaders_.size());
	pixelShaders_.insert(
		std::make_pair<unsigned int, std::unique_ptr<D3DPixelShader>>(
			newId, std::unique_ptr<D3DPixelShader>(new D3DPixelShader(pixelShader))));

	return newId;
}

/*virtual*/ unsigned int D3DGraphicsDevice::CreateSamplerState()
{
	D3D11_SAMPLER_DESC samplerDescription;
    ZeroMemory( &samplerDescription, sizeof(samplerDescription) );
    samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDescription.MinLOD = 0;
    samplerDescription.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState;
    HRESULT hr = device_->CreateSamplerState( &samplerDescription, &samplerState );
    if( FAILED( hr ) )
        return hr;

	unsigned int newId = static_cast<unsigned int>(samplerStates_.size());
	samplerStates_.insert(
		std::make_pair<unsigned int, std::unique_ptr<D3DSamplerState>>(
			newId, std::unique_ptr<D3DSamplerState>(new D3DSamplerState(samplerState))));

	return newId;
}

/*virtual*/ unsigned int D3DGraphicsDevice::CreateConstantBuffer(unsigned int dataSize)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = dataSize;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	ID3D11Buffer* constantBuffer;
    HRESULT hr = device_->CreateBuffer( &bd, NULL, &constantBuffer );
    if( FAILED( hr ) )
	{
		diagnostics_->GetLoggingStream() << "Unable to create constant buffer" << std::endl;
        throw initialisation_exception("Unable to create constant buffer.");
	}

	unsigned int newId = static_cast<unsigned int>(constantBuffers_.size());
	constantBuffers_.insert(
		std::make_pair<unsigned int, std::unique_ptr<D3DConstantBuffer>>(
			newId, std::unique_ptr<D3DConstantBuffer>(new D3DConstantBuffer(constantBuffer))));

	return newId;
}

/*virtual*/ unsigned int D3DGraphicsDevice::CreateTexture(const void* data, unsigned int dataSize)
{
	ID3D11ShaderResourceView* texture;

	HRESULT hr = D3DX11CreateShaderResourceViewFromMemory(
		device_,
		data,
		dataSize,
		NULL,
		NULL,
		&texture,
		NULL);

	if( FAILED( hr ) )
	{
		diagnostics_->GetLoggingStream() << "Unable to create texture." << std::endl;
        throw initialisation_exception("Unable to create texture.");
	}

	unsigned int newId = static_cast<unsigned int>(textures_.size());
	textures_.insert(
		std::make_pair<unsigned int, std::unique_ptr<D3DTexture>>(
			newId, std::unique_ptr<D3DTexture>(new D3DTexture(texture))));

	return newId;
}

/*virtual*/ unsigned int D3DGraphicsDevice::CreateVertexBuffer(const void* data, unsigned int dataSize)
{
	D3D11_BUFFER_DESC vertexBufferDescription;
	ZeroMemory( &vertexBufferDescription, sizeof(vertexBufferDescription) );
	vertexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDescription.ByteWidth = dataSize;
	vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescription.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA vertexResourceData;
	ZeroMemory( &vertexResourceData, sizeof(vertexResourceData) );
    vertexResourceData.pSysMem = data;
	ID3D11Buffer* vertexBuffer;
	HRESULT hr = device_->CreateBuffer( &vertexBufferDescription, &vertexResourceData, &vertexBuffer );
    if( FAILED( hr ) )
	{
		diagnostics_->GetLoggingStream() << "Unable to create vertex buffer" << std::endl;
        throw initialisation_exception("Unable to create vertex buffer.");
	}

	unsigned int newId = static_cast<unsigned int>(vertexBuffers_.size());
	vertexBuffers_.insert(
		std::make_pair<unsigned int, std::unique_ptr<D3DVertexBuffer>>(
			newId, std::unique_ptr<D3DVertexBuffer>(new D3DVertexBuffer(vertexBuffer))));

	return newId;
}

/*virtual*/ unsigned int D3DGraphicsDevice::CreateIndexBuffer(const void* data, unsigned int dataSize)
{
	D3D11_BUFFER_DESC indexBufferDescription;
	ZeroMemory( &indexBufferDescription, sizeof(indexBufferDescription) );
	indexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDescription.ByteWidth = dataSize;
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescription.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA indexResourceData;
	ZeroMemory( &indexResourceData, sizeof(indexResourceData) );
    indexResourceData.pSysMem = data;
	ID3D11Buffer* indexBuffer;
	HRESULT hr = device_->CreateBuffer( &indexBufferDescription, &indexResourceData, &indexBuffer );
    if( FAILED( hr ) )
	{
		diagnostics_->GetLoggingStream() << "Unable to create index buffer" << std::endl;
        throw initialisation_exception("Unable to create index buffer.");
	}

	unsigned int newId = static_cast<unsigned int>(indexBuffers_.size());
	indexBuffers_.insert(
		std::make_pair<unsigned int, std::unique_ptr<D3DIndexBuffer>>(
			newId, std::unique_ptr<D3DIndexBuffer>(new D3DIndexBuffer(indexBuffer))));

	return newId;
}

/*virtual*/ void D3DGraphicsDevice::UpdateConstantBufferData(unsigned int constantBufferId, const void* data)
{
	std::map<unsigned int, std::unique_ptr<D3DConstantBuffer>>::const_iterator constantBufferIter = constantBuffers_.find(constantBufferId);
	assert(constantBufferIter != constantBuffers_.end());
	D3DConstantBuffer* constantBuffer = constantBufferIter->second.get();

	deviceContext_->UpdateSubresource( constantBuffer->GetUnderlyingBuffer(), 0, NULL, data, 0, 0 );
}

/*virtual*/ void D3DGraphicsDevice::UpdateVertexBufferData(unsigned int vertexBufferId, const void* data)
{
	std::map<unsigned int, std::unique_ptr<D3DVertexBuffer>>::const_iterator vertexBufferIter = vertexBuffers_.find(vertexBufferId);
	assert(vertexBufferIter != vertexBuffers_.end());
	D3DVertexBuffer* vertexBuffer = vertexBufferIter->second.get();

	deviceContext_->UpdateSubresource( vertexBuffer->GetUnderlyingBuffer(), 0, NULL, data, 0, 0 );
}

/*virtual*/ void D3DGraphicsDevice::UpdateIndexBufferData(unsigned int indexBufferId, const void* data)
{
	std::map<unsigned int, std::unique_ptr<D3DIndexBuffer>>::const_iterator indexBufferIter = indexBuffers_.find(indexBufferId);
	assert(indexBufferIter != indexBuffers_.end());
	D3DIndexBuffer* indexBuffer = indexBufferIter->second.get();

	deviceContext_->UpdateSubresource( indexBuffer->GetUnderlyingBuffer(), 0, NULL, data, 0, 0 );
}

/*virtual*/ void D3DGraphicsDevice::SetVertexShader(unsigned int vertexShaderId)
{
	std::map<unsigned int, std::unique_ptr<D3DVertexShader>>::const_iterator vertexShaderIter = vertexShaders_.find(vertexShaderId);
	assert(vertexShaderIter != vertexShaders_.end());
	D3DVertexShader* vertexShader = vertexShaderIter->second.get();

	deviceContext_->IASetInputLayout( vertexShader->GetUnderlyingInputLayout() );
	deviceContext_->VSSetShader( vertexShader->GetUnderlyingShader(), NULL, 0 );
}

/*virtual*/ void D3DGraphicsDevice::SetVertexShaderConstantBuffer(unsigned int constantBufferId)
{
	std::map<unsigned int, std::unique_ptr<D3DConstantBuffer>>::const_iterator constantBufferIter = constantBuffers_.find(constantBufferId);
	assert(constantBufferIter != constantBuffers_.end());
	D3DConstantBuffer* constantBuffer = constantBufferIter->second.get();
	ID3D11Buffer* underlyingBuffer = constantBuffer->GetUnderlyingBuffer();

	deviceContext_->VSSetConstantBuffers( 0, 1, &underlyingBuffer );
}

/*virtual*/ void D3DGraphicsDevice::SetPixelShader(unsigned int pixelShaderId)
{
	std::map<unsigned int, std::unique_ptr<D3DPixelShader>>::const_iterator pixelShaderIter = pixelShaders_.find(pixelShaderId);
	assert(pixelShaderIter != pixelShaders_.end());
	D3DPixelShader* pixelShader = pixelShaderIter->second.get();

	deviceContext_->PSSetShader( pixelShader->GetUnderlyingShader(), NULL, 0 );
}

/*virtual*/ void D3DGraphicsDevice::SetPixelShaderConstantBuffer(unsigned int constantBufferId)
{
	std::map<unsigned int, std::unique_ptr<D3DConstantBuffer>>::const_iterator constantBufferIter = constantBuffers_.find(constantBufferId);
	assert(constantBufferIter != constantBuffers_.end());
	D3DConstantBuffer* constantBuffer = constantBufferIter->second.get();
	ID3D11Buffer* underlyingBuffer = constantBuffer->GetUnderlyingBuffer();

	deviceContext_->PSSetConstantBuffers( 0, 1, &underlyingBuffer );
}

/*virtual*/ void D3DGraphicsDevice::SetSamplerState(unsigned int samplerStateId)
{
	std::map<unsigned int, std::unique_ptr<D3DSamplerState>>::const_iterator samplerStateIter = samplerStates_.find(samplerStateId);
	assert(samplerStateIter != samplerStates_.end());
	D3DSamplerState* samplerState = samplerStateIter->second.get();
	ID3D11SamplerState* underlyingSamplerState = samplerState->GetUnderlyingSamplerState();

	deviceContext_->PSSetSamplers( 0, 1, &underlyingSamplerState );
}

/*virtual*/ void D3DGraphicsDevice::SetVertexBuffer(unsigned int vertexBufferId, unsigned int vertexStride)
{
	std::map<unsigned int, std::unique_ptr<D3DVertexBuffer>>::const_iterator vertexBufferIter = vertexBuffers_.find(vertexBufferId);
	assert(vertexBufferIter != vertexBuffers_.end());
	D3DVertexBuffer* vertexBuffer = vertexBufferIter->second.get();
	ID3D11Buffer* underlyingBuffer = vertexBuffer->GetUnderlyingBuffer();

	UINT offset = 0;
	deviceContext_->IASetVertexBuffers(0, 1, &underlyingBuffer, &vertexStride, &offset);
}

/*virtual*/ void D3DGraphicsDevice::SetIndexBuffer(unsigned int indexBufferId)
{
	std::map<unsigned int, std::unique_ptr<D3DIndexBuffer>>::const_iterator indexBufferIter = indexBuffers_.find(indexBufferId);
	assert(indexBufferIter != indexBuffers_.end());
	D3DIndexBuffer* indexBuffer = indexBufferIter->second.get();
	ID3D11Buffer* underlyingBuffer = indexBuffer->GetUnderlyingBuffer();

	deviceContext_->IASetIndexBuffer( underlyingBuffer, DXGI_FORMAT_R32_UINT, 0 );
}

/*virtual*/ void D3DGraphicsDevice::SetTexture(unsigned int textureId)
{
	std::map<unsigned int, std::unique_ptr<D3DTexture>>::const_iterator textureIter = textures_.find(textureId);
	assert(textureIter != textures_.end());
	D3DTexture* texture = textureIter->second.get();
	ID3D11ShaderResourceView* underlyingTexture = texture->GetUnderlyingTexture();

	deviceContext_->PSSetShaderResources( 0, 1, &underlyingTexture);
}

/*virtual*/ unsigned int D3DGraphicsDevice::GetOutputWidth() const
{
	return outputWidth_;
}

/*virtual*/ unsigned int D3DGraphicsDevice::GetOutputHeight() const
{
	return outputHeight_;
}

/*virtual*/ float D3DGraphicsDevice::GetOutputAspectRatio() const
{
	return aspectRatio_;
}

/*virtual*/ void D3DGraphicsDevice::ClearView(const Rorn::Maths::Float4& backColour)
{
	float clearColor[4] = { backColour.X, backColour.Y, backColour.Z, backColour.W };
	deviceContext_->ClearRenderTargetView( renderTargetView_, clearColor );
	deviceContext_->ClearDepthStencilView( depthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

/*virtual*/ void D3DGraphicsDevice::DrawIndexed(unsigned int numIndices)
{
	deviceContext_->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	deviceContext_->DrawIndexed(numIndices, 0, 0);
}

/*virtual*/ void D3DGraphicsDevice::Present()
{
	swapChain_->Present( 0, 0 );
}
