#include "Path.h"

Rorn::Text::ci_string Rorn::Text::Path::GetDirectoryFromPathName(const Rorn::Text::ci_string& pathName)
{
	Rorn::Text::ci_string::size_type lastBackSlashPos = pathName.find_last_of('\\');
	if(lastBackSlashPos != Rorn::Text::ci_string::npos)
		return pathName.substr(0, lastBackSlashPos);

	Rorn::Text::ci_string::size_type lastForwardSlashPos = pathName.find_last_of('/');
	if(lastForwardSlashPos != std::string::npos)
		return pathName.substr(0, lastForwardSlashPos);

	// The pathname is not a pathname.  Could throw here.
	return Rorn::Text::ci_string();
}

Rorn::Text::ci_string Rorn::Text::Path::GetFileNameFromPathName(const Rorn::Text::ci_string& pathName)
{
	Rorn::Text::ci_string::size_type lastBackSlashPos = pathName.find_last_of('\\');
	if(lastBackSlashPos != Rorn::Text::ci_string::npos)
		return pathName.substr(lastBackSlashPos + 1, pathName.size() - lastBackSlashPos);

	Rorn::Text::ci_string::size_type lastForwardSlashPos = pathName.find_last_of('/');
	if(lastForwardSlashPos != Rorn::Text::ci_string::npos)
		return pathName.substr(lastForwardSlashPos + 1, pathName.size() - lastBackSlashPos);

	// The pathname is not a pathname.  Could throw here.
	return Rorn::Text::ci_string();
}

Rorn::Text::ci_string Rorn::Text::Path::ChangeExtension(const Rorn::Text::ci_string& pathName, const Rorn::Text::ci_string& extension)
{
	Rorn::Text::ci_string directory = GetDirectoryFromPathName(pathName);
	Rorn::Text::ci_string existingFilename = GetFileNameFromPathName(pathName);

	Rorn::Text::ci_string::size_type firstDotPos = existingFilename.find_first_of('.');
	Rorn::Text::ci_string newFilename;
	if(firstDotPos != Rorn::Text::ci_string::npos)
	{
		newFilename = existingFilename.substr(0, firstDotPos) + "." + extension;
	}
	else
	{
		newFilename = existingFilename + "." + extension;
	}

	Rorn::Text::ci_string result = directory + "\\" + newFilename;
	return result;
}