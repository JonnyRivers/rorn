#pragma once

#include <cassert>
#include <vector>

#include "../../Exceptions/initialisation_exception.h"
#include "../../Exceptions/font_load_exception.h"
#include "../../Exceptions/model_load_exception.h"

#include "../../../Shared/RornMaths/BoundingBox.h"
#include "../../../Shared/RornMaths/Constants.h"
#include "../../../Shared/RornMaths/Float2.h"
#include "../../../Shared/RornMaths/Float4.h"

#include "DirectXRenderer.h"

#include "GraphicsDevice/D3DGraphicsDevice.h"

#include "Material/ConstantBufferTypes.h"
#include "Material/VertexFormatFlags.h"

#include "Model/BlitVertexFormat.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

DirectXRenderer::DirectXRenderer(HWND applicationWindowHandle, IDiagnostics* diagnostics, IFileSystem* fileSystem) 
	: diagnostics_(diagnostics), fileSystem_(fileSystem)
{
	diagnostics_->GetLoggingStream() << "Renderer instance is being created." << std::endl;

	graphicsDevice_ = new D3DGraphicsDevice(applicationWindowHandle, diagnostics);

	Material* untexturedMaterial = new Material(diagnostics, graphicsDevice_, 
		L"Untextured.fx", VertexFormatFlags::Position | VertexFormatFlags::Normal, sizeof(LitGeometryConstantBuffer));
	materials_[MaterialType::Untextured] = std::unique_ptr<Material>(untexturedMaterial);

	Material* diffuseOnlyMaterial = new Material(diagnostics, graphicsDevice_, 
		L"DiffuseOnly.fx", VertexFormatFlags::Position | VertexFormatFlags::Normal | VertexFormatFlags::UV0, sizeof(LitGeometryConstantBuffer));
	materials_[MaterialType::DiffuseOnly] = std::unique_ptr<Material>(diffuseOnlyMaterial);

	Material* blitMaterial = new Material(diagnostics, graphicsDevice_, 
		L"Blit.fx", VertexFormatFlags::Position | VertexFormatFlags::UV0, 0);
	materials_[MaterialType::Blit] = std::unique_ptr<Material>(blitMaterial);

	debugTextFontId_ = LoadFont(L"debug.font");

	debugTextRenderCommandIds_.resize(maxNumDebugTextRenderCommands_);
	for(std::vector<unsigned int>::iterator debugTextRenderCommandIter = debugTextRenderCommandIds_.begin() ; debugTextRenderCommandIter != debugTextRenderCommandIds_.end() ; ++debugTextRenderCommandIter)
	{
		*debugTextRenderCommandIter = CreateDebugTextRenderCommand();
	}

	diagnostics_->GetLoggingStream() << "Renderer instance was created successfully." << std::endl;
}

DirectXRenderer::~DirectXRenderer()
{
	diagnostics_->GetLoggingStream() << "Renderer instance is being destroyed." << std::endl;

	delete graphicsDevice_;
}

/*static*/ Matrix4x4 DirectXRenderer::BuildViewToProjectionMatrix(float fovAngle, float aspectRatio, float nearClipZ, float farClipZ)
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

void DirectXRenderer::StartFrame()
{
	debugTextItemsThisFrame_ = 0;
	debugRenderCommandsIdsToDraw_.clear();
}

