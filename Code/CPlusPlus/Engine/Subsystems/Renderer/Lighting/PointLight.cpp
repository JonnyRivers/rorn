#pragma once

#include "PointLight.h"

using namespace Rorn::Engine;

PointLight::PointLight(const Rorn::Maths::Position& position, const Rorn::Maths::Float4& colour, float luminosity)
	: position_(position), colour_(colour), luminosity_(luminosity)
{
}

PointLight::~PointLight()
{
}
