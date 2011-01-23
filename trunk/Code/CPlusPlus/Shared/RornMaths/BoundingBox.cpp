#include "BoundingBox.h"

using namespace Rorn::Maths;

BoundingBox::BoundingBox()
	: minimum_(0.0f, 0.0f, 0.0f), maximum_(0.0f, 0.0f, 0.0f)
{
}

BoundingBox::BoundingBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
	: minimum_(minX, minY, minZ), maximum_(maxX, maxY, maxZ)
{
}

Vector3 BoundingBox::GetMinimum() const
{
	return minimum_;
}

Vector3 BoundingBox::GetMaximum() const
{
	return maximum_;
}

Vector3 BoundingBox::GetCentre() const
{
	return Vector3(
		minimum_.X + ((maximum_.X - minimum_.X) / 2.0f),
		minimum_.Y + ((maximum_.Y - minimum_.Y) / 2.0f),
		minimum_.Z + ((maximum_.Z - minimum_.Z) / 2.0f));
}