void DirectXRenderer::Draw()
{
	graphicsDevice_->ClearView(Rorn::Maths::Float4(0.0f, 0.125f, 0.3f, 1.0f));

	Matrix4x4 worldToViewMatrix = cameras_[currentCameraId_]->BuildWorldToViewMatrix();
	Matrix4x4 viewToProjectionMatrix = BuildViewToProjectionMatrix( PiOverFour, graphicsDevice_->GetOutputAspectRatio(), 0.1f, 10000.0f );
	Matrix4x4 worldToProjectionMatrix = worldToViewMatrix * viewToProjectionMatrix;

	std::map<unsigned int, std::unique_ptr<ModelInstance>>::const_iterator modelInstanceIter;
	for(modelInstanceIter = modelInstances_.begin() ; modelInstanceIter != modelInstances_.end() ; ++modelInstanceIter)
	{
		DrawModelInstance(modelInstanceIter->first, worldToViewMatrix, worldToProjectionMatrix);
	}

	std::vector<unsigned int>::const_iterator debugRenderCommandIter;
	for(debugRenderCommandIter = debugRenderCommandsIdsToDraw_.begin() ; debugRenderCommandIter != debugRenderCommandsIdsToDraw_.end() ; ++debugRenderCommandIter)
	{
		DrawRenderCommand(*debugRenderCommandIter, Matrix4x4(), Matrix4x4(), Matrix4x4());
	}

	graphicsDevice_->Present();
}

// Model interface

/*virtual*/ unsigned int DirectXRenderer::LoadModel(const wchar_t* modelPathName)
{
	// This is too long and needs to be refactored further
	StreamReader fileReader = fileSystem_->OpenRead(modelPathName);

	char fileIdentifierBuffer[9];
	fileReader.ReadData(fileIdentifierBuffer, 8);
	fileIdentifierBuffer[8] = '\0';
	if(strcmp("RORNMODL", fileIdentifierBuffer) != 0)
	{
		throw model_load_exception("Attempt to load model with invalid file identifier, 'RORNMODL' was expected");
	}
	unsigned int versionNumber = fileReader.ReadUInt();
	if( versionNumber != 1)
	{
		throw model_load_exception("Attempt to load model with invalid version number, 1 was expected");
	}
	float bbMinX = fileReader.ReadFloat();
	float bbMinY = fileReader.ReadFloat();
	float bbMinZ = fileReader.ReadFloat();
	float bbMaxX = fileReader.ReadFloat();
	float bbMaxY = fileReader.ReadFloat();
	float bbMaxZ = fileReader.ReadFloat();
	Maths::BoundingBox boundingBox = Maths::BoundingBox(bbMinX, bbMinY, bbMinZ, bbMaxX, bbMaxY, bbMaxZ);
	unsigned int numCompiledTextures = fileReader.ReadUInt();
	unsigned int numRenderCommands = fileReader.ReadUInt();

	std::map<unsigned int, unsigned int> textureIdMap;// map from this model's texture index to the engine's texture id

	std::vector<unsigned int> createdTextureIds(numCompiledTextures);
	for(unsigned int compiledTextureIndex = 0 ; compiledTextureIndex != numCompiledTextures ; ++compiledTextureIndex)
	{
		unsigned int modelTextureId = fileReader.ReadUInt();
		unsigned int compiledTextureDataLength = fileReader.ReadUInt();
		unsigned char* compiledTextureData = new unsigned char[compiledTextureDataLength];
		fileReader.ReadData(compiledTextureData, compiledTextureDataLength);
		unsigned int deviceTextureId = graphicsDevice_->CreateTexture(compiledTextureData, compiledTextureDataLength);
		textureIdMap[modelTextureId] = deviceTextureId;// Store the model file->device mapping
		delete [] compiledTextureData;

		createdTextureIds[compiledTextureIndex] = deviceTextureId;
	}

	std::vector<unsigned int> renderCommandIds(numRenderCommands);
	for(int renderCommandIndex = 0 ; renderCommandIndex != numRenderCommands ; ++renderCommandIndex)
	{
		unsigned int materialType = fileReader.ReadUInt();

		// Could this be moved tools side?
		bool commandHasDiffuseMaterial = false;
		unsigned int vertexStride;
		if(materialType == 0)
		{
			vertexStride = 32;
		}
		else if(materialType == 1)
		{
			vertexStride = 40;
			commandHasDiffuseMaterial = true;
		}
		else
		{
			throw model_load_exception("Attempt to load model with invalid material type, a value between 0 and 1 was expected");
		}

		unsigned int geometryType = fileReader.ReadUInt();
		Rorn::Maths::Float4 ambientColour;
		fileReader.ReadFloat4(ambientColour);
		Rorn::Maths::Float4 diffuseColour;
		fileReader.ReadFloat4(diffuseColour);
		Rorn::Maths::Float4 specularColour;
		fileReader.ReadFloat4(specularColour);
		float phongExponent = fileReader.ReadFloat();

		unsigned int deviceDiffuseTextureId;
		if(commandHasDiffuseMaterial)
		{
			unsigned int modelDiffuseTextureId = fileReader.ReadUInt();
			deviceDiffuseTextureId = textureIdMap[modelDiffuseTextureId];
		}

		unsigned int numVertices = fileReader.ReadUInt();
		unsigned int vertexDataSize = numVertices * vertexStride;
		unsigned char* vertexData = new unsigned char[vertexDataSize];
		fileReader.ReadData(vertexData, vertexDataSize);
		unsigned int vertexBufferId = graphicsDevice_->CreateVertexBuffer(vertexData, vertexDataSize);
		delete [] vertexData;
		
		unsigned int numIndices = fileReader.ReadUInt();
		unsigned int indexDataSize = numIndices * sizeof(unsigned int);
		unsigned char* indexData = new unsigned char[indexDataSize];
		fileReader.ReadData(indexData, indexDataSize);
		unsigned int indexBufferId = graphicsDevice_->CreateIndexBuffer(indexData, indexDataSize);
		delete [] indexData;

		unsigned int newId = renderCommands_.size();
		RenderCommand* newRenderCommand = new RenderCommand(
			static_cast<MaterialType::Type>(materialType), geometryType, ambientColour, diffuseColour, specularColour, phongExponent, deviceDiffuseTextureId, vertexBufferId, indexBufferId, vertexStride, numIndices);
		renderCommands_.insert(std::make_pair<unsigned int, std::unique_ptr<RenderCommand>>(newId, std::unique_ptr<RenderCommand>(newRenderCommand)));

		renderCommandIds[renderCommandIndex] = newId;
	}

	unsigned int newId = models_.size();
	Model* newModel = new Model(boundingBox, createdTextureIds, renderCommandIds);
	models_.insert(std::make_pair<unsigned int, std::unique_ptr<Model>>(newId, std::unique_ptr<Model>(newModel)));

	return newId;
}

