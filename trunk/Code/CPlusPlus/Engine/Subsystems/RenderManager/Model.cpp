#include "Model.h"

#include "../FileManager/FileReader.h"
#include "RenderCommand.h"
#include "UntexturedRenderCommand.h"

using namespace Rorn::Engine;

Model::Model(void)
{
}

Model::~Model(void)
{
}

void Model::LoadFromFile(LPCTSTR modelPathName, ID3D11Device* device)
{
	FileReader fileReader(modelPathName);

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

void Model::Draw(ID3D11DeviceContext* deviceContext, CXMMATRIX instanceToWorldMatrix, CXMMATRIX worldToProjectionMatrix) const
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