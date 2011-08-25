#pragma once

#include "MainLight.h"

using namespace Rorn::Engine;

MainLight::MainLight()
	: direction_(0.0f, -1.0f, 0.0f), colour_(0.0f, 0.0f, 0.0f, 1.0f)
{
}

MainLight::MainLight(const Rorn::Maths::Vector3& direction, const Rorn::Maths::Float4& colour)
	: direction_(direction), colour_(colour)
{
}

MainLight::~MainLight()
{
	
}

void MainLight::SetDirection(const Rorn::Maths::Vector3& direction)
{
	direction_ = direction;
}

void MainLight::SetColour(const Rorn::Maths::Float4& colour)
{
	colour_ = colour;
}