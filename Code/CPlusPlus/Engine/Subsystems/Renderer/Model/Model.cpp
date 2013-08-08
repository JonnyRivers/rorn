#pragma once


#include "Model.h"

using namespace Rorn::Engine;

Model::Model(const Rorn::Maths::BoundingBox& boundingBox, const std::vector<unsigned int>& texturesCreated, const std::vector<unsigned int>& renderCommands, unsigned int boundsId)
	: boundingBox_(boundingBox), texturesCreated_(texturesCreated), renderCommands_(renderCommands), boundsId_(boundsId)
{
}

Model::~Model()
{
	
}

/*virtual*/ const Rorn::Maths::BoundingBox& Model::GetBoundingBox() const
{
	return boundingBox_;
}

/*virtual*/ unsigned int Model::GetBoundsId() const
{
	return boundsId_;
}