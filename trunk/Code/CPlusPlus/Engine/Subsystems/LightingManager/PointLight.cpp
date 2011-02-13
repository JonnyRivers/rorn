#include "PointLight.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

PointLight::PointLight(const Vector3& position, const Float4& color, float luminosity)
	: Position(position), Color(color), Luminosity(luminosity)
{
}

