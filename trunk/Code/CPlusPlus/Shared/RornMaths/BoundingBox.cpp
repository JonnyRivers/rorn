#include "BoundingBox.h"

#include <algorithm>

using namespace Rorn::Maths;

BoundingBox::BoundingBox()
	: minimum_(0.0f, 0.0f, 0.0f), maximum_(0.0f, 0.0f, 0.0f)
{
}

BoundingBox::BoundingBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
	: minimum_(minX, minY, minZ), maximum_(maxX, maxY, maxZ)
{
}

float BoundingBox::GetBoundingRadius() const
{
	float sizeX = maximum_.X - minimum_.X;
	float sizeY = maximum_.Y - minimum_.Y;
	float sizeZ = maximum_.Z - minimum_.Z;

	// just return half the biggest dimension
	return (std::max(sizeX, std::max(sizeY, sizeZ) ) * 0.5f);
}

Position BoundingBox::GetMinimum() const
{
	return minimum_;
}

Position BoundingBox::GetMaximum() const
{
	return maximum_;
}

Position BoundingBox::GetCentre() const
{
	return Position(
		minimum_.X + ((maximum_.X - minimum_.X) / 2.0f),
		minimum_.Y + ((maximum_.Y - minimum_.Y) / 2.0f),
		minimum_.Z + ((maximum_.Z - minimum_.Z) / 2.0f));
}
