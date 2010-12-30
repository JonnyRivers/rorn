#include "UntexturedRenderCommand.h"

using namespace Rorn::Engine;

UntexturedRenderCommand::UntexturedRenderCommand(void)
	: RenderCommand(NULL/*UntexturedSurfaceFormat singleton*/)
{
}


UntexturedRenderCommand::~UntexturedRenderCommand(void)
{
}

void UntexturedRenderCommand::LoadFromFile(FileReader& fileReader)
{
	int primitveTypeValue = fileReader.ReadInt();
	RenderCommand::PrimitiveType primitiveType = static_cast<RenderCommand::PrimitiveType>(primitveTypeValue);

	float ambientR = fileReader.ReadFloat();
	float ambientG = fileReader.ReadFloat();
	float ambientB = fileReader.ReadFloat();
	float diffuseR = fileReader.ReadFloat();
	float diffuseG = fileReader.ReadFloat();
	float diffuseB = fileReader.ReadFloat();
	float specularR = fileReader.ReadFloat();
	float specularG = fileReader.ReadFloat();
	float specularB = fileReader.ReadFloat();

	int numVertices = fileReader.ReadInt();
	for(int vertexIndex = 0 ; vertexIndex != numVertices ; ++vertexIndex)
	{
		float posX = fileReader.ReadFloat();
		float posY = fileReader.ReadFloat();
		float posZ = fileReader.ReadFloat();
		float norX = fileReader.ReadFloat();
		float norY = fileReader.ReadFloat();
		float norZ = fileReader.ReadFloat();
	}

	int numIndicies = fileReader.ReadInt();
	for(int indexIndex = 0 ; indexIndex != numIndicies ; ++indexIndex)
	{
		int index = fileReader.ReadInt();
	}
}