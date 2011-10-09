#pragma once

#include "PointLight.h"

using namespace Rorn::Maths;
using namespace Rorn::Engine;

PointLight::PointLight(const Vector4& position, const Float4& colour, float luminosity)
	: position_(position), colour_(colour), luminosity_(luminosity)
{
}

PointLight::~PointLight()
{
}