/*virtual*/ IModel* DirectXRenderer::GetModel(unsigned int modelId)
{
	return models_[modelId].get();
}

/*virtual*/ unsigned int DirectXRenderer::CreateModelInstance(unsigned int modelId, const Rorn::Maths::Matrix4x4& instanceToWorldMatrix)
{
	unsigned int newId = models_.size();
	ModelInstance* newModel = new ModelInstance(modelId, instanceToWorldMatrix);
	modelInstances_.insert(std::make_pair<unsigned int, std::unique_ptr<ModelInstance>>(newId, std::unique_ptr<ModelInstance>(newModel)));

	return newId;
}

/*virtual*/ IModelInstance* DirectXRenderer::GetModelInstance(unsigned int modelInstanceId)
{
	return modelInstances_[modelInstanceId].get();
}

// Camera interface
/*virtual*/ unsigned int DirectXRenderer::CreateFreeCamera(const Rorn::Maths::Vector3& position, const Rorn::Maths::EulerAngles& eulerAngles)
{
	unsigned int newId = cameras_.size();
	Camera* newCamera = new FreeCamera(position, eulerAngles);
	cameras_.insert(std::make_pair<unsigned int, std::unique_ptr<Camera>>(newId, std::unique_ptr<Camera>(newCamera)));

	return newId;
}

/*virtual*/ void DirectXRenderer::SetCurrentCamera(unsigned int cameraId)
{
	currentCameraId_ = cameraId;
}

