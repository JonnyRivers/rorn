#include "HResultFormatter.h"

#include <sstream>

using namespace Rorn::ErrorCodes;
		
std::wstring HResultFormatter::FormatHResult(HRESULT hr)
{
	LPTSTR formattedHRText = NULL;

	::FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPTSTR>(&formattedHRText),
		0,
		NULL);

	std::wstring result;

	if( formattedHRText != NULL )
	{
		result = formattedHRText;
		::LocalFree(formattedHRText);
	}

	return result;
}