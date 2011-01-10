#include "Light.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

Light::Light(const Vector3& direction, const Float4& color)
	: Direction(direction), Color(color)
{
}