/*virtual*/ ICamera* DirectXRenderer::GetCamera(unsigned int cameraId)
{
	return cameras_[cameraId].get();
}

// Light interface
/*virtual*/ void DirectXRenderer::SetAmbientLight(const Rorn::Maths::Float4& colour)
{
	ambientLight_.SetColour(colour);
}

/*virtual*/ void DirectXRenderer::SetMainLight(const Rorn::Maths::Vector3& direction, const Rorn::Maths::Float4& colour)
{
	mainLight_.SetDirection(direction);
	mainLight_.SetColour(colour);
}

/*virtual*/ unsigned int DirectXRenderer::CreatePointLight(const Rorn::Maths::Vector3& position, const Rorn::Maths::Float4& colour, float luminosity)
{
	unsigned int newId = pointLights_.size();
	PointLight* newPointLight = new PointLight(position, colour, luminosity);
	pointLights_.insert(std::make_pair<unsigned int, std::unique_ptr<PointLight>>(newId, std::unique_ptr<PointLight>(newPointLight)));

	return newId;
}

/*virtual*/ void DirectXRenderer::AddDebugText(const char* text, float x, float y)
{
	assert(debugTextItemsThisFrame_ < debugTextRenderCommandIds_.size());

	unsigned int nextAvailableDebugTextRenderCommandId = debugTextRenderCommandIds_[debugTextItemsThisFrame_];

	static BlitVertexFormat vertexData[maxNumDebugTextRenderCommandVerts_];// working buffer
	static unsigned int indexData[maxNumDebugTextRenderCommandVerts_];// working buffer

	Font* debugFont = fonts_[debugTextFontId_].get();

	unsigned int numCharacters = strlen(text);
	unsigned int vertexCount = numCharacters * 6;// 2 triangles per glyph

	int currentVertexIndex = 0;
	float currentX = x;
	float currentY = y;
	for(unsigned int characterIndex = 0 ; characterIndex < numCharacters ; ++characterIndex)
	{
		const Glyph& thisGlyph = debugFont->GetGlyph( text[characterIndex] );
		float glyphWidthPixels = static_cast<float>(thisGlyph.GetWidth());
		float glyphHeightPixels = static_cast<float>(thisGlyph.GetHeight());
		float glyphWidth = glyphWidthPixels / static_cast<float>(graphicsDevice_->GetOutputWidth()) * 2.0f;// from pixels to a -1 to 1 range
		float glyphHeight = glyphHeightPixels / static_cast<float>(graphicsDevice_->GetOutputHeight()) * 2.0f;// from pixels to a -1 to 1 range
		float leftPos = currentX;
		float topPos = currentY;
		float rightPos = leftPos + glyphWidth;
		float bottomPos = topPos - glyphHeight;

		// top left
		vertexData[currentVertexIndex].Position.X = leftPos;
		vertexData[currentVertexIndex].Position.Y = topPos;
		vertexData[currentVertexIndex].Position.Z = 0.0f;
		vertexData[currentVertexIndex].Position.W = 1.0f;
		vertexData[currentVertexIndex].DiffuseUV.X = thisGlyph.GetStartU();
		vertexData[currentVertexIndex].DiffuseUV.Y = thisGlyph.GetStartV();
		++currentVertexIndex;
		// top right
		vertexData[currentVertexIndex].Position.X = rightPos;
		vertexData[currentVertexIndex].Position.Y = topPos;
		vertexData[currentVertexIndex].Position.Z = 0.0f;
		vertexData[currentVertexIndex].Position.W = 1.0f;
		vertexData[currentVertexIndex].DiffuseUV.X = thisGlyph.GetEndU();
		vertexData[currentVertexIndex].DiffuseUV.Y = thisGlyph.GetStartV();
		++currentVertexIndex;
		// bottom right
		vertexData[currentVertexIndex].Position.X = rightPos;
		vertexData[currentVertexIndex].Position.Y = bottomPos;
		vertexData[currentVertexIndex].Position.Z = 0.0f;
		vertexData[currentVertexIndex].Position.W = 1.0f;
		vertexData[currentVertexIndex].DiffuseUV.X = thisGlyph.GetEndU();
		vertexData[currentVertexIndex].DiffuseUV.Y = thisGlyph.GetEndV();
		++currentVertexIndex;

		// bottom right
		vertexData[currentVertexIndex].Position.X = rightPos;
		vertexData[currentVertexIndex].Position.Y = bottomPos;
		vertexData[currentVertexIndex].Position.Z = 0.0f;
		vertexData[currentVertexIndex].Position.W = 1.0f;
		vertexData[currentVertexIndex].DiffuseUV.X = thisGlyph.GetEndU();
		vertexData[currentVertexIndex].DiffuseUV.Y = thisGlyph.GetEndV();
		++currentVertexIndex;
		// bottom left
		vertexData[currentVertexIndex].Position.X = leftPos;
		vertexData[currentVertexIndex].Position.Y = bottomPos;
		vertexData[currentVertexIndex].Position.Z = 0.0f;
		vertexData[currentVertexIndex].Position.W = 1.0f;
		vertexData[currentVertexIndex].DiffuseUV.X = thisGlyph.GetStartU();
		vertexData[currentVertexIndex].DiffuseUV.Y = thisGlyph.GetEndV();
		++currentVertexIndex;
		// top left
		vertexData[currentVertexIndex].Position.X = leftPos;
		vertexData[currentVertexIndex].Position.Y = topPos;
		vertexData[currentVertexIndex].Position.Z = 0.0f;
		vertexData[currentVertexIndex].Position.W = 1.0f;
		vertexData[currentVertexIndex].DiffuseUV.X = thisGlyph.GetStartU();
		vertexData[currentVertexIndex].DiffuseUV.Y = thisGlyph.GetStartV();
		++currentVertexIndex;

		currentX += glyphWidth;
	}

	int indexCount = vertexCount;
	for(int index = 0 ; index < indexCount ; ++index)
		indexData[index] = index;

	RenderCommand* nextAvailableDebugTextRenderCommand = renderCommands_[nextAvailableDebugTextRenderCommandId].get();
	graphicsDevice_->UpdateVertexBufferData(nextAvailableDebugTextRenderCommand->GetVertexBufferId(), vertexData);
	graphicsDevice_->UpdateIndexBufferData(nextAvailableDebugTextRenderCommand->GetIndexBufferId(), indexData);
	nextAvailableDebugTextRenderCommand->SetNumIndices(indexCount);

	debugRenderCommandsIdsToDraw_.push_back(nextAvailableDebugTextRenderCommandId);

	debugTextItemsThisFrame_++;
}

