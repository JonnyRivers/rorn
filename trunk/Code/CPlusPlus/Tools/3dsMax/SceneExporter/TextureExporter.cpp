#include "TextureExporter.h"

#include <sstream>

#include <Windows.h>

#include "../../../Shared/Text/Path.h"

std::string TextureExporter::RegisterTexture(const char* rawSourcePathName)
{
	Rorn::Text::ci_string sourcePathName(rawSourcePathName);
	std::map<Rorn::Text::ci_string, std::string>::const_iterator pathNameIter = 
		sourceTargetPathNameMap_.find(sourcePathName);
	if(pathNameIter == sourceTargetPathNameMap_.end())
	{
		std::string targetFileName = Rorn::Text::Path::GetFileNameFromPathName(rawSourcePathName);
		sourceTargetPathNameMap_.insert(
			std::make_pair<Rorn::Text::ci_string, std::string>(sourcePathName, targetFileName));

		return targetFileName;
	}
	else
	{
		return pathNameIter->second;
	}
}

void TextureExporter::CopyTexturesToDirectory(const char* directory) const
{
	std::map<Rorn::Text::ci_string, std::string>::const_iterator pathNameIter;
	for(pathNameIter = sourceTargetPathNameMap_.begin() ; pathNameIter != sourceTargetPathNameMap_.end() ; ++pathNameIter)
	{
		LPCSTR sourcePathName = pathNameIter->first.c_str();

		std::stringstream targetPathNameStream;
		targetPathNameStream << directory << '\\' << pathNameIter->second.c_str();

		// Need to handle failure here.  This is export failure.
		::CopyFile(sourcePathName, targetPathNameStream.str().c_str(), FALSE);
	}
}

void TextureExporter::Reset()
{
	sourceTargetPathNameMap_.clear();
}
