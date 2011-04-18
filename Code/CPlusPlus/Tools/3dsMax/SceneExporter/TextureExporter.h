#pragma once

#include <map>
#include <set>
#include <string>

#include "../../../Shared/Text/ci_string.h"

class TextureExporter
{
public:
	// Returns the target file name (placed alongside the XML itself)
	Rorn::Text::ci_string RegisterTexture(const char* pathName);

	// Copies all registered textures to the specified directory
	void CopyTexturesToDirectory(const char* directory) const;

	void Reset();
private:
	std::map<Rorn::Text::ci_string, Rorn::Text::ci_string> sourceTargetPathNameMap_;
};

