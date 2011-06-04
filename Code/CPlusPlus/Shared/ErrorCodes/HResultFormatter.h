#pragma once

#include <string>

#include <Windows.h>

namespace Rorn
{
	namespace ErrorCodes
	{
		class HResultFormatter
		{
		public:
			static std::wstring FormatHResult(HRESULT hr);
		};
	}
}