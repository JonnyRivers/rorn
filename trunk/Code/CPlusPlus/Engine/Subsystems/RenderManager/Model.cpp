#include "Model.h"

#include "../FileManager/FileReader.h"
#include "RenderCommand.h"
#include "UntexturedRenderCommand.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

Model::Model(void)
{
}

Model::~Model(void)
{
}

void Model::LoadFromFile(const wchar_t* modelPathName, ID3D11Device* device)
{
	FileReader fileReader(modelPathName);

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
	int numRenderCommands = fileReader.ReadInt();

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
	}
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