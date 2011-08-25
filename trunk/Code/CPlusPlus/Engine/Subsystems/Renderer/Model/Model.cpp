#pragma once


#include "Model.h"

using namespace Rorn::Engine;

Model::Model(const Rorn::Maths::BoundingBox& boundingBox, const std::vector<unsigned int>& texturesCreated, const std::vector<unsigned int>& renderCommands)
	: boundingBox_(boundingBox), texturesCreated_(texturesCreated), renderCommands_(renderCommands)
{
}

Model::~Model()
{
	
}

/*virtual*/ const Rorn::Maths::BoundingBox& Model::GetBoundingBox()
{
	return boundingBox_;
}