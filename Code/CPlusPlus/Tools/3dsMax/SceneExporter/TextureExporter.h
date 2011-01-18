#pragma once

#include <map>
#include <set>
#include <string>

#include "../../../Shared/Text/ci_string.h"

class TextureExporter
{
public:
	// Returns the target file name (placed alongside the XML itself)
	std::string RegisterTexture(const char* pathName);

	// Copies all registered textures to the specified directory
	void CopyTexturesToDirectory(const char* directory) const;

	void Reset();
private:
	std::map<Rorn::Text::ci_string, std::string> sourceTargetPathNameMap_;
};

