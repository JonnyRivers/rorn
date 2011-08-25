#pragma once

#include "Font.h"

using namespace Rorn::Engine;

Font::Font(const std::map<unsigned int, Glyph>& glyphMap, unsigned int textureId)
	: glyphMap_(glyphMap), textureId_(textureId)
{
}

Font::~Font()
{
}

const Glyph& Font::GetGlyph(unsigned int characterCode) const
{
	return glyphMap_.find(characterCode)->second;
}
