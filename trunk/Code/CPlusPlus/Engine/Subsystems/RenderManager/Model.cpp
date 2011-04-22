#include "Model.h"

#include <map>

#include "../TextureManager/TextureManager.h"
#include "../FileManager/FileReader.h"

#include "RenderCommand.h"
#include "UntexturedRenderCommand.h"
#include "DiffuseOnlyRenderCommand.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

Model::Model(void)
{
}

Model::~Model(void)
{
}

bool Model::LoadFromFile(const wchar_t* modelPathName, ID3D11Device* device)
{
	FileReader fileReader(modelPathName);

	if( !fileReader.IsFileHandleValid() )
		return false;

	// TODO validate version and file identifier
	char fileIdentifierBuffer[9];
	fileReader.ReadData(fileIdentifierBuffer, 8);
	fileIdentifierBuffer[8] = '\0';
	int versionNumber = fileReader.ReadInt();
	float bbMinX = fileReader.ReadFloat();
	float bbMinY = fileReader.ReadFloat();
	float bbMinZ = fileReader.ReadFloat();
	float bbMaxX = fileReader.ReadFloat();
	float bbMaxY = fileReader.ReadFloat();
	float bbMaxZ = fileReader.ReadFloat();
	boundingBox_ = Maths::BoundingBox(bbMinX, bbMinY, bbMinZ, bbMaxX, bbMaxY, bbMaxZ);
	int numCompiledTextures = fileReader.ReadInt();
	int numRenderCommands = fileReader.ReadInt();

	std::map<int, int> textureIdMap;// map from this model's texture index to the engine's texture id

	for(int compiledTextureIndex = 0 ; compiledTextureIndex != numCompiledTextures ; ++compiledTextureIndex)
	{
		int textureId = fileReader.ReadInt();
		int compiledTextureDataLength = fileReader.ReadInt();
		unsigned char* compiledTextureData = new unsigned char[compiledTextureDataLength];
		fileReader.ReadData(compiledTextureData, compiledTextureDataLength);
		textureIdMap[compiledTextureIndex] = 
			TextureManager::GetInstance().CreateTexture(compiledTextureData, compiledTextureDataLength);
		delete [] compiledTextureData;
	}

	for(int renderCommandIndex = 0 ; renderCommandIndex != numRenderCommands ; ++renderCommandIndex)
	{
		int surfaceFormat = fileReader.ReadInt();
		SurfaceFormatType surfaceFormatType = static_cast<SurfaceFormatType>(surfaceFormat);

		if(surfaceFormatType == Untextured)
		{
			UntexturedRenderCommand* newRenderCommand = new UntexturedRenderCommand();
			newRenderCommand->LoadFromFile(fileReader, device);
			renderCommands_.push_back(std::unique_ptr<RenderCommand>(newRenderCommand));
		}
		else if(surfaceFormatType == DiffuseOnly)
		{
			DiffuseOnlyRenderCommand* newRenderCommand = new DiffuseOnlyRenderCommand();
			newRenderCommand->LoadFromFile(fileReader, device, textureIdMap);
			renderCommands_.push_back(std::unique_ptr<RenderCommand>(newRenderCommand));
		}
	}

	return true;
}

const BoundingBox& Model::GetBoundingBox() const
{
	return boundingBox_;
}

void Model::Draw(ID3D11DeviceContext* deviceContext, 
	const Matrix4x4& instanceToWorldMatrix, const Matrix4x4& worldToProjectionMatrix) const
{
	std::list<std::unique_ptr<RenderCommand>>::const_iterator renderCommandIter;
	for(renderCommandIter = renderCommands_.cbegin() ; renderCommandIter != renderCommands_.cend() ; ++renderCommandIter)
		(*renderCommandIter)->Draw(deviceContext, instanceToWorldMatrix, worldToProjectionMatrix);
}

void Model::Release()
{
	std::list<std::unique_ptr<RenderCommand>>::iterator renderCommandIter;
	for(renderCommandIter = renderCommands_.begin() ; renderCommandIter != renderCommands_.end() ; ++renderCommandIter)
		(*renderCommandIter)->Release();
}