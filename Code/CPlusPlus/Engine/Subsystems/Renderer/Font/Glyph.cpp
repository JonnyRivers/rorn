#pragma once

#include "Glyph.h"

using namespace Rorn::Engine;

Glyph::Glyph(unsigned int x, 
			 unsigned int y,
			 unsigned int width,
			 unsigned int height,
			 float startU,
			 float startV,
			 float endU,
			 float endV)
			 : x_(x),
			   y_(y),
			   width_(width),
			   height_(height),
			   startU_(startU),
			   startV_(startV),
			   endU_(endU),
			   endV_(endV)
{
}

Glyph::~Glyph()
{
}