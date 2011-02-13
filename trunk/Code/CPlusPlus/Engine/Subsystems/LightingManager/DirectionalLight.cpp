#include "DirectionalLight.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

DirectionalLight::DirectionalLight(const Vector3& direction, const Float4& color)
	: Direction(direction), Color(color)
{
}

