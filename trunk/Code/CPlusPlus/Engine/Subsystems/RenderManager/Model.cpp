#include "Model.h"

#include "RenderCommand.h"

using namespace Rorn::Engine;

Model::Model(void)
{
}

Model::~Model(void)
{
}

void Model::Draw(ID3D11DeviceContext* deviceContext, CXMMATRIX instanceToWorldMatrix, CXMMATRIX worldToProjectionMatrix) const
{
	std::list<RenderCommand>::const_iterator renderCommandIter;
	for(renderCommandIter = renderCommands_.cbegin() ; renderCommandIter != renderCommands_.cend() ; ++renderCommandIter)
		renderCommandIter->Draw(deviceContext, instanceToWorldMatrix, worldToProjectionMatrix);
}