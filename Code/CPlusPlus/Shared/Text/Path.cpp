#include "Path.h"

std::string Rorn::Text::Path::GetDirectoryFromPathName(const std::string& pathName)
{
	std::string::size_type lastBackSlashPos = pathName.find_last_of('\\');
	if(lastBackSlashPos != std::string::npos)
		return pathName.substr(0, lastBackSlashPos);

	std::string::size_type lastForwardSlashPos = pathName.find_last_of('/');
	if(lastForwardSlashPos != std::string::npos)
		return pathName.substr(0, lastForwardSlashPos);

	// The pathname is not a pathname.  Could throw here.
	return std::string();
}

std::string Rorn::Text::Path::GetFileNameFromPathName(const std::string& pathName)
{
	std::string::size_type lastBackSlashPos = pathName.find_last_of('\\');
	if(lastBackSlashPos != std::string::npos)
		return pathName.substr(lastBackSlashPos + 1, pathName.size() - lastBackSlashPos);

	std::string::size_type lastForwardSlashPos = pathName.find_last_of('/');
	if(lastForwardSlashPos != std::string::npos)
		return pathName.substr(lastForwardSlashPos + 1, pathName.size() - lastBackSlashPos);

	// The pathname is not a pathname.  Could throw here.
	return std::string();
}