void DirectXRenderer::DrawModelInstance(unsigned int modelInstanceId, const Matrix4x4& worldToViewMatrix, const Matrix4x4& worldToProjectionMatrix) const
{
	std::map<unsigned int, std::unique_ptr<ModelInstance>>::const_iterator modelInstanceIter = modelInstances_.find(modelInstanceId);
	assert(modelInstanceIter != modelInstances_.end());

	DrawModel(modelInstanceIter->second->GetModelId(), modelInstanceIter->second->GetInstanceToWorldMatrix(), worldToViewMatrix, worldToProjectionMatrix);
}

void DirectXRenderer::DrawModel(unsigned int modelId, const Matrix4x4& instanceToWorldMatrix, const Matrix4x4& worldToViewMatrix, const Matrix4x4& worldToProjectionMatrix) const
{
	std::map<unsigned int, std::unique_ptr<Model>>::const_iterator modelIter = models_.find(modelId);
	assert(modelIter != models_.end());

	std::vector<unsigned int>::const_iterator renderCommandIdIter;
	for(renderCommandIdIter = modelIter->second->GetRenderCommandIds().begin() ; renderCommandIdIter != modelIter->second->GetRenderCommandIds().end() ; ++renderCommandIdIter)
	{
		DrawRenderCommand(*renderCommandIdIter, instanceToWorldMatrix, worldToViewMatrix, worldToProjectionMatrix);
	}
}

