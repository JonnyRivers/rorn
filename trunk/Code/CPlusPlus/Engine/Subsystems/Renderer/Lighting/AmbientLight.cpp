#pragma once

#include "AmbientLight.h"

using namespace Rorn::Engine;

AmbientLight::AmbientLight()
	: colour_(0.0f, 0.0f, 0.0f, 1.0f)
{
}

AmbientLight::AmbientLight(const Rorn::Maths::Float4& colour)
	: colour_(colour)
{
}

AmbientLight::~AmbientLight()
{
	
}

void AmbientLight::SetColour(const Rorn::Maths::Float4& colour)
{
	colour_ = colour;
}