void DirectXRenderer::DrawRenderCommand(unsigned int renderCommandId, const Matrix4x4& instanceToWorldMatrix, const Matrix4x4& worldToViewMatrix, const Matrix4x4& worldToProjectionMatrix) const
{
	std::map<unsigned int, std::unique_ptr<RenderCommand>>::const_iterator renderCommandlIter = renderCommands_.find(renderCommandId);
	assert(renderCommandlIter != renderCommands_.end());
	RenderCommand* renderCommand = renderCommandlIter->second.get();

	std::map<MaterialType::Type, std::unique_ptr<Material>>::const_iterator materialIter = materials_.find(renderCommand->GetMaterialType());
	assert(materialIter != materials_.end());
	Material* material = materialIter->second.get();

	// Setup material data
	if( renderCommand->GetMaterialType() == MaterialType::Untextured ||
		renderCommand->GetMaterialType() == MaterialType::DiffuseOnly)
	{
		LitGeometryConstantBuffer constantBuffer;
		constantBuffer.ModelToWorldMatrix = Matrix4x4::Transpose( instanceToWorldMatrix );
		constantBuffer.WorldToProjectionMatrix = Matrix4x4::Transpose( worldToProjectionMatrix );
		constantBuffer.AmbientColor = renderCommand->GetAmbientColour();
		constantBuffer.DiffuseColor = renderCommand->GetDiffuseColour();
		constantBuffer.SpecularColor = renderCommand->GetSpecularColour();
		constantBuffer.PhongExponent = renderCommand->GetPhongExponent();
		constantBuffer.AmbientLightColor = ambientLight_.GetColour();
		constantBuffer.MainLightDir = mainLight_.GetDirection();
		constantBuffer.MainLightColor = mainLight_.GetColour();
		constantBuffer.EyeDir = Vector3(worldToViewMatrix.M31, worldToViewMatrix.M32, worldToViewMatrix.M33);
		constantBuffer.NumActivePointLights = pointLights_.size();

		std::map<unsigned int, std::unique_ptr<PointLight>>::const_iterator pointLightIter;
		unsigned int pointLightIndex = 0;
		for(pointLightIter = pointLights_.begin() ; pointLightIter != pointLights_.end() ; ++pointLightIter)
		{
			PointLight* pointLight = pointLightIter->second.get();
			constantBuffer.PointLightPositions[pointLightIndex].X = pointLight->GetPosition().X;
			constantBuffer.PointLightPositions[pointLightIndex].Y = pointLight->GetPosition().Y;
			constantBuffer.PointLightPositions[pointLightIndex].Z = pointLight->GetPosition().Z;
			constantBuffer.PointLightPositions[pointLightIndex].W = pointLight->GetLuminosity();
			constantBuffer.PointLightColors[pointLightIndex] = pointLight->GetColour();

			++pointLightIndex;
		}

		graphicsDevice_->UpdateConstantBufferData(material->GetConstantBufferId(), &constantBuffer);
	}

	graphicsDevice_->SetVertexShader(material->GetVertexShaderId());
	if(material->HasConstantBuffer())
	{
		graphicsDevice_->SetVertexShaderConstantBuffer(material->GetConstantBufferId());
	}
	graphicsDevice_->SetPixelShader(material->GetVertexShaderId());
	if(material->HasConstantBuffer())
	{
		graphicsDevice_->SetPixelShaderConstantBuffer(material->GetConstantBufferId());
	}
	if(material->HasDiffuseMap())
	{
		graphicsDevice_->SetSamplerState(material->GetDiffuseSamplerStateId());
	}

	// Make the draw call
	graphicsDevice_->SetVertexBuffer(renderCommand->GetVertexBufferId(), renderCommand->GetVertexStride());
	graphicsDevice_->SetIndexBuffer(renderCommand->GetIndexBufferId());
	if(material->HasDiffuseMap())
	{
		graphicsDevice_->SetTexture(renderCommand->GetDiffuseTextureId());
	}
	graphicsDevice_->DrawIndexed(renderCommand->GetNumIndices());
}

unsigned int DirectXRenderer::CreateDebugTextRenderCommand()
{
	unsigned int vertexDataSize = maxNumDebugTextRenderCommandVerts_ * sizeof(BlitVertexFormat);
	unsigned char* vertexData = new unsigned char[vertexDataSize];
	unsigned int vertexBufferId = graphicsDevice_->CreateVertexBuffer(vertexData, vertexDataSize);
	delete [] vertexData;

	unsigned int indexDataSize = maxNumDebugTextRenderCommandVerts_ * sizeof(unsigned int);
	unsigned char* indexData = new unsigned char[indexDataSize];
	unsigned int indexBufferId = graphicsDevice_->CreateIndexBuffer(indexData, indexDataSize);
	delete [] indexData;

	unsigned int newId = renderCommands_.size();
	RenderCommand* newRenderCommand = new RenderCommand(
		MaterialType::Blit, 0, Float4(), Float4(), Float4(), 0.0f, 0, vertexBufferId, indexBufferId, sizeof(BlitVertexFormat), 0);
	renderCommands_.insert(std::make_pair<unsigned int, std::unique_ptr<RenderCommand>>(newId, std::unique_ptr<RenderCommand>(newRenderCommand)));

	return newId;
}

unsigned int DirectXRenderer::LoadFont(const wchar_t* fontPathName)
{
	StreamReader fileReader = fileSystem_->OpenRead(fontPathName);

	char fileIdentifierBuffer[9];
	fileReader.ReadData(fileIdentifierBuffer, 8);
	fileIdentifierBuffer[8] = '\0';
	if(strcmp("RORNFONT", fileIdentifierBuffer) != 0)
	{
		throw font_load_exception("Attempt to load font with invalid file identifier, 'RORNFONT' was expected");
	}

	unsigned int versionNumber = fileReader.ReadUInt();
	if( versionNumber != 1)
	{
		throw model_load_exception("Attempt to load font with invalid version number, 1 was expected");
	}

	unsigned int numGlyphs = fileReader.ReadUInt();
	std::map<unsigned int, Glyph> glyphMap;
	for(unsigned int glyphIndex = 0; glyphIndex < numGlyphs ; ++glyphIndex)
	{
		unsigned int characterCode = fileReader.ReadUInt();
		unsigned int x = fileReader.ReadUInt();
		unsigned int y = fileReader.ReadUInt();
		unsigned int width = fileReader.ReadUInt();
		unsigned int height = fileReader.ReadUInt();
		float startU = fileReader.ReadFloat();
		float startV = fileReader.ReadFloat();
		float endU = fileReader.ReadFloat();
		float endV = fileReader.ReadFloat();

		glyphMap.insert(std::make_pair<unsigned int, Glyph>(characterCode, Glyph(x, y, width, height, startU, startV, endU, endV)));
	}
	
	unsigned int compiledTextureDataLength = fileReader.ReadUInt();
	unsigned char* compiledTextureData = new unsigned char[compiledTextureDataLength];
	fileReader.ReadData(compiledTextureData, compiledTextureDataLength);
	unsigned int deviceTextureId = graphicsDevice_->CreateTexture(compiledTextureData, compiledTextureDataLength);
	delete [] compiledTextureData;

	unsigned int newId = fonts_.size();
	Font* newFont = new Font(glyphMap, deviceTextureId);
	fonts_.insert(std::make_pair<unsigned int, std::unique_ptr<Font>>(newId, std::unique_ptr<Font>(newFont)));

	return newId;
}